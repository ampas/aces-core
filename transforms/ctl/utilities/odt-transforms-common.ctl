//
// odt-transforms-common.ctl
// WGR8
//
// Contains functions and constants shared by forward and inverse ODT transforms 
// This is primarily to avoid code redundancy.
// This structure also has the benefit of facilitating any updates to these
// parameters or functions, if necessary.
//



import "rrt-transform-common";



const float D60_2_D65_CAT[3][3] = calculate_cat_matrix( ACES_PRI.white, REC709_PRI.white);


const float ODT_COEFS_LOW[9] = {
      -2.3188,
      -2.3188,
      -2.2614,
      -2.0618,
      -1.5918,
      -0.9764,
     -0.31832,
      0.35613,
       1.0064
};

const float ODT_COEFS_HIGH[6] = {
    0.37358,
	0.98891,
	1.44330,
	1.6029,
	1.66893,
	1.69355
};

const float ODT_XMAX = 1388.6;
const float ODT_XMID = 4.8;
const float ODT_XMIN = 0.0001;
const float ODT_YMAX = 48.0;
const float ODT_YMID = 4.8;
const float ODT_YMIN = 0.0048;

float odt_tonescale_segmented_fwd
  ( 
    varying float in,
    varying float COEFS_LOW[9] = ODT_COEFS_LOW,
    varying float COEFS_HIGH[6] = ODT_COEFS_HIGH,
    varying float XMAX = ODT_XMAX,
    varying float XMID = ODT_XMID,
    varying float XMIN = ODT_XMIN,
    varying float YMAX = ODT_YMAX,
    varying float YMID = ODT_YMID,
    varying float YMIN = ODT_YMIN
  )
{    
  const int N_KNOTS_LOW = 8;
  const int N_KNOTS_HIGH = 5;

  const float LO_SLOPE = 0.01;
  const float HI_SLOPE = 0.04;

  // Check for negatives or zero before taking the log. If negative or zero,
  // set to ACESMIN.
  float inCheck = in;
  if (inCheck <= 0.0) inCheck = pow(2.,-14); 

  float logIn = log10( inCheck);

  float logOut;

  // For log values in the lower knot range, apply the B-spline shaper, b(x)
  if ( logIn < log10(XMIN) ) 
  { 
    logOut = logIn * LO_SLOPE + ( log10(YMIN) - LO_SLOPE * log10(XMIN) );
  } 
  else if (( logIn >= log10(XMIN) ) && ( logIn < log10(XMID) )) 
  {
    float knot_coord = (N_KNOTS_LOW-1) * (logIn-log10(XMIN))/(log10(XMID)-log10(XMIN));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { COEFS_LOW[ j], COEFS_LOW[ j + 1], COEFS_LOW[ j + 2]};
    
    float monomials[ 3] = { t * t, t, 1. };
    logOut = dot_f3_f3( monomials, mult_f3_f33( cf, M));
  }    
  else if (( logIn >= log10(XMID) ) && ( logIn < log10(XMAX) )) 
  {
    float knot_coord = (N_KNOTS_HIGH-1) * (logIn-log10(XMID))/(log10(XMAX)-log10(XMID));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { COEFS_HIGH[ j], COEFS_HIGH[ j + 1], COEFS_HIGH[ j + 2]}; 

    float monomials[ 3] = { t * t, t, 1. };
    logOut = dot_f3_f3( monomials, mult_f3_f33( cf, M));
  } 
  else if ( logIn >= log10(XMAX) ) 
  { 
    logOut = logIn * HI_SLOPE + ( log10(YMAX) - HI_SLOPE * log10(XMAX));
  }

  return pow10(logOut);
}



float[3] huePreservingClip_to_p3d60( float XYZ[3])
{
  // Converts CIE XYZ tristimulus values to P3D60, performs a "smart-clip" by 
  // clamping to device primaries and performing a hue restore. The resulting P3
  // code values are then converted back to CIE XYZ tristimulus values and 
  // returned.
  
  const float XYZ_2_P3D60_PRI_MAT[4][4] = XYZtoRGB(P3D60_PRI,1.0);
  const float P3D60_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(P3D60_PRI,1.0);
  
  // CIE XYZ to P3D60 primaries
  float p3[3] = mult_f3_f44( XYZ, XYZ_2_P3D60_PRI_MAT);

  // Clip values < 0 or > 1 (i.e. projecting outside the display primaries)
  float p3Clamp[3] = clamp_f3( p3, 0., 1.);

  // Restore hue after clip operation ("smart-clip")
  p3 = restore_hue_dw3( p3, p3Clamp);

  // P3D60 to CIE XYZ
  return mult_f3_f44( p3, P3D60_PRI_2_XYZ_MAT);  
}




/* ---- Functions to compress highlights ---- */
// allow for simulated white points without clipping

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