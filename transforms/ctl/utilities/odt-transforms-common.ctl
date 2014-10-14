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



const float DEFAULT_YMIN_ABS = OCESMID;
const float DEFAULT_YMAX_ABS = 48.0;
const float DEFAULT_ODT_HI_SLOPE = 0.04;

const float DEFAULT_ODT_COEFS[6] = { 
	0.37358,
	0.98891,
	1.44330,
	1.60290,
	1.66893,
	1.69355
};

const unsigned int ODT_KNOT_LEN = 5;

float odt_tonescale_fwd
( // input is absolute luminance, output is absolute luminance
  varying float oces,
  varying float COEFS[6] = DEFAULT_ODT_COEFS,
  varying float hi_slope = DEFAULT_ODT_HI_SLOPE,
  varying float lumMin = DEFAULT_YMIN_ABS,
  varying float lumMax = DEFAULT_YMAX_ABS
)
{
  float ODT_KNOT_START = log10( rrt_tonescale_fwd( 0.18, RRT_COEFS) );
  float ODT_KNOT_END = log10( rrt_tonescale_fwd( 0.18*pow(2.,6.5), RRT_COEFS) );

  float ODT_KNOT_SPAN = ODT_KNOT_END - ODT_KNOT_START;
  float ODT_KNOT_INC = ODT_KNOT_SPAN / (ODT_KNOT_LEN - 1.);

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
    //     } 

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
  varying float COEFS[6] = DEFAULT_ODT_COEFS,   
  varying float hi_slope = DEFAULT_ODT_HI_SLOPE,
  varying float lumMin = DEFAULT_YMIN_ABS,
  varying float lumMax = DEFAULT_YMAX_ABS
)
{
  float ODT_KNOT_START = log10( rrt_tonescale_fwd( 0.18, RRT_COEFS) );
  float ODT_KNOT_END = log10( rrt_tonescale_fwd( 0.18*pow(2.,6.5), RRT_COEFS) );

  float ODT_KNOT_SPAN = ODT_KNOT_END - ODT_KNOT_START;
  float ODT_KNOT_INC = ODT_KNOT_SPAN / (ODT_KNOT_LEN - 1.);

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
    } else {
        cf[ 0] = COEFS[3];  cf[ 1] = COEFS[4];  cf[ 2] = COEFS[5];  j = 3;
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



/* --- Black point compensation --- */
float[3] bpc_fwd
( 
  float rgb[3],
  float SCALE,
  float BPC,
  float OUT_BP,
  float OUT_WP
)
{  
  // Apply black point compensation
  float offset_scaled[3];
  offset_scaled[0] = ( SCALE * rgb[0]) + BPC;
  offset_scaled[1] = ( SCALE * rgb[1]) + BPC;
  offset_scaled[2] = ( SCALE * rgb[2]) + BPC;

  // Convert luminance to black and white normalized code values
  //  scales the output device black point to CV 0.0
  //  Scales the output device white point to CV 1.0
  float bw_scaled[3];
  bw_scaled[0] = ( offset_scaled[0] - OUT_BP) / ( OUT_WP - OUT_BP);
  bw_scaled[1] = ( offset_scaled[1] - OUT_BP) / ( OUT_WP - OUT_BP);
  bw_scaled[2] = ( offset_scaled[2] - OUT_BP) / ( OUT_WP - OUT_BP);

  return bw_scaled;
}

float[3] bpc_inv
( 
  float bw_scaled[3], 
  float SCALE, 
  float BPC, 
  float OUT_BP, 
  float OUT_WP
)
{  
  // Convert black and white normalized code values to luminance
  //  scales CV 0.0 to output device black point
  //  scales CV 1.0 to output device white point
  float offset_scaled[3];
  offset_scaled[0] = bw_scaled[0] * ( OUT_WP - OUT_BP) + OUT_BP;
  offset_scaled[1] = bw_scaled[1] * ( OUT_WP - OUT_BP) + OUT_BP;
  offset_scaled[2] = bw_scaled[2] * ( OUT_WP - OUT_BP) + OUT_BP;

  // Undo black point compensation
  float rgb[3];
  rgb[0] = ( offset_scaled[0] - BPC) / SCALE;
  rgb[1] = ( offset_scaled[1] - BPC) / SCALE;
  rgb[2] = ( offset_scaled[2] - BPC) / SCALE;

  return rgb;
}

const float OCES_BP_CINEMA = 0.0001;  // luminance of OCES black point. 
                                      // (to be mapped to device black point)
const float OCES_WP_CINEMA = 48.0;    // luminance of OCES white point 
                                      // (to be mapped to device white point)
const float OUT_BP_CINEMA = 0.0048;   // luminance of output device black point 
                                      // (to which OCES black point is mapped)
const float OUT_WP_CINEMA = 48.0;     // luminance of output device white point
                                      // (to which OCES black point is mapped)
const float BPC_CINEMA = (OCES_BP_CINEMA * OUT_WP_CINEMA - OCES_WP_CINEMA * OUT_BP_CINEMA) / (OCES_BP_CINEMA - OCES_WP_CINEMA);
const float SCALE_CINEMA = (OUT_BP_CINEMA - OUT_WP_CINEMA) / (OCES_BP_CINEMA - OCES_WP_CINEMA);

float[3] bpc_cinema_fwd( float rgb[3])
{  
  return bpc_fwd( rgb, SCALE_CINEMA, BPC_CINEMA, OUT_BP_CINEMA, OUT_WP_CINEMA);
}

float[3] bpc_cinema_inv( float rgb[3])
{  
  return bpc_inv( rgb, SCALE_CINEMA, BPC_CINEMA, OUT_BP_CINEMA, OUT_WP_CINEMA);
}


const float OCES_BP_VIDEO = 0.0016;   // luminance of OCES black point. 
                                      // (to be mapped to device black point)
const float OCES_WP_VIDEO = 48.0;     // luminance of OCES white point 
                                      // (to be mapped to device white point)
const float OUT_BP_VIDEO = 0.01;      // luminance of output device black point 
                                      // (to which OCES black point is mapped)
const float OUT_WP_VIDEO = 100.0;     // luminance of output device white point
                                      // (to which OCES black point is mapped)
const float BPC_VIDEO = (OCES_BP_VIDEO * OUT_WP_VIDEO - OCES_WP_VIDEO * OUT_BP_VIDEO) / (OCES_BP_VIDEO - OCES_WP_VIDEO);
const float SCALE_VIDEO = (OUT_BP_VIDEO - OUT_WP_VIDEO) / (OCES_BP_VIDEO - OCES_WP_VIDEO);

float[3] bpc_video_fwd( float rgb[3])
{  
  return bpc_fwd( rgb, SCALE_VIDEO, BPC_VIDEO, OUT_BP_VIDEO, OUT_WP_VIDEO);
}

float[3] bpc_video_inv( float rgb[3])
{  
  return bpc_inv( rgb, SCALE_VIDEO, BPC_VIDEO, OUT_BP_VIDEO, OUT_WP_VIDEO);
}



float[3] smpteRange_to_fullRange( float rgbIn[3])
{
	const float REFBLACK = (  16. / 256.);
	const float REFWHITE = ( 235. / 256.);
	
	float rgbOut[3];
	rgbOut[0] = ( rgbIn[0] - REFBLACK) / ( REFWHITE - REFBLACK);
	rgbOut[1] = ( rgbIn[1] - REFBLACK) / ( REFWHITE - REFBLACK);
	rgbOut[2] = ( rgbIn[2] - REFBLACK) / ( REFWHITE - REFBLACK);

  return rgbOut;
}

float[3] fullRange_to_smpteRange( float rgbIn[3])
{
	const float REFBLACK = (  16. / 256.);
	const float REFWHITE = ( 235. / 256.);
	
	float rgbOut[3];
	rgbOut[0] = rgbIn[0] * ( REFWHITE - REFBLACK) + REFBLACK;
	rgbOut[1] = rgbIn[1] * ( REFWHITE - REFBLACK) + REFBLACK;
	rgbOut[2] = rgbIn[2] * ( REFWHITE - REFBLACK) + REFBLACK;

  return rgbOut;
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