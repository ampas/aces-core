
// <ACEStransformID>IDT.ARRI.Alexa-v2-logC-EI640-CCT7500.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI V2 LogC (EI640, 7500K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 640
//  and CCT of adopted white set to 7500K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.128130)
		return (pow(10,(x - 0.391007) / 0.250218) - 0.089004) / 5.061087;
	else
		return (x - 0.128130) / 5.198031;
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

	rOut = r_lin * 0.821930 + g_lin * 0.171791 + b_lin * 0.006279;
	gOut = r_lin * 0.089522 + g_lin * 1.128531 + b_lin * -0.218053;
	bOut = r_lin * 0.045851 + g_lin * -0.265380 + b_lin * 1.219530;
	aOut = 1.0;

}
