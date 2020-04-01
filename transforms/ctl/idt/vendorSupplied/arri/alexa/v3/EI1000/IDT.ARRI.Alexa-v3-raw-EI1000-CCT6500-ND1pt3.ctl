
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI1000-CCT6500-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI1000, 6500K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 1000
//  and CCT of adopted white set to 6500K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 1000.0;
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
	rOut = r_lin * 7.4637759172025e-01 + g_lin * 1.9124417016891e-01 + b_lin * 6.2378238110838e-02;
	gOut = r_lin * 7.3383907980859e-02 + g_lin * 1.0863442574049e+00 + b_lin * -1.5972816538574e-01;
	bOut = r_lin * 8.5424358133235e-02 + g_lin * -4.2509242273192e-01 + b_lin * 1.3396680645987e+00;
	aOut = 1.0;

}
