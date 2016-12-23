
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI1600-CCT3500-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI1600, 3500K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 1600
//  and CCT of adopted white set to 3500K
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
	rOut = r_lin * 7.4214681752703e-01 + g_lin * 1.3363714117678e-01 + b_lin * 1.2421604129619e-01;
	gOut = r_lin * 4.1324605920361e-02 + g_lin * 1.0328690723201e+00 + b_lin * -7.4193678240465e-02;
	bOut = r_lin * 7.1726194134801e-02 + g_lin * -4.9280086453080e-01 + b_lin * 1.4210746703960e+00;
	aOut = 1.0;

}
