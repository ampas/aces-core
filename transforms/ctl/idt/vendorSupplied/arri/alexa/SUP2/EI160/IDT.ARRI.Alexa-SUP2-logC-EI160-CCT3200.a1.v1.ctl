
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 160
//  and CCT of adopted white set to 3200K
// SUP 2.0
// Written by SUP2_IDT_maker.py v0.05 on Thursday 18 December 2014

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.108361)
		return (pow(10,(x - 0.391007) / 0.269035) - 0.089004) / 5.061087;
	else
		return (x - 0.108361) / 6.332427;
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

	rOut = r_lin * 0.782790 + g_lin * 0.093723 + b_lin * 0.123486;
	gOut = r_lin * 0.043431 + g_lin * 1.042236 + b_lin * -0.085667;
	bOut = r_lin * 0.034385 + g_lin * -0.326687 + b_lin * 1.292301;
	aOut = 1.0;

}
