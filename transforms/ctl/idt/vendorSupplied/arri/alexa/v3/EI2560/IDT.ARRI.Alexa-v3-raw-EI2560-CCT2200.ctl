
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI2560-CCT2200.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI2560, 2200K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 2560
//  and CCT of adopted white set to 2200K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 2560.0;
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
	rOut = r_lin * 7.8993865753485e-01 + g_lin * 5.3020738319421e-02 + b_lin * 1.5704060414573e-01;
	gOut = r_lin * -2.0916919345765e-03 + g_lin * 1.0577777753182e+00 + b_lin * -5.5686083383576e-02;
	bOut = r_lin * 2.0442022387559e-02 + g_lin * -3.7795888948292e-01 + b_lin * 1.3575168670954e+00;
	aOut = 1.0;

}
