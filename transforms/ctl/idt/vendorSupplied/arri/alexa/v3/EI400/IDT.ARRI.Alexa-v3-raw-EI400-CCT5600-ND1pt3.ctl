
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI400-CCT5600-ND1pt3.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI400, 5600K, ND1.3)</ACESuserName>

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
	rOut = r_lin * 7.4189908237715e-01 + g_lin * 1.8215992929323e-01 + b_lin * 7.5940988329617e-02;
	gOut = r_lin * 6.7694637031432e-02 + g_lin * 1.0688798646777e+00 + b_lin * -1.3657450170913e-01;
	bOut = r_lin * 8.2411071812710e-02 + g_lin * -4.3620136182617e-01 + b_lin * 1.3537902900135e+00;
	aOut = 1.0;

}
