
// <ACEStransformID>ACEScsc.VLog_VGamut_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>Panasonic Varicam V-Log V-Gamut to ACES2065-1</ACESuserName>


import "ACESlib.Utilities_Color";


const float VGAMUT_2_AP0_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( PANASONIC_VGAMUT_PRI, 
                                                     AP0,
                                                     CONE_RESP_MAT_CAT02 );


const float cut2 = 0.181;
const float b = 0.00873;
const float c = 0.241514;
const float d = 0.598206;

float VLog_to_lin( input varying float in)
{
    if ( in < cut2 )
    {
        return (in - 0.125)/5.6;
    }
    else
    {
        return pow(10.0, ((in - d) / c)) - b;
    }
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
    float lin_VGamut[3];
    lin_VGamut[0] = VLog_to_lin( rIn);
    lin_VGamut[1] = VLog_to_lin( gIn);
    lin_VGamut[2] = VLog_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_VGamut, VGAMUT_2_AP0_MAT);
  
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}