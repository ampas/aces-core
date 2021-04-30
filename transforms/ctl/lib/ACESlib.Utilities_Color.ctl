
// <ACEStransformID>urn:ampas:aces:transformId:v1.5:ACESlib.Utilities_Color.a1.1.0</ACEStransformID>
// <ACESuserName>ACES 1.0 Lib - Color Utilities</ACESuserName>

//
// Color related constants and functions
//



import "ACESlib.Utilities";



/* ---- Chromaticities of some common primary sets ---- */

const Chromaticities AP0 = // ACES Primaries from SMPTE ST2065-1
{
  { 0.73470,  0.26530},
  { 0.00000,  1.00000},
  { 0.00010, -0.07700},
  { 0.32168,  0.33767}
};

const Chromaticities AP1 = // Working space and rendering primaries for ACES 1.0
{
  { 0.713,    0.293},
  { 0.165,    0.830},
  { 0.128,    0.044},
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

const Chromaticities P3D65_PRI =
{
  { 0.68000,  0.32000},
  { 0.26500,  0.69000},
  { 0.15000,  0.06000},
  { 0.31270,  0.32900}
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
  { 0.68400,  0.31300},
  { 0.22100,  0.84800},
  { 0.08610, -0.10200},
  { 0.31270,  0.32900}
};

const Chromaticities REC2020_PRI = 
{
  { 0.70800,  0.29200},
  { 0.17000,  0.79700},
  { 0.13100,  0.04600},
  { 0.31270,  0.32900}
};

const Chromaticities RIMMROMM_PRI = 
{
  { 0.7347,  0.2653},
  { 0.1596,  0.8404},
  { 0.0366,  0.0001},
  { 0.3457,  0.3585}
};

const Chromaticities SONY_SGAMUT3_PRI =
{
  { 0.730,  0.280},
  { 0.140,  0.855},
  { 0.100, -0.050},
  { 0.3127,  0.3290}
};

const Chromaticities SONY_SGAMUT3_CINE_PRI =
{
  { 0.766,  0.275},
  { 0.225,  0.800},
  { 0.089, -0.087},
  { 0.3127,  0.3290}
};

// Note: No official published primaries exist as of this day for the
// Sony VENICE SGamut3 and Sony VENICE SGamut3.Cine colorspaces. The primaries
// have thus been derived from the IDT matrices.
const Chromaticities SONY_VENICE_SGAMUT3_PRI =
{
  { 0.740464264304292,  0.279364374750660},
  { 0.089241145423286,  0.893809528608105},
  { 0.110488236673827, -0.052579333080476},
  { 0.312700000000000,  0.329000000000000}
};

const Chromaticities SONY_VENICE_SGAMUT3_CINE_PRI =
{
  { 0.775901871567345,  0.274502392854799},
  { 0.188682902773355,  0.828684937020288},
  { 0.101337382499301, -0.089187517306263},
  { 0.312700000000000,  0.329000000000000}
};

const Chromaticities CANON_CGAMUT_PRI =
{
  { 0.7400,  0.2700},
  { 0.1700,  1.1400},
  { 0.0800, -0.1000},
  { 0.3127,  0.3290}
};

const Chromaticities RED_WIDEGAMUTRGB_PRI =
{
  { 0.780308,  0.304253},
  { 0.121595,  1.493994},
  { 0.095612, -0.084589},
  { 0.3127,  0.3290}
};

const Chromaticities PANASONIC_VGAMUT_PRI =
{
  { 0.730,  0.280},
  { 0.165,  0.840},
  { 0.100, -0.030},
  { 0.3127,  0.3290}
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

  float cat_matrix[3][3] = mult_f33_f33( coneRespMat, mult_f33_f33( vkMat, invert_f33( coneRespMat ) ) );

  return cat_matrix;
}



float[3][3] calculate_rgb_to_rgb_matrix
  ( Chromaticities SOURCE_PRIMARIES,
    Chromaticities DEST_PRIMARIES,
    float coneRespMat[3][3] = CONE_RESP_MAT_BRADFORD
  )
{
  //
  // Calculates and returns a 3x3 RGB-to-RGB matrix from the source primaries to the 
  // destination primaries. The returned matrix is effectively a concatenation of a 
  // conversion of the source RGB values into CIE XYZ tristimulus values, conversion to
  // cone response values or other space in which reconciliation of the encoding white is 
  // done, a conversion back to CIE XYZ tristimulus values, and finally conversion from 
  // CIE XYZ tristimulus values to the destination RGB values.
  //
  // By default, coneRespMat is set to CONE_RESP_MAT_BRADFORD. 
  // The default coneRespMat can be overridden at runtime. 
  //

  const float RGBtoXYZ_44[4][4] = RGBtoXYZ( SOURCE_PRIMARIES, 1.0);
  const float RGBtoXYZ_MAT[3][3] = 
    { {RGBtoXYZ_44[0][0], RGBtoXYZ_44[0][1], RGBtoXYZ_44[0][2]}, 
      {RGBtoXYZ_44[1][0], RGBtoXYZ_44[1][1], RGBtoXYZ_44[1][2]}, 
      {RGBtoXYZ_44[2][0], RGBtoXYZ_44[2][1], RGBtoXYZ_44[2][2]} };

  // Chromatic adaptation from source white to destination white chromaticity
  // Bradford cone response matrix is the default method
  const float CAT[3][3] = calculate_cat_matrix( SOURCE_PRIMARIES.white, 
                                                DEST_PRIMARIES.white,
                                                coneRespMat );

  const float XYZtoRGB_44[4][4] = XYZtoRGB( DEST_PRIMARIES, 1.0);
  const float XYZtoRGB_MAT[3][3] = 
    { {XYZtoRGB_44[0][0], XYZtoRGB_44[0][1], XYZtoRGB_44[0][2]}, 
      {XYZtoRGB_44[1][0], XYZtoRGB_44[1][1], XYZtoRGB_44[1][2]}, 
      {XYZtoRGB_44[2][0], XYZtoRGB_44[2][1], XYZtoRGB_44[2][2]}};

return mult_f33_f33( RGBtoXYZ_MAT, mult_f33_f33( CAT, XYZtoRGB_MAT));
  return mult_f33_f33( RGBtoXYZ_MAT, mult_f33_f33( CAT, XYZtoRGB_MAT));
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

float[3] moncurve_f_f3( float x[3], float gamma, float offs)
{
    float y[3];
    y[0] = moncurve_f( x[0], gamma, offs);
    y[1] = moncurve_f( x[1], gamma, offs);
    y[2] = moncurve_f( x[2], gamma, offs);
    return y;
}

float[3] moncurve_r_f3( float y[3], float gamma, float offs)
{
    float x[3];
    x[0] = moncurve_r( y[0], gamma, offs);
    x[1] = moncurve_r( y[1], gamma, offs);
    x[2] = moncurve_r( y[2], gamma, offs);
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

float[3] bt1886_f_f3( float V[3], float gamma, float Lw, float Lb)
{
    float L[3];
    L[0] = bt1886_f( V[0], gamma, Lw, Lb);
    L[1] = bt1886_f( V[1], gamma, Lw, Lb);
    L[2] = bt1886_f( V[2], gamma, Lw, Lb);
    return L;
}

float[3] bt1886_r_f3( float L[3], float gamma, float Lw, float Lb)
{
    float V[3];
    V[0] = bt1886_r( L[0], gamma, Lw, Lb);
    V[1] = bt1886_r( L[1], gamma, Lw, Lb);
    V[2] = bt1886_r( L[2], gamma, Lw, Lb);
    return V;
}

// SMPTE Range vs Full Range scaling formulas
float smpteRange_to_fullRange( float in)
{
    const float REFBLACK = (  64. / 1023.);
    const float REFWHITE = ( 940. / 1023.);

    return (( in - REFBLACK) / ( REFWHITE - REFBLACK));
}

float fullRange_to_smpteRange( float in)
{
    const float REFBLACK = (  64. / 1023.);
    const float REFWHITE = ( 940. / 1023.);

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


// SMPTE 431-2 defines the DCDM color encoding equations. 
// The equations for the decoding of the encoded color information are the 
// inverse of the encoding equations
// Note: Here the 4095 12-bit scalar is not used since the output of CTL is 0-1.
float[3] dcdm_decode( float XYZp[3])
{
    float XYZ[3];
    XYZ[0] = (52.37/48.0) * pow( XYZp[0], 2.6);  
    XYZ[1] = (52.37/48.0) * pow( XYZp[1], 2.6);  
    XYZ[2] = (52.37/48.0) * pow( XYZp[2], 2.6);  

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



// Base functions from SMPTE ST 2084-2014

// Constants from SMPTE ST 2084-2014
const float pq_m1 = 0.1593017578125; // ( 2610.0 / 4096.0 ) / 4.0;
const float pq_m2 = 78.84375; // ( 2523.0 / 4096.0 ) * 128.0;
const float pq_c1 = 0.8359375; // 3424.0 / 4096.0 or pq_c3 - pq_c2 + 1.0;
const float pq_c2 = 18.8515625; // ( 2413.0 / 4096.0 ) * 32.0;
const float pq_c3 = 18.6875; // ( 2392.0 / 4096.0 ) * 32.0;

const float pq_C = 10000.0;

// Converts from the non-linear perceptually quantized space to linear cd/m^2
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float ST2084_2_Y( float N )
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this assumes full range (0 - 1)
  float Np = pow( N, 1.0 / pq_m2 );
  float L = Np - pq_c1;
  if ( L < 0.0 )
    L = 0.0;
  L = L / ( pq_c2 - pq_c3 * Np );
  L = pow( L, 1.0 / pq_m1 );
  return L * pq_C; // returns cd/m^2
}

// Converts from linear cd/m^2 to the non-linear perceptually quantized space
// Note that this is in float, and assumes normalization from 0 - 1
// (0 - pq_C for linear) and does not handle the integer coding in the Annex 
// sections of SMPTE ST 2084-2014
float Y_2_ST2084( float C )
//pq_r
{
  // Note that this does NOT handle any of the signal range
  // considerations from 2084 - this returns full range (0 - 1)
  float L = C / pq_C;
  float Lm = pow( L, pq_m1 );
  float N = ( pq_c1 + pq_c2 * Lm ) / ( 1.0 + pq_c3 * Lm );
  N = pow( N, pq_m2 );
  return N;
}

float[3] Y_2_ST2084_f3( float in[3])
{
  // converts from linear cd/m^2 to PQ code values
  
  float out[3];
  out[0] = Y_2_ST2084( in[0]);
  out[1] = Y_2_ST2084( in[1]);
  out[2] = Y_2_ST2084( in[2]);

  return out;
}

float[3] ST2084_2_Y_f3( float in[3])
{
  // converts from PQ code values to linear cd/m^2
  
  float out[3];
  out[0] = ST2084_2_Y( in[0]);
  out[1] = ST2084_2_Y( in[1]);
  out[2] = ST2084_2_Y( in[2]);

  return out;
}


// Conversion of PQ signal to HLG, as detailed in Section 7 of ITU-R BT.2390-0
float[3] ST2084_2_HLG_1000nits_f3( float PQ[3]) 
{
    // ST.2084 EOTF (non-linear PQ to display light)
    float displayLinear[3] = ST2084_2_Y_f3( PQ);

    // HLG Inverse EOTF (i.e. HLG inverse OOTF followed by the HLG OETF)
    // HLG Inverse OOTF (display linear to scene linear)
    float Y_d = 0.2627*displayLinear[0] + 0.6780*displayLinear[1] + 0.0593*displayLinear[2];
    const float L_w = 1000.;
    const float L_b = 0.;
    const float alpha = (L_w-L_b);
    const float beta = L_b;
    const float gamma = 1.2;
    
    float sceneLinear[3];
    if (Y_d == 0.) { 
        /* This case is to protect against pow(0,-N)=Inf error. The ITU document
        does not offer a recommendation for this corner case. There may be a 
        better way to handle this, but for now, this works. 
        */ 
        sceneLinear[0] = 0.;
        sceneLinear[1] = 0.;
        sceneLinear[2] = 0.;        
    } else {
        sceneLinear[0] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[0]-beta)/alpha);
        sceneLinear[1] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[1]-beta)/alpha);
        sceneLinear[2] = pow( (Y_d-beta)/alpha, (1.-gamma)/gamma) * ((displayLinear[2]-beta)/alpha);
    }

    // HLG OETF (scene linear to non-linear signal value)
    const float a = 0.17883277;
    const float b = 0.28466892; // 1.-4.*a;
    const float c = 0.55991073; // 0.5-a*log(4.*a);

    float HLG[3];
    if (sceneLinear[0] <= 1./12) {
        HLG[0] = sqrt(3.*sceneLinear[0]);
    } else {
        HLG[0] = a*log(12.*sceneLinear[0]-b)+c;
    }
    if (sceneLinear[1] <= 1./12) {
        HLG[1] = sqrt(3.*sceneLinear[1]);
    } else {
        HLG[1] = a*log(12.*sceneLinear[1]-b)+c;
    }
    if (sceneLinear[2] <= 1./12) {
        HLG[2] = sqrt(3.*sceneLinear[2]);
    } else {
        HLG[2] = a*log(12.*sceneLinear[2]-b)+c;
    }

    return HLG;
}


// Conversion of HLG to PQ signal, as detailed in Section 7 of ITU-R BT.2390-0
float[3] HLG_2_ST2084_1000nits_f3( float HLG[3]) 
{
    const float a = 0.17883277;
    const float b = 0.28466892; // 1.-4.*a;
    const float c = 0.55991073; // 0.5-a*log(4.*a);

    const float L_w = 1000.;
    const float L_b = 0.;
    const float alpha = (L_w-L_b);
    const float beta = L_b;
    const float gamma = 1.2;

    // HLG EOTF (non-linear signal value to display linear)
    // HLG to scene-linear
    float sceneLinear[3];
    if ( HLG[0] >= 0. && HLG[0] <= 0.5) {
        sceneLinear[0] = pow(HLG[0],2.)/3.;
    } else {
        sceneLinear[0] = (exp((HLG[0]-c)/a)+b)/12.;
    }        
    if ( HLG[1] >= 0. && HLG[1] <= 0.5) {
        sceneLinear[1] = pow(HLG[1],2.)/3.;
    } else {
        sceneLinear[1] = (exp((HLG[1]-c)/a)+b)/12.;
    }        
    if ( HLG[2] >= 0. && HLG[2] <= 0.5) {
        sceneLinear[2] = pow(HLG[2],2.)/3.;
    } else {
        sceneLinear[2] = (exp((HLG[2]-c)/a)+b)/12.;
    }        
    
    float Y_s = 0.2627*sceneLinear[0] + 0.6780*sceneLinear[1] + 0.0593*sceneLinear[2];

    // Scene-linear to display-linear
    float displayLinear[3];
    displayLinear[0] = alpha * pow( Y_s, gamma-1.) * sceneLinear[0] + beta;
    displayLinear[1] = alpha * pow( Y_s, gamma-1.) * sceneLinear[1] + beta;
    displayLinear[2] = alpha * pow( Y_s, gamma-1.) * sceneLinear[2] + beta;
        
    // ST.2084 Inverse EOTF
    float PQ[3] = Y_2_ST2084_f3( displayLinear);

    return PQ;
}