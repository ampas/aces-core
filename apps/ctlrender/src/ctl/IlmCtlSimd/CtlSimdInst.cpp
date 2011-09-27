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
//	Instructions for the SIMD color transformation engine.
//
//-----------------------------------------------------------------------------

#include <CtlSimdInst.h>
#include <sstream>

using namespace std;

namespace Ctl {
namespace {

#if 0
    #include <iostream>
    #define debug(x) (cout << x << endl)
    #define debug_only(x) x
#else
    #define debug(x)
    #define debug_only(x)
#endif

//
// Make a SimdBoolMask uniform if all of its entries are true.
//

void
tryToMakeUniform (SimdBoolMask &mask, SimdXContext &xcontext)
{
    if (!mask.isVarying())
	return;

    for (int i = xcontext.regSize(); --i >= 0;)
	if (!mask[i])
	    return;

    mask.setVarying (false);
}

// Updates parentMask, setting elements to false whenever the
// return mask is true.
//
// Returns true if the child path should return (used for looping).  This is
// true when for all elements, if the childMask is true, the  returnMask
// is also true.
// 
bool
updateMask (SimdBoolMask &parentMask, 
	    SimdBoolMask &childMask,
	    SimdBoolMask &returnMask, 
	    SimdXContext &xcontext)
{
    bool keepRunning = false;
    if( returnMask.isVarying() )
    {
	if(!parentMask.isVarying() )
	    parentMask.setVarying(true);

	for(int i = xcontext.regSize(); --i >= 0;)
	{
	    parentMask[i] = parentMask[i] && !returnMask[i];
	    keepRunning = keepRunning || (childMask[i] && !returnMask[i]  );
	}
	return !keepRunning;
    }
    else if(returnMask[0])
    {
	if(parentMask.isVarying() )
	    parentMask.setVarying(false);

	parentMask.setVarying(false);
	parentMask[0] = false;
	return true;
    }
    else
    {
	return false;
    }

}

} // namespace


SimdInst::SimdInst (int lineNumber): _lineNumber(lineNumber), _nextInPath (0)
{
    // empty
}


SimdInst::~SimdInst ()
{
    // empty
}


void
SimdInst::setNextInPath (const SimdInst *nextInPath)
{
    _nextInPath = nextInPath;
}


void
SimdInst::executePath (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    for (const SimdInst *inst = this; inst; inst = inst->_nextInPath)
    {
	//
	// First check if we are supposed to return.
	// Assumes no instructions follow a return
	// statement in a path
	//

	SimdBoolMask &rMask = xcontext.returnMask();

	if(!rMask.isVarying() && rMask[0])
	    return;

	xcontext.setLineNumber(inst->lineNumber());

	debug_only(cout << xcontext.lineNumber() << " ");
	debug_only(cout << "fp=" << xcontext.stack().fp()
		   << " sp=" << xcontext.stack().sp() << " ");
	debug_only(inst->print(0));

	try
	{
	    xcontext.countInstruction();
	    inst->execute (mask, xcontext);
	}
        catch (Iex::BaseExc &e)
	{
	    REPLACE_EXC
		(e, "\n" <<
		 xcontext.fileName() << ":" <<
		 inst->lineNumber() << ": " << e);

	    throw e;
	}
	catch (std::exception &e)
	{
	    THROW (Iex::BaseExc, "\n" <<
		   xcontext.fileName() << ":" <<
		   inst->lineNumber() << ": "
		   "CTL run-time error (" << e.what() << ")");
	}
	catch (...)
	{
	    THROW (Iex::BaseExc, "\n" <<
		   xcontext.fileName() << ":" <<
		   inst->lineNumber() << ": "
		   "CTL run-time error");
	}
    }
}


void	
SimdInst::printPath (int indent) const
{
    for (const SimdInst *inst = this; inst; inst = inst->_nextInPath)
	inst->print (indent);
}


SimdBranchInst::SimdBranchInst
    (const SimdInst *truePath,
     const SimdInst *falsePath,
     bool mergeResults,
     int lineNumber)
:
	SimdInst(lineNumber),
	_truePath (truePath),
	_falsePath (falsePath),
	_mergeResults (mergeResults)
{
    // empty
}


void
SimdBranchInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    const SimdReg &condition =
	xcontext.stack().regSpRelative (-1);

    if (condition.isVarying())
    {
	SimdBoolMask trueMask (true);
	bool takeTruePath = false;

	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    trueMask[i] = (mask[i] & *(bool *)(condition[i]));
	    takeTruePath |= trueMask[i];
	}

	SimdBoolMask falseMask (true);
	bool takeFalsePath = false;

	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    falseMask[i] = (mask[i] & !*(bool *)(condition[i]));
	    takeFalsePath |= falseMask[i];
	}

	xcontext.stack().pop (1);

	if (takeTruePath)
	{
	    tryToMakeUniform (trueMask, xcontext);
	    _truePath->executePath (trueMask, xcontext);
	}

	if (takeFalsePath)
	{
	    tryToMakeUniform (falseMask, xcontext);
	    _falsePath->executePath (falseMask, xcontext);
	}
	if (takeTruePath || takeFalsePath)
	    updateMask(mask, mask, xcontext.returnMask(), xcontext);
	    


	// if both paths are executed and they generated a result,
	// merge the two results
	if( takeTruePath && takeFalsePath && _mergeResults )
	{
	    const SimdReg &tReg = xcontext.stack().regSpRelative (-2);
	    const SimdReg &fReg = xcontext.stack().regSpRelative (-1);

	    size_t eSize = tReg.elementSize();
	    SimdReg *outReg = new SimdReg(true, eSize);

	    try
	    {
		for (int i = xcontext.regSize(); --i >= 0;)
		{
		    if( trueMask[i] )
			memcpy ((*outReg)[i], tReg[i], eSize);
		    else if( falseMask[i] )
			memcpy ((*outReg)[i], fReg[i], eSize);
		}

		xcontext.stack().pop(2);
		xcontext.stack().push (outReg, TAKE_OWNERSHIP);
	    }
            catch (...)
	    {
		delete outReg;
		throw;
	    }
	}
    }
    else
    {
	bool takeTruePath = *(bool *)(condition[0]);
	xcontext.stack().pop (1);

	if (takeTruePath)
	    _truePath->executePath (mask, xcontext);
	else
	    _falsePath->executePath (mask, xcontext);

	updateMask(mask, mask, xcontext.returnMask(), xcontext);
    }
}


void
SimdBranchInst::print (int indent) const
{
    cout << setw (indent) << "" << "branch" << endl;
    cout << setw (indent + 1) << "" << "true path" << endl;

    if (_truePath)
	_truePath->printPath (indent + 2);

    cout << setw (indent + 1) << "" << "false path" << endl;

    if (_falsePath)
	_falsePath->printPath (indent + 2);
}


SimdLoopInst::SimdLoopInst
    (const SimdInst *conditionPath,
     const SimdInst *loopPath,
     int lineNumber)
:
    SimdInst(lineNumber),
    _conditionPath (conditionPath),
    _loopPath (loopPath)
{
    // empty
}


void
SimdLoopInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    SimdBoolMask loopMask (mask.isVarying());

    if (loopMask.isVarying())
    {
	for (int i = xcontext.regSize(); --i >= 0;)
	    loopMask[i] = mask[i];
    }
    else
    {
	loopMask[0] = mask[0];
    }

    bool takeLoopPath;

    do
    {
	_conditionPath->executePath (loopMask, xcontext);

	takeLoopPath = false;

	const SimdReg &condition = xcontext.stack().regSpRelative (-1);

	if (condition.isVarying())
	{
	    loopMask.setVarying (true);

	    for (int i = xcontext.regSize(); --i >= 0;)
	    {
		loopMask[i] &= *(bool*)(condition[i]);
		takeLoopPath |= loopMask[i];
	    }

	    tryToMakeUniform (loopMask, xcontext);
	}
	else
	{
	    takeLoopPath = *(bool*)(condition[0]);
	}

	xcontext.stack().pop (1);

	if (takeLoopPath)
	{
	    _loopPath->executePath (loopMask, xcontext);
	    if( updateMask(mask, loopMask, xcontext.returnMask(), xcontext) )
		break;
	}
    }
    while (takeLoopPath );
}


void
SimdLoopInst::print (int indent) const
{
    cout << setw (indent) << "" << "loop" << endl;
    cout << setw (indent + 1) << "" << "condition path" << endl;

    _conditionPath->printPath (indent + 2);

    cout << setw (indent + 1) << "" << "loop path" << endl;

    if (_loopPath)
	_loopPath->printPath (indent + 2);
}


SimdCallInst::SimdCallInst (const SimdInst *callPath, 
			    int numParameters, 
			    int lineNumber)
    : SimdInst(lineNumber), _callPath (callPath), _numParameters(numParameters)
{
    // empty
}


void
SimdCallInst::setCallPath (const SimdInst *callPath)
{
    _callPath = callPath;
}


void	
SimdCallInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    {
	StackFrame stackFrame (xcontext);
	SimdBoolMask callMask(mask, xcontext.regSize());
	
	_callPath->executePath (callMask, xcontext);
    }

    if( _numParameters > 0)
	xcontext.stack().pop ( _numParameters);
}


void
SimdCallInst::print (int indent) const
{
    cout << setw (indent) << "" <<
	    "function call " << _callPath << endl;
}


SimdCCallInst::SimdCCallInst (SimdCFunc func, int numParameters, int lineNumber)
    : SimdInst(lineNumber), _func (func), _numParameters(numParameters)
{
    // empty
}


void	
SimdCCallInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    {
	StackFrame stackFrame (xcontext);
	_func (mask, xcontext);
    }
    if( _numParameters > 0)
	xcontext.stack().pop ( _numParameters);
}


void
SimdCCallInst::print (int indent) const
{
    cout << setw (indent) << "" <<
	    "C++ function call " << (void *)_func << endl;
}


SimdReturnInst::SimdReturnInst (int lineNumber): SimdInst(lineNumber)
{
    // empty
}


void	
SimdReturnInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    SimdBoolMask &rMask = xcontext.returnMask();


    if(mask.isVarying())
    {
	bool seenFalse = false;

	if(!rMask.isVarying())
	    rMask.setVarying(true);
	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    if(mask[i])
	    {
		rMask[i] = true;
	    }
	    else if(!rMask[i])
	    {
		seenFalse = true;
	    }
	}
	if( !seenFalse )
	{
	    rMask.setVarying(false);
	}
    }
    else
    {
	rMask.setVarying(false);
	rMask[0] = true;
    }
	
}


void
SimdReturnInst::print (int indent) const
{
    cout << setw (indent) << "" << "return " << endl;
}


SimdPushStringLiteralInst::SimdPushStringLiteralInst
    (const string &value,
     int lineNumber)
:
    SimdInst (lineNumber),
    _value (value)
{
    // empty
}


void
SimdPushStringLiteralInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    SimdReg *out = new SimdReg (false, sizeof (string *));
    xcontext.stack().push (out, TAKE_OWNERSHIP);
    *(const string**)(*out)[0] = &_value;
}


void
SimdPushStringLiteralInst::print (int indent) const
{
    std::cout << setw (indent) << "" <<
		"push string literal " << _value << endl;
}


SimdPushRefInst::SimdPushRefInst (const SimdDataAddrPtr &in, int lineNumber)
    : SimdInst(lineNumber), _in (in)
{
    // empty
}


void
SimdPushRefInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    xcontext.stack().push (&_in->reg (xcontext), REFERENCE_ONLY);
}


void
SimdPushRefInst::print (int indent) const
{
    cout << setw (indent) << "" << "push reference ";

    if (_in)
	_in->print (0);
    else
	cout << "*** null address ***"  << endl;
}


SimdPopInst::SimdPopInst (int numRegs, int lineNumber)
    : SimdInst(lineNumber), _numRegs (numRegs)
{
    // empty
}


void
SimdPopInst::execute (SimdBoolMask &mask, SimdXContext &xcontext) const
{
    xcontext.stack().pop (_numRegs);
}


void
SimdPopInst::print (int indent) const
{
    cout << setw (indent) << "" << "pop " << _numRegs << " regs" << endl;
}




SimdAssignInst::SimdAssignInst (size_t opTypeSize, int lineNumber)
    : SimdInst(lineNumber), _opTypeSize(opTypeSize)
{
    // empty
}


void
SimdAssignInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    const SimdReg &in = xcontext.stack().regSpRelative(-1);
    SimdReg &out = xcontext.stack().regSpRelative(-2);

    if (in.isVarying() || mask.isVarying())
    {
	if (!mask.isVarying() &&
	    !in.isReference() &&
	    !out.isReference() &&
	    (in[1] - in[0] == (int)_opTypeSize))
	{
	    //
	    // The contents of in and out are contiguous in memory.
	    //

	    out.setVaryingDiscardData (true);
	    memcpy (out[0], in[0], xcontext.regSize() * _opTypeSize);
	}
	else
	{
	    //
	    // The contents of in and out are not contiguous in memory.
	    //

	    out.setVarying (true);

	    for (int i = xcontext.regSize(); --i >= 0;)
		if (mask[i])
		    memcpy(out[i], in[i], _opTypeSize);
	}
    }
    else
    {
	out.setVarying (false);
	memcpy(out[0], in[0], _opTypeSize);
    }

    xcontext.stack().pop (2);
}



void
SimdAssignInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "assignment " << std::endl;
}




SimdInitializeInst::SimdInitializeInst 
    (const SizeVector &sizes, 
     const SizeVector &offsets,
     int lineNumber)
	: SimdInst(lineNumber), _sizes(sizes), _offsets(offsets)
{
    // empty
}


void
SimdInitializeInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    int elements = _sizes.size();

    SimdReg &out = xcontext.stack().regSpRelative (-elements-1);

    bool varying = false;

    for( int i = 0; i < elements; i++ )
    {
	const SimdReg &in = xcontext.stack().regSpRelative(-i-1);
	if( in.isVarying() )
	{
	    varying = true;
	    out.setVarying (true);
	    break;
	}
    }
    if( !varying )
    {
	out.setVarying (false);
    }

    for( int i = 0; i < elements; i++)
    {
	const SimdReg &in = xcontext.stack().regSpRelative(i-elements);
	if (varying )
	{
	    for (int j = xcontext.regSize(); --j >= 0;)
		if (mask[j])
		    memcpy(out[j]+_offsets[i], in[j], _sizes[i]);
	}
	else
	{
	    memcpy(out[0]+_offsets[i], in[0], _sizes[i]);
	}
    }
    xcontext.stack().pop (elements+1);
}


void
SimdInitializeInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "initialize " << std::endl;
}




SimdAssignArrayInst::SimdAssignArrayInst (int size, size_t opTypeSize,
					  int lineNumber)
    : SimdInst(lineNumber), _size(size), _opTypeSize(opTypeSize)
{
    // empty
}


void
SimdAssignArrayInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    const SimdReg &in = xcontext.stack().regSpRelative(-1);
    SimdReg &out = xcontext.stack().regSpRelative(-2);

    if (in.isVarying() || mask.isVarying())
    {
	out.setVarying (true);
	for( int j = 0; j < xcontext.regSize(); j++)
	{
	    if(mask[j])
	    {
		memcpy(out[j], in[j], _size*_opTypeSize);
	    }

	}
    }
    else
    {
	out.setVarying (false);
	memcpy( out[0], in[0], _size*_opTypeSize);
    }
    xcontext.stack().pop (2);
}


void
SimdAssignArrayInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "array assignment " << std::endl;
}




SimdIndexArrayInst::SimdIndexArrayInst (size_t arrayElementSize, 
					int lineNumber,
					size_t arraySize)
    : SimdInst(lineNumber), 
      _arrayElementSize(arrayElementSize),
      _arraySize(arraySize)
{
    // empty
}

void
SimdIndexArrayInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    SimdReg &arrayReg = xcontext.stack().regSpRelative (-2);
    bool transferOwnership = 
	(xcontext.stack().ownerSpRelative(-2) == TAKE_OWNERSHIP);

    SimdReg &indexReg = xcontext.stack().regSpRelative(-1); 
    SimdReg *out = new SimdReg(arrayReg, indexReg, mask, 
			       _arrayElementSize,
			       _arraySize,
			       xcontext.regSize(),
			       transferOwnership);
    try
    {
	xcontext.stack().pop (2);
	xcontext.stack().push (out, TAKE_OWNERSHIP);
    }
    catch (...)
    {
	delete out;
	throw;
    }

}


void
SimdIndexArrayInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "Index Array " << std::endl;
}

SimdIndexVSArrayInst::SimdIndexVSArrayInst 
  (size_t arrayElementSize,
   const SimdDataAddrPtr &arrayElementSizePtr, 
   size_t arraySize,
   const SimdDataAddrPtr &arraySizePtr,
   int lineNumber)
    : SimdInst(lineNumber), 
      _arrayElementSize(arrayElementSize),
      _arrayElementSizePtr(arrayElementSizePtr),
      _arraySize(arraySize),
      _arraySizePtr(arraySizePtr)
{
    // empty
}

void
SimdIndexVSArrayInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    int arraySize;
    int arrayElementSize;

    if( _arraySize == 0 )
	arraySize = *(int*)(_arraySizePtr->reg(xcontext)[0]);
    else
	arraySize = _arraySize;

    if( _arrayElementSize == 0 )
	arrayElementSize = *(int*)(_arrayElementSizePtr->reg(xcontext)[0]);
    else
	arrayElementSize = _arrayElementSize;
    
    SimdReg &arrayReg = xcontext.stack().regSpRelative (-2);
    bool transferOwnership = 
	(xcontext.stack().ownerSpRelative(-2) == TAKE_OWNERSHIP);

    SimdReg &indexReg = xcontext.stack().regSpRelative(-1); 
    SimdReg *out = new SimdReg(arrayReg, indexReg, mask, 
			       arrayElementSize,
			       arraySize,
			       xcontext.regSize(),
			       transferOwnership);
    try
    {
	xcontext.stack().pop (2);
	xcontext.stack().push (out, TAKE_OWNERSHIP);
    }
    catch (...)
    {
	delete out;
	throw;
    }

}


void
SimdIndexVSArrayInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "Index VS Array " << std::endl;
}


SimdAccessMemberInst::SimdAccessMemberInst  (size_t offset, int lineNumber)
    : SimdInst(lineNumber), _offset(offset)
{
    // empty
}

void
SimdAccessMemberInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    SimdReg &structReg = xcontext.stack().regSpRelative (-1);
    bool transferOwnership = 
	(xcontext.stack().ownerSpRelative(-1) == TAKE_OWNERSHIP);

    SimdReg *out = new SimdReg(structReg, mask, _offset, 
			       xcontext.regSize(),
			       transferOwnership);

    try
    {
	xcontext.stack().pop (1);
	xcontext.stack().push (out, TAKE_OWNERSHIP);
    }
    catch (...)
    {
	delete out;
	throw;
    }

}


void
SimdAccessMemberInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" <<
		 "Access Member " << std::endl;
}



SimdPushPlaceholderInst::SimdPushPlaceholderInst 
   (size_t eSize, int lineNumber)
       : SimdInst(lineNumber), _eSize(eSize)
{
    // empty
}


void
SimdPushPlaceholderInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    SimdReg *out = new SimdReg(false, _eSize);
    try
    {
	xcontext.stack().push (out, TAKE_OWNERSHIP);
	memset((*out)[0],  0, _eSize);
    }
    catch (...)
    {
	delete out;
	throw;
    }
}

void
SimdPushPlaceholderInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" << "placeholder " << std::endl;
}


SimdFileNameInst::SimdFileNameInst (const string &fileName, int lineNumber)
    : SimdInst(lineNumber), _fileName(fileName)
{
    // empty
}


void
SimdFileNameInst::execute
    (SimdBoolMask &mask,
     SimdXContext &xcontext) const
{
    xcontext.setFileName(_fileName);
}

void
SimdFileNameInst::print (int indent) const
{
    std::cout << std::setw (indent) << "" << "fileName " 
	      << _fileName << std::endl;
}




} // namespace Ctl
