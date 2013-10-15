//
// utilities-aces.ctl
// v0.2.2
//
// Functions used by ACES system transforms that are unlikely to be reused in 
// algorithms outside of ACES.
//



import "utilities-color";



/* ----- Tone scale spline functions ----- */

// Textbook monomial to basis-function conversion matrix.
const float M[ 3][ 3] = {
  {  0.5, -1.0, 0.5 },
  { -1.0,  1.0, 0.5 },
  {  0.5,  0.0, 0.0 }
};

const float ACESMAX = 65504.0;
const float ACESMID = 0.18;
const float ACESMIN = ACESMID * ACESMID / ACESMAX;
// This is the same as: pow10(log10(ACESMID) - (log10(ACESMAX)-log10(ACESMID)));
const float OCESMAX = 10000.0;
const float OCESMID = 5.0;
const float OCESMIN = 0.0001;

const int RRT_KNOT_LEN = 21;
const float RRT_KNOT_START = log10(ACESMIN);
const float RRT_KNOT_END = log10(ACESMAX);
const float RRT_KNOT_SPAN = RRT_KNOT_END - RRT_KNOT_START;
const float RRT_KNOT_INC = RRT_KNOT_SPAN / (RRT_KNOT_LEN - 1.);

// These are the coefficients used for WGR5. Before RCv0.2.1, the number of coefs 
// may be reduced or the B-spline might be replaced with a function.
const float DEFAULT_RRT_COEFS[23] = {
  -4.00000,
  -4.00000,
  -4.00000,
  -3.95000,
  -3.82000,
  -3.55950,
  -3.12710,
  -2.52690,
  -1.72810,
  -0.76221,
  0.24019,
  1.15775,
  1.94320,
  2.61950,
  3.13250,
  3.49050,
  3.71550,
  3.85130,
  3.92710,
  3.96980,
  4.00000,
  4.00000,
  4.00000
}; 

float rrt_tonescale_fwd
  ( 
    varying float aces,                          // ACES value
    varying float COEFS[23] = DEFAULT_RRT_COEFS  // the control points
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

      // The following could be written as:
      //float cf[ 3] = { COEFS[ j], COEFS[ j + 1], COEFS[ j + 2]};
      // but for now is expanded for compatibility with CTL < v1.5.
      float cf[ 3];
      if ( j <= 0) {
          cf[ 0] = COEFS[0];  cf[ 1] = COEFS[1];  cf[ 2] = COEFS[2];
      } else if ( j == 1) {
          cf[ 0] = COEFS[1];  cf[ 1] = COEFS[2];  cf[ 2] = COEFS[3];
      } else if ( j == 2) {
          cf[ 0] = COEFS[2];  cf[ 1] = COEFS[3];  cf[ 2] = COEFS[4];
      } else if ( j == 3) {
          cf[ 0] = COEFS[3];  cf[ 1] = COEFS[4];  cf[ 2] = COEFS[5];
      } else if ( j == 4) {
          cf[ 0] = COEFS[4];  cf[ 1] = COEFS[5];  cf[ 2] = COEFS[6];
      } else if ( j == 5) {
          cf[ 0] = COEFS[5];  cf[ 1] = COEFS[6];  cf[ 2] = COEFS[7];
      } else if ( j == 6) {
          cf[ 0] = COEFS[6];  cf[ 1] = COEFS[7];  cf[ 2] = COEFS[8];
      } else if ( j == 7) {
          cf[ 0] = COEFS[7];  cf[ 1] = COEFS[8];  cf[ 2] = COEFS[9];
      } else if ( j == 8) {
          cf[ 0] = COEFS[8];  cf[ 1] = COEFS[9];  cf[ 2] = COEFS[10];
      } else if ( j == 9) {
          cf[ 0] = COEFS[9];  cf[ 1] = COEFS[10];  cf[ 2] = COEFS[11];
      } else if ( j == 10) {
          cf[ 0] = COEFS[10];  cf[ 1] = COEFS[11];  cf[ 2] = COEFS[12];
      } else if ( j == 11) {
          cf[ 0] = COEFS[11];  cf[ 1] = COEFS[12];  cf[ 2] = COEFS[13];
      } else if ( j == 12) {
          cf[ 0] = COEFS[12];  cf[ 1] = COEFS[13];  cf[ 2] = COEFS[14];
      } else if ( j == 13) {
          cf[ 0] = COEFS[13];  cf[ 1] = COEFS[14];  cf[ 2] = COEFS[15];
      } else if ( j == 14) {
          cf[ 0] = COEFS[14];  cf[ 1] = COEFS[15];  cf[ 2] = COEFS[16];
      } else if ( j == 15) {
          cf[ 0] = COEFS[15];  cf[ 1] = COEFS[16];  cf[ 2] = COEFS[17];
      } else if ( j == 16) {
          cf[ 0] = COEFS[16];  cf[ 1] = COEFS[17];  cf[ 2] = COEFS[18];
      } else if ( j == 17) {
          cf[ 0] = COEFS[17];  cf[ 1] = COEFS[18];  cf[ 2] = COEFS[19];
      } else if ( j == 18) {
          cf[ 0] = COEFS[18];  cf[ 1] = COEFS[19];  cf[ 2] = COEFS[20];
      } else if ( j == 19) {
          cf[ 0] = COEFS[19];  cf[ 1] = COEFS[20];  cf[ 2] = COEFS[21];
      } else {
          cf[ 0] = COEFS[20];  cf[ 1] = COEFS[21];  cf[ 2] = COEFS[22];
      }

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
  varying float COEFS[23] = DEFAULT_RRT_COEFS  // the control points
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
  } else {  
    // if (( logOces >= KNOT_Y[0] ) && ( logOces <= KNOT_Y[KNOT_LEN-1])) {
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




const float DEFAULT_YMIN_ABS = 5.0;
const float DEFAULT_YMAX_ABS = 48.0;
const float DEFAULT_ODT_HI_SLOPE = 0.04;
const float DEFAULT_ODT_COEFS[14] = { 
  0.59886, 
  0.80071, 
  0.98498, 
  1.14260, 
  1.27700, 
  1.38810, 
  1.47380, 
  1.53840, 
  1.58360, 
  1.61780, 
  1.64350, 
  1.66160, 
  1.67765, 
  1.68573 
};

const unsigned int ODT_KNOT_LEN = 13;
const float ODT_KNOT_START = log10( rrt_tonescale_fwd(0.18) );
const float ODT_KNOT_END = log10( rrt_tonescale_fwd(0.18*pow(2.,6.5)) );
const float ODT_KNOT_SPAN = ODT_KNOT_END - ODT_KNOT_START;
const float ODT_KNOT_INC = ODT_KNOT_SPAN / (ODT_KNOT_LEN - 1.);

float odt_tonescale_fwd
  ( // input is absolute luminance, output is absolute luminance
    varying float oces,
    varying float COEFS[14] = DEFAULT_ODT_COEFS,   
    varying float hi_slope = DEFAULT_ODT_HI_SLOPE,
    varying float lumMin = DEFAULT_YMIN_ABS,
    varying float lumMax = DEFAULT_YMAX_ABS
  )
{
  float ocesCheck = oces;
  if (ocesCheck < OCESMIN) ocesCheck = OCESMIN; 
    // Just a safety check to make sure no negative OCES values somehow got by

  float logOces = log10( ocesCheck);

  float logy;
  float y;

  // For logOces values in the knot range, apply the B-spline shaper, b(x)
  if (( logOces > ODT_KNOT_START ) && ( logOces < ODT_KNOT_END)) {

    float knot_coord = (logOces - ODT_KNOT_START) / ODT_KNOT_SPAN * ( ODT_KNOT_LEN-1);
    int j = knot_coord;
    float t = knot_coord - j;

      // The following could be written as:
      //float cf[ 3] = { COEFS[ j], COEFS[ j + 1], COEFS[ j + 2]};
      // but for now is expanded for compatibility with CTL < v1.5.
      float cf[ 3];
      if ( j <= 0) {
          cf[ 0] = COEFS[0];  cf[ 1] = COEFS[1];  cf[ 2] = COEFS[2];
      } else if ( j == 1) {
          cf[ 0] = COEFS[1];  cf[ 1] = COEFS[2];  cf[ 2] = COEFS[3];
      } else if ( j == 2) {
          cf[ 0] = COEFS[2];  cf[ 1] = COEFS[3];  cf[ 2] = COEFS[4];
      } else if ( j == 3) {
          cf[ 0] = COEFS[3];  cf[ 1] = COEFS[4];  cf[ 2] = COEFS[5];
      } else if ( j == 4) {
          cf[ 0] = COEFS[4];  cf[ 1] = COEFS[5];  cf[ 2] = COEFS[6];
      } else if ( j == 5) {
          cf[ 0] = COEFS[5];  cf[ 1] = COEFS[6];  cf[ 2] = COEFS[7];
      } else if ( j == 6) {
          cf[ 0] = COEFS[6];  cf[ 1] = COEFS[7];  cf[ 2] = COEFS[8];
      } else if ( j == 7) {
          cf[ 0] = COEFS[7];  cf[ 1] = COEFS[8];  cf[ 2] = COEFS[9];
      } else if ( j == 8) {
          cf[ 0] = COEFS[8];  cf[ 1] = COEFS[9];  cf[ 2] = COEFS[10];
      } else if ( j == 9) {
          cf[ 0] = COEFS[9];  cf[ 1] = COEFS[10];  cf[ 2] = COEFS[11];
      } else if ( j == 10) {
          cf[ 0] = COEFS[10];  cf[ 1] = COEFS[11];  cf[ 2] = COEFS[12];
      } else {
          cf[ 0] = COEFS[11];  cf[ 1] = COEFS[12];  cf[ 2] = COEFS[13];
      }

    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));
  }    

  else if ( logOces <= ODT_KNOT_START) { 
      logy = logOces;
  }

  else if ( logOces >= ODT_KNOT_END) { 
      logy = logOces * hi_slope + (log10(lumMax) - hi_slope * ODT_KNOT_END);
  }

  return pow10(logy);
}

float odt_tonescale_rev
( // input is absolute luminance, output is absolute luminance
  varying float y,
  varying float COEFS[14] = DEFAULT_ODT_COEFS,   // values of the B-spline control points
  varying float hi_slope = DEFAULT_ODT_HI_SLOPE,
  varying float lumMin = DEFAULT_YMIN_ABS,
  varying float lumMax = DEFAULT_YMAX_ABS
)
{
  // KNOT_Y is luminance of the spline at the knots.
  float KNOT_Y[ ODT_KNOT_LEN];
  for (int i = 0; i < ODT_KNOT_LEN; i = i+1) {
    KNOT_Y[ i] = ( COEFS[i] + COEFS[i+1]) / 2.;
  };

  float logy = log10( y);

  float logx;
  if (logy <= KNOT_Y[0]) {
    logx = logy;
  } else if (logy >= KNOT_Y[ODT_KNOT_LEN-1]) {
    logx = ODT_KNOT_END;
  } else {  
    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y[ 0] && logy <= KNOT_Y[ 1]) {
        cf[ 0] = COEFS[0];  cf[ 1] = COEFS[1];  cf[ 2] = COEFS[2];  j = 0;
    } else if ( logy > KNOT_Y[ 1] && logy <= KNOT_Y[ 2]) {
        cf[ 0] = COEFS[1];  cf[ 1] = COEFS[2];  cf[ 2] = COEFS[3];  j = 1;
    } else if ( logy > KNOT_Y[ 2] && logy <= KNOT_Y[ 3]) {
        cf[ 0] = COEFS[2];  cf[ 1] = COEFS[3];  cf[ 2] = COEFS[4];  j = 2;
    } else if ( logy > KNOT_Y[ 3] && logy <= KNOT_Y[ 4]) {
        cf[ 0] = COEFS[3];  cf[ 1] = COEFS[4];  cf[ 2] = COEFS[5];  j = 3;
    } else if ( logy > KNOT_Y[ 4] && logy <= KNOT_Y[ 5]) {
        cf[ 0] = COEFS[4];  cf[ 1] = COEFS[5];  cf[ 2] = COEFS[6];  j = 4;
    } else if ( logy > KNOT_Y[ 5] && logy <= KNOT_Y[ 6]) {
        cf[ 0] = COEFS[5];  cf[ 1] = COEFS[6];  cf[ 2] = COEFS[7];  j = 5;
    } else if ( logy > KNOT_Y[ 6] && logy <= KNOT_Y[ 7]) {
        cf[ 0] = COEFS[6];  cf[ 1] = COEFS[7];  cf[ 2] = COEFS[8];  j = 6;
    } else if ( logy > KNOT_Y[ 7] && logy <= KNOT_Y[ 8]) {
        cf[ 0] = COEFS[7];  cf[ 1] = COEFS[8];  cf[ 2] = COEFS[9];  j = 7;
    } else if ( logy > KNOT_Y[ 8] && logy <= KNOT_Y[ 9]) {
        cf[ 0] = COEFS[8];  cf[ 1] = COEFS[9];  cf[ 2] = COEFS[10];  j = 8;
    } else if ( logy > KNOT_Y[ 9] && logy <= KNOT_Y[10]) {
        cf[ 0] = COEFS[9];  cf[ 1] = COEFS[10];  cf[ 2] = COEFS[11];  j = 9;
    } else if ( logy > KNOT_Y[10] && logy <= KNOT_Y[11]) {
        cf[ 0] = COEFS[10];  cf[ 1] = COEFS[11];  cf[ 2] = COEFS[12];  j = 10;
    } else {
        cf[ 0] = COEFS[11];  cf[ 1] = COEFS[12];  cf[ 2] = COEFS[13];  j = 11;
    }

    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = ODT_KNOT_START + ( t + j) * ODT_KNOT_INC;
  } 
  
  return pow10( logx);
}


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

    // (If/else structure required for compatibility with CTL < v1.5.)
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
    } else
    y = 0;
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

// Regions of hue and intensity are targeted using a cubic basis shaper 
// function. The controls for the shape of this function are the center/peak 
// (in degrees), and the full width (in degrees) at the base. Values in the 
// center of the function get 1.0 of an adjustment while values at the tails 
// of the function get 0.0 adjustment.
float[3] scale_C_at_H
( 
  float rgb[3], 
  float centerH,   // center of targeted hue region (in degrees)
  float widthH,    // full width at base of targeted hue region (in degrees)
  float percentC   // percentage of scale: 1.0 is no adjustment (i.e. 100%)
)
{
  float ych[3] = rgb_2_ych( rgb);

  float centeredHue = center_hue( ych[2], centerH);

  float f_H = cubic_basis_shaper( centeredHue, widthH);

  float new_rgb[3];
  if (f_H > 0.0) {
    // Scale chroma in red/magenta region
    float new_ych[3] = ych;
    new_ych[1] = ych[1] * (f_H * (percentC - 1.0) + 1.0);
    new_rgb = ych_2_rgb( new_ych);
  } else { 
    // If not in affected hue region, just return original values
    // This helps to avoid precision errors that can occur in the RGB->YAB->RGB 
    // conversion
    new_rgb = rgb; 
  }

  return new_rgb;
}

float[3] scale_C_at_H_inv
( 
  float rgb[3], 
  float centerH,   // center of targeted hue region (in degrees)
  float widthH,    // full width at base of targeted hue region (in degrees)
  float percentC   // percentage of scale: 1.0 is no adjustment (i.e. 100%)
)
{
  float ych[3] = rgb_2_ych( rgb);

  float centeredHue = center_hue( ych[2], centerH);

  float f_H = cubic_basis_shaper( centeredHue, widthH);

  float new_rgb[3];
  if (f_H > 0.0) {
    // Scale chroma in red/magenta region
    float new_ych[3] = ych;
    new_ych[1] = ych[1] * 1.0/(f_H * (percentC - 1.0) + 1.0);
    new_rgb = ych_2_rgb( new_ych);
  } else { 
    // If not in affected hue region, just return original values
    // This helps to avoid precision errors that can occur in the RGB->YAB->RGB 
    // conversion
    new_rgb = rgb; 
  }

  return new_rgb;
}

int[3] order3( float r, float g, float b)
{  
  // Determine sort order, highest to lowest
   if (r > g) {
      if (g > b) {                    // r g b, hue [0,60]
         int order[3] = {0, 1, 2};
         return order;
      } else {
         if (r > b) {                 // r b g, hue [300,360]
            int order[3] = {0, 2, 1};
            return order;
         } else {                     // b r g, hue [240,300]
            int order[3] = {2, 0, 1};
            return order;
         }
      }
   }
   else {
      if (r > b) {                    // g r b, hue [60,120]
         int order[3] = {1, 0, 2};
         return order;
      } else {
         if (g > b) {                 // g b r, hue [120,180]
            int order[3] = {1, 2, 0};
            return order;
         } else {                     // b g r, hue [180,240]
            int order[3] = {2, 1, 0};
            return order;
         }
      }
   }
}

// Modify the hue of post_tone to match pre_tone
float[3] restore_hue_dw3( float pre_tone[3], float post_tone[3])
{
   int inds[3] = order3( pre_tone[0], pre_tone[1], pre_tone[2]);

   float orig_chroma = pre_tone[ inds[0]] - pre_tone[ inds[2]]; 

   float hue_factor = ( pre_tone[ inds[1] ] - pre_tone[ inds[2] ]) / orig_chroma;

   if ( orig_chroma == 0.) hue_factor = 0.;

   float new_chroma = post_tone[ inds[0] ] - post_tone[ inds[2] ];

   float out[3];
   out[ inds[ 0] ] = post_tone[ inds[0] ];
   out[ inds[ 1] ] = hue_factor * new_chroma + post_tone[ inds[2] ];
   out[ inds[ 2] ] = post_tone[ inds[2] ];

   return out;
}

// Functions to compress highlights to allow the simulated white point w/out clipping.
float roll_white_fwd( 
    float in,      // color value to adjust (white scaled to around 1.0)
    float new_wht, // white adjustment (e.g. 0.9 for 10% darkening)
    float width    // adjusted width (e.g. 0.25 for top quarter of the tone scale)
    )
{
    const float x0 = -1.0;
    const float x1 = x0 + width;
    const float y0 = -new_wht;
    const float y1 = x1;
    const float m1 = (x1 - x0);
    const float a = y0 - y1 + m1;
    const float b = 2 * ( y1 - y0) - m1;
    const float c = y0;
    const float t = (-in - x0) / (x1 - x0);
    float out = 0.0;
    if ( t < 0.0)
        out = -(t * b + c);
    else if ( t > 1.0)
        out = in;
    else
        out = -(( t * a + b) * t + c);
    return out;
}

float roll_white_rev( 
    float in,      // color value to adjust (white scaled to around 1.0)
    float new_wht, // white adjustment (e.g. 0.9 for 10% darkening)
    float width    // adjusted width (e.g. 0.25 for top quarter of the tone scale)
    )
{
    const float x0 = -1.0;
    const float x1 = x0 + width;
    const float y0 = -new_wht;
    const float y1 = x1;
    const float m1 = (x1 - x0);
    const float a = y0 - y1 + m1;
    const float b = 2. * ( y1 - y0) - m1;
    float c = y0;
    float out = 0.0;
    if ( -in < y0)
        out = -x0;
    else if ( -in > y1)
        out = in;
    else {
        c = c + in;
        const float discrim = sqrt( b * b - 4. * a * c);
        const float t = ( 2. * c) / ( -discrim - b);
        out = -(( t * ( x1 - x0)) + x0);
    }
    return out;
}
