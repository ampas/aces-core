
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI400-CCT5600.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI400, 5600K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 400
//  and CCT of adopted white set to 5600K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 400.0;
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
	rOut = r_lin * 7.9885684048660e-01 + g_lin * 1.5258852177229e-01 + b_lin * 4.8554637741112e-02;
	gOut = r_lin * 7.7015815731790e-02 + g_lin * 1.0897077843209e+00 + b_lin * -1.6672360005270e-01;
	bOut = r_lin * 4.2604928338653e-02 + g_lin * -2.8073642662267e-01 + b_lin * 1.2381314982840e+00;
	aOut = 1.0;

}
