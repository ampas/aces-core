
// <ACEStransformID>IDT.Sony.SLog2_SGamut_Tungsten_10i.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Sony SLog2 (tungsten)</ACESuserName>

//
// IDT for Sony Cameras - 10 bits - Tungsten (3200K or 4300K)
// Provided by Sony Electronics Corp.
//





/* ============ CONSTANTS ============ */
const float SGAMUT_TUNG_TO_ACES_MTX[3][3] = {
  { 1.0110238740,  0.1011994504,  0.0600766530},
  {-0.1362526051,  0.9562196265, -0.1010185315},
  { 0.1252287310, -0.0574190769,  1.0409418785}
};

const float B = 64.;
const float AB = 90.;
const float W = 940.;





/* ============ SUBFUNCTIONS ============ */
float SLog2_to_lin 
(
  float SLog,
  float b,
  float ab,
  float w
)
{
  float lin;
  
  if (SLog >= ab)
    lin = ( 219. * ( pow(10., ( ( ( SLog - b) / ( w - b) - 0.616596 - 0.03) / 0.432699)) - 0.037584) / 155.) * 0.9;
  else if (SLog < ab) 
    lin = ( ( ( SLog - b) / ( w - b) - 0.030001222851889303) / 3.53881278538813) * 0.9;
  
  return lin;
  
}





/* ============ Main Algorithm ============ */
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
  // Prepare input values based on application bit depth handling
  float SLog[3];
  SLog[0] = rIn * 1023.;
  SLog[1] = gIn * 1023.;
  SLog[2] = bIn * 1023.;

  // 10-bit Sony S-log to linear S-gamut
  float lin[3];
  lin[0] = SLog2_to_lin( SLog[0], B, AB, W);
  lin[1] = SLog2_to_lin( SLog[1], B, AB, W);
  lin[2] = SLog2_to_lin( SLog[2], B, AB, W);
  
  // S-Gamut to ACES matrix
  float aces[3] = mult_f3_f33( lin, SGAMUT_TUNG_TO_ACES_MTX);

  rOut = aces[0];
  gOut = aces[1];
  bOut = aces[2];
  aOut = aIn;
}