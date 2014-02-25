// 
// Inverse Reference Rendering Transform (RRT)
// v0.7.1
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
  /* --- Initialize a 3-element vector with input variables (OCES) --- */
    float ocesOrig[3] = {rIn, gIn, bIn};
    float oces[3] = {rIn, gIn, bIn};
  
  /* --- Apply the RRT tone scale independently to RGB --- */
    float rgbPost[3];
    rgbPost[0] = rrt_tonescale_rev( oces[0]);
    rgbPost[1] = rrt_tonescale_rev( oces[1]);
    rgbPost[2] = rrt_tonescale_rev( oces[2]);

    float aces[3] = restore_hue_dw3( ocesOrig, rgbPost);
        
  /* --- Red modifier --- */
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

    aces = restore_hue_dw3( ocesOrig, aces);

  /* --- Glow module --- */
    float saturation = rgb_2_saturation( aces);
    float ycOut = rgb_2_yc( aces);
   	float s = sigmoid_shaper( (saturation - 0.4) / 0.2);
   	float reducedGlow = 1. + glow_inv( ycOut, RRT_GLOW_GAIN * s, RRT_GLOW_MID);
  	
  	aces = mult_f_f3( ( reducedGlow), aces);

  // Assign OCES-RGB to output variables (OCES)
  rOut = aces[0];
  gOut = aces[1];
  bOut = aces[2];
  aOut = aIn;

}