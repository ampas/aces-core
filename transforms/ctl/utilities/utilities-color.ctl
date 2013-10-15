//
// utilities-color.ctl
// v0.2.2
//
// Color related constants and functions
//

/* ---- Chromaticities of some common primary sets ---- */

const Chromaticities ACES_PRI =
{
  { 0.73470,  0.26530},
  { 0.00000,  1.00000},
  { 0.00010, -0.07700},
  { 0.32168,  0.33767}
};

const Chromaticities REC709_PRI =
{
  { 0.64000,  0.33000},
  { 0.30000,  0.60000},
  { 0.15000,  0.06000},
  { 0.31270,  0.32900}
};

const Chromaticities P3D60_PRI =
{
  { 0.68000,  0.32000},
  { 0.26500,  0.69000},
  { 0.15000,  0.06000},
  { 0.32168,  0.33767}
};

const Chromaticities P3DCI_PRI =
{
  { 0.68000,  0.32000},
  { 0.26500,  0.69000},
  { 0.15000,  0.06000},
  { 0.31400,  0.35100}
};

const Chromaticities ARRI_ALEXA_WG_PRI =
{
  {0.68400, 0.31300},
  {0.22100, 0.84800},
  {0.08610, -0.10200},
  {0.31270, 0.32900}
};

const Chromaticities REC2020_PRI = 
{
  {0.70800, 0.29200},
  {0.17000, 0.79700},
  {0.13100, 0.04600},
  {0.31270, 0.32900}
};

const Chromaticities RIMMROMM_PRI = 
{
  {0.7347, 0.2653},
  {0.1596, 0.8404},
  {0.0366, 0.0001},
  {0.3457, 0.3585}
};




/* ---- Conversion Functions ---- */
// Various transformations between color encodings and data representations
//

// Transformations between CIE XYZ tristimulus values and CIE x,y 
// chromaticity coordinates
float[3] XYZ_2_xyY( float XYZ[3])
{  
  float xyY[3];
  xyY[0] = XYZ[0] / (XYZ[0] + XYZ[1] + XYZ[2]);
  xyY[1] = XYZ[1] / (XYZ[0] + XYZ[1] + XYZ[2]);  
  xyY[2] = XYZ[1];
  
  return xyY;
}

float[3] xyY_2_XYZ( input varying float xyY[3])
{
  float XYZ[3];  
  XYZ[0] = xyY[0] * xyY[2] / xyY[1];
  XYZ[1] = xyY[2];  
  XYZ[2] = (1.0 - xyY[0] - xyY[1]) * xyY[2] / xyY[1];
  
  return XYZ;
}


// Transformations between an RGB space, a "Yab" space, and a "YCH" space
const float sqrt3over4 = 0.433012701892219;  // sqrt(3.)/4.
const float RGB_2_YAB_MAT[3][3] = {
  {1./3., 1./2., 0.0},
  {1./3., -1./4.,  sqrt3over4},
  {1./3., -1./4., -sqrt3over4}
};

float[3] rgb_2_yab( float rgb[3])
{
  float yab[3] = mult_f3_f33( rgb, RGB_2_YAB_MAT);

  return yab;
}

float[3] yab_2_rgb( float yab[3])
{
  float rgb[3] = mult_f3_f33( yab, invert_f33(RGB_2_YAB_MAT));

  return rgb;
}

float[3] yab_2_ych( float yab[3])
{
  float ych[3] = yab;

  ych[1] = sqrt( pow( yab[1], 2.) + pow( yab[2], 2.) );

  ych[2] = atan2( yab[2], yab[1] ) * (180.0 / M_PI);
  if (ych[2] < 0.0) ych[2] = ych[2] + 360.;

  return ych;
}

float[3] ych_2_yab( float ych[3] ) 
{
  float yab[3];
  yab[0] = ych[0];

  float h = ych[2] * (M_PI / 180.0);
  yab[1] = ych[1]*cos(h);
  yab[2] = ych[1]*sin(h);

  return yab;
}

float[3] rgb_2_ych( float rgb[3]) 
{
  return yab_2_ych( rgb_2_yab( rgb));
}

float[3] ych_2_rgb( float ych[3]) 
{
  return yab_2_rgb( ych_2_yab( ych));
}




/* ---- Chromatic Adaptation ---- */

const float CONE_RESP_MAT_BRADFORD[3][3] = {
  {0.89510, -0.75020, 0.03890},
  {0.26640, 1.71350, -0.06850},
  {-0.16140, 0.03670, 1.02960}
};

const float CONE_RESP_MAT_CAT02[3][3] = {
  {0.73280, -0.70360, 0.00300},
  {0.42960, 1.69750, 0.01360},
  {-0.16240, 0.00610, 0.98340}
};

float[3][3] calculate_cat_matrix
  ( float src_xy[2],         // Chromaticity of source white
    float des_xy[2],         // Chromaticity of destination white
    float coneRespMat[3][3] = CONE_RESP_MAT_BRADFORD
  )
{
  //
  // Calculates and returns a 3x3 Von Kries chromatic adaptation transform 
  // from src_xy to des_xy using the cone response primaries defined 
  // by coneRespMat. By default, coneRespMat is set to CONE_RESP_MAT_BRADFORD. 
  // The default coneRespMat can be overridden at runtime. 
  //

  const float src_xyY[3] = { src_xy[0], src_xy[1], 1. };
  const float des_xyY[3] = { des_xy[0], des_xy[1], 1. };

  float src_XYZ[3] = xyY_2_XYZ( src_xyY );
  float des_XYZ[3] = xyY_2_XYZ( des_xyY );

  float src_coneResp[3] = mult_f3_f33( src_XYZ, coneRespMat);
  float des_coneResp[3] = mult_f3_f33( des_XYZ, coneRespMat);

  float vkMat[3][3] = {
      { des_coneResp[0] / src_coneResp[0], 0.0, 0.0 },
      { 0.0, des_coneResp[1] / src_coneResp[1], 0.0 },
      { 0.0, 0.0, des_coneResp[2] / src_coneResp[2] }
  };

  float cat_matrix[3][3] = mult_f33_f33( coneRespMat, transpose_f33( mult_f33_f33( transpose_f33( invert_f33( coneRespMat ) ), vkMat ) ) );

  return cat_matrix;
}





float[3][3] calc_sat_adjust_matrix 
  ( input varying float sat,
    input varying float rgb2Y[3]
  )
{
  //
  // This function determines the terms for a 3x3 saturation matrix that is
  // based on the luminance of the input.
  //
  float M[3][3];
  M[0][0] = (1.0 - sat) * rgb2Y[0] + sat;
  M[1][0] = (1.0 - sat) * rgb2Y[0];
  M[2][0] = (1.0 - sat) * rgb2Y[0];
  
  M[0][1] = (1.0 - sat) * rgb2Y[1];
  M[1][1] = (1.0 - sat) * rgb2Y[1] + sat;
  M[2][1] = (1.0 - sat) * rgb2Y[1];
  
  M[0][2] = (1.0 - sat) * rgb2Y[2];
  M[1][2] = (1.0 - sat) * rgb2Y[2];
  M[2][2] = (1.0 - sat) * rgb2Y[2] + sat;

  M = transpose_f33(M);    
  return M;
} 





/* ---- Signal encode/decode functions ---- */

float moncurve_f( float x, float gamma, float offs )
{
  // Forward monitor curve
  float y;
  const float fs = (( gamma - 1.0) / offs) * pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
  const float xb = offs / ( gamma - 1.0);
  if ( x >= xb) 
    y = pow( ( x + offs) / ( 1.0 + offs), gamma);
  else
    y = x * fs;
  return y;
}

float moncurve_r( float y, float gamma, float offs )
{
  // Reverse monitor curve
  float x;
  const float yb = pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
  const float rs = pow( ( gamma - 1.0) / offs, gamma - 1.0) * pow( ( 1.0 + offs) / gamma, gamma);
  if ( y >= yb) 
    x = ( 1.0 + offs) * pow( y, 1.0 / gamma) - offs;
  else
    x = y * rs;
  return x;
}

float bt1886_f( float V, float gamma, float Lw, float Lb)
{
  // The reference EOTF specified in Rec. ITU-R BT.1886
  // L = a(max[(V+b),0])^g
  float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
  float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
  float L = a * pow( max( V + b, 0.), gamma);
  return L;
}

float bt1886_r( float L, float gamma, float Lw, float Lb)
{
  // The reference EOTF specified in Rec. ITU-R BT.1886
  // L = a(max[(V+b),0])^g
  float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
  float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
  float V = pow( max( L / a, 0.), 1./gamma) - b;
  return V;
}