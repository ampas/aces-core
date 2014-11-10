// 
// Inverse P3D60 ODT
// WGR8
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";


float linCV_2_Y( float linCV, float Ymax, float Ymin) 
{
  return linCV * (Ymax - Ymin) + Ymin;
}


/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3D60_PRI;
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ( DISPLAY_PRI, 1.0);

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
  // --- Initialize a 3-element vector with input variables (0-1 CV) --- //
    float outputCV[3] = { rIn, gIn, bIn};

  // --- Decode to linear code values with inverse transfer function --- //
    float linearCV[3];
    linearCV[0] = pow( outputCV[0], DISPGAMMA);
    linearCV[1] = pow( outputCV[1], DISPGAMMA);
    linearCV[2] = pow( outputCV[2], DISPGAMMA);

print( "linearCV = \n");
print_f3( linearCV);
print( "\n");

  // --- Convert from display primary encoding --- //
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, DISPLAY_PRI_2_XYZ_MAT);
  
    // CIE XYZ to OCES RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_ACES_MAT);

print( "linearCV = \n");
print_f3( linearCV);
print( "\n");
  
    float rgbPre[3];
    rgbPre[0] = linCV_2_Y( linearCV[0], 48.0, 0.0048);
    rgbPre[1] = linCV_2_Y( linearCV[1], 48.0, 0.0048);
    rgbPre[2] = linCV_2_Y( linearCV[2], 48.0, 0.0048);

print( "rgbPre = \n");
print_f3( rgbPre);
print( "\n");
  
  // --- Apply the tonescale independently in rendering-space RGB --- //
    // OCES to RGB rendering space
    rgbPre = mult_f3_f44( rgbPre, ACES_2_RENDER_PRI_MAT);

print( "rgbPre = \n");
print_f3( rgbPre);
print( "\n");

    // Tonescale
    float rgbPost[3];
    rgbPost[0] = odt_tonescale_segmented_rev( rgbPre[0]);
    rgbPost[1] = odt_tonescale_segmented_rev( rgbPre[1]);
    rgbPost[2] = odt_tonescale_segmented_rev( rgbPre[2]);

print( "rgbPost = \n");
print_f3( rgbPost);
print( "\n");

// if (isinf_f( rgbPost[0])) rgbPost[0] = ODT_XMAX;

    // RGB rendering space back to OCES encoding
    float oces[3] = mult_f3_f44( rgbPost, RENDER_PRI_2_ACES_MAT);

print( "oces = \n");
print_f3( oces);
print( "\n");
    
  // --- Cast OCES to rOut, gOut, bOut --- //
    rOut = oces[0];
    gOut = oces[1];
    bOut = oces[2];
    aOut = aIn;
}