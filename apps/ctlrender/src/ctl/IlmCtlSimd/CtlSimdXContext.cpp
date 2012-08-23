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
//	class SimdXContext (SIMD execution-time context)
//
//-----------------------------------------------------------------------------

#include <CtlSimdXContext.h>
#include <CtlSimdReg.h>
#include <CtlSimdInst.h>
#include <CtlSimdAddr.h>
#include <CtlSimdInterpreter.h>
#include <CtlExc.h>
#include <cassert>

using namespace Iex;

namespace Ctl {


SimdStack::SimdStack (int size):
    _regPointers (new RegPointer[size]),
    _size (size),
    _sp (0),
    _fp (0)
{
    // empty
}


SimdStack::~SimdStack ()
{
    pop (_sp);
    delete [] _regPointers;
}


void
SimdStack::setFp (int fp)
{
    if (fp < 0 || fp > _sp)
	throw ArgExc ("Frame pointer out of range.");

    _fp = fp;
}


void
SimdStack::push (SimdReg *reg, RegOwnership ownership)
{
    if (_sp > _size)
    {
	if (ownership == TAKE_OWNERSHIP)
	    delete reg;

	throw StackOverflowExc ("Stack overflow.");
    }

    _regPointers[_sp].reg = reg;
    _regPointers[_sp].owner = (ownership == TAKE_OWNERSHIP);
    ++_sp;
}


void
SimdStack::pop (int n, bool giveUpOwnership /* = false */)
{
    if (n > _sp)
	throw StackUnderflowExc ("Stack underflow.");

    if (n < 0)
	throw StackLogicExc("Stack pop passed negative value.");

    while (n > 0)
    {
	--n;
	--_sp;

	if (_regPointers[_sp].owner && !giveUpOwnership)
	    delete _regPointers[_sp].reg;
	
    }
}


SimdReg &
SimdStack::regSpRelative (int offset) const
{
    int i = _sp + offset;

    if (i < 0 || i >= _sp)
	throw ArgExc ("Invalid stack-pointer relative address.");

    return *_regPointers[i].reg;
}


SimdReg &
SimdStack::regFpRelative (int offset) const
{
    int i = _fp + offset;

    if (i < 0 || i >= _sp)
	throw ArgExc ("Invalid frame-pointer relative address.");

    return *_regPointers[i].reg;
}


RegOwnership
SimdStack::ownerSpRelative (int offset) const
{
    int i = _sp + offset;

    if (i < 0 || i >= _sp)
	throw ArgExc ("Invalid frame-pointer relative address.");

    return _regPointers[i].owner ? TAKE_OWNERSHIP : REFERENCE_ONLY;
}


RegOwnership
SimdStack::ownerFpRelative (int offset) const
{
    int i = _fp + offset;

    if (i < 0 || i >= _sp)
	throw ArgExc ("Invalid frame-pointer relative address.");

    return _regPointers[i].owner ? TAKE_OWNERSHIP : REFERENCE_ONLY;
}


namespace {

const std::string unknown = "unkown";

} // namespace

SimdXContext::SimdXContext (SimdInterpreter &interpreter):
    _interpreter (interpreter),
    _stack (1000),
    _regSize (0),
    _returnMask (new SimdBoolMask(false)),
    _lineNumber (0),
    _module(0),
    _abortCount (0),
    _maxInstCount (0),
    _instCount (0),
    _fileName (&unknown)
{
    (*_returnMask)[0] = false;
}

SimdBoolMask *
SimdXContext::swapReturnMasks(SimdBoolMask *newMask)
{
    SimdBoolMask *tmp = _returnMask;
    _returnMask = newMask;
    return tmp;

}

SimdXContext::~SimdXContext ()
{
    delete _returnMask;
}


void	
SimdXContext::run (int regSize, const SimdInst *entryPoint)
{
    if( entryPoint == 0)  // if function is empy
	return;

    assert (regSize <= MAX_REG_SIZE && entryPoint != 0);

    _regSize = regSize;

    SimdBoolMask mask (false);
    mask[0] = true;

    _abortCount = _interpreter.abortCount();
    _maxInstCount = _interpreter.maxInstCount();
    _instCount = 0;

    entryPoint->executePath (mask, *this);
}


void	
SimdXContext::countInstruction ()
{
    if ((++_instCount & 0x01fff) == 0)
    {
	if (_maxInstCount && _instCount > _maxInstCount)
	    throw Ctl::MaxInstExc ("Maximum CTL instruction count exceeded.");

	if (_abortCount != _interpreter.abortCount())
	    throw Ctl::AbortExc ("CTL program aborted.");
    }
}


} // namespace Ctl
