
// <ACEStransformID>ACEScsc.ADX10_to_ACES.a1.0.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - ADX10</ACESuserName>

// 
// Universal ADX10 to ACES Transform
//



// ============ CONSTANTS ============ //
const float CDD_TO_CID[3][3] = {
	{0.75573, 0.05901, 0.16134},
	{0.22197, 0.96928, 0.07406},
	{0.02230, -0.02829, 0.76460}
};

const float EXP_TO_ACES[3][3] = {
	{0.72286, 0.11923, 0.01427},
	{0.12630, 0.76418, 0.08213},
	{0.15084, 0.11659, 0.90359}
};

const float LUT_1D[11][2] = {
	{-0.190000000000000, -6.000000000000000},
	{ 0.010000000000000, -2.721718645000000},
	{ 0.028000000000000, -2.521718645000000},
	{ 0.054000000000000, -2.321718645000000},
	{ 0.095000000000000, -2.121718645000000},
	{ 0.145000000000000, -1.921718645000000},
	{ 0.220000000000000, -1.721718645000000},
	{ 0.300000000000000, -1.521718645000000},
	{ 0.400000000000000, -1.321718645000000},
	{ 0.500000000000000, -1.121718645000000},
	{ 0.600000000000000, -0.926545676714876}
};

const float REF_PT = (7120.0 - 1520.0) / 8000.0 * (100.0 / 55.0) - log10(0.18);





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
	float adx[3];
  adx[0] = rIn * 1023.0;
	adx[1] = gIn * 1023.0;
	adx[2] = bIn * 1023.0;

	// Convert ADX10 values to Channel Dependent Density values
	float cdd[3];
	cdd[0] = ( adx[0] - 95.0) / 500.0;
	cdd[1] = ( adx[1] - 95.0) / 500.0;
	cdd[2] = ( adx[2] - 95.0) / 500.0;

	// Convert Channel Dependent Density values into Channel Independent Density values
	float cid[3] = mult_f3_f33( cdd, CDD_TO_CID);
	
	// Convert Channel Independent Density values to Relative Log Exposure values
	float logE[3];
	if ( cid[0] <= 0.6) logE[0] = interpolate1D( LUT_1D, cid[0]);
	if ( cid[1] <= 0.6) logE[1] = interpolate1D( LUT_1D, cid[1]);
	if ( cid[2] <= 0.6) logE[2] = interpolate1D( LUT_1D, cid[2]);
	
	if ( cid[0] > 0.6) logE[0] = ( 100.0 / 55.0) * cid[0] - REF_PT;
	if ( cid[1] > 0.6) logE[1] = ( 100.0 / 55.0) * cid[1] - REF_PT;
	if ( cid[2] > 0.6) logE[2] = ( 100.0 / 55.0) * cid[2] - REF_PT;

	// Convert Relative Log Exposure values to Relative Exposure values
	float exp[3];
	exp[0] = pow( 10, logE[0]);
	exp[1] = pow( 10, logE[1]);
	exp[2] = pow( 10, logE[2]);

	// Convert Relative Exposure values to ACES values
	float aces[3] = mult_f3_f33( exp, EXP_TO_ACES);

	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;	
}