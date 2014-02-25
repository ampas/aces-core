//
// rrt-transform-common.ctl
// v0.7.1
//
// Contains functions and constants shared by forward and inverse RRT transforms 
// This is primarily to avoid code redundancy. 
// This structure also has the benefit of facilitating any updates to these
// parameters or functions, if necessary.
//



import "utilities-color";



// ------- RRT constants

  // Neutral tone scale constants 
  const float ACESMAX = HALF_MAX; // 65504.0
  const float ACESMID = 0.18;     
  const float ACESMIN = ACESMID * ACESMID / ACESMAX;  
  const float OCESMAX = 10000.0;  // units of cd/m^2
  const float OCESMID = 5.0;      // units of cd/m^2
  const float OCESMIN = 0.0001;   // units of cd/m^2

  const int RRT_KNOT_LEN = 21;
  const float RRT_KNOT_START = log10(ACESMIN);
  const float RRT_KNOT_END = log10(ACESMAX);
  const float RRT_KNOT_SPAN = RRT_KNOT_END - RRT_KNOT_START;
  const float RRT_KNOT_INC = RRT_KNOT_SPAN / (RRT_KNOT_LEN - 1.);

  // These are the coefficients used for v0.7
  const float RRT_COEFS[23] = {
    -4.00000,
    -4.00000,
    -4.00000,
    -3.95000,
    -3.82000,
    -3.54650,
    -3.10000,
    -2.46700,
    -1.66000,
    -0.73000,
    0.24714,
    1.15080,
    1.96320,
    2.63950,
    3.15750,
    3.51050,
    3.71550,
    3.85130,
    3.92710,
    3.96980,
    4.00000,
    4.00000,
    4.00000
  };

  // "Glow" module constants
  const float RRT_GLOW_GAIN = 0.075;
  const float RRT_GLOW_MID = 0.1;

  // Red modifier constants
  const float RRT_RED_SCALE = 0.85;
  const float RRT_RED_PIVOT = 0.03;
  const float RRT_RED_HUE = 0.;
  const float RRT_RED_WIDTH = 120.;


// ------- Glow module functions
float glow_fwd( float ycIn, float glowGainIn, float glowMid)
{
   float glowGainOut;

   if (ycIn <= 2./3. * glowMid) {
     glowGainOut = glowGainIn;
   } else if ( ycIn >= 2. * glowMid) {
     glowGainOut = 0.;
   } else {
     glowGainOut = glowGainIn * (glowMid / ycIn - 1./2.);
   }

   return glowGainOut;
}

float glow_inv( float ycOut, float glowGainIn, float glowMid)
{
     float glowGainOut;

     if (ycOut <= ((1 + glowGainIn) * 2./3. * glowMid)) {
       glowGainOut = -glowGainIn / (1 + glowGainIn);
     } else if ( ycOut >= (2. * glowMid)) {
       glowGainOut = 0.;
     } else {
       glowGainOut = glowGainIn * (glowMid / ycOut - 1./2.) / (glowGainIn / 2. - 1.);
     }

     return glowGainOut;
}

float sigmoid_shaper( float x)
{
	// Sigmoid function in the range 0 to 1 spanning -2 to +2.

	float t = max( 1. - fabs( x / 2.), 0.);
	float y = 1. + sign(x) * (1. - t * t);
	
	return y / 2.;
}


// ------- Red modifier functions
float cubic_basis_shaper
( 
  varying float x, 
  varying float w   // full base width of the shaper function (in degrees)
)
{
  float M[4][4] = { { -1./6,  3./6, -3./6,  1./6 },
                    {  3./6, -6./6,  3./6,  0./6 },
                    { -3./6,  0./6,  3./6,  0./6 },
                    {  1./6,  4./6,  1./6,  0./6 } };
  
  float knots[5] = { -w/2.,
                     -w/4.,
                     0.,
                     w/4.,
                     w/2. };
  
  float y;
  if ((x > knots[0]) && (x < knots[4])) {  
    float knot_coord = (x - knots[0]) * 4./w;  
    int j = knot_coord;
    float t = knot_coord - j;
      
    float monomials[4] = { t*t*t, t*t, t, 1. };

    // (if/else structure required for compatibility with CTL < v1.5.)
    if ( j == 3) {
      y = monomials[0] * M[0][0] + monomials[1] * M[1][0] + 
          monomials[2] * M[2][0] + monomials[3] * M[3][0];
    } else if ( j == 2) {
      y = monomials[0] * M[0][1] + monomials[1] * M[1][1] + 
          monomials[2] * M[2][1] + monomials[3] * M[3][1];
    } else if ( j == 1) {
      y = monomials[0] * M[0][2] + monomials[1] * M[1][2] + 
          monomials[2] * M[2][2] + monomials[3] * M[3][2];
    } else if ( j == 0) {
      y = monomials[0] * M[0][3] + monomials[1] * M[1][3] + 
          monomials[2] * M[2][3] + monomials[3] * M[3][3];
    } else {
      y = 0.0;
    }
  }
  
  return y * 3/2.;
}

float center_hue( float hue, float centerH)
{
  float hueCentered = hue - centerH;
  if (hueCentered < -180.) hueCentered = hueCentered + 360.;
  else if (hueCentered > 180.) hueCentered = hueCentered - 360.;
  return hueCentered;
}

float uncenter_hue( float hueCentered, float centerH)
{
  float hue = hueCentered + centerH;
  if (hue < 0.) hue = hue + 360.;
  else if (hue > 360.) hue = hue - 360.;
  return hue;
}



// ------ Tone scale spline functions 
float rrt_tonescale_fwd
  ( 
    varying float aces,                          // ACES value
    varying float COEFS[23] = RRT_COEFS          // the control points
  )
{    
  // Check for negatives or zero before taking the log. If negative or zero,
  // set to ACESMIN.
  float acesCheck = aces;
  if (acesCheck <= ACESMIN) acesCheck = ACESMIN; 

  float logAces = log10( acesCheck);

  float logOces;

  // For logOces values in the knot range, apply the B-spline shaper, b(x)
  if (( logAces >= RRT_KNOT_START ) && ( logAces < RRT_KNOT_END)) 
  {
    float knot_coord = (logAces - RRT_KNOT_START) / RRT_KNOT_SPAN * ( RRT_KNOT_LEN-1);
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { COEFS[ j], COEFS[ j + 1], COEFS[ j + 2]};
    // NOTE: If the running a version of CTL < 1.5, you may get an 
    // exception thrown error, usually accompanied by "Array index out of range" 
    // If you receive this error, it is recommended that you update to CTL v1.5, 
    // which contains a number of important bug fixes. Otherwise, you may try 
    // uncommenting the below, which is longer, but equivalent to, the above 
    // line of code.
    //
    //     float cf[ 3];
    //     if ( j <= 0) {
    //         cf[ 0] = COEFS[0];  cf[ 1] = COEFS[1];  cf[ 2] = COEFS[2];
    //     } else if ( j == 1) {
    //         cf[ 0] = COEFS[1];  cf[ 1] = COEFS[2];  cf[ 2] = COEFS[3];
    //     } else if ( j == 2) {
    //         cf[ 0] = COEFS[2];  cf[ 1] = COEFS[3];  cf[ 2] = COEFS[4];
    //     } else if ( j == 3) {
    //         cf[ 0] = COEFS[3];  cf[ 1] = COEFS[4];  cf[ 2] = COEFS[5];
    //     } else if ( j == 4) {
    //         cf[ 0] = COEFS[4];  cf[ 1] = COEFS[5];  cf[ 2] = COEFS[6];
    //     } else if ( j == 5) {
    //         cf[ 0] = COEFS[5];  cf[ 1] = COEFS[6];  cf[ 2] = COEFS[7];
    //     } else if ( j == 6) {
    //         cf[ 0] = COEFS[6];  cf[ 1] = COEFS[7];  cf[ 2] = COEFS[8];
    //     } else if ( j == 7) {
    //         cf[ 0] = COEFS[7];  cf[ 1] = COEFS[8];  cf[ 2] = COEFS[9];
    //     } else if ( j == 8) {
    //         cf[ 0] = COEFS[8];  cf[ 1] = COEFS[9];  cf[ 2] = COEFS[10];
    //     } else if ( j == 9) {
    //         cf[ 0] = COEFS[9];  cf[ 1] = COEFS[10];  cf[ 2] = COEFS[11];
    //     } else if ( j == 10) {
    //         cf[ 0] = COEFS[10];  cf[ 1] = COEFS[11];  cf[ 2] = COEFS[12];
    //     } else if ( j == 11) {
    //         cf[ 0] = COEFS[11];  cf[ 1] = COEFS[12];  cf[ 2] = COEFS[13];
    //     } else if ( j == 12) {
    //         cf[ 0] = COEFS[12];  cf[ 1] = COEFS[13];  cf[ 2] = COEFS[14];
    //     } else if ( j == 13) {
    //         cf[ 0] = COEFS[13];  cf[ 1] = COEFS[14];  cf[ 2] = COEFS[15];
    //     } else if ( j == 14) {
    //         cf[ 0] = COEFS[14];  cf[ 1] = COEFS[15];  cf[ 2] = COEFS[16];
    //     } else if ( j == 15) {
    //         cf[ 0] = COEFS[15];  cf[ 1] = COEFS[16];  cf[ 2] = COEFS[17];
    //     } else if ( j == 16) {
    //         cf[ 0] = COEFS[16];  cf[ 1] = COEFS[17];  cf[ 2] = COEFS[18];
    //     } else if ( j == 17) {
    //         cf[ 0] = COEFS[17];  cf[ 1] = COEFS[18];  cf[ 2] = COEFS[19];
    //     } else if ( j == 18) {
    //         cf[ 0] = COEFS[18];  cf[ 1] = COEFS[19];  cf[ 2] = COEFS[20];
    //     } else if ( j == 19) {
    //         cf[ 0] = COEFS[19];  cf[ 1] = COEFS[20];  cf[ 2] = COEFS[21];
    //     } else {
    //         cf[ 0] = COEFS[20];  cf[ 1] = COEFS[21];  cf[ 2] = COEFS[22];
    //     }

    float monomials[ 3] = { t * t, t, 1. };
    logOces = dot_f3_f3( monomials, mult_f3_f33( cf, M));
  }    
  else if ( logAces < RRT_KNOT_START) { 
    logOces = ( COEFS[0] + COEFS[1]) / 2.;
  }
  else if ( logAces >= RRT_KNOT_END) { 
    logOces = ( COEFS[RRT_KNOT_LEN-1] + COEFS[RRT_KNOT_LEN]) / 2.;
  }

  return pow10(logOces);
}



float rrt_tonescale_rev
( 
  varying float oces,                          // OCES value
  varying float COEFS[23] = RRT_COEFS          // the control points
)
{
  // KNOT_Y is luminance of the spline at the knots.
  float KNOT_Y[ RRT_KNOT_LEN];
  for (int i = 0; i < RRT_KNOT_LEN; i = i+1) {
    KNOT_Y[ i] = ( COEFS[i] + COEFS[i+1]) / 2.;
  };

  float logOces = oces;
  if (logOces <= OCESMIN) logOces = OCESMIN; 

  logOces = log10( logOces );

  float logAces;
  if (logOces <= KNOT_Y[0]) {
    logAces = RRT_KNOT_START;
  } else if (logOces > KNOT_Y[RRT_KNOT_LEN-1]) {
    logAces = RRT_KNOT_END;
  } else {  // (( logOces > KNOT_Y[0] ) && ( logOces <= KNOT_Y[KNOT_LEN-1]))
    unsigned int j;
    float cf[ 3];
    if ( logOces > KNOT_Y[ 0] && logOces <= KNOT_Y[ 1]) {
        cf[ 0] = COEFS[0];  cf[ 1] = COEFS[1];  cf[ 2] = COEFS[2];  j = 0;
    } else if ( logOces > KNOT_Y[ 1] && logOces <= KNOT_Y[ 2]) {
        cf[ 0] = COEFS[1];  cf[ 1] = COEFS[2];  cf[ 2] = COEFS[3];  j = 1;
    } else if ( logOces > KNOT_Y[ 2] && logOces <= KNOT_Y[ 3]) {
        cf[ 0] = COEFS[2];  cf[ 1] = COEFS[3];  cf[ 2] = COEFS[4];  j = 2;
    } else if ( logOces > KNOT_Y[ 3] && logOces <= KNOT_Y[ 4]) {
        cf[ 0] = COEFS[3];  cf[ 1] = COEFS[4];  cf[ 2] = COEFS[5];  j = 3;
    } else if ( logOces > KNOT_Y[ 4] && logOces <= KNOT_Y[ 5]) {
        cf[ 0] = COEFS[4];  cf[ 1] = COEFS[5];  cf[ 2] = COEFS[6];  j = 4;
    } else if ( logOces > KNOT_Y[ 5] && logOces <= KNOT_Y[ 6]) {
        cf[ 0] = COEFS[5];  cf[ 1] = COEFS[6];  cf[ 2] = COEFS[7];  j = 5;
    } else if ( logOces > KNOT_Y[ 6] && logOces <= KNOT_Y[ 7]) {
        cf[ 0] = COEFS[6];  cf[ 1] = COEFS[7];  cf[ 2] = COEFS[8];  j = 6;
    } else if ( logOces > KNOT_Y[ 7] && logOces <= KNOT_Y[ 8]) {
        cf[ 0] = COEFS[7];  cf[ 1] = COEFS[8];  cf[ 2] = COEFS[9];  j = 7;
    } else if ( logOces > KNOT_Y[ 8] && logOces <= KNOT_Y[ 9]) {
        cf[ 0] = COEFS[8];  cf[ 1] = COEFS[9];  cf[ 2] = COEFS[10];  j = 8;
    } else if ( logOces > KNOT_Y[ 9] && logOces <= KNOT_Y[10]) {
        cf[ 0] = COEFS[9];  cf[ 1] = COEFS[10];  cf[ 2] = COEFS[11];  j = 9;
    } else if ( logOces > KNOT_Y[10] && logOces <= KNOT_Y[11]) {
        cf[ 0] = COEFS[10];  cf[ 1] = COEFS[11];  cf[ 2] = COEFS[12];  j = 10;
    } else if ( logOces > KNOT_Y[11] && logOces <= KNOT_Y[12]) {
        cf[ 0] = COEFS[11];  cf[ 1] = COEFS[12];  cf[ 2] = COEFS[13];  j = 11;
    } else if ( logOces > KNOT_Y[12] && logOces <= KNOT_Y[13]) {
        cf[ 0] = COEFS[12];  cf[ 1] = COEFS[13];  cf[ 2] = COEFS[14];  j = 12;
    } else if ( logOces > KNOT_Y[13] && logOces <= KNOT_Y[14]) {
        cf[ 0] = COEFS[13];  cf[ 1] = COEFS[14];  cf[ 2] = COEFS[15];  j = 13;
    } else if ( logOces > KNOT_Y[14] && logOces <= KNOT_Y[15]) {
        cf[ 0] = COEFS[14];  cf[ 1] = COEFS[15];  cf[ 2] = COEFS[16];  j = 14;
    } else if ( logOces > KNOT_Y[15] && logOces <= KNOT_Y[16]) {
        cf[ 0] = COEFS[15];  cf[ 1] = COEFS[16];  cf[ 2] = COEFS[17];  j = 15;
    } else if ( logOces > KNOT_Y[16] && logOces <= KNOT_Y[17]) {
        cf[ 0] = COEFS[16];  cf[ 1] = COEFS[17];  cf[ 2] = COEFS[18];  j = 16;
    } else if ( logOces > KNOT_Y[17] && logOces <= KNOT_Y[18]) {
        cf[ 0] = COEFS[17];  cf[ 1] = COEFS[18];  cf[ 2] = COEFS[19];  j = 17;
    } else if ( logOces > KNOT_Y[18] && logOces <= KNOT_Y[19]) {
        cf[ 0] = COEFS[18];  cf[ 1] = COEFS[19];  cf[ 2] = COEFS[20];  j = 18;
    } else {
        cf[ 0] = COEFS[19];  cf[ 1] = COEFS[20];  cf[ 2] = COEFS[21];  j = 19;
    }

    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logOces;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logAces = RRT_KNOT_START + ( t + j) * RRT_KNOT_INC;
  } 
  
  return pow10( logAces);
}
