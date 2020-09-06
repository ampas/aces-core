
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_CLog3_CGamut.a1.1.0</ACEStransformID>
// <ACESuserName>ACES2065-1 to Canon Log 3 Cinema Gamut</ACESuserName>


import "ACESlib.Utilities_Color";


const float AP0_2_CGAMUT_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( AP0, 
                                                     CANON_CGAMUT_PRI, 
                                                     CONE_RESP_MAT_CAT02 );


float lin_to_CLog3( input varying float in)
{
    float out;
    if ( in < -0.014 )
    {
        out = -0.36726845 * log10(1 - 14.98325 * in) + 0.12783901;
    }
    else if ( in <= 0.014 )
    {
        out = 1.9754798 * in + 0.12512219;
    }
    else
    {
        out = 0.36726845 * log10(14.98325 * in + 1) + 0.12240537;
    }
    return out;
}



void main
(   input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
    float ACES[3] = { rIn, gIn, bIn};

    float lin_CGamut[3] = mult_f3_f33( ACES, AP0_2_CGAMUT_MAT);

    rOut = lin_to_CLog3( lin_CGamut[0] / 0.9);
    gOut = lin_to_CLog3( lin_CGamut[1] / 0.9);
    bOut = lin_to_CLog3( lin_CGamut[2] / 0.9);
    aOut = aIn;
}
