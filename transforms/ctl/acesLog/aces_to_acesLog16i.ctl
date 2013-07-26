
// 
// ACES to ACESlog16i Transform
// 

//
// For verification purposes only, ACESlog16i was written into a 16-bit integral 
// file (specifically, a 16-bit TIFF).
// 
//   ctlrender -ctl aces_to_acesLog16i.ctl aces.exr acesLog16i.tiff -format tiff16
//
// ** ACESlog16i should NOT be written into a container file in actual 
// implementations! **
//


import "utilities";


float aces_to_acesLog32f( float aces) 
{
  const float aceslog_unity = 32768.;
  const float aceslog_xperstop = 2048.;
  float aces_denorm_trans = pow( 2., -15);
  float aces_denorm_fake0 = pow( 2., -16);

  if (aces < 0.0) return 0.0;
  
  /* compress denorms into 1 stop below last norm stop */
  float tmp;
  if (aces < aces_denorm_trans) {
    tmp = aces_denorm_fake0 + ( aces / 2.);
  } else {
    tmp = aces;
  }

  float acesLog32f = (log10(tmp) / log10(2)) * aceslog_xperstop + aceslog_unity;
  
  return acesLog32f;
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
  float aces[3] = {rIn, gIn, bIn};

  float acesLog32f[3];
  acesLog32f[0] = aces_to_acesLog32f( aces[0]);
  acesLog32f[1] = aces_to_acesLog32f( aces[1]);
  acesLog32f[2] = aces_to_acesLog32f( aces[2]);

  int acesLog16i[3];
  acesLog16i[0] = min( floor( acesLog32f[0] + 0.5), 65535);
  acesLog16i[1] = min( floor( acesLog32f[1] + 0.5), 65535);
  acesLog16i[2] = min( floor( acesLog32f[2] + 0.5), 65535);
  

  
  // **NOTE**: The scaling step below is NOT a part of the ACESlog conversion. 
  //
  // This step is only required when using ctlrender to process the images. 
  // When ctlrender sees a floating-point file as the input and an integral file 
  // format as the output, it assumes that values out the end of the transform 
  // will be floating point, and so multiplies the output values by 
  // (2^outputBitDepth)-1. Therefore, although the values of acesLog16i are the 
  // correct integer values, they must be scaled into a 0-1 range on output 
  // because ctlrender will scale them back up in the process of writing the 
  // integer file.
  // The ctlrender option -output_scale could be used for this, but it 
  // currently this option does not appear to function correctly.
  const float scalar = pow(2.,16)-1.;
  rOut = acesLog16i[0] / scalar;
  gOut = acesLog16i[1] / scalar;
  bOut = acesLog16i[2] / scalar;
  aOut = aIn;  
}