
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACEScct_to_ACES.a1.0.3</ACEStransformID>
// <ACESuserName>ACEScct to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - ACEScct to ACES
//
// converts ACEScct (AP1 w/ ACESlog encoding) to 
//          ACES2065-1 (AP0 w/ linear encoding)
//

// *-*-*-*-*-*-*-*-*
// ACEScct is intended to be transient and internal to software or hardware 
// systems, and is specifically not intended for interchange or archiving.
// ACEScct should NOT be written into a container file in actual implementations!
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common";



const float X_BRK = 0.0078125;
const float Y_BRK = 0.155251141552511;
const float A = 10.5402377416545;
const float B = 0.0729055341958355;



float ACEScct_to_lin( input varying float in)
{
    if (in > Y_BRK)
        return pow( 2., in*17.52-9.72);
    else
        return (in - B) / A;
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
    float lin_AP1[3];
    lin_AP1[0] = ACEScct_to_lin( rIn);
    lin_AP1[1] = ACEScct_to_lin( gIn);
    lin_AP1[2] = ACEScct_to_lin( bIn);

    float ACES[3] = mult_f3_f44( lin_AP1, AP1_2_AP0_MAT);
  
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}