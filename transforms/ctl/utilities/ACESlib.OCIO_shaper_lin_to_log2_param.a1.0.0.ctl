
// 
// Generic transform from linear to a log base-2 encoding
// 



float lin_to_log2_32f
(
  float lin, 
  float middleGrey, 
  float minExposure, 
  float maxExposure
) 
{
  if (lin <= 0.0) return 0.0;

  float log2 = (log10(lin / middleGrey) / log10(2.0));
  float logNorm = (log2 - minExposure)/(maxExposure - minExposure);

  if( logNorm < 0.0) logNorm = 0;

  return logNorm;
}



void main
(
  input varying float rIn,
  input varying float gIn,
  input varying float bIn,
  input varying float aIn,
  input varying float middleGrey,
  input varying float minExposure,
  input varying float maxExposure,
  output varying float rOut,
  output varying float gOut,
  output varying float bOut,
  output varying float aOut
)
{
  float lin[3] = {rIn, gIn, bIn};

  float log32f[3];
  log32f[0] = lin_to_log2_32f( lin[0], middleGrey, minExposure, maxExposure);
  log32f[1] = lin_to_log2_32f( lin[1], middleGrey, minExposure, maxExposure);
  log32f[2] = lin_to_log2_32f( lin[2], middleGrey, minExposure, maxExposure);

  const float scalar = 1.0;
  rOut = log32f[0] / scalar;
  gOut = log32f[1] / scalar;
  bOut = log32f[2] / scalar;
  aOut = aIn;  
}