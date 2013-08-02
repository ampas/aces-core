
//
// LMT for lower default contrast 
// v0.2
//

import "utilities";
import "utilities-aces";


float logacesf_bl_2_aces( float logacesf)
{
  return pow10(((logacesf * 1023. - 685.0) / 300.0 )) * 1.0109  - 0.0109;
}

float aces_2_logacesf_bl( float aces)
{
  return (685. + 300. * log10((aces + 0.0109) / (1.0109))) / 1023.;
}

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
  float aces[3] = { rIn, gIn, bIn};

  // Go to BL log
  float logaces_bl[3];
  logaces_bl[0] = aces_2_logacesf_bl( aces[0]);
  logaces_bl[1] = aces_2_logacesf_bl( aces[1]);
  logaces_bl[2] = aces_2_logacesf_bl( aces[2]);

  // Apply video grade
  const float lift = -0.069;
  const float gamma = 0.221;
  const float gain = 0.931;

  if (logaces_bl[0] >= 0.)
    logaces_bl[0] = lift + (gain - lift) * pow( logaces_bl[0], 1./(1.+gamma));
  else
    logaces_bl[0] = 0.0;
  if (logaces_bl[1] >= 0.)  
    logaces_bl[1] = lift + (gain - lift) * pow( logaces_bl[1], 1./(1.+gamma));
  else
    logaces_bl[1] = 0.0;
  if (logaces_bl[2] >= 0.)
    logaces_bl[2] = lift + (gain - lift) * pow( logaces_bl[2], 1./(1.+gamma));
  else
    logaces_bl[2] = 0.0;

  // Go to ACES
  rOut = logacesf_bl_2_aces( logaces_bl[0]);
  gOut = logacesf_bl_2_aces( logaces_bl[1]);
  bOut = logacesf_bl_2_aces( logaces_bl[2]);
  aOut = aIn;  
}