
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI400-CCT3900-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI400, 3900K, ND1.3)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 400
//  and CCT of adopted white set to 3900K
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
	rOut = r_lin * 7.4224343228049e-01 + g_lin * 1.4357774534641e-01 + b_lin * 1.1417882237310e-01;
	gOut = r_lin * 4.9267162740309e-02 + g_lin * 1.0398579892377e+00 + b_lin * -8.9125151978010e-02;
	bOut = r_lin * 7.5846304261200e-02 + g_lin * -4.7743791264021e-01 + b_lin * 1.4015916083790e+00;
	aOut = 1.0;

}
