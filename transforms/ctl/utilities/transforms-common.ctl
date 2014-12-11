//
// transforms-common.ctl
//
// Contains functions and constants shared by multiple forward and inverse 
// transforms 
//


import "utilities-color";


// Rendering primaries
const Chromaticities RENDER_PRI = 
{
  {0.713,	0.293},
  {0.165,	0.830},
  {0.128,	0.044},
  {0.32168,	0.33767}
};

const float ACES_2_XYZ_MAT[4][4] = RGBtoXYZ( ACES_PRI, 1.0);
const float XYZ_2_ACES_MAT[4][4] = XYZtoRGB( ACES_PRI, 1.0);
const float XYZ_2_RENDER_PRI_MAT[4][4] = XYZtoRGB( RENDER_PRI, 1.0);
const float RENDER_PRI_2_XYZ_MAT[4][4] = RGBtoXYZ( RENDER_PRI, 1.0);

const float ACES_2_RENDER_PRI_MAT[4][4] = mult_f44_f44( ACES_2_XYZ_MAT, XYZ_2_RENDER_PRI_MAT);
const float RENDER_PRI_2_ACES_MAT[4][4] = invert_f44( ACES_2_RENDER_PRI_MAT);

const float RENDER_RGB2Y[3] = { RENDER_PRI_2_XYZ_MAT[0][1], 
                                RENDER_PRI_2_XYZ_MAT[1][1], 
                                RENDER_PRI_2_XYZ_MAT[2][1] };



const float TINY = 1e-10;




float rgb_2_saturation( float rgb[3])
{
  return ( max( max_f3(rgb), TINY) - max( min_f3(rgb), TINY)) / max( max_f3(rgb), 1e-2);
}