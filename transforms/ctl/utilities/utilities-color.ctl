//
// utilities-color.ctl
// WGR8.5
//
// Color related constants and functions
//

/* ---- Chromaticities of some common primary sets ---- */

const Chromaticities ACES_PRI =
{
  { 0.73470,  0.26530},
  { 0.00000,  1.00000},
  { 0.00010, -0.07700},
  { 0.32168,  0.33767}
};

const Chromaticities REC709_PRI =
{
  { 0.64000,  0.33000},
  { 0.30000,  0.60000},
  { 0.15000,  0.06000},
  { 0.31270,  0.32900}
};

const Chromaticities P3D60_PRI =
{
  { 0.68000,  0.32000},
  { 0.26500,  0.69000},
  { 0.15000,  0.06000},
  { 0.32168,  0.33767}
};

const Chromaticities P3DCI_PRI =
{
  { 0.68000,  0.32000},
  { 0.26500,  0.69000},
  { 0.15000,  0.06000},
  { 0.31400,  0.35100}
};

const Chromaticities ARRI_ALEXA_WG_PRI =
{
  {0.68400, 0.31300},
  {0.22100, 0.84800},
  {0.08610, -0.10200},
  {0.31270, 0.32900}
};

const Chromaticities REC2020_PRI = 
{
  {0.70800, 0.29200},
  {0.17000, 0.79700},
  {0.13100, 0.04600},
  {0.31270, 0.32900}
};

const Chromaticities RIMMROMM_PRI = 
{
  {0.7347, 0.2653},
  {0.1596, 0.8404},
  {0.0366, 0.0001},
  {0.3457, 0.3585}
};




/* ---- Conversion Functions ---- */
// Various transformations between color encodings and data representations
//

// Transformations between CIE XYZ tristimulus values and CIE x,y 
// chromaticity coordinates
float[3] XYZ_2_xyY( float XYZ[3])
{  
  float xyY[3];
  float divisor = (XYZ[0] + XYZ[1] + XYZ[2]);
  if (divisor == 0.) divisor = 1e-10;
  xyY[0] = XYZ[0] / divisor;
  xyY[1] = XYZ[1] / divisor;  
  xyY[2] = XYZ[1];
  
  return xyY;
}

float[3] xyY_2_XYZ( input varying float xyY[3])
{
  float XYZ[3];
  XYZ[0] = xyY[0] * xyY[2] / max( xyY[1], 1e-10);
  XYZ[1] = xyY[2];  
  XYZ[2] = (1.0 - xyY[0] - xyY[1]) * xyY[2] / max( xyY[1], 1e-10);

  return XYZ;
}


// Transformations from RGB to other color representations
float rgb_2_hue( float rgb[3]) 
{
  // Returns a geometric hue angle in degrees (0-360) based on RGB values.
  // For neutral colors, hue is undefined and the function will return a quiet NaN value.
  float hue;
  if (rgb[0] == rgb[1] && rgb[1] == rgb[2]) {
    hue = FLT_NAN; // RGB triplets where RGB are equal have an undefined hue
  } else {
    hue = (180./M_PI) * atan2( sqrt(3)*(rgb[1]-rgb[2]), 2*rgb[0]-rgb[1]-rgb[2]);
  }
    
  if (hue < 0.) hue = hue + 360.;

  return hue;
}

float rgb_2_yc( float rgb[3], float ycRadiusWeight = 1.75)
{
  // Converts RGB to a luminance proxy, here called YC
  // YC is ~ Y + K * Chroma
  // Constant YC is a cone-shaped surface in RGB space, with the tip on the 
  // neutral axis, towards white.
  // YC is normalized: RGB 1 1 1 maps to YC = 1
  //
  // ycRadiusWeight defaults to 1.75, although can be overridden in function 
  // call to rgb_2_yc
  // ycRadiusWeight = 1 -> YC for pure cyan, magenta, yellow == YC for neutral 
  // of same value
  // ycRadiusWeight = 2 -> YC for pure red, green, blue  == YC for  neutral of 
  // same value.

  float r = rgb[0]; 
  float g = rgb[1]; 
  float b = rgb[2];
  
  float chroma = sqrt(b*(b-g)+g*(g-r)+r*(r-b));

  return ( b + g + r + ycRadiusWeight * chroma) / 3.;
}



/* ---- Chromatic Adaptation ---- */

const float CONE_RESP_MAT_BRADFORD[3][3] = {
  { 0.89510, -0.75020,  0.03890},
  { 0.26640,  1.71350, -0.06850},
  {-0.16140,  0.03670,  1.02960}
};

const float CONE_RESP_MAT_CAT02[3][3] = {
  { 0.73280, -0.70360,  0.00300},
  { 0.42960,  1.69750,  0.01360},
  {-0.16240,  0.00610,  0.98340}
};

float[3][3] calculate_cat_matrix
  ( float src_xy[2],         // x,y chromaticity of source white
    float des_xy[2],         // x,y chromaticity of destination white
    float coneRespMat[3][3] = CONE_RESP_MAT_BRADFORD
  )
{
  //
  // Calculates and returns a 3x3 Von Kries chromatic adaptation transform 
  // from src_xy to des_xy using the cone response primaries defined 
  // by coneRespMat. By default, coneRespMat is set to CONE_RESP_MAT_BRADFORD. 
  // The default coneRespMat can be overridden at runtime. 
  //

  const float src_xyY[3] = { src_xy[0], src_xy[1], 1. };
  const float des_xyY[3] = { des_xy[0], des_xy[1], 1. };

  float src_XYZ[3] = xyY_2_XYZ( src_xyY );
  float des_XYZ[3] = xyY_2_XYZ( des_xyY );

  float src_coneResp[3] = mult_f3_f33( src_XYZ, coneRespMat);
  float des_coneResp[3] = mult_f3_f33( des_XYZ, coneRespMat);

  float vkMat[3][3] = {
      { des_coneResp[0] / src_coneResp[0], 0.0, 0.0 },
      { 0.0, des_coneResp[1] / src_coneResp[1], 0.0 },
      { 0.0, 0.0, des_coneResp[2] / src_coneResp[2] }
  };

  float cat_matrix[3][3] = mult_f33_f33( coneRespMat, transpose_f33( mult_f33_f33( transpose_f33( invert_f33( coneRespMat ) ), vkMat ) ) );

  return cat_matrix;
}





float[3][3] calc_sat_adjust_matrix 
  ( input varying float sat,
    input varying float rgb2Y[3]
  )
{
  //
  // This function determines the terms for a 3x3 saturation matrix that is
  // based on the luminance of the input.
  //
  float M[3][3];
  M[0][0] = (1.0 - sat) * rgb2Y[0] + sat;
  M[1][0] = (1.0 - sat) * rgb2Y[0];
  M[2][0] = (1.0 - sat) * rgb2Y[0];
  
  M[0][1] = (1.0 - sat) * rgb2Y[1];
  M[1][1] = (1.0 - sat) * rgb2Y[1] + sat;
  M[2][1] = (1.0 - sat) * rgb2Y[1];
  
  M[0][2] = (1.0 - sat) * rgb2Y[2];
  M[1][2] = (1.0 - sat) * rgb2Y[2];
  M[2][2] = (1.0 - sat) * rgb2Y[2] + sat;

  M = transpose_f33(M);    
  return M;
} 





/* ---- Signal encode/decode functions ---- */

float moncurve_f( float x, float gamma, float offs )
{
  // Forward monitor curve
  float y;
  const float fs = (( gamma - 1.0) / offs) * pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
  const float xb = offs / ( gamma - 1.0);
  if ( x >= xb) 
    y = pow( ( x + offs) / ( 1.0 + offs), gamma);
  else
    y = x * fs;
  return y;
}

float moncurve_r( float y, float gamma, float offs )
{
  // Reverse monitor curve
  float x;
  const float yb = pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
  const float rs = pow( ( gamma - 1.0) / offs, gamma - 1.0) * pow( ( 1.0 + offs) / gamma, gamma);
  if ( y >= yb) 
    x = ( 1.0 + offs) * pow( y, 1.0 / gamma) - offs;
  else
    x = y * rs;
  return x;
}

float bt1886_f( float V, float gamma, float Lw, float Lb)
{
  // The reference EOTF specified in Rec. ITU-R BT.1886
  // L = a(max[(V+b),0])^g
  float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
  float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
  float L = a * pow( max( V + b, 0.), gamma);
  return L;
}

float bt1886_r( float L, float gamma, float Lw, float Lb)
{
  // The reference EOTF specified in Rec. ITU-R BT.1886
  // L = a(max[(V+b),0])^g
  float a = pow( pow( Lw, 1./gamma) - pow( Lb, 1./gamma), gamma);
  float b = pow( Lb, 1./gamma) / ( pow( Lw, 1./gamma) - pow( Lb, 1./gamma));
  float V = pow( max( L / a, 0.), 1./gamma) - b;
  return V;
}


// SMPTE Range vs Full Range scaling formulas
float smpteRange_to_fullRange( float in)
{
	const float REFBLACK = (  16. / 256.);
	const float REFWHITE = ( 235. / 256.);
	
  return (( in - REFBLACK) / ( REFWHITE - REFBLACK));
}

float fullRange_to_smpteRange( float in)
{
	const float REFBLACK = (  16. / 256.);
	const float REFWHITE = ( 235. / 256.);
	
	return ( in * ( REFWHITE - REFBLACK) + REFBLACK );
}

float[3] smpteRange_to_fullRange_f3( float rgbIn[3])
{
	float rgbOut[3];
	rgbOut[0] = smpteRange_to_fullRange( rgbIn[0]);
	rgbOut[1] = smpteRange_to_fullRange( rgbIn[1]);
	rgbOut[2] = smpteRange_to_fullRange( rgbIn[2]);

  return rgbOut;
}

float[3] fullRange_to_smpteRange_f3( float rgbIn[3])
{
	float rgbOut[3];
	rgbOut[0] = fullRange_to_smpteRange( rgbIn[0]);
	rgbOut[1] = fullRange_to_smpteRange( rgbIn[1]);
	rgbOut[2] = fullRange_to_smpteRange( rgbIn[2]);

  return rgbOut;
}


// SMPTE 431-2 defines the DCDM color decoding equations. The equations for the decoding of the encoded color information are the inverse of the encoding equations
float[3] dcdm_decode( int XYZp[3])
{
  float XYZ[3];
  XYZ[0] = (52.37/48.0) * pow( XYZp[0]/4095., 2.6);  
  XYZ[1] = (52.37/48.0) * pow( XYZp[1]/4095., 2.6);  
  XYZ[2] = (52.37/48.0) * pow( XYZp[2]/4095., 2.6);  
  
  return XYZ;
}

float[3] dcdm_encode( float XYZ[3])
{
  float XYZp[3];
  XYZp[0] = pow( (48./52.37) * XYZ[0], 1./2.6);
  XYZp[1] = pow( (48./52.37) * XYZ[1], 1./2.6);
  XYZp[2] = pow( (48./52.37) * XYZ[2], 1./2.6);
  
  return XYZp;
}


// PQ formulas
const float n = 0.15930176;
const float m = 78.84375;
const float c1 = 0.8359375;
const float c2 = 18.8515625;
const float c3 = 18.6875;
const float encodeMax = 10000.;

float PQ10K_fwd( float in)
{
  // encodes from luminance to code values
  
  float gRel = clamp( in/encodeMax, 0., 1.);

  return pow((c1+c2*pow(gRel,n))/(1.0+c3*pow(gRel,n)), m);
}

float PQ10K_rev( float in)
{
  // decodes from code values to luminance
  
  return encodeMax*pow((pow(in,1./m)-c1) / (c2-c3*(pow(in,1./m))),1./n);
}

float[3] PQ10K_fwd_f3( float in[3])
{
  // encodes from luminance to code values
  
  float out[3];
  out[0] = PQ10K_fwd( in[0]);
  out[1] = PQ10K_fwd( in[1]);
  out[2] = PQ10K_fwd( in[2]);

  return out;
}

float[3] PQ10K_rev_f3( float in[3])
{
  // decodes from code values to luminance
  
  float out[3];
  out[0] = PQ10K_rev( in[0]);
  out[1] = PQ10K_rev( in[1]);
  out[2] = PQ10K_rev( in[2]);

  return out;
}