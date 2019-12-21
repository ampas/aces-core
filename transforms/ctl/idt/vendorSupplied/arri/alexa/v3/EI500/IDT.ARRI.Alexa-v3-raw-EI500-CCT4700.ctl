
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI500-CCT4700.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI500, 4700K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 500
//  and CCT of adopted white set to 4700K
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
	rOut = r_lin * 7.8896941351304e-01 + g_lin * 1.3720946916483e-01 + b_lin * 7.3821117322125e-02;
	gOut = r_lin * 6.7712183237801e-02 + g_lin * 1.0692321478819e+00 + b_lin * -1.3694433111975e-01;
	bOut = r_lin * 4.0314052398722e-02 + g_lin * -2.9456041305294e-01 + b_lin * 1.2542463606542e+00;
	aOut = 1.0;

}
