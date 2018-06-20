
// <ACEStransformID>ACESutil.HLG_to_DolbyPQ_1000nits.a1.1</ACEStransformID>
// <ACESuserName>HLG to Dolby PQ</ACESuserName>



import "ACESlib.Utilities_Color";



// Conversion from HLG to PQ signal as detailed in Section 7 of ITU-R BT.2390-0
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
    float HLG[3] = { rIn, gIn, bIn};

    float PQ[3] = HLG_2_ST2084_1000nits_f3( HLG);

    rOut = PQ[0];
    gOut = PQ[1];
    bOut = PQ[2];
    aOut = aIn;
}