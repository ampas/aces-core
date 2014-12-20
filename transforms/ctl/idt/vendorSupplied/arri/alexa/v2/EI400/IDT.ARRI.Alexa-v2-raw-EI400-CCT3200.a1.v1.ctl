
// <ACEStransformID>IDT.ARRI.Alexa-v2-raw-EI400-CCT3200.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI400, 3200K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 400
//  and CCT of adopted white set to 3200K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

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
	rOut = r_lin * 0.782790 + g_lin * 0.093723 + b_lin * 0.123486;
	gOut = r_lin * 0.043431 + g_lin * 1.042236 + b_lin * -0.085667;
	bOut = r_lin * 0.034385 + g_lin * -0.326687 + b_lin * 1.292301;
	aOut = 1.0;

}
