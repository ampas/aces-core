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
#include <iostream>
#include <exception>
#include <assert.h>
#include <sstream>
#include <CtlFunctionCall.h>
#include <ImathMath.h>
#include <IexBaseExc.h>

using namespace Ctl;
using namespace std;
using namespace Imath;

void
testSimple(SimdInterpreter &interp)
{
    int numTries = 3;

    interp.loadModule ("testCppCall");


    int numArgs;
    FunctionCallPtr func;
    FunctionArgPtr ret;

    // test return value of a simple function
    {
	func = interp.newFunctionCall("cppCall::funcRETi");
	numArgs = func->numInputArgs();
	assert(numArgs == 0);

	for(int i = 0; i < numTries; i++)
	{
	    func->callFunction(1000);
	    ret = func->returnValue();
	    int retVal = *(int*)(ret->data());
	    assert(retVal == 5);
	}
    }

    // test passing & return value of a simple function
    {
	func = interp.newFunctionCall("cppCall::funcINifbRETf");
	numArgs = func->numInputArgs();
	assert(numArgs == 3);
	
	float f = 1.2345;
	FunctionArgPtr arg1 = func->inputArg(1);
	assert(!arg1->isVarying());
	((float*)(arg1->data()))[0] = f;

	for(int i = 0; i < numTries; i++)
	{
	    func->callFunction(1000);
	    ret = func->returnValue();
	    float retVal = *(float*)(ret->data());
	    assert(retVal == f);
	}
    }

    // test passing w/ default value
    {
	func = interp.newFunctionCall("cppCall::funcINifbRETfD");
	numArgs = func->numInputArgs();
	assert(numArgs == 3);
	
	float f = 1.2345;
	FunctionArgPtr arg2 = func->inputArg(2);
	assert(!arg2->isVarying());
	((float*)(arg2->data()))[0] = f;

	for(int i = 0; i < numTries; i++)
	{
	    func->callFunction(1000);
	    ret = func->returnValue();
	    float retVal = *(float*)(ret->data());
	    assert(retVal == f);
	}
    }

    // test default value
    {
	func = interp.newFunctionCall("cppCall::funcINifbRETfD");
	numArgs = func->numInputArgs();
	assert(numArgs == 3);
	
	FunctionArgPtr arg2 = func->inputArg(2);
	assert(!arg2->isVarying());
	arg2->setDefaultValue();

	for(int i = 0; i < numTries; i++)
	{
	    func->callFunction(1000);
	    ret = func->returnValue();
	    float retVal = *(float*)(ret->data());
	    assert(retVal == 2.0);
	}
    }

    // test default value after setting to something else
    {
	func = interp.newFunctionCall("cppCall::funcINifbRETfD");
	numArgs = func->numInputArgs();
	assert(numArgs == 3);
	
	FunctionArgPtr arg2 = func->inputArg(2);
	((float*)(arg2->data()))[0] = 1.2345;
	assert(!arg2->isVarying());
	arg2->setDefaultValue();

	for(int i = 0; i < numTries; i++)
	{
	    func->callFunction(1000);
	    ret = func->returnValue();
	    float retVal = *(float*)(ret->data());
	    assert(retVal == 2.0);
	}
    }

    // call set default value when none exists
    {
	func = interp.newFunctionCall("cppCall::funcINifbRETf");
	numArgs = func->numInputArgs();
	assert(numArgs == 3);
	
	float f = 1.2345;
	FunctionArgPtr arg1 = func->inputArg(1);
	assert(!arg1->isVarying());
	arg1->setDefaultValue();
	((float*)(arg1->data()))[0] = f;
	arg1->setDefaultValue();

	for(int i = 0; i < numTries; i++)
	{
	    func->callFunction(1000);
	    ret = func->returnValue();
	    float retVal = *(float*)(ret->data());
	    assert(retVal == f);
	}
    }
    
    // test value varying
    {
	func = interp.newFunctionCall("cppCall::funcINifbRETfD2");
	numArgs = func->numInputArgs();
	assert(numArgs == 3);
	
	FunctionArgPtr arg2 = func->inputArg(2);
	assert(arg2->isVarying());
	float * data = (float*)(arg2->data());

	int dataSize = 30;
	assert (interp.maxSamples() >= dataSize);

	for(int i = 0; i < dataSize; i++)
	{
	    data[i] = i + .1;
	}

	func->callFunction(dataSize);

	ret = func->returnValue();
	data = (float*)(ret->data());
	for(int i = 0; i < dataSize; i++)
	{
	    assert (equalWithRelError (data[i], i + .2f, 0.00001f));
	}


    }
}

void
testVariable(SimdInterpreter &interp)
{
    interp.loadModule ("testCppCall");

    FunctionCallPtr func;

    // test calling a funcs with variable size arguments
    // this should fail because it is not supported
    {
        try
        {
            func = interp.newFunctionCall("cppCall::funcWVaryiable1");
            assert(false);
        }
        catch(Iex::ArgExc e)
        {
        }
        try
        {
            func = interp.newFunctionCall("cppCall::funcWVaryiable2");
            assert(false);
        }
        catch(Iex::ArgExc e)
        {
        }
        
    }


}

void
testCppCall()
{
    cout << "Testing function calls from C++." << endl;
    try
    {
	SimdInterpreter interp;
	testSimple(interp);
        testVariable(interp);
	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << e.what() << endl;
	assert (false);
    }
}
