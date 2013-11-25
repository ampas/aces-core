
// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 160
//  and CCT of adopted white set to 2600K
// Written by v2_IDT_maker.py v0.05 on Wednesday 20 November 2013 by jgoldstone

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
	rOut = r_lin * 0.784282 + g_lin * 0.070865 + b_lin * 0.144852;
	gOut = r_lin * 0.022122 + g_lin * 1.041631 + b_lin * -0.063753;
	bOut = r_lin * 0.028916 + g_lin * -0.355789 + b_lin * 1.326872;
	aOut = 1.0;

}
