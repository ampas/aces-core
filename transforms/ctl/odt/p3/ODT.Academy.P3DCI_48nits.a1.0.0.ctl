
// <ACEStransformID>ODT.Academy.P3DCI_48nits.a1.0.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Output - P3-DCI</ACESuserName>

// 
// Output Device Transform - P3DCI (D60 Simulation)
//

//
// Summary :
//  This transform is intended for mapping OCES onto a P3 digital cinema 
//  projector that is calibrated to a DCI white point at 48 cd/m^2. The assumed 
//  observer adapted white is D60, and the viewing environment is that of a dark
//  theater. 
//
// Device Primaries : 
//  CIE 1931 chromaticities:  x         y         Y
//              Red:          0.68      0.32
//              Green:        0.265     0.69
//              Blue:         0.15      0.06
//              White:        0.314     0.351     48 cd/m^2
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



import "ACESlib.Utilities.a1.0.0";
import "ACESlib.Transform_Common.a1.0.0";
import "ACESlib.ODT_Common.a1.0.0";
import "ACESlib.Tonescales.a1.0.0";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3DCI_PRI;
const float XYZ_2_DISPLAY_PRI_MAT[4][4] = XYZtoRGB( DISPLAY_PRI, 1.0);

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
    float oces[3] = { rIn, gIn, bIn};

  // OCES to RGB rendering space
    float rgbPre[3] = mult_f3_f44( oces, AP0_2_AP1_MAT);

  // Apply the tonescale independently in rendering-space RGB
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c9_fwd( rgbPre[0]);
    rgbPost[1] = segmented_spline_c9_fwd( rgbPre[1]);
    rgbPost[2] = segmented_spline_c9_fwd( rgbPre[2]);

  // Scale luminance to linear code value
    float linearCV[3];
    linearCV[0] = Y_2_linCV( rgbPost[0], CINEMA_WHITE, CINEMA_BLACK);
    linearCV[1] = Y_2_linCV( rgbPost[1], CINEMA_WHITE, CINEMA_BLACK);
    linearCV[2] = Y_2_linCV( rgbPost[2], CINEMA_WHITE, CINEMA_BLACK);

  // --- Compensate for different white point being darker  --- //
  // This adjustment is to correct an issue that exists in ODTs where the device 
  // is calibrated to a white chromaticity other than D60. In order to simulate 
  // D60 on such devices, unequal code values are sent to the display to achieve 
  // neutrals at D60. In order to produce D60 on a device calibrated to the DCI 
  // white point (i.e. equal code values yield CIE x,y chromaticities of 0.314, 
  // 0.351) the red channel is higher than green and blue to compensate for the 
  // "greenish" DCI white. This is the correct behavior but it means that as 
  // highlight increase, the red channel will hit the device maximum first and 
  // clip, resulting in a chromaticity shift as the green and blue channels 
  // continue to increase.
  // To avoid this clipping error, a slight scale factor is applied to allow the 
  // ODTs to simulate D60 within the D65 calibration white point. However, the 
  // magnitude of the scale factor required for the P3DCI ODT was considered too 
  // large. Therefore, the scale factor was reduced and the additional required 
  // compression was achieved via a reshaping of the highlight rolloff in 
  // conjunction with the scale. The shape of this rolloff was determined 
  // throught subjective experiments and deemed to best reproduce the 
  // "character" of the highlights in the P3D60 ODT.

    // Roll off highlights to avoid need for as much scaling
    linearCV[0] = roll_white_fwd( linearCV[0], NEW_WHT, ROLL_WIDTH);
    linearCV[1] = roll_white_fwd( linearCV[1], NEW_WHT, ROLL_WIDTH);
    linearCV[2] = roll_white_fwd( linearCV[2], NEW_WHT, ROLL_WIDTH);

    // Scale and clamp white to avoid casted highlights due to D60 simulation
    linearCV[0] = min( linearCV[0], NEW_WHT) * SCALE;
    linearCV[1] = min( linearCV[1], NEW_WHT) * SCALE;
    linearCV[2] = min( linearCV[2], NEW_WHT) * SCALE;

  // Convert to display primary encoding
    // Rendering space RGB to XYZ
    float XYZ[3] = mult_f3_f44( linearCV, AP1_2_XYZ_MAT);

    // CIE XYZ to display primaries
    linearCV = mult_f3_f44( XYZ, XYZ_2_DISPLAY_PRI_MAT);

  // Handle out-of-gamut values
    // Clip values < 0 or > 1 (i.e. projecting outside the display primaries)
    linearCV = clamp_f3( linearCV, 0., 1.);
  
  // Encode linear code values with transfer function
    float outputCV[3] = pow_f3( linearCV, 1./ DISPGAMMA);
  
    rOut = outputCV[0];
    gOut = outputCV[1];
    bOut = outputCV[2];
    aOut = aIn;
}