
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI160-CCT2400.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI160, 2400K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 160
//  and CCT of adopted white set to 2400K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

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
	rOut = r_lin * 7.8650859648504e-01 + g_lin * 6.2052342777150e-02 + b_lin * 1.5143906073781e-01;
	gOut = r_lin * 1.1536976034709e-02 + g_lin * 1.0469294688819e+00 + b_lin * -5.8466444916626e-02;
	bOut = r_lin * 2.5554039615717e-02 + g_lin * -3.6716807418049e-01 + b_lin * 1.3416140345648e+00;
	aOut = 1.0;

}
