
// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 500
//  and CCT of adopted white set to 5600K
// SUP 2.0
// Written by SUP2_IDT_maker.py v0.05 on Thursday 18 December 2014

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

	rOut = r_lin * 0.798857 + g_lin * 0.152589 + b_lin * 0.048555;
	gOut = r_lin * 0.077016 + g_lin * 1.089708 + b_lin * -0.166724;
	bOut = r_lin * 0.042605 + g_lin * -0.280736 + b_lin * 1.238131;
	aOut = 1.0;

}
