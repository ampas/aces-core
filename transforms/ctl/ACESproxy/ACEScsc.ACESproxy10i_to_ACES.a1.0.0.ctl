
// <ACEStransformID>ACEScsc.ACESproxy10i_to_ACES.a1.0.0</ACEStransformID>
// <ACESuserName>ACESproxy to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - ACESproxy (10-bit) to ACES
//
// converts ACESproxy (AP1 w/ ACESproxy encoding) to 
//          ACES2065-1 (AP0 w/ linear encoding)
//
// This transform follows the formulas from S-2013-001. Please refer to the 
// aforementioned document for the exact math and a table of reference values.
//

// *-*-*-*-*-*-*-*-*
// ACESproxy is intended to be a transient encoding used only for signal 
// transmission in systems limited to 10- or 12-bit video signals.
// ACESproxy is not intended for interchange, mastering finals, or archiving 
// and as such should NOT be written into a container file in actual 
// implementations! 
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common.a1.0.0";



const float StepsPerStop = 50.;
const float MidCVoffset = 425.;
const int CVmin = 64;
const int CVmax = 940;



float ACESproxy_to_lin
( 
  input varying float in,
  input varying float StepsPerStop,
  input varying float MidCVoffset,
  input varying float CVmin,
  input varying float CVmax
)
{  
  return pow( 2., ( in - MidCVoffset)/StepsPerStop - 2.5);
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
    float AP1_lin[3];
    AP1_lin[0] = ACESproxy_to_lin( rIn, StepsPerStop, MidCVoffset, CVmin, CVmax);
    AP1_lin[1] = ACESproxy_to_lin( gIn, StepsPerStop, MidCVoffset, CVmin, CVmax);
    AP1_lin[2] = ACESproxy_to_lin( bIn, StepsPerStop, MidCVoffset, CVmin, CVmax);

    float ACES[3] = mult_f3_f44( AP1_lin, AP1_2_AP0_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}