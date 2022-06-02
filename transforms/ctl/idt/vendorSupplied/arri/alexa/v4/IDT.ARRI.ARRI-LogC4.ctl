// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.ARRI-LogC4.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI LogC4 </ACESuserName>

// ARRI IDT for ARRI LogC4


// Constants
const float a = (pow(2.0, 18.0) - 16.0) / 117.45;
const float b = (1023.0 - 95.0) / 1023.0;
const float c = 95.0 / 1023.0;
const float s = (7.0 * log(2.0) * pow(2.0, 7.0 - 14.0 * c / b)) / (a * b);
const float t = (pow(2.0, 14.0 * (-c / b) + 6.0) - 64.0) / a;

// LogC4 Curve Decoding Function
float normalizedLogC4ToRelativeSceneLinear( float x) {

    if (x < 0.0) {
        return x * s + t;
    }

    float p = 14.0 * (x - c) / b + 6.0;
    return (pow(2.0, p) - 64.0) / a;
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

	float r_lin = normalizedLogC4ToRelativeSceneLinear(rIn);
	float g_lin = normalizedLogC4ToRelativeSceneLinear(gIn);
	float b_lin = normalizedLogC4ToRelativeSceneLinear(bIn);

    // Matrix AWG4 D65 --CAT02--> ACES AP0 D60
	rOut = r_lin *  0.7509573628 + g_lin *  0.1444227867 + b_lin *  0.1046198505;
	gOut = r_lin *  0.0008218371 + g_lin *  1.0073975849 + b_lin * -0.0082194220;
	bOut = r_lin * -0.0004999521 + g_lin * -0.0008541772 + b_lin *  1.0013541294;
	aOut = 1.0;

}