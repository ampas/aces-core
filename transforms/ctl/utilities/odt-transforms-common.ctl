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
    1.60290,
    1.66893,
    1.69355
};

const float ODT_XMAX = rrt_tonescale_fwd( 0.18*pow(2.,6.5) );
const float ODT_XMID = rrt_tonescale_fwd( 0.18 );
const float ODT_XMIN = 0.0001;
const float ODT_YMAX = 48.0;
const float ODT_YMID = 4.8;
const float ODT_YMIN = 0.0048;

const float ODT_LO_SLOPE = 0.01;
const float ODT_HI_SLOPE = 0.04;

const int N_KNOTS_LOW = 8;
const int N_KNOTS_HIGH = 5;

float odt_tonescale_segmented_fwd
  ( 
    varying float x,
    varying float COEFS_LOW[9] = ODT_COEFS_LOW,
    varying float COEFS_HIGH[6] = ODT_COEFS_HIGH,
    varying float LO_SLOPE = ODT_LO_SLOPE,
    varying float HI_SLOPE = ODT_HI_SLOPE,    
    varying float XMAX = ODT_XMAX,
    varying float XMID = ODT_XMID,
    varying float XMIN = ODT_XMIN,
    varying float YMAX = ODT_YMAX,
    varying float YMID = ODT_YMID,
    varying float YMIN = ODT_YMIN
  )
{    
  // Check for negatives or zero before taking the log. If negative or zero,
  // set to ACESMIN.
  float xCheck = x;
  if (xCheck <= 0.0) xCheck = pow(2.,-14); 

  float logx = log10( xCheck);

  float logy;

  // For log values in the lower knot range, apply the B-spline shaper, b(x)
  if ( logx <= log10(XMIN) ) { 

    logy = logx * LO_SLOPE + ( log10(YMIN) - LO_SLOPE * log10(XMIN) );

  } else if (( logx > log10(XMIN) ) && ( logx < log10(XMID) )) {

    float knot_coord = (N_KNOTS_LOW-1) * (logx-log10(XMIN))/(log10(XMID)-log10(XMIN));
    int j = knot_coord;
    float t = knot_coord - j;

    float cf[ 3] = { COEFS_LOW[ j], COEFS_LOW[ j + 1], COEFS_LOW[ j + 2]};
    
    float monomials[ 3] = { t * t, t, 1. };
    logy = dot_f3_f3( monomials, mult_f3_f33( cf, M));

  } else if (( logx >= log10(XMID) ) && ( logx < log10(XMAX) )) {

    float knot_coord = (N_KNOTS_HIGH-1) * (logx-log10(XMID))/(log10(XMAX)-log10(XMID));
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


float odt_tonescale_segmented_rev
  ( 
    varying float y,
    varying float COEFS_LOW[9] = ODT_COEFS_LOW,
    varying float COEFS_HIGH[6] = ODT_COEFS_HIGH,
    varying float LO_SLOPE = ODT_LO_SLOPE,
    varying float HI_SLOPE = ODT_HI_SLOPE,    
    varying float XMAX = ODT_XMAX,
    varying float XMID = ODT_XMID,
    varying float XMIN = ODT_XMIN,
    varying float YMAX = ODT_YMAX,
    varying float YMID = ODT_YMID,
    varying float YMIN = ODT_YMIN
  )
{  
  const float KNOT_INC_LOW = (log10(XMID) - log10(XMIN)) / (N_KNOTS_LOW - 1.);
  const float KNOT_INC_HIGH = (log10(XMAX) - log10(XMID)) / (N_KNOTS_HIGH - 1.);
  
  // KNOT_Y is luminance of the spline at each knot
  float KNOT_Y_LOW[ N_KNOTS_LOW];
  for (int i = 0; i < N_KNOTS_LOW; i = i+1) {
    KNOT_Y_LOW[ i] = ( COEFS_LOW[i] + COEFS_LOW[i+1]) / 2.;
  };

  float KNOT_Y_HIGH[ N_KNOTS_HIGH];
  for (int i = 0; i < N_KNOTS_HIGH; i = i+1) {
    KNOT_Y_HIGH[ i] = ( COEFS_HIGH[i] + COEFS_HIGH[i+1]) / 2.;
  };

  float logy = log10( y);

  float logx;
  if (logy <= log10(YMIN)) {

    logx = (logy - ( log10(YMIN) - LO_SLOPE * log10(XMIN))) / LO_SLOPE;

  } else if ( (logy > log10(YMIN)) && (logy <= log10(YMID)) ) {

    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y_LOW[ 0] && logy <= KNOT_Y_LOW[ 1]) {
        cf[ 0] = COEFS_LOW[0];  cf[ 1] = COEFS_LOW[1];  cf[ 2] = COEFS_LOW[2];  j = 0;
    } else if ( logy > KNOT_Y_LOW[ 1] && logy <= KNOT_Y_LOW[ 2]) {
        cf[ 0] = COEFS_LOW[1];  cf[ 1] = COEFS_LOW[2];  cf[ 2] = COEFS_LOW[3];  j = 1;
    } else if ( logy > KNOT_Y_LOW[ 2] && logy <= KNOT_Y_LOW[ 3]) {
        cf[ 0] = COEFS_LOW[2];  cf[ 1] = COEFS_LOW[3];  cf[ 2] = COEFS_LOW[4];  j = 2;
    } else if ( logy > KNOT_Y_LOW[ 3] && logy <= KNOT_Y_LOW[ 4]) {
        cf[ 0] = COEFS_LOW[3];  cf[ 1] = COEFS_LOW[4];  cf[ 2] = COEFS_LOW[5];  j = 3;
    } else if ( logy > KNOT_Y_LOW[ 4] && logy <= KNOT_Y_LOW[ 5]) {
        cf[ 0] = COEFS_LOW[4];  cf[ 1] = COEFS_LOW[5];  cf[ 2] = COEFS_LOW[6];  j = 4;
    } else if ( logy > KNOT_Y_LOW[ 5] && logy <= KNOT_Y_LOW[ 6]) {
        cf[ 0] = COEFS_LOW[5];  cf[ 1] = COEFS_LOW[6];  cf[ 2] = COEFS_LOW[7];  j = 5;
    } else if ( logy > KNOT_Y_LOW[ 6] && logy <= KNOT_Y_LOW[ 7]) {
        cf[ 0] = COEFS_LOW[6];  cf[ 1] = COEFS_LOW[7];  cf[ 2] = COEFS_LOW[8];  j = 6;
    }
    
    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = log10(XMIN) + ( t + j) * KNOT_INC_LOW;

  } else if ( (logy > log10(YMID)) && (logy < log10(YMAX)) ) {

    unsigned int j;
    float cf[ 3];
    if ( logy > KNOT_Y_HIGH[ 0] && logy <= KNOT_Y_HIGH[ 1]) {
        cf[ 0] = COEFS_HIGH[0];  cf[ 1] = COEFS_HIGH[1];  cf[ 2] = COEFS_HIGH[2];  j = 0;
    } else if ( logy > KNOT_Y_HIGH[ 1] && logy <= KNOT_Y_HIGH[ 2]) {
        cf[ 0] = COEFS_HIGH[1];  cf[ 1] = COEFS_HIGH[2];  cf[ 2] = COEFS_HIGH[3];  j = 1;
    } else if ( logy > KNOT_Y_HIGH[ 2] && logy <= KNOT_Y_HIGH[ 3]) {
        cf[ 0] = COEFS_HIGH[2];  cf[ 1] = COEFS_HIGH[3];  cf[ 2] = COEFS_HIGH[4];  j = 2;
    } else if ( logy > KNOT_Y_HIGH[ 3] && logy <= KNOT_Y_HIGH[ 4]) {
        cf[ 0] = COEFS_HIGH[3];  cf[ 1] = COEFS_HIGH[4];  cf[ 2] = COEFS_HIGH[5];  j = 3;
    }
    
    const float tmp[ 3] = mult_f3_f33( cf, M);

    float a = tmp[ 0];
    float b = tmp[ 1];
    float c = tmp[ 2];
    c = c - logy;

    const float d = sqrt( b * b - 4. * a * c);

    const float t = ( 2. * c) / ( -d - b);

    logx = log10(XMID) + ( t + j) * KNOT_INC_HIGH;

  } else { //if ( logy >= log10(YMAX) ) {

    logx = (logy - ( log10(YMAX) - HI_SLOPE * log10(XMAX))) / HI_SLOPE;

  }
  
  return pow10( logx);
}


float Y_2_linCV( float Y, float Ymax, float Ymin) 
{
  return (Y - Ymin) / (Ymax - Ymin);
}

float linCV_2_Y( float linCV, float Ymax, float Ymin) 
{
  return linCV * (Ymax - Ymin) + Ymin;
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