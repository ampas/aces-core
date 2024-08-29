
// <ACEStransformID>urn:ampas:aces:transformId:v2.0:Lib.Academy.OutputTransform.a2.v1</ACEStransformID>
// <ACESuserName>Output Transform Functions</ACESuserName>

//
// Library File with functions and presets used for the forward and inverse output
// transform
//

// Gamut compression constants
const float smoothCusps = 0.12;
const float smoothM = 0.27;
const float cuspMidBlend = 1.3;

const float focusGainBlend = 0.3;
const float focusAdjustGain = 0.55;
const float focusDistance = 1.35;
const float focusDistanceScaling = 1.75;

// Constant used in gamut mapping
const float compressionThreshold = 0.75;

const int gamutTableSize = 360;       // add 1 extra entry at end that is to duplicate first entry for wrapped hue
const int additionalTableEntries = 2; // allots for extra entries to wrap the hues without special cases
const int totalTableSize = gamutTableSize + additionalTableEntries;
const int baseIndex = 1; // array index for smallest hue, which is not necessarily a 0.0 hue angle

const float gammaMinimum = 0.0;
const float gammaMaximum = 5.0;
const float gammaSearchStep = 0.4;
const float gammaAccuracy = 1e-5;

// Academy Primaries 0 (i.e. "ACES" Primaries from SMPTE ST2065-1)
const Chromaticities AP0 =
    {
        {0.73470, 0.26530},
        {0.00000, 1.00000},
        {0.00010, -0.07700},
        {0.32168, 0.33767}
    };

// Academy Primaries 1
const Chromaticities AP1 =
    {
        {0.713, 0.293},
        {0.165, 0.830},
        {0.128, 0.044},
        {0.32168, 0.33767}
    };

// "Reach" Primaries - equal to ACES "AP1" primaries
const Chromaticities REACH_PRI = AP1;

const float AP0_XYZ_TO_RGB[3][3] = XYZtoRGB_f33(AP0, 1.0);
const float AP0_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33(AP0, 1.0);

const float AP1_XYZ_TO_RGB[3][3] = XYZtoRGB_f33(AP1, 1.0);

const float referenceLuminance = 100.;

// CAM Parameters
const float L_A = 100.;
const float Y_b = 20.;

const float ac_resp = 1.0;
const float ra = 2. * ac_resp;
const float ba = 0.05 + (2. - ra);

float[3] viewingConditionsToSurround(int viewingConditions = 1)
{
    float newSurround[3];

    if (viewingConditions == 0)
    { // "Dark"
        newSurround[0] = 0.8;
        newSurround[1] = 0.525;
        newSurround[2] = 0.8;
    }
    else if (viewingConditions == 1)
    { // "Dim
        newSurround[0] = 0.9;
        newSurround[1] = 0.59;
        newSurround[2] = 0.9;
    }
    else if (viewingConditions == 2)
    { // "Average"
        newSurround[0] = 1.0;
        newSurround[1] = 0.69;
        newSurround[2] = 1.0;
    }
    else
    {
    }
    return newSurround;
}

const Chromaticities CAM16_PRI =
    {
        {0.8336, 0.1735},
        {2.3854, -1.4659},
        {0.087, -0.125},
        {0.333, 0.333}
    };

const float MATRIX_16[3][3] = XYZtoRGB_f33(CAM16_PRI, 1.0);

float[3][3] generate_panlrcm(float ra = 2.,
                             float ba = 0.05)
{
    float panlrcm_data[3][3] =
        {
            {ra, 1., 1. / 9.},
            {1., -12. / 11., 1. / 9.},
            {ba, 1. / 11., -2. / 9.}};
    float panlrcm[3][3] = invert_f33(panlrcm_data);

    // Normalize columns so that first row is 460
    for (int i = 0; i < 3; i = i + 1)
    {
        float n = 460.0 / panlrcm[0][i];
        panlrcm[0][i] = panlrcm[0][i] * n;
        panlrcm[1][i] = panlrcm[1][i] * n;
        panlrcm[2][i] = panlrcm[2][i] * n;
    }

    return panlrcm;
}

const float panlrcm[3][3] = generate_panlrcm(ra, ba); // Matrix for Hellwig inverse

struct ODTParams
{
    float peakLuminance;

    // Tonescale                // Set via TSParams structure
    float n_r; // normalized white in nits (what 1.0 should be)
    float g;   // surround / contrast
    float t_1; // shadow toe or flare/glare compensation
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
    float chromaCompressScale;

    float focusDist;

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

float wrap_to_360(float hue)
{
    float y = fmod(hue, 360.);
    if (y < 0.)
    {
        y = y + 360.;
    }
    return y;
}

int hue_position_in_uniform_table(float hue, int table_size)
{
    const float wrapped_hue = wrap_to_360(hue);
    int result = (wrapped_hue / 360. * table_size);
    return result;
}

int next_position_in_table(int entry, int table_size)
{
    int result = (entry + 1) % table_size;
    return result;
}

float base_hue_for_position(int i_lo, int table_size)
{
    float result = i_lo * 360. / table_size;
    return result;
}

// CAM Functions
float[3] post_adaptation_non_linear_response_compression_forward(float RGB[3], float F_L)
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

float[3] post_adaptation_non_linear_response_compression_inverse(float RGB[3], float F_L)
{
    float RGB_p[3];
    RGB_p[0] = (sign(RGB[0]) * 100. / F_L * spow((27.13 * fabs(RGB[0])) / (400. - fabs(RGB[0])), 1.0 / 0.42));
    RGB_p[1] = (sign(RGB[1]) * 100. / F_L * spow((27.13 * fabs(RGB[1])) / (400. - fabs(RGB[1])), 1.0 / 0.42));
    RGB_p[2] = (sign(RGB[2]) * 100. / F_L * spow((27.13 * fabs(RGB[2])) / (400. - fabs(RGB[2])), 1.0 / 0.42));

    return RGB_p;
}

float[3] XYZ_to_Hellwig2022_JMh(float XYZ[3],
                                float XYZ_w[3],
                                int viewingConditions = 1)
{
    float surround[3] = viewingConditionsToSurround(viewingConditions);

    float Y_w = XYZ_w[1];

    // Step 0 - Converting CIE XYZ tristimulus values to sharpened RGB values
    float RGB_w[3] = mult_f3_f33(XYZ_w, MATRIX_16);

    // Ignore degree of adaptation.
    float D = 1.0;

    // Viewing condition dependent parameters
    float k = 1. / (5. * L_A + 1.);
    float k4 = pow(k, 4.);
    float F_L = 0.2 * k4 * (5. * L_A) + 0.1 * pow((1. - k4), 2.) * spow(5. * L_A, 1. / 3.);
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

    float RGB_aw[3] = post_adaptation_non_linear_response_compression_forward(RGB_wc, F_L);
    float A_w = ra * RGB_aw[0] + RGB_aw[1] + ba * RGB_aw[2];

    // Step 1 - Converting CIE XYZ tristimulus values to sharpened RGB values
    float RGB[3] = mult_f3_f33(XYZ, MATRIX_16);

    // Step 2
    float RGB_c[3];
    RGB_c[0] = D_RGB[0] * RGB[0];
    RGB_c[1] = D_RGB[1] * RGB[1];
    RGB_c[2] = D_RGB[2] * RGB[2];

    // Step 3 - apply  forward post-adaptation non-linear response compression
    float RGB_a[3] = post_adaptation_non_linear_response_compression_forward(RGB_c, F_L);

    // Step 4 - Converting to preliminary cartesian  coordinates
    float a = RGB_a[0] - 12. * RGB_a[1] / 11. + RGB_a[2] / 11.;
    float b = (RGB_a[0] + RGB_a[1] - 2. * RGB_a[2]) / 9.;

    // Computing the hue angle math h
    // Unclear why this isn't matching the python version
    float hr = atan2(b, a);
    float h = wrap_to_360(radians_to_degrees(hr));

    // Step 6 - Computing achromatic responses for the stimulus
    float A = ra * RGB_a[0] + RGB_a[1] + ba * RGB_a[2];

    // Step 7 - Computing the correlate of lightness, J
    float J = 100. * pow(A / A_w, surround[1] * z);

    // Step 9 - Computing the correlate of colourfulness, M
    float M = 43. * surround[2] * sqrt(a * a + b * b);

    if (J == 0.0)
        M = 0.0;

    float return_JMh[3] = {J, M, h};
    return return_JMh;
}

float[3] Hellwig2022_JMh_to_XYZ(float JMh[3],
                                float XYZ_w[3],
                                int viewingConditions = 1)
{
    float surround[3] = viewingConditionsToSurround(viewingConditions);

    float J = JMh[0];
    float M = JMh[1];
    float h = JMh[2];

    float Y_w = XYZ_w[1];

    // Step 0 - Converting CIE XYZ tristimulus values to sharpened RGB values.
    float RGB_w[3] = mult_f3_f33(XYZ_w, MATRIX_16);

    // Ignore degree of adaptation.
    float D = 1.0;

    // Viewing condition dependent parameters
    float k = 1.0 / (5.0 * L_A + 1.0);
    float k4 = pow(k, 4.0);
    float F_L = 0.2 * k4 * (5.0 * L_A) + 0.1 * pow((1.0 - k4), 2.0) * spow(5.0 * L_A, 1.0 / 3.0);
    float n = sdiv(Y_b, Y_w);
    float z = 1.48 + sqrt(n);

    float D_RGB[3];
    D_RGB[0] = D * Y_w / RGB_w[0] + 1.0 - D;
    D_RGB[1] = D * Y_w / RGB_w[1] + 1.0 - D;
    D_RGB[2] = D * Y_w / RGB_w[2] + 1.0 - D;

    float RGB_wc[3];
    RGB_wc[0] = D_RGB[0] * RGB_w[0];
    RGB_wc[1] = D_RGB[1] * RGB_w[1];
    RGB_wc[2] = D_RGB[2] * RGB_w[2];

    float RGB_aw[3] = post_adaptation_non_linear_response_compression_forward(RGB_wc, F_L);

    float A_w = ra * RGB_aw[0] + RGB_aw[1] + ba * RGB_aw[2];

    float hr = degrees_to_radians(h);

    // No Helmholtz-Kohlrausch effect

    // Computing achromatic respons A for the stimulus
    float A = A_w * spow(J / 100.0, 1.0 / (surround[1] * z));

    // Computing P_p_1 to P_p_2
    float P_p_1 = 43.0 * surround[2];
    float P_p_2 = A;

    // Step 3 - Computing opponent colour dimensions a and b
    float gamma = M / P_p_1;
    float a = gamma * cos(hr);
    float b = gamma * sin(hr);

    // Step 4 - Applying post-adaptation non-linear response compression matrix
    float vec[3] = {P_p_2, a, b};
    float RGB_a[3] = mult_f_f3(1.0 / 1403.0, mult_f3_f33(vec, panlrcm));

    // Step 5 - Applying the inverse post-adaptation non-linear respnose compression
    float RGB_c[3] = post_adaptation_non_linear_response_compression_inverse(RGB_a, F_L);

    // Step 6
    float RGB[3];
    RGB[0] = RGB_c[0] / D_RGB[0];
    RGB[1] = RGB_c[1] / D_RGB[1];
    RGB[2] = RGB_c[2] / D_RGB[2];

    // Step 7
    float XYZ[3] = mult_f3_f33(RGB, invert_f33(MATRIX_16));

    return XYZ;
}

float Hellwig_J_to_Y(float J,
                     float surround = 0.59,
                     float L_A = 100.,
                     float Y_b = 20.)
{
    // Viewing conditions dependent parameters (could be pre-calculated)
    float k = 1.0 / (5. * L_A + 1.);
    float k4 = k * k * k * k;
    float F_L = 0.2 * k4 * (5. * L_A) + 0.1 * pow((1. - k4), 2.) * pow(5. * L_A, 1. / 3.);
    float n = Y_b / 100.;
    float z = 1.48 + sqrt(n);
    float F_L_W = pow(F_L, 0.42);
    float A_w = (400. * F_L_W) / (27.13 + F_L_W);

    float A = A_w * pow(fabs(J) / 100., 1. / (surround * z));

    return sign(J) * 100. / F_L * pow((27.13 * A) / (400.0 - A), 1. / 0.42);
}

float Y_to_Hellwig_J(float Y,
                     float surround = 0.59,
                     float L_A = 100.,
                     float Y_b = 20.)
{
    // Viewing conditions dependent parameters (could be pre-calculated)
    float k = 1.0 / (5. * L_A + 1.);
    float k4 = k * k * k * k;
    float F_L = 0.2 * k4 * (5. * L_A) + 0.1 * pow((1. - k4), 2.) * pow(5. * L_A, 1. / 3.);
    float n = Y_b / 100.;
    float z = 1.48 + sqrt(n);
    float F_L_W = pow(F_L, 0.42);
    float A_w = (400. * F_L_W) / (27.13 + F_L_W);

    float F_L_Y = pow(F_L * fabs(Y) / 100., 0.42);

    return sign(Y) * 100. * pow(((400. * F_L_Y) / (27.13 + F_L_Y)) / A_w, surround * z);
}

float[3] clamp_XYZ_to_AP1(float XYZ[3],
                          float XYZ_to_AP1_matrix[4][4],
                          float peakLuminance)
{
    float ap1[3] = mult_f3_f44(XYZ, XYZ_to_AP1_matrix);

    const float upper_clamp_limit = 8. * (128 + 768 * (log(peakLuminance/100.)/log(10000./100.))); 
    // limit to nice power of 2 (3 stops) above that needed to max out
    // note the quantity (128 + ...) is the definition of r_hit from the tonescale constants

    float ap1_clamped[3] = clamp_f3(ap1, 0., upper_clamp_limit);
    float XYZ_clamped[3] = mult_f3_f44(ap1_clamped, invert_f44(XYZ_to_AP1_matrix));

    return XYZ_clamped;
}

float[3] aces_to_JMh(float aces[3],
                     float peakLuminance)
{
    const float ACES_TO_XYZ_M[3][3] = RGBtoXYZ_f33(AP0, 1.0);

    // AP0 to XYZ
    float XYZ[3] = mult_f3_f33(aces, ACES_TO_XYZ_M);

    // Clamp to half float range
    const float AP1_XYZ_TO_RGB[4][4] = XYZtoRGB(AP1, 1.0);
    XYZ = clamp_XYZ_to_AP1(XYZ, AP1_XYZ_TO_RGB, peakLuminance);

    // XYZ to JMh
    float RGB_w[3] = {referenceLuminance, referenceLuminance, referenceLuminance};
    float XYZ_w[3] = mult_f3_f33(RGB_w, ACES_TO_XYZ_M);

    float XYZluminance[3] = mult_f_f3(referenceLuminance, XYZ);
    float JMh[3] = XYZ_to_Hellwig2022_JMh(XYZluminance, XYZ_w);

    return JMh;
}

float[3] JMh_to_aces(float JMh[3],
                     float peakLuminance)
{
    const float XYZ_TO_ACES_M[3][3] = XYZtoRGB_f33(AP0, 1.0);
    const float RGB_w[3] = {referenceLuminance, referenceLuminance, referenceLuminance};
    float XYZ_w_aces[3] = mult_f3_f33(RGB_w, invert_f33(XYZ_TO_ACES_M));

    // JMh to XYZ
    float XYZluminance[3] = Hellwig2022_JMh_to_XYZ(JMh, XYZ_w_aces);

    float XYZ[3] = mult_f_f3(1. / referenceLuminance, XYZluminance);

    // XYZ to ACES
    float ACES[3] = mult_f3_f33(XYZ, XYZ_TO_ACES_M);

    return ACES;
}

float[3] JMh_to_output_XYZ(float JMh[3],
                           ODTParams PARAMS)
{
    float XYZluminance[3] = Hellwig2022_JMh_to_XYZ(JMh,
                                                   PARAMS.XYZ_w_limit);

    float XYZ[3] = mult_f_f3(1. / referenceLuminance, XYZluminance);

    return XYZ;
}

float[3] XYZ_output_to_JMh(float XYZ[3],
                           ODTParams PARAMS)
{
    float XYZluminance[3] = mult_f_f3(referenceLuminance, XYZ);

    float JMh[3] = XYZ_to_Hellwig2022_JMh(XYZluminance,
                                          PARAMS.XYZ_w_limit);

    return JMh;
}

float compressionFunction(float v,
                          float thr,
                          float lim,
                          bool invert = false)
{
    float s = (lim - thr) * (1.0 - thr) / (lim - 1.0);
    float nd = (v - thr) / s;

    float vCompressed;

    if (invert)
    {
        if (v < thr || lim < 1.0001 || v > thr + s)
        {
            vCompressed = v;
        }
        else
        {
            vCompressed = thr + s * (-nd / (nd - 1));
        }
    }
    else
    {
        if (v < thr || lim < 1.0001)
        {
            vCompressed = v;
        }
        else
        {
            vCompressed = thr + s * nd / (1.0 + nd);
        }
    }

    return vCompressed;
}

int midpoint(int low, int high)
{
    return (low + high) / 2;
}

float[2] cuspFromTable(float h,
                       float table[][3])
{
    float lo[3];
    float hi[3];

    int low_i = 0;
    int high_i = baseIndex + gamutTableSize; // allowed as we have an extra entry in the table
    int i = hue_position_in_uniform_table(h, gamutTableSize) + baseIndex;

    while (low_i + 1 < high_i)
    {
        if (h > table[i][2])
        {
            low_i = i;
        }
        else
        {
            high_i = i;
        }
        i = midpoint(low_i, high_i);
    }
    lo = table[high_i - 1];
    hi = table[high_i];

    float t = (h - lo[2]) / (hi[2] - lo[2]);
    float cuspJ = lerp(lo[0], hi[0], t);
    float cuspM = lerp(lo[1], hi[1], t);

    float cuspJM[2] = {cuspJ, cuspM};

    return cuspJM;
}

float reachMFromTable(float h,
                      float table[])
{
    int i_lo = hue_position_in_uniform_table(h, table.size);
    int i_hi = next_position_in_table(i_lo, table.size);

    float t = (h - i_lo) / (i_hi - i_lo);

    return lerp(table[i_lo], table[i_hi], t);
}

// A "toe" function that remaps the given value x between 0 and limit.
// The k1 and k2 parameters change the size and shape of the toe.
// https://www.desmos.com/calculator/6vplvw14ti
float toe(float x,
          float limit,
          float k1_in,
          float k2_in,
          bool invert = false)
{
    if (x > limit)
        return x;

    float k2 = max(k2_in, 0.001);
    float k1 = sqrt(k1_in * k1_in + k2 * k2);
    float k3 = (limit + k1) / (limit + k2);

    if (invert)
    {
        return (x * x + k1 * x) / (k3 * (x + k2));
    }
    else
    {
        const float minus_b = k3 * x - k1;
        const float minus_c = k2 * k3 * x;
        return 0.5 * (minus_b + sqrt(minus_b * minus_b + 4. * minus_c));
    }
}

// Chroma compression
//
// Compresses colors inside the gamut with the aim for colorfulness to have an
// appropriate rate of change from display black to display white, and from
// achromatic outward to purer colors.
//
float chromaCompressionNorm(float h,
                            ODTParams PARAMS)
{

    float hr = degrees_to_radians(h);

    float a = cos(hr);
    float b = sin(hr);
    float cos_hr2 = a * a - b * b;
    float sin_hr2 = 2.0 * a * b;
    float cos_hr3 = 4.0 * a * a * a - 3.0 * a;
    float sin_hr3 = 3.0 * b - 4.0 * b * b * b;

    float M = 11.34072 * a +
              16.46899 * cos_hr2 +
              7.88380 * cos_hr3 +
              14.66441 * b +
              -6.37224 * sin_hr2 +
              9.19364 * sin_hr3 +
              77.12896;

    return M * PARAMS.chromaCompressScale;
}

// In-gamut chroma compression
//
// Compresses colors inside the gamut with the aim for colorfulness to have an
// appropriate rate of change from display black to display white, and from
// achromatic outward to purer colors.
float chromaCompression(float JMh[3],
                        float origJ,
                        ODTParams PARAMS,
                        float REACHM_TABLE[],
                        bool invert = false)
{
    float J = JMh[0];
    float M = JMh[1];
    float h = JMh[2];

    if (M == 0.0)
    {
        return M;
    }

    float nJ = J / PARAMS.limitJmax;
    float snJ = max(0., 1. - nJ);
    float Mnorm = chromaCompressionNorm(h, PARAMS);
    float limit = pow(nJ, PARAMS.model_gamma) * reachMFromTable(h, REACHM_TABLE) / Mnorm;

    float toe_limit = limit - 0.001;
    float toe_snJ_sat = snJ * PARAMS.sat;
    float toe_sqrt_nJ_sat_thr = sqrt(nJ * nJ + PARAMS.sat_thr);
    float toe_nJ_compr = nJ * PARAMS.compr;

    if (!invert) { // Forward chroma compression
        // Rescaling of M with the tonescaled J to get the M to the same range as
        // J after the tonescale.  The rescaling uses the Hellwig2022 model gamma to
        // keep the M/J ratio correct (keeping the chromaticities constant).
        M = M * pow(J / origJ, PARAMS.model_gamma);

        // Normalize M with the rendering space cusp M
        M = M / Mnorm;

        // Expand the colorfulness by running the toe function in reverse.  The goal is to
        // expand less saturated colors less and more saturated colors more.  The expansion
        // increases saturation in the shadows and mid-tones but not in the highlights.
        // The 0.001 offset starts the expansions slightly above zero.  The sat_thr makes
        // the toe less aggressive near black to reduce the expansion of noise.
        M = limit - toe(limit - M,
                        toe_limit,
                        toe_snJ_sat,
                        toe_sqrt_nJ_sat_thr,
                        false);

        // Compress the colorfulness.  The goal is to compress less saturated colors more and
        // more saturated colors less, especially in the highlights.  This step creates the
        // saturation roll-off in the highlights, but attemps to preserve pure colors.  This
        // mostly affects highlights and mid-tones, and does not compress shadows.
        M = toe(M,
                limit,
                toe_nJ_compr,
                snJ,
                false);

        // Denormalize
        M = M * Mnorm;
    }
    else
    {
        M = M / Mnorm;
        M = toe(M,
                limit,
                toe_nJ_compr,
                snJ,
                true);
        M = limit - toe(limit - M,
                        toe_limit,
                        toe_snJ_sat,
                        toe_sqrt_nJ_sat_thr,
                        true);
        M = M * Mnorm;
        M = M * pow(J / origJ, -PARAMS.model_gamma);
    }

    return M;
}

float[3] tonemapAndCompress_fwd(float inputJMh[3],
                                ODTParams PARAMS,
                                float REACHM_TABLE[])
{
    float outputJMh[3];

    float linear = Hellwig_J_to_Y(inputJMh[0]) / referenceLuminance;

    TSParams TSPARAMS = init_TSParams(PARAMS.peakLuminance);
    float luminanceTS = tonescale_fwd(linear, TSPARAMS);

    float tonemappedJ = Y_to_Hellwig_J(luminanceTS);

    float tonemappedJMh[3] = {tonemappedJ, inputJMh[1], inputJMh[2]};

    outputJMh = tonemappedJMh;
    outputJMh[1] = chromaCompression(outputJMh,
                                     inputJMh[0],
                                     PARAMS,
                                     REACHM_TABLE,
                                     false);

    return outputJMh;
}

float[3] tonemapAndCompress_inv(float JMh[3],
                                ODTParams PARAMS,
                                float REACHM_TABLE[])
{
    float tonemappedJMh[3] = JMh;

    float luminance = Hellwig_J_to_Y(JMh[0]);

    TSParams TSPARAMS = init_TSParams(PARAMS.peakLuminance);
    float linear = tonescale_inv(luminance / referenceLuminance,
                                 TSPARAMS);

    float untonemappedJ = Y_to_Hellwig_J(linear * referenceLuminance);
    float untonemappedColorJMh[3] = {untonemappedJ, tonemappedJMh[1], tonemappedJMh[2]};

    // Chroma compression
    untonemappedColorJMh[1] = chromaCompression(tonemappedJMh,
                                                untonemappedColorJMh[0],
                                                PARAMS,
                                                REACHM_TABLE,
                                                true);

    return untonemappedColorJMh;
}

float solve_J_intersect(float J,
                        float M,
                        float focusJ,
                        float maxJ,
                        float slope_gain)
{
    float a = M / (focusJ * slope_gain);
    float b = 0.0;
    float c = 0.0;
    float intersectJ = 0.0;

    if (J < focusJ)
    {
        b = 1.0 - M / slope_gain;
    }
    else
    {
        b = -(1.0 + M / slope_gain + maxJ * M / (focusJ * slope_gain));
    }

    if (J < focusJ)
    {
        c = -J;
    }
    else
    {
        c = maxJ * M / slope_gain + J;
    }

    float root = sqrt(b * b - 4.0 * a * c);

    if (J < focusJ)
    {
        intersectJ = 2.0 * c / (-b - root);
    }
    else
    {
        intersectJ = 2.0 * c / (-b + root);
    }

    return intersectJ;
}

float[3] findGamutBoundaryIntersection(float JMh_s[3],
                                       float JM_cusp_in[2],
                                       float J_focus,
                                       float J_max,
                                       float slope_gain,
                                       float gamma_top,
                                       float gamma_bottom)
{
    float slope = 0.0;

    float s = max(0.000001, smoothCusps);
    float JM_cusp[2] = JM_cusp_in;
    JM_cusp[1] = JM_cusp_in[1] * (1.0 + smoothM * s); // M

    float J_intersect_source = solve_J_intersect(JMh_s[0],
                                                 JMh_s[1],
                                                 J_focus,
                                                 J_max,
                                                 slope_gain);
    float J_intersect_cusp = solve_J_intersect(JM_cusp[0],
                                               JM_cusp[1],
                                               J_focus,
                                               J_max,
                                               slope_gain);

    if (J_intersect_source < J_focus)
    {
        slope = J_intersect_source * (J_intersect_source - J_focus) / (J_focus * slope_gain);
    }
    else
    {
        slope = (J_max - J_intersect_source) * (J_intersect_source - J_focus) / (J_focus * slope_gain);
    }

    float M_boundary_lower = J_intersect_cusp * pow(J_intersect_source / J_intersect_cusp, 1. / gamma_bottom) / (JM_cusp[0] / JM_cusp[1] - slope);

    float M_boundary_upper = JM_cusp[1] * (J_max - J_intersect_cusp) * pow((J_max - J_intersect_source) / (J_max - J_intersect_cusp), 1. / gamma_top) / (slope * JM_cusp[1] + J_max - JM_cusp[0]);

    float M_boundary = JM_cusp[1] * smin(M_boundary_lower / JM_cusp[1], M_boundary_upper / JM_cusp[1], s);

    float J_boundary = J_intersect_source + slope * M_boundary;

    float return_JMh[3] = {J_boundary, M_boundary, J_intersect_source};

    return return_JMh;
}

float hueDependentUpperHullGamma(float h,
                                 float gamma_table[])
{
    const int i_lo = hue_position_in_uniform_table(h, gamutTableSize) + baseIndex;
    const int i_hi = next_position_in_table(i_lo, gamma_table.size);

    const float base_hue = base_hue_for_position(i_lo - baseIndex, gamutTableSize);

    const float t = wrap_to_360(h) - base_hue;

    return lerp(gamma_table[i_lo], gamma_table[i_hi], t);
}

float getFocusGain(float J,
                   float cuspJ,
                   float limitJmax)
{

    float thr = lerp(cuspJ, limitJmax, focusGainBlend);
    if (J > thr)
    {
        // Approximate inverse required above threshold
        float gain = (limitJmax - thr) / max(0.0001, (limitJmax - min(limitJmax, J)));
        return pow(log10(gain), 1. / focusAdjustGain) + 1.;
    }
    else
    {
        // Analytic inverse possible below cusp
        return 1.;
    }
}

float[3] getReachBoundary(float J,
                          float M,
                          float h,
                          ODTParams PARAMS,
                          float JMcusp[2],
                          float focusJ,
                          float reachTable[])
{
    float limitJmax = PARAMS.limitJmax;
    float midJ = PARAMS.midJ;
    float model_gamma = PARAMS.model_gamma;
    float focusDist = PARAMS.focusDist;

    const float reachMaxM = reachMFromTable(h, reachTable);

    float slope_gain = limitJmax * focusDist * getFocusGain(J, JMcusp[0], limitJmax);

    float intersectJ = solve_J_intersect(J, M, focusJ, limitJmax, slope_gain);
    float slope;
    if (intersectJ < focusJ)
    {
        slope = intersectJ * (intersectJ - focusJ) / (focusJ * slope_gain);
    }
    else
    {
        slope = (limitJmax - intersectJ) * (intersectJ - focusJ) / (focusJ * slope_gain);
    }
    float boundary = limitJmax * pow(intersectJ / limitJmax, model_gamma) * reachMaxM / (limitJmax - slope * reachMaxM);
    float result[3] = {J, boundary, h};
    return result;
}

float[3] compressGamut(float JMh[3],
                       ODTParams PARAMS,
                       float Jx,
                       float gamutCuspTable[][3],
                       float gamutTopGamma[],
                       float reachTable[],
                       bool invert = false)
{
    float limitJmax = PARAMS.limitJmax;
    float midJ = PARAMS.midJ;
    float focusDist = PARAMS.focusDist;
    float model_gamma = PARAMS.model_gamma;

    float project_from[2] = {JMh[0], JMh[1]};
    float JMcusp[2] = cuspFromTable(JMh[2], gamutCuspTable);

    if (JMh[1] < 0.0001 || JMh[0] > limitJmax)
    {
        float JMh_return[3] = {JMh[0], 0.0, JMh[2]};
        return JMh_return;
    }

    // Calculate where the out of gamut color is projected to
    float focusJ = lerp(JMcusp[0], midJ, min(1., cuspMidBlend - (JMcusp[0] / limitJmax)));

    float slope_gain = limitJmax * focusDist * getFocusGain(Jx, JMcusp[0], limitJmax);

    // Find gamut intersection
    float gamma_top = hueDependentUpperHullGamma(JMh[2], gamutTopGamma);
    float gamma_bottom = PARAMS.lowerHullGamma;

    float boundaryReturn[3] = findGamutBoundaryIntersection(JMh,
                                                            JMcusp,
                                                            focusJ,
                                                            limitJmax,
                                                            slope_gain,
                                                            gamma_top,
                                                            gamma_bottom);

    float JMboundary[2] = {boundaryReturn[0], boundaryReturn[1]};
    float project_to[2] = {boundaryReturn[2], 0.0};
    float projectJ = boundaryReturn[2];

    // Calculate AP1 reach boundary
    float reachBoundary[3] = getReachBoundary(JMboundary[0],
                                              JMboundary[1],
                                              JMh[2],
                                              PARAMS,
                                              JMcusp,
                                              focusJ,
                                              reachTable);

    float difference = max(1.0001, reachBoundary[1] / JMboundary[1]);
    float threshold = max(compressionThreshold, 1. / difference);

    // Compress the out of gamut color along the projection line
    float v = project_from[1] / JMboundary[1];

    v = compressionFunction(v,
                            threshold,
                            difference,
                            invert);

    float JMcompressed[2];
    JMcompressed[0] = project_to[0] + v * (JMboundary[0] - project_to[0]);
    JMcompressed[1] = project_to[1] + v * (JMboundary[1] - project_to[1]);

    float return_JMh[3] = {JMcompressed[0], JMcompressed[1], JMh[2]};

    return return_JMh;
}

float[3] gamutMap_fwd(float JMh[3],
                      ODTParams PARAMS,
                      float gamutCuspTable[][3],
                      float gamutTopGamma[],
                      float reachTable[])
{
    return compressGamut(JMh,
                         PARAMS,
                         JMh[0],
                         gamutCuspTable,
                         gamutTopGamma,
                         reachTable,
                         false);
}

float[3] gamutMap_inv(float JMh[3],
                      ODTParams PARAMS,
                      float gamutCuspTable[][3],
                      float gamutTopGamma[],
                      float reachTable[])
{
    float JMcusp[2] = cuspFromTable(JMh[2], gamutCuspTable);
    float Jx = JMh[0];

    // Analytic inverse below threshold
    if (Jx <= lerp(JMcusp[0], PARAMS.limitJmax, focusGainBlend))
        return compressGamut(JMh,
                             PARAMS,
                             Jx,
                             gamutCuspTable,
                             gamutTopGamma,
                             reachTable,
                             true);

    // Approximation above threshold
    Jx = compressGamut(JMh,
                       PARAMS,
                       Jx,
                       gamutCuspTable,
                       gamutTopGamma,
                       reachTable,
                       true)[0];
    return compressGamut(JMh,
                         PARAMS,
                         Jx,
                         gamutCuspTable,
                         gamutTopGamma,
                         reachTable,
                         true);
}

float[3] RGB_to_JMh(float RGB[3],
                    float RGB_TO_XYZ_M[3][3],
                    float peakLuminance)
{
    float luminanceRGB[3] = mult_f_f3(peakLuminance, RGB); // Scale factor is technically equal to [(peakLuminance / referenceLuminance) * referenceLuminance] but referenceLuminance cancels out, so just multiply by peakLuminance
    float XYZ[3] = mult_f3_f33(luminanceRGB, RGB_TO_XYZ_M);

    float RGB_w[3] = {referenceLuminance, referenceLuminance, referenceLuminance};
    float XYZ_w[3] = mult_f3_f33(RGB_w, RGB_TO_XYZ_M);

    float JMh[3] = XYZ_to_Hellwig2022_JMh(XYZ, XYZ_w);

    return JMh;
}

float[3] JMh_to_RGB(float JMh[3],
                    float XYZ_TO_RGB_M[3][3],
                    float peakLuminance)
{
    float RGB_w[3] = {referenceLuminance, referenceLuminance, referenceLuminance};
    float XYZ_w[3] = mult_f3_f33(RGB_w, invert_f33(XYZ_TO_RGB_M));

    float luminanceXYZ[3] = Hellwig2022_JMh_to_XYZ(JMh, XYZ_w);

    float luminanceRGB[3] = mult_f3_f33(luminanceXYZ, XYZ_TO_RGB_M);
    float RGB[3] = mult_f_f3(1. / peakLuminance, luminanceRGB); // Scale factor is technically equal to [1. / (peakLuminance/referenceLuminance) / referenceLuminance] and referenceLuminance cancels out, so just divide by peakLuminance

    return RGB;
}

// Table building functions
bool any_below_zero(float newLimitRGB[3])
{
    return (newLimitRGB[0] < 0. || newLimitRGB[1] < 0. || newLimitRGB[2] < 0.);
}

float[totalTableSize][3] make_gamut_table(Chromaticities C,
                                          float peakLuminance)
{
    const float RGB_TO_XYZ_M[3][3] = RGBtoXYZ_f33(C, 1.0);

    float gamutCuspTableUnsorted[gamutTableSize][3];

    int i;
    for (i = 0; i < gamutTableSize; i = i + 1)
    {
        float i_float = i;
        float hNorm = i_float / gamutCuspTableUnsorted.size;

        float HSV[3] = {hNorm, 1., 1.};
        float RGB[3] = HSV_to_RGB(HSV);

        gamutCuspTableUnsorted[i] = RGB_to_JMh(RGB,
                                               RGB_TO_XYZ_M,
                                               peakLuminance);
    }

    int minhIndex = 0;
    for (i = 0; i < gamutTableSize; i = i + 1)
    {
        if (gamutCuspTableUnsorted[i][2] < gamutCuspTableUnsorted[minhIndex][2])
            minhIndex = i;
    }

    float gamutCuspTable[totalTableSize][3]; // allocate 2 extra entries to ease the code to handle hues wrapping around
    for (i = 0; i < gamutTableSize; i = i + 1)
    {
        gamutCuspTable[i + baseIndex] = gamutCuspTableUnsorted[(minhIndex + i) % gamutTableSize];
    }

    // Copy last populated entry to first empty spot
    gamutCuspTable[0] = gamutCuspTable[baseIndex + gamutTableSize - 1];

    // Copy first populated entry to last empty spot
    gamutCuspTable[baseIndex + gamutTableSize] = gamutCuspTable[baseIndex];

    // Wrap the hues, to maintain monotonicity. These entries will fall outside [0.0, 360.0]
    gamutCuspTable[0][2] = gamutCuspTable[0][2] - 360.0;
    gamutCuspTable[gamutTableSize + 1][2] = gamutCuspTable[gamutTableSize + 1][2] + 360.0;

    return gamutCuspTable;
}

// Finds reach gamut M value at limitJmax
float[gamutTableSize] make_reachM_table(Chromaticities C,
                                        float limitJmax,
                                        float peakLuminance)
{
    const float XYZ_TO_RGB_M[3][3] = XYZtoRGB_f33(C, 1.0);

    float reachTable[gamutTableSize];

    int i;
    for (i = 0; i < gamutTableSize; i = i + 1)
    {
        float i_float = i;
        float hue = base_hue_for_position(i, gamutTableSize);

        float search_range = 50.;
        float low = 0.;
        float high = low + search_range;
        bool outside = false;

        while ((outside != true) & (high < 1300.))
        {
            float searchJMh[3] = {limitJmax, high, hue};
            float newLimitRGB[3] = JMh_to_RGB(searchJMh,
                                              XYZ_TO_RGB_M,
                                              peakLuminance);
            outside = any_below_zero(newLimitRGB);
            if (outside == false)
            {
                low = high;
                high = high + search_range;
            }
        }

        while (high - low > 1e-2)
        {
            float sampleM = (high + low) / 2.;
            float searchJMh[3] = {limitJmax, sampleM, hue};
            float newLimitRGB[3] = JMh_to_RGB(searchJMh,
                                              XYZ_TO_RGB_M,
                                              peakLuminance);
            outside = any_below_zero(newLimitRGB);
            if (outside)
            {
                high = sampleM;
            }
            else
            {
                low = sampleM;
            }
        }

        reachTable[i] = high;
    }

    return reachTable;
}

bool outside_hull(float newLimitRGB[3])
{
    // limit value, once we cross this value, we are outside of the top gamut shell
    float maxRGBtestVal = 1.0;

    return newLimitRGB[0] > maxRGBtestVal || newLimitRGB[1] > maxRGBtestVal || newLimitRGB[2] > maxRGBtestVal;
}

bool evaluate_gamma_fit(float JMcusp[2],
                        float testJMh[][3],
                        ODTParams PARAMS,
                        float topGamma,
                        float peakLuminance)
{
    float midJ = PARAMS.midJ;
    float limitJmax = PARAMS.limitJmax;
    float lowerHullGamma = PARAMS.lowerHullGamma;
    float focusDist = PARAMS.focusDist;

    float focusJ = lerp(JMcusp[0], midJ, min(1.0, cuspMidBlend - (JMcusp[0] / limitJmax)));

    for (int testIndex = 0; testIndex < testJMh.size; testIndex = testIndex + 1)
    {
        float slope_gain = limitJmax * focusDist * getFocusGain(testJMh[testIndex][0], JMcusp[0], limitJmax);
        float approxLimit[3] = findGamutBoundaryIntersection(testJMh[testIndex],
                                                             JMcusp,
                                                             focusJ,
                                                             limitJmax,
                                                             slope_gain,
                                                             topGamma,
                                                             lowerHullGamma);
        float approximate_JMh[3] = {approxLimit[0], approxLimit[1], testJMh[testIndex][2]};

        float newLimitRGB[3] = JMh_to_RGB(approximate_JMh,
                                          PARAMS.LIMIT_XYZ_TO_RGB,
                                          peakLuminance);

        if (!outside_hull(newLimitRGB))
        {
            return false;
        }
    }
    return true;
}

float[totalTableSize] make_upper_hull_gamma_table(float gamutCuspTable[][3],
                                                  ODTParams PARAMS,
                                                  float peakLuminance)
{
    // Find upper hull gamma values for the gamut mapper.
    // Start by taking a h angle
    // Get the cusp J value for that angle
    // Find a J value halfway to the Jmax
    // Iterate through gamma values until the approximate max M is
    // negative through the actual boundary

    // positions between the cusp and Jmax we will check variables that get
    // set as we iterate through, once all are set to true we break the loop

    const int test_count = 3;
    float testPositions[test_count] = {0.01, 0.5, 0.99};
    float gammaTable[gamutTableSize];
    float gamutTopGamma[totalTableSize];

    for (int i = 0; i < gamutTableSize; i = i + 1)
    {
        // Default value. This will get overridden as we loop,
        // but can be a good diagnostic to make sure things are working
        gammaTable[i] = -1.;

        // get cusp from cusp table at hue position
        float hue = base_hue_for_position(i, gamutTableSize);
        float JMcusp[2] = cuspFromTable(hue, gamutCuspTable);

        float testJMh[test_count][3];
        // create test values halfway between the cusp and the Jmax
        for (int testIndex = 0; testIndex < testJMh.size; testIndex = testIndex + 1)
        {
            float testJ = JMcusp[0] + ((PARAMS.limitJmax - JMcusp[0]) * testPositions[testIndex]);
            testJMh[testIndex][0] = testJ;
            testJMh[testIndex][1] = JMcusp[1];
            testJMh[testIndex][2] = hue;
        }

        float search_range = gammaSearchStep;
        float low = gammaMinimum;
        float high = low + search_range;
        bool outside = false;

        while (!(outside) && (high < 5.0))
        {
            bool gammaFound = evaluate_gamma_fit(JMcusp,
                                                 testJMh,
                                                 PARAMS,
                                                 high,
                                                 peakLuminance);
            if (!gammaFound)
            {
                low = high;
                high = high + search_range;
            }
            else
            {
                outside = true;
            }
        }

        float testGamma = -1.0;
        while ((high - low) > gammaAccuracy)
        {
            testGamma = (high + low) / 2.;
            bool gammaFound = evaluate_gamma_fit(JMcusp,
                                                 testJMh,
                                                 PARAMS,
                                                 testGamma,
                                                 peakLuminance);
            if (gammaFound)
            {
                high = testGamma;
                gammaTable[i] = high;
            }
            else
            {
                low = testGamma;
            }
        }

        if (gammaTable[i] < 0.)
        {
            print("Did not find top gamma for hue: ", hue, "\n");
        }

        // Duplicate gamma value to array, leaving empty entries at first and last position
        gamutTopGamma[i + baseIndex] = gammaTable[i];
    }

    // Copy last populated entry to first empty spot
    gamutTopGamma[0] = gammaTable[gamutTableSize - 1];

    // Copy first populated entry to last empty spot
    gamutTopGamma[totalTableSize - 1] = gammaTable[0];

    return gamutTopGamma;
}

ODTParams init_ODTParams(
    float peakLuminance,
    Chromaticities limitingPrimaries,
    Chromaticities encodingPrimaries,
    float viewingConditions = 1 // 0 = "dark"; 1 = "dim"; 2 = "average"
)
{
    TSParams TSPARAMS = init_TSParams(peakLuminance);

    float limitJmax = Y_to_Hellwig_J(peakLuminance);
    float midJ = Y_to_Hellwig_J(TSPARAMS.c_t * 100.);

    const Chromaticities INPUT_PRI = // equal to ACES "AP0" primaries
        {
            {0.73470, 0.26530},
            {0.00000, 1.00000},
            {0.00010, -0.07700},
            {0.32168, 0.33767}};

    // Chroma compress presets
    const float chroma_compress = 2.4;
    const float chroma_compress_fact = 3.3;
    const float chroma_expand = 1.3;
    const float chroma_expand_fact = 0.69;
    const float chroma_expand_thr = 0.5;

    // Calculated chroma compress variables
    const float log_peak = log10(TSPARAMS.n / TSPARAMS.n_r);
    const float compr = chroma_compress + (chroma_compress * chroma_compress_fact) * log_peak;
    const float sat = max(0.2, chroma_expand - (chroma_expand * chroma_expand_fact) * log_peak);
    const float sat_thr = chroma_expand_thr / TSPARAMS.n;
    const float chromaCompressScale = pow(0.03379 * peakLuminance, 0.30596) - 0.45135;

    const float surround[3] = viewingConditionsToSurround(viewingConditions);
    const float model_gamma = 1. / (surround[1] * (1.48 + sqrt(Y_b / L_A)));

    const float focusDist = focusDistance +
                            focusDistance * focusDistanceScaling * log_peak;

    const float RGB_w[3] = {100., 100., 100.};

    // Input Primaries (AP0)
    const float INPUT_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33(INPUT_PRI, 1.0);
    const float INPUT_XYZ_TO_RGB[3][3] = XYZtoRGB_f33(INPUT_PRI, 1.0);
    float XYZ_w_in[3] = mult_f3_f33(RGB_w, INPUT_RGB_TO_XYZ);

    const float lowerHullGamma = 1.14;

    // Limiting Primaries
    const float LIMIT_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33(limitingPrimaries, 1.0);
    const float LIMIT_XYZ_TO_RGB[3][3] = XYZtoRGB_f33(limitingPrimaries, 1.0);
    float XYZ_w_limit[3] = mult_f3_f33(RGB_w, LIMIT_RGB_TO_XYZ);

    // Output / Encoding Primaries
    const float OUTPUT_RGB_TO_XYZ[3][3] = RGBtoXYZ_f33(encodingPrimaries, 1.0);
    const float OUTPUT_XYZ_TO_RGB[3][3] = XYZtoRGB_f33(encodingPrimaries, 1.0);
    float XYZ_w_output[3] = mult_f3_f33(RGB_w, OUTPUT_RGB_TO_XYZ);

    const ODTParams ODTPARAMS = {
        peakLuminance,

        // Tonescale
        TSPARAMS.n_r, // normalized white in nits (what 1.0 should be)
        TSPARAMS.g,   // surround / contrast
        TSPARAMS.t_1,
        TSPARAMS.c_t,
        TSPARAMS.s_2,
        TSPARAMS.u_2,
        TSPARAMS.m_2,

        // Chroma Compression
        limitJmax,
        midJ,
        model_gamma,
        sat,
        sat_thr,
        compr,
        chromaCompressScale,

        focusDist,

        // Limit
        {{LIMIT_RGB_TO_XYZ[0][0], LIMIT_RGB_TO_XYZ[0][1], LIMIT_RGB_TO_XYZ[0][2]},
         {LIMIT_RGB_TO_XYZ[1][0], LIMIT_RGB_TO_XYZ[1][1], LIMIT_RGB_TO_XYZ[1][2]},
         {LIMIT_RGB_TO_XYZ[2][0], LIMIT_RGB_TO_XYZ[2][1], LIMIT_RGB_TO_XYZ[2][2]}},
        {{LIMIT_XYZ_TO_RGB[0][0], LIMIT_XYZ_TO_RGB[0][1], LIMIT_XYZ_TO_RGB[0][2]},
         {LIMIT_XYZ_TO_RGB[1][0], LIMIT_XYZ_TO_RGB[1][1], LIMIT_XYZ_TO_RGB[1][2]},
         {LIMIT_XYZ_TO_RGB[2][0], LIMIT_XYZ_TO_RGB[2][1], LIMIT_XYZ_TO_RGB[2][2]}},
        {XYZ_w_limit[0], XYZ_w_limit[1], XYZ_w_limit[2]},

        // Output
        {{OUTPUT_RGB_TO_XYZ[0][0], OUTPUT_RGB_TO_XYZ[0][1], OUTPUT_RGB_TO_XYZ[0][2]},
         {OUTPUT_RGB_TO_XYZ[1][0], OUTPUT_RGB_TO_XYZ[1][1], OUTPUT_RGB_TO_XYZ[1][2]},
         {OUTPUT_RGB_TO_XYZ[2][0], OUTPUT_RGB_TO_XYZ[2][1], OUTPUT_RGB_TO_XYZ[2][2]}},
        {{OUTPUT_XYZ_TO_RGB[0][0], OUTPUT_XYZ_TO_RGB[0][1], OUTPUT_XYZ_TO_RGB[0][2]},
         {OUTPUT_XYZ_TO_RGB[1][0], OUTPUT_XYZ_TO_RGB[1][1], OUTPUT_XYZ_TO_RGB[1][2]},
         {OUTPUT_XYZ_TO_RGB[2][0], OUTPUT_XYZ_TO_RGB[2][1], OUTPUT_XYZ_TO_RGB[2][2]}},
        {XYZ_w_output[0], XYZ_w_output[1], XYZ_w_output[2]},

        lowerHullGamma // lowerHullGamma
    };

    return ODTPARAMS;
}

float[3] outputTransform_fwd(float aces[3],
                             float peakLuminance,
                             ODTParams PARAMS,
                             Chromaticities limitingPri,
                             float GAMUT_CUSP_TABLE[][3],
                             float GAMUT_TOP_GAMMA[],
                             float REACHM_TABLE[])
{
    float JMh[3] = aces_to_JMh(aces,
                               peakLuminance);

    float tonemappedJMh[3] = tonemapAndCompress_fwd(JMh,
                                                    PARAMS,
                                                    REACHM_TABLE);

    float compressedJMh[3] = gamutMap_fwd(tonemappedJMh,
                                          PARAMS,
                                          GAMUT_CUSP_TABLE,
                                          GAMUT_TOP_GAMMA,
                                          REACHM_TABLE);

    float XYZ[3] = JMh_to_output_XYZ(compressedJMh,
                                     PARAMS);

    return XYZ;
}

float[3] outputTransform_inv(float XYZ[3],
                             float peakLuminance,
                             ODTParams PARAMS,
                             Chromaticities limitingPri,
                             float GAMUT_CUSP_TABLE[][3],
                             float GAMUT_TOP_GAMMA[],
                             float REACHM_TABLE[])
{
    float compressedJMh[3] = XYZ_output_to_JMh(XYZ,
                                               PARAMS);

    float tonemappedJMh[3] = gamutMap_inv(compressedJMh,
                                          PARAMS,
                                          GAMUT_CUSP_TABLE,
                                          GAMUT_TOP_GAMMA,
                                          REACHM_TABLE);

    float JMh[3] = tonemapAndCompress_inv(tonemappedJMh,
                                          PARAMS,
                                          REACHM_TABLE);

    float aces[3] = JMh_to_aces(JMh,
                                peakLuminance);

    return aces;
}