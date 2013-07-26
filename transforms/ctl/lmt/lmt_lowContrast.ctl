
//
// LMT for lower default contrast 
// v0.2
//

import "utilities";
import "utilities-aces";

void main 
( // input is linear ACES, output is modified ACES
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
  const float POWER = 0.85;
  
  float SCALAR = 0.18 / pow( 0.18, POWER);
  
  if (rIn > 0) rOut = pow( rIn, POWER) * SCALAR; else rOut = rIn;
  if (gIn > 0) gOut = pow( gIn, POWER) * SCALAR; else gOut = gIn;
  if (bIn > 0) bOut = pow( bIn, POWER) * SCALAR; else bOut = bIn;

  aOut = aIn;
}