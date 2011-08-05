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


#ifndef INCLUDED_CTL_SIMD_ADDR_H
#define INCLUDED_CTL_SIMD_ADDR_H

//-----------------------------------------------------------------------------
//
//	Data and code addresses for the CTL interpreter's SIMD back end
//
//-----------------------------------------------------------------------------

#include <CtlAddr.h>
#include <CtlSimdXContext.h>
#include <CtlSimdCFunc.h>

namespace Ctl {

class SimdDataAddr;
typedef RcPtr <SimdDataAddr> SimdDataAddrPtr;

class SimdInstAddr;
typedef RcPtr <SimdInstAddr> SimdInstAddrPtr;

class SimdCFuncAddr;
typedef RcPtr <SimdCFuncAddr> SimdCFuncAddrPtr;


class SimdDataAddr: public DataAddr
{
  public:

    //------------------------------
    // Constructor, absolute address
    //------------------------------

    SimdDataAddr (SimdReg *reg);


    //--------------------------------------------
    // Constructor, frame-pointer relative address
    //--------------------------------------------

    SimdDataAddr (int fpOffset);


    //-----------------------------------------
    // Copy constructor and assignment operator
    //-----------------------------------------

    SimdDataAddr (const SimdDataAddr &other);
    const SimdDataAddr &	operator = (const SimdDataAddr &other);


    //------------------------------------------------------------
    // Return a C++ reference to the register to which the
    // SimdDataAddr object points. 
    //------------------------------------------------------------

    SimdReg &		reg (SimdXContext &xcontext) const;

    //------------------------------------------------------------
    // Returns a pointer to a simdReg if this is not a relative
    // address.  Returns zero if it is relative.  Used during
    // parsing to intialize registers.
    //------------------------------------------------------------

    SimdReg *		reg () const;


    //-----------------------------------------
    // Print a register address (for debugging)
    //-----------------------------------------

    virtual void	print (int indent) const;

  private:

    union
    {
	SimdReg*        	_reg;
	int      		_fpOffset;
    };

    bool	_fpRelative;
};


class SimdInstAddr: public CodeAddr
{
  public:

    //---------------------------------------------
    // Class SimdInstAddr represents the address of
    // a SimdInst object (usually the entry point
    // for a function written in CTL).
    //---------------------------------------------

    SimdInstAddr (const SimdInst *inst): _inst (inst) {}

    const SimdInst *	inst () {return _inst;}
    virtual void	print (int indent) const;

  private:

    const SimdInst *	_inst;
};


class SimdCFuncAddr: public CodeAddr
{
  public:

    //----------------------------------------------
    // Class SimdCFuncAddr represents the address
    // of a function that is written in C++ and that
    // can be called from CTL.
    //----------------------------------------------

    SimdCFuncAddr (SimdCFunc func): _func (func) {}

    SimdCFunc		func () {return _func;}
    virtual void	print (int indent) const;

  private:

    SimdCFunc		_func;
};


//---------------
// Implementation
//---------------

inline SimdReg &
SimdDataAddr::reg (SimdXContext &xcontext) const
{
    if (_fpRelative)
	return xcontext.stack().regFpRelative (_fpOffset);
    else
	return *_reg;
}

inline SimdReg *
SimdDataAddr::reg () const
{
    if (_fpRelative)
	return 0;
    else
	return _reg;
}

} // namespace Ctl

#endif
