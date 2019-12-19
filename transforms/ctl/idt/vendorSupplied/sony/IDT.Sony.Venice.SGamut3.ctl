// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.Sony.Venice.SGamut3.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Sony Venice Linear S-Gamut3</ACESuserName>
//
// Provided by Sony Electronics Corp.
//
//------------------------------------------------------------
//  VENICE S-Gamut3 To AP0 matrix coeffs
//------------------------------------------------------------
const float matrixCoeff[3][3] =
{
	{  0.7933297411,  0.0155810585, -0.0188647478 },
	{  0.0890786256,  1.0327123069,  0.0127694121 },
	{  0.1175916333, -0.0482933654,  1.0060953358 }
};

//------------------------------------------------------------
//  main
//------------------------------------------------------------
void main (
	input varying float rIn,
	input varying float gIn,
	input varying float bIn,
	input varying float aIn,
	output varying float rOut,
	output varying float gOut,
	output varying float bOut,
	output varying float aOut )
{
	float linear[3];
	linear[0] = rIn;
	linear[1] = gIn;
	linear[2] = bIn;

	float aces[3] = mult_f3_f33( linear, matrixCoeff );

	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}
