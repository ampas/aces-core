
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI200-CCT3200-ND1pt3.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI200, 3200K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 200
//  and CCT of adopted white set to 3200K
// Written by v3_IDT_maker.py v0.08 on Friday 19 December 2014

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
	rOut = r_lin * 0.742758 + g_lin * 0.124957 + b_lin * 0.132285;
	gOut = r_lin * 0.033503 + g_lin * 1.029404 + b_lin * -0.062907;
	bOut = r_lin * 0.067390 + g_lin * -0.507000 + b_lin * 1.439610;
	aOut = 1.0;

}
