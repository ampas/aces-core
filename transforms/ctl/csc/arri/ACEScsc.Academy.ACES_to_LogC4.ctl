
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACEScsc.Academy.ACES_to_LogC4.a1.1.0</ACEStransformID>
// <ACESuserName>ACES2065-1 to ARRI LogC4</ACESuserName>

//
// ACES Color Space Conversion - ACES to ARRI LogC4
//
// converts ACES2065-1 (AP0 w/ linear encoding) to ARRI LogC4
//



import "ACESlib.Utilities_Color";


const float AP0_2_AWG4_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( AP0, 
                                                     ARRI_ALEXA_WG4_PRI, 
                                                     CONE_RESP_MAT_CAT02);


// LogC4 Curve Encoding Function
float RelativeSceneLinearToNormalizedLogC4( float x) {

    // Constants
    const float a = (pow(2.0, 18.0) - 16.0) / 117.45;
    const float b = (1023.0 - 95.0) / 1023.0;
    const float c = 95.0 / 1023.0;
    const float s = (7.0 * log(2.0) * pow(2.0, 7.0 - 14.0 * c / b)) / (a * b);
    const float t = (pow(2.0, 14.0 * (-c / b) + 6.0) - 64.0) / a;

    if (x < t) {
        return (x - t) / s;
    }

    return (log2(a * x + 64.0) - 6.0) / 14.0 * b + c;
}



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
    float ACES[3] = { rIn, gIn, bIn};

    float lin_AWG4[3] = mult_f3_f33( ACES, AP0_2_AWG4_MAT);

    rOut = RelativeSceneLinearToNormalizedLogC4( lin_AWG4[0]);
    gOut = RelativeSceneLinearToNormalizedLogC4( lin_AWG4[1]);
    bOut = RelativeSceneLinearToNormalizedLogC4( lin_AWG4[2]);
    aOut = aIn;
}