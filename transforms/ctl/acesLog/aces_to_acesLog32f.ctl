
// 
// ACES to ACESlog32f Transform
// 

//
// For verification purposes only, ACESlog32f was written into a 32-bit floating 
// point file (specifically, a 32-bit OpenEXR).
//
//   ctlrender -ctl aces_to_acesLog32f.ctl aces.exr acesLog32f.exr -format exr32
// 
// ** ACESlog32f should NOT be written into a container file in actual 
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



  // Note: The scaling step seen in "aces_to_acesLog16.ctl" is not required here 
  // because the input and output file formats are floating point. Thus, 
  // ctlrender does not automatically assume scaling and rOut,gOut,bOut can be 
  // the values of acesLog itself.
  rOut = acesLog32f[0];
  gOut = acesLog32f[1];
  bOut = acesLog32f[2];
  aOut = aIn;    
}