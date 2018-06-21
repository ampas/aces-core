
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI3200-CCT2100-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI3200, 2100K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 3200
//  and CCT of adopted white set to 2100K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 3200.0;
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
	rOut = r_lin * 7.5250295379604e-01 + g_lin * 8.9327599408901e-02 + b_lin * 1.5816944679506e-01;
	gOut = r_lin * -3.1747442541030e-02 + g_lin * 1.0699468166599e+00 + b_lin * -3.8199374118818e-02;
	bOut = r_lin * 1.6727160041241e-02 + g_lin * -5.7430317220877e-01 + b_lin * 1.5575760121675e+00;
	aOut = 1.0;

}
