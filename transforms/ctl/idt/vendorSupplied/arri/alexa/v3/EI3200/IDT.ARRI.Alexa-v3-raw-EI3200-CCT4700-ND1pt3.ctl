
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI3200-CCT4700-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI3200, 4700K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 3200
//  and CCT of adopted white set to 4700K
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
	rOut = r_lin * 7.3923135476030e-01 + g_lin * 1.6771636954827e-01 + b_lin * 9.3052275691432e-02;
	gOut = r_lin * 5.9358921309018e-02 + g_lin * 1.0498828140348e+00 + b_lin * -1.0924173534377e-01;
	bOut = r_lin * 7.7920559256699e-02 + g_lin * -4.5361956386311e-01 + b_lin * 1.3756990046064e+00;
	aOut = 1.0;

}
