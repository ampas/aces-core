
// <ACEStransformID>ACEScsc.ACES_to_SLog3_SGamut3Cine.a1.v1</ACEStransformID>
// <ACESuserName>ACES2065-1 to SLog3 SGamut3Cine</ACESuserName>

//
// ACES Color Space Conversion - ACES to S-Log3 S-Gamut3.Cine
//
// converts ACES2065-1 (AP0 w/ linear encoding) to 
//          S-Log3, S-Gamut3.Cine
//



const float AP0_2_SG3C_MAT[3][3] = {
  { 1.5554591070,  0.0090216145,  0.0442640666},
  {-0.3932807985,  0.9185569566,  0.0118502607},
  {-0.1621783087,  0.0724214290,  0.9438856727}
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

    float lin_SG3C[3] = mult_f3_f33( ACES, AP0_2_SG3C_MAT);

    rOut = lin_to_SLog3( lin_SG3C[0]);
    gOut = lin_to_SLog3( lin_SG3C[1]);
    bOut = lin_to_SLog3( lin_SG3C[2]);
    aOut = aIn;
}