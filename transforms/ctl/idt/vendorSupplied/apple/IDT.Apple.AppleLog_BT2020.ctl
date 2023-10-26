// <ACEStransformID>urn:ampas:aces:transformId:v1.5:IDT.Apple.AppleLog_BT2020.a1.v1</ACEStransformID>
// <ACESuserName>ACES 1.0 Input - AppleLog Rec2020</ACESuserName>

import "ACESlib.Utilities_Color";

// ITU-R BT.2020 -to- ACES conversion matrix
const float REC2020_2_ACES_MAT[3][3] = calculate_rgb_to_rgb_matrix(REC2020_PRI, AP0);


float AppleLog_to_Linear( float x ) {
    const float R_0 = -0.05641088;
    const float R_t = 0.01;
    const float c = 47.28711236;
    const float b = 0.00964052;
    const float g = 0.08550479;
    const float d = 0.69336945;
    const float P_t = c * pow((R_t - R_0), 2.0);

    if (x >= P_t) {
        return pow(2.0, (x - d) / g) - b;
    }
    else if (x < P_t && x >= 0.0) {
        return sqrt(x / c) + R_0;
    }
    else {
        return R_0;
    }
}


void main
(   input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut)
{
    float lin[3];
    lin[0] = AppleLog_to_Linear(rIn);
    lin[1] = AppleLog_to_Linear(gIn);
    lin[2] = AppleLog_to_Linear(bIn);

    float ACES[3] = mult_f3_f33(lin, REC2020_2_ACES_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}

