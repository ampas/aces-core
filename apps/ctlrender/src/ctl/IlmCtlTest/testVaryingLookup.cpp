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
#include <half.h>
#include <iostream>
#include <exception>
#include <assert.h>
#include <sstream>

using namespace Ctl;
using namespace std;

void
testLookup1D (Interpreter &interp)
{
    cout << "1D, linear, regular spacing" << endl;

    FunctionCallPtr func = interp.newFunctionCall ("varyingLookup1D");
    assert (func);

    FunctionArgPtr pMin = func->findInputArg ("pMin");
    assert (pMin);
    assert (pMin->type().cast<FloatType>());
    assert (pMin->isVarying());
    char *pMinData = pMin->data();
    size_t pMinSize = pMin->type()->alignedObjectSize();

    FunctionArgPtr pMax = func->findInputArg ("pMax");
    assert (pMax);
    assert (pMax->type().cast<FloatType>());
    assert (pMax->isVarying());
    char *pMaxData = pMax->data();
    size_t pMaxSize = pMax->type()->alignedObjectSize();

    FunctionArgPtr p = func->findInputArg ("p");
    assert (p);
    assert (p->type().cast<FloatType>());
    assert (p->isVarying());
    char *pData = p->data();
    size_t pSize = p->type()->alignedObjectSize();

    assert (interp.maxSamples() >= 10);

    *(float *)(pMinData + pMinSize * 0) = 10.0;
    *(float *)(pMinData + pMinSize * 1) = 10.0;
    *(float *)(pMinData + pMinSize * 2) = 0.0;
    *(float *)(pMinData + pMinSize * 3) = 0.0;
    *(float *)(pMinData + pMinSize * 4) = 0.0;
    *(float *)(pMinData + pMinSize * 5) = 0.0;
    *(float *)(pMinData + pMinSize * 6) = 10.0;
    *(float *)(pMinData + pMinSize * 7) = 10.0;
    *(float *)(pMinData + pMinSize * 8) = 10.0;
    *(float *)(pMinData + pMinSize * 9) = 10.0;

    *(float *)(pMaxData + pMaxSize * 0) = 40.0;
    *(float *)(pMaxData + pMaxSize * 1) = 40.0;
    *(float *)(pMaxData + pMaxSize * 2) = 3.0;
    *(float *)(pMaxData + pMaxSize * 3) = 3.0;
    *(float *)(pMaxData + pMaxSize * 4) = 3.0;
    *(float *)(pMaxData + pMaxSize * 5) = 3.0;
    *(float *)(pMaxData + pMaxSize * 6) = 40.0;
    *(float *)(pMaxData + pMaxSize * 7) = 40.0;
    *(float *)(pMaxData + pMaxSize * 8) = 40.0;
    *(float *)(pMaxData + pMaxSize * 9) = 40.0;

    *(float *)(pData + pSize * 0) = 0.0;
    *(float *)(pData + pSize * 1) = 10.0;
    *(float *)(pData + pSize * 2) = 0.25;
    *(float *)(pData + pSize * 3) = 0.5;
    *(float *)(pData + pSize * 4) = 0.75;
    *(float *)(pData + pSize * 5) = 1.0;
    *(float *)(pData + pSize * 6) = 15.0;
    *(float *)(pData + pSize * 7) = 35.0;
    *(float *)(pData + pSize * 8) = 40.0;
    *(float *)(pData + pSize * 9) = 50.0;

    func->callFunction (10);

    FunctionArgPtr q = func->findOutputArg ("q");
    assert (q);
    assert (q->type().cast<FloatType>());
    assert (q->isVarying());
    char *qData = q->data();
    size_t qSize = q->type()->alignedObjectSize();

    assert (*(float *)(qData + qSize * 0) == 2);
    assert (*(float *)(qData + qSize * 1) == 2);
    assert (*(float *)(qData + qSize * 2) == 3);
    assert (*(float *)(qData + qSize * 3) == 4);
    assert (*(float *)(qData + qSize * 4) == 5);
    assert (*(float *)(qData + qSize * 5) == 6);
    assert (*(float *)(qData + qSize * 6) == 4);
    assert (*(float *)(qData + qSize * 7) == 4);
    assert (*(float *)(qData + qSize * 8) == 7);
    assert (*(float *)(qData + qSize * 9) == 7);
}



void
testLookupCubic1D (Interpreter &interp)
{
    cout << "1D, cubic, regular spacing" << endl;

    FunctionCallPtr func = interp.newFunctionCall ("varyingLookupCubic1D");
    assert (func);

    FunctionArgPtr pMin = func->findInputArg ("pMin");
    assert (pMin);
    assert (pMin->type().cast<FloatType>());
    assert (pMin->isVarying());
    char *pMinData = pMin->data();
    size_t pMinSize = pMin->type()->alignedObjectSize();

    FunctionArgPtr pMax = func->findInputArg ("pMax");
    assert (pMax);
    assert (pMax->type().cast<FloatType>());
    assert (pMax->isVarying());
    char *pMaxData = pMax->data();
    size_t pMaxSize = pMax->type()->alignedObjectSize();

    FunctionArgPtr p = func->findInputArg ("p");
    assert (p);
    assert (p->type().cast<FloatType>());
    assert (p->isVarying());
    char *pData = p->data();
    size_t pSize = p->type()->alignedObjectSize();

    assert (interp.maxSamples() >= 9);

    *(float *)(pMinData + pMinSize * 0) = 10.0;
    *(float *)(pMinData + pMinSize * 1) = 10.0;
    *(float *)(pMinData + pMinSize * 2) = 1.0;
    *(float *)(pMinData + pMinSize * 3) = 1.0;
    *(float *)(pMinData + pMinSize * 4) = 1.0;
    *(float *)(pMinData + pMinSize * 5) = 1.0;
    *(float *)(pMinData + pMinSize * 6) = 10.0;
    *(float *)(pMinData + pMinSize * 7) = 10.0;
    *(float *)(pMinData + pMinSize * 8) = 10.0;

    *(float *)(pMaxData + pMaxSize * 0) = 40.0;
    *(float *)(pMaxData + pMaxSize * 1) = 40.0;
    *(float *)(pMaxData + pMaxSize * 2) = 3.0;
    *(float *)(pMaxData + pMaxSize * 3) = 3.0;
    *(float *)(pMaxData + pMaxSize * 4) = 3.0;
    *(float *)(pMaxData + pMaxSize * 5) = 3.0;
    *(float *)(pMaxData + pMaxSize * 6) = 40.0;
    *(float *)(pMaxData + pMaxSize * 7) = 40.0;
    *(float *)(pMaxData + pMaxSize * 8) = 40.0;

    *(float *)(pData + pSize * 0) = 0.0;
    *(float *)(pData + pSize * 1) = 10.0;
    *(float *)(pData + pSize * 2) = 0.0;
    *(float *)(pData + pSize * 3) = 1.0;
    *(float *)(pData + pSize * 4) = 2.0;
    *(float *)(pData + pSize * 5) = 3.0;
    *(float *)(pData + pSize * 6) = 40.0;
    *(float *)(pData + pSize * 7) = 50.0;
    *(float *)(pData + pSize * 8) = 25.0;

    func->callFunction (10);

    FunctionArgPtr q = func->findOutputArg ("q");
    assert (q);
    assert (q->type().cast<FloatType>());
    assert (q->isVarying());
    char *qData = q->data();
    size_t qSize = q->type()->alignedObjectSize();

    assert (*(float *)(qData + qSize * 0) == 2);
    assert (*(float *)(qData + qSize * 1) == 2);
    assert (*(float *)(qData + qSize * 2) == 2);
    assert (*(float *)(qData + qSize * 3) == 2);
    assert (*(float *)(qData + qSize * 4) == 3);
    assert (*(float *)(qData + qSize * 5) == 4);
    assert (*(float *)(qData + qSize * 6) == 4);
    assert (*(float *)(qData + qSize * 7) == 4);
    assert (*(float *)(qData + qSize * 8) == 3);
}


void
testLookup3D (Interpreter &interp)
{
    cout << "3D, linear, regular spacing" << endl;

    FunctionCallPtr func = interp.newFunctionCall ("varyingLookup3D");
    assert (func);

    FunctionArgPtr pMin = func->findInputArg ("pMin");
    assert (pMin);
    assert (pMin->type().cast<ArrayType>());
    assert (pMin->type().cast<ArrayType>()->elementType().cast<FloatType>());
    assert (pMin->type().cast<ArrayType>()->size() == 3);
    assert (pMin->isVarying());
    char *pMinData = pMin->data();
    size_t pMinSize = pMin->type()->alignedObjectSize();
    size_t pMinESize = pMin->type().cast<ArrayType>()->elementSize();

    FunctionArgPtr pMax = func->findInputArg ("pMax");
    assert (pMax);
    assert (pMax->type().cast<ArrayType>());
    assert (pMax->type().cast<ArrayType>()->elementType().cast<FloatType>());
    assert (pMax->type().cast<ArrayType>()->size() == 3);
    assert (pMax->isVarying());
    char *pMaxData = pMax->data();
    size_t pMaxSize = pMax->type()->alignedObjectSize();
    size_t pMaxESize = pMax->type().cast<ArrayType>()->elementSize();

    FunctionArgPtr p1 = func->findInputArg ("p1");
    assert (p1);
    assert (p1->type().cast<ArrayType>());
    assert (p1->type().cast<ArrayType>()->elementType().cast<FloatType>());
    assert (p1->type().cast<ArrayType>()->size() == 3);
    assert (p1->isVarying());
    char *p1Data = p1->data();
    size_t p1Size = p1->type()->alignedObjectSize();
    size_t p1ESize = p1->type().cast<ArrayType>()->elementSize();

    FunctionArgPtr p20 = func->findInputArg ("p20");
    assert (p20);
    assert (p20->type().cast<FloatType>());
    assert (p20->isVarying());
    char *p20Data = p20->data();
    size_t p20Size = p20->type()->alignedObjectSize();

    FunctionArgPtr p21 = func->findInputArg ("p21");
    assert (p21);
    assert (p21->type().cast<FloatType>());
    assert (p21->isVarying());
    char *p21Data = p21->data();
    size_t p21Size = p21->type()->alignedObjectSize();

    FunctionArgPtr p22 = func->findInputArg ("p22");
    assert (p22);
    assert (p22->type().cast<FloatType>());
    assert (p22->isVarying());
    char *p22Data = p22->data();
    size_t p22Size = p22->type()->alignedObjectSize();

    FunctionArgPtr p30 = func->findInputArg ("p30");
    assert (p30);
    assert (p30->type().cast<HalfType>());
    assert (p30->isVarying());
    char *p30Data = p30->data();
    size_t p30Size = p30->type()->alignedObjectSize();

    FunctionArgPtr p31 = func->findInputArg ("p31");
    assert (p31);
    assert (p31->type().cast<HalfType>());
    assert (p31->isVarying());
    char *p31Data = p31->data();
    size_t p31Size = p31->type()->alignedObjectSize();

    FunctionArgPtr p32 = func->findInputArg ("p32");
    assert (p32);
    assert (p32->type().cast<HalfType>());
    assert (p32->isVarying());
    char *p32Data = p32->data();
    size_t p32Size = p32->type()->alignedObjectSize();

    assert (interp.maxSamples() >= 2 * 3 * 4);

    for (size_t i = 0; i < 2; ++i)
    {
	for (size_t j = 0; j < 3; ++j)
	{
	    for (size_t k = 0; k < 4; ++k)
	    {
		size_t m = i * 3 * 4 + j * 4 + k;

		*(float *)(pMinData + pMinSize * m + pMinESize * 0) = 2 + i;
		*(float *)(pMinData + pMinSize * m + pMinESize * 1) = 4 + j;
		*(float *)(pMinData + pMinSize * m + pMinESize * 2) = 6 + k;

		*(float *)(pMaxData + pMaxSize * m + pMaxESize * 0) = 3 + i;
		*(float *)(pMaxData + pMaxSize * m + pMaxESize * 1) = 6 + j;
		*(float *)(pMaxData + pMaxSize * m + pMaxESize * 2) = 9 + k;

		*(float *)(p1Data + p1Size * m + p1ESize * 0) = 2 * i + 2;
		*(float *)(p1Data + p1Size * m + p1ESize * 1) = 2 * j + 4;
		*(float *)(p1Data + p1Size * m + p1ESize * 2) = 2 * k + 6;

		*(float *)(p20Data + p20Size * m) = 2 * i + 2;
		*(float *)(p21Data + p21Size * m) = 2 * j + 4;
		*(float *)(p22Data + p22Size * m) = 2 * k + 6;

		*(half *)(p30Data + p30Size * m) = 2 * i + 2;
		*(half *)(p31Data + p31Size * m) = 2 * j + 4;
		*(half *)(p32Data + p32Size * m) = 2 * k + 6;
	    }
	}
    }

    func->callFunction (2 * 3 * 4);

    FunctionArgPtr q1 = func->findOutputArg ("q1");
    assert (q1);
    assert (q1->type().cast<ArrayType>());
    assert (q1->type().cast<ArrayType>()->elementType().cast<FloatType>());
    assert (q1->type().cast<ArrayType>()->size() == 3);
    assert (q1->isVarying());
    char *q1Data = q1->data();
    size_t q1Size = q1->type()->alignedObjectSize();
    size_t q1ESize = q1->type().cast<ArrayType>()->elementSize();

    FunctionArgPtr q20 = func->findOutputArg ("q20");
    assert (q20);
    assert (q20->type().cast<FloatType>());
    assert (q20->isVarying());
    char *q20Data = q20->data();
    size_t q20Size = q20->type()->alignedObjectSize();

    FunctionArgPtr q21 = func->findOutputArg ("q21");
    assert (q21);
    assert (q21->type().cast<FloatType>());
    assert (q21->isVarying());
    char *q21Data = q21->data();
    size_t q21Size = q21->type()->alignedObjectSize();

    FunctionArgPtr q22 = func->findOutputArg ("q22");
    assert (q22);
    assert (q22->type().cast<FloatType>());
    assert (q22->isVarying());
    char *q22Data = q22->data();
    size_t q22Size = q22->type()->alignedObjectSize();

    FunctionArgPtr q30 = func->findOutputArg ("q30");
    assert (q30);
    assert (q30->type().cast<HalfType>());
    assert (q30->isVarying());
    char *q30Data = q30->data();
    size_t q30Size = q30->type()->alignedObjectSize();

    FunctionArgPtr q31 = func->findOutputArg ("q31");
    assert (q31);
    assert (q31->type().cast<HalfType>());
    assert (q31->isVarying());
    char *q31Data = q31->data();
    size_t q31Size = q31->type()->alignedObjectSize();

    FunctionArgPtr q32 = func->findOutputArg ("q32");
    assert (q32);
    assert (q32->type().cast<HalfType>());
    assert (q32->isVarying());
    char *q32Data = q32->data();
    size_t q32Size = q32->type()->alignedObjectSize();

    for (size_t i = 0; i < 2; ++i)
    {
	for (size_t j = 0; j < 3; ++j)
	{
	    for (size_t k = 0; k < 4; ++k)
	    {
		size_t m = i * 3 * 4 + j * 4 + k;

		assert (*(float *)(q1Data + q1Size * m + q1ESize * 0) == i);
		assert (*(float *)(q1Data + q1Size * m + q1ESize * 1) == j);
		assert (*(float *)(q1Data + q1Size * m + q1ESize * 2) == k);

		assert (*(float *)(q20Data + q20Size * m) == i);
		assert (*(float *)(q21Data + q21Size * m) == j);
		assert (*(float *)(q22Data + q22Size * m) == k);

		assert (*(half *)(q30Data + q30Size * m) == i);
		assert (*(half *)(q31Data + q31Size * m) == j);
		assert (*(half *)(q32Data + q32Size * m) == k);
	    }
	}
    }
}


void
testInterpolate1D (Interpreter &interp)
{
    cout << "1D, linear, random spacing" << endl;

    FunctionCallPtr func =
	interp.newFunctionCall ("varyingInterpolate1D");

    assert (func);

    FunctionArgPtr p = func->findInputArg ("p");
    assert (p);
    assert (p->type().cast<FloatType>());
    assert (p->isVarying());
    char *pData = p->data();
    size_t pSize = p->type()->alignedObjectSize();

    assert (interp.maxSamples() >= 6);

    *(float *)(pData + pSize * 0) = -1.0;
    *(float *)(pData + pSize * 1) = 0.0;
    *(float *)(pData + pSize * 2) = 1;
    *(float *)(pData + pSize * 3) = 2;
    *(float *)(pData + pSize * 4) = 3;
    *(float *)(pData + pSize * 5) = 4;

    func->callFunction (6);

    FunctionArgPtr q = func->findOutputArg ("q");
    assert (q);
    assert (q->type().cast<FloatType>());
    assert (q->isVarying());
    char *qData = q->data();
    size_t qSize = q->type()->alignedObjectSize();

    assert (*(float *)(qData + qSize * 0) == 2);
    assert (*(float *)(qData + qSize * 1) == 2);
    assert (*(float *)(qData + qSize * 2) == 2);
    assert (*(float *)(qData + qSize * 3) == 3);
    assert (*(float *)(qData + qSize * 4) == 4);
    assert (*(float *)(qData + qSize * 5) == 4);
}


void
testInterpolateCubic1D (Interpreter &interp)
{
    cout << "1D, cubic, random spacing" << endl;

    FunctionCallPtr func =
	interp.newFunctionCall ("varyingInterpolateCubic1D");

    assert (func);

    FunctionArgPtr p = func->findInputArg ("p");
    assert (p);
    assert (p->type().cast<FloatType>());
    assert (p->isVarying());
    char *pData = p->data();
    size_t pSize = p->type()->alignedObjectSize();

    assert (interp.maxSamples() >= 6);

    *(float *)(pData + pSize * 0) = -1.0;
    *(float *)(pData + pSize * 1) = 0.0;
    *(float *)(pData + pSize * 2) = 1;
    *(float *)(pData + pSize * 3) = 2;
    *(float *)(pData + pSize * 4) = 3;
    *(float *)(pData + pSize * 5) = 4;

    func->callFunction (6);

    FunctionArgPtr q = func->findOutputArg ("q");
    assert (q);
    assert (q->type().cast<FloatType>());
    assert (q->isVarying());
    char *qData = q->data();
    size_t qSize = q->type()->alignedObjectSize();

    assert (*(float *)(qData + qSize * 0) == 3);
    assert (*(float *)(qData + qSize * 1) == 3);
    assert (*(float *)(qData + qSize * 2) == 3);
    assert (*(float *)(qData + qSize * 3) == 4);
    assert (*(float *)(qData + qSize * 4) == 5);
    assert (*(float *)(qData + qSize * 5) == 5);
}


void
testVaryingLookup ()
{
    try
    {
	SimdInterpreter interp;
	interp.loadModule ("testVaryingLookup");

	cout << "Testing table lookups with varying data" << endl;

	testLookup1D (interp);
	testLookupCubic1D (interp);
	testLookup3D (interp);
	testInterpolate1D (interp);
	testInterpolateCubic1D (interp);

	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << endl << e.what() << endl;
	assert (false);
    }
}
