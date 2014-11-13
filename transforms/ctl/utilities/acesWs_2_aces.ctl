import "utilities";
import "utilities-color";


// Working space primaries
const Chromaticities ACESWS =
{
  {0.713,	0.293},
  {0.165,	0.830},
  {0.128,	0.044},
  {0.32168,	0.33767}
};

const float XYZ_2_ACES_MAT[4][4] = XYZtoRGB( ACES_PRI, 1.0);
const float ACESWS_2_XYZ_MAT[4][4] = RGBtoXYZ( ACESWS, 1.0);
const float ACESWS_2_ACES_MAT[4][4] = mult_f44_f44( ACESWS_2_XYZ_MAT, XYZ_2_ACES_MAT);


float acesLogScaled_2_lin( input varying float in)
{
  return pow(2.0, ((in*876.0+64.0-425.0)/50.0-2.5));
}


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
  float acesWS_lin[3];
  acesWS_lin[0] = acesLogScaled_2_lin( rIn);
  acesWS_lin[1] = acesLogScaled_2_lin( gIn);
  acesWS_lin[2] = acesLogScaled_2_lin( bIn);
  
  float aces[3] = mult_f3_f44( acesWS_lin, ACESWS_2_ACES_MAT);

	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}