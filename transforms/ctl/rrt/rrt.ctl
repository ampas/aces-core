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
// Reference Rendering Transform (RRT)
//
// The transform consists of four main steps:
//	- a rendering curve (via a b-spline)
//	- 3x3 matrix
//	- per-hue contrast adjustments with blending between hue regions
//	- desaturation
//





import "utilities";
import "splines";





/* ============ RRT CONSTANTS ============ */
const float RRT_MTX[3][3] = {         			
	{ 0.926, 0.130, -0.035},
	{ 0.038, 0.800, -0.035},
	{ 0.036, 0.070,  1.070}  
};

const float BASE_HALF_WIDTH = 60.0; // In degrees
const float BASE_HALF_WIDTH_RADIANS = BASE_HALF_WIDTH * (M_PI/180.0);

const float R_LUT[11][2] = { // affects red hues
{0.0000002540, 0.0000002540},{0.0000008970, 0.0000008970},{0.0000031703, 0.0000039912},{0.0001645508, 0.0002926173},
{0.0026896762, 0.0053666096},{0.0347936508, 0.0694224602},{0.2590001407, 0.2590001407},{1.0000000000, 0.5623413252},
{3.9856577231, 2.8216295831},{13.9990962657, 13.9990962657},{49.1699764178, 49.1699764178}
}; 

const float Y_LUT[9][2] = { // affects yellow hues
{0.0000002358, 0.0000002358},{0.0000008327, 0.0000008327},{0.0000029424, 0.0000029424},
{0.0001527566, 0.0001527566},{0.0322923760, 0.0406536927},{0.9282607769, 1.0540713276},
{3.6994741752, 5.3296660801},{12.9957096566, 16.0657128371},{45.6457134961, 59.4155333542}
}; 

const float G_LUT[9][2] = { // affects green hues
{0.0000001912, 0.0000001912},{0.0000006753, 0.0000006753},{0.0000023862, 0.0000023862},
{0.0001238797, 0.0001238797},{0.0261938900, 0.0261938900},{0.7528006753, 0.7528006753},
{3.0002676549, 3.0002676549},{10.5390144507, 10.5390144507},{37.0168961033, 37.0168961033} 
}; // unity

const float C_LUT[9][2] = { // affects cyan hues
{0.0000001912, 0.0000001912},{0.0000006753, 0.0000006753},{0.0000023862, 0.0000023862},
{0.0001238797, 0.0001238797},{0.0261938900, 0.0261938900},{0.7528006753, 0.7528006753},
{3.0002676549, 3.0002676549},{10.5390144507, 10.5390144507},{37.0168961033, 37.0168961033}
}; // unity

const float B_LUT[11][2] = { // affects blue hues
{0.0000002540, 0.0000002540},{0.0000008970, 0.0000008970},{0.0000031703, 0.0000031703},
{0.0001645508, 0.0001645508},{0.0026896762, 0.0026896762},{0.0347936508, 0.0347936508},
{0.2590001407, 0.2590001407},{1.0000000000, 1.0000000000},{3.9856577231, 1.2603756379},
{13.9990962657, 2.2187072372},{49.1699764178, 3.1024157749}
};

const float M_LUT[9][2] = { // affects magenta hues
{0.0000001912, 0.0000001912},{0.0000006753, 0.0000006753},{0.0000023862, 0.0000023862},
{0.0001238797, 0.0001238797},{0.0261938900, 0.0261938900},{0.7528006753, 0.7528006753},
{3.0002676549, 3.0002676549},{10.5390144507, 10.5390144507},{37.0168961033, 37.0168961033}
}; //unity

const float R_MTX[3][3] = {
	{1.0, -0.1, -0.1},
	{0.0, 1.2, -0.1},
	{0.0, -0.1, 1.2}
};

const float RGB_TO_Y[3] = { 1./3. , 1./3., 1./3.};
	
const float SAT = 0.86; 





/* ============ RRT SUBFUNCTIONS ============ */

// ================================================================= //
void determine_channel_order
	(input varying float rgb[3],
	 output varying float low,
	 output varying float mid,
	 output varying float hgh)
{
 	// 
	// This function returns the smallest, middle, and largest value
	// of an input RGB triplet. If the high is zero, this implies
	// that all three values are <= zero and will return a very tiny
	// number to avoid a divide by zero.
	//
	
	low = min( min( rgb[0], rgb[1]), rgb[2]);
	mid = max( max( min( rgb[0], rgb[1]), min( rgb[0], rgb[2])), min( rgb[1], rgb[2]));
	hgh = max( max( rgb[0], rgb[1]), rgb[2]);
	hgh = max( hgh, 1e-12);
}
// ================================================================= //

// ================================================================= //
float determine_sat( varying float rgb[3] )
{
 	// 
	// This function determines the "saturation" of an input RGB triplet
	// using a ratio of minimum to maximum.
	//
	
	float min_chan = min( min( rgb[0], rgb[1]), rgb[2]);
	float max_chan = max( max( rgb[0], rgb[1]), rgb[2]);
	float mid_chan = max( max( min( rgb[0], rgb[1]), min( rgb[0], rgb[2])), min( rgb[1], rgb[2]));
	max_chan = max( max_chan, 1e-9);
	
	return ( 1.0 - ( min_chan / max_chan) );
}
// ================================================================= //

// ================================================================= //
void
	calc_sat_adjust_matrix
    (input varying float sat,
	 input varying float rgb2Y[3],
     output varying float M[3][3])
{
 	//
	// This function determines the terms for a 3x3 saturation matrix 
	// based on the luminance of the input.
	//
	
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
} 
// ================================================================= //





/* ============ RRT - Main Algorithm ============ */
void main (
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
	float aces[3] = {rIn, gIn, bIn};

	/* --- RRT Tonecurve --- */
    float rgbPostTonecurve[3];
    rgbPostTonecurve[0] = rrt_shaper_fwd(aces[0]);
    rgbPostTonecurve[1] = rrt_shaper_fwd(aces[1]);
    rgbPostTonecurve[2] = rrt_shaper_fwd(aces[2]);

		// Clip negative values
		rgbPostTonecurve[0] = max( rgbPostTonecurve[0], 0.0);
		rgbPostTonecurve[1] = max( rgbPostTonecurve[1], 0.0);
		rgbPostTonecurve[2] = max( rgbPostTonecurve[2], 0.0);
	
	/* --- RRT Matrix --- */
		float rgbPostMatrix[3] = mult_f3_f33( rgbPostTonecurve, RRT_MTX );
	
		// Clip negative values
		rgbPostMatrix[0] = max( rgbPostMatrix[0], 0.0);
		rgbPostMatrix[1] = max( rgbPostMatrix[1], 0.0);
		rgbPostMatrix[2] = max( rgbPostMatrix[2], 0.0);
	
	/* --- RRT Hue Specific Adjustments --- */
		// Determine ordering of channels
		float low;	float mid;	float hgh;
		determine_channel_order( rgbPostMatrix, low, mid, hgh);

		// Initialize variables
			float rgbPostHueProcess[3] = rgbPostMatrix;  // Duplicate of input values to hue region processing
			float rgbPostHueProcessR[3] = rgbPostHueProcess; 
			float rgbPostHueProcessY[3] = rgbPostHueProcess;
			float rgbPostHueProcessG[3] = rgbPostHueProcess; 
			float rgbPostHueProcessC[3] = rgbPostHueProcess;
			float rgbPostHueProcessB[3] = rgbPostHueProcess; 
			float rgbPostHueProcessM[3] = rgbPostHueProcess;
			
			float intensR = 0.0; float intensY = 0.0; float intensG = 0.0;
			float intensC = 0.0; float intensB = 0.0; float intensM = 0.0;
	
			float lutIn[3] = rgbPostMatrix; // Unity value
			
			float hueAngleDegrees = -1.0;
			
			float amtR = 0.0; float amtY = 0.0; float amtG = 0.0;
			float amtC = 0.0; float amtB = 0.0; float amtM = 0.0;

			float sat = determine_sat( rgbPostMatrix);			
	
		// Find the hue angle of the color
		if (( hgh == rgbPostMatrix[0]) && ( mid == rgbPostMatrix[1]) && ( low == rgbPostMatrix[2]) && ( low !=hgh)) // Red-yellow
			hueAngleDegrees = 60.0 * ( ( mid - low) / ( hgh - low)); 
		if (( hgh == rgbPostMatrix[1]) && ( mid == rgbPostMatrix[0]) && ( low == rgbPostMatrix[2]) && ( low != hgh)) // Yellow-green
			hueAngleDegrees = 60.0 * (2.0 - ( mid - low) / ( hgh - low));	
		if (( hgh == rgbPostMatrix[1]) && ( mid == rgbPostMatrix[2]) && ( low == rgbPostMatrix[0]) && ( low != hgh)) // Green-cyan
			hueAngleDegrees = 60.0 * (2.0 + ( mid - low) / ( hgh - low)); 
		if (( hgh == rgbPostMatrix[2]) && ( mid == rgbPostMatrix[1]) && ( low == rgbPostMatrix[0]) && ( low != hgh)) // Cyan-blue
			hueAngleDegrees = 60.0 * (4.0 - ( mid - low) / ( hgh - low)); 
		if (( hgh == rgbPostMatrix[2]) && ( mid == rgbPostMatrix[0]) && ( low == rgbPostMatrix[1]) && ( low != hgh)) // Blue-magenta
			hueAngleDegrees = 60.0 * (4.0 + ( mid - low) / ( hgh - low)); 
		if (( hgh == rgbPostMatrix[0]) && ( mid == rgbPostMatrix[2]) && ( low == rgbPostMatrix[1]) && ( low != hgh)) // Magenta-red
			hueAngleDegrees = 60.0 * (6.0 - ( mid - low) / ( hgh - low)); 
		// else: rgbo is a neutral and will maintain initialized hueAngleDegrees = -1.0;
		float hueAngleRadians = hueAngleDegrees * (M_PI/180.0);

		// RED - centered at 360 / 0 degrees or 0 / 2*M_PI		
		if (( ( hueAngleDegrees > (360.0-BASE_HALF_WIDTH) ) && ( hueAngleDegrees <= 360.0 )) || (( hueAngleDegrees >= 0.0 ) && ( hueAngleDegrees < BASE_HALF_WIDTH ))) {
			
			float R[3];
			R = mult_f3_f33( rgbPostMatrix, R_MTX );
			
			intensR = 0.5* cos( (hueAngleRadians - (BASE_HALF_WIDTH_RADIANS + (0.0/3.0*M_PI-BASE_HALF_WIDTH_RADIANS))) * (M_PI/BASE_HALF_WIDTH_RADIANS) ) + 0.5;
			amtR = sat * intensR;

			float lutOutR[3];
			ratio_preserving_tonecurve_lut1D( R_LUT, R, lutOutR );
						
			rgbPostHueProcessR[0] = amtR * ( lutOutR[0] - lutIn[0]) + lutIn[0];
			rgbPostHueProcessR[1] = amtR * ( lutOutR[1] - lutIn[1]) + lutIn[1];
			rgbPostHueProcessR[2] = amtR * ( lutOutR[2] - lutIn[2]) + lutIn[2];					
			}

		// YELLOW - centered at 60 degrees or M_PI/3.0
		if ( (hueAngleDegrees > (60.0-BASE_HALF_WIDTH) ) && (hueAngleDegrees <= (60.0+BASE_HALF_WIDTH) ) ) {

			intensY = 0.5* cos( (hueAngleRadians - (BASE_HALF_WIDTH_RADIANS + (1.0/3.0*M_PI-BASE_HALF_WIDTH_RADIANS))) * (M_PI/BASE_HALF_WIDTH_RADIANS) ) + 0.5;
			amtY = sat * intensY;

			float lutOutY[3];
			ratio_preserving_tonecurve_lut1D( Y_LUT, rgbPostMatrix, lutOutY );
				
			rgbPostHueProcessY[0] = amtY * ( lutOutY[0] - lutIn[0]) + lutIn[0];
			rgbPostHueProcessY[1] = amtY * ( lutOutY[1] - lutIn[1]) + lutIn[1];
			rgbPostHueProcessY[2] = amtY * ( lutOutY[2] - lutIn[2]) + lutIn[2];					
			}
			
		// GREEN - centered at 120 degrees or 2.0*M_PI/3.0			
		if ((hueAngleDegrees > (120.0-BASE_HALF_WIDTH)) && (hueAngleDegrees <= (120.0+BASE_HALF_WIDTH))) {

			intensG = 0.5* cos( (hueAngleRadians - (BASE_HALF_WIDTH_RADIANS + (2.0/3.0*M_PI-BASE_HALF_WIDTH_RADIANS))) * (M_PI/BASE_HALF_WIDTH_RADIANS) ) + 0.5;
			amtG = sat * intensG;

			float lutOutG[3];
			ratio_preserving_tonecurve_lut1D( G_LUT, rgbPostMatrix, lutOutG );
				
			rgbPostHueProcessG[0] = amtG * ( lutOutG[0] - lutIn[0]) + lutIn[0];
			rgbPostHueProcessG[1] = amtG * ( lutOutG[1] - lutIn[1]) + lutIn[1];
			rgbPostHueProcessG[2] = amtG * ( lutOutG[2] - lutIn[2]) + lutIn[2];					
			}

		// CYAN - centered at 180 degrees or M_PI			
		if ((hueAngleDegrees > (180.0-BASE_HALF_WIDTH)) && (hueAngleDegrees <= (180.0+BASE_HALF_WIDTH))) {
			
			intensC = 0.5* cos( (hueAngleRadians - (BASE_HALF_WIDTH_RADIANS + (3.0/3.0*M_PI-BASE_HALF_WIDTH_RADIANS))) * (M_PI/BASE_HALF_WIDTH_RADIANS) ) + 0.5;
			amtC = sat * intensC;

			float lutOutC[3];
			ratio_preserving_tonecurve_lut1D( C_LUT, rgbPostMatrix, lutOutC );
				
			rgbPostHueProcessC[0] = amtC * ( lutOutC[0] - lutIn[0]) + lutIn[0];
			rgbPostHueProcessC[1] = amtC * ( lutOutC[1] - lutIn[1]) + lutIn[1];
			rgbPostHueProcessC[2] = amtC * ( lutOutC[2] - lutIn[2]) + lutIn[2];		
 			}

		// BLUE - centered at 270 degrees or 4.0*M_PI/3.0 			
		if ((hueAngleDegrees > (240.0-BASE_HALF_WIDTH)) && (hueAngleDegrees <= (240.0+BASE_HALF_WIDTH))) {
			
			intensB = 0.5* cos( (hueAngleRadians - (BASE_HALF_WIDTH_RADIANS + (4.0/3.0*M_PI-BASE_HALF_WIDTH_RADIANS))) * (M_PI/BASE_HALF_WIDTH_RADIANS) ) + 0.5;
			amtB = sat * intensB;

			float lutOutB[3];
			ratio_preserving_tonecurve_lut1D( B_LUT, rgbPostMatrix, lutOutB );
				
			rgbPostHueProcessB[0] = amtB * ( lutOutB[0] - lutIn[0]) + lutIn[0];
			rgbPostHueProcessB[1] = amtB * ( lutOutB[1] - lutIn[1]) + lutIn[1];
			rgbPostHueProcessB[2] = amtB * ( lutOutB[2] - lutIn[2]) + lutIn[2];					
			}
			
		// MAGENTA - centered at 330 degrees or 5.0*M_PI/3.0
		if ((hueAngleDegrees > (300.0-BASE_HALF_WIDTH)) && (hueAngleDegrees <= (300.0+BASE_HALF_WIDTH))) {
			
			intensM = 0.5* cos( (hueAngleRadians - (BASE_HALF_WIDTH_RADIANS + (5.0/3.0*M_PI-BASE_HALF_WIDTH_RADIANS))) * (M_PI/BASE_HALF_WIDTH_RADIANS) ) + 0.5;
			amtM = sat * intensM;

			float lutOutM[3];
			ratio_preserving_tonecurve_lut1D( M_LUT, rgbPostMatrix, lutOutM );
				
			// Linearly interpolate between the two "endpoint"-LUTs to get the processed value.
			rgbPostHueProcessM[0] = amtM * (lutOutM[0] - lutIn[0]) + lutIn[0];
			rgbPostHueProcessM[1] = amtM * (lutOutM[1] - lutIn[1]) + lutIn[1];
			rgbPostHueProcessM[2] = amtM * (lutOutM[2] - lutIn[2]) + lutIn[2];					
			}

		// Hue region blending function - Applies a percentage of adjustments based on where
		// color falls in overlap between hue regions. At most, only two intens parameters should
		// be greater than or equal to zero.
		if (hueAngleDegrees >= 0.0) {
			float sumIntens = intensR + intensY + intensG + intensC + intensB + intensM;
	
			float percentR = intensR * (1.0/sumIntens);
			float percentY = intensY * (1.0/sumIntens);
			float percentG = intensG * (1.0/sumIntens);
			float percentC = intensC * (1.0/sumIntens);
			float percentB = intensB * (1.0/sumIntens);
			float percentM = intensM * (1.0/sumIntens);
					
			rgbPostHueProcess[0] = percentR * rgbPostHueProcessR[0] + 
								   percentY * rgbPostHueProcessY[0] + 
								   percentG * rgbPostHueProcessG[0] + 
								   percentC * rgbPostHueProcessC[0] +
								   percentB * rgbPostHueProcessB[0] + 
								   percentM * rgbPostHueProcessM[0];
			rgbPostHueProcess[1] = percentR * rgbPostHueProcessR[1] + 
								   percentY * rgbPostHueProcessY[1] + 
								   percentG * rgbPostHueProcessG[1] + 
								   percentC * rgbPostHueProcessC[1] +
								   percentB * rgbPostHueProcessB[1] + 
								   percentM * rgbPostHueProcessM[1];
			rgbPostHueProcess[2] = percentR * rgbPostHueProcessR[2] + 
								   percentY * rgbPostHueProcessY[2] + 
								   percentG * rgbPostHueProcessG[2] + 
								   percentC * rgbPostHueProcessC[2] +
								   percentB * rgbPostHueProcessB[2] + 
								   percentM * rgbPostHueProcessM[2];
		} else { 
			// Neutral color, so use the original value from pre-hue processing
			rgbPostHueProcess = rgbPostMatrix;
		} 
	
	/* --- RRT Saturation Adjustment --- */
		float SatMatrix[3][3];
		calc_sat_adjust_matrix( SAT, RGB_TO_Y, SatMatrix);

		float rgbPostSat[3];
		rgbPostSat = mult_f3_f33( rgbPostHueProcess, SatMatrix);

		// Clip
		rgbPostSat[0] = max( rgbPostSat[0], 0.0);
		rgbPostSat[1] = max( rgbPostSat[1], 0.0);
		rgbPostSat[2] = max( rgbPostSat[2], 0.0);

	// Output
	rOut = rgbPostSat[0];
	gOut = rgbPostSat[1];
	bOut = rgbPostSat[2];
    aOut = aIn;
}