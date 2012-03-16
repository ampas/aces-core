
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 400
//  and CCT of adopted white set to 11000K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.121428)
		return (pow(10,(x - 0.391007) / 0.256598) - 0.089004) / 5.061087;
	else
		return (x - 0.121428) / 5.656190;
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

	rOut = r_lin * 0.856168 + g_lin * 0.184553 + b_lin * -0.040721;
	gOut = r_lin * 0.102620 + g_lin * 1.173687 + b_lin * -0.276307;
	bOut = r_lin * 0.051004 + g_lin * -0.258938 + b_lin * 1.207934;
	aOut = 1.0;

}
