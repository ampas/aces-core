
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 1000
//  and CCT of adopted white set to 3500K
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

	rOut = r_lin * 0.784065 + g_lin * 0.103060 + b_lin * 0.112875;
	gOut = r_lin * 0.050690 + g_lin * 1.047016 + b_lin * -0.097707;
	bOut = r_lin * 0.035987 + g_lin * -0.315607 + b_lin * 1.279620;
	aOut = 1.0;

}
