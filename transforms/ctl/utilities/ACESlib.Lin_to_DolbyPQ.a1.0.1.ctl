
// <ACEStransformID>ACESlib.Lin_to_SMPTE2048.a1.0.1</ACEStransformID>

// 
// Generic transform from linear to encoding specified in SMPTE ST2048
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
  rOut = Y_2_ST2048( rIn );
  gOut = Y_2_ST2048( gIn );
  bOut = Y_2_ST2048( bIn );
  aOut = aIn;  
}