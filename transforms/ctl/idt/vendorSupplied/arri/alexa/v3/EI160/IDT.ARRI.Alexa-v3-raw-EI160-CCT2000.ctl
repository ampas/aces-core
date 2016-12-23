
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI160-CCT2000.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI160, 2000K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 160
//  and CCT of adopted white set to 2000K
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
	rOut = r_lin * 7.9489710714933e-01 + g_lin * 4.4674719188390e-02 + b_lin * 1.6042817366228e-01;
	gOut = r_lin * -2.0296064360422e-02 + g_lin * 1.0779807174559e+00 + b_lin * -5.7684653095483e-02;
	bOut = r_lin * 1.2145353281210e-02 + g_lin * -3.8482859978041e-01 + b_lin * 1.3726832464992e+00;
	aOut = 1.0;

}
