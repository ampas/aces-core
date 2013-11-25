
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 1000
//  and CCT of adopted white set to 2900K
// Written by v2_IDT_maker.py v0.05 on Wednesday 20 November 2013 by jgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.134495)
		return (pow(10,(x - 0.391007) / 0.244161) - 0.089004) / 5.061087;
	else
		return (x - 0.134495) / 4.684112;
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

	rOut = r_lin * 0.782719 + g_lin * 0.083022 + b_lin * 0.134259;
	gOut = r_lin * 0.034248 + g_lin * 1.039811 + b_lin * -0.074059;
	bOut = r_lin * 0.032204 + g_lin * -0.340046 + b_lin * 1.307842;
	aOut = 1.0;

}
