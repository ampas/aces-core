
// <ACEStransformID>ACESlib.ODT_Common.a1.0.0</ACEStransformID>

//
// Contains functions and constants shared by forward and inverse ODT transforms 
//




// Target white and black points for cinema system tonescale
const float CINEMA_WHITE = 48.0;
const float CINEMA_BLACK = 0.02; // CINEMA_WHITE / 2400.




// Gamma compensation factor
const float DIM_SURROUND_GAMMA = 0.9811;

// Saturation compensation factor
const float ODT_SAT_FACTOR = 0.93;
const float ODT_SAT_MAT[3][3] = calc_sat_adjust_matrix( ODT_SAT_FACTOR, AP1_RGB2Y);




const float D60_2_D65_CAT[3][3] = calculate_cat_matrix( AP0.white, REC709_PRI.white);




float Y_2_linCV( float Y, float Ymax, float Ymin) 
{
  return (Y - Ymin) / (Ymax - Ymin);
}

float linCV_2_Y( float linCV, float Ymax, float Ymin) 
{
  return linCV * (Ymax - Ymin) + Ymin;
}



float[3] darkSurround_to_dimSurround( float linearCV[3])
{
  float XYZ[3] = mult_f3_f44( linearCV, AP1_2_XYZ_MAT); 

  float xyY[3] = XYZ_2_xyY(XYZ);
  xyY[2] = clamp( xyY[2], 0., HALF_POS_INF);
  xyY[2] = pow( xyY[2], DIM_SURROUND_GAMMA);
  XYZ = xyY_2_XYZ(xyY);

  return mult_f3_f44( XYZ, XYZ_2_AP1_MAT);
}

float[3] dimSurround_to_darkSurround( float linearCV[3])
{
  float XYZ[3] = mult_f3_f44( linearCV, AP1_2_XYZ_MAT); 

  float xyY[3] = XYZ_2_xyY(XYZ);
  xyY[2] = clamp( xyY[2], 0., HALF_POS_INF);
  xyY[2] = pow( xyY[2], 1./DIM_SURROUND_GAMMA);
  XYZ = xyY_2_XYZ(xyY);

  return mult_f3_f44( XYZ, XYZ_2_AP1_MAT);
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