
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI3200-CCT2200-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI3200, 2200K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 3200
//  and CCT of adopted white set to 2200K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 3200.0;
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
	rOut = r_lin * 7.5104059508573e-01 + g_lin * 9.1469315228211e-02 + b_lin * 1.5749008968606e-01;
	gOut = r_lin * -2.0937322835474e-02 + g_lin * 1.0577652170109e+00 + b_lin * -3.6827894175455e-02;
	bOut = r_lin * 2.6912411913694e-02 + g_lin * -5.6979322917938e-01 + b_lin * 1.5428808172657e+00;
	aOut = 1.0;

}
