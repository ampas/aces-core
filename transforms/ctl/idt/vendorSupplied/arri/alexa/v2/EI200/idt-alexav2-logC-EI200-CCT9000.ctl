
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 200
//  and CCT of adopted white set to 9000K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.111543)
		return (pow(10,(x - 0.391007) / 0.266007) - 0.089004) / 5.061087;
	else
		return (x - 0.111543) / 6.189953;
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

	rOut = r_lin * 0.839546 + g_lin * 0.176921 + b_lin * -0.016467;
	gOut = r_lin * 0.097104 + g_lin * 1.149298 + b_lin * -0.246402;
	bOut = r_lin * 0.049370 + g_lin * -0.263311 + b_lin * 1.213941;
	aOut = 1.0;

}
