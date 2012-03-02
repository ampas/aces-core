
// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 500
//  and CCT of adopted white set to 5600K
// Written by v3_IDT_maker.py v0.06 on Thursday 01 March 2012 by alex

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
	rOut = r_lin * 0.745860 + g_lin * 0.174608 + b_lin * 0.079532;
	gOut = r_lin * 0.070558 + g_lin * 1.062302 + b_lin * -0.132861;
	bOut = r_lin * 0.086383 + g_lin * -0.444339 + b_lin * 1.357957;
	aOut = 1.0;

}
