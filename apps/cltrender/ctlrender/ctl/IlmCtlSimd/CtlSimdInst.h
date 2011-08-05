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


#ifndef INCLUDED_CTL_SIMD_INST_H
#define INCLUDED_CTL_SIMD_INST_H

//-----------------------------------------------------------------------------
//
//	Instructions for the SIMD color transformation engine.
//
//-----------------------------------------------------------------------------

#include <CtlSimdReg.h>
#include <CtlSimdAddr.h>
#include <CtlSyntaxTree.h>
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <string>

namespace Ctl {

class SimdInst
{
  public:

    SimdInst (int lineNumber);
    virtual ~SimdInst ();

    void		setNextInPath (const SimdInst *nextInPath);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const = 0;

    void		executePath (SimdBoolMask &mask,
				     SimdXContext &xcontext) const;

    virtual void	print (int indent) const = 0;
    void		printPath (int indent) const;

    int                 lineNumber() const { return _lineNumber; }

  private:

    const int           _lineNumber;
    const SimdInst *	_nextInPath;
};



//
// When used in binary operators, the branch inst paths leave a register on
// the stack.  In this case mergeResults should be set to true.  
// If the conditional is varying, both the truePath and falsePath 
// may be executed - mergeResults will merge the top two registers on the
// stack at the concolusion of the instruction.
class SimdBranchInst: public SimdInst
{
  public:

    SimdBranchInst (const SimdInst *truePath,
		    const SimdInst *falsePath,
		    bool mergeResults,
		    int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    const SimdInst *		_truePath;
    const SimdInst *		_falsePath;
    const bool                  _mergeResults;
};


class SimdLoopInst: public SimdInst
{
  public:

    SimdLoopInst (const SimdInst *conditionPath,
		  const SimdInst *loopPath,
		  int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    const SimdInst *		_conditionPath;
    const SimdInst *		_loopPath;
};


class SimdCallInst: public SimdInst
{
  public:

    SimdCallInst (const SimdInst *callPath,
		  int numParameters,
		  int lineNumber);

    void		setCallPath (const SimdInst *callPath);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    const SimdInst *	_callPath;
    int                 _numParameters;
};


class SimdCCallInst: public SimdInst
{
  public:

    SimdCCallInst (SimdCFunc func, 
		   int numParameters,
		   int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    SimdCFunc		_func;
    int                 _numParameters;
};


class SimdReturnInst: public SimdInst
{
  public:

    SimdReturnInst (int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;
};


template <class In, class Out, template <class I, class O> class Op>
class SimdUnaryOpInst: public SimdInst
{
  public:

    SimdUnaryOpInst (int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

};


template <class In1, class In2, class Out,
	  template <class I1, class I2, class O> class Op>
class SimdBinaryOpInst: public SimdInst
{
  public:

    SimdBinaryOpInst (int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

};


class SimdAssignInst: public SimdInst
{
  public:

    SimdAssignInst (size_t opTypeSize, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    size_t  _opTypeSize;
};



class SimdInitializeInst: public SimdInst
{
  public:

    SimdInitializeInst (const SizeVector &sizes, 
			const SizeVector &offsets,
			int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    SizeVector _sizes;
    SizeVector _offsets;
};


class SimdAssignArrayInst: public SimdInst
{
  public:

    SimdAssignArrayInst (int size,  size_t opTypeSize, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    int _size;
    size_t _opTypeSize;
};


class SimdIndexArrayInst: public SimdInst
{
  public:

    SimdIndexArrayInst (size_t arrayElementSize, 
			int lineNumber, 
			size_t arraySize);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    size_t _arrayElementSize;
    size_t _arraySize;
};

class SimdIndexVSArrayInst: public SimdInst
{
  public:

    SimdIndexVSArrayInst (size_t arrayElementSize,
			  const SimdDataAddrPtr &arrayElementSizePtr, 
			  size_t arraySize,
			  const SimdDataAddrPtr &arraySizePtr,
			  int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    const size_t _arrayElementSize;
    const SimdDataAddrPtr _arrayElementSizePtr;

    const size_t _arraySize;
    const SimdDataAddrPtr _arraySizePtr;
};


class SimdAccessMemberInst: public SimdInst
{
  public:

    SimdAccessMemberInst (size_t offset, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    size_t _offset;
};


template <class T>
class SimdPushLiteralInst: public SimdInst
{
  public:

    SimdPushLiteralInst (T value, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    T			_value;
};


class SimdPushStringLiteralInst: public SimdInst
{
  public:

    SimdPushStringLiteralInst (const std::string &value, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    std::string		_value;
};


class SimdPushRefInst: public SimdInst
{
  public:

    SimdPushRefInst (const SimdDataAddrPtr &in, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    SimdDataAddrPtr	_in;
};


class SimdPushPlaceholderInst: public SimdInst
{
  public:

    SimdPushPlaceholderInst (size_t eSize, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    int _eSize;
    
};


class SimdPopInst: public SimdInst
{
  public:

    SimdPopInst (int numRegs, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    int			_numRegs;
};


class SimdFileNameInst: public SimdInst
{
  public:

    SimdFileNameInst (const std::string &fileName, int lineNumber);

    virtual void	execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const;

    virtual void	print (int indent) const;

  private:

    std::string		_fileName;
};

//---------------
// Implementation
//---------------

template <class In, class Out, template <class I, class O> class Op>
SimdUnaryOpInst<In, Out, Op>::SimdUnaryOpInst (int lineNumber)
    : SimdInst(lineNumber)
{
    // empty
}


template <class In, class Out, template <class I, class O> class Op>
void
SimdUnaryOpInst<In, Out, Op>::execute (SimdBoolMask &mask,
				       SimdXContext &xcontext) const
{
    const SimdReg &in = xcontext.stack().regSpRelative(-1);
    SimdReg * out = new SimdReg(in.isVarying() || mask.isVarying(),
				sizeof(Out));

    try
    {
	if (in.isVarying() || mask.isVarying())
	{
	    if (!mask.isVarying() && !in.isReference())
	    {
		//
		// The contents of in are contiguous in
		// memory and mask is uniform.
		//

		const In *inPtr = (In *)in[0];
		Out *outPtr = (Out *)(*out)[0];
		Out *outEnd = outPtr + xcontext.regSize();

		while (outPtr < outEnd)
		    Op<In,Out>::execute (*(inPtr++), *(outPtr++));
	    }
	    else
	    {
		//
		// Mask is not uniform or the contents of in
		// may not be contiguous in memory.
		//

		for (int i = xcontext.regSize(); --i >= 0;)
		    if (mask[i])
			Op<In,Out>::execute (*(In*)(in[i]), 
					     (*(Out*)(*out)[i]));
	    }
	}
	else
	{
	    Op<In,Out>::execute (*(In*)(in[0]), *(Out*)(*out)[0]);
	}
    }
    catch (...)
    {
	delete out;
	throw;
    }

    xcontext.stack().pop (1);
    xcontext.stack().push (out, TAKE_OWNERSHIP);
}


template <class In, class Out, template <class I, class O> class Op>
void
SimdUnaryOpInst<In, Out, Op>::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "unary op " <<
		 typeid(Op<In,Out>).name() << std::endl;
}


template <class In1, class In2, class Out,
	  template <class I1, class I2, class O> class Op>
SimdBinaryOpInst<In1, In2, Out, Op>::SimdBinaryOpInst (int lineNumber)
    : SimdInst(lineNumber)
{
    // empty
}


template <class In1, class In2, class Out,
	  template <class I1, class I2, class O> class Op>
void
SimdBinaryOpInst<In1, In2, Out, Op>::execute (SimdBoolMask &mask,
					      SimdXContext &xcontext) const
{
    const SimdReg &in1 = xcontext.stack().regSpRelative(-2);
    const SimdReg &in2 = xcontext.stack().regSpRelative(-1);

    SimdReg * out = 
	new SimdReg(in1.isVarying() || in2.isVarying() || mask.isVarying(),
		    sizeof(Out));

    try
    {
	if (in1.isVarying() || in2.isVarying() || mask.isVarying())
	{
	    if (!mask.isVarying() && !in1.isReference() && !in2.isReference())
	    {
		//
		// Mask is uniform and the contents of input registers
		// in1 and in2 are contiguous in memory.  At least one
		// of the input registers is varying.
		//

		const In1 *in1Ptr = (In1 *)in1[0];
		const In2 *in2Ptr = (In2 *)in2[0];
		Out *outPtr = (Out *)(*out)[0];
		Out *outEnd = outPtr + xcontext.regSize();

		if (in1.isVarying() && in2.isVarying())
		{
		    while (outPtr < outEnd)
			Op<In1,In2,Out>::execute (*(in1Ptr++),
						  *(in2Ptr++),
						  *(outPtr++));
		}
		else if (in1.isVarying())
		{
		    while (outPtr < outEnd)
			Op<In1,In2,Out>::execute (*(in1Ptr++),
						  *(in2Ptr),
						  *(outPtr++));
		}
		else
		{
		    while (outPtr < outEnd)
			Op<In1,In2,Out>::execute (*(in1Ptr),
						  *(in2Ptr++),
						  *(outPtr++));
		}
	    }
	    else
	    {
		//
		// Mask is varying or the contents of the input
		// registers may not be contiguous in memory.
		//

		for (int i = xcontext.regSize(); --i >= 0;)
		    if (mask[i])
			Op<In1,In2,Out>::execute (*(In1*)(in1[i]), 
						  *(In2*)(in2[i]), 
						  *(Out*)((*out)[i]));
	    }
	}
	else
	{
	    Op<In1,In2,Out>::execute (*(In1*)(in1[0]), 
				      *(In2*)(in2[0]), 
				      *(Out*)((*out)[0]));
	}
    }
    catch (...)
    {
	delete out;
	throw;
    }

    xcontext.stack().pop (2);
    xcontext.stack().push (out, TAKE_OWNERSHIP);
}


template <class In1, class In2, class Out,
	  template <class I1, class I2, class O> class Op>
void
SimdBinaryOpInst<In1, In2, Out, Op>::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "binary op " <<
		 typeid(Op<In1,In2,Out>).name() << std::endl;
}



template <class T>
SimdPushLiteralInst<T>::SimdPushLiteralInst (T value, int lineNumber)
    : SimdInst(lineNumber), _value (value)
{
    // empty
}


template <class T>
void
SimdPushLiteralInst<T>::execute (SimdBoolMask &mask,
				 SimdXContext &xcontext) const
{
    SimdReg *out = new SimdReg(false, sizeof(T));
    xcontext.stack().push (out, TAKE_OWNERSHIP);
    memcpy((*out)[0],  &_value, sizeof(_value));
}


template <class T>
void
SimdPushLiteralInst<T>::print (int indent) const
{
    std::cout << std::setw (indent) << "" << "push literal " 
	      << _value << " " <<  typeid(T).name() << std::endl;
}


} // namespace Ctl

#endif
