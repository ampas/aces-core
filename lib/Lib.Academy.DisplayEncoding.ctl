
// <ACEStransformID>urn:ampas:aces:transformId:v2.0:Lib.Academy.DisplayEncoding.a2.v1</ACEStransformID>
// <ACESuserName>Display Encoding Functions</ACESuserName>

// 
// Library File with functions used for the display encoding/decoding steps
//


float[3] apply_white_scale( float XYZluminance[3], 
                            ODTParams PARAMS, 
                            bool invert )
{
    float RGB_w[3] = mult_f3_f33( PARAMS.XYZ_w_limit, PARAMS.OUTPUT_XYZ_TO_RGB);
    float RGB_w_f[3] = mult_f_f3( 1./referenceLuminance, RGB_w);
    float scale = 1. / max( max(RGB_w_f[0], RGB_w_f[1]), RGB_w_f[2]); 
    // scale factor is equal to 1/largestChannel
            
    if (invert) {
        return mult_f_f3( 1./scale, XYZluminance);
    } else {
        return mult_f_f3( scale, XYZluminance);
    }
}


// Forward monitor curve - moncurve_f() with gamma=2.4 and offset=0.055 matches the inverse EOTF found in IEC 61966-2-1:1999 (sRGB)
float moncurve_fwd( float x, 
                    float gamma, 
                    float offs )
{
    float y;
    const float fs = (( gamma - 1.0) / offs) * pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
    const float xb = offs / ( gamma - 1.0);
    if ( x >= xb) {
        y = pow( ( x + offs) / ( 1.0 + offs), gamma);
    } else {
        y = x * fs;
    }
    return y;
}

// Reverse monitor curve - moncurve_r() with gamma=2.4 and offset=0.055 matches the inverse EOTF found in IEC 61966-2-1:1999 (sRGB)
float moncurve_inv( float y, 
                    float gamma, 
                    float offs )
{
    float x;
    const float yb = pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
    const float rs = pow( ( gamma - 1.0) / offs, gamma - 1.0) * pow( ( 1.0 + offs) / gamma, gamma);
    if ( y >= yb) {
        x = ( 1.0 + offs) * pow( y, 1.0 / gamma) - offs;
    } else {
        x = y * rs;
    }
    return x;
}

float[3] moncurve_fwd_f3( float x[3], 
                          float gamma, 
                          float offs )
{
    float y[3];
    y[0] = moncurve_fwd( x[0], gamma, offs);
    y[1] = moncurve_fwd( x[1], gamma, offs);
    y[2] = moncurve_fwd( x[2], gamma, offs);
    return y;
}

float[3] moncurve_inv_f3( float y[3], 
                          float gamma, 
                          float offs )
{
    float x[3];
    x[0] = moncurve_inv( y[0], gamma, offs);
    x[1] = moncurve_inv( y[1], gamma, offs);
    x[2] = moncurve_inv( y[2], gamma, offs);
    return x;
}


// The forward OTF specified in Rec. ITU-R BT.1886
// L = a(max[(V+b),0])^g
float bt1886_fwd( float V, 
                  float gamma, 
                  float Lw = 1.0, 
                  float Lb = 0.0 )
{
    float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
    float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
    float L = a * pow( max( V + b, 0.), gamma);
    return L;
}

// The reverse EOTF specified in Rec. ITU-R BT.1886
// L = a(max[(V+b),0])^g
float bt1886_inv( float L, 
                  float gamma, 
                  float Lw = 1.0, 
                  float Lb = 0.0 )
{
    float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
    float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
    float V = pow( max( L / a, 0.), 1./gamma) - b;
    return V;
}

float[3] bt1886_fwd_f3( float V[3], 
                      float gamma, 
                      float Lw = 1.0, 
                      float Lb = 0.0 )
{
    float L[3];
    L[0] = bt1886_fwd( V[0], gamma, Lw, Lb);
    L[1] = bt1886_fwd( V[1], gamma, Lw, Lb);
    L[2] = bt1886_fwd( V[2], gamma, Lw, Lb);
    return L;
}

float[3] bt1886_inv_f3( float L[3], 
                        float gamma, 
                        float Lw = 1.0,  
                        float Lb = 0.0 )
{
    float V[3];
    V[0] = bt1886_inv( L[0], gamma, Lw, Lb);
    V[1] = bt1886_inv( L[1], gamma, Lw, Lb);
    V[2] = bt1886_inv( L[2], gamma, Lw, Lb);
    return V;
}


// SMPTE Range vs Full Range scaling formulas
float smpteRange_to_fullRange( float in )
{
    const float REFBLACK = (  64. / 1023.);
    const float REFWHITE = ( 940. / 1023.);

    return (( in - REFBLACK) / ( REFWHITE - REFBLACK));
}

float fullRange_to_smpteRange( float in )
{
    const float REFBLACK = (  64. / 1023.);
    const float REFWHITE = ( 940. / 1023.);

    return ( in * ( REFWHITE - REFBLACK) + REFBLACK );
}

float[3] smpteRange_to_fullRange_f3( float rgbIn[3] )
{
    float rgbOut[3];
    rgbOut[0] = smpteRange_to_fullRange( rgbIn[0]);
    rgbOut[1] = smpteRange_to_fullRange( rgbIn[1]);
    rgbOut[2] = smpteRange_to_fullRange( rgbIn[2]);

    return rgbOut;
}

float[3] fullRange_to_smpteRange_f3( float rgbIn[3] )
{
    float rgbOut[3];
    rgbOut[0] = fullRange_to_smpteRange( rgbIn[0]);
    rgbOut[1] = fullRange_to_smpteRange( rgbIn[1]);
    rgbOut[2] = fullRange_to_smpteRange( rgbIn[2]);

    return rgbOut;
}



// Base functions from SMPTE ST 2084-2014

// Constants from SMPTE ST 2084-2014
const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
const float pq_m2 = 78.84375; // ( 2523.0 / 4096.0 ) * 128.0;
const float pq_c1 = 0.8359375; // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
const float pq_c2 = 18.8515625; // ( 2413.0 / 4096.0 ) * 32.0;
const float pq_c3 = 18.6875; // ( 2392.0 / 4096.0 ) * 32.0;

const float pq_C = 10000.0;

// Converts from the non-linear perceptually quantized space to linear cd/m^2
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float ST2084_to_Y( float N )
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this assumes full range (0 - 1)
  float Np = pow( N, 1.0 / pq_m2 );
  float L = Np - pq_c1;
  if ( L < 0.0 )
    L = 0.0;
  L = L / ( pq_c2 - pq_c3 * Np );
  L = pow( L, 1.0 / pq_m1 );
  return L * pq_C; // returns cd/m^2
}

// Converts from linear cd/m^2 to the non-linear perceptually quantized space
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float Y_to_ST2084( float C )
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this returns full range (0 - 1)
  float L = C / pq_C;
  float Lm = pow( L, pq_m1 );
  float N = ( pq_c1 + pq_c2 * Lm ) / ( 1.0 + pq_c3 * Lm );
  N = pow( N, pq_m2 );
  return N;
}

// converts from linear cd/m^2 to PQ code values
float[3] Y_to_ST2084_f3( float in[3] )
{  
  float out[3];
  out[0] = Y_to_ST2084( in[0]);
  out[1] = Y_to_ST2084( in[1]);
  out[2] = Y_to_ST2084( in[2]);

  return out;
}

// converts from PQ code values to linear cd/m^2
float[3] ST2084_to_Y_f3( float in[3] )
{
  float out[3];
  out[0] = ST2084_to_Y( in[0]);
  out[1] = ST2084_to_Y( in[1]);
  out[2] = ST2084_to_Y( in[2]);

  return out;
}


// Conversion of PQ signal to HLG, as detailed in Section 7 of ITU-R BT.2390-0
float[3] ST2084_to_HLG_1000nits_f3( float PQ[3] ) 
{
    // ST.2084 EOTF (non-linear PQ to display light)
    float displayLinear[3] = ST2084_to_Y_f3( PQ);

    // HLG Inverse EOTF (i.e. HLG inverse OOTF followed by the HLG OETF)
    // HLG Inverse OOTF (display linear to scene linear)
    float Y_d = 0.2627*displayLinear[0] + 0.6780*displayLinear[1] + 0.0593*displayLinear[2];
    const float L_w = 1000.;
    const float L_b = 0.;
    const float alpha = (L_w-L_b);
    const float beta = L_b;
    const float gamma = 1.2;
    
    float sceneLinear[3];
    if (Y_d == 0.) { 
        /* This case is to protect against pow(0,-N)=Inf error. The ITU document
        does not offer a recommendation for this corner case. There may be a 
        better way to handle this, but for now, this works. 
        */ 
        sceneLinear[0] = 0.;
        sceneLinear[1] = 0.;
        sceneLinear[2] = 0.;        
    } else {
        sceneLinear[0] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[0]-beta)/alpha);
        sceneLinear[1] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[1]-beta)/alpha);
        sceneLinear[2] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[2]-beta)/alpha);
    }

    // HLG OETF (scene linear to non-linear signal value)
    const float a = 0.17883277;
    const float b = 0.28466892; // 1.-4.*a;
    const float c = 0.55991073; // 0.5-a*log(4.*a);

    float HLG[3];
    if (sceneLinear[0] <= 1./12) {
        HLG[0] = sqrt(3.*sceneLinear[0]);
    } else {
        HLG[0] = a*log(12.*sceneLinear[0]-b)+c;
    }
    if (sceneLinear[1] <= 1./12) {
        HLG[1] = sqrt(3.*sceneLinear[1]);
    } else {
        HLG[1] = a*log(12.*sceneLinear[1]-b)+c;
    }
    if (sceneLinear[2] <= 1./12) {
        HLG[2] = sqrt(3.*sceneLinear[2]);
    } else {
        HLG[2] = a*log(12.*sceneLinear[2]-b)+c;
    }

    return HLG;
}


// Conversion of HLG to PQ signal, as detailed in Section 7 of ITU-R BT.2390-0
float[3] HLG_to_ST2084_1000nits_f3( float HLG[3]) 
{
    const float a = 0.17883277;
    const float b = 0.28466892; // 1.-4.*a;
    const float c = 0.55991073; // 0.5-a*log(4.*a);

    const float L_w = 1000.;
    const float L_b = 0.;
    const float alpha = (L_w-L_b);
    const float beta = L_b;
    const float gamma = 1.2;

    // HLG EOTF (non-linear signal value to display linear)
    // HLG to scene-linear
    float sceneLinear[3];
    if ( HLG[0] >= 0. && HLG[0] <= 0.5) {
        sceneLinear[0] = pow(HLG[0],2.)/3.;
    } else {
        sceneLinear[0] = (exp((HLG[0]-c)/a)+b)/12.;
    }        
    if ( HLG[1] >= 0. && HLG[1] <= 0.5) {
        sceneLinear[1] = pow(HLG[1],2.)/3.;
    } else {
        sceneLinear[1] = (exp((HLG[1]-c)/a)+b)/12.;
    }        
    if ( HLG[2] >= 0. && HLG[2] <= 0.5) {
        sceneLinear[2] = pow(HLG[2],2.)/3.;
    } else {
        sceneLinear[2] = (exp((HLG[2]-c)/a)+b)/12.;
    }        
    
    float Y_s = 0.2627*sceneLinear[0] + 0.6780*sceneLinear[1] + 0.0593*sceneLinear[2];

    // Scene-linear to display-linear
    float displayLinear[3];
    displayLinear[0] = alpha * pow( Y_s, gamma-1.) * sceneLinear[0] + beta;
    displayLinear[1] = alpha * pow( Y_s, gamma-1.) * sceneLinear[1] + beta;
    displayLinear[2] = alpha * pow( Y_s, gamma-1.) * sceneLinear[2] + beta;
        
    // ST.2084 Inverse EOTF
    float PQ[3] = Y_to_ST2084_f3( displayLinear);

    return PQ;
}




//  0 - display linear
//  1 - ST.2084
//  2 - HLG
//  3 - gamma 2.6
//  4 - BT.1886 with gamma 2.4
//  5 - gamma 2.2
//  6 - sRGB IEC 61966-2-1:1999

float[3] eotf_inv( float rgb_linear[3],
                   int eotf_enum )
{
    if (eotf_enum == 0) {
        // display linear
        return rgb_linear;
    } else if (eotf_enum == 1) {
        // ST. 2084
        float rgb_clamped[3] = clamp_f3( rgb_linear, 0.0, referenceLuminance);
        return Y_to_ST2084_f3( mult_f_f3( referenceLuminance, rgb_clamped) );
    } else if (eotf_enum == 2) {
        // HLG
        float rgb_clamped[3] = clamp_f3( rgb_linear, 0.0, referenceLuminance);
        float PQ[3] = Y_to_ST2084_f3( mult_f_f3( referenceLuminance, rgb_clamped) );
        return ST2084_to_HLG_1000nits_f3( PQ );
    } else if (eotf_enum == 3) {
        // gamma 2.6
        float rgb_clamped[3] = clamp_f3( rgb_linear, 0.0, 1.0);
        return pow_f3( rgb_clamped, 1/2.6);
    } else if (eotf_enum == 4) {
        // BT.1886 with gamma 2.4
        float rgb_clamped[3] = clamp_f3( rgb_linear, 0.0, 1.0);
        return bt1886_inv_f3( rgb_clamped, 2.4, 1.0, 0.0 );
    } else if (eotf_enum == 5) {
        // gamma 2.2
        float rgb_clamped[3] = clamp_f3( rgb_linear, 0.0, 1.0);
        return pow_f3( rgb_clamped, 1/2.2);
    } else {
        // sRGB IEC 61966-2-1:1999
        float rgb_clamped[3] = clamp_f3( rgb_linear, 0.0, 1.0);
        return moncurve_inv_f3( rgb_clamped, 2.4, 0.055);
    }
}

float[3] eotf( float rgb_cv[3],
               int eotf_enum )
{
    if (eotf_enum == 0) {
        // display linear
        return rgb_cv;
    } else if (eotf_enum == 1) {
        // ST. 2084
        return mult_f_f3( 1./referenceLuminance, ST2084_to_Y_f3( rgb_cv ));
    } else if (eotf_enum == 2) {
        // HLG
        float PQ[3] = HLG_to_ST2084_1000nits_f3( rgb_cv);
        return mult_f_f3( 1/referenceLuminance, ST2084_to_Y_f3( PQ ));
    } else if (eotf_enum == 3) {
        // gamma 2.6
        return pow_f3( rgb_cv, 2.6);
    } else if (eotf_enum == 4) {
        // BT.1886 with gamma 2.4
        return bt1886_fwd_f3( rgb_cv, 2.4, 1.0, 0.0 );
    } else if (eotf_enum == 5) {
        // gamma 2.2
        return pow_f3( rgb_cv, 2.2);
    } else {
        // sRGB IEC 61966-2-1:1999
        return moncurve_fwd_f3( rgb_cv, 2.4, 0.055);
    }
}

float[3] clamp_zero_to_peakLuminance(float XYZ_in[3],
                                     ODTParams PARAMS)
{
    float rgb[3] = mult_f3_f33(XYZ_in, PARAMS.LIMIT_XYZ_TO_RGB);

    // Clamp to relative peakLuminance in RGB space prior to white scaling
    rgb = clamp_f3(rgb, 0.0, PARAMS.peakLuminance / referenceLuminance);

    float XYZ[3] = mult_f3_f33(rgb, PARAMS.LIMIT_RGB_TO_XYZ);

    return XYZ;
}

float[3] white_limiting(float XYZ_in[3],
                        ODTParams PARAMS,
                        bool scale_white,
                        bool invert = false)
{
    // In the forward direction, the white limiting step clamps the output to
    // between zero and the peak luminance value so that values do not exceed
    // the maximum value of the tonescale (because the tonescale extends
    // slightly above the peak luminance so that it crosses through the peak
    // luminance value with some slope and does not approach an asymptote). If
    // the creative white differs from the calibration white of the display,
    // unequal display code values will be required to produce the neutral of
    // the creative white. Without scaling, one channel would hit the max value
    // first while the other channels continue to increase, resulting in a hue
    // shift. To avoid this, the white scaling finds the largest channel and
    // applies a scale factor to force the point where this channel hits max to
    // 1.0, assuring that all three channels "fit" within the peak value.
    // In the inverse direction, the white scaling is removed.

    float XYZ[3] = XYZ_in;

    // Clamp to peak luminance in the forward direction
    if (!invert)
        XYZ = clamp_zero_to_peakLuminance(XYZ, PARAMS);

    // White point scaling
    if (scale_white)
    {
        if (invert)
        { // Remove white scaling
            XYZ = apply_white_scale(XYZ, PARAMS, true);
        }
        else
        { // Apply white scaling
            XYZ = apply_white_scale(XYZ, PARAMS, false);
        }
    }
    return XYZ;
}

float[3] display_encoding(float XYZ[3],
                          ODTParams PARAMS,
                          int eotf_enum,
                          float linear_scale = 1.0)
{
    // XYZ to display RGB
    float RGB_display_linear[3] = mult_f3_f33(XYZ, PARAMS.OUTPUT_XYZ_TO_RGB);

    // Linear scale factor
    RGB_display_linear = mult_f_f3(linear_scale, RGB_display_linear);

    // Apply inverse EOTF
    float out[3] = eotf_inv(RGB_display_linear, eotf_enum);

    return out;
}

float[3] display_decoding(float cv[3],
                          ODTParams PARAMS,
                          int eotf_enum,
                          float linear_scale = 1.0)
{
    // Apply EOTF
    float RGB_display_linear[3] = eotf(cv, eotf_enum);

    // Linear scale factor
    RGB_display_linear = mult_f_f3(1. / linear_scale, RGB_display_linear);

    // Display RGB to XYZ
    float XYZ[3] = mult_f3_f33(RGB_display_linear, PARAMS.OUTPUT_RGB_TO_XYZ);

    return XYZ;
}