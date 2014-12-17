//
// ACES Color Space Conversion - ACESproxy to ACES
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
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common.a1.0.0";



float ACESproxy_to_lin( input varying float in)
{
  float out;
  
  out = in*876.+64.;
  return pow( 2., (out-425.)/50. - 2.5);
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
    AP1_lin[0] = ACESproxy_to_lin( rIn);
    AP1_lin[1] = ACESproxy_to_lin( gIn);
    AP1_lin[2] = ACESproxy_to_lin( bIn);

    float aces[3] = mult_f3_f44( AP1_lin, AP1_2_AP0_MAT);

    rOut = aces[0];
    gOut = aces[1];
    bOut = aces[2];
    aOut = aIn;
}