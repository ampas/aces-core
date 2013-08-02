
// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 200
//  and CCT of adopted white set to 2200K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

const float EI = 200.0;
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
	rOut = r_lin * 0.791119 + g_lin * 0.050240 + b_lin * 0.158641;
	gOut = r_lin * -0.001191 + g_lin * 1.055091 + b_lin * -0.053900;
	bOut = r_lin * 0.021743 + g_lin * -0.380933 + b_lin * 1.359190;
	aOut = 1.0;

}
