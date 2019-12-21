
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI800-CCT9000-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI800, 9000K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 800
//  and CCT of adopted white set to 9000K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 800.0;
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
	rOut = r_lin * 7.5949126103710e-01 + g_lin * 2.0703136402026e-01 + b_lin * 3.3477374942638e-02;
	gOut = r_lin * 8.2780210170624e-02 + g_lin * 1.1271333000075e+00 + b_lin * -2.0991351017814e-01;
	bOut = r_lin * 9.1061976081711e-02 + g_lin * -4.0879586601605e-01 + b_lin * 1.3177338899343e+00;
	aOut = 1.0;

}
