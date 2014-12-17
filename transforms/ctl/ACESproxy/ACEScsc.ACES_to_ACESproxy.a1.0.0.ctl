//
// ACES Color Space Conversion - ACES to ACESproxy
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ACESproxy (AP1 w/ ACESproxy encoding)
//
 
// *-*-*-*-*-*-*-*-*
// ACESproxy is intended to be a transient encoding used only for signal 
// transmission in systems limited to 10- or 12-bit video signals.
// ACESproxy is not intended for interchange, mastering finals, or archiving 
// and as such should NOT be written into a container file in actual 
// implementations! 
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common.a1.0.0";



float lin_to_ACESproxy( input varying float in)
{
  float out;

  if (in < 0.0) 
    out = 0.0;
  else
    out = (log2(in)+2.5)*50.0 + 425.0;
    out = clamp( out, 64., 940.);
  
  return (out-64.0)/876.0;
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
    float aces[3] = { rIn, gIn, bIn};

    aces = clamp_f3( aces, 0.0, HALF_POS_INF);

    float ACESproxy_lin[3] = mult_f3_f44( aces, AP0_2_AP1_MAT);

    rOut = lin_to_ACESproxy( ACESproxy_lin[0]);
    gOut = lin_to_ACESproxy( ACESproxy_lin[1]);
    bOut = lin_to_ACESproxy( ACESproxy_lin[2]);
    aOut = aIn;  
}