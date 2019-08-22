// <ACEStransformID>IDT.Sony.Venice.SGamut3Cine</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Sony Venice Linear S-Gamut3.Cine</ACESuserName>
//
// Provided by Sony Electronics Corp.
//
//------------------------------------------------------------
//  VENICE S-Gamut3.Cine To AP0 matrix coeffs
//------------------------------------------------------------
const float matrixCoeff[3][3] =
{
	{  0.6742570921, -0.0093136061, -0.0382090673 },
	{  0.2205717359,  1.1059588614, -0.0179383766 },
	{  0.1051711720, -0.0966452553,  1.0561474439 }
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
