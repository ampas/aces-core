
// <ACEStransformID>ACESlib.DolbyPQ_to_Lin.a1.0.1.ctl</ACEStransformID>

// 
// Generic transform from Dolby PQ to linear
// 

import "ACESlib.Transform_Common.a1.0.1";

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
  rOut = pq_f( rIn );
  gOut = pq_f( gIn );
  bOut = pq_f( bIn );
  aOut = aIn;  
}