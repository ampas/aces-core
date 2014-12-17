//
// ACES Color Space Conversion - ACES to ACEScc
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ACEScc (AP1 w/ logarithmic encoding)
//

// *-*-*-*-*-*-*-*-*
// ACEScc is intended to be transient and internal to software or hardware 
// systems, and is specifically not intended for interchange or archiving.
// ACEScc should NOT be written into a container file in actual implementations!
// *-*-*-*-*-*-*-*-*



import "ACESlib.Transform_Common.a1.0.0";



float lin_to_ACEScc( input varying float in)
{
  float out;

  if (in <= 0)
    out = -0.35828683;
  else if (in < pow(2.,-15.))
    out = (log2( pow(2.,-16.) + in * 0.5) + 9.72) / 17.52;
  else
    out = (log2(in) + 9.72) / 17.52;
    
  return out;
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

    float ACEScc_lin[3] = mult_f3_f44( aces, AP0_2_AP1_MAT);

    rOut = lin_to_ACEScc( ACEScc_lin[0]);
    gOut = lin_to_ACEScc( ACEScc_lin[1]);
    bOut = lin_to_ACEScc( ACEScc_lin[2]);
    aOut = aIn;
}