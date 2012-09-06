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
// Output Device Transform for IEC 61966-2-1:1999 (sRGB)
//
// This transform shall be used with a device calibrated to match the reference computer
// monitor specified in IEC 61966-2-1:1999. 
//
// The assumed observer adapted white point shall be the chromaticity coordinates 
// specified for white in IEC 61966-2-1:1999.
//
// Viewing Environment:
// 		Environment specified in SMPTE RP 431-2-2007
//		Note: This environement is consistent with the viewing environment typical
//		of a motion picture theater.  This ODT makes no attempt to compensate for 
//		viewing environment variables specified in IEC 61966-2-1:1999 typcially found
//		in a normally lit office environment.





import "utilities";
import "splines";





/* ============ CONSTANTS ============ */
const float CAT[3][3] = {
	{0.98823, -0.00569, 0.00035},
	{-0.00789, 0.99869, 0.00112},
	{0.01676, 0.00667, 1.07808}
};





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
	output varying float aOut
)
{
	float oces[3] = {rIn, gIn, bIn};

	// RDT Tone Curve
	float rgbPostTonecurve[3];
	ratio_preserving_odt_tonecurve( oces, rgbPostTonecurve);
	
	// Convert to Rec709 primaries
		// RGB to XYZ
		float ACEStoXYZ[4][4] =  RGBtoXYZ( ACESChromaticities, 1.0);
		float XYZ[3] = mult_f3_f44( rgbPostTonecurve, ACEStoXYZ);

     	// Convert to D65
     	XYZ = mult_f3_f33( XYZ, CAT);
	   
        // XYZ to Rec709
        float XYZtoRec709[4][4] =  XYZtoRGB( rec709Chromaticities, 1.0);
   		float RGBo[3] = mult_f3_f44( XYZ, XYZtoRec709);

	// Clip	data to range zero to one	
	RGBo[0] = clip_0_to_1( RGBo[0] );
	RGBo[1] = clip_0_to_1( RGBo[1] );
	RGBo[2] = clip_0_to_1( RGBo[2] );

	// Monitor curve
	const float gamma = 2.4;
	const float offset = 0.055;

	rOut = moncurve_r( RGBo[0], gamma, offset);
	gOut = moncurve_r( RGBo[1], gamma, offset);
	bOut = moncurve_r( RGBo[2], gamma, offset);
	aOut = aIn;
}