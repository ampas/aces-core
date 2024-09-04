
// <ACEStransformID>urn:ampas:aces:transformId:v2.0:Lib.Academy.ColorSpaces.a2.v1</ACEStransformID>
// <ACESuserName>Color Space Conversion</ACESuserName>

//
// Functions for assorted color space operations
//



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