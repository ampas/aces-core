
// <ACEStransformID>ACESlib.DolbyPQ_to_Lin.a1.0.1</ACEStransformID>

// 
// Generic transform from SMPTE ST2084 to linear
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
  rOut = ST2084_2_Y( rIn );
  gOut = ST2084_2_Y( gIn );
  bOut = ST2084_2_Y( bIn );
  aOut = aIn;  
}