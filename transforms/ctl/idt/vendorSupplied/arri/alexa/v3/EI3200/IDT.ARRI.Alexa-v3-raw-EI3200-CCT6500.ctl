
// <ACEStransformID>IDT.ARRI.Alexa-v3-raw-EI3200-CCT6500.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI3200, 6500K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 3200
//  and CCT of adopted white set to 6500K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 3200.0;
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
	rOut = r_lin * 8.0993138448319e-01 + g_lin * 1.6274056090100e-01 + b_lin * 2.7328054615806e-02;
	gOut = r_lin * 8.3730895599753e-02 + g_lin * 1.1086665559927e+00 + b_lin * -1.9239745159242e-01;
	bOut = r_lin * 4.4166429898004e-02 + g_lin * -2.7203799179867e-01 + b_lin * 1.2278715619007e+00;
	aOut = 1.0;

}
