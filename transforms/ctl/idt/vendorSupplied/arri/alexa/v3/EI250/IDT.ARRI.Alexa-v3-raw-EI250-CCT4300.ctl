
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI250-CCT4300.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI250, 4300K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 250
//  and CCT of adopted white set to 4300K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 250.0;
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
	rOut = r_lin * 7.8581866104042e-01 + g_lin * 1.2810823648769e-01 + b_lin * 8.6073102471891e-02;
	gOut = r_lin * 6.2428264523113e-02 + g_lin * 1.0605423879261e+00 + b_lin * -1.2297065244917e-01;
	bOut = r_lin * 3.9034545622592e-02 + g_lin * -3.0301997902326e-01 + b_lin * 1.2639854334007e+00;
	aOut = 1.0;

}
