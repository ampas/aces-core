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
//	The Standard Library of C++ functions that can be called from CTL
//	
//	- 3D scattered data interpolation
//
//-----------------------------------------------------------------------------

#include <CtlSimdStdLibInterpolator.h>
#include <CtlSimdStdLibTemplates.h>
#include <CtlSimdStdLibrary.h>
#include <CtlSimdStdTypes.h>
#include <CtlSimdCFunc.h>
#include <CtlRbfInterpolator.h>
#include <ImathFun.h>
#include <cassert>

using namespace Imath;
using namespace std;

namespace Ctl {
namespace {

void
scatteredDataToGrid3D (int dataSize,
		       const Imath::V3f data[][2],
		       const V3f &pMin,
		       const V3f &pMax,
		       const V3i &gridSize,
		       V3f grid[])
{
    RbfInterpolator interp (dataSize, data);

    float s, t;
    V3f p;

    for (int i = 0; i < gridSize.x; ++i)
    {
	s = float (i) / float (gridSize.x - 1);
	t = 1 - s;
	p.x = pMin.x * t + pMax.x * s;

	for (int j = 0; j < gridSize.y; ++j)
	{
	    s = float (j) / float (gridSize.y - 1);
	    t = 1 - s;
	    p.y = pMin.y * t + pMax.y * s;

	    for (int k = 0; k < gridSize.z; ++k)
	    {
		s = float (k) / float (gridSize.z - 1);
		t = 1 - s;
		p.z = pMin.z * t + pMax.z * s;

		grid[(i * gridSize.y + j) * gridSize.z + k] = interp.value (p);
	    }
	}
    }
}


void
simdScatteredDataToGrid3D (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // void scatteredDataToGrid3D (float data[][2][3],
    //                             float pMin[3], float pMax[3],
    //                             output float grid[][][][3])
    //

    const SimdReg &dataSize0  = xcontext.stack().regFpRelative (-1);
    const SimdReg &data       = xcontext.stack().regFpRelative (-2);
    const SimdReg &pMin       = xcontext.stack().regFpRelative (-3);
    const SimdReg &pMax       = xcontext.stack().regFpRelative (-4);
    const SimdReg &gridSize2  = xcontext.stack().regFpRelative (-5);
    const SimdReg &gridSize1  = xcontext.stack().regFpRelative (-6);
    const SimdReg &gridSize0  = xcontext.stack().regFpRelative (-7);
    SimdReg &grid             = xcontext.stack().regFpRelative (-8);

    assert (!dataSize0.isVarying() &&
	    !gridSize0.isVarying() &&
	    !gridSize1.isVarying() &&
	    !gridSize2.isVarying());

    V3i gridSize (*(int *)(gridSize0[0]),
	          *(int *)(gridSize1[0]),
		  *(int *)(gridSize2[0]));

    int dataSize = *(int *)(dataSize0[0]);

    typedef const V3f (*DataPtr)[2];
    typedef V3f *GridPtr;

    if (data.isVarying() ||
        pMin.isVarying() ||
	pMax.isVarying())
    {
	grid.setVarying (true);

	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    if (mask[i])
	    {
		scatteredDataToGrid3D (dataSize,
				       (DataPtr)(data[i]),
				       *(const V3f *)(pMin[i]),
				       *(const V3f *)(pMax[i]),
				       gridSize,
				       (GridPtr)(grid[i]));
	    }
	}
    }
    else
    {
	grid.setVarying (false);

	scatteredDataToGrid3D (dataSize,
			       (DataPtr)(data[0]),
			       *(const V3f *)(pMin[0]),
			       *(const V3f *)(pMax[0]),
			       gridSize,
			       (GridPtr)(grid[0]));
    }
}

} // namespace


void
declareSimdStdLibInterpolator (SymbolTable &symtab, SimdStdTypes &types)
{
    declareSimdCFunc (symtab, simdScatteredDataToGrid3D,
		      types.funcType_v_f023_f3_f3_of0003(),
		      "scatteredDataToGrid3D");
}

} // namespace Ctl
