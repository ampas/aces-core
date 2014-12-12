//
// ACES Color Space Conversion - ACES to ACESproxy
//
// converts ACES2065-1 (AP0 w/ linear encoding) to AP1 w/ ACESproxy encoding
//
 
//
// ** ACESproxy should not be written into a container file in actual 
// implementations! **
//



import "transforms-common";



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
    float aces[3] = { rIn, gIn, bIn};

    aces = clamp_f3( aces, 0.0, HALF_POS_INF);

    float AP1_lin[3] = mult_f3_f44( aces, AP0_2_AP1_MAT);

    rOut = lin_to_acesProxy( AP1_lin[0]);
    gOut = lin_to_acesProxy( AP1_lin[1]);
    bOut = lin_to_acesProxy( AP1_lin[2]);
    aOut = aIn;  
}