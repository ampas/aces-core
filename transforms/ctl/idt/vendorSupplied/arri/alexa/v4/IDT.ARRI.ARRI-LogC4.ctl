// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.ARRI.ARRI-LogC4.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ARRI LogC4 </ACESuserName>

// ARRI IDT for ARRI LogC4


// LogC4 Curve Decoding Function
float normalizedLogC4ToRelativeSceneLinear( float x) {

    // Constants
    const float a = (pow(2.0, 18.0) - 16.0) / 117.45;
    const float b = (1023.0 - 95.0) / 1023.0;
    const float c = 95.0 / 1023.0;
    const float s = (7.0 * log(2.0) * pow(2.0, 7.0 - 14.0 * c / b)) / (a * b);
    const float t = (pow(2.0, 14.0 * (-c / b) + 6.0) - 64.0) / a;

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

    // Matrix AWG4 D65 --CAT02--> ACES AP0 ACES White Point
	rOut = r_lin *  0.750957362824734131 + g_lin *  0.144422786709757084 + b_lin *  0.104619850465508965;
	gOut = r_lin *  0.000821837079380207 + g_lin *  1.007397584885003194 + b_lin * -0.008219421964383583;
	bOut = r_lin * -0.000499952143533471 + g_lin * -0.000854177231436971 + b_lin *  1.001354129374970370;
	aOut = 1.0;

}
