
// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 1280
//  and CCT of adopted white set to 3500K
// Written by v3_IDT_maker.py v0.07 on Saturday 10 March 2012 by josephgoldstone

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
	rOut = r_lin * 0.785505 + g_lin * 0.099743 + b_lin * 0.114753;
	gOut = r_lin * 0.051809 + g_lin * 1.043895 + b_lin * -0.095704;
	bOut = r_lin * 0.037452 + g_lin * -0.319189 + b_lin * 1.281737;
	aOut = 1.0;

}
