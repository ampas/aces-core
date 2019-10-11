
// <ACEStransformID>ACEScsc.SLog3_SG3C_to_ACES.a1.v1</ACEStransformID>
// <ACESuserName>SLog3 SG3C to ACES2065-1</ACESuserName>

//
// ACES Color Space Conversion - RED Log3G10 RWG to ACES
//
// converts S-Log3, S-Gamut3.Cine to 
//          ACES2065-1 (AP0 w/ linear encoding)
//



const float SG3C_2_AP0_MAT[3][3] = {
  { 0.6387886672, -0.0039159061, -0.0299072021},
  { 0.2723514337,  1.0880732308, -0.0264325799},
  { 0.0888598992, -0.0841573249,  1.0563397820}
};



float SLog3_to_lin( input varying float in)
{
    float out;
    if ( in >= 171.2102946929 / 1023.0 )
    {
        out = pow(10.0, (in * 1023.0 - 420.0) / 261.5) * (0.18 + 0.01) - 0.01;
    }
    else
    {
        out = (in * 1023.0 - 95.0) * 0.01125000 / (171.2102946929 - 95.0);
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
    float lin_SG3C[3];
    lin_SG3C[0] = SLog3_to_lin( rIn);
    lin_SG3C[1] = SLog3_to_lin( gIn);
    lin_SG3C[2] = SLog3_to_lin( bIn);

    float ACES[3] = mult_f3_f33( lin_SG3C, SG3C_2_AP0_MAT);
  
    rOut = ACES[0];
    gOut = ACES[1];
    bOut = ACES[2];
    aOut = aIn;
}