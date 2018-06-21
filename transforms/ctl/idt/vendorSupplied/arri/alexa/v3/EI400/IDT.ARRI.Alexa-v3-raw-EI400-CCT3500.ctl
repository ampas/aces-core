
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI400-CCT3500.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI400, 3500K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 400
//  and CCT of adopted white set to 3500K
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
	rOut = r_lin * 7.8406490815281e-01 + g_lin * 1.0305977074015e-01 + b_lin * 1.1287532110704e-01;
	gOut = r_lin * 5.0690108991893e-02 + g_lin * 1.0470164413216e+00 + b_lin * -9.7706550313499e-02;
	bOut = r_lin * 3.5986641800015e-02 + g_lin * -3.1560659303024e-01 + b_lin * 1.2796199512302e+00;
	aOut = 1.0;

}
