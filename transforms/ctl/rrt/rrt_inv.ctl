// 
// Inverse Reference Rendering Transform (RRT)
// WGR8
//


import "utilities";
import "transforms-common";
import "rrt-transform-common";



// Inverse Reference Rendering Transform (RRT)
//   Input is OCES RGB (linearly encoded)
//   Output is ACES RGB (linearly encoded)
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
  // --- Initialize a 3-element vector with input variables (OCES) --- //
    float oces[3] = {rIn, gIn, bIn};
  
  // --- Apply the RRT tone scale independently to RGB --- //
    // ACES to RGB rendering space
    float rgbPre[3] = mult_f3_f44( oces, ACES_2_RENDER_PRI_MAT);

    // Tonescale
    float rgbPost[3];
    rgbPost[0] = rrt_tonescale_rev( rgbPre[0], RRT_COEFS);
    rgbPost[1] = rrt_tonescale_rev( rgbPre[1], RRT_COEFS);
    rgbPost[2] = rrt_tonescale_rev( rgbPre[2], RRT_COEFS);

    // RGB rendering space back to ACES encoding
    float aces[3] = mult_f3_f44( rgbPost, RENDER_PRI_2_ACES_MAT);
        
  // --- Red modifier --- //
    float hue = rgb_2_hue( aces);
    float centeredHue = center_hue( hue, RRT_RED_HUE);
    float hueWeight = cubic_basis_shaper( centeredHue, RRT_RED_WIDTH);

    float minChan;
    if (centeredHue < 0) { // min_f3(aces) = aces[1] (i.e. magenta-red)
      minChan = aces[1];
    } else { // min_f3(aces) = aces[2] (i.e. yellow-red)
      minChan = aces[2];
    }

    float a = hueWeight * (1. - RRT_RED_SCALE) - 1.;
    float b = aces[0] - hueWeight * (RRT_RED_PIVOT + minChan) * (1. - RRT_RED_SCALE);
    float c = hueWeight * RRT_RED_PIVOT * minChan * (1. - RRT_RED_SCALE);

    aces[0] = ( -b - sqrt( b * b - 4. * a * c)) / ( 2. * a);

  // Assign ACES-RGB to output variables (ACES)
  rOut = aces[0];
  gOut = aces[1];
  bOut = aces[2];
  aOut = aIn;

}