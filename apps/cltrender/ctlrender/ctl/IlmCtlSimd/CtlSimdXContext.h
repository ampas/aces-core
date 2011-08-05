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


#ifndef INCLUDED_CTL_SIMD_X_CONTEXT_H
#define INCLUDED_CTL_SIMD_X_CONTEXT_H

//-----------------------------------------------------------------------------
//
//	class SimdXContext (SIMD execution-time context)
//	
//	The SIMD (single instruction / multiple data) engine
//	that executes compiled color transformation programs.
//
//-----------------------------------------------------------------------------

#include <vector>
#include <CtlSimdModule.h>
#include <typeinfo>
#include <CtlSimdReg.h>
#include <string>

namespace Ctl {

class SimdInst;
class SimdInterpreter;


enum RegOwnership
{
    TAKE_OWNERSHIP,
    REFERENCE_ONLY
};


class SimdStack
{
  public:

     SimdStack (int size);
    ~SimdStack ();

    void	push (SimdReg *reg, RegOwnership ownership);
    void	pop (int n, bool giveUpOwnership = false);

    int		sp () const		{return _sp;}
    int		fp () const		{return _fp;}
    void	setFp (int fp);

    SimdReg &	regSpRelative (int offset) const;
    SimdReg &	regFpRelative (int offset) const;

    RegOwnership ownerSpRelative (int offset) const;
    RegOwnership ownerFpRelative (int offset) const;

  private:

    struct RegPointer
    {
	SimdReg *	reg;
	bool		owner;
    };

    RegPointer *	_regPointers;
    int			_size;
    int			_sp;
    int			_fp;

};


class SimdXContext
{
  public:

    SimdXContext (SimdInterpreter &interpreter);
    virtual ~SimdXContext ();

    void		run (int regSize, const SimdInst *entryPoint);

    SimdBoolMask &	returnMask () const		{return *_returnMask;}
    SimdBoolMask *      swapReturnMasks(SimdBoolMask *newMask);

    SimdStack &		stack ()			{return _stack;}
    int			regSize () const		{return _regSize;}

    int			lineNumber () const		{return _lineNumber;}
    void		setLineNumber (int ln)   	{_lineNumber = ln;}
    
    const std::string & fileName () const		{return *_fileName;}
    void		setFileName (const std::string &fn) {_fileName = &fn;}

    SimdModule*		module()			{return _module;}
    void		setModule(SimdModule* m)	{_module = m;}

    void		countInstruction ();

    SimdInterpreter &interpreter(void) const { return _interpreter; };

  private:

    SimdInterpreter &	_interpreter;

    SimdStack		_stack;
    int			_regSize;
    SimdBoolMask *	_returnMask;

    int			_lineNumber;
    SimdModule *	_module;

    unsigned long	_abortCount;
    unsigned long	_maxInstCount;
    unsigned long	_instCount;
    const std::string *	_fileName;
};


//
// A helper class to construct and destruct stack frames.
// Used by class SimdCallInst, below.
//

class StackFrame
{
  public:

    StackFrame (SimdXContext &xcontext):
	_xcontext (xcontext),
	_stack(xcontext.stack()),
	_savedSp (_stack.sp()),
	_savedFp (_stack.fp()),
	_savedRMask (new SimdBoolMask(false))
    {
	_stack.setFp (_stack.sp());
	
	(*_savedRMask)[0] = 0;
	_savedRMask = _xcontext.swapReturnMasks(_savedRMask);

    }

    ~StackFrame ()
    {
	_stack.pop (_stack.sp() - _savedSp);
	_stack.setFp (_savedFp);

	_savedRMask = _xcontext.swapReturnMasks(_savedRMask);
	delete _savedRMask;
    }

  private:

    SimdXContext & _xcontext;
    SimdStack    & _stack;
    int		   _savedSp;
    int		   _savedFp;
    SimdBoolMask * _savedRMask;
};



} // namespace Ctl

#endif
