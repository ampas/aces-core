
// <ACEStransformID>ACESlib.OCIOshaper_to_Lin_param.a1.0.0.ctl</ACEStransformID>

// 
// Generic transform from an OCIOshaper encoding to linear
// 

import "ACESlib.Transform_Common.a1.0.0";

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

  rOut = pq_f( rIn )*scale + minLinear;
  gOut = pq_f( gIn )*scale + minLinear;
  bOut = pq_f( bIn )*scale + minLinear;
  aOut = aIn;  
}