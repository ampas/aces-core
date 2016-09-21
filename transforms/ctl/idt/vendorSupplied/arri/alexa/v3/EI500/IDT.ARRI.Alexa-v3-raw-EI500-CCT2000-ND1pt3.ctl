
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI500-CCT2000-ND1pt3.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI500, 2000K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 500
//  and CCT of adopted white set to 2000K
// Written by v3_IDT_maker.py v0.08 on Friday 19 December 2014

const float EI = 500.0;
const float black = 256.0 / 65535.0;
const float exp_factor = 0.18 / (0.01 * (400.0/EI));

void main
(	input varying float rIn,
	input varying float gIn,
	input varying float bIn,
	input varying float aIn,
	output varying float rOut,
	output varying float gOut,
	output varying float bOut,
	output varying float aOut)
{

	// convert to white-balanced, black-subtracted linear values
	float r_lin = (rIn - black) * exp_factor;
	float g_lin = (gIn - black) * exp_factor;
	float b_lin = (bIn - black) * exp_factor;

	// convert to ACES primaries using CCT-dependent matrix
	rOut = r_lin * 0.754108 + g_lin * 0.088338 + b_lin * 0.157555;
	gOut = r_lin * -0.044583 + g_lin * 1.086354 + b_lin * -0.041771;
	bOut = r_lin * 0.003821 + g_lin * -0.576333 + b_lin * 1.572512;
	aOut = 1.0;

}
