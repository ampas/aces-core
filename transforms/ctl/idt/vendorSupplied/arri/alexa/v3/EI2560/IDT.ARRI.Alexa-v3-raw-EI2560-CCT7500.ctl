
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI2560-CCT7500.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI2560, 7500K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 2560
//  and CCT of adopted white set to 7500K
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
	rOut = r_lin * 8.2193007662850e-01 + g_lin * 1.7179075288804e-01 + b_lin * 6.2791704834632e-03;
	gOut = r_lin * 8.9522061750106e-02 + g_lin * 1.1285313233809e+00 + b_lin * -2.1805338513099e-01;
	bOut = r_lin * 4.5850892229268e-02 + g_lin * -2.6538039329937e-01 + b_lin * 1.2195295010701e+00;
	aOut = 1.0;

}
