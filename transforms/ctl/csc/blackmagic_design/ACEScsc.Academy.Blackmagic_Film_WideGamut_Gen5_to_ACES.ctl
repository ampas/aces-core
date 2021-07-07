
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.BMDFilm_WideGamut_Gen5_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>Blackmagic Film Wide Gamut (Gen 5) to ACES2605-1</ACESuserName>

//
// ACES Color Space Conversion - Blackmagic Film Wide Gamut (Gen 5) to ACES2065-1
//
// converts Blackmagic Film Wide Gamut (Gen 5) to 
//          ACES2065-1 (AP0 w/ linear encoding)
//



import "ACESlib.Utilities_Color";



const float AP0_TO_BMD_CAM_WG_GEN5_PRI_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( AP0,
                                                     BMD_CAM_WG_GEN5_PRI,
                                                     CONE_RESP_MAT_CAT02 );

const float A = 0.08692876065491224;
const float B = 0.005494072432257808;
const float C = 0.5300133392291939;
const float D = 8.283605932402494;
const float E = 0.09246575342465753;
const float LIN_CUT = 0.005;
const float LOG_CUT = D * LIN_CUT + E;

// Inverse OETF
float BMDFilm_Gen5_to_lin( input varying float y) {
    float x;
    if ( y < LOG_CUT ) {
        x = (y - E) / D;
    } else {
        x = exp((y - C)/A) - B;
    }
    return x;
}



void main
(   input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
    // Blackmagic Film (Gen 5) Inverse OETF
    float lin_BMD_Cam_WG[3];
    lin_BMD_Cam_WG[0] = BMDFilm_Gen5_to_lin( rIn);
    lin_BMD_Cam_WG[1] = BMDFilm_Gen5_to_lin( gIn);
    lin_BMD_Cam_WG[2] = BMDFilm_Gen5_to_lin( bIn);

    // Blackmagic Camera Wide Gamut (Gen 5) to AP0
    float ACES[3] = mult_f3_f33( lin_BMD_Cam_WG, BMD_CAM_WG_GEN5_PRI_TO_AP0_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}