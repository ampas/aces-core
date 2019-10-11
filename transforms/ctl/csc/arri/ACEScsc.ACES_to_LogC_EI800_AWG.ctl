
// <ACEStransformID>ACEScsc.ACES_to_LogC_EI800_AWG.a1.v1</ACEStransformID>
// <ACESuserName>ACES2065-1 to LogC EI800 AWG</ACESuserName>

//
// ACES Color Space Conversion - ACES to LogC (EI800) AWG
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          ALEXA LogC (EI800) ALEXA Wide Gamut
//



const float midGraySignal = 0.01;
const float cut = 1.0 / 9.0;
const float slope = 3.9086503371;
const float offset =  -1.3885369913;
const float encOffset = 0.3855369987;
const float gain = 800.0 / 400.0;
const float encGain = 0.2471896383;
const float gray = 0.005;
const float nz = 0.0522722750;

const float AP0_2_AWG_MAT[3][3] = {
  { 1.5159863829, -0.1283275799, -0.0105107561},
  {-0.3613418588,  1.0193145873,  0.0608329325},
  {-0.1546444592,  0.1090123949,  0.9496764954}
};



float lin_to_LogC( input varying float in)
{
    float out;
    float ns = in / (0.18 * gain / midGraySignal);
    ns = nz + (ns / gray);
    if (ns > cut)
    {
        out = log10(ns);
    }
    else
    {
        out = offset + (ns * slope);
    }
    return encOffset + (out * encGain);
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

    float lin_AWG[3] = mult_f3_f33( ACES, AP0_2_AWG_MAT);

    rOut = lin_to_LogC( lin_AWG[0]);
    gOut = lin_to_LogC( lin_AWG[1]);
    bOut = lin_to_LogC( lin_AWG[2]);
    aOut = aIn;
}