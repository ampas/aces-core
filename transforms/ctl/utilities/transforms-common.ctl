//
// transforms-common.ctl
// WGR8
//
// Contains functions and constants shared by multiple forward and inverse 
// transforms. This is primarily to avoid code redundancy, particularly in the 
// ODTs. This structure also has the benefit of facilitating any updates to 
// these parameters or functions, if necessary.
//


import "utilities-color";


// Rendering primaries
const Chromaticities RENDER_PRI = 
{
  {0.70800, 0.29200},
  {0.17000, 0.79700},
  {0.13100, 0.04600},
  {0.32168, 0.33767}
};

const float ACES_2_XYZ_MAT[4][4] = RGBtoXYZ( ACES_PRI, 1.0);
const float XYZ_2_ACES_MAT[4][4] = XYZtoRGB( ACES_PRI, 1.0);
const float XYZ_2_RENDER_PRI_MAT[4][4] = XYZtoRGB( RENDER_PRI, 1.0);

const float ACES_2_RENDER_PRI_MAT[4][4] = mult_f44_f44( ACES_2_XYZ_MAT, XYZ_2_RENDER_PRI_MAT);
const float RENDER_PRI_2_ACES_MAT[4][4] = invert_f44( ACES_2_RENDER_PRI_MAT);



const float TINY = 1e-10;



// Textbook monomial to basis-function conversion matrix.
const float M[ 3][ 3] = {
  {  0.5, -1.0, 0.5 },
  { -1.0,  1.0, 0.5 },
  {  0.5,  0.0, 0.0 }
};



// ------- Hue restore functions
int[3] order3( float r, float g, float b)
{  
   // determines sort order, highest to lowest
   
   if (r > g) {
      if (g > b) {                    // r g b, hue [0,60]
         int order[3] = {0, 1, 2};
         return order;
      } else {
         if (r > b) {                 // r b g, hue [300,360]
            int order[3] = {0, 2, 1};
            return order;
         } else {                     // b r g, hue [240,300]
            int order[3] = {2, 0, 1};
            return order;
         }
      }
   }
   else {
      if (r > b) {                    // g r b, hue [60,120]
         int order[3] = {1, 0, 2};
         return order;
      } else {
         if (g > b) {                 // g b r, hue [120,180]
            int order[3] = {1, 2, 0};
            return order;
         } else {                     // b g r, hue [180,240]
            int order[3] = {2, 1, 0};
            return order;
         }
      }
   }
}



float[3] restore_hue_dw3( float pre_tone[3], float post_tone[3])
{
    // modifies the hue of post_tone RGB to match hue pre_tone RGB, by moving 
    // the middle channel

    int inds[3] = order3( pre_tone[0], pre_tone[1], pre_tone[2]);

    float orig_chroma = pre_tone[ inds[0]] - pre_tone[ inds[2]]; 

    float hue_factor = 0;

    if (orig_chroma != 0.) hue_factor = ( pre_tone[ inds[1] ] - pre_tone[ inds[2] ]) / orig_chroma;

    float new_chroma = post_tone[ inds[0] ] - post_tone[ inds[2] ];

    float out[3];
    out[ inds[ 0] ] = post_tone[ inds[0] ];
    out[ inds[ 1] ] = hue_factor * new_chroma + post_tone[ inds[2] ];
    out[ inds[ 2] ] = post_tone[ inds[2] ];

    return out;
}



float rgb_2_saturation( float rgb[3])
{
  return ( max( max_f3(rgb), TINY) - max( min_f3(rgb), TINY)) / max( max_f3(rgb), 1e-2);
}