// 
// Output Device Transform to Rec709
// v0.7
//

//
// Summary :
//  This transform is intended for mapping OCES onto a Rec.709 broadcast monitor
//  that is calibrated to a D65 white point at 100 cd/m^2. The assumed observer 
//  adapted white is D65, and the viewing environment is that of a dark theater. 
//
// Device Primaries : 
//  Primaries are those specified in Rec. ITU-R BT.709
//  CIE 1931 chromaticities:  x         y         Y
//              Red:          0.64      0.33
//              Green:        0.3       0.6
//              Blue:         0.15      0.06
//              White:        0.3217    0.329     100 cd/m^2
//
// Display EOTF :
//  The reference electro-optical transfer function specified in 
//  Rec. ITU-R BT.1886.
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.3217       0.329
//
// Viewing Environment:
//  Environment specified in SMPTE RP 431-2-2007
//   Note: This environment is consistent with the viewing environment typical
//     of a motion picture theater. This ODT makes no attempt to compensate for 
//     viewing environment variables more typical of those associated with the 
//     home.
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = REC709_PRI;
const float OCES_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(ACES_PRI,1.0);
const float XYZ_2_DISPLAY_PRI_MAT[4][4] = XYZtoRGB(DISPLAY_PRI,1.0);

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
  /* --- Initialize a 3-element vector with input variables (OCES) --- */
    float oces[3] = { rIn, gIn, bIn};

  /* --- Apply hue-preserving tone scale with saturation preservation --- */
    float rgbPost[3] = odt_tonescale_fwd_f3( oces);

  /* --- Apply black point compensation --- */  
    float linearCV[3] = bpc_cinema_fwd( rgbPost);

  /* --- Convert to display primary encoding --- */
    // OCES RGB to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, OCES_PRI_2_XYZ_MAT);

  /* --- Handle out-of-gamut values --- */
    // Clip to P3 gamut using hue-preserving clip
    XYZ = huePreservingClip_to_p3d60( XYZ);

      // Apply CAT from ACES white point to assumed observer adapted white point
      XYZ = mult_f3_f33( XYZ, D60_2_D65_CAT);

    // CIE XYZ to display RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_DISPLAY_PRI_MAT);

    // Clip values < 0 or > 1 (i.e. projecting outside the display primaries)
    // Note: there is no hue restore step here.
    linearCV = clamp_f3( linearCV, 0., 1.);

  /* --- Encode linear code values with transfer function --- */
    float outputCV[3];
    outputCV[0] = bt1886_r( linearCV[0], DISPGAMMA, L_W, L_B);
    outputCV[1] = bt1886_r( linearCV[1], DISPGAMMA, L_W, L_B);
    outputCV[2] = bt1886_r( linearCV[2], DISPGAMMA, L_W, L_B);
  
  /* --- Cast outputCV to rOut, gOut, bOut --- */
    rOut = outputCV[0];
    gOut = outputCV[1];
    bOut = outputCV[2];
    aOut = aIn;
}