// 
// Reference Rendering Transform (RRT)
//
//   Input is ACES
//   Output is OCES
//



import "utilities";
import "transforms-common";
import "rrt-transform-common";
import "tonescales";


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
//   print("\n\nACES:\t"); print_f3(aces);

  // --- Glow module --- //
    float saturation = rgb_2_saturation( aces);
    float ycIn = rgb_2_yc( aces);
    float s = sigmoid_shaper( (saturation - 0.4) / 0.2);
    float addedGlow = 1. + glow_fwd( ycIn, RRT_GLOW_GAIN * s, RRT_GLOW_MID);

    aces = mult_f_f3( addedGlow, aces);
  
  // --- Red modifier --- //
    float hue = rgb_2_hue( aces);
    float centeredHue = center_hue( hue, RRT_RED_HUE);
    float hueWeight = cubic_basis_shaper( centeredHue, RRT_RED_WIDTH);
		
	  aces[0] = aces[0] + hueWeight * saturation * (RRT_RED_PIVOT - aces[0]) * (1. - RRT_RED_SCALE);

  // --- ACES to RGB rendering space --- //
    aces = clamp_f3( aces, 0., HALF_POS_INF);  // avoids saturated negative colors from flipping positive

//   print("\nacesAfterGlowAndRed:\t"); print_f3(aces);

    float rgbPre[3] = mult_f3_f44( aces, ACES_2_RENDER_PRI_MAT);

//   print("\nrgbPre:\t"); print_f3(rgbPre);

    rgbPre = clamp_f3( rgbPre, 0., HALF_MAX);

//   print("\nrgbPreClamp:\t"); print_f3(rgbPre);

  // --- Global desaturation --- //
    rgbPre = mult_f3_f33( rgbPre, RRT_SAT_MAT);

//   print("\nrgbPreDesat:\t"); print_f3(rgbPre);

  // --- Apply the tonescale independently in rendering-space RGB --- //
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c5_fwd( rgbPre[0]);
    rgbPost[1] = segmented_spline_c5_fwd( rgbPre[1]);
    rgbPost[2] = segmented_spline_c5_fwd( rgbPre[2]);
//     rgbPost[0] = rrt_tonescale_fwd( rgbPre[0]);
//     rgbPost[1] = rrt_tonescale_fwd( rgbPre[1]);
//     rgbPost[2] = rrt_tonescale_fwd( rgbPre[2]);

//   print("\nrgbPost:\t"); print_f3(rgbPost);

  // --- RGB rendering space to OCES --- //
    float rgbOces[3] = mult_f3_f44( rgbPost, RENDER_PRI_2_ACES_MAT);

//   print("\nOCES:\t"); print_f3(rgbOces);
    
  // Assign OCES RGB to output variables (OCES)
  rOut = rgbOces[0];
  gOut = rgbOces[1];
  bOut = rgbOces[2];
  aOut = aIn;
}