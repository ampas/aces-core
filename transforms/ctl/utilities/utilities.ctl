////////////////////////////////////////////////////////////////////////////////////// 
// License Terms for Academy Color Encoding System Components                       //
//                                                                                  //
// Academy Color Encoding System (ACES) software and tools are provided by the      //
// Academy under the following terms and conditions: A worldwide, royalty-free,     //
// non-exclusive right to copy, modify, create derivatives, and use, in source and  //
// binary forms, is hereby granted, subject to acceptance of this license.          //
// Performance of any of the aforementioned acts indicates acceptance to be bound   //
// by the following terms and conditions:                                           //
//                                                                                  //
// Copies of source code, in whole or in part, must retain the above copyright      //
// notice, this list of conditions and the Disclaimer of Warranty.                  //
//                                                                                  //
// Use in binary form must retain the copyright notice (below), this list of        //
// conditions and the Disclaimer of Warranty in the documentation and/or other      //
// materials provided with the distribution.                                        //
//                                                                                  //
// * Nothing in this license shall be deemed to grant any rights to trademarks,     //
// copyrights, patents, trade secrets or any other intellectual property of         //
// A.M.P.A.S. or any contributors, except as expressly stated herein.               //
//                                                                                  //
// * Neither the name "A.M.P.A.S." nor the name of any other contributors to this   //
// software may be used to endorse or promote products derivative of or based on    //
// this software without express prior written permission of A.M.P.A.S. or the      //
// contributors, as appropriate.                                                    //
//                                                                                  //
// * This license shall be construed pursuant to the laws of the State of           //
// California, and any disputes related thereto shall be subject to the             //
// jurisdiction of the courts therein.                                              //
//                                                                                  //
// Copyright © 2012 Academy of Motion Picture Arts and Sciences (A.M.P.A.S.).       //
// Portions contributed by others as indicated. All rights reserved.                //
//                                                                                  //
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND CONTRIBUTORS //
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,    //
// THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND //
// NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT SHALL A.M.P.A.S., OR ANY            //
// CONTRIBUTORS OR DISTRIBUTORS, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    //
// SPECIAL, EXEMPLARY, RESITUTIONARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  //
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR   //
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF           //
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE  //
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF        //
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                       //
//                                                                                  //
// WITHOUT LIMITING THE GENERALITY OF THE FOREGOING, THE ACADEMY SPECIFICALLY       //
// DISCLAIMS ANY REPRESENTATIONS OR WARRANTIES WHATSOEVER RELATED TO PATENT OR      //
// OTHER INTELLECTUAL PROPERTY RIGHTS IN THE ACADEMY COLOR ENCODING SYSTEM, OR      //
// APPLICATIONS THEREOF, HELD BY PARTIES OTHER THAN A.M.P.A.S.,WHETHER DISCLOSED OR //
// UNDISCLOSED.                                                                     //
//////////////////////////////////////////////////////////////////////////////////////   


//
// Utilities.ctl
//
// Data and functions that may be useful for writing CTL programs.
//

import "splines";

const Chromaticities ACESChromaticities =
{
	{ 0.73470,  0.26530},
	{ 0.00000,  1.00000},
	{ 0.00010, -0.07700},
	{ 0.32168,  0.33767}
};

const Chromaticities rec709Chromaticities =
{
    { 0.64000,  0.33000},
    { 0.30000,  0.60000},
    { 0.15000,  0.06000},
    { 0.31270,  0.32900}
};

const Chromaticities P3D60Chromaticities =
{
	{ 0.68000,  0.32000},
	{ 0.26500,  0.69000},
	{ 0.15000,  0.06000},
	{ 0.32168,  0.33767}
};

const Chromaticities P3DCIChromaticities =
{
	{ 0.68000,  0.32000},
	{ 0.26500,  0.69000},
	{ 0.15000,  0.06000},
	{ 0.31400,  0.35100}
};

float[3]
convertRGBtoXYZ
    (Chromaticities chromaticities,
     float whiteLuminance,
     float R,
     float G,
     float B)
{
    float M[4][4] = RGBtoXYZ (chromaticities, whiteLuminance);
    float RGB[3] = {R, G, B};
    return mult_f3_f44 (RGB, M);
}

void
convertRGBtoXYZ_f
    (Chromaticities chromaticities,
     float whiteLuminance,
     float RGB[3],
     output float X,
     output float Y,
     output float Z)
{
    float M[4][4] = RGBtoXYZ (chromaticities, whiteLuminance);
    float XYZ[3] = mult_f3_f44 (RGB, M);
    X = XYZ[0];
    Y = XYZ[1];
    Z = XYZ[2];
}

void
convertRGBtoXYZ_h
    (Chromaticities chromaticities,
     float whiteLuminance,
     float RGB[3],
     output half X,
     output half Y,
     output half Z)
{
    float M[4][4] = RGBtoXYZ (chromaticities, whiteLuminance);
    float XYZ[3] = mult_f3_f44 (RGB, M);
    X = XYZ[0];
    Y = XYZ[1];
    Z = XYZ[2];
}

void
convertXYZtoRGB_f
    (Chromaticities chromaticities,
     float whiteLuminance,
     float XYZ[3],
     output float R,
     output float G,
     output float B)
{
    float M[4][4] = XYZtoRGB (chromaticities, whiteLuminance);
    float RGB[3] = mult_f3_f44 (XYZ, M);
    R = RGB[0];
    G = RGB[1];
    B = RGB[2];
}

void
convertXYZtoRGB_h
    (Chromaticities chromaticities,
     float whiteLuminance,
     float XYZ[3],
     output half R,
     output half G,
     output half B)
{
    float M[4][4] = XYZtoRGB (chromaticities, whiteLuminance);
    float RGB[3] = mult_f3_f44 (XYZ, M);
    R = RGB[0];
    G = RGB[1];
    B = RGB[2];
}

float
min
	(float a,
	 float b)
{
  if (a < b)
    return a;
  else
    return b;
}

float max
	(float a,
	 float b)
{
  if (a > b)
    return a;
  else
    return b;
}

float
clip
	(float v)
{
  return min(v, 1.0);
}

float
clip_0_to_1
	(float v)
{
	float w;
	if (v > 1.0) {
		w = 1.0; 
	} else if (v < 0.0) {
		w = 0.0;
	} else {
		w = v;
	}
	return w;
}

float 
moncurve_f(
   float x, 
   float gamma,
   float offs
)
{
	//
	// Forward monitor curve
	//
	
   float y;
   const float fs = (( gamma - 1.0) / offs) * pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
   const float xb = offs / ( gamma - 1.0);
   if ( x >= xb) 
      y = pow( ( x + offs) / ( 1.0 + offs), gamma);
   else
      y = x * fs;
   return y;
}

float
moncurve_r(
   float y, 
   float gamma,
   float offs
)
{
	//
	// Reverse monitor curve
	//

   float x;
   const float yb = pow( offs * gamma / ( ( gamma - 1.0) * ( 1.0 + offs)), gamma);
   const float rs = pow( ( gamma - 1.0) / offs, gamma - 1.0) * pow( ( 1.0 + offs) / gamma, gamma);
   if ( y >= yb) 
      x = ( 1.0 + offs) * pow( y, 1.0 / gamma) - offs;
   else
      x = y * rs;
   return x;
}

void 
ratio_preserving_tonecurve_lut1D(
	float lut[][], 
	float rgb[3],
	output float rgbOut[3] 
)
{
	//
	// The "ratio preserving tonecurve" is used to avoid hue/chroma shifts. 
	// It sends a norm value through a 1D-LUT and scales the RGB values based on the output.
	//
	
	const float NTH_POWER = 2.0;

	float normRGB = ( pow(rgb[0],NTH_POWER) + pow(rgb[1],NTH_POWER) + pow(rgb[2],NTH_POWER) ) /
					( pow(rgb[0],NTH_POWER-1) + pow(rgb[1],NTH_POWER-1) + pow(rgb[2],NTH_POWER-1) );
	normRGB = max( normRGB, 1e-12);
	
	float normRGBo = interpolate1D( lut, normRGB );

	rgbOut[0] = rgb[0] * normRGBo / normRGB;
	rgbOut[1] = rgb[1] * normRGBo / normRGB;
	rgbOut[2] = rgb[2] * normRGBo / normRGB;	
}

float determine_inv_sat( varying float rgb[3] , varying float norm)
{
	float min_chan = min( min( rgb[0], rgb[1]), rgb[2]);
	float max_chan = max( max( rgb[0], rgb[1]), rgb[2]);

	// When norm = mx, inv_sat = mn / mx.
	float inv_sat = 1.0 - ( max_chan - min_chan) / max( norm, 1e-8);

	// This is near 0 for saturated colors and 1 for neutrals.
	return inv_sat;
}

void ratio_preserving_odt_tonecurve
( 	input varying float rgbIn[3],
	output varying float rgbOut[3] 
)
{
	// New norm is just max( RGB).
	float normRGB = max( max( rgbIn[0], rgbIn[1]), rgbIn[2]);
	normRGB = max( normRGB, 1e-8);  // avoid div by 0 below

	float normRGBo = rdt_shaper_fwd( normRGB );

	// Apply ratio-preserving tone-curve.
	rgbOut[0] = ( normRGBo / normRGB) * rgbIn[0];
	rgbOut[1] = ( normRGBo / normRGB) * rgbIn[1];
	rgbOut[2] = ( normRGBo / normRGB) * rgbIn[2];

	// Note: rgbOut is roughly [0,1] since it is post tone-curve.
	float inv_sat = determine_inv_sat( rgbOut, normRGBo);

	// Note: Using pow reduces the rate of desaturation.
	float d = inv_sat * pow( normRGBo, 3.);

	// Desaturate low-chroma highlights towards a neutral with the same norm.
	rgbOut[0] = ( 1. - d) * rgbOut[0] + d * normRGBo;
	rgbOut[1] = ( 1. - d) * rgbOut[1] + d * normRGBo;
	rgbOut[2] = ( 1. - d) * rgbOut[2] + d * normRGBo;
}

float[3] smpteRange_to_fullRange( varying float rgbIn[3] )
{ 
	const float REFWHITE = ( 940.0 / 1023.0);
	const float REFBLACK = (  64.0 / 1023.0);
	
	float rgbOut[3];
	rgbOut[0] = ( rgbIn[0] - REFBLACK) / ( REFWHITE - REFBLACK);
	rgbOut[1] = ( rgbIn[1] - REFBLACK) / ( REFWHITE - REFBLACK);
	rgbOut[2] = ( rgbIn[2] - REFBLACK) / ( REFWHITE - REFBLACK);
	
	return rgbOut;
}

float[3] fullRange_to_smpteRange( varying float rgbIn[3] )
{ 
	const float REFWHITE = ( 940.0 / 1023.0);
	const float REFBLACK = (  64.0 / 1023.0);
	
	float rgbOut[3];
	rgbOut[0] = rgbIn[0]  * ( REFWHITE - REFBLACK) + REFBLACK;
	rgbOut[1] = rgbIn[1]  * ( REFWHITE - REFBLACK) + REFBLACK;
	rgbOut[2] = rgbIn[2]  * ( REFWHITE - REFBLACK) + REFBLACK;
	
	return rgbOut;
}

float[3] xyY_2_XYZt( float xyY[3] )
{

	float z = 1-xyY[0]-xyY[1];
	
	float XYZt[3];
	XYZt[0] = xyY[0] / xyY[1] * xyY[2];
	XYZt[1] = xyY[2];
	XYZt[2] = z / xyY[1] * xyY[2];
	
	return XYZt;
	
}


const float coneRespMatCAT02[3][3] = {
{0.73280, -0.70360, 0.00300},
{0.42960,  1.69750, 0.01360},
{-0.16240, 0.00610, 0.98340}
};

const float coneRespMatBrad[3][3] = {
{0.40024, -0.22630, 0.00000},
{0.70760, 1.16532, 0.00000},
{-0.08081, 0.04570, 0.91822}
};


float[3][3] calculate_cat_matrix( float src_xyY[3], 
								  float des_xyY[3], 
								  float coneRespMat[3][3] = coneRespMatCAT02)
								  
{

	float src_XYZt[3] = xyY_2_XYZt( src_xyY );
	float des_XYZt[3] = xyY_2_XYZt( des_xyY );
	
	float src_coneResp[3] = mult_f3_f33( src_XYZt, coneRespMat);
	float des_coneResp[3] = mult_f3_f33( des_XYZt, coneRespMat);

	float vkMat[3][3] = {
	{ des_coneResp[0] / src_coneResp[0], 0.0, 0.0 },
	{ 0.0, des_coneResp[1] / src_coneResp[1], 0.0 },
	{ 0.0, 0.0, des_coneResp[2] / src_coneResp[2] }
	};
		
	float cat_matrix[3][3] = mult_f33_f33( coneRespMat, transpose_f33( mult_f33_f33( transpose_f33( invert_f33( coneRespMat ) ), vkMat ) ) );
	
	return cat_matrix;
	
}

