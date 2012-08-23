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
//	- color space conversions
//
//-----------------------------------------------------------------------------

#include <CtlSimdStdLibColorSpace.h>
#include <CtlSimdStdLibTemplates.h>
#include <CtlSimdStdLibrary.h>
#include <CtlSimdStdTypes.h>
#include <CtlSimdCFunc.h>
#include <CtlColorSpace.h>

using namespace Imath;
using namespace std;

namespace Ctl {
namespace {

DEFINE_SIMD_FUNC_2_ARG (RgbToXyz, RGBtoXYZ(a1,a2), M44f, Chromaticities, float);
DEFINE_SIMD_FUNC_2_ARG (XyzToRgb, XYZtoRGB(a1,a2), M44f, Chromaticities, float);
DEFINE_SIMD_FUNC_2_ARG (XyzToLuv, XYZtoLuv(a1,a2), V3f, V3f, V3f);
DEFINE_SIMD_FUNC_2_ARG (LuvToXyz, LuvtoXYZ(a1,a2), V3f, V3f, V3f);
DEFINE_SIMD_FUNC_2_ARG (XyzToLab, XYZtoLab(a1,a2), V3f, V3f, V3f);
DEFINE_SIMD_FUNC_2_ARG (LabToXyz, LabtoXYZ(a1,a2), V3f, V3f, V3f);

} // namespace


void
declareSimdStdLibColorSpace (SymbolTable &symtab, SimdStdTypes &types)
{
    declareSimdCFunc (symtab, simdFunc2Arg <RgbToXyz>,
		      types.funcType_f44_chr_f(), "RGBtoXYZ");

    declareSimdCFunc (symtab, simdFunc2Arg <XyzToRgb>,
		      types.funcType_f44_chr_f(), "XYZtoRGB");

    declareSimdCFunc (symtab, simdFunc2Arg <LuvToXyz>,
		      types.funcType_f3_f3_f3(), "LuvtoXYZ");

    declareSimdCFunc (symtab, simdFunc2Arg <XyzToLuv>,
		      types.funcType_f3_f3_f3(), "XYZtoLuv");

    declareSimdCFunc (symtab, simdFunc2Arg <LabToXyz>,
		      types.funcType_f3_f3_f3(), "LabtoXYZ");

    declareSimdCFunc (symtab, simdFunc2Arg <XyzToLab>,
		      types.funcType_f3_f3_f3(), "XYZtoLab");
}

} // namespace Ctl
