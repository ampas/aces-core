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
//	class SimdLContext
//
//-----------------------------------------------------------------------------

#include <CtlSimdLContext.h>
#include <CtlSimdSyntaxTree.h>
#include <CtlSimdType.h>
#include <CtlSimdModule.h>
#include <CtlSimdReg.h>
#include <CtlSimdInst.h>
#include <CtlSimdAddr.h>
#include <CtlSymbolTable.h>
#include <cassert>

using namespace std;

#if 0
    #include <iostream>
    #define debug(x) (cout << x << endl)
#else
    #define debug(x)
#endif

namespace Ctl {


SimdLContext::SimdLContext
    (istream &file,
     Module *module,
     SymbolTable &symtab):
    LContext (file, module, symtab)
{
    newPath();
    newStackFrame();
}


SimdModule *
SimdLContext::simdModule ()
{
    return static_cast <SimdModule *> (module());
}


void
SimdLContext::mustFixCall (SimdCallInst *inst, const SymbolInfoPtr &info)
{
    debug ("mustFixCall (inst = " << inst << ", info = " << &info << ")");
    _fixCallsList.push_back (FixCall (inst, info));
}


void
SimdLContext::fixCalls ()
{
    debug ("fixCalls ()");

    for (FixCallsList::const_iterator i = _fixCallsList.begin();
	 i != _fixCallsList.end();
	 ++i)
    {
	SimdInstAddrPtr addr = i->info->addr();
	assert (addr->inst());

	i->inst->setCallPath (addr->inst());
	debug ("\tcall inst = " << i->inst << ", call path = " << addr->inst());
    }
}


void	
SimdLContext::addInst (SimdInst *inst)
{
    simdModule()->addInst (inst);

    if (_currentPath.firstInst == 0)
	_currentPath.firstInst = inst;

    if (_currentPath.lastInst != 0)
	_currentPath.lastInst->setNextInPath (inst);

    _currentPath.lastInst = inst;
}


void
SimdLContext::appendPath(const Path &path)
{
    if( _currentPath.lastInst )
    {
	_currentPath.lastInst->setNextInPath(path.firstInst);
    }
    else
    {
	_currentPath.firstInst= path.firstInst;
    }
    _currentPath.lastInst = path.lastInst;

}

void
SimdLContext::addStaticData (SimdReg *reg)
{
    simdModule()->addStaticData (reg);
}


void
SimdLContext::newPath ()
{
    _currentPath.firstInst = 0;
    _currentPath.lastInst = 0;
}


void
SimdLContext::newStackFrame ()
{
    _nextParameterAddr = -1;
    _locals.clear();
}


AddrPtr
SimdLContext::parameterAddr (const DataTypePtr &parameterType)
{
    return new SimdDataAddr (_nextParameterAddr--);
}


AddrPtr
SimdLContext::returnValueAddr (const DataTypePtr &returnType)
{
    return new SimdDataAddr (_nextParameterAddr--);
}


AddrPtr
SimdLContext::autoVariableAddr (const DataTypePtr &variableType)
{
    _locals.push_back(variableType);
    return new SimdDataAddr (_locals.size() - 1);
}


ModuleNodePtr
SimdLContext::newModuleNode
    (int lineNumber,
     const StatementNodePtr &constants,
     const FunctionNodePtr &functions) const
{
    return new SimdModuleNode (lineNumber, constants, functions);
}


FunctionNodePtr
SimdLContext::newFunctionNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info,
     const StatementNodePtr &body) const
{
    return new SimdFunctionNode (lineNumber, name, info, body, _locals);
}


VariableNodePtr
SimdLContext::newVariableNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info,
     const ExprNodePtr &initialValue,
     bool assignInitialValue) const
{
    return new SimdVariableNode
	(lineNumber, name, info, initialValue, assignInitialValue);
}


AssignmentNodePtr
SimdLContext::newAssignmentNode
    (int lineNumber,
     const ExprNodePtr &lhs,
     const ExprNodePtr &rhs) const
{
    return new SimdAssignmentNode (lineNumber, lhs, rhs);
}


ExprStatementNodePtr
SimdLContext::newExprStatementNode 
    (int lineNumber,
     const ExprNodePtr &expr) const
{
    return new SimdExprStatementNode (lineNumber, expr);
}


IfNodePtr
SimdLContext::newIfNode
    (int lineNumber,
     const ExprNodePtr &condition,
     const StatementNodePtr &truePath,
     const StatementNodePtr &falsePath) const
{
    return new SimdIfNode (lineNumber, condition, truePath, falsePath);
}


ReturnNodePtr
SimdLContext::newReturnNode
    (int lineNumber,
     const SymbolInfoPtr &info,
     const ExprNodePtr &returnedValue) const
{
    return new SimdReturnNode (lineNumber, info, returnedValue);
}


WhileNodePtr
SimdLContext::newWhileNode
    (int lineNumber,
     const ExprNodePtr &condition,
     const StatementNodePtr &loopBody) const
{
    return new SimdWhileNode (lineNumber, condition, loopBody);
}


BinaryOpNodePtr
SimdLContext::newBinaryOpNode
    (int lineNumber,
     Token op,
     const ExprNodePtr &leftOperand,
     const ExprNodePtr &rightOperand) const
{
    return new SimdBinaryOpNode (lineNumber, op, leftOperand, rightOperand);
}


UnaryOpNodePtr
SimdLContext::newUnaryOpNode
    (int lineNumber,
     Token op,
     const ExprNodePtr &operand) const
{
    return new SimdUnaryOpNode (lineNumber, op, operand);
}


ArrayIndexNodePtr
SimdLContext::newArrayIndexNode
    (int lineNumber,
     const ExprNodePtr &array,
     const ExprNodePtr &index) const
{
    return new SimdArrayIndexNode (lineNumber, array, index);
}

MemberNodePtr
SimdLContext::newMemberNode
    (int lineNumber,
     const ExprNodePtr &obj,
     const std::string &member) const
{
    return new SimdMemberNode (lineNumber, obj, member);
}


SizeNodePtr
SimdLContext::newSizeNode
    (int lineNumber,
     const ExprNodePtr &obj) const
{
    return new SimdSizeNode (lineNumber, obj);
}

NameNodePtr
SimdLContext::newNameNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info) const
{
    return new SimdNameNode (lineNumber, name, info);
}


BoolLiteralNodePtr
SimdLContext::newBoolLiteralNode (int lineNumber, bool value) const
{
    return new SimdBoolLiteralNode (lineNumber, *this, value);
}


IntLiteralNodePtr
SimdLContext::newIntLiteralNode (int lineNumber, int value) const
{
    return new SimdIntLiteralNode (lineNumber, *this, value);
}


UIntLiteralNodePtr
SimdLContext::newUIntLiteralNode (int lineNumber, unsigned value) const
{
    return new SimdUIntLiteralNode (lineNumber, *this, value);
}


HalfLiteralNodePtr
SimdLContext::newHalfLiteralNode (int lineNumber, half value) const
{
    return new SimdHalfLiteralNode (lineNumber, *this, value);
}


FloatLiteralNodePtr
SimdLContext::newFloatLiteralNode (int lineNumber, float value) const
{
    return new SimdFloatLiteralNode (lineNumber, *this, value);
}


StringLiteralNodePtr
SimdLContext::newStringLiteralNode (int lineNumber, const string &value) const
{
    return new SimdStringLiteralNode (lineNumber, *this, value);
}


CallNodePtr
SimdLContext::newCallNode
    (int lineNumber,
     const NameNodePtr &function,
     const ExprNodeVector &arguments) const
{
    return new SimdCallNode (lineNumber, function, arguments);
}


ValueNodePtr
SimdLContext::newValueNode
    (int lineNumber,
     const ExprNodeVector &elements) const
{
    return new SimdValueNode (lineNumber, elements);
}


VoidTypePtr
SimdLContext::newVoidType () const
{
    static VoidTypePtr voidType = new SimdVoidType;
    return voidType;
}


BoolTypePtr
SimdLContext::newBoolType () const
{
    static BoolTypePtr boolType = new SimdBoolType ();
    return boolType;
}


IntTypePtr
SimdLContext::newIntType () const
{
    static IntTypePtr intType = new SimdIntType ();
    return intType;
}


UIntTypePtr
SimdLContext::newUIntType () const
{
    static UIntTypePtr uintType = new SimdUIntType ();
    return uintType;
}


HalfTypePtr
SimdLContext::newHalfType () const
{
    static HalfTypePtr halfType = new SimdHalfType ();
    return halfType;
}


FloatTypePtr
SimdLContext::newFloatType () const
{
    static FloatTypePtr floatType = new SimdFloatType ();
    return floatType;
}


StringTypePtr
SimdLContext::newStringType () const
{
    static StringTypePtr stringType = new SimdStringType ();
    return stringType;
}


ArrayTypePtr
SimdLContext::newArrayType (const DataTypePtr &baseType, 
			    int size,
			    ArrayTypeUsage usage /*= NON_PARAMETER */)
{
    if( usage == PARAMETER)
	return new SimdArrayType (baseType, size, this);
    else
	return new SimdArrayType (baseType, size);
}


StructTypePtr	
SimdLContext::newStructType
   (const std::string &name,
    const MemberVector &members) const
{
    return new SimdStructType(name, members);
}

FunctionTypePtr
SimdLContext::newFunctionType
    (const DataTypePtr &returnType,
     bool returnVarying,
     const ParamVector &parameters) const
{
    return new SimdFunctionType (returnType, returnVarying, parameters);
}


} // namespace Ctl
