

//
// IDT for 12-bit S-Log/S-Gamut Sony cameras
// Provided by Sony Electronics Corp.
//





/* ============ CONSTANTS ============ */
const float SGAMUT_TO_ACES_MTX[3][3] = {
	{ 0.754338638,  0.021198141, -0.009756991 },
	{ 0.133697046,  1.005410934,  0.004508563 },
	{ 0.111968437, -0.026610548,  1.005253201 }
};





/* ============ SUBFUNCTIONS ============ */
float SLog12_to_lin (
	float SLog
)
{
	return (pow(10,((((SLog/16-16)/219)-0.616596-0.03)/0.432699))-0.037584)*0.9;
}





/* ============ Main Algorithm ============ */
void
main
(   input varying float rIn,
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
	SLog[0] = rIn * 4095.0;
	SLog[1] = gIn * 4095.0;
	SLog[2] = bIn * 4095.0;

	// 12-bit Sony S-log to linear S-gamut
	float lin[3];
	lin[0] = SLog12_to_lin( SLog[0]);
	lin[1] = SLog12_to_lin( SLog[1]);
	lin[2] = SLog12_to_lin( SLog[2]);

	// S-Gamut to ACES matrix
	float aces[3] = mult_f3_f33( lin, SGAMUT_TO_ACES_MTX);
	
	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}