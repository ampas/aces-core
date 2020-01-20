
// <ACEStransformID>ACEScsc.CLog3_CGamut_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>CLog3 CGamut to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - CLog3 CGamut to ACES
//
// converts Canon Log 3, Cinema Gamut to 
//          ACES2065-1 (AP0 w/ linear encoding)
//



const float CGamut_2_AP0_MAT[3][3] = {
  { 0.763064455,  0.149021161,  0.087914384},
  { 0.003657457,  1.106960380, -0.110617837},
  {-0.009407794, -0.218383305,  1.227791099}
};



float CLog3_to_lin( input varying float in)
{
    float out;
    if ( in < 0.097465473 )
    {
        out = -(pow(10, (0.12783901 - in) / 0.36726845) - 1) / 14.98325;
    }
    else if ( in <= 0.15277891 )
    {
        out = (in - 0.12512219) / 1.9754798;
    }
    else
    {
        out = (pow(10, (in - 0.12240537) / 0.36726845) - 1) / 14.98325;
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
    lin_CGamut[0] = 0.9 * CLog3_to_lin( rIn);
    lin_CGamut[1] = 0.9 * CLog3_to_lin( gIn);
    lin_CGamut[2] = 0.9 * CLog3_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_CGamut, CGamut_2_AP0_MAT);

    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}
