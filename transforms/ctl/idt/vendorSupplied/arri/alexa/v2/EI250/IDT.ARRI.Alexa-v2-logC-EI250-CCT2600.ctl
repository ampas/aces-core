
// <ACEStransformID>IDT.ARRI.Alexa-v2-logC-EI250-CCT2600.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI V2 LogC (EI250, 2600K)</ACESuserName>

// ARRI ALEXA IDT for ALEXA logC files
//  with camera EI set to 250
//  and CCT of adopted white set to 2600K
// Written by v2_IDT_maker.py v0.05 on Friday 19 December 2014

float
normalizedLogC2ToRelativeExposure(float x) {
	if (x > 0.114725)
		return (pow(10,(x - 0.391007) / 0.262978) - 0.089004) / 5.061087;
	else
		return (x - 0.114725) / 6.034414;
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

	rOut = r_lin * 0.784282 + g_lin * 0.070865 + b_lin * 0.144852;
	gOut = r_lin * 0.022122 + g_lin * 1.041631 + b_lin * -0.063753;
	bOut = r_lin * 0.028916 + g_lin * -0.355789 + b_lin * 1.326872;
	aOut = 1.0;

}
