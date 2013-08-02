
// 
// ACES to ACESproxy12 Transform
// 

//
// For verification purposes only, ACESproxy12 was written into 16-bit floating 
// point file (specifically, a 16-bit TIFF).
//
//   ctlrender -ctl aces_to_acesProxy12.ctl aces.exr acesProxy12.tiff -format tiff16
//
// ** ACESproxy12 should not be written into a container file in actual 
// implementations! **
//


import "utilities";


// ACESproxy12 Parameters
const int CVmin = 0;
const int CVmax = 4095;
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

  float aces[3] = {rIn, gIn, bIn};

  int acesProxy[3];
  if (aces[0] > 0.) 
    acesProxy[0] = max(CVmin, min( CVmax,  ( log10( aces[0])/(log10(2)) - MidLogOffset) * StepsPerStop + MidCVoffset ) ) + 0.5;
  else
    acesProxy[0] = CVmin;
  if (aces[1] > 0.)
    acesProxy[1] = max(CVmin, min( CVmax,  ( log10( aces[1])/(log10(2)) - MidLogOffset) * StepsPerStop + MidCVoffset ) ) + 0.5;
  else
    acesProxy[1] = CVmin;  
  if (aces[2] > 0.)
    acesProxy[2] = max(CVmin, min( CVmax,  ( log10( aces[2])/(log10(2)) - MidLogOffset) * StepsPerStop + MidCVoffset ) ) + 0.5;
  else
    acesProxy[2] = CVmin;  
  


  // **NOTE**: The scaling step below is NOT normally a part of the ACESproxy 
  // conversion. 
  //
  // This step is only required when using ctlrender to process the images. 
  // When ctlrender sees a floating-point file as the input and an integral file 
  // format as the output, it assumes that values out the end of the transform 
  // will be floating point, and so multiplies the output values by 
  // (2^outputBitDepth)-1. Therefore, although the values of acesProxy are the 
  // correct integer values, they must be scaled into a 0-1 range on output 
  // because ctlrender will scale them back up in the process of writing the 
  // integer file.
  // The ctlrender option -output_scale could be used for this, but it 
  // currently this option does not appear to function correctly.
  const float scalar = pow(2.,16)-1.;
  rOut = acesProxy[0] / scalar;
  gOut = acesProxy[1] / scalar;
  bOut = acesProxy[2] / scalar;
  aOut = aIn;  
}