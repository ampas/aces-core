// 
// Inverse DCDM ODT (X'Y'Z' to OCES)
// v0.7.1
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* ----- ODT Parameters ------ */
const float XYZ_2_OCES_PRI_MAT[4][4] = XYZtoRGB(ACES_PRI,1.0);

const float DISPGAMMA = 2.6; 



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

  /* --- Decode with inverse transfer function --- */
    float XYZ[3];
    XYZ[0] = 52.37/48. * pow( outputCV[0], DISPGAMMA);
    XYZ[1] = 52.37/48. * pow( outputCV[1], DISPGAMMA);
    XYZ[2] = 52.37/48. * pow( outputCV[2], DISPGAMMA);

  /* --- Convert CIE XYZ to OCES RGB encoding --- */
  float linearCV[3] = mult_f3_f44( XYZ, XYZ_2_OCES_PRI_MAT);

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