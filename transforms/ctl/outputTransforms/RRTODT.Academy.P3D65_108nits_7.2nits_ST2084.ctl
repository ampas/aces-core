
// <ACEStransformID>ODT.Academy.P3D65_108nits_7.2nits_ST2084.a1.1</ACEStransformID>
// <ACESuserName>ACES 1.0 Output - P3D65 ST2084 (108 nits)</ACESuserName>

// 
// Output Transform - P3D65 (108 cd/m^2)
//

//
// Summary :
//  This transform maps ACES onto a P3D65 ST.2084 HDR display calibrated 
//  to a D65 white point at 108 cd/m^2. The assumed observer adapted white is 
//  D65, and the viewing environment is that of a dark surround. Mid-gray
//  luminance is targeted at 7.2 cd/m^2.
//
//  A use case for this transform would be mastering for a theatrical release
//  in Dolby Cinema.
//
// Device Primaries : 
//  Primaries are those specified in Rec. ITU-R BT.2020
//  CIE 1931 chromaticities:  x         y         Y
//              Red:          0.68      0.32
//              Green:        0.265     0.69
//              Blue:         0.15      0.06
//              White:        0.3127    0.329     108 cd/m^2
//              18% Gray:     0.3127    0.329     7.2 cd/m^2
//
// Display EOTF :
//  The reference electro-optical transfer function specified in SMPTE ST 
//  2084-2014. This transform makes no attempt to address the Annex functions 
//  which address integer quantization.
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



const float Y_MIN = 0.0001;                     // black luminance (cd/m^2)
const float Y_MID = 7.2;                        // mid-point luminance (cd/m^2)
const float Y_MAX = 108.0;                      // peak white luminance (cd/m^2)

const Chromaticities DISPLAY_PRI = P3D65_PRI;   // encoding primaries (device setup)
const Chromaticities LIMITING_PRI = P3D65_PRI;  // limiting primaries

const int EOTF = 0;                             // 0: ST-2084 (PQ)
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

    float cv[3] = outputTransform( aces, Y_MIN,
                                         Y_MID,
                                         Y_MAX,
                                         DISPLAY_PRI,
                                         LIMITING_PRI,
                                         EOTF,
                                         SURROUND,
                                         STRETCH_BLACK,
                                         D60_SIM,
                                         LEGAL_RANGE );

    rOut = cv[0];
    gOut = cv[1];
    bOut = cv[2];
    aOut = aIn;
}