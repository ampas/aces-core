// 
// Inverse Rec2020 Output Device Transform (D60 simulation)
// v0.7.1
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* ----- ODT Parameters ------ */
const Chromaticities DISPLAY_PRI = REC2020_PRI;
const float XYZ_2_OCES_PRI_MAT[4][4] = XYZtoRGB(ACES_PRI,1.0);
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(DISPLAY_PRI,1.0);

const float DISPGAMMA = 2.4; 
const float L_W = 1.0;
const float L_B = 0.0;

const float SCALE = 0.955;



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

  /* --- SMPTE range to full range--- */
    outputCV = smpteRange_to_fullRange( outputCV);

  /* --- Decode to linear code values with inverse transfer function --- */
    float linearCV[3];
    linearCV[0] = bt1886_f( outputCV[0], DISPGAMMA, L_W, L_B);
    linearCV[1] = bt1886_f( outputCV[1], DISPGAMMA, L_W, L_B);
    linearCV[2] = bt1886_f( outputCV[2], DISPGAMMA, L_W, L_B);

  /* --- Convert from display primary encoding --- */
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, DISPLAY_PRI_2_XYZ_MAT);

    // CIE XYZ to OCES RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_OCES_PRI_MAT);

  /* --- Remove scaling done for D60 simulation --- */
    linearCV[0] = linearCV[0] / SCALE;
    linearCV[1] = linearCV[1] / SCALE;
    linearCV[2] = linearCV[2] / SCALE;
    
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