
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI160-CCT2600-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI160, 2600K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 160
//  and CCT of adopted white set to 2600K
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
	rOut = r_lin * 7.4640268401676e-01 + g_lin * 1.0460454267072e-01 + b_lin * 1.4899277331252e-01;
	gOut = r_lin * 9.0267361135148e-03 + g_lin * 1.0335756136084e+00 + b_lin * -4.2602349721919e-02;
	bOut = r_lin * 5.1508147528781e-02 + g_lin * -5.4349291126732e-01 + b_lin * 1.4919847637385e+00;
	aOut = 1.0;

}
