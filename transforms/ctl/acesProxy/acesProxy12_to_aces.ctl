
// 
// ACESproxy12 to ACES Transform
// 

//
// For verification purposes, ACESproxy12 is assumed to be in a 16-bit integral 
// file (specifically, a 16-bit TIFF).
//
//   ctlrender -ctl acesProxy12_to_aces.ctl -input_scale 1.0 acesProxy12.tiff aces.exr -format aces
//
// ** ACESproxy12 should not be written into a container file in actual 
// implementations! **
//


// ACESproxy12 Parameters
const float CVmin = 0.;
const float CVmax = 4095.;
const float StepsPerStop = 200.;
const float MidCVoffset = 1700.;
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