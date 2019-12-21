// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.Sony.SLog3_SGamut3.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Sony SLog3 SGamut3</ACESuserName>

// Provided by Sony Electronics Corp.
//

//------------------------------------------------------------------------------------
//  S-Gamut 3 To ACES(Primaries0) matrix
//------------------------------------------------------------------------------------
const float matrixCoef[3][3] =
{
	{  0.7529825954,  0.0217076974, -0.0094160528 },
	{  0.1433702162,  1.0153188355,  0.0033704179 },
	{  0.1036471884, -0.0370265329,  1.0060456349 }
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
