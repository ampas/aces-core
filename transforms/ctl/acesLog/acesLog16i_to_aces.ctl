
// 
// ACESlog16i to ACES Transform
// 

//
// For verification purposes, ACESlog16i is assumed to be in a 16-bit integral 
// file (specifically, a 16-bit TIFF).
//
//   ctlrender -ctl acesLog16i_to_aces.ctl -input_scale 1.0 acesLog16i.tiff aces.exr -format aces
//
// ** ACESlog16i should NOT be written into a container file in actual 
// implementations! **
//


float aceslog_to_aces( float aceslog) 
{
  const float aceslog_unity = 32768;
  const float aceslog_xperstop = 2048;
  float aces_denorm_trans = pow( 2., -15);
  float aces_denorm_fake0 = pow( 2., -16);

  float aces = pow( 2., (aceslog - aceslog_unity) / aceslog_xperstop);
  
  /* restore denorms */  
  if (aces < aces_denorm_trans) {
    aces = (aces - aces_denorm_fake0) * 2.;
  }
  
  return aces;
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
  rOut = aceslog_to_aces( rIn);
  gOut = aceslog_to_aces( gIn);
  bOut = aceslog_to_aces( bIn);
  aOut = aIn;  
}