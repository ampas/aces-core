
// <ACEStransformID>ACESlib.Transform_Common.a1.0.0</ACEStransformID>

//
// Contains functions and constants shared by multiple forward and inverse 
// transforms 
//



import "ACESlib.Utilities_Color.a1.0.0";
import "ACESlib.Utilities.a1.0.0";



const float AP0_2_XYZ_MAT[4][4] = RGBtoXYZ( AP0, 1.0);
const float XYZ_2_AP0_MAT[4][4] = XYZtoRGB( AP0, 1.0);

const float AP1_2_XYZ_MAT[4][4] = RGBtoXYZ( AP1, 1.0);
const float XYZ_2_AP1_MAT[4][4] = XYZtoRGB( AP1, 1.0);

const float AP0_2_AP1_MAT[4][4] = mult_f44_f44( AP0_2_XYZ_MAT, XYZ_2_AP1_MAT);
const float AP1_2_AP0_MAT[4][4] = mult_f44_f44( AP1_2_XYZ_MAT, XYZ_2_AP0_MAT);

const float AP1_RGB2Y[3] = { AP1_2_XYZ_MAT[0][1], 
                             AP1_2_XYZ_MAT[1][1], 
                             AP1_2_XYZ_MAT[2][1] };



const float TINY = 1e-10;



float rgb_2_saturation( float rgb[3])
{
  return ( max( max_f3(rgb), TINY) - max( min_f3(rgb), TINY)) / max( max_f3(rgb), 1e-2);
}