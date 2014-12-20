
// <ACEStransformID>ODT.Academy.RGBmonitor_D60sim_100nits_dim.a1.0.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Output - sRGB (D60 sim.)</ACESuserName>

// 
// Output Device Transform - RGB computer monitor (D60 simulation)
//

//
// Summary :
//  This transform is intended for mapping OCES onto a desktop computer monitor 
//  typical of those used in motion picture visual effects production used to 
//  simulate the image appearance produced by odt_p3d60. These monitors may
//  occasionally be referred to as "sRGB" displays, however, the monitor for
//  which this transform is designed does not exactly match the specifications
//  in IEC 61966-2-1:1999.
// 
//  The assumed observer adapted white is D60, and the viewing environment is 
//  that of a dim surround. 
//
//  The monitor specified is intended to be more typical of those found in 
//  visual effects production.
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
//  IEC 61966-2-1:1999.
//
// Signal Range:
//    This tranform outputs full range code values.
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.32168      0.33767
//
// Viewing Environment:
//   This ODT has a compensation for viewing environment variables more typical 
//   of those associated with video mastering.
//



import "ACESlib.Utilities.a1.0.0";
import "ACESlib.Transform_Common.a1.0.0";
import "ACESlib.ODT_Common.a1.0.0";
import "ACESlib.Tonescales.a1.0.0";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = REC709_PRI;
const float XYZ_2_DISPLAY_PRI_MAT[4][4] = XYZtoRGB(DISPLAY_PRI,1.0);

const float DISPGAMMA = 2.4; 
const float OFFSET = 0.055;

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
  // ODTs to simulate D60 within the D65 calibration white point. 

    // Scale and clamp white to avoid casted highlights due to D60 simulation
    linearCV[0] = min( linearCV[0], 1.0) * SCALE;
    linearCV[1] = min( linearCV[1], 1.0) * SCALE;
    linearCV[2] = min( linearCV[2], 1.0) * SCALE;

  // Apply gamma adjustment to compensate for dim surround
    linearCV = darkSurround_to_dimSurround( linearCV);

  // Apply desaturation to compensate for luminance difference
    linearCV = mult_f3_f33( linearCV, ODT_SAT_MAT);

  // Convert to display primary encoding
    // Rendering space RGB to XYZ
    float XYZ[3] = mult_f3_f44( linearCV, AP1_2_XYZ_MAT);

    // CIE XYZ to display primaries
    linearCV = mult_f3_f44( XYZ, XYZ_2_DISPLAY_PRI_MAT);

  // Handle out-of-gamut values
    // Clip values < 0 or > 1 (i.e. projecting outside the display primaries)
    linearCV = clamp_f3( linearCV, 0., 1.);
  
  // Encode linear code values with transfer function
    float outputCV[3];
    // moncurve_r with gamma of 2.4 and offset of 0.055 matches the EOTF found in IEC 61966-2-1:1999 (sRGB)
    outputCV[0] = moncurve_r( linearCV[0], DISPGAMMA, OFFSET);
    outputCV[1] = moncurve_r( linearCV[1], DISPGAMMA, OFFSET);
    outputCV[2] = moncurve_r( linearCV[2], DISPGAMMA, OFFSET);
  
    rOut = outputCV[0];
    gOut = outputCV[1];
    bOut = outputCV[2];
    aOut = aIn;
}