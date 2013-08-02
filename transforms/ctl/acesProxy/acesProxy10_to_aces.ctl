
// 
// ACESproxy10 to ACES Transform
// 

//
// For verification purposes, ACESproxy10 is assumed to be in a 16-bit integral 
// file (specifically, a 16-bit TIFF).
//
//   ctlrender -ctl acesProxy10_to_aces.ctl -input_scale 1.0 acesProxy10.tiff aces.exr -format aces
//
// ** ACESproxy10 should not be written into a container file in actual 
// implementations! **
// 


// ACESproxy10 Parameters
const float CVmin = 0.;
const float CVmax = 1023.;
const float StepsPerStop = 50.;
const float MidCVoffset = 425.;
const float MidLogOffset = -2.5;


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
  rOut = pow( 2., ( ( (rIn - MidCVoffset) / StepsPerStop ) + MidLogOffset) );
  gOut = pow( 2., ( ( (gIn - MidCVoffset) / StepsPerStop ) + MidLogOffset) );
  bOut = pow( 2., ( ( (bIn - MidCVoffset) / StepsPerStop ) + MidLogOffset) );
  aOut = aIn;  
}