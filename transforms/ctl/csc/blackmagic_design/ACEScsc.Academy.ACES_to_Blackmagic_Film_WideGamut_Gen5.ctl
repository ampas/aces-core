
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_BMDFilm_WideGamut_Gen5.a1.v1</ACEStransformID>
// <ACESuserName>ACES2605-1 to Blackmagic Film Wide Gamut (Gen 5)</ACESuserName>

//
// ACES Color Space Conversion - ACES2065-1 to Blackmagic Film Wide Gamut (Gen 5)
//
// converts ACES2065-1 (AP0 w/ linear encoding) to
//          Blackmagic Film Wide Gamut (Gen 5)
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

// Forward OETF
float lin_to_BMDFilm_Gen5( input varying float x) {
    float y;
    if ( x < LIN_CUT ) {
        y = D * x + E;
    } else {
        y = A * log(x + B) + C;
    }
    return y;
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
    float ACES[3] = {rIn, gIn, bIn};

    // AP0 to Blackmagic Camera Wide Gamut (Gen 5)
    float lin_BMD_Cam_WG_Gen5[3] = mult_f3_f33( ACES, AP0_TO_BMD_CAM_WG_GEN5_PRI_MAT);

    // Blackmagic Film (Gen 5) Forward OETF
    rOut = lin_to_BMDFilm_Gen5( lin_BMD_Cam_WG_Gen5[0]);
    gOut = lin_to_BMDFilm_Gen5( lin_BMD_Cam_WG_Gen5[1]);
    bOut = lin_to_BMDFilm_Gen5( lin_BMD_Cam_WG_Gen5[2]);
    aOut = aIn;
}