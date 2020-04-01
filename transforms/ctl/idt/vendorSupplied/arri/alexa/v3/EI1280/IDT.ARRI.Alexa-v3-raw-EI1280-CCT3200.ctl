
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI1280-CCT3200.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI1280, 3200K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 1280
//  and CCT of adopted white set to 3200K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 1280.0;
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
	rOut = r_lin * 7.8279029723869e-01 + g_lin * 9.3723256574718e-02 + b_lin * 1.2348644618659e-01;
	gOut = r_lin * 4.3430608649221e-02 + g_lin * 1.0422361331474e+00 + b_lin * -8.5666741796593e-02;
	bOut = r_lin * 3.4385400745453e-02 + g_lin * -3.2668689715477e-01 + b_lin * 1.2923014964093e+00;
	aOut = 1.0;

}
