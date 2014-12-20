
// <ACEStransformID>InvODT.Academy.DCDM.a1.0.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Inverse Output - DCDM</ACESuserName>

// 
// Inverse Output Device Transform - DCDM (X'Y'Z')
//



import "ACESlib.Utilities.a1.0.0";
import "ACESlib.Transform_Common.a1.0.0";
import "ACESlib.ODT_Common.a1.0.0";
import "ACESlib.Tonescales.a1.0.0";



/* ----- ODT Parameters ------ */
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
    float outputCV[3] = { rIn, gIn, bIn};

  // Decode with inverse transfer function
    float XYZ[3] = dcdm_decode( outputCV);

  // CIE XYZ to rendering space RGB
    float linearCV[3] = mult_f3_f44( XYZ, XYZ_2_AP1_MAT);
  
  // Scale code value to luminance
    float rgbPre[3];
    rgbPre[0] = linCV_2_Y( linearCV[0], CINEMA_WHITE, CINEMA_BLACK);
    rgbPre[1] = linCV_2_Y( linearCV[1], CINEMA_WHITE, CINEMA_BLACK);
    rgbPre[2] = linCV_2_Y( linearCV[2], CINEMA_WHITE, CINEMA_BLACK);

  // Apply the tonescale independently in rendering-space RGB
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c9_rev( rgbPre[0]);
    rgbPost[1] = segmented_spline_c9_rev( rgbPre[1]);
    rgbPost[2] = segmented_spline_c9_rev( rgbPre[2]);

  // Rendering space RGB to OCES
    float oces[3] = mult_f3_f44( rgbPost, AP1_2_AP0_MAT);

    rOut = oces[0];
    gOut = oces[1];
    bOut = oces[2];
    aOut = aIn;
}