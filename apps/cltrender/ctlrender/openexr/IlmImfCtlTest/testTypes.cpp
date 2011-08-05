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

#include <ImfCtlApplyTransforms.h>
#include <CtlSimdInterpreter.h>
#include <ImfHeader.h>
#include <ImfFrameBuffer.h>
#include <ImfArray.h>
#include <ImfBoxAttribute.h>
#include <ImfChromaticitiesAttribute.h>
#include <ImfDoubleAttribute.h>
#include <ImfFloatAttribute.h>
#include <ImfIntAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfVecAttribute.h>
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
runTest (Interpreter &interp)
{
    Rand48 rand (0);

    //
    // Create transformNames
    //

    StringList transformNames;
    transformNames.push_back ("function3");

    //
    // Create inHeader, envHeader, outHeader
    //

    Header inHeader;

    inHeader.insert ("box2i1",
	Box2iAttribute (Box2i (V2i (12, 34), V2i (65, 78))));

    inHeader.insert ("box2f1",
	Box2fAttribute (Box2f (V2f (12.5, 34.5), V2f (65.5, 78.5))));

    inHeader.insert ("chr1",
	ChromaticitiesAttribute (Chromaticities (V2f (23.4, 56.7),
						 V2f (89.0, 12.3),
						 V2f (45.6, 78.9),
						 V2f (12.5, 34.7))));

    inHeader.insert ("d1", DoubleAttribute (123.5));
    inHeader.insert ("f1", FloatAttribute (678.91));
    inHeader.insert ("i1", IntAttribute (23456));

    inHeader.insert ("m33f1",
	M33fAttribute (M33f (11.5, 13.5, 14.5,
			     31.5, 33.5, 34.5,
			     41.5, 43.5, 44.5)));

    inHeader.insert ("m44f1",
	M44fAttribute (M44f (11.2, 12.2, 13.2, 14.2,
			     21.2, 22.2, 23.2, 24.2,
			     31.2, 32.2, 33.2, 34.2,
			     41.2, 42.2, 43.2, 44.2)));

    inHeader.insert ("v2f1", V2fAttribute (V2f (1.5, 2.5)));
    inHeader.insert ("v2i1", V2iAttribute (V2i (5, 2)));
    inHeader.insert ("v3f1", V3fAttribute (V3f (13.4, 11.5, 12.5)));
    inHeader.insert ("v3i1", V3iAttribute (V3i (123, 456, 789)));

    Header envHeader;

    Header outHeader;

    outHeader.insert ("box2i2",
	Box2iAttribute (Box2i (V2i (0, 0), V2i (0, 0))));

    outHeader.insert ("box2f2",
	Box2fAttribute (Box2f (V2i (0, 0), V2i (0, 0))));

    outHeader.insert ("chr2",
	ChromaticitiesAttribute (Chromaticities (V2f (0, 0),
						 V2f (0, 0),
						 V2f (0, 0),
						 V2f (0, 0))));

    outHeader.insert ("d2", DoubleAttribute (0));
    outHeader.insert ("f2", FloatAttribute (0));
    outHeader.insert ("i2", IntAttribute (0));

    outHeader.insert ("m33f2",
	M33fAttribute (M33f (0, 0, 0,
			     0, 0, 0,
			     0, 0, 0)));

    outHeader.insert ("m44f2",
	M44fAttribute (M44f (0, 0, 0, 0,
			     0, 0, 0, 0,
			     0, 0, 0, 0,
			     0, 0, 0, 0)));

    outHeader.insert ("v2f2", V2fAttribute (V2f (0, 0)));
    outHeader.insert ("v2i2", V2iAttribute (V2i (0, 0)));
    outHeader.insert ("v3f2", V3fAttribute (V3f (0, 0, 0)));
    outHeader.insert ("v3i2", V3iAttribute (V3i (0, 0, 0)));

    //
    // Create inFb, outFb
    //

    Box2i tw (V2i (1110, -80), V2i (1200, 100));

    size_t twWidth  = (tw.max.x - tw.min.x + 1);
    size_t twHeight = (tw.max.y - tw.min.y + 1);
    size_t nPixels  = twWidth * twHeight;

    size_t xStride    = 1;
    size_t yStride    = twWidth;
    size_t baseOffset = tw.min.y * yStride + tw.min.x * xStride;

    Array <half> vh1 (nPixels);
    Array <float> vf1 (nPixels);
    Array <unsigned int> vu1 (nPixels);

    for (size_t i = 0; i < nPixels; ++i)
    {
	vh1[i] = rand.nextf();
	vf1[i] = rand.nextf();
	vu1[i] = (unsigned int) rand.nexti();
    }

    FrameBuffer inFb;

    inFb.insert
	("vh1", Slice (HALF,
		       (char *)&vh1[0] - baseOffset * sizeof (half),
		       xStride * sizeof (half),
		       yStride * sizeof (half)));

    inFb.insert
		("vf1", Slice (Imf::FLOAT,
		       (char *)&vf1[0] - baseOffset * sizeof (float),
		       xStride * sizeof (float),
		       yStride * sizeof (float)));

    inFb.insert
		("vu1", Slice (Imf::UINT,
		       (char *)&vu1[0] - baseOffset * sizeof (unsigned int),
		       xStride * sizeof (unsigned int),
		       yStride * sizeof (unsigned int)));

    Array <half> vh2 (nPixels);
    Array <float> vf2 (nPixels);
    Array <unsigned int> vu2 (nPixels);

    for (size_t i = 0; i < nPixels; ++i)
    {
	vh2[i] = 0;
	vf2[i] = 0;
	vu2[i] = 0;
    }

    FrameBuffer outFb;

    outFb.insert
	("vh2", Slice (HALF,
		       (char *)&vh2[0] - baseOffset * sizeof (half),
		       xStride * sizeof (half),
		       yStride * sizeof (half)));

    outFb.insert
		("vf2", Slice (Imf::FLOAT,
		       (char *)&vf2[0] - baseOffset * sizeof (float),
		       xStride * sizeof (float),
		       yStride * sizeof (float)));

    outFb.insert
		("vu2", Slice (Imf::UINT,
		       (char *)&vu2[0] - baseOffset * sizeof (unsigned int),
		       xStride * sizeof (unsigned int),
		       yStride * sizeof (unsigned int)));

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

    assert (outHeader.typedAttribute<Box2iAttribute>("box2i2").value() ==
	     inHeader.typedAttribute<Box2iAttribute>("box2i1").value());

    assert (outHeader.typedAttribute<Box2fAttribute>("box2f2").value() ==
	     inHeader.typedAttribute<Box2fAttribute>("box2f1").value());

    const Chromaticities &chr2 =
	outHeader.typedAttribute<ChromaticitiesAttribute>("chr2").value();

    const Chromaticities &chr1 =
	 inHeader.typedAttribute<ChromaticitiesAttribute>("chr1").value();

    assert (chr2.red == chr1.red);
    assert (chr2.green == chr1.green);
    assert (chr2.blue == chr1.blue);
    assert (chr2.white == chr1.white);

    assert (outHeader.typedAttribute<DoubleAttribute>("d2").value() ==
	     inHeader.typedAttribute<DoubleAttribute>("d1").value());

    assert (outHeader.typedAttribute<FloatAttribute>("f2").value() ==
	     inHeader.typedAttribute<FloatAttribute>("f1").value());

    assert (outHeader.typedAttribute<IntAttribute>("i2").value() ==
	     inHeader.typedAttribute<IntAttribute>("i1").value());

    assert (outHeader.typedAttribute<M33fAttribute>("m33f2").value() ==
	     inHeader.typedAttribute<M33fAttribute>("m33f1").value());

    assert (outHeader.typedAttribute<M44fAttribute>("m44f2").value() ==
	     inHeader.typedAttribute<M44fAttribute>("m44f1").value());

    assert (outHeader.typedAttribute<V2iAttribute>("v2i2").value() ==
	     inHeader.typedAttribute<V2iAttribute>("v2i1").value());

    assert (outHeader.typedAttribute<V2fAttribute>("v2f2").value() ==
	     inHeader.typedAttribute<V2fAttribute>("v2f1").value());

    assert (outHeader.typedAttribute<V3iAttribute>("v3i2").value() ==
	     inHeader.typedAttribute<V3iAttribute>("v3i1").value());

    assert (outHeader.typedAttribute<V3fAttribute>("v3f2").value() ==
	     inHeader.typedAttribute<V3fAttribute>("v3f1").value());

    //
    // Check data in outFb
    //

    for (size_t i = 0; i < nPixels; ++i)
    {
	assert (vh2[i] == vh1[i]);
	assert (vf2[i] == vf1[i]);
	assert (vu2[i] == vu1[i]);
    }
}


} // namespace


void
testTypes ()
{
    try
    {
	cout << "Testing function argument types" << endl;

	SimdInterpreter interp;
	runTest (interp);

	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << e.what() << endl;
	assert (false);
    }
}
