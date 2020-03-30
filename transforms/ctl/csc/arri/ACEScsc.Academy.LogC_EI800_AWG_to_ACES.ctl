
// <ACEStransformID>ACEScsc.LogC_EI800_AWG_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>LogC EI800 AWG to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - LogC (EI800) AWG to ACES
//
// converts ALEXA LogC (EI800) ALEXA Wide Gamut to 
//          ACES2065-1 (AP0 w/ linear encoding)
//


import "ACESlib.Utilities_Color";


const float AWG_2_AP0_MAT[3][3] = 
                        calculate_rgb_to_rgb_matrix( ARRI_ALEXA_WG_PRI, 
                                                     AP0, 
                                                     CONE_RESP_MAT_CAT02);


const float midGraySignal = 0.01;
const float cut = 1.0 / 9.0;
const float slope = 3.9086503371;
const float offset =  -1.3885369913;
const float encOffset = 0.3855369987;
const float gain = 800.0 / 400.0;
const float encGain = 0.2471896383;
const float gray = 0.005;
const float nz = 0.0522722750;

float LogC_to_lin( input varying float in)
{
    float out = (in - encOffset) / encGain;
    float ns = (out - offset) / slope;
    if (ns > cut)
    {
        ns = pow(10.0, out);
    }
    ns = (ns - nz) * gray;
    return ns * (0.18 * gain / midGraySignal);
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
    float lin_AWG[3];
    lin_AWG[0] = LogC_to_lin( rIn);
    lin_AWG[1] = LogC_to_lin( gIn);
    lin_AWG[2] = LogC_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_AWG, AWG_2_AP0_MAT);
  
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}