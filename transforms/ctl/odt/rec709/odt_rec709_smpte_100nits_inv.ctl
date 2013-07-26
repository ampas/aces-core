// 
// Inverse Rec709 Output Device Transform
// v0.2
//

import "utilities";
import "utilities-aces";

/* ----- ODT Parameters ------ */
const Chromaticities DISPLAY_PRI = REC709_PRI;
const float OCES_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(ACES_PRI,1.0);
const float XYZ_2_DISPLAY_PRI_MAT[4][4] = XYZtoRGB(DISPLAY_PRI,1.0);

const Chromaticities RENDERING_PRI = 
{
  {0.73470, 0.26530},
  {0.00000, 1.00000},
  {0.12676, 0.03521},
  {0.32168, 0.33767}
};
const float XYZ_2_RENDERING_PRI_MAT[4][4] = XYZtoRGB(RENDERING_PRI,1.0);
const float OCES_PRI_2_RENDERING_PRI_MAT[4][4] = mult_f44_f44( OCES_PRI_2_XYZ_MAT, XYZ_2_RENDERING_PRI_MAT);

// ODT parameters related to black point compensation (BPC) and encoding
const float ODT_OCES_BP = 0.0016;
const float ODT_OCES_WP = 48.0;
const float OUT_BP = 0.01;
const float OUT_WP = 100.0;

const float DISPGAMMA = 2.4; 
const unsigned int BITDEPTH = 10;
const unsigned int CV_BLACK = 64;
const unsigned int CV_WHITE = 940;

// Derived BPC and scale parameters
const float BPC = (ODT_OCES_BP * OUT_WP - ODT_OCES_WP * OUT_BP) / 
                  (ODT_OCES_BP - ODT_OCES_WP);
const float SCALE = (OUT_BP - OUT_WP) / (ODT_OCES_BP - ODT_OCES_WP);

/* --- Chromatic adaptation ---*/
const float CAT[3][3] = calculate_cat_matrix( ACES_PRI.white, DISPLAY_PRI.white);


void main 
(
  input varying float rIn, 
  input varying float gIn, 
  input varying float bIn, 
  input varying float aIn,
  output varying float rOut,
  output varying float gOut,
  output varying float bOut,
  output varying float aOut
)
{
  // Put input variables (display code values) into a 3-element vector
  float outputCV[3] = {rIn, gIn, bIn};

  // This step converts 0-1 code values back to integers
  outputCV = mult_f_f3( pow(2.,BITDEPTH)-1., outputCV);

  // Undo CCTF
  float offset_scaled[3];
  offset_scaled[0] = bt1886_f( (outputCV[0] - CV_BLACK) / (CV_WHITE - CV_BLACK), DISPGAMMA, OUT_WP, OUT_BP);
  offset_scaled[1] = bt1886_f( (outputCV[1] - CV_BLACK) / (CV_WHITE - CV_BLACK), DISPGAMMA, OUT_WP, OUT_BP);
  offset_scaled[2] = bt1886_f( (outputCV[2] - CV_BLACK) / (CV_WHITE - CV_BLACK), DISPGAMMA, OUT_WP, OUT_BP);

  // Undo black point compensation
  float rgbOut[3];
  rgbOut[0] = (offset_scaled[0] - BPC) / SCALE;
  rgbOut[1] = (offset_scaled[1] - BPC) / SCALE;
  rgbOut[2] = (offset_scaled[2] - BPC) / SCALE;
  
  rgbOut = clamp_f3( rgbOut, 0., HALF_POS_INF);    

  // Display RGB to CIE XYZ
  float XYZ[3] = mult_f3_f44( rgbOut, invert_f44(XYZ_2_DISPLAY_PRI_MAT));

    // Apply CAT from ACES white to Assumed Observer Adapted White
    XYZ = mult_f3_f33( XYZ, invert_f33(CAT));
  
  // CIE XYZ to OCES  
  float rgbPost[3] = mult_f3_f44( XYZ, invert_f44(OCES_PRI_2_XYZ_MAT));

  // OCES to rendering primaries
  float rgbPre[3] = mult_f3_f44( rgbPost, OCES_PRI_2_RENDERING_PRI_MAT);
  rgbPost[0] = odt_tonescale_rev( rgbPre[0]);
  rgbPost[1] = odt_tonescale_rev( rgbPre[1]);
  rgbPost[2] = odt_tonescale_rev( rgbPre[2]);    

  float rgbRestored[3] = restore_hue_dw3( rgbPre, rgbPost);

  // Rendering primaries to OCES
  float oces[3] = mult_f3_f44( rgbRestored, invert_f44(OCES_PRI_2_RENDERING_PRI_MAT));

  rOut = oces[0];
  gOut = oces[1];
  bOut = oces[2];
  aOut = aIn;
}