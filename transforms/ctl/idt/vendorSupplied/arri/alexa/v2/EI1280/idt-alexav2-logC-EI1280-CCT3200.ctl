
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 1280
//  and CCT of adopted white set to 3200K
// Written by v2_IDT_maker.py v0.05 on Saturday 10 March 2012 by josephgoldstone

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.138015)
		return (pow(10,(x - 0.391007) / 0.240810) - 0.089004) / 5.061087;
	else
		return (x - 0.138015) / 4.369609;
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

	rOut = r_lin * 0.784193 + g_lin * 0.090441 + b_lin * 0.125367;
	gOut = r_lin * 0.044520 + g_lin * 1.039130 + b_lin * -0.083650;
	bOut = r_lin * 0.035830 + g_lin * -0.330242 + b_lin * 1.294413;
	aOut = 1.0;

}
