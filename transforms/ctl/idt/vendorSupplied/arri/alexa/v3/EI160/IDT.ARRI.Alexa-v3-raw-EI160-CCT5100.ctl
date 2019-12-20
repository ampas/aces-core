
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI160-CCT5100.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI160, 5100K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 160
//  and CCT of adopted white set to 5100K
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
	rOut = r_lin * 7.9304097907986e-01 + g_lin * 1.4478668584766e-01 + b_lin * 6.2172335072471e-02;
	gOut = r_lin * 7.2227845445030e-02 + g_lin * 1.0783170220376e+00 + b_lin * -1.5054486748263e-01;
	bOut = r_lin * 4.1417129674105e-02 + g_lin * -2.8764131714283e-01 + b_lin * 1.2462241874687e+00;
	aOut = 1.0;

}
