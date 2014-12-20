
// <ACEStransformID>ACESlib.OCIO_shaper_log2_to_lin_param.a1.0.0</ACEStransformID>

// 
// Generic transform from log base-2 encoding to linear
// 



float log2_to_lin_32f
(
  float logNorm, 
  float middleGrey, 
  float minExposure, 
  float maxExposure
)
{
  if (logNorm < 0.0) return 0.0;

  float log2 = logNorm*(maxExposure - minExposure) + minExposure;
  float lin = pow(2.0, log2) * middleGrey;
  
  return lin;
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
  rOut = log2_to_lin_32f( rIn, middleGrey, minExposure, maxExposure);
  gOut = log2_to_lin_32f( gIn, middleGrey, minExposure, maxExposure);
  bOut = log2_to_lin_32f( bIn, middleGrey, minExposure, maxExposure);
  aOut = aIn;  
}