///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2003, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include <ImfCtlApplyTransforms.h>
#include <CtlSimdInterpreter.h>
#include <ImfHeader.h>
#include <ImfFrameBuffer.h>
#include <ImfArray.h>
#include <ImfFloatAttribute.h>
#include <ImfThreading.h>
#include <ImathRandom.h>
#include <iostream>
#include <exception>
#include <cassert>

using namespace Ctl;
using namespace Imf;
using namespace ImfCtl;
using namespace Imath;
using namespace std;

namespace {

void
runTest (Interpreter &interp, int numThreads)
{
    cout << "\tnumber of threads = " << numThreads << endl;

    setGlobalThreadCount (numThreads);

    Rand48 rand (numThreads);

    //
    // Create transformNames
    //

    StringList transformNames;
    transformNames.push_back ("function1");
    transformNames.push_back ("function2");

    //
    // Create inHeader, envHeader, outHeader
    //

    Header inHeader;
    inHeader.insert ("a1", FloatAttribute (rand.nextf()));
    inHeader.insert ("f2", FloatAttribute (rand.nextf()));

    Header envHeader;
    envHeader.insert ("b1", FloatAttribute (rand.nextf()));
    envHeader.insert ("g2", FloatAttribute (rand.nextf()));

    Header outHeader;
    outHeader.insert ("a2", FloatAttribute (-1));
    outHeader.insert ("a3", FloatAttribute (-1));
    outHeader.insert ("b3", FloatAttribute (-1));
    outHeader.insert ("c3", FloatAttribute (-1));
    outHeader.insert ("f3", FloatAttribute (-1));
    outHeader.insert ("g3", FloatAttribute (-1));
    outHeader.insert ("i3", FloatAttribute (-1));
    outHeader.insert ("n2", FloatAttribute (-1));

    //
    // Create inFb, outFb
    //

    Box2i tw (V2i (1110, -80), V2i (1200, 100));

    size_t twWidth  = (tw.max.x - tw.min.x + 1);
    size_t twHeight = (tw.max.y - tw.min.y + 1);
    size_t nPixels  = twWidth * twHeight;

    size_t xStride    = sizeof (half);
    size_t yStride    = sizeof (half) * twWidth;
    size_t baseOffset = tw.min.y * yStride + tw.min.x * xStride;

    Array <half> d1 (nPixels);
    Array <half> h2 (nPixels);

    for (size_t i = 0; i < nPixels; ++i)
    {
	d1[i] = rand.nextf();
	h2[i] = rand.nextf();
    }

    FrameBuffer inFb;

    inFb.insert
	("d1", Slice (HALF, (char *)&d1[0] - baseOffset, xStride, yStride));

    inFb.insert
	("h2", Slice (HALF, (char *)&h2[0] - baseOffset, xStride, yStride));

    Array <half> d2 (nPixels);
    Array <half> d3 (nPixels);
    Array <half> e3 (nPixels);
    Array <half> h3 (nPixels);
    Array <half> j3 (nPixels);
    Array <half> l2 (nPixels);

    FrameBuffer outFb;

    outFb.insert
	("d2", Slice (HALF, (char *)&d2[0] - baseOffset, xStride, yStride));

    outFb.insert
	("d3", Slice (HALF, (char *)&d3[0] - baseOffset, xStride, yStride));

    outFb.insert
	("e3", Slice (HALF, (char *)&e3[0] - baseOffset, xStride, yStride));

    outFb.insert
	("h3", Slice (HALF, (char *)&h3[0] - baseOffset, xStride, yStride));

    outFb.insert
	("j3", Slice (HALF, (char *)&j3[0] - baseOffset, xStride, yStride));

    outFb.insert
	("l2", Slice (HALF, (char *)&l2[0] - baseOffset, xStride, yStride));

    //
    // Call functions
    //

    applyTransforms (interp,
		     transformNames,
		     tw,
		     envHeader,
		     inHeader,
		     inFb,
		     outHeader,
		     outFb);

    //
    // Check data in outHeader
    //

    assert (outHeader.typedAttribute<FloatAttribute>("a2").value() ==
	     inHeader.typedAttribute<FloatAttribute>("a1").value());

    assert (outHeader.typedAttribute<FloatAttribute>("a3").value() ==
	     inHeader.typedAttribute<FloatAttribute>("a1").value());

    assert (outHeader.typedAttribute<FloatAttribute>("b3").value() ==
	    envHeader.typedAttribute<FloatAttribute>("b1").value());

    assert (outHeader.typedAttribute<FloatAttribute>("c3").value() == 23.5);

    assert (outHeader.typedAttribute<FloatAttribute>("f3").value() ==
	     inHeader.typedAttribute<FloatAttribute>("f2").value());

    assert (outHeader.typedAttribute<FloatAttribute>("g3").value() ==
	    envHeader.typedAttribute<FloatAttribute>("g2").value());

    assert (outHeader.typedAttribute<FloatAttribute>("i3").value() == 1.0);

    assert (outHeader.typedAttribute<FloatAttribute>("n2").value() == 456.0);

    //
    // Check data in outFb
    //

    for (size_t i = 0; i < nPixels; ++i)
    {
	assert (d2[i] == d1[i]);
	assert (d3[i] == d1[i]);
	assert (e3[i] == 12);
	assert (h3[i] == h2[i]);
	assert (j3[i] == 2.0);
	assert (l2[i] == 234.0);
    }
}


} // namespace


void
testSourceDestination ()
{
    try
    {
	cout << "Testing function argument sources and destinations" << endl;

	SimdInterpreter interp;
	runTest (interp, 0);
	runTest (interp, 1);
	runTest (interp, 2);
	runTest (interp, 3);

	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << e.what() << endl;
	assert (false);
    }
}
