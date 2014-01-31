// 
// Reference Rendering Transform (RRT)
// Working Group Release 6
//

import "utilities";
import "transforms-common";
import "rrt-transform-common";


// Reference Rendering Transform (RRT)
//   Input is ACES RGB (linearly encoded)
//   Output is OCES RGB (linearly encoded)
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
  /* --- Initialize a 3-element vector with input variables (ACES) --- */
    float acesOrig[3] = {rIn, gIn, bIn};
    float aces[3] = {rIn, gIn, bIn};

  /* --- Glow module --- */
    float saturation = rgb_2_saturation( aces);
    float ycIn = rgb_2_yc( aces);
    float s = sigmoid_shaper( (saturation - 0.4) / 0.2);
    float addedGlow = 1. + glow_fwd( ycIn, RRT_GLOW_GAIN * s, RRT_GLOW_MID);

    aces = mult_f_f3( addedGlow, aces);
  
  /* --- Red modifier --- */
    float hue = rgb_2_hue( aces);
    float centeredHue = center_hue( hue, RRT_RED_HUE);
    float hueWeight = cubic_basis_shaper( centeredHue, RRT_RED_WIDTH);
		
	  aces[0] = aces[0] + hueWeight * saturation * (RRT_RED_PIVOT - aces[0]) * (1. - RRT_RED_SCALE);

	  aces = restore_hue_dw3( acesOrig, aces);

  /* --- Apply the RRT tone scale independently to RGB --- */
    float rgbPost[3];
    rgbPost[0] = rrt_tonescale_fwd( aces[0]);
    rgbPost[1] = rrt_tonescale_fwd( aces[1]);
    rgbPost[2] = rrt_tonescale_fwd( aces[2]);

    // Restore the hue to the pre-tonescale hue
    float oces[3] = restore_hue_dw3( aces, rgbPost);
    
  // Assign OCES-RGB to output variables (OCES)
  rOut = oces[0];
  gOut = oces[1];
  bOut = oces[2];
  aOut = aIn;
}