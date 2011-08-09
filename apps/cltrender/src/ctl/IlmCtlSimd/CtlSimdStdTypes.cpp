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

//-----------------------------------------------------------------------------
//
//	A class that generates the function and data types needed
//	for the SIMD version of the CTL standard library.
//
//-----------------------------------------------------------------------------

#include <CtlSimdStdTypes.h>
#include <CtlSymbolTable.h>
#include <CtlLContext.h>

using namespace std;

namespace Ctl {


SimdStdTypes::SimdStdTypes (LContext &lcontext): _lcontext (lcontext)
{
    // empty
}


SimdStdTypes::~SimdStdTypes ()
{
    // empty
}


DataTypePtr
SimdStdTypes::type_v ()
{
    // void

    return _lcontext.newVoidType();
}


DataTypePtr
SimdStdTypes::type_b ()
{
    // bool

    return _lcontext.newBoolType();
}


DataTypePtr
SimdStdTypes::type_i ()
{
    // int

    return _lcontext.newIntType();
}


DataTypePtr
SimdStdTypes::type_ui ()
{
    // unsigned int

    return _lcontext.newUIntType();
}


DataTypePtr
SimdStdTypes::type_h ()
{
    // unsigned int

    return _lcontext.newHalfType();
}


DataTypePtr
SimdStdTypes::type_f ()
{
    // float

    return _lcontext.newFloatType();
}


DataTypePtr
SimdStdTypes::type_s ()
{
    // string

    return _lcontext.newStringType();
}


DataTypePtr
SimdStdTypes::type_i2 ()
{
    // int[2]

    if (!_type_i2)
	_type_i2 = _lcontext.newArrayType (type_i(), 2);

    return _type_i2;
}


DataTypePtr
SimdStdTypes::type_f2 ()
{
    // float[2]

    if (!_type_f2)
	_type_f2 = _lcontext.newArrayType (type_f(), 2);

    return _type_f2;
}


DataTypePtr
SimdStdTypes::type_f3 ()
{
    // float[3]

    if (!_type_f3)
	_type_f3 = _lcontext.newArrayType (type_f(), 3);

    return _type_f3;
}


DataTypePtr
SimdStdTypes::type_f33 ()
{
    // float[3][3]

    if (!_type_f33)
	_type_f33 =
	    _lcontext.newArrayType (_lcontext.newArrayType (type_f(), 3), 3);

    return _type_f33;
}


DataTypePtr
SimdStdTypes::type_f44 ()
{
    // float[4][4]

    if (!_type_f44)
	_type_f44 =
	    _lcontext.newArrayType (_lcontext.newArrayType (type_f(), 4), 4);

    return _type_f44;
}


DataTypePtr
SimdStdTypes::type_box2i ()
{
    // struct Box2i
    // {
    //     int min[2];
    //     int max[2];
    // };

    if (!_type_box2i)
    {
	string structName =
	    _lcontext.symtab().getAbsoluteName ("Box2i");

	MemberVector m;
	m.push_back (Member ("min", type_i2()));
	m.push_back (Member ("max", type_i2()));

	_type_box2i = _lcontext.newStructType (structName, m);
	SymbolInfoPtr info = new SymbolInfo (0, RWA_NONE, true, _type_box2i);
	_lcontext.symtab().defineSymbol (structName, info);
    }

    return _type_box2i;
}


DataTypePtr
SimdStdTypes::type_box2f ()
{
    // struct Box2f
    // {
    //     float min[2];
    //     float max[2];
    // };

    if (!_type_box2f)
    {
	string structName =
	    _lcontext.symtab().getAbsoluteName ("Box2f");

	MemberVector m;
	m.push_back (Member ("min", type_f2()));
	m.push_back (Member ("max", type_f2()));

	_type_box2f = _lcontext.newStructType (structName, m);
	SymbolInfoPtr info = new SymbolInfo (0, RWA_NONE, true, _type_box2f);
	_lcontext.symtab().defineSymbol (structName, info);
    }

    return _type_box2f;
}


DataTypePtr
SimdStdTypes::type_chr ()
{
    // struct Chromaticities
    // {
    //     float red[2];
    //     float green[2];
    //     float blue[2];
    //     float white[2];
    // };

    if (!_type_chr)
    {
	string structName =
	    _lcontext.symtab().getAbsoluteName ("Chromaticities");

	MemberVector m;
	m.push_back (Member ("red", type_f2()));
	m.push_back (Member ("green", type_f2()));
	m.push_back (Member ("blue", type_f2()));
	m.push_back (Member ("white", type_f2()));

	_type_chr = _lcontext.newStructType (structName, m);
	SymbolInfoPtr info = new SymbolInfo (0, RWA_NONE, true, _type_chr);
	_lcontext.symtab().defineSymbol (structName, info);
    }

    return _type_chr;
}


FunctionTypePtr
SimdStdTypes::funcType_v_b ()
{
    // void func (bool)

    if (!_funcType_v_b)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_b(), 0, RWA_READ, false));
	_funcType_v_b = _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_b;
}


FunctionTypePtr
SimdStdTypes::funcType_v_i ()
{
    // void func (int)

    if (!_funcType_v_i)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_i(), 0, RWA_READ, false));
	_funcType_v_i = _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_i;
}


FunctionTypePtr
SimdStdTypes::funcType_v_ui ()
{
    // void func (unsigned int)

    if (!_funcType_v_ui)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_ui(), 0, RWA_READ, false));
	_funcType_v_ui = _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_ui;
}


FunctionTypePtr
SimdStdTypes::funcType_v_h ()
{
    // void func (half)

    if (!_funcType_v_h)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_h(), 0, RWA_READ, false));
	_funcType_v_h = _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_h;
}


FunctionTypePtr
SimdStdTypes::funcType_v_f ()
{
    // void func (float)

    if (!_funcType_v_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	_funcType_v_f = _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_f;
}


FunctionTypePtr
SimdStdTypes::funcType_v_s ()
{
    // void func (string)

    if (!_funcType_v_s)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_s(), 0, RWA_READ, false));
	_funcType_v_s = _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_s;
}


FunctionTypePtr
SimdStdTypes::funcType_b_h ()
{
    // bool func (half)

    if (!_funcType_b_h)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_h(), 0, RWA_READ, false));
	_funcType_b_h = _lcontext.newFunctionType (type_b(), false, p);
    }

    return _funcType_b_h;
}


FunctionTypePtr
SimdStdTypes::funcType_b_f ()
{
    // bool func (float)

    if (!_funcType_b_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	_funcType_b_f = _lcontext.newFunctionType (type_b(), false, p);
    }

    return _funcType_b_f;
}


FunctionTypePtr
SimdStdTypes::funcType_f_f ()
{
    // float func (float)

    if (!_funcType_f_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	_funcType_f_f = _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_f;
}


FunctionTypePtr
SimdStdTypes::funcType_h_f ()
{
    // half func (float)

    if (!_funcType_h_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	_funcType_h_f = _lcontext.newFunctionType (type_h(), false, p);
    }

    return _funcType_h_f;
}


FunctionTypePtr
SimdStdTypes::funcType_f_h ()
{
    // float func (half)

    if (!_funcType_f_h)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_h(), 0, RWA_READ, false));
	_funcType_f_h = _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_h;
}


FunctionTypePtr
SimdStdTypes::funcType_f_f_f ()
{
    // float func (float, float)

    if (!_funcType_f_f_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f(), 0, RWA_READ, false));
	_funcType_f_f_f = _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_f_f;
}


FunctionTypePtr
SimdStdTypes::funcType_h_h_f ()
{
    // half func (half, float)

    if (!_funcType_h_h_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_h(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f(), 0, RWA_READ, false));
	_funcType_h_h_f = _lcontext.newFunctionType (type_h(), false, p);
    }

    return _funcType_h_h_f;
}


FunctionTypePtr
SimdStdTypes::funcType_f33_f33 ()
{
    // float[3][3] func (float[3][3])

    if (!_funcType_f33_f33)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f33(), 0, RWA_READ, false));
	_funcType_f33_f33 = _lcontext.newFunctionType (type_f33(), false, p);
    }

    return _funcType_f33_f33;
}


FunctionTypePtr
SimdStdTypes::funcType_f44_f44 ()
{
    // float[4][4] func (float[4][4])

    if (!_funcType_f44_f44)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f44(), 0, RWA_READ, false));
	_funcType_f44_f44 = _lcontext.newFunctionType (type_f44(), false, p);
    }

    return _funcType_f44_f44;
}


FunctionTypePtr
SimdStdTypes::funcType_f33_f33_f33 ()
{
    // float[3][3] func (float[3][3], float[3][3])

    if (!_funcType_f33_f33_f33)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f33(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f33(), 0, RWA_READ, false));
	
	_funcType_f33_f33_f33 =
	    _lcontext.newFunctionType (type_f33(), false, p);
    }

    return _funcType_f33_f33_f33;
}


FunctionTypePtr
SimdStdTypes::funcType_f44_f44_f44 ()
{
    // float[4][4] func (float[4][4], float[4][4])

    if (!_funcType_f44_f44_f44)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f44(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f44(), 0, RWA_READ, false));
	
	_funcType_f44_f44_f44 =
	    _lcontext.newFunctionType (type_f44(), false, p);
    }

    return _funcType_f44_f44_f44;
}


FunctionTypePtr
SimdStdTypes::funcType_f33_f_f33 ()
{
    // float[3][3] func (float, float[3][3])

    if (!_funcType_f33_f_f33)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f33(), 0, RWA_READ, false));

	_funcType_f33_f_f33 =
	    _lcontext.newFunctionType (type_f33(), false, p);
    }

    return _funcType_f33_f_f33;
}


FunctionTypePtr
SimdStdTypes::funcType_f44_f_f44 ()
{
    // float[4][4] func (float, float[4][4])

    if (!_funcType_f44_f_f44)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f44(), 0, RWA_READ, false));

	_funcType_f44_f_f44 =
	    _lcontext.newFunctionType (type_f44(), false, p);
    }

    return _funcType_f44_f_f44;
}


FunctionTypePtr
SimdStdTypes::funcType_f3_f3_f33 ()
{
    // float[3] func (float[3], float[3][3])

    if (!_funcType_f3_f3_f33)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f33(), 0, RWA_READ, false));

	_funcType_f3_f3_f33 =
	    _lcontext.newFunctionType (type_f3(), false, p);
    }

    return _funcType_f3_f3_f33;
}


FunctionTypePtr
SimdStdTypes::funcType_f3_f3_f44 ()
{
    // float[3] func (float[3], float[4][4])

    if (!_funcType_f3_f3_f44)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f44(), 0, RWA_READ, false));

	_funcType_f3_f3_f44 =
	    _lcontext.newFunctionType (type_f3(), false, p);
    }

    return _funcType_f3_f3_f44;
}


FunctionTypePtr
SimdStdTypes::funcType_f3_f3_f3 ()
{
    // float[3] func (float[3], float[3])

    if (!_funcType_f3_f3_f3)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));

	_funcType_f3_f3_f3 =
	    _lcontext.newFunctionType (type_f3(), false, p);
    }

    return _funcType_f3_f3_f3;
}


FunctionTypePtr
SimdStdTypes::funcType_f3_f_f3 ()
{
    // float[3] func (float, float[3])

    if (!_funcType_f3_f_f3)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));

	_funcType_f3_f_f3 =
	    _lcontext.newFunctionType (type_f3(), false, p);
    }

    return _funcType_f3_f_f3;
}


FunctionTypePtr
SimdStdTypes::funcType_f_f3_f3 ()
{
    // float func (float[3], float[3])

    if (!_funcType_f_f3_f3)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));

	_funcType_f_f3_f3 =
	    _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_f3_f3;
}


FunctionTypePtr
SimdStdTypes::funcType_f_f3 ()
{
    // float func (float[3])

    if (!_funcType_f_f3)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_f3(), 0, RWA_READ, false));

	_funcType_f_f3 =
	    _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_f3;
}


FunctionTypePtr
SimdStdTypes::funcType_f44_chr_f ()
{
    // float func (Chromaticities, float)

    if (!_funcType_f44_chr_f)
    {
	ParamVector p;
	p.push_back (Param ("a1", type_chr(), 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f(), 0, RWA_READ, false));

	_funcType_f44_chr_f =
	    _lcontext.newFunctionType (type_f44(), false, p);
    }

    return _funcType_f44_chr_f;
}


FunctionTypePtr	
SimdStdTypes::funcType_f_f0_f_f_f ()
{
    // float func (float[], float, float, float)

    if (!_funcType_f_f0_f_f_f)
    {
	SizeVector sizes;
	sizes.push_back (0);

	DataTypePtr type_f0 =
	    _lcontext.newArrayType (type_f(), sizes, LContext::PARAMETER);

	ParamVector p;
	p.push_back (Param ("a1", type_f0,  0, RWA_READ, false));
	p.push_back (Param ("a2", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a3", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a4", type_f(), 0, RWA_READ, false));

	_funcType_f_f0_f_f_f =
	    _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_f0_f_f_f;
}


FunctionTypePtr
SimdStdTypes::funcType_f_f02_f ()
{
    // float func (float[][2], float)

    if (!_funcType_f_f02_f)
    {
	SizeVector sizes;
	sizes.push_back (0);
	sizes.push_back (2);

	DataTypePtr type_f02 =
	    _lcontext.newArrayType (type_f(), sizes, LContext::PARAMETER);

	ParamVector p;
	p.push_back (Param ("a1", type_f02,  0, RWA_READ, false));
	p.push_back (Param ("a2", type_f(), 0, RWA_READ, false));

	_funcType_f_f02_f =
	    _lcontext.newFunctionType (type_f(), false, p);
    }

    return _funcType_f_f02_f;
}


FunctionTypePtr
SimdStdTypes::funcType_f3_f0003_f3_f3_f3 ()
{
    // float[3] func (float[][][][3], float[3], float[3], float[3])

    if (!_funcType_f3_f0003_f3_f3_f3)
    {
	SizeVector sizes;
	sizes.push_back (0);
	sizes.push_back (0);
	sizes.push_back (0);
	sizes.push_back (3);

	DataTypePtr type_f0003 =
	    _lcontext.newArrayType (type_f(), sizes, LContext::PARAMETER);

	ParamVector p;
	p.push_back (Param ("a1", type_f0003, 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a3", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a4", type_f3(), 0, RWA_READ, false));

	_funcType_f3_f0003_f3_f3_f3 =
	    _lcontext.newFunctionType (type_f3(), false, p);
    }

    return _funcType_f3_f0003_f3_f3_f3;
}


FunctionTypePtr
SimdStdTypes::funcType_v_f0003_f3_f3_fff_offf ()
{
    // void func (float[][][][3], float[3], float[3],
    //		  float, float, float,
    //		  output float, output float, output float)

    if (!_funcType_v_f0003_f3_f3_fff_offf)
    {
	SizeVector sizes;
	sizes.push_back (0);
	sizes.push_back (0);
	sizes.push_back (0);
	sizes.push_back (3);

	DataTypePtr type_f0003 =
	    _lcontext.newArrayType (type_f(), sizes, LContext::PARAMETER);

	ParamVector p;
	p.push_back (Param ("a1", type_f0003,  0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a3", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a4", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a5", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a6", type_f(), 0, RWA_READ, false));
	p.push_back (Param ("a7", type_f(), 0, RWA_WRITE, false));
	p.push_back (Param ("a8", type_f(), 0, RWA_WRITE, false));
	p.push_back (Param ("a9", type_f(), 0, RWA_WRITE, false));

	_funcType_v_f0003_f3_f3_fff_offf =
	    _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_f0003_f3_f3_fff_offf;
}


FunctionTypePtr
SimdStdTypes::funcType_v_f0003_f3_f3_hhh_ohhh ()
{
    // void func (float[][][][3], float[3], float[3],
    //		  half, half, half,
    //		  output half, output half, output half)

    if (!_funcType_v_f0003_f3_f3_hhh_ohhh)
    {
	SizeVector sizes;
	sizes.push_back (0);
	sizes.push_back (0);
	sizes.push_back (0);
	sizes.push_back (3);

	DataTypePtr type_f0003 =
	    _lcontext.newArrayType (type_f(), sizes, LContext::PARAMETER);

	ParamVector p;
	p.push_back (Param ("a1", type_f0003, 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a3", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a4", type_h(), 0, RWA_READ, false));
	p.push_back (Param ("a5", type_h(), 0, RWA_READ, false));
	p.push_back (Param ("a6", type_h(), 0, RWA_READ, false));
	p.push_back (Param ("a7", type_h(), 0, RWA_WRITE, false));
	p.push_back (Param ("a8", type_h(), 0, RWA_WRITE, false));
	p.push_back (Param ("a9", type_h(), 0, RWA_WRITE, false));

	_funcType_v_f0003_f3_f3_hhh_ohhh =
	    _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_f0003_f3_f3_hhh_ohhh;
}


FunctionTypePtr
SimdStdTypes::funcType_v_f023_f3_f3_of0003 ()
{
    // void func (float[][2][3], float[3], float[3], output float[][][][3])

    if (!_funcType_v_f023_f3_f3_of0003)
    {
	SizeVector inSizes;
	inSizes.push_back (0);
	inSizes.push_back (2);
	inSizes.push_back (3);

	DataTypePtr type_f023 =
	    _lcontext.newArrayType (type_f(), inSizes, LContext::PARAMETER);

	SizeVector outSizes;
	outSizes.push_back (0);
	outSizes.push_back (0);
	outSizes.push_back (0);
	outSizes.push_back (3);

	DataTypePtr type_f0003 =
	    _lcontext.newArrayType (type_f(), outSizes, LContext::PARAMETER);

	ParamVector p;
	p.push_back (Param ("a1", type_f023, 0, RWA_READ, false));
	p.push_back (Param ("a2", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a3", type_f3(), 0, RWA_READ, false));
	p.push_back (Param ("a4", type_f0003, 0, RWA_WRITE, false));

	_funcType_v_f023_f3_f3_of0003 =
	    _lcontext.newFunctionType (type_v(), false, p);
    }

    return _funcType_v_f023_f3_f3_of0003;
}


} // namespace Ctl
