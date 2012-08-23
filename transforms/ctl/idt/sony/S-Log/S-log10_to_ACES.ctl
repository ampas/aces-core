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
// IDT for 10-bit S-Log/S-Gamut Sony cameras
// Provided by Sony Electronics Corp.
//





/* ============ CONSTANTS ============ */
const float SGAMUT_TO_ACES_MTX[3][3] = {
	{ 0.754338638,  0.021198141, -0.009756991 },
	{ 0.133697046,  1.005410934,  0.004508563 },
	{ 0.111968437, -0.026610548,  1.005253201 }
};





/* ============ SUBFUNCTIONS ============ */
float SLog10_to_lin (
	float SLog
)
{
	return (pow(10,((((SLog/4-16)/219)-0.616596-0.03)/0.432699))-0.037584)*0.9;
}





/* ============ Main Algorithm ============ */
void
main
(   input varying float rIn,
    input varying float gIn,
    input varying float bIn,
    input varying float aIn,
    output varying float rOut,
    output varying float gOut,
    output varying float bOut,
    output varying float aOut
)
{
	// Prepare input values based on application bit depth handling
	float SLog[3];
	SLog[0] = rIn * 1023.0;
	SLog[1] = gIn * 1023.0;
	SLog[2] = bIn * 1023.0;

	// 10-bit Sony S-log to linear S-gamut
	float lin[3];
	lin[0] = SLog10_to_lin( SLog[0]);
	lin[1] = SLog10_to_lin( SLog[1]);
	lin[2] = SLog10_to_lin( SLog[2]);
	
	// S-Gamut to ACES matrix
	float aces[3] = mult_f3_f33( lin, SGAMUT_TO_ACES_MTX);

	rOut = aces[0];
	gOut = aces[1];
	bOut = aces[2];
	aOut = aIn;
}