
// <ACEStransformID>ACESlib.RRT_Common.a1.0.0</ACEStransformID>

//
// Contains functions and constants shared by forward and inverse RRT transforms
//



// "Glow" module constants
const float RRT_GLOW_GAIN = 0.05;
const float RRT_GLOW_MID = 0.08;

// Red modifier constants
const float RRT_RED_SCALE = 0.82;
const float RRT_RED_PIVOT = 0.03;
const float RRT_RED_HUE = 0.;
const float RRT_RED_WIDTH = 135.;

// Desaturation contants
const float RRT_SAT_FACTOR = 0.96;
const float RRT_SAT_MAT[3][3] = calc_sat_adjust_matrix( RRT_SAT_FACTOR, AP1_RGB2Y);




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
