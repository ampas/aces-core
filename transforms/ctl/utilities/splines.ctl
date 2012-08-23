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

// Textbook monomial to basis-function conversion matrix.
const float M[ 3][ 3] = {
	{  0.5, -1.0, 0.5 },
	{ -1.0,  1.0, 0.5 },
	{  0.5,  0.0, 0.0 }
};

// === RRT Splines === //
//
// Algorithms for applying RRT tone curve in forward and reverse direction.
//
// 		vers 1.0 by Doug Walker  	2012-01-23
// 	 	modified by Scott Dyer		2012-02-28
//

	// Parameters controlling RRT shadow and highlight extrapolation
	const float RRT_HI_SLOPE = -0.55; // "units" of -log10(OCES)/log10(ACES)
	const float RRT_LO_SLOPE = -0.55; // "units" of -log10(OCES)/log10(ACES)
	const float RRT_NEG_SLOPE = -0.1;

	// The knots are in units of log10(ACES).
	const float RRT_KNOT_START = -3.0;
	const float RRT_KNOT_END = 0.5;
	const unsigned int RRT_KNOT_LEN = 8;
	const float RRT_KNOT_DEL = ( RRT_KNOT_END - RRT_KNOT_START) / ( RRT_KNOT_LEN - 1);
	
	// B-spline coefficients.
	// These are in -log10(OCES) space (i.e. OCES densities).
	// These control points are located at the midpoints of each knot interval.
	const float RRT_COEFS0 =  5.0706;
	const float RRT_COEFS1 =  4.7706;
	const float RRT_COEFS2 =  4.;
	const float RRT_COEFS3 =  2.98;
	const float RRT_COEFS4 =  1.874;
	const float RRT_COEFS5 =  0.95;
	const float RRT_COEFS6 =  0.225;
	const float RRT_COEFS7 = -0.225;
	const float RRT_COEFS8 = RRT_COEFS7 + (RRT_HI_SLOPE * RRT_KNOT_DEL); // -0.5
	// The locations of these control points in log10(ACES) space are as follows 
	// (i.e. midway between the knots):
	// -3.25, -2.75, -2.25, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75
		
	// RRT_KNOT_DENS is the density of the spline at the knots
	const float RRT_KNOT_DENS[ RRT_KNOT_LEN] = {
		( RRT_COEFS0 + RRT_COEFS1) / 2.,
		( RRT_COEFS1 + RRT_COEFS2) / 2.,
		( RRT_COEFS2 + RRT_COEFS3) / 2.,
		( RRT_COEFS3 + RRT_COEFS4) / 2.,
		( RRT_COEFS4 + RRT_COEFS5) / 2.,
		( RRT_COEFS5 + RRT_COEFS6) / 2.,
		( RRT_COEFS6 + RRT_COEFS7) / 2.,
		( RRT_COEFS7 + RRT_COEFS8) / 2.
	};

// Input and output are in linear (not log) units.
float rrt_shaper_fwd( varying float x)
{
    float y;
	// Linear extrapolation in linear space for negative and very dark values.
	if ( x <= 0.0 )
		y = x * -(RRT_NEG_SLOPE);
	else {
		const float logh = log10( x);
		float dens;
	
	    // Linear extrapolation in log space for dark and light values.
		if ( logh <= RRT_KNOT_START) 			// Dark values
			dens = ( RRT_LO_SLOPE * logh) + ( RRT_KNOT_DENS[ 0] - RRT_LO_SLOPE * RRT_KNOT_START);		
		else if ( logh >= RRT_KNOT_END)			// Light values
			dens = ( RRT_HI_SLOPE * logh) + ( RRT_KNOT_DENS[ RRT_KNOT_LEN-1] - RRT_HI_SLOPE * RRT_KNOT_END);
		// For typical ACES values, apply a B-spline curve.
		else {
			float t = ( logh - RRT_KNOT_START) / RRT_KNOT_DEL;
			const int j = t;
			t = t - j;
	
			// Would like to do this:
			// float cf[ 3] = { RRT_COEFS[ j], RRT_COEFS[ j + 1], RRT_COEFS[ j + 2]};
			// or at least:
			// cf[ 0] = RRT_COEFS[ j];
			// cf[ 1] = RRT_COEFS[ j + 1];
			// cf[ 2] = RRT_COEFS[ j + 2];
			// But apparently CTL bugs prevent it, so we do the following:
			float cf[ 3];
			if ( j <= 0) {
				cf[ 0] = RRT_COEFS0;  cf[ 1] = RRT_COEFS1;  cf[ 2] = RRT_COEFS2;
			}
			else if ( j == 1) {
				cf[ 0] = RRT_COEFS1;  cf[ 1] = RRT_COEFS2;  cf[ 2] = RRT_COEFS3;
			}
			else if ( j == 2) {
				cf[ 0] = RRT_COEFS2;  cf[ 1] = RRT_COEFS3;  cf[ 2] = RRT_COEFS4;
			}
			else if ( j == 3) {
				cf[ 0] = RRT_COEFS3;  cf[ 1] = RRT_COEFS4;  cf[ 2] = RRT_COEFS5;
			}
			else if ( j == 4) {
				cf[ 0] = RRT_COEFS4;  cf[ 1] = RRT_COEFS5;  cf[ 2] = RRT_COEFS6;
			}
			else if ( j == 5) {
				cf[ 0] = RRT_COEFS5;  cf[ 1] = RRT_COEFS6;  cf[ 2] = RRT_COEFS7;
			}
			else {
				cf[ 0] = RRT_COEFS6;  cf[ 1] = RRT_COEFS7;  cf[ 2] = RRT_COEFS8;
			}
	
			const float monomials[ 3] = { t * t, t, 1. };
			dens = dot_f3_f3( monomials, mult_f3_f33( cf, M));
		}
		y = pow10( -dens);
 	}
	return y;
}
// ================================================================= //

// Algorithm for applying RRT tone curve in reverse direction.
//

// Input and output are in linear (not log) units.
float rrt_shaper_rev( varying float y)
{
    float x;
    // Linear extrapolation in linear space for negative & very dark values.
    if ( y <= 0.0)
        x = y / -RRT_NEG_SLOPE;
    else {
        const float dens = -log10( y);
        float logh;

        // Linear extrapolation in log space for dark and light values.
        if ( dens >= RRT_KNOT_DENS[ 0])
            logh = (dens - RRT_KNOT_DENS[0] + RRT_LO_SLOPE * RRT_KNOT_START) / RRT_LO_SLOPE;
	    else if ( dens <= RRT_KNOT_DENS[ RRT_KNOT_LEN - 1])
            logh = (dens - RRT_KNOT_DENS[ RRT_KNOT_LEN-1] + RRT_HI_SLOPE * RRT_KNOT_END) / RRT_HI_SLOPE;
        // For typical ACES values, invert the B-spline curve.
        else {
            unsigned int j;
            float cf[ 3];
            if ( dens > RRT_KNOT_DENS[ 1]) {
                cf[ 0] = RRT_COEFS0;  cf[ 1] = RRT_COEFS1;  cf[ 2] = RRT_COEFS2;  j = 0;
            }
            else if ( dens > RRT_KNOT_DENS[ 2]) {
                cf[ 0] = RRT_COEFS1;  cf[ 1] = RRT_COEFS2;  cf[ 2] = RRT_COEFS3;  j = 1;
            }
            else if ( dens > RRT_KNOT_DENS[ 3]) {
                cf[ 0] = RRT_COEFS2;  cf[ 1] = RRT_COEFS3;  cf[ 2] = RRT_COEFS4;  j = 2;
            }
            else if ( dens > RRT_KNOT_DENS[ 4]) {
                cf[ 0] = RRT_COEFS3;  cf[ 1] = RRT_COEFS4;  cf[ 2] = RRT_COEFS5;  j = 3;
            }
            else if ( dens > RRT_KNOT_DENS[ 5]) {
                cf[ 0] = RRT_COEFS4;  cf[ 1] = RRT_COEFS5;  cf[ 2] = RRT_COEFS6;  j = 4;
            }
            else if ( dens > RRT_KNOT_DENS[ 6]) {
                cf[ 0] = RRT_COEFS5;  cf[ 1] = RRT_COEFS6;  cf[ 2] = RRT_COEFS7;  j = 5;
            }
            else {
                cf[ 0] = RRT_COEFS6;  cf[ 1] = RRT_COEFS7;  cf[ 2] = RRT_COEFS8;  j = 6;
            }

            const float tmp[ 3] = mult_f3_f33( cf, M);

            float a = tmp[ 0];
            float b = tmp[ 1];
            float c = tmp[ 2];
            c = c - dens;
            const float d = sqrt( b * b - 4. * a * c);

            const float t = ( 2. * c) / ( d - b);

            logh = RRT_KNOT_START + ( t + j) * RRT_KNOT_DEL;
        }
        x = pow10( logh);
    }
    return x;
}

// ================================================================= //



// === ODT SPLINE === //
//
// Algorithm for applying ODT tone curve in forward direction.
//
// 		vers 1.0  Doug Walker  		2012-01-23
// 		modified by Scott Dyer		2012-02-28

	// B-spline coefficients.
	// The units are density of the output.
	const float ODT_COEFS0 = -0.007;
	const float ODT_COEFS1 = -0.00441;
	const float ODT_COEFS2 =  0.0227;
	const float ODT_COEFS3 =  0.217;
	const float ODT_COEFS4 =  0.617;
	const float ODT_COEFS5 =  1.170;
	const float ODT_COEFS6 =  1.79;
	const float ODT_COEFS7 =  2.38;
	const float ODT_COEFS8 =  2.88;
	const float ODT_COEFS9 =  3.489;
	const float ODT_COEFS10 = 4.0;   // NB: keep this less than or equal to -log10( FLARE)
	
	// The flare term allows the spline to more rapidly approach zero
	// while keeping the shape of the curve well-behaved in density space.
	const float ODT_FLARE = 1e-4;
	
	// The last control point is fixed to yield a specific density at the
	// end of the knot domain.
	//const float ODT_COEFS11 = 2. * ( -log10( ODT_FLARE) - 0.001) - ODT_COEFS10;
	// Note: Apparently a CTL bug prevents calling log10() here, so
	// you'll need to update this manually if you change ODT_FLARE.
	const float ODT_COEFS11 = ODT_COEFS10 + 2. * ( 4. - ODT_COEFS10);
	
	// The knots are in units of OCES density.
	const unsigned int ODT_KNOT_LEN = 11;
	const float ODT_KNOT_START = -0.9;
	const float ODT_KNOT_END = 5.0; 
	
	// The ODT_KNOT_POW adjusts the spacing to put more knots near the toe (highlights).
	const float ODT_KNOT_POW = 1. / 1.3;
	const float ODT_OFFS = ODT_KNOT_START;
	const float ODT_SC = ODT_KNOT_END - ODT_KNOT_START;
	
	// KNOT_DENS is density of the spline at the knots.
	const float ODT_KNOT_DENS[ 11] = {  
		( ODT_COEFS0 + ODT_COEFS1) / 2.,
		( ODT_COEFS1 + ODT_COEFS2) / 2.,
		( ODT_COEFS2 + ODT_COEFS3) / 2.,
		( ODT_COEFS3 + ODT_COEFS4) / 2.,
		( ODT_COEFS4 + ODT_COEFS5) / 2.,
		( ODT_COEFS5 + ODT_COEFS6) / 2.,
		( ODT_COEFS6 + ODT_COEFS7) / 2.,
		( ODT_COEFS7 + ODT_COEFS8) / 2.,
		( ODT_COEFS8 + ODT_COEFS9) / 2.,
		( ODT_COEFS9 + ODT_COEFS10) / 2.,
		( ODT_COEFS10 + ODT_COEFS11) / 2.
	};
	
	// Parameters controlling linear extrapolation.
	const float ODT_LIGHT_SLOPE = 0.023;
	const float ODT_CROSSOVER = pow(10,-ODT_KNOT_END);
	const float ODT_REV_CROSSOVER = pow10( -ODT_KNOT_DENS[ ODT_KNOT_LEN - 1]) - ODT_FLARE;
	const float ODT_DARK_SLOPE = ODT_REV_CROSSOVER / ODT_CROSSOVER;
	
// Input and output are in linear (not log) units.
float rdt_shaper_fwd( varying float x)
{
    float y;
    // Linear extrapolation in linear space for negative & very dark values.
    if ( x <= ODT_CROSSOVER) 
        y = x * ODT_DARK_SLOPE;
    else {
        float in_dens = -log10( x);
        float out_dens;
        float knot_coord = ( in_dens - ODT_OFFS) / ODT_SC;

        // Linear extrapolation in log space for very light values.
        if ( knot_coord <= 0.)
            out_dens = ODT_KNOT_DENS[ 0] - ( ODT_KNOT_START - in_dens) * ODT_LIGHT_SLOPE;

        // For typical OCES values, apply a B-spline curve.
        else {
            knot_coord = ( ODT_KNOT_LEN - 1) * pow( knot_coord, ODT_KNOT_POW);
            int j = knot_coord;
            float t = knot_coord - j;

            // Would like to do this:
            //float cf[ 3] = { ODT_COEFS[ j], ODT_COEFS[ j + 1], ODT_COEFS[ j + 2]};
            // or at least:
            //cf[ 0] = ODT_COEFS[ j];
            //cf[ 1] = ODT_COEFS[ j + 1];
            //cf[ 2] = ODT_COEFS[ j + 2];
            // But apparently CTL bugs prevent it, so we do the following:
            float cf[ 3];
            if ( j <= 0) {
                cf[ 0] = ODT_COEFS0;  cf[ 1] = ODT_COEFS1;  cf[ 2] = ODT_COEFS2;
            }
            else if ( j == 1) {
                cf[ 0] = ODT_COEFS1;  cf[ 1] = ODT_COEFS2;  cf[ 2] = ODT_COEFS3;
            }
            else if ( j == 2) {
                cf[ 0] = ODT_COEFS2;  cf[ 1] = ODT_COEFS3;  cf[ 2] = ODT_COEFS4;
            }
            else if ( j == 3) {
                cf[ 0] = ODT_COEFS3;  cf[ 1] = ODT_COEFS4;  cf[ 2] = ODT_COEFS5;
            }
            else if ( j == 4) {
                cf[ 0] = ODT_COEFS4;  cf[ 1] = ODT_COEFS5;  cf[ 2] = ODT_COEFS6;
            }
            else if ( j == 5) {
                cf[ 0] = ODT_COEFS5;  cf[ 1] = ODT_COEFS6;  cf[ 2] = ODT_COEFS7;
            }
            else if ( j == 6) {
                cf[ 0] = ODT_COEFS6;  cf[ 1] = ODT_COEFS7;  cf[ 2] = ODT_COEFS8;
            }
            else if ( j == 7) {
                cf[ 0] = ODT_COEFS7;  cf[ 1] = ODT_COEFS8;  cf[ 2] = ODT_COEFS9;
            }
            else if ( j == 8) {
                cf[ 0] = ODT_COEFS8;  cf[ 1] = ODT_COEFS9;  cf[ 2] = ODT_COEFS10;
            }
            else {
                cf[ 0] = ODT_COEFS9;  cf[ 1] = ODT_COEFS10;  cf[ 2] = ODT_COEFS11;
            }

            float monomials[ 3] = { t * t, t, 1. };
            out_dens = dot_f3_f3( monomials, mult_f3_f33( cf, M));
        }
        y = pow10( -out_dens) - ODT_FLARE;
    }
    return y;
}


// Input and output are in linear (not log) units.
float rdt_shaper_rev( varying float y)
{
    float x;
    // Linear extrapolation in linear space for negative & very dark values.
    if ( y <= ODT_REV_CROSSOVER)
        x = y / ODT_DARK_SLOPE;
    else {
        float out_dens = -log10( y + ODT_FLARE);
        float in_dens;

        // Linear extrapolation in log space for very light values.
        if ( out_dens <= ODT_KNOT_DENS[ 0])
            in_dens = ODT_KNOT_START + ( out_dens - ODT_KNOT_DENS[ 0]) / ODT_LIGHT_SLOPE;

        // For typical OCES values, invert the B-spline curve.
        else {
            float cf[ 3];
            unsigned int j;
            if ( out_dens < ODT_KNOT_DENS[ 1]) {
                cf[ 0] = ODT_COEFS0;  cf[ 1] = ODT_COEFS1;  cf[ 2] = ODT_COEFS2;  j = 0;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 2]) {
                cf[ 0] = ODT_COEFS1;  cf[ 1] = ODT_COEFS2;  cf[ 2] = ODT_COEFS3;  j = 1;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 3]) {
                cf[ 0] = ODT_COEFS2;  cf[ 1] = ODT_COEFS3;  cf[ 2] = ODT_COEFS4;  j = 2;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 4]) {
                cf[ 0] = ODT_COEFS3;  cf[ 1] = ODT_COEFS4;  cf[ 2] = ODT_COEFS5;  j = 3;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 5]) {
                cf[ 0] = ODT_COEFS4;  cf[ 1] = ODT_COEFS5;  cf[ 2] = ODT_COEFS6;  j = 4;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 6]) {
                cf[ 0] = ODT_COEFS5;  cf[ 1] = ODT_COEFS6;  cf[ 2] = ODT_COEFS7;  j = 5;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 7]) {
                cf[ 0] = ODT_COEFS6;  cf[ 1] = ODT_COEFS7;  cf[ 2] = ODT_COEFS8;  j = 6;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 8]) {
                cf[ 0] = ODT_COEFS7;  cf[ 1] = ODT_COEFS8;  cf[ 2] = ODT_COEFS9;  j = 7;
            }
            else if ( out_dens < ODT_KNOT_DENS[ 9]) {
                cf[ 0] = ODT_COEFS8;  cf[ 1] = ODT_COEFS9;  cf[ 2] = ODT_COEFS10; j = 8;
            }
            else {
                cf[ 0] = ODT_COEFS9;  cf[ 1] = ODT_COEFS10;  cf[ 2] = ODT_COEFS11; j = 9;
            }

            const float tmp[ 3] = mult_f3_f33( cf, M);

            float a = tmp[ 0];
            float b = tmp[ 1];
            float c = tmp[ 2];
            c = c - out_dens;
            const float d = sqrt( b * b - 4. * a * c);

            const float t = ( 2. * c) / ( -d - b);

            const float knot_coord = t + j;
            in_dens = pow( knot_coord / ( ODT_KNOT_LEN - 1), 1. / ODT_KNOT_POW) * ODT_SC + ODT_OFFS;
        }
        x = pow10( -in_dens);
    }
    return x;
}
// ================================================================= //