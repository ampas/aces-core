
// <ACEStransformID>IDT.ARRI.Alexa-v2-logC-EI160-CCT2000.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI V2 LogC (EI160, 2000K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 160
//  and CCT of adopted white set to 2000K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

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

	rOut = r_lin * 0.794897 + g_lin * 0.044675 + b_lin * 0.160428;
	gOut = r_lin * -0.020296 + g_lin * 1.077981 + b_lin * -0.057685;
	bOut = r_lin * 0.012145 + g_lin * -0.384829 + b_lin * 1.372683;
	aOut = 1.0;

}
