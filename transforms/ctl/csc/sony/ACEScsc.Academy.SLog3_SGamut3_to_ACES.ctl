
// <ACEStransformID>ACEScsc.SLog3_SGamut3_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>SLog3 SGamut3 to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - S-Log3 S-Gamut3 to ACES
//
// converts S-Log3, S-Gamut3 to 
//          ACES2065-1 (AP0 w/ linear encoding)
//


import "ACESlib.Utilities_Color";


const float SGAMUT3_2_AP0_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( SONY_SGAMUT3_PRI, 
                                                     AP0, 
                                                     CONE_RESP_MAT_CAT02);


float SLog3_to_lin( input varying float in)
{
    float out;
    if ( in >= 171.2102946929 / 1023.0 )
    {
        out = pow(10.0, (in * 1023.0 - 420.0) / 261.5) * (0.18 + 0.01) - 0.01;
    }
    else
    {
        out = (in * 1023.0 - 95.0) * 0.01125000 / (171.2102946929 - 95.0);
    }
    return out;
}



void main
(   
    input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
    float lin_SGamut3[3];
    lin_SGamut3[0] = SLog3_to_lin( rIn);
    lin_SGamut3[1] = SLog3_to_lin( gIn);
    lin_SGamut3[2] = SLog3_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_SGamut3, SGAMUT3_2_AP0_MAT);
  
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}