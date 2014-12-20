
// <ACEStransformID>ACEScsc.ACEScg_to_ACES.a1.0.0</ACEStransformID>
// <ACESuserName>ACEScg to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - ACEScg to ACES
//
// converts ACEScg (AP1 w/ linear encoding) to
//          ACES2065-1 (AP0 w/ linear encoding)
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
    float ACEScg[3] = { rIn, gIn, bIn};

    float ACES[3] = mult_f3_f44( ACEScg, AP1_2_AP0_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}