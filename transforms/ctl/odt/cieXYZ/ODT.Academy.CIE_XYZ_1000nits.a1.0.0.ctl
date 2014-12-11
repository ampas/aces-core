// 
// Output Device Transform - CIE XYZ (1000 cd/m^2)
//

//
// Summary :
//  This transform is intended for mapping OCES to display colorimetry -
//  specified in CIE XYZ tristimulus values. The assumed observer adapted white 
//  is D60, and the viewing environment is a dim surround.
//
//  The XYZ values represent the target display colorimetry for a display with a 
//  dynamic range of 200000:1 and are scaled 0-1. The XYZ values must be 
//  converted to display code values with another transform.
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.32168      0.33767
//
// Viewing Environment:
//   This ODT has a compensation for viewing environment variables more typical 
//   of those associated with video mastering.
//


import "utilities";
import "transforms-common";
import "odt-transforms-common";
import "tonescales";



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
    float rgbPre[3] = mult_f3_f44( oces, ACES_2_RENDER_PRI_MAT);

  // Apply the tonescale independently in rendering-space RGB
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c9_fwd( rgbPre[0], ODT_1000nits);
    rgbPost[1] = segmented_spline_c9_fwd( rgbPre[1], ODT_1000nits);
    rgbPost[2] = segmented_spline_c9_fwd( rgbPre[2], ODT_1000nits);

  // Scale luminance to linear code value
    float linearCV[3];
    linearCV[0] = Y_2_linCV( rgbPost[0], CINEMA_WHITE, CINEMA_BLACK);
    linearCV[1] = Y_2_linCV( rgbPost[1], CINEMA_WHITE, CINEMA_BLACK);
    linearCV[2] = Y_2_linCV( rgbPost[2], CINEMA_WHITE, CINEMA_BLACK);

  // Rendering space RGB to XYZ
    float XYZ[3] = mult_f3_f44( linearCV, RENDER_PRI_2_XYZ_MAT);

    rOut = XYZ[0];
    gOut = XYZ[1];
    bOut = XYZ[2];
    aOut = aIn;
}