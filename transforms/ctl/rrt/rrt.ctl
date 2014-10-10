// 
// Reference Rendering Transform (RRT)
// WGR8
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
  // --- Initialize a 3-element vector with input variables (ACES) --- //
    float aces[3] = {rIn, gIn, bIn};
  
  // --- Red modifier --- //
    float saturation = rgb_2_saturation( aces);
    float hue = rgb_2_hue( aces);
    float centeredHue = center_hue( hue, RRT_RED_HUE);
    float hueWeight = cubic_basis_shaper( centeredHue, RRT_RED_WIDTH);
		
	  aces[0] = aces[0] + hueWeight * saturation * (RRT_RED_PIVOT - aces[0]) * (1. - RRT_RED_SCALE);

  // --- Apply the tonescale independently in rendering-space RGB --- //
    // ACES to RGB rendering space
    float rgbPre[3] = mult_f3_f44( aces, ACES_2_RENDER_PRI_MAT);

    // Tonescale
    float rgbPost[3];
    rgbPost[0] = rrt_tonescale_fwd( rgbPre[0], RRT_COEFS);
    rgbPost[1] = rrt_tonescale_fwd( rgbPre[1], RRT_COEFS);
    rgbPost[2] = rrt_tonescale_fwd( rgbPre[2], RRT_COEFS);

    // RGB rendering space back to OCES encoding
    float rgbOces[3] = mult_f3_f44( rgbPost, RENDER_PRI_2_ACES_MAT);
    
  // Assign OCES-RGB to output variables (OCES)
  rOut = rgbOces[0];
  gOut = rgbOces[1];
  bOut = rgbOces[2];
  aOut = aIn;
}