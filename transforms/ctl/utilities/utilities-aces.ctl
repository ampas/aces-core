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
const float M_ODT[ 3][ 3] = {
  {  0.5, -1.0, 0.5 },
  { -1.0,  1.0, 0.5 },
  {  0.5,  0.0, 0.0 }
};

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
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M_ODT));
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

    const float tmp[ 3] = mult_f3_f33( cf, M_ODT);

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
