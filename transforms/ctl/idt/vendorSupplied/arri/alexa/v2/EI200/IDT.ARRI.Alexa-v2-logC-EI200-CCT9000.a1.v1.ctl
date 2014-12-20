
// <ACEStransformID>IDT.ARRI.Alexa-v2-logC-EI200-CCT9000.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI V2 LogC (EI200, 9000K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 200
//  and CCT of adopted white set to 9000K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

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

	rOut = r_lin * 0.837609 + g_lin * 0.180630 + b_lin * -0.018239;
	gOut = r_lin * 0.095639 + g_lin * 1.152554 + b_lin * -0.248193;
	bOut = r_lin * 0.047538 + g_lin * -0.259448 + b_lin * 1.211910;
	aOut = 1.0;

}
