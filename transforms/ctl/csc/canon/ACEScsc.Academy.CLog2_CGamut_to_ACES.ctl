
// <ACEStransformID>ACEScsc.CLog2_CGamut_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>CLog2 CGamut to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - CLog2 CGamut to ACES
//
// converts Canon Log 2, Cinema Gamut to 
//          ACES2065-1 (AP0 w/ linear encoding)
//



const float CGamut_2_AP0_MAT[3][3] = {
  { 0.763064455,  0.149021161,  0.087914384},
  { 0.003657457,  1.106960380, -0.110617837},
  {-0.009407794, -0.218383305,  1.227791099}
};



float CLog2_to_lin( input varying float in)
{
    float out;
    if ( in < 0.092864125 )
    {
        out = -(pow(10, (0.092864125 - in) / 0.24136077) - 1) / 87.099375;
    }
    else
    {
        out = (pow(10, (in - 0.092864125) / 0.24136077) - 1) / 87.099375;
    }
    return out;
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
    float lin_CGamut[3];
    lin_CGamut[0] = 0.9 * CLog2_to_lin( rIn);
    lin_CGamut[1] = 0.9 * CLog2_to_lin( gIn);
    lin_CGamut[2] = 0.9 * CLog2_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_CGamut, CGamut_2_AP0_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}
