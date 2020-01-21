
// <ACEStransformID>InvODT.Academy.P3DCI_D65sim_48nits.a1.1</ACEStransformID>
// <ACESuserName>ACES 1.0 Inverse Output - P3-DCI (D65 simulation)</ACESuserName>

// 
// Inverse Output Device Transform - P3DCI (D65 Simulation)
//



import "ACESlib.Utilities";
import "ACESlib.Transform_Common";
import "ACESlib.ODT_Common";
import "ACESlib.Tonescales";



/* --- ODT Parameters --- */
const Chromaticities DISPLAY_PRI = P3DCI_PRI;
const float DISPLAY_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(DISPLAY_PRI,1.0);

const float DISPGAMMA = 2.6; 

// Rolloff white settings for P3DCI (D65 simulation)
const float NEW_WHT = 0.908;
const float ROLL_WIDTH = 0.5;    
const float SCALE = 0.9575;



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

    // Apply CAT from assumed observer adapted white to ACES white point
    XYZ = mult_f3_f33( XYZ, invert_f33( D60_2_D65_CAT));

    // CIE XYZ to rendering space RGB
    linearCV = mult_f3_f44( XYZ, XYZ_2_AP1_MAT);

    // Undo highlight roll-off and scaling
    linearCV[0] = roll_white_rev( linearCV[0] / SCALE, NEW_WHT, ROLL_WIDTH);
    linearCV[1] = roll_white_rev( linearCV[1] / SCALE, NEW_WHT, ROLL_WIDTH);
    linearCV[2] = roll_white_rev( linearCV[2] / SCALE, NEW_WHT, ROLL_WIDTH);

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