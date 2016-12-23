
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI500-CCT2600.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI500, 2600K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 500
//  and CCT of adopted white set to 2600K
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
	rOut = r_lin * 7.8428242539315e-01 + g_lin * 7.0865398205144e-02 + b_lin * 1.4485217640170e-01;
	gOut = r_lin * 2.2122031844680e-02 + g_lin * 1.0416308060182e+00 + b_lin * -6.3752837862853e-02;
	bOut = r_lin * 2.8916486462163e-02 + g_lin * -3.5578875136390e-01 + b_lin * 1.3268722649017e+00;
	aOut = 1.0;

}
