
// <ACEStransformID>InvODT.Academy.P3D60_PQ_1000nits.a1.0.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Inverse Output - P3-D60 PQ (1000 nits)</ACESuserName>

// 
// Inverse Output Device Transform - P3D60 (1000 cd/m^2)
//



import "ACESlib.Utilities.a1.0.0";
import "ACESlib.Transform_Common.a1.0.0";
import "ACESlib.ODT_Common.a1.0.0";
import "ACESlib.Tonescales.a1.0.0";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3D60_PRI;
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

  // Decode with inverse PQ transfer function
    float rgb[3] = pq_f_f3( outputCV);

  // Convert from display primary encoding
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( rgb, DISPLAY_PRI_2_XYZ_MAT);
  
    // CIE XYZ to rendering space RGB
    float rgbPre[3] = mult_f3_f44( XYZ, XYZ_2_AP1_MAT);

  // Apply the tonescale independently in rendering-space RGB
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c9_rev( rgbPre[0], ODT_1000nits);
    rgbPost[1] = segmented_spline_c9_rev( rgbPre[1], ODT_1000nits);
    rgbPost[2] = segmented_spline_c9_rev( rgbPre[2], ODT_1000nits);

  // Rendering space RGB to OCES
    float oces[3] = mult_f3_f44( rgbPost, AP1_2_AP0_MAT);

    rOut = oces[0];
    gOut = oces[1];
    bOut = oces[2];
    aOut = aIn;
}