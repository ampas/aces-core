
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI320-CCT2100.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI320, 2100K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 320
//  and CCT of adopted white set to 2100K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 320.0;
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
	rOut = r_lin * 7.9219968557155e-01 + g_lin * 4.8664615568597e-02 + b_lin * 1.5913569885985e-01;
	gOut = r_lin * -1.0492258276909e-02 + g_lin * 1.0663602183434e+00 + b_lin * -5.5867960066484e-02;
	bOut = r_lin * 1.6824541200579e-02 + g_lin * -3.8224266169820e-01 + b_lin * 1.3654181204976e+00;
	aOut = 1.0;

}
