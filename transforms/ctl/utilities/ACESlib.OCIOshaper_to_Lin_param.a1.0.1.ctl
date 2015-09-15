
// <ACEStransformID>ACESlib.OCIOshaper_to_Lin_param.a1.0.1.ctl</ACEStransformID>

// 
// Generic transform from an OCIOshaper encoding to linear
// 



import "ACESlib.Utilities_Color.a1.0.1";



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
  float PQ_max = pq_C;
  float minLinear = pow(2.0, minExposure) * middleGrey;
  float maxLinear = pow(2.0, maxExposure) * middleGrey;
  float scale = (maxLinear - minLinear) / PQ_max;

  rOut = ST2084_2_Y( rIn )*scale + minLinear;
  gOut = ST2084_2_Y( gIn )*scale + minLinear;
  bOut = ST2084_2_Y( bIn )*scale + minLinear;
  aOut = aIn;  
}