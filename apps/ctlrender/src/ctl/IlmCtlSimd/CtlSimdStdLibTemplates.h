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

#ifndef INCLUDED_CTL_SIMD_STD_LIB_TEMPLATES_H
#define INCLUDED_CTL_SIMD_STD_LIB_TEMPLATES_H

//-----------------------------------------------------------------------------
//
//	Templates an macros that are useful for the implementation of the
//	Standard Library of C++ functions that can be called from CTL.
//
//-----------------------------------------------------------------------------

#include <CtlSimdStdTypes.h>
#include <CtlSimdCFunc.h>
#include <CtlSimdReg.h>
#include <CtlSimdXContext.h>

namespace Ctl {

//
// Templated functions with one, two or more arguments that can be called
// from CTL.  The functions will operate on uniform or varying data.  The
// actual operation performed depends on the Func template parameter.
//

template <class Func>
void
simdFunc1Arg (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    const SimdReg &a1 = xcontext.stack().regFpRelative(-1);
    SimdReg &returnValue = xcontext.stack().regFpRelative (-2);

    if (a1.isVarying())
    {
	if (!mask.isVarying() &&
	    !a1.isReference() &&
	    !returnValue.isReference())
	{
	    //
	    // Mask is uniform and a1 and the return value
	    // are contiguous in memory.
	    //

	    returnValue.setVaryingDiscardData (true);

	    const typename Func::Arg1T *a1Ptr =
				(typename Func::Arg1T *)(a1[0]);

	    typename Func::ReturnT *returnPtr =
				(typename Func::ReturnT *)(returnValue[0]);

	    typename Func::ReturnT *returnEnd =
				returnPtr + xcontext.regSize();

	    while (returnPtr < returnEnd)
		*(returnPtr++) = Func::call (*(a1Ptr++));
	}
	else
	{
	    //
	    // Mask is varying, or a1 or the return value
	    // may not be contiguous in memory.
	    //

	    returnValue.setVarying (true);

	    for (int i = xcontext.regSize(); --i >= 0;)
		if (mask[i])
		    *(typename Func::ReturnT *)(returnValue[i]) =
			Func::call (*(typename Func::Arg1T *)(a1[i]));
	}
    }
    else
    {
	returnValue.setVarying (false);

	*(typename Func::ReturnT *)(returnValue[0]) =
	    Func::call (*(typename Func::Arg1T *)(a1[0]));
    }
}


template <class Func>
void
simdFunc2Arg (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    const SimdReg &a1 = xcontext.stack().regFpRelative (-1);
    const SimdReg &a2 = xcontext.stack().regFpRelative (-2);
    SimdReg &returnValue = xcontext.stack().regFpRelative (-3);

    if (a1.isVarying() || a2.isVarying())
    {
	if (!mask.isVarying() &&
	    !a1.isReference() &&
	    !a2.isReference() &&
	    !returnValue.isReference())
	{
	    //
	    // Mask is uniform and a1, a2 and the return value are
	    // contiguous in memory.  A1, a2 or both are varying.
	    //

	    returnValue.setVaryingDiscardData (true);

	    const typename Func::Arg1T *a1Ptr =
				(typename Func::Arg1T *)(a1[0]);

	    const typename Func::Arg2T *a2Ptr =
				(typename Func::Arg2T *)(a2[0]);

	    typename Func::ReturnT *returnPtr =
				(typename Func::ReturnT *)(returnValue[0]);

	    typename Func::ReturnT *returnEnd =
				returnPtr + xcontext.regSize();

	    if (a1.isVarying() && a2.isVarying())
	    {
		while (returnPtr < returnEnd)
		    *(returnPtr++) = Func::call (*(a1Ptr++), *(a2Ptr++));
	    }
	    else if (a1.isVarying())
	    {
		while (returnPtr < returnEnd)
		    *(returnPtr++) = Func::call (*(a1Ptr++), *(a2Ptr));
	    }
	    else
	    {
		while (returnPtr < returnEnd)
		    *(returnPtr++) = Func::call (*(a1Ptr), *(a2Ptr++));
	    }
	}
	else
	{
	    //
	    // Mask is varying, or a1, a2 or the return value
	    // may not be contiguous in memory.
	    //

	    returnValue.setVarying (true);

	    for (int i = xcontext.regSize(); --i >= 0;)
		if (mask[i])
		    *(typename Func::ReturnT *)(returnValue[i]) =
			Func::call (*(typename Func::Arg1T *)(a1[i]), 
				    *(typename Func::Arg2T *)(a2[i]));
	}
    }
    else
    {
	returnValue.setVarying (false);

	*(typename Func::ReturnT *)(returnValue[0]) =
	    Func::call (*(typename Func::Arg1T *)(a1[0]), 
		        *(typename Func::Arg2T *)(a2[0]));
    }
}


//
// Function objects that can be passed to the templates above.
//

#define DEFINE_SIMD_FUNC_1_ARG(className, op, rT, a1T)			\
									\
    struct className							\
    {									\
	typedef rT ReturnT;						\
	typedef a1T Arg1T;						\
									\
	static ReturnT							\
	call (const Arg1T &a1)						\
	{								\
	    return op;							\
	}								\
    };


#define DEFINE_SIMD_FUNC_2_ARG(className, op, rT, a1T, a2T)		\
									\
    struct className							\
    {									\
	typedef rT ReturnT;						\
	typedef a1T Arg1T;						\
	typedef a2T Arg2T;						\
									\
	static ReturnT							\
	call (const Arg1T &a1,						\
	      const Arg2T &a2)						\
	{								\
	    return op;							\
	}								\
    };


} // namespace Ctl

#endif
