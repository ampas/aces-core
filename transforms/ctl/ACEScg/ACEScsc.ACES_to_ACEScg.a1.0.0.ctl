
// <ACEStransformID>ACEScsc.ACES_to_ACEScg.a1.0.0</ACEStransformID>
// <ACESuserName>ACES2065-1 to ACEScg</ACESuserName>

//
// ACES Color Space Conversion - ACES to ACEScg
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ACEScg (AP1 w/ linear encoding)
//



import "ACESlib.Transform_Common.a1.0.0";



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

    ACES = clamp_f3( ACES, 0.0, HALF_POS_INF);

    float ACEScg[3] = mult_f3_f44( ACES, AP0_2_AP1_MAT);

    rOut = ACEScg[0];
    gOut = ACEScg[1];
    bOut = ACEScg[2];
    aOut = aIn;
}