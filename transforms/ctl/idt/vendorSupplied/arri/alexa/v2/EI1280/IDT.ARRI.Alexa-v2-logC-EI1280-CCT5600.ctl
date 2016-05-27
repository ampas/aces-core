
// <ACEStransformID>IDT.ARRI.Alexa-v2-logC-EI1280-CCT5600.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI V2 LogC (EI1280, 5600K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 1280
//  and CCT of adopted white set to 5600K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

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

	rOut = r_lin * 0.798857 + g_lin * 0.152589 + b_lin * 0.048555;
	gOut = r_lin * 0.077016 + g_lin * 1.089708 + b_lin * -0.166724;
	bOut = r_lin * 0.042605 + g_lin * -0.280736 + b_lin * 1.238131;
	aOut = 1.0;

}
