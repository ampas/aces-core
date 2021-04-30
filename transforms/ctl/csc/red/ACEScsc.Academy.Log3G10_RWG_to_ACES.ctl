
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.Log3G10_RWG_to_ACES.a1.1.0</ACEStransformID>
// <ACESuserName>RED Log3G10 REDWideGamutRGB to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - RED Log3G10 REDWideGamutRGB to ACES
//
// converts RED Log3G10 REDWideGamutRGB to
//          ACES2065-1 (AP0 w/ linear encoding)
//



import "ACESlib.Utilities_Color";


const float RWG_2_AP0_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( RED_WIDEGAMUTRGB_PRI, 
                                                     AP0);


const float a = 0.224282;
const float b = 155.975327;
const float c = 0.01;
const float g = 15.1927;

float Log3G10_to_lin( input varying float in)
{
    float out;
    if ( in < 0.0 )
    {
        out = (in / g);
    }
    else
    {
        out = (pow(10.0, in / a) - 1.0) / b;
    }
    return out - c;
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
    float lin_RWG[3];
    lin_RWG[0] = Log3G10_to_lin( rIn);
    lin_RWG[1] = Log3G10_to_lin( gIn);
    lin_RWG[2] = Log3G10_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_RWG, RWG_2_AP0_MAT);
  
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}