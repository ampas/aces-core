
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI1600-CCT2900.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI1600, 2900K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 1600
//  and CCT of adopted white set to 2900K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 1600.0;
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
	rOut = r_lin * 7.8271888483542e-01 + g_lin * 8.3021954381137e-02 + b_lin * 1.3425916078345e-01;
	gOut = r_lin * 3.4247624875605e-02 + g_lin * 1.0398108997401e+00 + b_lin * -7.4058524615691e-02;
	bOut = r_lin * 3.2204382916568e-02 + g_lin * -3.4004619098244e-01 + b_lin * 1.3078418080659e+00;
	aOut = 1.0;

}
