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

void
testNested1(int in1,
	    int in2,
	    int & out1,
	    int & out2,
	    int & out3,
	    int & out4,
	    int & out5)
{
    int cond = in1;

    int i = 0;
    while (i < in2)
    {
	if(in1 > 0)
	{
	    out1 = out1 + 1;
	    return;
	}
	else
	{
	    out2 = out2 +1;
	}

	cond = cond -1;
	out3 = out3 + 1;
	return;
	out4 = out4 + 1;
	i = i+1;
    }
    out5 = out5 + 1;
}



void
testReturn(Interpreter &interp,
	       int dataSize,
	       bool *ifIn,
	       int *whileIn,
	       int *forIn,
	       int *nestIn1,
	       int *nestIn2,
	       string ctlFuncName)
{


    FunctionCallPtr func;
    FunctionArgPtr ret;

    // boolean operators on varying data
    {
	func = interp.newFunctionCall(ctlFuncName);
	
	FunctionArgPtr ifInArg = func->inputArg(0);
	FunctionArgPtr whileInArg = func->inputArg(1);
	FunctionArgPtr forInArg = func->inputArg(2);
	FunctionArgPtr nestIn1Arg = func->inputArg(3);
	FunctionArgPtr nestIn2Arg = func->inputArg(4);

	bool * ifInData      = (bool*)(ifInArg->data());
	bool * whileInData      = (bool*)(whileInArg->data());
	bool * forInData      = (bool*)(forInArg->data());
	bool * nestIn1Data      = (bool*)(nestIn1Arg->data());
	bool * nestIn2Data      = (bool*)(nestIn2Arg->data());


	memcpy(ifInData, ifIn, dataSize*sizeof(bool));
	memcpy(whileInData, whileIn, dataSize*sizeof(int));
	memcpy(forInData, forIn, dataSize*sizeof(int));
	memcpy(nestIn1Data, nestIn1, dataSize*sizeof(int));
	memcpy(nestIn2Data, nestIn2, dataSize*sizeof(int));

	func->callFunction(dataSize);

	FunctionArgPtr ifOutArg = func->outputArg(0);
	FunctionArgPtr whileOutArg = func->outputArg(1);
	FunctionArgPtr forOutArg = func->outputArg(2);
	FunctionArgPtr nestOut1Arg = func->outputArg(3);
	FunctionArgPtr nestOut2Arg = func->outputArg(4);
	FunctionArgPtr nestOut3Arg = func->outputArg(5);
	FunctionArgPtr nestOut4Arg = func->outputArg(6);
	FunctionArgPtr nestOut5Arg = func->outputArg(7);


	int * ifOutData = (int*)(ifOutArg->data());
 	int * whileOutData = (int*)(whileOutArg->data());
 	int * forOutData = (int*)(forOutArg->data());
  	int * nestOut1Data = (int*)(nestOut1Arg->data());
  	int * nestOut2Data = (int*)(nestOut2Arg->data());
   	int * nestOut3Data = (int*)(nestOut3Arg->data());
   	int * nestOut4Data = (int*)(nestOut4Arg->data());
   	int * nestOut5Data = (int*)(nestOut5Arg->data());

	int out1, out2, out3, out4, out5;
	for(int i = 0; i < dataSize; i++)
	{
	    out1 = out2 = out3 = out4 = out5 = 0;

	    if(ifIn[i])
		assert(ifOutData[i] == 1);
	    else
		assert(ifOutData[i] == 2);
		
	    if(whileIn[i] > 0)
		assert(whileOutData[i] == 1);
	    else
		assert(whileOutData[i] == -1);

 	    if(forIn[i] > 0)
 		assert(forOutData[i] == 1);
 	    else
 		assert(forOutData[i] == -1);


	    testNested1(nestIn1[i], nestIn2[i], 
			out1, out2, out3, out4, out5);
	    
	    assert(out1 == nestOut1Data[i]);

 	    assert(out2 == nestOut2Data[i]);
 	    assert(out3 == nestOut3Data[i]);
 	    assert(out4 == nestOut4Data[i]);
 	    assert(out5 == nestOut5Data[i]);
	}
    }
}


void
getRangeBool(bool *aData, int dataSize)
{
    for(int i = 0; i < dataSize; i++)
    {
	aData[i] = (i%2 == 0);
    }
}


void
test1(Interpreter &interp)
{

   const int dataSize = 4;

   bool ifIn[dataSize]    = {true, false, true, false};
   int  whileIn[dataSize] = {2,    -1,     2,    -10  };
   int  forIn[dataSize]   = {2,    0,     -1,    -10  };
   int  nestIn1[dataSize] = {2,    -1,    10,    -10  };
   int  nestIn2[dataSize] = {2,    2,     -1,    -10  };

   testReturn(interp, dataSize,
		   ifIn, whileIn, forIn, nestIn1, nestIn2,
		   "testVaryingReturn::testReturn");

    const int dataSize2 = 30;

    bool ifIn2[dataSize2];
    int  forIn2[dataSize2];
    int  whileIn2[dataSize2];
    int  nestIn3[dataSize2];
    int  nestIn4[dataSize2];

    getRangeBool(ifIn2, dataSize2);
    getRange(whileIn2, dataSize2, 2);
    getRange(forIn2, dataSize2, 3);
    getRange(nestIn3, dataSize2, 5);
    getRange(nestIn4, dataSize2, 6);

    testReturn(interp, dataSize2,
		   ifIn2, whileIn2, forIn2, nestIn3, nestIn4,
		   "testVaryingReturn::testReturn");
}



void
testVaryingReturn()
{
    cout << "Testing varying data with branching & return stmt." << endl;
    try
    {
	SimdInterpreter interp;
	interp.loadModule ("testVaryingReturn");

	test1(interp);

	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << endl << e.what() << endl;
	assert (false);
    }
}
