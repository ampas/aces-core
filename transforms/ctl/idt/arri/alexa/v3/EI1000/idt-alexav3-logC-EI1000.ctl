
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 1000
// Written by v3_IDT_maker.py v0.07 on Saturday 10 March 2012 by josephgoldstone

float
normalizedLogCToRelativeExposure(float x) {
	if (x > 0.153048)
		return (pow(10,(x - 0.386036) / 0.244161) - 0.047995) / 5.555556;
	else
		return (x - 0.092814) / 5.301878;
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
