// Gamut compression constants
const float smoothCusps = 0.24;
const float smoothJ = 0.058;
const float smoothM = 0.188;
const float cuspMidBlend = 1.3;

const float focusGainBlend = 0.2;
const float focusAdjustGain = 0.55;
const float focusDistance = 1.35;
const float focusDistanceScaling = 1.75;


struct ODTParams {
        float peakLuminance;
        float referenceLuminance;
        
        // Tonescale                // Set via TSParams structure
        float n_r;                  // normalized white in nits (what 1.0 should be)
        float g;                    // surround / contrast
        float t_1;                  // shadow toe or flare/glare compensation
        float c_t;                  
        float s_2;
        float u_2;
        float m_2;
        
        // Chroma Compression
        float limitJmax;
        float midJ;
        float model_gamma;
        float sat;
        float sat_thr;
        float compr;
            
        // Limit
        float LIMIT_RGB_TO_XYZ[3][3];
        float LIMIT_XYZ_TO_RGB[3][3];
        float XYZ_w_limit[3];

        // Output
        float OUTPUT_RGB_TO_XYZ[3][3];
        float OUTPUT_XYZ_TO_RGB[3][3];
        float XYZ_w_output[3];
        
        float lowerHullGamma;
};

// TABLE FUNCTIONS
float wrap_to_360( float hue) {
	float y = fmod( hue, 360.);
	if ( y < 0.)
	{
		y = y + 360.;
	}
	return y;
}

int hue_position_in_uniform_table( float hue, int table_size) {
    const float wrapped_hue = wrap_to_360( hue);
    int result = (wrapped_hue / 360. * table_size);
    return result;
}

int next_position_in_table( int entry, int table_size) {
    int result = (entry + 1) % table_size;
    return result;
}

float base_hue_for_position(int i_lo, int table_size) {
    float result = i_lo * 360. / table_size;
    return result;
}

const Chromaticities AP0 = // ACES Primaries from SMPTE ST2065-1
{
  { 0.73470,  0.26530},
  { 0.00000,  1.00000},
  { 0.00010, -0.07700},
  { 0.32168,  0.33767}
};

const Chromaticities AP1 = // Working space and rendering primaries for ACES 1.0
{
  { 0.713,    0.293},
  { 0.165,    0.830},
  { 0.128,    0.044},
  { 0.32168,  0.33767}
};





float[3] post_adaptation_non_linear_response_compression_forward( float RGB[3], float F_L)
{
    float F_L_RGB[3];
    F_L_RGB[0] = spow(F_L * fabs(RGB[0]) / 100., 0.42);
    F_L_RGB[1] = spow(F_L * fabs(RGB[1]) / 100., 0.42);
    F_L_RGB[2] = spow(F_L * fabs(RGB[2]) / 100., 0.42);
    
    float RGB_c[3];
    RGB_c[0] = (400. * copysign(1., RGB[0]) * F_L_RGB[0]) / (27.13 + F_L_RGB[0]);
    RGB_c[1] = (400. * copysign(1., RGB[1]) * F_L_RGB[1]) / (27.13 + F_L_RGB[1]);
    RGB_c[2] = (400. * copysign(1., RGB[2]) * F_L_RGB[2]) / (27.13 + F_L_RGB[2]);

    return RGB_c;
}

float[3] post_adaptation_non_linear_response_compression_inverse( float RGB[3], float F_L)
{
    float RGB_p[3];
    RGB_p[0] = (sign(RGB[0]) * 100. / F_L * spow((27.13 * fabs(RGB[0])) / (400. - fabs(RGB[0])), 1.0 / 0.42));
    RGB_p[1] = (sign(RGB[1]) * 100. / F_L * spow((27.13 * fabs(RGB[1])) / (400. - fabs(RGB[1])), 1.0 / 0.42));
    RGB_p[2] = (sign(RGB[2]) * 100. / F_L * spow((27.13 * fabs(RGB[2])) / (400. - fabs(RGB[2])), 1.0 / 0.42));

    return RGB_p;
}


// CAM Parameters
const float L_A = 100.;
const float Y_b = 20.;

const float ac_resp = 1.0;
const float ra = 2. * ac_resp;
const float ba = 0.05 + (2. - ra);

const float surround[3] = { 0.9, 0.59, 0.9 };

const Chromaticities CAM16_PRI = 
    {
      { 0.8336,  0.1735 },
      { 2.3854, -1.4659 },
      { 0.13  , -0.14   },
      { 0.333 ,  0.333  }
    };

const float MATRIX_16[3][3] = XYZtoRGB_f33( CAM16_PRI, 1.0);


float[3][3] generate_panlrcm( float ra = 2.,
                              float ba = 0.05 )
{
	float panlrcm_data[3][3] =
	{
        { ra,   1.    ,  1./9.},
        { 1., -12./11.,  1./9.},
        { ba,   1./11., -2./9.}
	};
	float panlrcm[3][3] = invert_f33( panlrcm_data);

	// Normalize columns so that first row is 460
	for (int i = 0; i < 3; i = i + 1) {
	  float n = 460.0 / panlrcm[0][i];
	  panlrcm[0][i] = panlrcm[0][i] * n;
	  panlrcm[1][i] = panlrcm[1][i] * n;
	  panlrcm[2][i] = panlrcm[2][i] * n;
	}

	return panlrcm;
}
      
const float panlrcm[3][3] = generate_panlrcm( ra, ba);   // Matrix for Hellwig inverse


float[3] XYZ_to_Hellwig2022_JMh( float XYZ[3], 
                                 float XYZ_w[3] )
{
    float Y_w = XYZ_w[1];

    // Step 0 - Converting CIE XYZ tristimulus values to sharpened RGB values
    float RGB_w[3] = mult_f3_f33( XYZ_w, MATRIX_16);
     
    // Ignore degree of adaptation.
    float D = 1.0;
    
    // Viewing condition dependent parameters
    float k = 1. / (5. * L_A + 1.);
    float k4 = pow( k, 4.);
    float F_L = 0.2 * k4 * (5. * L_A) + 0.1 * pow( (1.-k4), 2.) * spow( 5. * L_A, 1./3.);
    float n = Y_b / Y_w;
    float z = 1.48 + sqrt(n);

    float D_RGB[3];
    D_RGB[0] = D * Y_w / RGB_w[0] + 1. - D;
    D_RGB[1] = D * Y_w / RGB_w[1] + 1. - D;
    D_RGB[2] = D * Y_w / RGB_w[2] + 1. - D;
    
    float RGB_wc[3];
    RGB_wc[0] = D_RGB[0] * RGB_w[0];
    RGB_wc[1] = D_RGB[1] * RGB_w[1];
    RGB_wc[2] = D_RGB[2] * RGB_w[2];

    float RGB_aw[3] = post_adaptation_non_linear_response_compression_forward( RGB_wc, F_L);    
    float A_w = ra * RGB_aw[0] + RGB_aw[1] + ba * RGB_aw[2];
    
    // Step 1 - Converting CIE XYZ tristimulus values to sharpened RGB values
    float RGB[3] = mult_f3_f33( XYZ, MATRIX_16);

    // Step 2
    float RGB_c[3];
    RGB_c[0] = D_RGB[0] * RGB[0];
    RGB_c[1] = D_RGB[1] * RGB[1];
    RGB_c[2] = D_RGB[2] * RGB[2];
    
    // Step 3 - apply  forward post-adaptation non-linear response compression
    float RGB_a[3] = post_adaptation_non_linear_response_compression_forward( RGB_c, F_L);

    // Step 4 - Converting to preliminary cartesian  coordinates
    float a = RGB_a[0] - 12. * RGB_a[1] / 11. + RGB_a[2] / 11.;
    float b = (RGB_a[0] + RGB_a[1] - 2. * RGB_a[2]) / 9.;

    // Computing the hue angle math h
    // Unclear why this isn't matching the python version
    float hr = atan2(b,a);
    float h = wrap_to_360( radians_to_degrees(hr) );
    
    // Step 6 - Computing achromatic responses for the stimulus
    float A = ra * RGB_a[0] + RGB_a[1] + ba * RGB_a[2];

    // Step 7 - Computing the correlate of lightness, J
    float J = 100. * pow( A / A_w, surround[1] * z);
    
    // Step 9 - Computing the correlate of colourfulness, M
    float M = 43. * surround[2] * sqrt(a * a + b * b);
    
    if (J == 0.0)
        M = 0.0;

    float return_JMh[3] = {J, M, h};
    return return_JMh;
}


float[3] Hellwig2022_JMh_to_XYZ( float JMh[3], 
                                 float XYZ_w[3] ) 
{
    float J = JMh[0];
    float M = JMh[1];
    float h = JMh[2];
    
    float Y_w = XYZ_w[1];
    
    // Step 0 - Converting CIE XYZ tristimulus values to sharpened RGB values.
    float RGB_w[3] = mult_f3_f33( XYZ_w, MATRIX_16);

    // Ignore degree of adaptation.
    float D = 1.0;
    
    // Viewing condition dependent parameters
    float k = 1.0 / (5.0 * L_A + 1.0);
    float k4 = pow( k, 4.0);
    float F_L = 0.2 * k4 * (5.0 * L_A) + 0.1 * pow( (1.0-k4), 2.0) * spow( 5.0 * L_A, 1.0/3.0);
    float n = sdiv( Y_b, Y_w);
    float z = 1.48 + sqrt(n);

    float D_RGB[3];
    D_RGB[0] = D * Y_w / RGB_w[0] + 1.0 - D;
    D_RGB[1] = D * Y_w / RGB_w[1] + 1.0 - D;
    D_RGB[2] = D * Y_w / RGB_w[2] + 1.0 - D;
    
    float RGB_wc[3];
    RGB_wc[0] = D_RGB[0] * RGB_w[0];
    RGB_wc[1] = D_RGB[1] * RGB_w[1];
    RGB_wc[2] = D_RGB[2] * RGB_w[2];

    float RGB_aw[3] = post_adaptation_non_linear_response_compression_forward( RGB_wc, F_L);
    
    float A_w = ra * RGB_aw[0] + RGB_aw[1] + ba * RGB_aw[2];

    float hr = degrees_to_radians(h);
    
    // No Helmholtz-Kohlrausch effect
    
    // Computing achromatic respons A for the stimulus
    float A = A_w * spow(J/100.0, 1.0/ (surround[1] * z));
        
    // Computing P_p_1 to P_p_2
    float P_p_1 = 43.0 * surround[2];
    float P_p_2 = A;
    
    // Step 3 - Computing opponent colour dimensions a and b
    float gamma = M / P_p_1;
    float a = gamma * cos(hr);
    float b = gamma * sin(hr);
    
    // Step 4 - Applying post-adaptation non-linear response compression matrix
    float vec[3] = {P_p_2, a, b};
    float RGB_a[3] = mult_f_f3( 1.0/1403.0, mult_f3_f33( vec, panlrcm));

    // Step 5 - Applying the inverse post-adaptation non-linear respnose compression
    float RGB_c[3] = post_adaptation_non_linear_response_compression_inverse( RGB_a, F_L);

    // Step 6
    float RGB[3];
    RGB[0] = RGB_c[0] / D_RGB[0];
    RGB[1] = RGB_c[1] / D_RGB[1];
    RGB[2] = RGB_c[2] / D_RGB[2];
    
    // Step 7
    float XYZ[3] = mult_f3_f33( RGB, invert_f33( MATRIX_16) );
    
    return XYZ;
}


float Hellwig_J_to_Y(float J, float surround = 0.59, float L_A = 100., float Y_b = 20.)
{
    // Viewing conditions dependent parameters (could be pre-calculated)
    float k     = 1.0 / (5. * L_A + 1.);
    float k4    = k*k*k*k;
    float F_L   = 0.2 * k4 * (5. * L_A) + 0.1 * pow((1. - k4), 2.) * pow(5. * L_A, 1. / 3.) ;
    float n     = Y_b / 100.;
    float z     = 1.48 + sqrt(n);
    float F_L_W = pow(F_L, 0.42);
    float A_w   = (400. * F_L_W) / (27.13 + F_L_W);

    float A = A_w * pow(fabs(J) / 100., 1. / (surround * z));

    return sign(J) * 100. / F_L * pow((27.13 * A) / (400.0 - A), 1. / 0.42);
}

float Y_to_Hellwig_J( float Y, float surround = 0.59, float L_A = 100., float Y_b = 20.)
{
    // Viewing conditions dependent parameters (could be pre-calculated)
    float k     = 1.0 / (5. * L_A + 1.);
    float k4    = k*k*k*k;
    float F_L   = 0.2 * k4 * (5. * L_A) + 0.1 * pow((1. - k4), 2.) * pow(5. * L_A, 1. / 3.) ;
    float n     = Y_b / 100.;
    float z     = 1.48 + sqrt(n);
    float F_L_W = pow(F_L, 0.42);
    float A_w   = (400. * F_L_W) / (27.13 + F_L_W);

    float F_L_Y = pow(F_L * fabs(Y) / 100., 0.42);

    return sign(Y) * 100. * pow(((400. * F_L_Y) / (27.13 + F_L_Y)) / A_w, surround * z);
}

float[3] clamp_XYZ_to_AP1( float XYZ[3], 
                           float XYZ_to_AP1_matrix[4][4] ) 
{
    float ap1[3] = mult_f3_f44( XYZ, XYZ_to_AP1_matrix);

    float ap1_clamped[3] = clamp_f3( ap1, 0., HALF_POS_INF);
    float XYZ_clamped[3] = mult_f3_f44( ap1_clamped, invert_f44( XYZ_to_AP1_matrix));

    return XYZ_clamped;
}

float[3] aces_to_JMh( float aces[3], 
                      float peakLuminance ) 
{
    const float ACES_TO_XYZ_M[3][3] = RGBtoXYZ_f33( AP0, 1.0 );

    // AP0 to XYZ
    float XYZ[3] = mult_f3_f33( aces, ACES_TO_XYZ_M );

    // Clamp to half float range
    const float AP1_XYZ_TO_RGB[4][4] = XYZtoRGB( AP1, 1.0);
    XYZ = clamp_XYZ_to_AP1( XYZ, AP1_XYZ_TO_RGB );

    // XYZ to JMh
    float RGB_w[3] = {100., 100., 100.};
    float XYZ_w[3] = mult_f3_f33( RGB_w, ACES_TO_XYZ_M );

    float XYZluminance[3] = mult_f_f3( 100., XYZ);
    float JMh[3] = XYZ_to_Hellwig2022_JMh( XYZluminance, XYZ_w );
    
    return JMh;
}

float[3] JMh_to_aces( float JMh[3], 
                      float peakLuminance ) 
{
    const float XYZ_TO_ACES_M[3][3] = XYZtoRGB_f33( AP0, 1.0 );
    const float RGB_w[3] = {100., 100., 100.};
    float XYZ_w_aces[3] = mult_f3_f33( RGB_w, invert_f33( XYZ_TO_ACES_M) );

    // JMh to XYZ
    float XYZluminance[3] = Hellwig2022_JMh_to_XYZ( JMh, XYZ_w_aces );

    float XYZ[3] = mult_f_f3( 1/100., XYZluminance);
    
    // XYZ to ACES
    float ACES[3] = mult_f3_f33( XYZ, XYZ_TO_ACES_M );
    
    return ACES;
}

float[3] JMh_to_output_XYZ( float JMh[3],
                            ODTParams PARAMS )
{
//     float RGB_w[3] = {100., 100., 100.};
//     float XYZ_w[3] = mult_f3_f33( RGB_w, PARAMS.OUTPUT_RGB_TO_XYZ );

    float XYZluminance[3];
    if (PARAMS.XYZ_w_limit[0] == PARAMS.XYZ_w_output[0]) { // use outputWhite as displayWhite
        XYZluminance = Hellwig2022_JMh_to_XYZ( JMh, 
                                               PARAMS.XYZ_w_output );
    } else {  // "creative white" so use limitWhite as displayWhite
        XYZluminance = Hellwig2022_JMh_to_XYZ( JMh, 
                                               PARAMS.XYZ_w_limit );
        float RGB_w[3] = mult_f3_f33( PARAMS.XYZ_w_limit, PARAMS.OUTPUT_XYZ_TO_RGB);
        float RGB_w_f[3] = mult_f_f3( 1/100., RGB_w);
        float largestChannel = max( max(RGB_w_f[0], RGB_w_f[1]), RGB_w_f[2]);
        XYZluminance = mult_f_f3( 1./largestChannel, XYZluminance);
    }

    float XYZ[3] = mult_f_f3( 1/100., XYZluminance);
    
    return XYZ;
}


float[3] XYZ_output_to_JMh( float XYZ[3], 
                            ODTParams PARAMS )
{
    float XYZluminance[3] = mult_f_f3( 100., XYZ);

    float JMh[3];
    if (PARAMS.XYZ_w_limit[0] == PARAMS.XYZ_w_output[0]) { // use outputWhite as displayWhite
        JMh = XYZ_to_Hellwig2022_JMh( XYZluminance, 
                                      PARAMS.XYZ_w_output );
    } else {  // "creative white" so use limitWhite as displayWhite
        JMh = XYZ_to_Hellwig2022_JMh( XYZluminance, 
                                      PARAMS.XYZ_w_limit );

        float RGB_w[3] = mult_f3_f33( PARAMS.XYZ_w_limit, PARAMS.OUTPUT_XYZ_TO_RGB);

        float RGB_w_f[3] = mult_f_f3( 1/100., RGB_w);
        float largestChannel = max( max(RGB_w_f[0], RGB_w_f[1]), RGB_w_f[2]);
        XYZluminance = mult_f_f3( 1./largestChannel, XYZluminance);
    }
        
    return JMh;
}
    

// "PowerP" compression function (also used in the ACES Reference Gamut Compression)
// values of v above 'threshold' are compressed by a 'power' function
// so that an input value of 'limit' results in an output of 1.0
float compressPowerP( float v, 
                      float thr, 
                      float lim, 
                      float power,
                      bool invert = false )
{
    float s = (lim-thr) / pow( pow( (1.0-thr) / (lim-thr), -power) - 1.0, 1.0/power);
    float nd = (v - thr) / s;
    float p = pow(nd, power);

    float vCompressed;

    if (invert) {
        if (v < thr || lim < 1.0001 || v < thr + s) {
            vCompressed = v;
        } else {
            vCompressed = thr + s * pow(-(pow((v - thr) / s, power) / (pow((v - thr) / s, power) - 1.)), 1. / power);
        }
    } else {
        if ( v < thr || lim < 1.0001) {
            vCompressed = v;
        } else {
            vCompressed = thr + s * nd / ( pow(1.0 + p, 1.0 / power));
        }
    }

    return vCompressed;
}

float[2] cuspFromTable( float h, 
                        float table[][3] )
{
    float lo[3];
    float hi[3];
    
    if (h <= table[0][2]) {
        lo = table[table.size-1];
        lo[2] = lo[2]-360.0;
        hi = table[0];
    } else if (h >= table[table.size-1][2]) {
        lo = table[table.size-1];
        hi = table[0];
        hi[2] = hi[2]+360.0;
    } else {
        for (int i = 1; i < table.size; i = i+1) {
            if (h <= table[i][2] && h > table[i-1][2]) {
                lo = table[i-1];
                hi = table[i];
            }
        }
    }
    
    float t = (h - lo[2]) / (hi[2] - lo[2]);
    float cuspJ = lerp( lo[0], hi[0], t);
    float cuspM = lerp( lo[1], hi[1], t);
    
    float cuspJM[2] = { cuspJ, cuspM };
    
    return cuspJM;    
}


float reachFromTable( float h, 
                      float table[] )
{
    int lo = floor(fmod(h, 360.0));
    int hi = ceil(fmod(h, 360.0));
    if (hi == 360)
        hi = 0;
    float t = fmod(h, 1.0);

    return lerp(table[lo], table[hi], t);
}


// A "toe" function that remaps the given value x between 0 and limit.
// The k1 and k2 parameters change the size and shape of the toe.
// https://www.desmos.com/calculator/6vplvw14ti
float toe( float x, 
           float limit, 
           float k1_in, 
           float k2_in,
           bool invert = false )
{
    if (x > limit)
      return x;

    float k2 = max(k2_in, 0.001);
    float k1 = sqrt(k1_in * k1_in + k2 * k2);
    float k3 = (limit + k1) / (limit + k2);
    
    if (invert) {
        return (x * x + k1 * x) / (k3 * (x + k2));
    } else {
        return 0.5 * (k3 * x - k1 + sqrt((k3 * x - k1) * (k3 * x - k1) + 4. * k2 * k3 * x));
    }
}

// In-gamut chroma compression
//
// Compresses colors inside the gamut with the aim for colorfulness to have an
// appropriate rate of change from display black to display white, and from
// achromatic outward to purer colors.
float chromaCompression_fwd( float JMh[3], 
                             float origJ, 
                             ODTParams PARAMS,
                             float REACH_GAMUT_TABLE[][3],
                             float REACH_TABLE[] )
{
    float J = JMh[0];
    float M = JMh[1];
    float h = JMh[2];
    
    if (M == 0.0) {
        return M;
    }

    float nJ = J / PARAMS.limitJmax;
    float snJ = max(0., 1. - nJ);
    float Mnorm = cuspFromTable( h, REACH_GAMUT_TABLE)[1];
    float limit = pow( nJ, PARAMS.model_gamma) * reachFromTable(h, REACH_TABLE) / Mnorm;

    // Rescaling of M with the tonescaled J to get the M to the same range as
    // J after the tonescale.  The rescaling uses the Hellwig2022 model gamma to
    // keep the M/J ratio correct (keeping the chromaticities constant).
    M = M * pow( J / origJ, PARAMS.model_gamma);

    // Normalize M with the rendering space cusp M        
    M = M / Mnorm;

    // Expand the colorfulness by running the toe function in reverse.  The goal is to
    // expand less saturated colors less and more saturated colors more.  The expansion
    // increases saturation in the shadows and mid-tones but not in the highlights.
    // The 0.001 offset starts the expansions slightly above zero.  The sat_thr makes
    // the toe less aggressive near black to reduce the expansion of noise.
    M = limit - toe( limit - M, 
                     limit - 0.001, 
                     snJ * PARAMS.sat, 
                     sqrt(nJ * nJ + PARAMS.sat_thr),
                     false );

    // Compress the colorfulness.  The goal is to compress less saturated colors more and
    // more saturated colors less, especially in the highlights.  This step creates the
    // saturation roll-off in the highlights, but attemps to preserve pure colors.  This
    // mostly affects highlights and mid-tones, and does not compress shadows.        
    M = toe( M, 
             limit, 
             nJ * PARAMS.compr, 
             snJ,
             false );

    // Denormalize            
    M = M * Mnorm;

    return M;    
}

float chromaCompression_inv( float JMh[3], 
                             float origJ, 
                             ODTParams PARAMS,
                             float REACH_GAMUT_TABLE[][3],
                             float REACH_TABLE[] )
{
    float J = JMh[0];
    float M = JMh[1];
    float h = JMh[2];
    
    if (M == 0.0) {
        return M;
    }

    float nJ = J / PARAMS.limitJmax;
    float snJ = max(0., 1. - nJ);
    float Mnorm = cuspFromTable( h, REACH_GAMUT_TABLE)[1];
    float limit = pow( nJ, PARAMS.model_gamma) * reachFromTable(h, REACH_TABLE) / Mnorm;

    M = M / Mnorm;
    M = toe( M, 
             limit, 
             nJ * PARAMS.compr, 
             snJ, 
             true );
    M = limit - toe( limit - M, 
                     limit - 0.001, 
                     snJ * PARAMS.sat, 
                     sqrt(nJ * nJ + PARAMS.sat_thr), 
                     true );
    M = M * Mnorm;
    M = M * pow( J / origJ, -PARAMS.model_gamma);

    return M;    
}

float[3] tonemapAndCompress_fwd( float inputJMh[3], 
                                 ODTParams PARAMS, 
                                 float REACH_GAMUT_TABLE[][3], 
                                 float REACH_TABLE[] )
{
    float outputJMh[3];
    
    float linear = Hellwig_J_to_Y( inputJMh[0] ) / PARAMS.referenceLuminance;

    TSParams TSPARAMS = init_TSParams( PARAMS.peakLuminance );
    float luminanceTS = tonescale_fwd( linear, TSPARAMS);

    float tonemappedJ = Y_to_Hellwig_J( luminanceTS );

    float tonemappedJMh[3] = { tonemappedJ, inputJMh[1], inputJMh[2]};

    outputJMh = tonemappedJMh;
    outputJMh[1] = chromaCompression_fwd( outputJMh, 
                                          inputJMh[0], 
                                          PARAMS, 
                                          REACH_GAMUT_TABLE,
                                          REACH_TABLE );

    return outputJMh;

}


float[3] tonemapAndCompress_inv( float JMh[3],
                                 ODTParams PARAMS, 
                                 float REACH_GAMUT_TABLE[][3], 
                                 float REACH_TABLE[] )
{
    float tonemappedJMh[3] = JMh;
    
    float luminance = Hellwig_J_to_Y( JMh[0] );

    TSParams TSPARAMS = init_TSParams( PARAMS.peakLuminance );  
    float linear = tonescale_inv( luminance / PARAMS.referenceLuminance, 
                                  TSPARAMS );

    float untonemappedJ = Y_to_Hellwig_J( linear * PARAMS.referenceLuminance );
    float untonemappedColorJMh[3] = {untonemappedJ, tonemappedJMh[1], tonemappedJMh[2]};

    // Chroma compression
    untonemappedColorJMh[1] = chromaCompression_inv( tonemappedJMh,
                                                     untonemappedColorJMh[0], 
                                                     PARAMS,
                                                     REACH_GAMUT_TABLE,
                                                     REACH_TABLE );

    return  untonemappedColorJMh;
  }
                                 


float solve_J_intersect( float J,
                         float M, 
                         float focusJ, 
                         float maxJ, 
                         float slope_gain )
{
    float a = M / (focusJ * slope_gain);
    float b = 0.0;
    float c = 0.0;
    float intersectJ = 0.0;
    
    if (J < focusJ) {
        b = 1.0 - M / slope_gain;
    } else {
        b= -(1.0 + M / slope_gain + maxJ * M / (focusJ * slope_gain));
    } 

    if (J < focusJ) {
        c = -J;
    } else {
        c = maxJ * M / slope_gain + J;
    }

    float root = sqrt( b * b - 4.0 * a * c);

    if (J < focusJ){
        intersectJ = 2.0 * c / (-b - root);
    } else {
        intersectJ = 2.0 * c / (-b + root);
    } 

    return intersectJ;
}


float[3] findGamutBoundaryIntersection( float JMh_s[3], 
                                        float JM_cusp_in[2], 
                                        float J_focus, 
                                        float J_max, 
                                        float slope_gain, 
                                        float gamma_top, 
                                        float gamma_bottom )
{
    float slope = 0.0;

    float s = max(0.000001, smoothCusps);
    float JM_cusp[2];
    JM_cusp[0] = JM_cusp_in[0] * (1.0 + smoothJ * s);   // J
    JM_cusp[1] = JM_cusp_in[1] * (1.0 + smoothM * s);   // M

    float J_intersect_source = solve_J_intersect( JMh_s[0], 
                                                  JMh_s[1], 
                                                  J_focus, 
                                                  J_max, 
                                                  slope_gain );
    float J_intersect_cusp = solve_J_intersect( JM_cusp[0], 
                                                JM_cusp[1], 
                                                J_focus, 
                                                J_max, 
                                                slope_gain );

//     print( "JM_source = \t", JMh_s[0], "\t", JMh_s[1], "\n");
//     print( "slope_gain = \t", slope_gain, "\n");
//     print( "newCusp = \t", JM_cusp[0], "\t", JM_cusp[1], "\n");
//     print( "J_int_s = \t", J_intersect_source, "\n");
//     print( "J_int_c = \t", J_intersect_cusp, "\n");
// 
//     print( "\n");
//     

    if (J_intersect_source < J_focus) {
        slope = J_intersect_source * (J_intersect_source - J_focus) / (J_focus * slope_gain);
    } else {
        slope = (J_max - J_intersect_source) * (J_intersect_source - J_focus) / (J_focus * slope_gain);
    } 

    float M_boundary_lower = J_intersect_cusp * pow(J_intersect_source / J_intersect_cusp, 1. / gamma_bottom) / (JM_cusp[0] / JM_cusp[1] - slope);

    float M_boundary_upper = JM_cusp[1] * (J_max - J_intersect_cusp) * pow((J_max - J_intersect_source) / (J_max - J_intersect_cusp), 1. / gamma_top) / (slope * JM_cusp[1] + J_max - JM_cusp[0]);

    float M_boundary = JM_cusp[1] * smin(M_boundary_lower / JM_cusp[1], M_boundary_upper / JM_cusp[1], s);

    float J_boundary = J_intersect_source + slope * M_boundary;

    float return_JMh[3] = { J_boundary, M_boundary, J_intersect_source };

    return return_JMh;
}


float hueDependentUpperHullGamma( float h, float gamma_table[]) {

    const int i_lo = hue_position_in_uniform_table(h, gamma_table.size);
    const int i_hi = next_position_in_table(i_lo, gamma_table.size);

    const float base_hue = base_hue_for_position(i_lo, gamma_table.size);
    const float t = h - base_hue;

    return lerp( gamma_table[i_lo], gamma_table[i_hi], t);
}

// https://www.desmos.com/calculator/oe2fscya80
float getFocusGain( float J, 
                    float cuspJ,
                    float limitJmax ) 
{

    float thr = lerp(cuspJ, limitJmax, focusGainBlend);
    if (J > thr) {
        // Approximate inverse required above threshold
        float gain = (limitJmax - thr) / max(0.0001, (limitJmax - min(limitJmax, J)));
        return pow( log10(gain), 1. / focusAdjustGain) + 1.;
    } else {
        // Analytic inverse possible below cusp
        return 1.;
    }
}

float[3] getReachBoundary( float J, 
                           float M, 
                           float h,
                           ODTParams PARAMS,
                           float reachTable[][3] ) 
{
    float limitJmax = PARAMS.limitJmax;
    float midJ = PARAMS.midJ;
//     float cuspMidBlend = PARAMS.cuspMidBlend;
//     float focusDist = PARAMS.focusDistance;
//     float focusGainBlend = PARAMS.focusGainBlend;
//     float focusAdjustGain = PARAMS.focusAdjustGain;
    float model_gamma = PARAMS.model_gamma;
    
    const int i_lo = hue_position_in_uniform_table(h, reachTable.size);
    const int i_hi = next_position_in_table(i_lo, reachTable.size);

    const float lo[3] = reachTable[i_lo];
    const float hi[3] = reachTable[i_hi];

    const float t = (h - lo[2]) / (hi[2] - lo[2]);

    const float reachMaxM = lerp(lo[1], hi[1], t);

    float JMcusp[2] = cuspFromTable( h, reachTable );
    float focusJ = lerp( JMcusp[0], 
                         midJ, 
                         min(1.0, cuspMidBlend - (JMcusp[0] / limitJmax)) );
    float slope_gain = limitJmax * focusDistance * getFocusGain( J, 
                                                                 JMcusp[0],
                                                                 limitJmax );

    float intersectJ = solve_J_intersect( J, M, focusJ, limitJmax, slope_gain);
    float slope;
    if (intersectJ < focusJ) {
        slope = intersectJ * (intersectJ - focusJ) / (focusJ * slope_gain);
    } else {
        slope = (limitJmax - intersectJ) * (intersectJ - focusJ) / (focusJ * slope_gain);
    }
    float boundary = limitJmax * pow(intersectJ / limitJmax, model_gamma) * reachMaxM / (limitJmax - slope * reachMaxM);
    float result[3] = { J, boundary, h };
    return result;
}




float[3] compressGamut( float JMh[3], 
                        ODTParams PARAMS,
                        float Jx,
                        float gamutCuspTable[][3], 
                        float gamutTopGamma[], 
                        float reachGamutTable[][3],
                        bool invert=false )
{
    float limitJmax = PARAMS.limitJmax;
    float midJ = PARAMS.midJ;
//     float cuspMidBlend = PARAMS.cuspMidBlend;
//     float focusDist = PARAMS.focusDistance;
//     float focusGainBlend = PARAMS.focusGainBlend;
//     float focusAdjustGain = PARAMS.focusAdjustGain;
//     float smoothCusps = PARAMS.smoothCusps;
//     float smoothJ = PARAMS.smoothJ;
//     float smoothM = PARAMS.smoothM;

    float project_from[2] = {JMh[0], JMh[1]};
    float JMcusp[2] = cuspFromTable(JMh[2], gamutCuspTable);
    
    if (project_from[1] == 0.0)
        return JMh;
    
    // Calculate where the out of gamut color is projected to
    float focusJ = lerp( JMcusp[0], midJ, min(1., cuspMidBlend - (JMcusp[0] / limitJmax)));

    float slope_gain = limitJmax * focusDistance * getFocusGain( Jx, 
                                                                 JMcusp[0],
                                                                 limitJmax );

    float projectJ = solve_J_intersect( project_from[0], project_from[1], focusJ, limitJmax, slope_gain);
    
    // Find gamut intersection    
    float gamma_top = hueDependentUpperHullGamma( JMh[2], gamutTopGamma);
    float gamma_bottom = 1.14;

    float boundaryReturn[3] = findGamutBoundaryIntersection( JMh, 
                                                             JMcusp, 
                                                             focusJ, 
                                                             limitJmax, 
                                                             slope_gain, 
                                                             gamma_top, 
                                                             gamma_bottom );
                                                                                                                         
    float JMboundary[2] = {boundaryReturn[0], boundaryReturn[1]};
    float project_to[2] = {boundaryReturn[2], 0.};
    projectJ = boundaryReturn[2];

    const float compressionFuncParams[4] = {0.75, 1.1, 1.3, 1.2};

    float JMh_reachBoundary[3] = getReachBoundary( JMboundary[0], 
                                                   JMboundary[1], 
                                                   JMh[2], 
                                                   PARAMS,
                                                   reachGamutTable);
    float locusMax = JMh_reachBoundary[1];
    float difference = max(1.0001, locusMax / JMboundary[1] );
    float threshold = max( compressionFuncParams[0], 1. / difference);

    float interpolatedCompressionFuncParams[4] = {threshold, difference, difference, compressionFuncParams[3]};



    // Compress the out of gamut color along the projection line
    float JMcompressed[2] = project_from;

    if (JMh[0] < limitJmax && JMh[1] > 0.0) {
        float v = project_from[1] / JMboundary[1];
        v = compressPowerP( v, 
                            interpolatedCompressionFuncParams[0], 
                            lerp( interpolatedCompressionFuncParams[2], 
                                  interpolatedCompressionFuncParams[1], 
                                  projectJ / limitJmax),
                            interpolatedCompressionFuncParams[3],
                            invert );
        JMcompressed[0] = project_to[0] + v * (JMboundary[0] - project_to[0]);
        JMcompressed[1] = project_to[1] + v * (JMboundary[1] - project_to[1]);
    } else {
        JMcompressed[0] = JMh[0];
        JMcompressed[1] = 0.;
    }

    float return_JMh[3] = { JMcompressed[0], JMcompressed[1], JMh[2] };
    
    return return_JMh;
}

float[3] gamutMap_fwd( float JMh[3], 
                       ODTParams PARAMS,
                       float gamutCuspTable[][3], 
                       float gamutTopGamma[], 
                       float reachGamutTable[][3] )
{
    return compressGamut( JMh, 
                          PARAMS,
                          JMh[0], 
                          gamutCuspTable, 
                          gamutTopGamma, 
                          reachGamutTable, 
                          false );
}

float[3] gamutMap_inv( float JMh[3], 
                       ODTParams PARAMS,  
                       float gamutCuspTable[][3], 
                       float gamutTopGamma[], 
                       float reachGamutTable[][3] )
{
    float JMcusp[2] = cuspFromTable( JMh[2], gamutCuspTable);
    float Jx = JMh[0];
    
    // Analytic inverse below threshold
    if (Jx <= lerp( JMcusp[0], PARAMS.limitJmax, focusGainBlend) )
        return compressGamut( JMh, 
                              PARAMS,
                              Jx, 
                              gamutCuspTable, 
                              gamutTopGamma, 
                              reachGamutTable, 
                              true );

    // Approximation above threshold
    Jx = compressGamut( JMh,
                        PARAMS,
                        Jx,  
                        gamutCuspTable, 
                        gamutTopGamma, 
                        reachGamutTable, 
                        true )[0];
    return compressGamut( JMh,
                          PARAMS, 
                          Jx, 
                          gamutCuspTable, 
                          gamutTopGamma, 
                          reachGamutTable, 
                          true );
}