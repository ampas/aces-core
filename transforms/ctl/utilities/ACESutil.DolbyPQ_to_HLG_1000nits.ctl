
// <ACEStransformID>ACESutil.DolbyPQ_to_HLG_1000nits.a1.1</ACEStransformID>
// <ACESuserName>Dolby PQ to HLG</ACESuserName>



import "ACESlib.Utilities_Color";



// Conversion of PQ signal to HLG, as detailed in Section 7 of ITU-R BT.2390-0
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
    float PQ[3] = { rIn, gIn, bIn};

    float HLG[3] = ST2084_2_HLG_1000nits_f3( PQ);

    rOut = HLG[0];
    gOut = HLG[1];
    bOut = HLG[2];
    aOut = aIn;  
}