
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:InvODT.Academy.P3D65_D60sim_48nits.a1.1.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Inverse Output - P3-D65 (D60 simulation)</ACESuserName>

// 
// Inverse Output Device Transform - P3D65 (D60 simulation)
//



import "ACESlib.Utilities";
import "ACESlib.Transform_Common";
import "ACESlib.ODT_Common";
import "ACESlib.Tonescales";




/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3D65_PRI;
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ( DISPLAY_PRI, 1.0);

const float DISPGAMMA = 2.6; 

// Rolloff white settings for P3D65 (D60 simulation)
const float SCALE = 0.964;


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

    // Decode to linear code values with inverse transfer function
    float linearCV[3] = pow_f3( outputCV, DISPGAMMA);

    // Convert from display primary encoding
    // Display primaries to CIE XYZ
    float XYZ[3] = mult_f3_f44( linearCV, DISPLAY_PRI_2_XYZ_MAT);

    // CIE XYZ to rendering space RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_AP1_MAT);

    // Undo scaling done for D60 simulation
    linearCV[0] = linearCV[0] / SCALE;
    linearCV[1] = linearCV[1] / SCALE;
    linearCV[2] = linearCV[2] / SCALE;

    // Scale linear code value to luminance
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