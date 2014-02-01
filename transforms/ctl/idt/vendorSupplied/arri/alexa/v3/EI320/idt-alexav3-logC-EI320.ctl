
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 320
// Written by v3_IDT_maker.py v0.08 on Wednesday 20 November 2013 by jgoldstone

float
normalizedLogCToRelativeExposure(float x) {
	if (x > 0.135761)
		return (pow(10,(x - 0.383508) / 0.259627) - 0.068767) / 5.555556;
	else
		return (x - 0.092791) / 5.637726;
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

	float r_lin = normalizedLogCToRelativeExposure(rIn);
	float g_lin = normalizedLogCToRelativeExposure(gIn);
	float b_lin = normalizedLogCToRelativeExposure(bIn);

	rOut = r_lin * 0.680206 + g_lin * 0.236137 + b_lin * 0.083658;
	gOut = r_lin * 0.085415 + g_lin * 1.017471 + b_lin * -0.102886;
	bOut = r_lin * 0.002057 + g_lin * -0.062563 + b_lin * 1.060506;
	aOut = 1.0;

}
