
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_VLog_VGamut.a1.1.0</ACEStransformID>
// <ACESuserName>ACES2065-1 to Panasonic Varicam V-Log V-Gamut</ACESuserName>


import "ACESlib.Utilities_Color";


const float AP0_2_VGAMUT_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( AP0, 
                                                     PANASONIC_VGAMUT_PRI );


const float cut1 = 0.01;
const float b = 0.00873;
const float c = 0.241514;
const float d = 0.598206;

float lin_to_VLog( input varying float in)
{
    if ( in < cut1 )
    {
        return 5.6 * in + 0.125;
    }
    else
    {
        return c * log10(in + b) + d;
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
    float ACES[3] = { rIn, gIn, bIn};

    float lin_VGamut[3] = mult_f3_f33( ACES, AP0_2_VGAMUT_MAT);

    rOut = lin_to_VLog( lin_VGamut[0]);
    gOut = lin_to_VLog( lin_VGamut[1]);
    bOut = lin_to_VLog( lin_VGamut[2]);
    aOut = aIn;
}