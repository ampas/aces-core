
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:RRTODT.Academy.P3D65_1000nits_15nits_DisplayP3.a1.1.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Output - DisplayP3 (1000 nits)</ACESuserName>

// 
// Output Device Transform - DisplayP3 (1000 cd/m^2)
//

//
// Summary :
//  This transform is intended for mapping ACES onto a display calibrated using
//  the DCI P3 primaries, a D65 white point, and the sRGB transfer function as
//  given by https://developer.apple.com/documentation/coregraphics/cgcolorspace/3684560-extendeddisplayp3
//
//  There is no display standard associated with the color space thus, for
//  consistency, 100 cd/m^2 is recommended as the display normalization
//  luminance. The assumed observer adapted white is D65, and the viewing
//  environment is that of a dark surround. Mid-gray luminance is targeted at
//  15 cd/m^2.
//
// NOTE :
//  The parameterized "Output Transform" function used as basis for this 
//  transform includes a parameter labeled "SURROUND". However, as noted in 
//  "ACESlib.OutputTransforms.ctl", this sub-module is currently a "placeholder"
//  and as such currently only supports a rendering intended for dark surround 
//  environments. While it is planned for future releases to fully activate the 
//  this module and provide adjustment for dark/dim/normal surrounds, setting 
//  SURROUND equal to either dim or normal will have no effect at this time.
//  
//  That being said, these Output Transforms have been used successfully on
//  major feature and television projects when mastering for home HDR where a 
//  dim surround environment is assumed. Based on that, these transforms are 
//  considered adequate for use for dim surround mastering, although, as always,
//  creative adjustments to contrast and saturation may be desirable and should
//  be saved as a "trim pass."
//
// Device Primaries : 
//  Primaries are those specified in Rec. ITU-R BT.2020
//  CIE 1931 chromaticities:  x         y         Y
//              Red:          0.68      0.32
//              Green:        0.265     0.69
//              Blue:         0.15      0.06
//              White:        0.3127    0.329     1000 cd/m^2
//              18% Gray:     0.3127    0.329     15 cd/m^2
//
// Display EOTF :
//  The sRGB piece-wise transfer function specified in IEC 61966-2-1:1999 (sRGB)
//  but unclamped, such that values >1 represent luminance above 100 cd/m^2.
//
// Assumed observer adapted white point:
//         CIE 1931 chromaticities:    x            y
//                                     0.3127       0.329
//
// Viewing Environment:
//  Environment specified in SMPTE RP 431-2-2007
//



import "ACESlib.Utilities";
import "ACESlib.OutputTransforms";
import "ACESlib.Tonescales";



// NOTE: The EOTF is *NOT* gamma 2.4, it follows IEC 61966-2-1:1999
const float DISPGAMMA = 2.4; 
const float OFFSET = 0.055;


const float Y_MIN = 0.0001;                     // black luminance (cd/m^2)
const float Y_MID = 15.0;                       // mid-point luminance (cd/m^2)
const float Y_MAX = 1000.0;                     // peak white luminance (cd/m^2)

const Chromaticities DISPLAY_PRI = P3D65_PRI;   // encoding primaries (device setup)
const Chromaticities LIMITING_PRI = P3D65_PRI;  // limiting primaries

const int EOTF = 4;                             // 0: ST-2084 (PQ)
                                                // 1: BT.1886 (Rec.709/2020 settings) 
                                                // 2: sRGB (mon_curve w/ presets)
                                                // 3: gamma 2.6
                                                // 4: linear (no EOTF)
                                                // 5: HLG

const int SURROUND = 0;                         // 0: dark ( NOTE: this is the only active setting! )
                                                // 1: dim ( *inactive* - selecting this will have no effect )
                                                // 2: normal ( *inactive* - selecting this will have no effect )

const bool STRETCH_BLACK = true;                // stretch black luminance to a PQ code value of 0
const bool D60_SIM = false;                       
const bool LEGAL_RANGE = false;


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
    float aces[3] = {rIn, gIn, bIn};

    float linearCV[3] = outputTransform( aces, Y_MIN,
                                               Y_MID,
                                               Y_MAX,
                                               DISPLAY_PRI,
                                               LIMITING_PRI,
                                               EOTF,
                                               SURROUND,
                                               STRETCH_BLACK,
                                               D60_SIM,
                                               LEGAL_RANGE );

    // Normalize linear values to 100 cd/m^2
    linearCV[0] = linearCV[0] / 100.0;
    linearCV[1] = linearCV[1] / 100.0;
    linearCV[2] = linearCV[2] / 100.0;

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