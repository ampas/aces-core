
// <ACEStransformID>LMT.Academy.BlueLightArtifactFix.a1.1</ACEStransformID>
// <ACESuserName>ACES 1.0 Look - Blue Light Artifact Fix</ACESuserName>

//
// LMT for desaturating blue hues in order to reduce the artifact where bright 
// blue colors (e.g. sirens, headlights, LED lighting, etc.) may become 
// oversaturated or exhibit hue shifts as a result of clipping.
// 


const float correctionMatrix[3][3] = {
  { 0.9404372683,  0.0083786969,  0.0005471261},
  {-0.0183068787,  0.8286599939, -0.0008833746},
  { 0.0778696104,  0.1629613092,  1.0003362486}
};


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
    float aces[3] = {rIn, gIn, bIn};

    float acesMod[3] = mult_f3_f33( aces, correctionMatrix);

    rOut = acesMod[0];
    gOut = acesMod[1];
    bOut = acesMod[2];
    aOut = aIn;
}