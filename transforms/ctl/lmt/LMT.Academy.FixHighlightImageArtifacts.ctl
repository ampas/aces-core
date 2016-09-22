//
// LMT for fixing occasional image artifacts seen in bright highlights (e.g. sirens,headlights,etc.)
// Note that this will change scene colorimetry! (but tends to do so in a pleasing way)
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