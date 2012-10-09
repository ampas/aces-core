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
// Output Device Transform for a broadcast monitor conforming to the
// reference OETF specified in Rec. ITU-R BT.1886 and primaries specified in 
// Rec. ITU-R BT.709.
//
// The assumed observer adapted white point shall be the chromaticity coordinates 
// specified for white in Rec. ITU-R BT.709.
//
// This transform shall be used with a device calibrated to match the primaries
// specified Rec. ITU-R BT.709 and reference electro-optical transfer function 
// specified in Rec. ITU-R BT.1886 where L = 100 cd/m^2 Lw = 1.0 and Lb = 0.0 .
//
// Viewing Environment:
// 		Environment specified in SMPTE RP 431-2-2007
//		Note: This environement is consistent with the viewing environment typical
//		of a motion picture theater.  This ODT makes no attempt to compensate for 
//		viewing environment variables more typical of those associated with the home.
//
// By default this transform outputs full range code values. If smpte (legal) range code
// values are desired the default can be overridden at runtime by setting the input 
// variable smpteRangeOut equal to true.
//
// Example: 
// ctlrender -ctl odt_rec709.ctl -param1 smpteRangeOut 1.0 oces.exr image709legal.tif
//





import "utilities";
import "splines";





/* ============ CONSTANTS ============ */
const float SRC_WHITE[3] = {ACESChromaticities.white[0], ACESChromaticities.white[1], 1.0};
const float DEST_WHITE[3] = {rec709Chromaticities.white[0], rec709Chromaticities.white[1], 1.0};
const float CAT[3][3] = calculate_cat_matrix( SRC_WHITE, DEST_WHITE );





/* ============ ODT - Main Algorithm ============ */
void main
(  
	input varying float rIn,
	input varying float gIn,
	input varying float bIn,
	input varying float aIn,
	output varying float rOut,
	output varying float gOut,
	output varying float bOut,
	output varying float aOut,
	input uniform bool smpteRangeOut = false
)
{
	float oces[3] = {rIn, gIn, bIn};

	// RDT Tone Curve
	float rgbPostTonecurve[3];
	ratio_preserving_odt_tonecurve( oces, rgbPostTonecurve);
	
	// Convert to Rec709 primaries
		// RGB to XYZ
		float ACEStoXYZ[4][4] = RGBtoXYZ( ACESChromaticities, 1.0);
		float XYZ[3] = mult_f3_f44( rgbPostTonecurve, ACEStoXYZ);

		// Chromatic adaptation from ACES White to Assumed Observer Adapted White
		XYZ = mult_f3_f33( XYZ, CAT);

		// XYZ to Rec709
		float XYZtoRec709[4][4] = XYZtoRGB( rec709Chromaticities, 1.0);
		float RGBo[3] = mult_f3_f44( XYZ, XYZtoRec709);

	// Clip	data to range zero to one	
	RGBo[0] = clip_0_to_1( RGBo[0] );
	RGBo[1] = clip_0_to_1( RGBo[1] );
	RGBo[2] = clip_0_to_1( RGBo[2] );

	// Per ITU-R BT.1886 EOTF
	const float GAMMA = 2.4;
	const float L_B = 0.000; 	// screen luminance for black
	const float L_W = 1.0; 		// screen luminance for white
		// Note: L_B and L_W may be modified to luminance of an actual CRT.
		// Per RECOMMENDATION ITU-R BT.1886 - Appendix 1. L_B has been set to 0.0
		// targeting a CRT with a contrast ratio of 2000:1.	
	const float A = pow( pow( L_W, ( 1.0 / GAMMA)) - pow( L_B, ( 1.0 / GAMMA)), GAMMA);
	const float B = pow( L_B, ( 1.0 / GAMMA)) / ( pow( L_W, ( 1.0 / GAMMA)) - pow( L_B, (1.0/GAMMA))); 

	rOut = ( pow( RGBo[0],(1.0/GAMMA)) / A ) - B;
	gOut = ( pow( RGBo[1],(1.0/GAMMA)) / A ) - B;
	bOut = ( pow( RGBo[2],(1.0/GAMMA)) / A ) - B;	
	aOut = aIn;
	
	// Scale to SMPTE range if 
	if (smpteRangeOut == true) {
	
		float fullRange[3] = {rOut, gOut, bOut};
		float smpteRange[3] = fullRange_to_smpteRange( fullRange );
		
		rOut = smpteRange[0];
		gOut = smpteRange[1];
		bOut = smpteRange[2];	
	}	
}