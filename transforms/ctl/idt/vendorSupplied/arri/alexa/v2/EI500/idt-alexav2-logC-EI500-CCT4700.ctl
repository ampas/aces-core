
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 500
//  and CCT of adopted white set to 4700K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.124610)
		return (pow(10,(x - 0.391007) / 0.253569) - 0.089004) / 5.061087;
	else
		return (x - 0.124610) / 5.449261;
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

	rOut = r_lin * 0.790584 + g_lin * 0.133665 + b_lin * 0.075751;
	gOut = r_lin * 0.068954 + g_lin * 1.065978 + b_lin * -0.134932;
	bOut = r_lin * 0.041900 + g_lin * -0.298330 + b_lin * 1.256430;
	aOut = 1.0;

}
