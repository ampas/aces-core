import "utilities";
import "utilities-color";


// Working space primaries
const Chromaticities ACESWS =
{
  {0.70800, 0.29400},
  {0.16490, 0.83000},
  {0.12900, 0.04300},
  {0.32168, 0.33767}
};

const float ACES_2_XYZ_MAT[4][4] = RGBtoXYZ( ACES_PRI, 1.0);
const float XYZ_2_ACESWS_MAT[4][4] = XYZtoRGB( ACESWS, 1.0);
const float ACES_2_ACESWS_MAT[4][4] = mult_f44_f44( ACES_2_XYZ_MAT, XYZ_2_ACESWS_MAT);


float lin_2_cineonLog( input varying float in)
{
  if (in < 0.0106232)
    return in*5.3676533+0.0926363;
  else 
    return 0.5696259+0.1073531*log(in+0.00937677);
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

	rOut = lin_2_cineonLog( acesWS[0]);
	gOut = lin_2_cineonLog( acesWS[1]);
	bOut = lin_2_cineonLog( acesWS[2]);
	aOut = aIn;
}