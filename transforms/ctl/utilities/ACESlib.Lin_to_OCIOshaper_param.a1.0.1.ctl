
// <ACEStransformID>ACESlib.Lin_to_OCIOshaper_param.a1.0.1.ctl</ACEStransformID>

// 
// Generic transform from linear to an OCIOshaper encoding
// 

import "ACESlib.Transform_Common.a1.0.1";

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

  rOut = pq_r( (rIn - minLinear)/scale );
  gOut = pq_r( (gIn - minLinear)/scale );
  bOut = pq_r( (bIn - minLinear)/scale );
  aOut = aIn;  
}