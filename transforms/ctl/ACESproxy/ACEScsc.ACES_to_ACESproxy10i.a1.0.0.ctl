
// <ACEStransformID>ACEScsc.ACES_to_ACESproxy10i.a1.0.0</ACEStransformID>
// <ACESuserName>ACES2065-1 to ACESproxy</ACESuserName>

//
// ACES Color Space Conversion - ACES to ACESproxy (10-bit)
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ACESproxy (AP1 w/ ACESproxy encoding)
//
// This transform follows the formulas from section 4.3 in S-2013-001
//
 
// *-*-*-*-*-*-*-*-*
// ACESproxy is intended to be a transient encoding used only for signal 
// transmission in systems limited to 10- or 12-bit video signals.
// ACESproxy is not intended for interchange, mastering finals, or archiving 
// and as such should NOT be written into a container file in actual 
// implementations! 
// 
//
// For verification purposes only, ACESproxy was written into 16-bit integral 
// point file (specifically, a 16-bit TIFF).
//
//   ctlrender -ctl ACEScsc.ACES_to_ACESproxy10i.a1.0.0.ctl aces.exr acesProxy10i.tiff -format tiff16
//
// ** ACESproxy should not be written into a container file in actual 
// implementations! **
// 
// *-*-*-*-*-*-*-*-*


import "ACESlib.Transform_Common.a1.0.0";



const float StepsPerStop = 50.;
const float MidCVoffset = 425.;
const int CVmin = 64;
const int CVmax = 940;



int lin_to_ACESproxy
( 
  input varying float in,
  input varying float StepsPerStop,
  input varying float MidCVoffset,
  input varying float CVmin,
  input varying float CVmax
)
{
  if (in < pow(2.,(in-MidCVoffset)/StepsPerStop)-2.5) 
    return CVmin;
  else
    return max( CVmin, min( CVmax,( log2(in) + 2.5) * StepsPerStop + MidCVoffset)) + 0.5;
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

    ACES = clamp_f3( ACES, 0.0, HALF_POS_INF);

    float ACESproxy_lin[3] = mult_f3_f44( ACES, AP0_2_AP1_MAT);

    rOut = lin_to_ACESproxy( ACESproxy_lin[0], StepsPerStop, MidCVoffset, CVmin, CVmax);
    gOut = lin_to_ACESproxy( ACESproxy_lin[1], StepsPerStop, MidCVoffset, CVmin, CVmax);
    bOut = lin_to_ACESproxy( ACESproxy_lin[2], StepsPerStop, MidCVoffset, CVmin, CVmax);
    aOut = aIn;  
}