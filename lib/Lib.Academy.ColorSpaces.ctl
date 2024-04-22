
// <ACEStransformID>urn:ampas:aces:transformId:v2.0:Lib.Academy.ColorSpaces.a2.v1</ACEStransformID>
// <ACESuserName>Color Space and Conversion Matrices</ACESuserName>

//
// Constants and functions for assorted common color spaces
//



/* ---- Chromaticities of some common primary sets ---- */

const Chromaticities AP0 = // ACES Primaries from SMPTE ST2065-1
{
  { 0.73470,  0.26530},
  { 0.00000,  1.00000},
  { 0.00010, -0.07700},
  { 0.32168,  0.33767}
};

const Chromaticities AP1 = // Working space and rendering primaries for ACES 1.0
{
  { 0.713,    0.293},
  { 0.165,    0.830},
  { 0.128,    0.044},
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

const Chromaticities P3D65_PRI =
{
  { 0.68000,  0.32000},
  { 0.26500,  0.69000},
  { 0.15000,  0.06000},
  { 0.31270,  0.32900}
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
  { 0.68400,  0.31300},
  { 0.22100,  0.84800},
  { 0.08610, -0.10200},
  { 0.31270,  0.32900}
};

const Chromaticities REC2020_PRI = 
{
  { 0.70800,  0.29200},
  { 0.17000,  0.79700},
  { 0.13100,  0.04600},
  { 0.31270,  0.32900}
};

const Chromaticities RIMMROMM_PRI = 
{
  { 0.7347,  0.2653},
  { 0.1596,  0.8404},
  { 0.0366,  0.0001},
  { 0.3457,  0.3585}
};

const Chromaticities SONY_SGAMUT3_PRI =
{
  { 0.730,  0.280},
  { 0.140,  0.855},
  { 0.100, -0.050},
  { 0.3127,  0.3290}
};

const Chromaticities SONY_SGAMUT3_CINE_PRI =
{
  { 0.766,  0.275},
  { 0.225,  0.800},
  { 0.089, -0.087},
  { 0.3127,  0.3290}
};

// Note: No official published primaries exist as of this day for the
// Sony VENICE SGamut3 and Sony VENICE SGamut3.Cine colorspaces. The primaries
// have thus been derived from the IDT matrices.
const Chromaticities SONY_VENICE_SGAMUT3_PRI =
{
  { 0.740464264304292,  0.279364374750660},
  { 0.089241145423286,  0.893809528608105},
  { 0.110488236673827, -0.052579333080476},
  { 0.312700000000000,  0.329000000000000}
};

const Chromaticities SONY_VENICE_SGAMUT3_CINE_PRI =
{
  { 0.775901871567345,  0.274502392854799},
  { 0.188682902773355,  0.828684937020288},
  { 0.101337382499301, -0.089187517306263},
  { 0.312700000000000,  0.329000000000000}
};

const Chromaticities CANON_CGAMUT_PRI =
{
  { 0.7400,  0.2700},
  { 0.1700,  1.1400},
  { 0.0800, -0.1000},
  { 0.3127,  0.3290}
};

const Chromaticities RED_WIDEGAMUTRGB_PRI =
{
  { 0.780308,  0.304253},
  { 0.121595,  1.493994},
  { 0.095612, -0.084589},
  { 0.3127,  0.3290}
};

const Chromaticities PANASONIC_VGAMUT_PRI =
{
  { 0.730,  0.280},
  { 0.165,  0.840},
  { 0.100, -0.030},
  { 0.3127,  0.3290}
};

const Chromaticities BMD_CAM_WG_GEN5_PRI =
{
  { 0.7177215,  0.3171181},
  { 0.2280410,  0.8615690},
  { 0.1005841, -0.0820452},
  { 0.3127170,  0.3290312}
};


/* ---- Conversion Functions ---- */
// Various transformations between color encodings and data representations
//

// Transformations between CIE XYZ tristimulus values and CIE x,y 
// chromaticity coordinates
float[3] XYZ_2_xyY( float XYZ[3])
{  
  float xyY[3];
  float divisor = (XYZ[0] + XYZ[1] + XYZ[2]);
  if (divisor == 0.) divisor = 1e-10;
  xyY[0] = XYZ[0] / divisor;
  xyY[1] = XYZ[1] / divisor;  
  xyY[2] = XYZ[1];
  
  return xyY;
}

float[3] xyY_2_XYZ( input varying float xyY[3])
{
  float XYZ[3];
  XYZ[0] = xyY[0] * xyY[2] / max( xyY[1], 1e-10);
  XYZ[1] = xyY[2];  
  XYZ[2] = (1.0 - xyY[0] - xyY[1]) * xyY[2] / max( xyY[1], 1e-10);

  return XYZ;
}

/* ---- Chromatic Adaptation ---- */

const float CONE_RESP_MAT_BRADFORD[3][3] = {
  { 0.89510, -0.75020,  0.03890},
  { 0.26640,  1.71350, -0.06850},
  {-0.16140,  0.03670,  1.02960}
};

const float CONE_RESP_MAT_CAT02[3][3] = {
  { 0.73280, -0.70360,  0.00300},
  { 0.42960,  1.69750,  0.01360},
  {-0.16240,  0.00610,  0.98340}
};

float[3][3] calculate_cat_matrix
  ( float src_xy[2],         // x,y chromaticity of source white
    float des_xy[2],         // x,y chromaticity of destination white
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

  float cat_matrix[3][3] = mult_f33_f33( coneRespMat, mult_f33_f33( vkMat, invert_f33( coneRespMat ) ) );

  return cat_matrix;
}

float[3][3] calculate_rgb_to_rgb_matrix
  ( Chromaticities SOURCE_PRIMARIES,
    Chromaticities DEST_PRIMARIES,
    float coneRespMat[3][3] = CONE_RESP_MAT_BRADFORD
  )
{
  //
  // Calculates and returns a 3x3 RGB-to-RGB matrix from the source primaries to the 
  // destination primaries. The returned matrix is effectively a concatenation of a 
  // conversion of the source RGB values into CIE XYZ tristimulus values, conversion to
  // cone response values or other space in which reconciliation of the encoding white is 
  // done, a conversion back to CIE XYZ tristimulus values, and finally conversion from 
  // CIE XYZ tristimulus values to the destination RGB values.
  //
  // By default, coneRespMat is set to CONE_RESP_MAT_BRADFORD. 
  // The default coneRespMat can be overridden at runtime. 
  //

  const float RGBtoXYZ_44[4][4] = RGBtoXYZ( SOURCE_PRIMARIES, 1.0);
  const float RGBtoXYZ_MAT[3][3] = 
    { {RGBtoXYZ_44[0][0], RGBtoXYZ_44[0][1], RGBtoXYZ_44[0][2]}, 
      {RGBtoXYZ_44[1][0], RGBtoXYZ_44[1][1], RGBtoXYZ_44[1][2]}, 
      {RGBtoXYZ_44[2][0], RGBtoXYZ_44[2][1], RGBtoXYZ_44[2][2]} };

  // Chromatic adaptation from source white to destination white chromaticity
  // Bradford cone response matrix is the default method
  const float CAT[3][3] = calculate_cat_matrix( SOURCE_PRIMARIES.white, 
                                                DEST_PRIMARIES.white,
                                                coneRespMat );

  const float XYZtoRGB_44[4][4] = XYZtoRGB( DEST_PRIMARIES, 1.0);
  const float XYZtoRGB_MAT[3][3] = 
    { {XYZtoRGB_44[0][0], XYZtoRGB_44[0][1], XYZtoRGB_44[0][2]}, 
      {XYZtoRGB_44[1][0], XYZtoRGB_44[1][1], XYZtoRGB_44[1][2]}, 
      {XYZtoRGB_44[2][0], XYZtoRGB_44[2][1], XYZtoRGB_44[2][2]}};

  return mult_f33_f33( RGBtoXYZ_MAT, mult_f33_f33( CAT, XYZtoRGB_MAT));
}


/* ---- Chromaticities of some common primary sets ---- */

const float AP0_2_XYZ_MAT[4][4] = RGBtoXYZ( AP0, 1.0);
const float XYZ_2_AP0_MAT[4][4] = XYZtoRGB( AP0, 1.0);

const float AP1_2_XYZ_MAT[4][4] = RGBtoXYZ( AP1, 1.0);
const float XYZ_2_AP1_MAT[4][4] = XYZtoRGB( AP1, 1.0);

const float AP0_2_AP1_MAT[4][4] = mult_f44_f44( AP0_2_XYZ_MAT, XYZ_2_AP1_MAT);
const float AP1_2_AP0_MAT[4][4] = mult_f44_f44( AP1_2_XYZ_MAT, XYZ_2_AP0_MAT);