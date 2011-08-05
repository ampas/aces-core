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
//	class SimdFunctionCall, class SimdFunctionArg
//
//-----------------------------------------------------------------------------

#include <CtlSimdFunctionCall.h>
#include <Iex.h>
#include <CtlSymbolTable.h>
#include <assert.h>
#include <CtlSimdInterpreter.h>

using namespace std;
using namespace Iex;

namespace Ctl {


SimdFunctionCall::SimdFunctionCall
    (SimdInterpreter &interpreter,
     const string &name,
     FunctionTypePtr type,
     SimdInstAddrPtr addr,
     SymbolTable &symbols)
:
     FunctionCall (name),
     _xcontext (interpreter),
     _entryPoint (addr->inst()),
     _symbols(symbols)
{
    {
	SimdReg *returnReg =
	    new SimdReg (type->returnVarying(),
			 type->returnType()->alignedObjectSize());

	_xcontext.stack().push (returnReg, TAKE_OWNERSHIP);

	setReturnValue (new SimdFunctionArg ("",
					     this,
					     type->returnType(),
					     type->returnVarying(),
					     returnReg));
    }

    const ParamVector &parameters = type->parameters();

    vector<FunctionArgPtr> inputs;
    vector<FunctionArgPtr> outputs;
    for (int i = parameters.size() - 1; i >= 0; --i)
    {
	const Param &param = parameters[i];

	SimdReg *paramReg =
	    new SimdReg (param.varying,
		         param.type->alignedObjectSize());

	_xcontext.stack().push (paramReg, TAKE_OWNERSHIP);

	FunctionArgPtr arg = new SimdFunctionArg (param.name,
						  this,
						  param.type,
						  param.varying,
						  paramReg);
	if (param.isWritable())
	    outputs.push_back(arg);
	else
	    inputs.push_back(arg);
    }

    int count = 0;
    for(vector<FunctionArgPtr>::reverse_iterator it = inputs.rbegin();
	it != inputs.rend();
	++it)
    {
	setInputArg (count++, *it);
    }

    count = 0;
    for(vector<FunctionArgPtr>::reverse_iterator it = outputs.rbegin();
	it != outputs.rend();
	++it)
    {
	setOutputArg (count++, *it);
    }
}


void	
SimdFunctionCall::callFunction (size_t numSamples)
{
    StackFrame stackFrame (_xcontext);

    _xcontext.run (numSamples, _entryPoint);

    {
	const SimdFunctionArgPtr arg = returnValue();

	if (arg->isVarying() && !arg->reg()->isVarying())
	{
	    arg->reg()->setVarying (true);
	}
	else if (!arg->isVarying() && arg->reg()->isVarying())
	{
	    THROW (TypeExc,
		   "The return type of CTL function " <<
		   arg->func()->name() << " is uniform, "
		   "but the function returned a varying value.");
	}
    }

    for (size_t i = 0; i < numOutputArgs(); ++i)
    {
	const SimdFunctionArgPtr arg = outputArg (i);

	if (arg->isVarying() && !arg->reg()->isVarying())
	{
	    arg->reg()->setVarying (true);
	}
	else if (!arg->isVarying() && arg->reg()->isVarying())
	{
	    THROW (TypeExc,
		   "Output parameter " << arg->name() << " of CTL "
		   "function " << arg->func()->name() << " is uniform, "
		   "but the function returned a varying value.");
	}
    }
}


SimdFunctionArg::SimdFunctionArg
    (const std::string &name,
     const FunctionCallPtr &func,
     const DataTypePtr &type,
     bool varying,
     SimdReg *reg)
:
    FunctionArg (name, func, type, varying),
    _reg (reg),
    _defaultReg (0)
{
    // Find the register associated with the parameter default value
    string staticName = func->name() + "$" + name;

    SimdFunctionCallPtr sfunc(func);
    SymbolInfoPtr info = sfunc->symbols().lookupSymbol( staticName );
    if( info )
    {
	_defaultReg = &SimdDataAddrPtr(info->addr())->reg(*sfunc->xContext());
    }
}


SimdFunctionArg::~SimdFunctionArg ()
{
    // empty
}


char *
SimdFunctionArg::data ()
{
    return (*_reg)[0];
}


bool
SimdFunctionArg::hasDefaultValue ()
{
    return _defaultReg != 0;
}


void
SimdFunctionArg::setDefaultValue ()
{
    assert(_reg);
    if( _defaultReg )
    {
        // note: default values are never varying
        // Copy default value into the argument's register, replicating
        // if the argument is varying.
        if(_reg->isVarying())
        {
	    for (int i = MAX_REG_SIZE; --i >= 0;)
                memcpy((*_reg)[i], (*_defaultReg)[0], _reg->elementSize());
        }
        else
        {
            memcpy((*_reg)[0], (*_defaultReg)[0], _reg->elementSize());
        }
    }
}

void
SimdFunctionArg::setVarying(bool varying)
{
    assert(_reg);
    _reg->setVarying(varying);
    FunctionArg::setVarying(_reg->isVarying());
}

size_t SimdFunctionArg::elements(void) const {
	SimdFunctionCallPtr fc(func());

	return fc->xContext()->interpreter().maxSamples();
}

} // namespace Ctl
