// 
// Output Device Transform - CIE XYZ (48 cd/m^2)
//

//
// Summary :
//  This transform is intended for mapping OCES to display colorimetry -
//  specified in CIE XYZ tristimulus values. The assumed observer adapted white 
//  is D60, and the viewing environment is that of a dark theater. 
//
//  The XYZ values represent the target display colorimetry for a display with a 
//  dynamic range of 2400:1 and are scaled 0-1. The XYZ values must be converted 
//  to display code values with another transform.
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

  // Rendering space RGB to XYZ
    float XYZ[3] = mult_f3_f44( linearCV, AP1_2_XYZ_MAT);

    rOut = XYZ[0];
    gOut = XYZ[1];
    bOut = XYZ[2];
    aOut = aIn;
}