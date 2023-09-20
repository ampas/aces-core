
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:InvRRTODT.Academy.P3D65_1000nits_15nits_DisplayP3.a1.1.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Inverse Output - DisplayP3 (1000 nits)</ACESuserName>


import "ACESlib.Utilities";
import "ACESlib.OutputTransforms";
import "ACESlib.Tonescales";



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
    float cv[3] = {rIn, gIn, bIn};

    // Decode to linear code values with inverse transfer function
    float linearCV[3];
    // moncurve_f with gamma of 2.4 and offset of 0.055 matches the EOTF found in IEC 61966-2-1:1999 (sRGB)
    linearCV[0] = moncurve_f( cv[0], DISPGAMMA, OFFSET);
    linearCV[1] = moncurve_f( cv[1], DISPGAMMA, OFFSET);
    linearCV[2] = moncurve_f( cv[2], DISPGAMMA, OFFSET);

    // Scale to absolute linear values in cd/m^2
    linearCV[0] = linearCV[0] * 100.0;
    linearCV[1] = linearCV[1] * 100.0;
    linearCV[2] = linearCV[2] * 100.0;

    float aces[3] = invOutputTransform( linearCV, Y_MIN,
                                                  Y_MID,
                                                  Y_MAX,
                                                  DISPLAY_PRI,
                                                  LIMITING_PRI,
                                                  EOTF,
                                                  SURROUND,
                                                  STRETCH_BLACK,
                                                  D60_SIM,
                                                  LEGAL_RANGE );

    rOut = aces[0];
    gOut = aces[1];
    bOut = aces[2];
    aOut = aIn;
}