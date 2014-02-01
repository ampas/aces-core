// 
// Inverse Rec709 Output Device Transform
// v0.7
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* ----- ODT Parameters ------ */
const Chromaticities DISPLAY_PRI = REC709_PRI;
const float XYZ_2_OCES_PRI_MAT[4][4] = XYZtoRGB(ACES_PRI,1.0);
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(DISPLAY_PRI,1.0);

const float DISPGAMMA = 2.4; 
const float L_W = 1.0;
const float L_B = 0.0;



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
  /* --- Initialize a 3-element vector with input variables (0-1 CV) --- */
    float outputCV[3] = { rIn, gIn, bIn};

  /* --- Decode to linear code values with inverse transfer function --- */
    float linearCV[3];
    linearCV[0] = bt1886_f( outputCV[0], DISPGAMMA, L_W, L_B);
    linearCV[1] = bt1886_f( outputCV[1], DISPGAMMA, L_W, L_B);
    linearCV[2] = bt1886_f( outputCV[2], DISPGAMMA, L_W, L_B);

  /* --- Convert from display primary encoding --- */
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, DISPLAY_PRI_2_XYZ_MAT);

      // Apply CAT from assumed observer adapted white to ACES white point
      XYZ = mult_f3_f33( XYZ, invert_f33( D60_2_D65_CAT));
  
    // CIE XYZ to OCES RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_OCES_PRI_MAT);
  
  /* --- Apply inverse black point compensation --- */  
    float rgbPre[3] = bpc_cinema_inv( linearCV);
  
  /* --- Apply inverse hue-preserving tone scale w/ sat preservation --- */
    float oces[3] = odt_tonescale_inv_f3( rgbPre);
  
  /* --- Cast OCES to rOut, gOut, bOut --- */  
    rOut = oces[0];
    gOut = oces[1];
    bOut = oces[2];
    aOut = aIn;
}