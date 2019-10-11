
// <ACEStransformID>ACEScsc.Log3G10_RWG_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>Log3G10 RWG to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - RED Log3G10 RWG to ACES
//
// converts RED Log3G10, RED Wide Gamut to 
//          ACES2065-1 (AP0 w/ linear encoding)
//



const float a = 0.224282;
const float b = 155.975327;
const float c = 0.01;
const float g = 15.1927;

const float RWG_2_AP0_MAT[3][3] = {
  { 0.7850585442,  0.0231738066, -0.0737605663},
  { 0.0838583156,  1.0878975877, -0.3145898729},
  { 0.1310821505, -0.1110709153,  1.3883506702}
};



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