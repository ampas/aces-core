// 
// Output Device Transform to P3D60
// v0.7
//

//
// Summary :
//  This transform is intended for mapping OCES onto a P3 digital cinema 
//  projector that is calibrated to a D60 white point at 48 cd/m^2. The assumed 
//  observer adapted white is D60, and the viewing environment is that of a dark //  theater. 
//
// Device Primaries : 
//  CIE 1931 chromaticities:  x         y         Y
//              Red:          0.68      0.32
//              Green:        0.265     0.69
//              Blue:         0.15      0.06
//              White:        0.32168   0.33767   48 cd/m^2
//
// Display EOTF :
//  Gamma: 2.6
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.32168      0.33767
//
// Viewing Environment:
//  Environment specified in SMPTE RP 431-2-2007
//



import "utilities";
import "transforms-common";
import "odt-transforms-common";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3D60_PRI;
const float OCES_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(ACES_PRI,1.0);
const float XYZ_2_DISPLAY_PRI_MAT[4][4] = XYZtoRGB(DISPLAY_PRI,1.0);

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
  /* --- Initialize a 3-element vector with input variables (OCES) --- */
    float oces[3] = { rIn, gIn, bIn};

  /* --- Apply hue-preserving tone scale with saturation preservation --- */
    float rgbPost[3] = odt_tonescale_fwd_f3( oces);

  /* --- Apply black point compensation --- */  
    float linearCV[3] = bpc_cinema_fwd( rgbPost);

  /* --- Convert to display primary encoding --- */
    // OCES RGB to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, OCES_PRI_2_XYZ_MAT);

    // CIE XYZ to display primaries
    linearCV = mult_f3_f44( XYZ, XYZ_2_DISPLAY_PRI_MAT);

  /* --- Handle out-of-gamut values --- */
    // Clip values < 0 or > 1 (i.e. projecting outside the display primaries)
    float linearCVClamp[3] = clamp_f3( linearCV, 0., 1.);
  
    // Restore hue after clip operation ("smart-clip")
    linearCV = restore_hue_dw3( linearCV, linearCVClamp);

  /* --- Encode linear code values with transfer function --- */
    float outputCV[3];
    outputCV[0] = pow( linearCV[0], 1./DISPGAMMA);
    outputCV[1] = pow( linearCV[1], 1./DISPGAMMA);
    outputCV[2] = pow( linearCV[2], 1./DISPGAMMA);
  
  /* --- Cast outputCV to rOut, gOut, bOut --- */
    rOut = outputCV[0];
    gOut = outputCV[1];
    bOut = outputCV[2];
    aOut = aIn;
}