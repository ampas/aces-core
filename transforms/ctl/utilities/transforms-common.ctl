//
// transforms-common.ctl
//
// WGR8.5
//
// Contains functions and constants shared by multiple forward and inverse 
// transforms 
//


import "utilities-color";


// Rendering primaries
const Chromaticities RENDER_PRI = 
{
  {0.713,	0.293},
  {0.165,	0.830},
  {0.128,	0.044},
  {0.32168,	0.33767}
};

const float ACES_2_XYZ_MAT[4][4] = RGBtoXYZ( ACES_PRI, 1.0);
const float XYZ_2_ACES_MAT[4][4] = XYZtoRGB( ACES_PRI, 1.0);
const float XYZ_2_RENDER_PRI_MAT[4][4] = XYZtoRGB( RENDER_PRI, 1.0);
const float RENDER_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ( RENDER_PRI, 1.0);

const float ACES_2_RENDER_PRI_MAT[4][4] = mult_f44_f44( ACES_2_XYZ_MAT, XYZ_2_RENDER_PRI_MAT);
const float RENDER_PRI_2_ACES_MAT[4][4] = invert_f44( ACES_2_RENDER_PRI_MAT);

const float RENDER_RGB2Y[3] = { RENDER_PRI_2_XYZ_MAT[0][1], 
                                RENDER_PRI_2_XYZ_MAT[1][1], 
                                RENDER_PRI_2_XYZ_MAT[2][1] };



const float TINY = 1e-10;


// Textbook monomial to basis-function conversion matrix.
const float M[ 3][ 3] = {
  {  0.5, -1.0, 0.5 },
  { -1.0,  1.0, 0.5 },
  {  0.5,  0.0, 0.0 }
};


float rgb_2_saturation( float rgb[3])
{
  return ( max( max_f3(rgb), TINY) - max( min_f3(rgb), TINY)) / max( max_f3(rgb), 1e-2);
}



const int SYS_N_KNOTS_LOW = 4;
const int SYS_N_KNOTS_HIGH = 4;

float bspline_segmented_fwd
  ( 
    varying float x,
    varying float COEFS_LOW[5],
    varying float COEFS_HIGH[5],
    varying float LO_SLOPE,
    varying float HI_SLOPE,
    varying float XMAX,
    varying float XMID,
    varying float XMIN,
    varying float YMAX,
    varying float YMID,
    varying float YMIN
  )
{    
  // Check for negatives or zero before taking the log. If negative or zero,
  // set to ACESMIN.
  float xCheck = x;
  if (xCheck <= 0.0) xCheck = pow(2.,-14); 

  float logx = log10( xCheck);

  float logy;

  if ( logx <= log10(XMIN) ) { 

    logy = logx * LO_SLOPE + ( log10(YMIN) - LO_SLOPE * log10(XMIN) );

  } else if (( logx > log10(XMIN) ) && ( logx < log10(XMID) )) {

    float knot_coord = (SYS_N_KNOTS_LOW-1) * (logx-log10(XMIN))/(log10(XMID)-log10(XMIN));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { COEFS_LOW[ j], COEFS_LOW[ j + 1], COEFS_LOW[ j + 2]};
    
    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else if (( logx >= log10(XMID) ) && ( logx < log10(XMAX) )) {

    float knot_coord = (SYS_N_KNOTS_HIGH-1) * (logx-log10(XMID))/(log10(XMAX)-log10(XMID));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { COEFS_HIGH[ j], COEFS_HIGH[ j + 1], COEFS_HIGH[ j + 2]}; 

    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else { //if ( logIn >= log10(XMAX) ) { 

    logy = logx * HI_SLOPE + ( log10(YMAX) - HI_SLOPE * log10(XMAX) );

  }

  return pow10(logy);
  
}
