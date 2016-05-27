
// <ACEStransformID>IDT.Sony.SLog1_SGamut_12i.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - Sony SLog1</ACESuserName>

//
// IDT for Sony Cameras - 12 bits
// Provided by Sony Electronics Corp.
//





/* ============ CONSTANTS ============ */
const float SGAMUT_TO_ACES_MTX[3][3] = {
	{ 0.754338638,  0.021198141, -0.009756991 },
	{ 0.133697046,  1.005410934,  0.004508563 },
	{ 0.111968437, -0.026610548,  1.005253201 }
};

const float B = 256.;
const float AB = 360.;
const float W = 3760.;





/* ============ SUBFUNCTIONS ============ */
float SLog1_to_lin 
(
	float SLog,
	float b,
	float ab,
	float w
)
{
	float lin;
	
	if (SLog >= ab)
		lin = ( pow(10., ( ( ( SLog - b) / ( w - b) - 0.616596 - 0.03) / 0.432699)) - 0.037584) * 0.9;
	else if (SLog < ab) 
		lin = ( ( ( SLog - b) / ( w - b) - 0.030001222851889303) / 5.) * 0.9;
	
	return lin;
	
}





/* ============ Main Algorithm ============ */
void main
(
    input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
	// Prepare input values based on application bit depth handling
	float SLog[3];
	SLog[0] = rIn * 4095.;
	SLog[1] = gIn * 4095.;
	SLog[2] = bIn * 4095.;

	// 12-bit Sony S-log to linear S-gamut
	float lin[3];
	lin[0] = SLog1_to_lin( SLog[0], B, AB, W);
	lin[1] = SLog1_to_lin( SLog[1], B, AB, W);
	lin[2] = SLog1_to_lin( SLog[2], B, AB, W);

	// S-Gamut to ACES matrix
	float aces[3] = mult_f3_f33( lin, SGAMUT_TO_ACES_MTX);
	
	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}