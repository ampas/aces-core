
// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 1000
//  and CCT of adopted white set to 3200K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

const float EI = 1000.0;
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
	rOut = r_lin * 0.784193 + g_lin * 0.090441 + b_lin * 0.125367;
	gOut = r_lin * 0.044520 + g_lin * 1.039130 + b_lin * -0.083650;
	bOut = r_lin * 0.035830 + g_lin * -0.330242 + b_lin * 1.294413;
	aOut = 1.0;

}
