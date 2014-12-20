
// <ACEStransformID>ACESlib.Tonescales.a1.0.0</ACEStransformID>


// Textbook monomial to basis-function conversion matrix.
const float M[ 3][ 3] = {
  {  0.5, -1.0, 0.5 },
  { -1.0,  1.0, 0.5 },
  {  0.5,  0.0, 0.0 }
};



struct SplineMapPoint
{
  float x;
  float y;
};

struct SegmentedSplineParams_c5
{
  float coefsLow[6];    // coefs for B-spline between minPoint and midPoint (units of log luminance)
  float coefsHigh[6];   // coefs for B-spline between midPoint and maxPoint (units of log luminance)
  SplineMapPoint minPoint; // {luminance, luminance} linear extension below this
  SplineMapPoint midPoint; // {luminance, luminance} 
  SplineMapPoint maxPoint; // {luminance, luminance} linear extension above this
  float slopeLow;       // log-log slope of low linear extension
  float slopeHigh;      // log-log slope of high linear extension
};

struct SegmentedSplineParams_c9
{
  float coefsLow[10];    // coefs for B-spline between minPoint and midPoint (units of log luminance)
  float coefsHigh[10];   // coefs for B-spline between midPoint and maxPoint (units of log luminance)
  SplineMapPoint minPoint; // {luminance, luminance} linear extension below this
  SplineMapPoint midPoint; // {luminance, luminance} 
  SplineMapPoint maxPoint; // {luminance, luminance} linear extension above this
  float slopeLow;       // log-log slope of low linear extension
  float slopeHigh;      // log-log slope of high linear extension
};


const SegmentedSplineParams_c5 RRT_PARAMS =
{
  // coefsLow[6]
  { -4.0000000000, -4.0000000000, -3.1573765773, -0.4852499958, 1.8477324706, 1.8477324706 },
  // coefsHigh[6]
  { -0.7185482425, 2.0810307172, 3.6681241237, 4.0000000000, 4.0000000000, 4.0000000000 },
  { 0.18*pow(2.,-15), 0.0001},    // minPoint
  { 0.18,                4.8},    // midPoint  
  { 0.18*pow(2., 18), 10000.},    // maxPoint
  0.0,  // slopeLow
  0.0   // slopeHigh
};


float segmented_spline_c5_fwd
  ( 
    varying float x,
    varying SegmentedSplineParams_c5 C = RRT_PARAMS
  )
{
  const int N_KNOTS_LOW = 4;
  const int N_KNOTS_HIGH = 4;

  // Check for negatives or zero before taking the log. If negative or zero,
  // set to ACESMIN.
  float xCheck = x;
  if (xCheck <= 0.0) xCheck = pow(2., -14.); 

  float logx = log10( xCheck);

  float logy;

  if ( logx <= log10(C.minPoint.x) ) { 

    logy = logx * C.slopeLow + ( log10(C.minPoint.y) - C.slopeLow * log10(C.minPoint.x) );

  } else if (( logx > log10(C.minPoint.x) ) && ( logx < log10(C.midPoint.x) )) {

    float knot_coord = (N_KNOTS_LOW-1) * (logx-log10(C.minPoint.x))/(log10(C.midPoint.x)-log10(C.minPoint.x));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { C.coefsLow[ j], C.coefsLow[ j + 1], C.coefsLow[ j + 2]};
    // NOTE: If the running a version of CTL < 1.5, you may get an 
    // exception thrown error, usually accompanied by "Array index out of range" 
    // If you receive this error, it is recommended that you update to CTL v1.5, 
    // which contains a number of important bug fixes. Otherwise, you may try 
    // uncommenting the below, which is longer, but equivalent to, the above 
    // line of code.
    //
    // float cf[ 3];
    // if ( j <= 0) {
    //     cf[ 0] = C.coefsLow[0];  cf[ 1] = C.coefsLow[1];  cf[ 2] = C.coefsLow[2];
    // } else if ( j == 1) {
    //     cf[ 0] = C.coefsLow[1];  cf[ 1] = C.coefsLow[2];  cf[ 2] = C.coefsLow[3];
    // } else if ( j == 2) {
    //     cf[ 0] = C.coefsLow[2];  cf[ 1] = C.coefsLow[3];  cf[ 2] = C.coefsLow[4];
    // } else if ( j == 3) {
    //     cf[ 0] = C.coefsLow[3];  cf[ 1] = C.coefsLow[4];  cf[ 2] = C.coefsLow[5];
    // } else if ( j == 4) {
    //     cf[ 0] = C.coefsLow[4];  cf[ 1] = C.coefsLow[5];  cf[ 2] = C.coefsLow[6];
    // } else if ( j == 5) {
    //     cf[ 0] = C.coefsLow[5];  cf[ 1] = C.coefsLow[6];  cf[ 2] = C.coefsLow[7];
    // } else if ( j == 6) {
    //     cf[ 0] = C.coefsLow[6];  cf[ 1] = C.coefsLow[7];  cf[ 2] = C.coefsLow[8];
    // } 
    
    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else if (( logx >= log10(C.midPoint.x) ) && ( logx < log10(C.maxPoint.x) )) {

    float knot_coord = (N_KNOTS_HIGH-1) * (logx-log10(C.midPoint.x))/(log10(C.maxPoint.x)-log10(C.midPoint.x));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { C.coefsHigh[ j], C.coefsHigh[ j + 1], C.coefsHigh[ j + 2]}; 
    // NOTE: If the running a version of CTL < 1.5, you may get an 
    // exception thrown error, usually accompanied by "Array index out of range" 
    // If you receive this error, it is recommended that you update to CTL v1.5, 
    // which contains a number of important bug fixes. Otherwise, you may try 
    // uncommenting the below, which is longer, but equivalent to, the above 
    // line of code.
    //
    // float cf[ 3];
    // if ( j <= 0) {
    //     cf[ 0] = C.coefsHigh[0];  cf[ 1] = C.coefsHigh[1];  cf[ 2] = C.coefsHigh[2];
    // } else if ( j == 1) {
    //     cf[ 0] = C.coefsHigh[1];  cf[ 1] = C.coefsHigh[2];  cf[ 2] = C.coefsHigh[3];
    // } else if ( j == 2) {
    //     cf[ 0] = C.coefsHigh[2];  cf[ 1] = C.coefsHigh[3];  cf[ 2] = C.coefsHigh[4];
    // } else if ( j == 3) {
    //     cf[ 0] = C.coefsHigh[3];  cf[ 1] = C.coefsHigh[4];  cf[ 2] = C.coefsHigh[5];
    // } else if ( j == 4) {
    //     cf[ 0] = C.coefsHigh[4];  cf[ 1] = C.coefsHigh[5];  cf[ 2] = C.coefsHigh[6];
    // } else if ( j == 5) {
    //     cf[ 0] = C.coefsHigh[5];  cf[ 1] = C.coefsHigh[6];  cf[ 2] = C.coefsHigh[7];
    // } else if ( j == 6) {
    //     cf[ 0] = C.coefsHigh[6];  cf[ 1] = C.coefsHigh[7];  cf[ 2] = C.coefsHigh[8];
    // } 

    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else { //if ( logIn >= log10(C.maxPoint.x) ) { 

    logy = logx * C.slopeHigh + ( log10(C.maxPoint.y) - C.slopeHigh * log10(C.maxPoint.x) );

  }

  return pow10(logy);
  
}


float segmented_spline_c5_rev
  ( 
    varying float y,
    varying SegmentedSplineParams_c5 C = RRT_PARAMS
  )
{  
  const int N_KNOTS_LOW = 4;
  const int N_KNOTS_HIGH = 4;

  const float KNOT_INC_LOW = (log10(C.midPoint.x) - log10(C.minPoint.x)) / (N_KNOTS_LOW - 1.);
  const float KNOT_INC_HIGH = (log10(C.maxPoint.x) - log10(C.midPoint.x)) / (N_KNOTS_HIGH - 1.);
  
  // KNOT_Y is luminance of the spline at each knot
  float KNOT_Y_LOW[ N_KNOTS_LOW];
  for (int i = 0; i < N_KNOTS_LOW; i = i+1) {
    KNOT_Y_LOW[ i] = ( C.coefsLow[i] + C.coefsLow[i+1]) / 2.;
  };

  float KNOT_Y_HIGH[ N_KNOTS_HIGH];
  for (int i = 0; i < N_KNOTS_HIGH; i = i+1) {
    KNOT_Y_HIGH[ i] = ( C.coefsHigh[i] + C.coefsHigh[i+1]) / 2.;
  };

  float logy = log10( y);

  float logx;
  if (logy <= log10(C.minPoint.y)) {

    logx = log10(C.minPoint.x);

  } else if ( (logy > log10(C.minPoint.y)) && (logy <= log10(C.midPoint.y)) ) {

    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y_LOW[ 0] && logy <= KNOT_Y_LOW[ 1]) {
        cf[ 0] = C.coefsLow[0];  cf[ 1] = C.coefsLow[1];  cf[ 2] = C.coefsLow[2];  j = 0;
    } else if ( logy > KNOT_Y_LOW[ 1] && logy <= KNOT_Y_LOW[ 2]) {
        cf[ 0] = C.coefsLow[1];  cf[ 1] = C.coefsLow[2];  cf[ 2] = C.coefsLow[3];  j = 1;
    } else if ( logy > KNOT_Y_LOW[ 2] && logy <= KNOT_Y_LOW[ 3]) {
        cf[ 0] = C.coefsLow[2];  cf[ 1] = C.coefsLow[3];  cf[ 2] = C.coefsLow[4];  j = 2;
    } 
    
    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = log10(C.minPoint.x) + ( t + j) * KNOT_INC_LOW;

  } else if ( (logy > log10(C.midPoint.y)) && (logy < log10(C.maxPoint.y)) ) {

    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y_HIGH[ 0] && logy <= KNOT_Y_HIGH[ 1]) {
        cf[ 0] = C.coefsHigh[0];  cf[ 1] = C.coefsHigh[1];  cf[ 2] = C.coefsHigh[2];  j = 0;
    } else if ( logy > KNOT_Y_HIGH[ 1] && logy <= KNOT_Y_HIGH[ 2]) {
        cf[ 0] = C.coefsHigh[1];  cf[ 1] = C.coefsHigh[2];  cf[ 2] = C.coefsHigh[3];  j = 1;
    } else if ( logy > KNOT_Y_HIGH[ 2] && logy <= KNOT_Y_HIGH[ 3]) {
        cf[ 0] = C.coefsHigh[2];  cf[ 1] = C.coefsHigh[3];  cf[ 2] = C.coefsHigh[4];  j = 2;
    } 
    
    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = log10(C.midPoint.x) + ( t + j) * KNOT_INC_HIGH;

  } else { //if ( logy >= log10(C.maxPoint.y) ) {

    logx = log10(C.maxPoint.x);

  }
  
  return pow10( logx);

}






const SegmentedSplineParams_c9 ODT_48nits =
{
  // coefsLow[10]
  { -1.6989700043, -1.6989700043, -1.4779000000, -1.2291000000, -0.8648000000, -0.4480000000, 0.0051800000, 0.4511080334, 0.9113744414, 0.9113744414},
  // coefsHigh[10]
  { 0.5154386965, 0.8470437783, 1.1358000000, 1.3802000000, 1.5197000000, 1.5985000000, 1.6467000000, 1.6746091357, 1.6878733390, 1.6878733390 },
  {segmented_spline_c5_fwd( 0.18*pow(2.,-6.5) ),  0.02},    // minPoint
  {segmented_spline_c5_fwd( 0.18 ),                4.8},    // midPoint  
  {segmented_spline_c5_fwd( 0.18*pow(2.,6.5) ),   48.0},    // maxPoint
  0.0,  // slopeLow
  0.04  // slopeHigh
};

const SegmentedSplineParams_c9 ODT_1000nits =
{
  // coefsLow[10]
  { -2.3010299957, -2.3010299957, -1.9312000000, -1.5205000000, -1.0578000000, -0.4668000000, 0.1193800000, 0.7088134201, 1.2911865799, 1.2911865799 },
  // coefsHigh[10]
  { 0.8089132070, 1.1910867930, 1.5683000000, 1.9483000000, 2.3083000000, 2.6384000000, 2.8595000000, 2.9872608805, 3.0127391195, 3.0127391195 },
  {segmented_spline_c5_fwd( 0.18*pow(2.,-12.) ),  0.005},    // minPoint
  {segmented_spline_c5_fwd( 0.18 ),                10.0},    // midPoint  
  {segmented_spline_c5_fwd( 0.18*pow(2.,10.) ),  1000.0},    // maxPoint
  0.0,  // slopeLow
  0.06  // slopeHigh
};

const SegmentedSplineParams_c9 ODT_2000nits =
{
  // coefsLow[10]
  { -2.3010299957, -2.3010299957, -1.9312000000, -1.5205000000, -1.0578000000, -0.4668000000, 0.1193800000, 0.7088134201, 1.2911865799, 1.2911865799 },
  // coefsHigh[10]
  { 0.8019952042, 1.1980047958, 1.5943000000, 1.9973000000, 2.3783000000, 2.7684000000, 3.0515000000, 3.2746293562, 3.3274306351, 3.3274306351 },
  {segmented_spline_c5_fwd( 0.18*pow(2.,-12.) ),  0.005},    // minPoint
  {segmented_spline_c5_fwd( 0.18 ),                10.0},    // midPoint  
  {segmented_spline_c5_fwd( 0.18*pow(2.,11.) ),  2000.0},    // maxPoint
  0.0,  // slopeLow
  0.12  // slopeHigh
};

const SegmentedSplineParams_c9 ODT_4000nits =
{
  // coefsLow[10]
  { -2.3010299957, -2.3010299957, -1.9312000000, -1.5205000000, -1.0578000000, -0.4668000000, 0.1193800000, 0.7088134201, 1.2911865799, 1.2911865799 },
  // coefsHigh[10]
  { 0.7973186613, 1.2026813387, 1.6093000000, 2.0108000000, 2.4148000000, 2.8179000000, 3.1725000000, 3.5344995451, 3.6696204376, 3.6696204376 },
  {segmented_spline_c5_fwd( 0.18*pow(2.,-12.) ),  0.005},    // minPoint
  {segmented_spline_c5_fwd( 0.18 ),                10.0},    // midPoint  
  {segmented_spline_c5_fwd( 0.18*pow(2.,12.) ),  4000.0},    // maxPoint
  0.0,  // slopeLow
  0.3   // slopeHigh
};















float segmented_spline_c9_fwd
  ( 
    varying float x,
    varying SegmentedSplineParams_c9 C = ODT_48nits
  )
{    
  const int N_KNOTS_LOW = 8;
  const int N_KNOTS_HIGH = 8;

  // Check for negatives or zero before taking the log. If negative or zero,
  // set to OCESMIN.
  float xCheck = x;
  if (xCheck <= 0.0) xCheck = 1e-4; 

  float logx = log10( xCheck);

  float logy;

  if ( logx <= log10(C.minPoint.x) ) { 

    logy = logx * C.slopeLow + ( log10(C.minPoint.y) - C.slopeLow * log10(C.minPoint.x) );

  } else if (( logx > log10(C.minPoint.x) ) && ( logx < log10(C.midPoint.x) )) {

    float knot_coord = (N_KNOTS_LOW-1) * (logx-log10(C.minPoint.x))/(log10(C.midPoint.x)-log10(C.minPoint.x));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { C.coefsLow[ j], C.coefsLow[ j + 1], C.coefsLow[ j + 2]};
    // NOTE: If the running a version of CTL < 1.5, you may get an 
    // exception thrown error, usually accompanied by "Array index out of range" 
    // If you receive this error, it is recommended that you update to CTL v1.5, 
    // which contains a number of important bug fixes. Otherwise, you may try 
    // uncommenting the below, which is longer, but equivalent to, the above 
    // line of code.
    //
    // float cf[ 3];
    // if ( j <= 0) {
    //     cf[ 0] = C.coefsLow[0];  cf[ 1] = C.coefsLow[1];  cf[ 2] = C.coefsLow[2];
    // } else if ( j == 1) {
    //     cf[ 0] = C.coefsLow[1];  cf[ 1] = C.coefsLow[2];  cf[ 2] = C.coefsLow[3];
    // } else if ( j == 2) {
    //     cf[ 0] = C.coefsLow[2];  cf[ 1] = C.coefsLow[3];  cf[ 2] = C.coefsLow[4];
    // } else if ( j == 3) {
    //     cf[ 0] = C.coefsLow[3];  cf[ 1] = C.coefsLow[4];  cf[ 2] = C.coefsLow[5];
    // } else if ( j == 4) {
    //     cf[ 0] = C.coefsLow[4];  cf[ 1] = C.coefsLow[5];  cf[ 2] = C.coefsLow[6];
    // } else if ( j == 5) {
    //     cf[ 0] = C.coefsLow[5];  cf[ 1] = C.coefsLow[6];  cf[ 2] = C.coefsLow[7];
    // } else if ( j == 6) {
    //     cf[ 0] = C.coefsLow[6];  cf[ 1] = C.coefsLow[7];  cf[ 2] = C.coefsLow[8];
    // } 
    
    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else if (( logx >= log10(C.midPoint.x) ) && ( logx < log10(C.maxPoint.x) )) {

    float knot_coord = (N_KNOTS_HIGH-1) * (logx-log10(C.midPoint.x))/(log10(C.maxPoint.x)-log10(C.midPoint.x));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { C.coefsHigh[ j], C.coefsHigh[ j + 1], C.coefsHigh[ j + 2]}; 
    // NOTE: If the running a version of CTL < 1.5, you may get an 
    // exception thrown error, usually accompanied by "Array index out of range" 
    // If you receive this error, it is recommended that you update to CTL v1.5, 
    // which contains a number of important bug fixes. Otherwise, you may try 
    // uncommenting the below, which is longer, but equivalent to, the above 
    // line of code.
    //
    // float cf[ 3];
    // if ( j <= 0) {
    //     cf[ 0] = C.coefsHigh[0];  cf[ 1] = C.coefsHigh[1];  cf[ 2] = C.coefsHigh[2];
    // } else if ( j == 1) {
    //     cf[ 0] = C.coefsHigh[1];  cf[ 1] = C.coefsHigh[2];  cf[ 2] = C.coefsHigh[3];
    // } else if ( j == 2) {
    //     cf[ 0] = C.coefsHigh[2];  cf[ 1] = C.coefsHigh[3];  cf[ 2] = C.coefsHigh[4];
    // } else if ( j == 3) {
    //     cf[ 0] = C.coefsHigh[3];  cf[ 1] = C.coefsHigh[4];  cf[ 2] = C.coefsHigh[5];
    // } else if ( j == 4) {
    //     cf[ 0] = C.coefsHigh[4];  cf[ 1] = C.coefsHigh[5];  cf[ 2] = C.coefsHigh[6];
    // } else if ( j == 5) {
    //     cf[ 0] = C.coefsHigh[5];  cf[ 1] = C.coefsHigh[6];  cf[ 2] = C.coefsHigh[7];
    // } else if ( j == 6) {
    //     cf[ 0] = C.coefsHigh[6];  cf[ 1] = C.coefsHigh[7];  cf[ 2] = C.coefsHigh[8];
    // } 

    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else { //if ( logIn >= log10(C.maxPoint.x) ) { 

    logy = logx * C.slopeHigh + ( log10(C.maxPoint.y) - C.slopeHigh * log10(C.maxPoint.x) );

  }

  return pow10(logy);
  
}


float segmented_spline_c9_rev
  ( 
    varying float y,
    varying SegmentedSplineParams_c9 C = ODT_48nits
  )
{  
  const int N_KNOTS_LOW = 8;
  const int N_KNOTS_HIGH = 8;

  const float KNOT_INC_LOW = (log10(C.midPoint.x) - log10(C.minPoint.x)) / (N_KNOTS_LOW - 1.);
  const float KNOT_INC_HIGH = (log10(C.maxPoint.x) - log10(C.midPoint.x)) / (N_KNOTS_HIGH - 1.);
  
  // KNOT_Y is luminance of the spline at each knot
  float KNOT_Y_LOW[ N_KNOTS_LOW];
  for (int i = 0; i < N_KNOTS_LOW; i = i+1) {
    KNOT_Y_LOW[ i] = ( C.coefsLow[i] + C.coefsLow[i+1]) / 2.;
  };

  float KNOT_Y_HIGH[ N_KNOTS_HIGH];
  for (int i = 0; i < N_KNOTS_HIGH; i = i+1) {
    KNOT_Y_HIGH[ i] = ( C.coefsHigh[i] + C.coefsHigh[i+1]) / 2.;
  };

  float logy = log10( y);

  float logx;
  if (logy <= log10(C.minPoint.y)) {

    logx = log10(C.minPoint.x);

  } else if ( (logy > log10(C.minPoint.y)) && (logy <= log10(C.midPoint.y)) ) {

    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y_LOW[ 0] && logy <= KNOT_Y_LOW[ 1]) {
        cf[ 0] = C.coefsLow[0];  cf[ 1] = C.coefsLow[1];  cf[ 2] = C.coefsLow[2];  j = 0;
    } else if ( logy > KNOT_Y_LOW[ 1] && logy <= KNOT_Y_LOW[ 2]) {
        cf[ 0] = C.coefsLow[1];  cf[ 1] = C.coefsLow[2];  cf[ 2] = C.coefsLow[3];  j = 1;
    } else if ( logy > KNOT_Y_LOW[ 2] && logy <= KNOT_Y_LOW[ 3]) {
        cf[ 0] = C.coefsLow[2];  cf[ 1] = C.coefsLow[3];  cf[ 2] = C.coefsLow[4];  j = 2;
    } else if ( logy > KNOT_Y_LOW[ 3] && logy <= KNOT_Y_LOW[ 4]) {
        cf[ 0] = C.coefsLow[3];  cf[ 1] = C.coefsLow[4];  cf[ 2] = C.coefsLow[5];  j = 3;
    } else if ( logy > KNOT_Y_LOW[ 4] && logy <= KNOT_Y_LOW[ 5]) {
        cf[ 0] = C.coefsLow[4];  cf[ 1] = C.coefsLow[5];  cf[ 2] = C.coefsLow[6];  j = 4;
    } else if ( logy > KNOT_Y_LOW[ 5] && logy <= KNOT_Y_LOW[ 6]) {
        cf[ 0] = C.coefsLow[5];  cf[ 1] = C.coefsLow[6];  cf[ 2] = C.coefsLow[7];  j = 5;
    } else if ( logy > KNOT_Y_LOW[ 6] && logy <= KNOT_Y_LOW[ 7]) {
        cf[ 0] = C.coefsLow[6];  cf[ 1] = C.coefsLow[7];  cf[ 2] = C.coefsLow[8];  j = 6;
    }
    
    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = log10(C.minPoint.x) + ( t + j) * KNOT_INC_LOW;

  } else if ( (logy > log10(C.midPoint.y)) && (logy < log10(C.maxPoint.y)) ) {

    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y_HIGH[ 0] && logy <= KNOT_Y_HIGH[ 1]) {
        cf[ 0] = C.coefsHigh[0];  cf[ 1] = C.coefsHigh[1];  cf[ 2] = C.coefsHigh[2];  j = 0;
    } else if ( logy > KNOT_Y_HIGH[ 1] && logy <= KNOT_Y_HIGH[ 2]) {
        cf[ 0] = C.coefsHigh[1];  cf[ 1] = C.coefsHigh[2];  cf[ 2] = C.coefsHigh[3];  j = 1;
    } else if ( logy > KNOT_Y_HIGH[ 2] && logy <= KNOT_Y_HIGH[ 3]) {
        cf[ 0] = C.coefsHigh[2];  cf[ 1] = C.coefsHigh[3];  cf[ 2] = C.coefsHigh[4];  j = 2;
    } else if ( logy > KNOT_Y_HIGH[ 3] && logy <= KNOT_Y_HIGH[ 4]) {
        cf[ 0] = C.coefsHigh[3];  cf[ 1] = C.coefsHigh[4];  cf[ 2] = C.coefsHigh[5];  j = 3;
    } else if ( logy > KNOT_Y_HIGH[ 4] && logy <= KNOT_Y_HIGH[ 5]) {
        cf[ 0] = C.coefsHigh[4];  cf[ 1] = C.coefsHigh[5];  cf[ 2] = C.coefsHigh[6];  j = 4;
    } else if ( logy > KNOT_Y_HIGH[ 5] && logy <= KNOT_Y_HIGH[ 6]) {
        cf[ 0] = C.coefsHigh[5];  cf[ 1] = C.coefsHigh[6];  cf[ 2] = C.coefsHigh[7];  j = 5;
    } else if ( logy > KNOT_Y_HIGH[ 6] && logy <= KNOT_Y_HIGH[ 7]) {
        cf[ 0] = C.coefsHigh[6];  cf[ 1] = C.coefsHigh[7];  cf[ 2] = C.coefsHigh[8];  j = 6;
    }
    
    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = log10(C.midPoint.x) + ( t + j) * KNOT_INC_HIGH;

  } else { //if ( logy >= log10(C.maxPoint.y) ) {

    logx = log10(C.maxPoint.x);

  }
  
  return pow10( logx);
}










