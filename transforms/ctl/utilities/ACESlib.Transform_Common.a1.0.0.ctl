//
// Contains functions and constants shared by multiple forward and inverse 
// transforms 
//



import "ACESlib.Utilities_Color.a1.0.0";



const Chromaticities AP1 = 
{
  {0.713,	0.293},
  {0.165,	0.830},
  {0.128,	0.044},
  {0.32168,	0.33767}
};

const float AP0_2_XYZ_MAT[4][4] = RGBtoXYZ( AP0, 1.0);
const float XYZ_2_AP0_MAT[4][4] = XYZtoRGB( AP0, 1.0);

const float AP1_2_XYZ_MAT[4][4] = RGBtoXYZ( AP1, 1.0);
const float XYZ_2_AP1_MAT[4][4] = XYZtoRGB( AP1, 1.0);

const float AP0_2_AP1_MAT[4][4] = mult_f44_f44( AP0_2_XYZ_MAT, XYZ_2_AP1_MAT);
const float AP1_2_ACES_MAT[4][4] = invert_f44( ACES_2_AP1_MAT);

const float AP1_RGB2Y[3] = { AP1_2_XYZ_MAT[0][1], 
                             AP1_2_XYZ_MAT[1][1], 
                             AP1_2_XYZ_MAT[2][1] };



const float TINY = 1e-10;




float acesLog_to_lin( input varying float in)
{
  return pow(2.0, ((in*876.0+64.0-425.0)/50.0-2.5));
}

float lin_2_acesLog( input varying float in)
{
  float out;
  if (in > 0)
    out = ((log(in)/log(2.0)+2.5)*50.0+425.0);
  else
    out = 0.0;
    
  return (out-64.0)/876.0;
}



float lin_2_acesProxy( input varying float in)
{
  float out;
  if (in > 0) 
    out = clamp( ((log(in)/log(2.0)+2.5)*50.0+425.0), 64., 940.);
  
  return (out-64.0)/876.0;
}

float acesProxy_to_lin( input varying float in)
{
  return acesLog_to_lin( in);
}



float rgb_2_saturation( float rgb[3])
{
  return ( max( max_f3(rgb), TINY) - max( min_f3(rgb), TINY)) / max( max_f3(rgb), 1e-2);
}