
// <ACEStransformID>ACEScsc.ACES_to_SLog3_SGamut3.a1.v1</ACEStransformID>
// <ACESuserName>ACES2065-1 to SLog3 SGamut3</ACESuserName>

//
// ACES Color Space Conversion - ACES to S-Log3 S-Gamut3
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          S-Log3, S-Gamut3
//



const float AP0_2_SG3_MAT[3][3] = {
  { 1.3316572111, -0.0280131244,  0.0125574528},
  {-0.1875611006,  0.9887375645, -0.0050679052},
  {-0.1440961106,  0.0392755599,  0.9925104526}
};



float lin_to_SLog3( input varying float in)
{
    float out;
    if ( in >= 0.01125000 )
    {
        out = (420.0 + log10((in + 0.01) / (0.18 + 0.01)) * 261.5) / 1023.0;
    }
    else
    {
        out = (in * (171.2102946929 - 95.0) / 0.01125000 + 95.0) / 1023.0;
    }
    return out;
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

    float lin_SG3[3] = mult_f3_f33( ACES, AP0_2_SG3_MAT);

    rOut = lin_to_SLog3( lin_SG3[0]);
    gOut = lin_to_SLog3( lin_SG3[1]);
    bOut = lin_to_SLog3( lin_SG3[2]);
    aOut = aIn;
}