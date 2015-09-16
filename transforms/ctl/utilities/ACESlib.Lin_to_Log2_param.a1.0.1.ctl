
// <ACEStransformID>ACESlib.Lin_to_Log2_param.a1.0.1</ACEStransformID>

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

  float log2 = log2(lin / middleGrey);
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
  rOut = lin_to_log2_32f( rIn, middleGrey, minExposure, maxExposure);
  gOut = lin_to_log2_32f( gIn, middleGrey, minExposure, maxExposure);
  bOut = lin_to_log2_32f( bIn, middleGrey, minExposure, maxExposure);
  aOut = aIn;  
}