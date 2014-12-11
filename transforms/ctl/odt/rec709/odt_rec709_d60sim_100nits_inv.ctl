// 
// Inverse Output Device Transform - Rec709 (D60 simulation)
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";
import "tonescales";



/* ----- ODT Parameters ------ */
const Chromaticities DISPLAY_PRI = REC709_PRI;
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
    output varying float aOut,
    input varying int fullrange = 0
)
{  
    float outputCV[3] = { rIn, gIn, bIn};

  // Default output is to SMPTE range
    if (fullRange == 0) {
      outputCV = smpteRange_to_fullRange( outputCV);
    }

  // Decode to linear code values with inverse transfer function
    float linearCV[3];
    linearCV[0] = bt1886_f( outputCV[0], DISPGAMMA, L_W, L_B);
    linearCV[1] = bt1886_f( outputCV[1], DISPGAMMA, L_W, L_B);
    linearCV[2] = bt1886_f( outputCV[2], DISPGAMMA, L_W, L_B);

  // Convert from display primary encoding
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, DISPLAY_PRI_2_XYZ_MAT);
  
    // CIE XYZ to rendering space RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_RENDER_PRI_MAT);

  // Undo scaling done for D60 simulation
    linearCV[0] = linearCV[0] / SCALE;
    linearCV[1] = linearCV[1] / SCALE;
    linearCV[2] = linearCV[2] / SCALE;
  
  // Scale linear code value to luminance
    float rgbPre[3];
    rgbPre[0] = linCV_2_Y( linearCV[0], CINEMA_WHITE, CINEMA_BLACK);
    rgbPre[1] = linCV_2_Y( linearCV[1], CINEMA_WHITE, CINEMA_BLACK);
    rgbPre[2] = linCV_2_Y( linearCV[2], CINEMA_WHITE, CINEMA_BLACK);

  // Apply the tonescale independently in rendering-space RGB
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c9_rev( rgbPre[0]);
    rgbPost[1] = segmented_spline_c9_rev( rgbPre[1]);
    rgbPost[2] = segmented_spline_c9_rev( rgbPre[2]);

  // Rendering space RGB to OCES
    float oces[3] = mult_f3_f44( rgbPost, RENDER_PRI_2_ACES_MAT);

    rOut = oces[0];
    gOut = oces[1];
    bOut = oces[2];
    aOut = aIn;
}