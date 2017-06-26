
// <ACEStransformID>InvODT.Academy.Rec2020_ST2084_4000nits.a1.0.3</ACEStransformID>
// <ACESuserName>ACES 1.0 Inverse Output - Rec.2020 ST2084 (4000 nits)</ACESuserName>

// 
// Inverse Output Device Transform - Rec.2020 (4000 cd/m^2)
//



import "ACESlib.Utilities";
import "ACESlib.Transform_Common";
import "ACESlib.ODT_Common";
import "ACESlib.Tonescales";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = REC2020_PRI;
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ( DISPLAY_PRI, 1.0);



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
    float outputCV[3] = { rIn, gIn, bIn};

  // Decode with inverse ST2084 transfer function
    float rgb[3] = ST2084_2_Y_f3( outputCV);

  // Convert from display primary encoding
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( rgb, DISPLAY_PRI_2_XYZ_MAT);

      // Apply CAT from assumed observer adapted white to ACES white point
      XYZ = mult_f3_f33( XYZ, invert_f33( D60_2_D65_CAT));

    // CIE XYZ to rendering space RGB
    float rgbPre[3] = mult_f3_f44( XYZ, XYZ_2_AP1_MAT);

  // Add small offset that was used to allow for a code value of 0
    rgbPre = add_f_f3( pow10(-4.4550166483), rgbPre);

  // Apply the tonescale independently in rendering-space RGB
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c9_rev( rgbPre[0], ODT_4000nits);
    rgbPost[1] = segmented_spline_c9_rev( rgbPre[1], ODT_4000nits);
    rgbPost[2] = segmented_spline_c9_rev( rgbPre[2], ODT_4000nits);

  // Rendering space RGB to OCES
    float oces[3] = mult_f3_f44( rgbPost, AP1_2_AP0_MAT);

    rOut = oces[0];
    gOut = oces[1];
    bOut = oces[2];
    aOut = aIn;
}
