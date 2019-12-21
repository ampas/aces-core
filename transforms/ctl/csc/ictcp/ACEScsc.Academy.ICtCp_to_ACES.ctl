
// <ACEStransformID>ACEScsc.ICtCp_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>ICtCp to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - ICtCp to ACES
//
// converts ICtCp to 
//          ACES2065-1 (AP0 w/ linear encoding)
//




// The LMS_2_AP0_MAT is the matrix inverse of the AP0_2_LMS_MAT defined in the 
// accompanying transform.
const float LMS_2_AP0_MAT[3][3] = {
  { 2.2034860017, -0.5267000086, -0.0465914122},
  {-1.4028871323,  1.5838401289, -0.0457828327},
  { 0.1994183978, -0.0571107433,  1.0924829098}
};


// The ICpCt_2_LMSp_MAT is the matrix inverse of the LMSp_2_ICpCt_MAT defined
// in the accompanying transform.
const float ICtCp_2_LMSp_MAT[3][3] = {
  { 1.0,  1.0,  1.0},
  { 0.0086064753, -0.0086064753,  0.5600463058},
  { 0.1110335306, -0.1110335306, -0.3206319566}
};


// Constants from SMPTE ST 2084-2014
const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
const float pq_m2 = 78.84375; // ( 2523.0 / 4096.0 ) * 128.0;
const float pq_c1 = 0.8359375; // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
const float pq_c2 = 18.8515625; // ( 2413.0 / 4096.0 ) * 32.0;
const float pq_c3 = 18.6875; // ( 2392.0 / 4096.0 ) * 32.0;

const float pq_C = 10000.0;

// Converts from the non-linear perceptually quantized space to linear cd/m^2
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float ST2084_2_Y( float N )
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this assumes full range (0 - 1)
  float Np = pow( N, 1.0 / pq_m2 );
  float L = Np - pq_c1;
  if ( L < 0.0 )
    L = 0.0;
  L = L / ( pq_c2 - pq_c3 * Np );
  L = pow( L, 1.0 / pq_m1 );
  return L * pq_C; // returns cd/m^2
}

// Scale factor equal to PQ_rev( lin_2_acescct( 0.18) ) / 0.18
const float scale = 209.;



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
    float ICtCp[3] = { rIn, gIn, bIn};

    // Calculate LMSp
    float LMSp[3] = mult_f3_f33( ICtCp, ICtCp_2_LMSp_MAT);

    // Apply ST 2084 non-linearity
    float LMS[3];
    LMS[0] = ST2084_2_Y( LMSp[0]);
    LMS[1] = ST2084_2_Y( LMSp[1]);
    LMS[2] = ST2084_2_Y( LMSp[2]);

    // Calculate ACES
    float ACES[3] = mult_f3_f33( LMS, LMS_2_AP0_MAT);
    
    // Scale ACES
    ACES = mult_f_f3( 1./scale, ACES);
    
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}