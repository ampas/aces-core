
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI200-CCT7500-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI200, 7500K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 200
//  and CCT of adopted white set to 7500K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 200.0;
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
	rOut = r_lin * 7.5185027588127e-01 + g_lin * 1.9940980049558e-01 + b_lin * 4.8739923623151e-02;
	gOut = r_lin * 7.8038754417232e-02 + g_lin * 1.1049071044764e+00 + b_lin * -1.8294585889358e-01;
	bOut = r_lin * 8.8214023268782e-02 + g_lin * -4.1653655677960e-01 + b_lin * 1.3283225335108e+00;
	aOut = 1.0;

}
