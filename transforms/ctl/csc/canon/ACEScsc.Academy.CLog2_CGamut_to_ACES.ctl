
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.CLog2_CGamut_to_ACES.a1.1.0</ACEStransformID>
// <ACESuserName>Canon Log 2 Cinema Gamut to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - Canon Log 2 Cinema Gamut to ACES
//
// converts Canon Log 2 Cinema Gamut to
//          ACES2065-1 (AP0 w/ linear encoding)
//



import "ACESlib.Utilities_Color";


const float CGAMUT_2_AP0_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( CANON_CGAMUT_PRI, 
                                                     AP0, 
                                                     CONE_RESP_MAT_CAT02 );


float CLog2_to_lin( input varying float in)
{
    float out;
    if ( in < 0.092864125 )
    {
        out = -(pow(10, (0.092864125 - in) / 0.24136077) - 1) / 87.099375;
    }
    else
    {
        out = (pow(10, (in - 0.092864125) / 0.24136077) - 1) / 87.099375;
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
    float lin_CGamut[3];
    lin_CGamut[0] = 0.9 * CLog2_to_lin( rIn);
    lin_CGamut[1] = 0.9 * CLog2_to_lin( gIn);
    lin_CGamut[2] = 0.9 * CLog2_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_CGamut, CGAMUT_2_AP0_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}
