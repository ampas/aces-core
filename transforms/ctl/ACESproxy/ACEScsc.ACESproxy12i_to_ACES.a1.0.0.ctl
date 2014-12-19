//
// ACES Color Space Conversion - ACESproxy (12-bit) to ACES
//
// converts ACESproxy (AP1 w/ ACESproxy encoding) to 
//          ACES2065-1 (AP0 w/ linear encoding)
//

// *-*-*-*-*-*-*-*-*
// ACESproxy is intended to be a transient encoding used only for signal 
// transmission in systems limited to 10- or 12-bit video signals.
// ACESproxy is not intended for interchange, mastering finals, or archiving 
// and as such should NOT be written into a container file in actual 
// implementations! 
// 
//
// For verification purposes, ACESproxy10 is assumed to be in a 16-bit integral 
// file (specifically, a 16-bit TIFF).
//
//   ctlrender -ctl ACEScsc.ACESproxy10i_to_ACES.a1.0.0.ctl acesProxy10i.tiff aces.exr -format aces
//
// ** ACESproxy should not be written into a container file in actual 
// implementations! **
// 
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common.a1.0.0";



const float StepsPerStop = 200.;
const float MidCVoffset = 1700.;
const int CVmin = 256;
const int CVmax = 3760;



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