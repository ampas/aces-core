
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 800
//  and CCT of adopted white set to 2600K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.131313)
		return (pow(10,(x - 0.391007) / 0.247189) - 0.089004) / 5.061087;
	else
		return (x - 0.131313) / 4.950469;
}

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

	float r_lin = normalizedLogC2ToRelativeExposure(rIn);
	float g_lin = normalizedLogC2ToRelativeExposure(gIn);
	float b_lin = normalizedLogC2ToRelativeExposure(bIn);

	rOut = r_lin * 0.785591 + g_lin * 0.067735 + b_lin * 0.146674;
	gOut = r_lin * 0.023131 + g_lin * 1.038638 + b_lin * -0.061769;
	bOut = r_lin * 0.030306 + g_lin * -0.359182 + b_lin * 1.328876;
	aOut = 1.0;

}
