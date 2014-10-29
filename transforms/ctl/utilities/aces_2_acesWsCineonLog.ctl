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

const float ACES_2_XYZ_MAT[4][4] = RGBtoXYZ( ACES_PRI, 1.0);
const float XYZ_2_ACESWS_MAT[4][4] = XYZtoRGB( ACESWS, 1.0);
const float ACES_2_ACESWS_MAT[4][4] = mult_f44_f44( ACES_2_XYZ_MAT, XYZ_2_ACESWS_MAT);


float lin_2_acesLogScaled( input varying float in)
{
  float out;
  if (in > 0)
    out = ((log(in)/log(2.0)+2.5)*50.0+425.0);
  else
    out = 0.0;
    
  return (out-64.0)/876.0;
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
  float aces[3] = { rIn, gIn, bIn};

  float acesWS[3] = mult_f3_f44( aces, ACES_2_ACESWS_MAT);

	rOut = lin_2_acesLogScaled( acesWS[0]);
	gOut = lin_2_acesLogScaled( acesWS[1]);
	bOut = lin_2_acesLogScaled( acesWS[2]);
	aOut = aIn;
}