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
// splines.ctl
//
// Code for RRT and ODT tonecurves.
//


// === RRT Spline === //
//
// Algorithm for applying RRT tone curve in forward direction.
//
// 		vers 1.0 by Doug Walker  	2012-01-23
// 	 	modified by Scott Dyer		2012-02-28

// Input and output are in linear (not log) units.
float rrt_shaper_fwd( varying float x)
{

	// Parameters controlling extrapolation
	const float SLOPE = -0.55; // "units" of -log10(OCES)/log10(ACES)
	const float NEG_SLOPE = -0.1;
	
	// The knots are in units of log10(ACES).
	const float KNOT_START = -3.0;
	const float KNOT_END = 0.5;
	const unsigned int KNOT_LEN = 8;
	const float KNOT_DEL = ( KNOT_END - KNOT_START) / ( KNOT_LEN - 1);
	
	// B-spline coefficients.
	// These are in -log10(OCES) space (i.e. OCES densities).
	// These control points are located at the midpoints of each knot interval.
	const float COEFS0 =  5.0706;
	const float COEFS1 =  4.7706;
	const float COEFS2 =  4.;
	const float COEFS3 =  2.98;
	const float COEFS4 =  1.874;
	const float COEFS5 =  0.95;
	const float COEFS6 =  0.225;
	const float COEFS7 = -0.225;
	const float COEFS8 = COEFS7 + (SLOPE * KNOT_DEL); // -0.5
	// The locations of these control points in log10(ACES) space
	// are as follows (i.e. midway between the knots):
	// -3.25, -2.75, -2.25, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75
	
	// KNOT_DENS is density of the spline at the knots.
	const float KNOT_DENS[ KNOT_LEN] = {
		( COEFS0 + COEFS1) / 2.,
		( COEFS1 + COEFS2) / 2.,
		( COEFS2 + COEFS3) / 2.,
		( COEFS3 + COEFS4) / 2.,
		( COEFS4 + COEFS5) / 2.,
		( COEFS5 + COEFS6) / 2.,
		( COEFS6 + COEFS7) / 2.,
		( COEFS7 + COEFS8) / 2.
	};
	
	// Textbook monomial to basis-function conversion matrix.
	const float M[ 3][ 3] = {
		{  0.5, -1.0, 0.5 },
		{ -1.0,  1.0, 0.5 },
		{  0.5,  0.0, 0.0 }
	};


    float y;
	// Linear extrapolation in linear space for negative and very dark values.
	if ( x <= 0.0 )
		y = x * -(NEG_SLOPE);
	else {
		const float logh = log10( x);
		float dens;
	
	    // Linear extrapolation in log space for dark and light values.
		if ( logh <= KNOT_START) 			// Dark values
			dens = ( SLOPE * logh) + ( KNOT_DENS[ 0] - SLOPE * KNOT_START);	
	
		else if ( logh >= KNOT_END)			// Light values
			dens = ( SLOPE * logh) + ( KNOT_DENS[ KNOT_LEN-1] - SLOPE * KNOT_END);
	
		// For typical ACES values, apply a B-spline curve.
		else {
			float t = ( logh - KNOT_START) / KNOT_DEL;
			const int j = t;
			t = t - j;
	
			// Would like to do this:
			// float cf[ 3] = { COEFS[ j], COEFS[ j + 1], COEFS[ j + 2]};
			// or at least:
			// cf[ 0] = COEFS[ j];
			// cf[ 1] = COEFS[ j + 1];
			// cf[ 2] = COEFS[ j + 2];
			// But apparently CTL bugs prevent it, so we do the following:
			float cf[ 3];
			if ( j <= 0) {
				cf[ 0] = COEFS0;  cf[ 1] = COEFS1;  cf[ 2] = COEFS2;
			}
			else if ( j == 1) {
				cf[ 0] = COEFS1;  cf[ 1] = COEFS2;  cf[ 2] = COEFS3;
			}
			else if ( j == 2) {
				cf[ 0] = COEFS2;  cf[ 1] = COEFS3;  cf[ 2] = COEFS4;
			}
			else if ( j == 3) {
				cf[ 0] = COEFS3;  cf[ 1] = COEFS4;  cf[ 2] = COEFS5;
			}
			else if ( j == 4) {
				cf[ 0] = COEFS4;  cf[ 1] = COEFS5;  cf[ 2] = COEFS6;
			}
			else if ( j == 5) {
				cf[ 0] = COEFS5;  cf[ 1] = COEFS6;  cf[ 2] = COEFS7;
			}
			else {
				cf[ 0] = COEFS6;  cf[ 1] = COEFS7;  cf[ 2] = COEFS8;
			}
	
			const float monomials[ 3] = { t * t, t, 1. };
			dens = dot_f3_f3( monomials, mult_f3_f33( cf, M));
		}
		y = pow10( -dens);
 	}
	return y;
}
// ================================================================= //




// === ODT SPLINE === //
//
// Algorithm for applying ODT tone curve in forward direction.
//
// 		vers 1.0  Doug Walker  		2012-01-23
// 		modified by Scott Dyer		2012-02-28

// Input and output are in linear (not log) units.
float rdt_shaper_fwd( varying float x)
{

	// B-spline coefficients.
	// The units are density of the output.
	const float COEFS0 = -0.008;
	const float COEFS1 = -0.00616;
	const float COEFS2 =  0.026;
	const float COEFS3 =  0.185;
	const float COEFS4 =  0.521;
	const float COEFS5 =  0.993;
	const float COEFS6 =  1.563;
	const float COEFS7 =  2.218;
	const float COEFS8 =  2.795;
	const float COEFS9 =  3.36;
	const float COEFS10 = 4.0;   // NB: keep this less than or equal to -log10( FLARE)
	// The locations of these control points in OCES density space are:
	// -1., -0.79, -0.44, -0.01, 0.48, 1.01, 1.58, 2.18, 2.82, 3.47, 4.15, 4.85
	
	// The flare term allows the spline to more rapidly approach zero
	// while keeping the shape of the curve well-behaved in density space.
	const float FLARE = 1e-4;
	
	// The last control point is fixed to yield a specific density at the
	// end of the knot domain.
	//const float COEFS11 = 2. * ( -log10( FLARE) - 0.001) - COEFS10;
	// Note: Apparently a CTL bug prevents calling log10() here, so
	// you'll need to update this manually if you change FLARE.
	const float COEFS11 = COEFS10 + 2. * ( 4. - COEFS10);
	
	// The knots are in units of OCES density.
	const unsigned int KNOT_LEN = 11;
	const float KNOT_START = -0.9;
	const float KNOT_END = 4.484256; 
	
	// The KNOT_POW adjusts the spacing to put more knots near the toe (highlights).
	const float KNOT_POW = 1. / 1.3;
	const float OFFS = KNOT_START;
	const float SC = KNOT_END - KNOT_START;
	
	// KNOT_DENS is density of the spline at the knots.
	const float KNOT_DENS[ 11] = {  
		( COEFS0 + COEFS1) / 2.,
		( COEFS1 + COEFS2) / 2.,
		( COEFS2 + COEFS3) / 2.,
		( COEFS3 + COEFS4) / 2.,
		( COEFS4 + COEFS5) / 2.,
		( COEFS5 + COEFS6) / 2.,
		( COEFS6 + COEFS7) / 2.,
		( COEFS7 + COEFS8) / 2.,
		( COEFS8 + COEFS9) / 2.,
		( COEFS9 + COEFS10) / 2.,
		( COEFS10 + COEFS11) / 2.
	};
	
	// Parameters controlling linear extrapolation.
	const float LIGHT_SLOPE = 0.023;
	const float CROSSOVER = pow(10,-KNOT_END);
	const float REV_CROSSOVER = pow10( -KNOT_DENS[ KNOT_LEN - 1]) - FLARE;
	const float DARK_SLOPE = REV_CROSSOVER / CROSSOVER;
	
	// Textbook monomial to basis-function conversion matrix.
	const float M[ 3][ 3] = {
		{  0.5, -1.0, 0.5 },
		{ -1.0,  1.0, 0.5 },
		{  0.5,  0.0, 0.0 }
	};

    float y;
    // Linear extrapolation in linear space for negative & very dark values.
    if ( x <= CROSSOVER) 
        y = x * DARK_SLOPE;
    else {
        float in_dens = -log10( x);
        float out_dens;
        float knot_coord = ( in_dens - OFFS) / SC;

        // Linear extrapolation in log space for very light values.
        if ( knot_coord <= 0.)
            out_dens = KNOT_DENS[ 0] - ( KNOT_START - in_dens) * LIGHT_SLOPE;

        // For typical OCES values, apply a B-spline curve.
        else {
            knot_coord = ( KNOT_LEN - 1) * pow( knot_coord, KNOT_POW);
            int j = knot_coord;
            float t = knot_coord - j;

            // Would like to do this:
            //float cf[ 3] = { COEFS[ j], COEFS[ j + 1], COEFS[ j + 2]};
            // or at least:
            //cf[ 0] = COEFS[ j];
            //cf[ 1] = COEFS[ j + 1];
            //cf[ 2] = COEFS[ j + 2];
            // But apparently CTL bugs prevent it, so we do the following:
            float cf[ 3];
            if ( j <= 0) {
                cf[ 0] = COEFS0;  cf[ 1] = COEFS1;  cf[ 2] = COEFS2;
            }
            else if ( j == 1) {
                cf[ 0] = COEFS1;  cf[ 1] = COEFS2;  cf[ 2] = COEFS3;
            }
            else if ( j == 2) {
                cf[ 0] = COEFS2;  cf[ 1] = COEFS3;  cf[ 2] = COEFS4;
            }
            else if ( j == 3) {
                cf[ 0] = COEFS3;  cf[ 1] = COEFS4;  cf[ 2] = COEFS5;
            }
            else if ( j == 4) {
                cf[ 0] = COEFS4;  cf[ 1] = COEFS5;  cf[ 2] = COEFS6;
            }
            else if ( j == 5) {
                cf[ 0] = COEFS5;  cf[ 1] = COEFS6;  cf[ 2] = COEFS7;
            }
            else if ( j == 6) {
                cf[ 0] = COEFS6;  cf[ 1] = COEFS7;  cf[ 2] = COEFS8;
            }
            else if ( j == 7) {
                cf[ 0] = COEFS7;  cf[ 1] = COEFS8;  cf[ 2] = COEFS9;
            }
            else if ( j == 8) {
                cf[ 0] = COEFS8;  cf[ 1] = COEFS9;  cf[ 2] = COEFS10;
            }
            else {
                cf[ 0] = COEFS9;  cf[ 1] = COEFS10;  cf[ 2] = COEFS11;
            }

            float monomials[ 3] = { t * t, t, 1. };
            out_dens = dot_f3_f3( monomials, mult_f3_f33( cf, M));
        }
        y = pow10( -out_dens) - FLARE;
    }
    return y;
}
// ================================================================= //