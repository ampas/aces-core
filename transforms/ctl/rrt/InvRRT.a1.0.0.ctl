
// <ACEStransformID>InvRRT.a1.0.0</ACEStransformID>

// 
// Inverse Reference Rendering Transform (RRT)
//
//   Input is OCES
//   Output is ACES
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
  // --- Initialize a 3-element vector with input variables (OCES) --- //
    float oces[3] = {rIn, gIn, bIn};

  // --- OCES to RGB rendering space --- //
    float rgbPre[3] = mult_f3_f44( oces, AP0_2_AP1_MAT);

  // --- Apply the tonescale independently in rendering-space RGB --- //
    float rgbPost[3];
    rgbPost[0] = segmented_spline_c5_rev( rgbPre[0]);
    rgbPost[1] = segmented_spline_c5_rev( rgbPre[1]);
    rgbPost[2] = segmented_spline_c5_rev( rgbPre[2]);

  // --- Global desaturation --- //
    rgbPost = mult_f3_f33( rgbPost, invert_f33(RRT_SAT_MAT));

    rgbPost = clamp_f3( rgbPost, 0., HALF_MAX);

  // --- RGB rendering space to ACES --- //
    float aces[3] = mult_f3_f44( rgbPost, AP1_2_AP0_MAT);

    aces = clamp_f3( aces, 0., HALF_MAX);

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

  // --- Glow module --- //
    float saturation = rgb_2_saturation( aces);
    float ycOut = rgb_2_yc( aces);
   	float s = sigmoid_shaper( (saturation - 0.4) / 0.2);
   	float reducedGlow = 1. + glow_inv( ycOut, RRT_GLOW_GAIN * s, RRT_GLOW_MID);
  	
  	aces = mult_f_f3( ( reducedGlow), aces);

  // Assign ACES RGB to output variables (ACES)
  rOut = aces[0];
  gOut = aces[1];
  bOut = aces[2];
  aOut = aIn;
}