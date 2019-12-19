
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.Alexa-v3-raw-EI640-CCT9000.a1.v2</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRIRAW (EI640, 9000K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA linear files
//  with camera EI set to 640
//  and CCT of adopted white set to 9000K
// Written by v3_IDT_maker.py v0.09 on Thursday 22 December 2016

const float EI = 640.0;
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
	rOut = r_lin * 8.3760896555078e-01 + g_lin * 1.8063041702187e-01 + b_lin * -1.8239382572650e-02;
	gOut = r_lin * 9.5639237216296e-02 + g_lin * 1.1525538851626e+00 + b_lin * -2.4819312237886e-01;
	bOut = r_lin * 4.7538209115456e-02 + g_lin * -2.5944785373071e-01 + b_lin * 1.2119096446152e+00;
	aOut = 1.0;

}
