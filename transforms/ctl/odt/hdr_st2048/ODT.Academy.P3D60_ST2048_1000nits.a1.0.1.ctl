
// <ACEStransformID>ODT.Academy.P3D60_ST2048_1000nits.a1.0.1</ACEStransformID>
// <ACESuserName>ACES 1.0 Output - P3-D60 ST2048 (1000 nits)</ACESuserName>

// 
// Output Device Transform - P3D60 (1000 cd/m^2)
//

//
// Summary :
//  This transform is intended for mapping OCES onto an HDR display calibrated 
//  to a D60 white point at 1000 cd/m^2. The assumed observer adapted white is 
//  D60, and the viewing environment is that of a dim surround. 
//
// Device Primaries : 
//  CIE 1931 chromaticities:  x         y         Y
//              Red:          0.68      0.32
//              Green:        0.265     0.69
//              Blue:         0.15      0.06
//              White:        0.32168   0.33767   1000 cd/m^2
//
// Display EOTF :
//  The reference electro-optical transfer function specified in SMPTE ST 
//  2084-2014. This transform makes no attempt to address the Annex functions 
//  which address integer quantization.
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.32168      0.33767
//
// Viewing Environment:
//  This ODT is designed for a viewing environment more typically associated 
//  with video mastering.
//



import "ACESlib.Utilities.a1.0.1";
import "ACESlib.Transform_Common.a1.0.1";
import "ACESlib.ODT_Common.a1.0.1";
import "ACESlib.Tonescales.a1.0.1";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3D60_PRI;
const float XYZ_2_DISPLAY_PRI_MAT[4][4] = XYZtoRGB( DISPLAY_PRI, 1.0);



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
    rgbPost[0] = segmented_spline_c9_fwd( rgbPre[0], ODT_1000nits);
    rgbPost[1] = segmented_spline_c9_fwd( rgbPre[1], ODT_1000nits);
    rgbPost[2] = segmented_spline_c9_fwd( rgbPre[2], ODT_1000nits);

  // Subtract small offset to allow for a code value of 0
    rgbPost = add_f_f3( -pow10(-4.4550166483), rgbPost);

  // Convert to display primary encoding
    // Rendering space RGB to XYZ
    float XYZ[3] = mult_f3_f44( rgbPost, AP1_2_XYZ_MAT);

    // CIE XYZ to display primaries
    float rgb[3] = mult_f3_f44( XYZ, XYZ_2_DISPLAY_PRI_MAT);

  // Handle out-of-gamut values
    // Clip values < 0 (i.e. projecting outside the display primaries)
    rgb = clamp_f3( rgb, 0., HALF_POS_INF);

  // Encode with ST2048 transfer function
    float outputCV[3] = Y_2_ST2048_f3( rgb);

    rOut = outputCV[0];
    gOut = outputCV[1];
    bOut = outputCV[2];
    aOut = aIn;
}