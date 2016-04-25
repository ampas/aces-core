// <ACEStransformID>IDT.Sony.SLog3_SGamut3Cine.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Sony SLog3 SGamut3Cine</ACESuserName>

// Provided by Sony Electronics Corp.
//

//------------------------------------------------------------------------------------
//  S-Gamut 3.Cine To ACES(Primaries0) matrix
//------------------------------------------------------------------------------------
const float matrixCoef[3][3] =
{
	{  0.6387886672, -0.0039159060, -0.0299072021 },
	{  0.2723514337,  1.0880732309, -0.0264325799 },
	{  0.0888598991, -0.0841573249,  1.0563397820 }
};

//------------------------------------------------------------------------------------
//  S-Log 3 to linear
//------------------------------------------------------------------------------------
float SLog3_to_linear( float SLog )
{
	float out;

	if (SLog >= 171.2102946929 / 1023.0)
	{
		out = pow(10.0, (SLog*1023.0-420.0)/261.5)*(0.18+0.01)-0.01;
	}
	else
	{
		out = (SLog*1023.0-95.0)*0.01125000/(171.2102946929-95.0);
	}

	return out;
}

//------------------------------------------------------------------------------------
//  main
//------------------------------------------------------------------------------------
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
	float SLog3[3];
	SLog3[0] = rIn;
	SLog3[1] = gIn;
	SLog3[2] = bIn;

	float linear[3];
	linear[0] = SLog3_to_linear( SLog3[0] );
	linear[1] = SLog3_to_linear( SLog3[1] );
	linear[2] = SLog3_to_linear( SLog3[2] );

	float ACES[3] = mult_f3_f33( linear, matrixCoef );

	rOut = ACES[0];
	gOut = ACES[1];
	bOut = ACES[2];
	aOut = aIn;
}
