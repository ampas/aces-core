
// <ACEStransformID>IDT.ARRI.Alexa-v2-raw-EI160-CCT6500.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI160, 6500K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 160
//  and CCT of adopted white set to 6500K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

const float EI = 160.0;
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
	rOut = r_lin * 0.809931 + g_lin * 0.162741 + b_lin * 0.027328;
	gOut = r_lin * 0.083731 + g_lin * 1.108667 + b_lin * -0.192397;
	bOut = r_lin * 0.044166 + g_lin * -0.272038 + b_lin * 1.227872;
	aOut = 1.0;

}
