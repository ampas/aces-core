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


#include <CtlSimdInterpreter.h>
#include <CtlFunctionCall.h>
#include <CtlType.h>
#include <ImathMath.h>
#include <iostream>
#include <exception>
#include <assert.h>
#include <sstream>
#include <limits>
#include <half.h>
#include <testVarying.h>

using namespace Ctl;
using namespace Imath;
using namespace std;


struct Hodgepodge
{
    float f;
    int i;
    bool b;
    bool ab[2];
    half h;
};
	  

void
varyingDefaults(Interpreter &interp,
		int dataSize,
		bool *b1Orig,
		float *f1Orig,
		half *h1Orig,
		int  *i1Orig,
		Hodgepodge * hod1Orig,
		Hodgepodge * ahod1Orig,
		string ctlFuncName)
{


    FunctionCallPtr func;
    FunctionArgPtr ret;

    // check default values for varying parameters
    {
	func = interp.newFunctionCall(ctlFuncName);
	
	assert(func->numInputArgs() == 12);
	assert(func->numOutputArgs() == 12);

	FunctionArgPtr b1InArg = func->findInputArg("b1In");
	FunctionArgPtr b2InArg = func->findInputArg("b2In");
	FunctionArgPtr f1InArg = func->findInputArg("f1In");
	FunctionArgPtr f2InArg = func->findInputArg("f2In");
	FunctionArgPtr h1InArg = func->findInputArg("h1In");
	FunctionArgPtr h2InArg = func->findInputArg("h2In");
	FunctionArgPtr i1InArg = func->findInputArg("i1In");
	FunctionArgPtr i2InArg = func->findInputArg("i2In");

	FunctionArgPtr hod1InArg = func->findInputArg("hod1In");
	FunctionArgPtr hod2InArg = func->findInputArg("hod2In");
	FunctionArgPtr ahod1InArg = func->findInputArg("ahod1In");
	FunctionArgPtr ahod2InArg = func->findInputArg("ahod2In");

	bool * b1In   = (bool*)(b1InArg->data());
	float * f1In   = (float*)(f1InArg->data());
	float * f2In   = (float*)(f2InArg->data());
	half * h1In   = (half*)(h1InArg->data());
	int * i1In   = (int*)(i1InArg->data());
	int * i2In   = (int*)(i2InArg->data());
	Hodgepodge * hod1In   = (Hodgepodge*)(hod1InArg->data());
	Hodgepodge * ahod1In   = (Hodgepodge*)(ahod1InArg->data());

	assert(b1InArg->isVarying());
	memcpy(b1In, b1Orig, dataSize*sizeof(bool));
	assert(b2InArg->isVarying());
	b2InArg->setDefaultValue();
	assert(b2InArg->isVarying());

	assert(i1InArg->isVarying());
	memcpy(i1In, i1Orig, dataSize*sizeof(int));
	assert(i2InArg->isVarying());
	i2InArg->setDefaultValue();
	assert(i2InArg->isVarying());

	assert(h1InArg->isVarying());
	memcpy(h1In, h1Orig, dataSize*sizeof(half));
	assert(h2InArg->isVarying());
	h2InArg->setDefaultValue();
	assert(h2InArg->isVarying());

	assert(f1InArg->isVarying());
	memcpy(f1In, f1Orig, dataSize*sizeof(float));

	assert(f2InArg->isVarying());
	memcpy(f2In, f1Orig, dataSize*sizeof(float));
	f2InArg->setDefaultValue();
	assert(f2InArg->isVarying());

	f2InArg = func->findInputArg("f2In");
	assert(f2InArg->isVarying());

	assert(hod1InArg->isVarying());
	memcpy(hod1In, hod1Orig, dataSize*sizeof(Hodgepodge));
	assert(hod2InArg->isVarying());
	hod2InArg->setDefaultValue();
	assert(hod2InArg->isVarying());

	assert(ahod1InArg->isVarying());
	memcpy(ahod1In, ahod1Orig, dataSize*sizeof(Hodgepodge)*2);
	assert(ahod2InArg->isVarying());
	ahod2InArg->setDefaultValue();
	assert(ahod2InArg->isVarying());

        //  Call the function multiple times to verify that 
        //  the inputs are still varying, and data pointers are
        //  stil valid
        for(int ncalls = 0; ncalls < 3; ncalls++)
        {
            // Try setting the value again, after setting to 
            // the default
            assert(b1InArg->isVarying());
            memcpy(b1In, b1Orig, dataSize*sizeof(bool));
            assert(b2InArg->isVarying());
            b2InArg->setDefaultValue();
            assert(b2InArg->isVarying());

            // Check that the input is still set to the default
            // from previous call
            assert(i2InArg->isVarying());
            for(int i = 0; i < dataSize; i++)
                assert(i2In[i] == 46);
        
            func->callFunction(dataSize);

            FunctionArgPtr b1OutArg = func->findOutputArg("b1Out");
            assert( b1OutArg->isVarying());
            FunctionArgPtr b2OutArg = func->findOutputArg("b2Out");
            assert( b2OutArg->isVarying());

            FunctionArgPtr f1OutArg = func->findOutputArg("f1Out");
            assert( f1OutArg->isVarying());
            FunctionArgPtr f2OutArg = func->findOutputArg("f2Out");
            assert( f2OutArg->isVarying());

            FunctionArgPtr h1OutArg = func->findOutputArg("h1Out");
            assert( h1OutArg->isVarying());
            FunctionArgPtr h2OutArg = func->findOutputArg("h2Out");
            assert( h2OutArg->isVarying());

            FunctionArgPtr i1OutArg = func->findOutputArg("i1Out");
            assert( i1OutArg->isVarying());
            FunctionArgPtr i2OutArg = func->findOutputArg("i2Out");
            assert( i2OutArg->isVarying());

            FunctionArgPtr hod1OutArg = func->findOutputArg("hod1Out");
            assert( hod1OutArg->isVarying());
            FunctionArgPtr hod2OutArg = func->findOutputArg("hod2Out");
            assert( hod2OutArg->isVarying());

            FunctionArgPtr ahod1OutArg = func->findOutputArg("ahod1Out");
            assert( ahod1OutArg->isVarying());
            FunctionArgPtr ahod2OutArg = func->findOutputArg("ahod2Out");
            assert( ahod2OutArg->isVarying());


            bool * b1Out = (bool*)(b1OutArg->data());
            bool * b2Out = (bool*)(b2OutArg->data());
            float * f1Out = (float*)(f1OutArg->data());
            float * f2Out = (float*)(f2OutArg->data());
            half * h1Out = (half*)(h1OutArg->data());
            half * h2Out = (half*)(h2OutArg->data());
            int * i1Out = (int*)(i1OutArg->data());
            int * i2Out = (int*)(i2OutArg->data());
            Hodgepodge * hod1Out = (Hodgepodge*)(hod1OutArg->data());
            Hodgepodge * hod2Out = (Hodgepodge*)(hod2OutArg->data());
            Hodgepodge * ahod1Out = (Hodgepodge*)(ahod1OutArg->data());
            Hodgepodge * ahod2Out = (Hodgepodge*)(ahod2OutArg->data());


            for(int i = 0; i < dataSize; i++)
            {
                assert(b1Out[i] == b1Orig[i]);
                assert(b2Out[i] == false);

                assert(equalWithAbsError(f1Out[i], f1Orig[i], .0001f));
                assert(equalWithAbsError(f2Out[i], -12399999.f, .00001f));

                assert(equalWithAbsError(h1Out[i], h1Orig[i], half(.0001)));
                assert(equalWithAbsError(h2Out[i], half(23), half(.0001)));

                assert(i1Out[i] == i1Orig[i]);
                assert(i2Out[i] == 46);

                assert(equalWithAbsError(hod1Out[i].f, hod1Orig[i].f, .0001f));
                assert(hod1Out[i].i == hod1Orig[i].i);
                assert(hod1Out[i].f == hod1Orig[i].f);
                assert(hod1Out[i].ab[0] == hod1Orig[i].ab[0]);
                assert(hod1Out[i].ab[1] == hod1Orig[i].ab[1]);
                assert(equalWithAbsError(hod1Out[i].h, hod1Orig[i].h, half(.0001)));

                assert(equalWithAbsError(hod2Out[i].f, 1.3f, .0001f));
                assert(hod2Out[i].i == -2);
                assert(hod2Out[i].b == true);
                assert(hod2Out[i].ab[0] == true);
                assert(hod2Out[i].ab[0] == true);
                assert(equalWithAbsError(hod2Out[i].h, half(.1), half(.001)));

                assert(equalWithAbsError
                       (ahod1Out[2*i].f, ahod1Orig[2*i].f, .0001f));

                assert(ahod1Out[2*i].i == ahod1Orig[2*i].i);
                assert(ahod1Out[2*i].f == ahod1Orig[2*i].f);
                assert(ahod1Out[2*i].ab[0] == ahod1Orig[2*i].ab[0]);
                assert(ahod1Out[2*i].ab[1] == ahod1Orig[2*i].ab[1]);

                assert(equalWithAbsError
                       (ahod1Out[2*i].h, ahod1Orig[2*i].h, half(.0001)));

                assert(equalWithAbsError
                       (ahod1Out[2*i+1].f, ahod1Orig[2*i+1].f, .0001f));

                assert(ahod1Out[2*i+1].i == ahod1Orig[2*i+1].i);
                assert(ahod1Out[2*i+1].f == ahod1Orig[2*i+1].f);
                assert(ahod1Out[2*i+1].ab[0] == ahod1Orig[2*i+1].ab[0]);
                assert(ahod1Out[2*i+1].ab[1] == ahod1Orig[2*i+1].ab[1]);

                assert(equalWithAbsError
                       (ahod1Out[2*i+1].h, ahod1Orig[2*i+1].h, half (.0001)));

                assert(equalWithAbsError (ahod2Out[2*i].f, -1.2f, .0001f));
                assert(ahod2Out[2*i].i == -1);
                assert(ahod2Out[2*i].b == true);
                assert(ahod2Out[2*i].ab[0] == true);
                assert(ahod2Out[2*i].ab[1] == false);

                assert(equalWithAbsError
                       (ahod2Out[2*i].h, half(-.342), half(.0001)));

                assert(equalWithAbsError(ahod2Out[2*i+1].f, 1.2f, .0001f));
                assert(ahod2Out[2*i+1].i == 1);
                assert(ahod2Out[2*i+1].b == false);
                assert(ahod2Out[2*i+1].ab[0] == false);
                assert(ahod2Out[2*i+1].ab[1] == true);
                assert(equalWithAbsError
                       (ahod2Out[2*i+1].h, half(.342), half(.0001)));

            }
        }
    }
}



template <class T>
void
testOpsIntT(Interpreter &interp,
	    int dataSize,
	    T *aOrig,
	    T *bOrig,
	    string ctlFuncName)
{


    FunctionCallPtr func;
    FunctionArgPtr ret;

    // boolean operators on varying data
    {
	func = interp.newFunctionCall(ctlFuncName);
	
	assert(func->numInputArgs() == 2);
	assert(func->numOutputArgs() == 12);

	FunctionArgPtr aArg = func->inputArg(0);
	FunctionArgPtr bArg = func->inputArg(1);

	assert( aArg->isVarying());
	assert( bArg->isVarying());
	
	T * a      = (T*)(aArg->data());
	T * b      = (T*)(bArg->data());


	memcpy(a, aOrig, dataSize*sizeof(T));
	memcpy(b, bOrig, dataSize*sizeof(T));

	func->callFunction(dataSize);

	FunctionArgPtr equalsArg = func->outputArg(0);
	FunctionArgPtr notArg = func->outputArg(1);
	FunctionArgPtr bitAndArg = func->outputArg(2);
	FunctionArgPtr andArg = func->outputArg(3);
	FunctionArgPtr bitOrArg = func->outputArg(4);
	FunctionArgPtr orArg = func->outputArg(5);
	FunctionArgPtr exOrArg = func->outputArg(6);
	FunctionArgPtr greaterArg = func->outputArg(7);
	FunctionArgPtr greaterEqualsArg = func->outputArg(8);
	FunctionArgPtr lessArg = func->outputArg(9);
	FunctionArgPtr lessEqualsArg = func->outputArg(10);
	FunctionArgPtr notEqualsArg = func->outputArg(11);

	assert( equalsArg->isVarying());
	assert( notArg->isVarying());
	bool * equalsData = (bool*)(equalsArg->data());
	bool * notData = (bool*)(notArg->data());
	T * bitAndData = (T*)(bitAndArg->data());
	bool * andData = (bool*)(andArg->data());
	T * bitOrData = (T*)(bitOrArg->data());
	bool * orData = (bool*)(orArg->data());
	T * exOrData = (T*)(exOrArg->data());
	bool * greaterData = (bool*)(greaterArg->data());
	bool * greaterEqualsData = (bool*)(greaterEqualsArg->data());
	bool * lessData = (bool*)(lessArg->data());
	bool * lessEqualsData = (bool*)(lessEqualsArg->data());
	bool * notEqualsData = (bool*)(notEqualsArg->data());


	for(int i = 0; i < dataSize; i++)
	{
 	    assert(equalsData[i] == bool(aOrig[i] == bOrig[i]));
 	    assert(notData[i] ==    bool(!aOrig[i]));
	    assert(bitAndData[i] == T(aOrig[i] & bOrig[i]));
 	    assert(andData[i] ==    bool(aOrig[i] && bOrig[i]));
	    assert(bitOrData[i] ==  T(aOrig[i] | bOrig[i]));
 	    assert(orData[i] ==     bool(aOrig[i] || bOrig[i]));
	    assert(exOrData[i] ==   T(aOrig[i] ^ bOrig[i]));
 	    assert(greaterData[i] == bool(aOrig[i] > bOrig[i]));
 	    assert(greaterEqualsData[i] == bool(aOrig[i] >= bOrig[i]));
 	    assert(lessData[i] == bool(aOrig[i] < bOrig[i]));
 	    assert(lessEqualsData[i] == bool(aOrig[i] <= bOrig[i]));
 	    assert(notEqualsData[i] == bool(aOrig[i] != bOrig[i]));

	}
    }
}



template <class T>
void
testOpsFloatT(Interpreter &interp,
	      int dataSize,
	      T *aOrig,
	      T *bOrig,
	      string ctlFuncName)
{


    FunctionCallPtr func;
    FunctionArgPtr ret;

    // boolean operators on varying data
    {
	func = interp.newFunctionCall(ctlFuncName);
	
	assert(func->numInputArgs() == 2);
	assert(func->numOutputArgs() == 9);

	FunctionArgPtr aArg = func->inputArg(0);
	FunctionArgPtr bArg = func->inputArg(1);

	assert( aArg->isVarying());
	assert( bArg->isVarying());
	
	T * a      = (T*)(aArg->data());
	T * b      = (T*)(bArg->data());


	memcpy(a, aOrig, dataSize*sizeof(T));
	memcpy(b, bOrig, dataSize*sizeof(T));

	func->callFunction(dataSize);

	FunctionArgPtr equalsArg = func->outputArg(0);
	FunctionArgPtr notArg = func->outputArg(1);
	FunctionArgPtr andArg = func->outputArg(2);
	FunctionArgPtr orArg = func->outputArg(3);
	FunctionArgPtr greaterArg = func->outputArg(4);
	FunctionArgPtr greaterEqualsArg = func->outputArg(5);
	FunctionArgPtr lessArg = func->outputArg(6);
	FunctionArgPtr lessEqualsArg = func->outputArg(7);
	FunctionArgPtr notEqualsArg = func->outputArg(8);

	assert( equalsArg->isVarying());
	assert( notArg->isVarying());
	bool * equalsData = (bool*)(equalsArg->data());
	bool * notData = (bool*)(notArg->data());
	bool * andData = (bool*)(andArg->data());
	bool * orData = (bool*)(orArg->data());
	bool * greaterData = (bool*)(greaterArg->data());
	bool * greaterEqualsData = (bool*)(greaterEqualsArg->data());
	bool * lessData = (bool*)(lessArg->data());
	bool * lessEqualsData = (bool*)(lessEqualsArg->data());
	bool * notEqualsData = (bool*)(notEqualsArg->data());


	for(int i = 0; i < dataSize; i++)
	{
 	    assert(equalsData[i] == bool(aOrig[i] == bOrig[i]));
 	    assert(notData[i] ==    bool(!aOrig[i]));
 	    assert(andData[i] ==    bool(aOrig[i] && bOrig[i]));
 	    assert(orData[i] ==     bool(aOrig[i] || bOrig[i]));
 	    assert(greaterData[i] == bool(aOrig[i] > bOrig[i]));
 	    assert(greaterEqualsData[i] == bool(aOrig[i] >= bOrig[i]));
 	    assert(lessData[i] == bool(aOrig[i] < bOrig[i]));
 	    assert(lessEqualsData[i] == bool(aOrig[i] <= bOrig[i]));
 	    assert(notEqualsData[i] == bool(aOrig[i] != bOrig[i]));
	}
    }
}


const int dataSize = 30;


void
getRangeBool (bool *aData, 
	      bool *bData,
	      int dataSize)
{
    bool b = false;
    for(int i = 0; i < dataSize; i++)
    {
	if( i%3 == 0)  
	    b = !b;
	aData[i] = i%2 == 0;
	bData[i] = bool(b);
    }
    

}

void
testOps(Interpreter &interp)
{
    const int dataSize = 23;

    {   // boolean operands
	bool a[dataSize], b[dataSize];
	getRangeBool(a, b, dataSize);
	testOpsIntT(interp, dataSize, a, b, "testVarying::boolOps");
    }

    { 	// float operands
	float a[dataSize], b[dataSize];
	getRange(a, dataSize, 1);
	getRange(b, dataSize, 5);
	testOpsFloatT(interp, dataSize, a, b, "testVarying::floatOps");
    }

    { 	// half operands
	half a[dataSize], b[dataSize];
	getRange(a, dataSize, 1);
	getRange(b, dataSize, 5);
	testOpsFloatT(interp, dataSize, a, b, "testVarying::halfOps");
    }

    {
	// int operands
	int a[dataSize], b[dataSize];
	getRange(a, dataSize, 1);
	getRange(b, dataSize, 5);

	testOpsIntT(interp, dataSize, a, b, "testVarying::intOps");
    }

    {
	// unsigned int operands
	unsigned int a[dataSize], b[dataSize];
	getRange(a, dataSize, 1);
	getRange(b, dataSize, 5);

	testOpsIntT(interp, dataSize, a, b, "testVarying::unsignedOps");
   }
}

void
testDefaults(Interpreter &interp)
{
    const int dataSize = 4;

    // int operands
    bool  b1[dataSize], b12[dataSize];
    float f1[dataSize], f12[dataSize];
    half  h1[dataSize], h12[dataSize];
    int   i1[dataSize], i12[dataSize];
    Hodgepodge hod1[dataSize];
    Hodgepodge ahod1[2*dataSize];

    getRange(b1, dataSize, 1);
    getRange(f1, dataSize, 1);
    getRange(h1, dataSize, 1);
    getRange(i1, dataSize, 1);

    getRange(b12, dataSize, 2);
    getRange(f12, dataSize, 2);
    getRange(h12, dataSize, 2);
    getRange(i12, dataSize, 2);

    for(int i = 0; i < dataSize; i++)
    {
	hod1[i].f = f1[i];
	hod1[i].i = i1[i];
	hod1[i].b = b1[i];
	hod1[i].ab[0] = b1[i];
	hod1[i].ab[1] = !b1[i];
	hod1[i].h = h1[i];

	ahod1[2*i].f = f1[i];
	ahod1[2*i].i = i1[i];
	ahod1[2*i].b = b1[i];
	ahod1[2*i].ab[0] = b1[i];
	ahod1[2*i].ab[1] = !b1[i];
	ahod1[2*i].h = h1[i];

	ahod1[2*i+1].f = f12[i];
	ahod1[2*i+1].i = i12[i];
	ahod1[2*i+1].b = b12[i];
	ahod1[2*i+1].ab[0] = b12[i];
	ahod1[2*i+1].ab[1] = !b12[i];
	ahod1[2*i+1].h = h12[i];
    }
    
    varyingDefaults(interp, dataSize, 
		    b1,
		    f1,
		    h1,
		    i1,
		    hod1,
		    ahod1,
		    "testVarying::varyingDefaults");
}






void
testHodgeGen(Interpreter &interp,
	  const int dataSize,
	  Hodgepodge aOrig[],
	  Hodgepodge bOrig[])
{
    FunctionCallPtr func;
    FunctionArgPtr ret;

    // boolean operators on varying data
    {
	func = interp.newFunctionCall("testVarying::structVary");
	
	assert(func->numInputArgs() == 2);
	assert(func->numOutputArgs() == 5);

	FunctionArgPtr aHoArg = func->inputArg(0);
	FunctionArgPtr bHoArg = func->inputArg(1);

	assert( aHoArg->isVarying());
	
	Hodgepodge * aHo  = (Hodgepodge*)(aHoArg->data());
	Hodgepodge * bHo  = (Hodgepodge*)(bHoArg->data());

	memcpy(aHo, aOrig, sizeof (Hodgepodge) * dataSize);
	memcpy(bHo, bOrig, sizeof (Hodgepodge) * dataSize);

	func->callFunction(dataSize);

	FunctionArgPtr hoArg = func->outputArg(0);
	FunctionArgPtr fArg = func->outputArg(1);
	FunctionArgPtr iArg = func->outputArg(2);
	FunctionArgPtr bArg = func->outputArg(3);
	FunctionArgPtr hArg = func->outputArg(4);

	Hodgepodge *ho = (Hodgepodge*)(hoArg->data());
	float *f = (float *)(fArg->data());
	int *i = (int *)(iArg->data());
	bool *b = (bool *)(bArg->data());
	

	for(int j = 0; j < dataSize; j++)
	{
    	    assert(ho[j].f == aOrig[j].f);
	    assert(ho[j].i == aOrig[j].i);
  	    assert(ho[j].b == aOrig[j].b);
  	    assert(ho[j].h == aOrig[j].h);

	    assert(f[j] == aOrig[j].f);
	    assert(i[j] == aOrig[j].i);
	    assert(b[j] == aOrig[j].b);
	}
    }
}



void
testStruct(Interpreter &interp)
{
    const int dataSize = 30;

    int ai[dataSize];
    int bi[dataSize];

    float af[dataSize];
    float bf[dataSize];

    bool ab[dataSize];
    bool bb[dataSize];

    half ah[dataSize];
    half bh[dataSize];

    getRange(ai, dataSize, 1);
    getRange(bi, dataSize, 7);

    getRange(af, dataSize, 1);
    getRange(bf, dataSize, 5);

    getRange(ah, dataSize, 1);
    getRange(bh, dataSize, 6);

    getRangeBool(ab, bb, dataSize);

    Hodgepodge aHo[dataSize];
    Hodgepodge bHo[dataSize];

    for( int i = 0; i < dataSize; i++)
    {
	aHo[i].f = af[i];
	aHo[i].i = ai[i];
	aHo[i].b = ab[i];
	aHo[i].h = ah[i];
    }

    testHodgeGen(interp, dataSize, aHo, bHo);
}


void
testFunctionCalls (Interpreter &interp, size_t nSamples)
{
    FunctionCallPtr func =
	interp.newFunctionCall ("testVarying::functionCalls");

    assert (func->numInputArgs() == 4);
    assert (func->numOutputArgs() == 3);

    FunctionArgPtr x = func->findInputArg ("x");
    FunctionArgPtr y = func->findInputArg ("y");
    FunctionArgPtr z = func->findInputArg ("z");
    FunctionArgPtr w = func->findInputArg ("w");

    assert (x->type().cast<FloatType>() && x->isVarying());
    assert (y->type().cast<FloatType>() && y->isVarying());
    assert (z->type().cast<FloatType>() && z->isVarying());
    assert (w->type().cast<FloatType>() && w->isVarying());

    for (size_t i = 0; i < nSamples; ++i)
    {
	float &xi = *(float *)(x->data() + x->type()->alignedObjectSize() * i);
	float &yi = *(float *)(y->data() + y->type()->alignedObjectSize() * i);
	float &zi = *(float *)(z->data() + z->type()->alignedObjectSize() * i);
	float &wi = *(float *)(w->data() + w->type()->alignedObjectSize() * i);

	xi = float (i);
	yi = float (i + 1);
	zi = float (i % 4 + 2);
	wi = float (i % 5 + 2);
    }

    func->callFunction (nSamples);

    FunctionArgPtr a = func->findOutputArg ("a");
    FunctionArgPtr b = func->findOutputArg ("b");
    FunctionArgPtr c = func->findOutputArg ("c");

    assert (a->type().cast<FloatType>() && a->isVarying());
    assert (b->type().cast<FloatType>() && b->isVarying());
    assert (c->type().cast<FloatType>() && c->isVarying());

    for (size_t i = 0; i < nSamples; ++i)
    {
	float &xi = *(float *)(x->data() + x->type()->alignedObjectSize() * i);
	float &yi = *(float *)(y->data() + y->type()->alignedObjectSize() * i);
	float &zi = *(float *)(z->data() + z->type()->alignedObjectSize() * i);
	float &wi = *(float *)(w->data() + w->type()->alignedObjectSize() * i);

	float &ai = *(float *)(a->data() + a->type()->alignedObjectSize() * i);
	float &bi = *(float *)(b->data() + b->type()->alignedObjectSize() * i);
	float &ci = *(float *)(c->data() + c->type()->alignedObjectSize() * i);

	assert (equalWithRelError (ai, float (sqrt (xi)), 0.0001f));
	assert (equalWithRelError (bi, yi * yi, 0.0001f));
	assert (equalWithRelError (ci, float (pow (zi, wi)), 0.0001f));
    }
}


void
testVarying()
{
    cout << "Testing varying data." << endl;
    try
    {
	SimdInterpreter interp;
	interp.loadModule ("testVarying");

 	testOps(interp);
 	testStruct(interp);
	testDefaults(interp);

	testFunctionCalls(interp, interp.maxSamples());

	if (interp.maxSamples() >= 2)
	    testFunctionCalls(interp, interp.maxSamples() / 2);

	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << e.what() << endl;
	assert (false);
    }
}
