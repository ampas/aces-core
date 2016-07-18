
// <ACEStransformID>ACESutil.premultAlpha_to_straightAlpha.a1.0.3</ACEStransformID>
// <ACESuserName>Pre-multiplied Alpha to Straight Alpha</ACESuserName>

// 
// Transform to convert Alpha Premultiplied RGB values to straight Alpha RGB values
// 

void main
(
  input varying float rIn,
  input varying float gIn,
  input varying float bIn,
  input varying float aIn,
  output varying float rOut,
  output varying float gOut,
  output varying float bOut,
  output varying float aOut,
  input uniform float limit = 0.001
)
{
float rgb[3] = { rIn, gIn, bIn};

if ( rgb[0] >= limit) {
	rgb[0] = rgb[0] / aIn;
}

if ( rgb[1] >= limit) {
	rgb[1] = rgb[1] / aIn;
}

if ( rgb[2] >= limit) {
	rgb[2] = rgb[2] / aIn;
}

rOut = rgb[0];
gOut = rgb[1];
bOut = rgb[2];
aOut = aIn;
}