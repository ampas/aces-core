
// <ACEStransformID>RRT.a1.0.0</ACEStransformID>

// 
// Reference Rendering Transform (RRT)
//
//   Input is ACES
//   Output is OCES
//



import "ACESlib.Utilities.a1.0.0";
import "ACESlib.Transform_Common.a1.0.0";
import "ACESlib.RRT_Common.a1.0.0";
import "ACESlib.Tonescales.a1.0.0";



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
    aces = clamp_f3( aces, 0., HALF_POS_INF);  // avoids saturated negative colors from becoming positive in the matrix

    float rgbPre[3] = mult_f3_f44( aces, AP0_2_AP1_MAT);

    rgbPre = clamp_f3( rgbPre, 0., HALF_MAX);

  // --- Global desaturation --- //
    rgbPre = mult_f3_f33( rgbPre, RRT_SAT_MAT);

  // --- Apply the tonescale independently in rendering-space RGB --- //
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c5_fwd( rgbPre[0]);
    rgbPost[1] = segmented_spline_c5_fwd( rgbPre[1]);
    rgbPost[2] = segmented_spline_c5_fwd( rgbPre[2]);

  // --- RGB rendering space to OCES --- //
    float rgbOces[3] = mult_f3_f44( rgbPost, AP1_2_AP0_MAT);
    
  // Assign OCES RGB to output variables (OCES)
  rOut = rgbOces[0];
  gOut = rgbOces[1];
  bOut = rgbOces[2];
  aOut = aIn;
}