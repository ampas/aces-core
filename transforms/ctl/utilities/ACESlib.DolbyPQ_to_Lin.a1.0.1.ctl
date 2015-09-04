
// <ACEStransformID>ACESlib.ST2048_to_Lin.a1.0.1.ctl</ACEStransformID>

// 
// Generic transform from SMPTE ST2048 to linear
// 



import "ACESlib.Utilities_Color.a1.0.1";



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
  rOut = ST2048_2_Y( rIn );
  gOut = ST2048_2_Y( gIn );
  bOut = ST2048_2_Y( bIn );
  aOut = aIn;  
}