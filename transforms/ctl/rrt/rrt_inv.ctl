// 
// Inverse Reference Rendering Transform (RRT)
// v0.2
//

import "utilities";
import "utilities-aces";

const Chromaticities RENDERING_PRI = 
{
  {0.73470, 0.26530},
  {0.00000, 1.00000},
  {0.12676, 0.03521},
  {0.32168, 0.33767}
};

const float ACES_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ(ACES_PRI,1.0);
const float XYZ_2_RENDERING_PRI_MAT[4][4] = XYZtoRGB(RENDERING_PRI,1.0);
const float ACES_PRI_2_RENDERING_PRI_MAT[4][4] = mult_f44_f44( ACES_PRI_2_XYZ_MAT, XYZ_2_RENDERING_PRI_MAT);

// Chroma scaling parameters
const float center = 350.;    // center hue (in degrees)
const float width = 140.;     // full base width (in degrees)
const float percent = 0.83;   // percent scale (0-1)

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
  // Put input variables into a 3-element array (OCES)
  float oces[3] = {rIn, gIn, bIn};

  // Convert from OCES RGB encoding to rendering primaries RGB encoding
  float rgbPre[3] = mult_f3_f44( oces, ACES_PRI_2_RENDERING_PRI_MAT);

    // Apply the reverse of the RRT tone scale independently to RGB
    float rgbPost[3];
    rgbPost[0] = rrt_tonescale_rev( rgbPre[0]);
    rgbPost[1] = rrt_tonescale_rev( rgbPre[1]);
    rgbPost[2] = rrt_tonescale_rev( rgbPre[2]);

  // Restore the hue to the pre-tonescale hue
  float rgbRestored[3] = restore_hue_dw3( rgbPre, rgbPost);

  // Convert from rendering primaries RGB encoding to ACES RGB encoding
  float aces[3] = mult_f3_f44( rgbRestored, invert_f44(ACES_PRI_2_RENDERING_PRI_MAT));

  // Adjust ACES values
  float yab[3] = rgb_2_yab( aces);

    // Scale chroma in red/magenta region
    yab = scale_C_at_H_inv( yab, center, width, percent);

  aces = yab_2_rgb( yab);

  // Assign ACES-RGB to output variables (ACES)
  rOut = aces[0];
  gOut = aces[1];
  bOut = aces[2];
  aOut = aIn;
}