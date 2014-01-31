// 
// Inverse P3DCI D60 simulation Output Device Transform
// Working Group Release 6
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3DCI_PRI;
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(DISPLAY_PRI,1.0);
const float XYZ_2_OCES_PRI_MAT[4][4] = XYZtoRGB(ACES_PRI,1.0);

const float DISPGAMMA = 2.6; 

// Rolloff white settings for P3DCI
const float NEW_WHT = 0.918;
const float ROLL_WIDTH = 0.5;    
const float SCALE = 0.96;



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
    linearCV[0] = pow( outputCV[0], DISPGAMMA);
    linearCV[1] = pow( outputCV[1], DISPGAMMA);
    linearCV[2] = pow( outputCV[2], DISPGAMMA);

  /* --- Convert from display primary encoding --- */
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, DISPLAY_PRI_2_XYZ_MAT);
  
    // CIE XYZ to OCES RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_OCES_PRI_MAT);
  
  /* --- Compensate for different white point being darker  --- */    
    // Undo highlight roll-off and scaling
    linearCV[0] = roll_white_rev( linearCV[0] / SCALE, NEW_WHT, ROLL_WIDTH);
    linearCV[1] = roll_white_rev( linearCV[1] / SCALE, NEW_WHT, ROLL_WIDTH);
    linearCV[2] = roll_white_rev( linearCV[2] / SCALE, NEW_WHT, ROLL_WIDTH);

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