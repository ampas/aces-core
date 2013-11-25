
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 1600
//  and CCT of adopted white set to 2000K
// Written by v2_IDT_maker.py v0.05 on Wednesday 20 November 2013 by jgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.141197)
		return (pow(10,(x - 0.391007) / 0.237781) - 0.089004) / 5.061087;
	else
		return (x - 0.141197) / 4.070466;
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

	rOut = r_lin * 0.794897 + g_lin * 0.044675 + b_lin * 0.160428;
	gOut = r_lin * -0.020296 + g_lin * 1.077981 + b_lin * -0.057685;
	bOut = r_lin * 0.012145 + g_lin * -0.384829 + b_lin * 1.372683;
	aOut = 1.0;

}
