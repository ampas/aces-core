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


#ifndef INCLUDED_CTL_SIMD_OP_H
#define INCLUDED_CTL_SIMD_OP_H

//-----------------------------------------------------------------------------
//
//	Operations for the SIMD color transformation engine.
//
//-----------------------------------------------------------------------------

namespace Ctl {


template <class In1, class In2, class Out>
struct BinaryMinusOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 - in2);
    }
};


template <class In1, class In2, class Out>
struct BitAndOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 & in2);
    }
};


template <class In1, class In2, class Out>
struct BoolBitAndOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (!!in1 & !!in2);
    }
};


template <class In, class Out>
struct BitNotOp
{
    static void
    execute (const In &in, Out &out)
    {
	out = Out (~in);
    }
};


template <class In1, class In2, class Out>
struct BitOrOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 | in2);
    }
};


template <class In1, class In2, class Out>
struct BoolBitOrOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (!!in1 | !!in2);
    }
};


template <class In1, class In2, class Out>
struct BitXorOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 ^ in2);
    }
};


template <class In1, class In2, class Out>
struct BoolBitXorOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (!!in1 ^ !!in2);
    }
};


template <class In, class Out>
struct CopyOp
{
    static void
    execute (const In &in, Out &out)
    {
	out = Out (in);
    }
};


template <class In1, class In2, class Out>
struct DivOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 / in2);
    }
};


template <class In1, class In2, class Out>
struct IntDivOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = (in2 != 0)? Out (in1 / in2): Out (0);
    }
};


template <class In1, class In2, class Out>
struct EqualOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 == in2);
    }
};


template <class In1, class In2, class Out>
struct GreaterEqualOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 >= in2);
    }
};


template <class In1, class In2, class Out>
struct GreaterOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 > in2);
    }
};


template <class In1, class In2, class Out>
struct LeftShiftOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 << in2);
    }
};


template <class In1, class In2, class Out>
struct LessEqualOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 <= in2);
    }
};


template <class In1, class In2, class Out>
struct LessOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 < in2);
    }
};


template <class In1, class In2, class Out>
struct ModOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 % in2);
    }
};


template <class In1, class In2, class Out>
struct NotEqualOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 != in2);
    }
};


template <class In, class Out>
struct NotOp
{
    static void
    execute (const In &in, Out &out)
    {
	out = Out (!in);
    }
};


template <class In1, class In2, class Out>
struct PlusOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 + in2);
    }
};


template <class In1, class In2, class Out>
struct RightShiftOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 >> in2);
    }
};


template <class In1, class In2, class Out>
struct TimesOp
{
    static void
    execute (const In1 &in1, const In2 &in2, Out &out)
    {
	out = Out (in1 * in2);
    }
};


template <class In, class Out>
struct UnaryMinusOp
{
    static void
    execute (const In &in, Out &out)
    {
	out = Out (-in);
    }
};


} // namespace Ctl

#endif
