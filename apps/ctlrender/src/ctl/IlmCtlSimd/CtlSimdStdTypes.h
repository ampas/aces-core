///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Academy of Motion Picture Arts and Sciences
// ("A.M.P.A.S."). Portions contributed by others as indicated.
// All rights reserved.
// 
// A world-wide, royalty-free, non-exclusive right to distribute, copy,
// modify, create derivatives, and use, in source and binary forms, is
// hereby granted, subject to acceptance of this license. Performance of
// any of the aforementioned acts indicates acceptance to be bound by the
// following terms and conditions:
// 
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty.
// 
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty
//     in the documentation and/or other materials provided with the
//     distribution.
// 
//   * Nothing in this license shall be deemed to grant any rights to
//     trademarks, copyrights, patents, trade secrets or any other
//     intellectual property of A.M.P.A.S. or any contributors, except
//     as expressly stated herein, and neither the name of A.M.P.A.S.
//     nor of any other contributors to this software, may be used to
//     endorse or promote products derived from this software without
//     specific prior written permission of A.M.P.A.S. or contributor,
//     as appropriate.
// 
// This license shall be governed by the laws of the State of California,
// and subject to the jurisdiction of the courts therein.
// 
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT ARE DISCLAIMED. IN NO
// EVENT SHALL A.M.P.A.S., ANY CONTRIBUTORS OR DISTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


#ifndef INCLUDED_CTL_SIMD_STD_TYPES_H
#define INCLUDED_CTL_SIMD_STD_TYPES_H

//-----------------------------------------------------------------------------
//
//	A class that generates the function and data types needed
//	for the SIMD version of the CTL standard library.
//
//-----------------------------------------------------------------------------

#include <CtlType.h>

namespace Ctl {

class SymbolTable;

class SimdStdTypes
{
  public:

     SimdStdTypes (LContext &lcontext);
    ~SimdStdTypes ();

    DataTypePtr 	type_v ();		// void

    DataTypePtr 	type_b ();		// bool

    DataTypePtr 	type_i ();		// int

    DataTypePtr		type_ui ();		// unsigned int

    DataTypePtr		type_h ();		// unsigned int

    DataTypePtr		type_f ();		// float

    DataTypePtr		type_s ();		// string

    DataTypePtr		type_i2 ();		// int[2]

    DataTypePtr		type_f2 ();		// float[2]

    DataTypePtr		type_f3 ();		// float[3]

    DataTypePtr		type_f33 ();		// float[3][3]

    DataTypePtr		type_f44 ();		// float[4][4]

    DataTypePtr		type_box2i ();		// struct Box2i
						// {
						//     int min[2];
						//     int max[2];
						// };

    DataTypePtr		type_box2f ();		// struct Box2f
						// {
						//     float min[2];
						//     float max[2];
						// };

    DataTypePtr		type_chr ();		// struct Chromaticities
						// {
						//     float red[2];
						//     float green[2];
						//     float blue[2];
						//     float white[2];
						// };

    FunctionTypePtr	funcType_v_b ();	// void func (bool)

    FunctionTypePtr	funcType_v_i ();	// void func (int)

    FunctionTypePtr	funcType_v_ui ();	// void func (unsigned int)

    FunctionTypePtr	funcType_v_h ();	// void func (half)

    FunctionTypePtr	funcType_v_f ();	// void func (float)

    FunctionTypePtr	funcType_v_s ();	// void func (string)

    FunctionTypePtr	funcType_b_h ();	// bool func (half)

    FunctionTypePtr	funcType_b_f ();	// bool func (float)

    FunctionTypePtr	funcType_f_f ();	// float func (float)

    FunctionTypePtr	funcType_h_f ();	// half func (float)

    FunctionTypePtr	funcType_f_h ();	// float func (half)

    FunctionTypePtr	funcType_f_f_f ();	// float func (float, float)

    FunctionTypePtr	funcType_h_h_f ();	// half func (half, float)

    FunctionTypePtr	funcType_f33_f33 ();	// float[3][3] func
						//	(float[3][3])

    FunctionTypePtr	funcType_f44_f44 ();	// float[4][4] func
						//	(float[4][4])

    FunctionTypePtr	funcType_f33_f33_f33 ();// float[3][3] func
    						//	(float[3][3],
						//	 float[3][3])

    FunctionTypePtr	funcType_f44_f44_f44 ();// float[4][4] func
    						//	(float[4][4],
						//	 float[4][4])

    FunctionTypePtr	funcType_f33_f_f33 ();	// float[3][3] func
    						//	(float,
						//	 float[3][3])

    FunctionTypePtr	funcType_f44_f_f44 ();	// float[4][4] func
    						//	(float,
						//	 float[4][4])

    FunctionTypePtr	funcType_f3_f3_f44 ();	// float[3] func
    						//	(float[3],
						//	 float[4][4])

    FunctionTypePtr	funcType_f3_f3_f33 ();	// float[3] func
    						//	(float[3],
						//	 float[3][3])

    FunctionTypePtr	funcType_f3_f3_f3 ();	// float[3] func
    						//	(float[3],
						//	 float[3])

    FunctionTypePtr	funcType_f3_f_f3 ();	// float[3] func
    						//	(float,
						//	 float[3])

    FunctionTypePtr	funcType_f_f3_f3 ();	// float func
    						//	(float[3],
						//	 float[3])

    FunctionTypePtr	funcType_f_f3 ();	// float func (float[3])

    FunctionTypePtr	funcType_f44_chr_f ();	// float func
    						//	(Chromaticities,
						//	 float)

    FunctionTypePtr	funcType_f_f0_f_f_f ();	// float func
    						//	(float[],
						//	 float,
						//	 float,
						//	 float)

    FunctionTypePtr	funcType_f_f02_f ();	// float func
    						//	(float[][2],
						//	 float)

    FunctionTypePtr	funcType_f3_f0003_f3_f3_f3 ();
						// float[3] func
						//	(float[][][][3],
						//	 float[3],
						//	 float[3],
						//	 float[3])

    FunctionTypePtr	funcType_v_f0003_f3_f3_fff_offf ();
						// void func
						//	(float[][][][3],
						//	 float[3],
						//	 float[3],
						//	 float,
						//	 float,
						//	 float,
						//	 output float,
						//	 output float,
						//	 output float)

    FunctionTypePtr	funcType_v_f0003_f3_f3_hhh_ohhh ();
						// void func
						//	(float[][][][3],
						//	 float[3],
						//	 float[3],
						//	 half,
						//	 half,
						//	 half,
						//	 output half,
						//	 output half,
						//	 output half)

    FunctionTypePtr	funcType_v_f023_f3_f3_of0003 ();
						// void func
						//	(float[][2][3],
						//	 float[3],
						//	 float[3],
						//	 output float[][][][3])
  private:

    LContext &		_lcontext;

    DataTypePtr		_type_i2;
    DataTypePtr		_type_f2;
    DataTypePtr		_type_f3;
    DataTypePtr		_type_f33;
    DataTypePtr		_type_f44;
    DataTypePtr		_type_box2i;
    DataTypePtr		_type_box2f;
    DataTypePtr		_type_chr;
    FunctionTypePtr	_funcType_v_b;
    FunctionTypePtr	_funcType_v_i;
    FunctionTypePtr	_funcType_v_ui;
    FunctionTypePtr	_funcType_v_h;
    FunctionTypePtr	_funcType_v_f;
    FunctionTypePtr	_funcType_v_s;
    FunctionTypePtr	_funcType_b_h;
    FunctionTypePtr	_funcType_b_f;
    FunctionTypePtr	_funcType_f_f;
    FunctionTypePtr	_funcType_h_f;
    FunctionTypePtr	_funcType_f_h;
    FunctionTypePtr	_funcType_f_f_f;
    FunctionTypePtr	_funcType_h_h_f;
    FunctionTypePtr	_funcType_f33_f33;
    FunctionTypePtr	_funcType_f44_f44;
    FunctionTypePtr	_funcType_f33_f33_f33;
    FunctionTypePtr	_funcType_f44_f44_f44;
    FunctionTypePtr	_funcType_f33_f_f33;
    FunctionTypePtr	_funcType_f44_f_f44;
    FunctionTypePtr	_funcType_f3_f3_f33;
    FunctionTypePtr	_funcType_f3_f3_f44;
    FunctionTypePtr	_funcType_f3_f3_f3;
    FunctionTypePtr	_funcType_f3_f_f3;
    FunctionTypePtr	_funcType_f_f3_f3;
    FunctionTypePtr	_funcType_f_f3;
    FunctionTypePtr	_funcType_f44_chr_f;
    FunctionTypePtr	_funcType_f_f0_f_f_f;
    FunctionTypePtr	_funcType_f_f02_f;
    FunctionTypePtr	_funcType_f3_f0003_f3_f3_f3;
    FunctionTypePtr	_funcType_v_f0003_f3_f3_fff_offf;
    FunctionTypePtr	_funcType_v_f0003_f3_f3_hhh_ohhh;
    FunctionTypePtr	_funcType_v_f023_f3_f3_of0003;
};


} // namespace Ctl

#endif
