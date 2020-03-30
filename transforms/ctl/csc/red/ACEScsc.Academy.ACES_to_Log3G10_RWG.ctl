
// <ACEStransformID>ACEScsc.ACES_to_Log3G10_RWG.a1.v1</ACEStransformID>
// <ACESuserName>ACES2065-1 to Log3G10 RWG</ACESuserName>

//
// ACES Color Space Conversion - ACES to RED Log3G10 RWG
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          RED Log3G10, RED Wide Gamut
//


import "ACESlib.Utilities_Color";


const float AP0_2_RWG_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( AP0, 
                                                     RED_WIDEGAMUTRGB_PRI);


const float a = 0.224282;
const float b = 155.975327;
const float c = 0.01;
const float g = 15.1927;

float lin_to_Log3G10( input varying float in)
{
    float out = in + c;
    if (out < 0.0)
    {
        out =  out * g;
    }
    else
    {
        out = a * log10((out * b) + 1.0);
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

    float lin_RWG[3] = mult_f3_f33( ACES, AP0_2_RWG_MAT);

    rOut = lin_to_Log3G10( lin_RWG[0]);
    gOut = lin_to_Log3G10( lin_RWG[1]);
    bOut = lin_to_Log3G10( lin_RWG[2]);
    aOut = aIn;
}