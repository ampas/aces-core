
// <ACEStransformID>ACESlib.Lin_to_OCIOshaper_param.a1.0.1.ctl</ACEStransformID>

// 
// Generic transform from linear to an OCIOshaper encoding
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

  rOut = Y_2_ST2084( (rIn - minLinear)/scale );
  gOut = Y_2_ST2084( (gIn - minLinear)/scale );
  bOut = Y_2_ST2084( (bIn - minLinear)/scale );
  aOut = aIn;  
}