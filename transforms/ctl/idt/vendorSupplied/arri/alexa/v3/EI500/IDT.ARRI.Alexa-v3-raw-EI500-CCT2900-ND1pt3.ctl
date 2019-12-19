
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI500-CCT2900-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI500, 2900K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 500
//  and CCT of adopted white set to 2900K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 500.0;
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
	rOut = r_lin * 7.4412489005325e-01 + g_lin * 1.1519388755778e-01 + b_lin * 1.4068122238897e-01;
	gOut = r_lin * 2.3210900582781e-02 + g_lin * 1.0288310232817e+00 + b_lin * -5.2041923864458e-02;
	bOut = r_lin * 6.1165459801368e-02 + g_lin * -5.2391327003678e-01 + b_lin * 1.4627478102354e+00;
	aOut = 1.0;

}
