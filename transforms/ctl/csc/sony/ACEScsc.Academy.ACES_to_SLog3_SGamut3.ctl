
// <ACEStransformID>ACEScsc.ACES_to_SLog3_SGamut3.a1.v1</ACEStransformID>
// <ACESuserName>ACES2065-1 to S-Log3 S-Gamut3</ACESuserName>


import "ACESlib.Utilities_Color";


const float AP0_2_SGAMUT3_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( AP0, 
                                                     SONY_SGAMUT3_PRI, 
                                                     CONE_RESP_MAT_CAT02);


float lin_to_SLog3( input varying float in)
{
    float out;
    if ( in >= 0.01125000 )
    {
        out = (420.0 + log10((in + 0.01) / (0.18 + 0.01)) * 261.5) / 1023.0;
    }
    else
    {
        out = (in * (171.2102946929 - 95.0) / 0.01125000 + 95.0) / 1023.0;
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
    float ACES[3] = { rIn, gIn, bIn};

    float lin_SGamut3[3] = mult_f3_f33( ACES, AP0_2_SGAMUT3_MAT);

    rOut = lin_to_SLog3( lin_SGamut3[0]);
    gOut = lin_to_SLog3( lin_SGamut3[1]);
    bOut = lin_to_SLog3( lin_SGamut3[2]);
    aOut = aIn;
}