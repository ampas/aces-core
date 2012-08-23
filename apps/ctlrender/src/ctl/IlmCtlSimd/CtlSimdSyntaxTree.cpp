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
//	The syntax tree for the SIMD implementation of the
//	color transformation language.
//
//-----------------------------------------------------------------------------

#include <CtlSimdSyntaxTree.h>
#include <CtlSimdModule.h>
#include <CtlSimdLContext.h>
#include <CtlSimdInst.h>
#include <CtlSymbolTable.h>
#include <CtlSimdType.h>
#include <cassert>
#include <vector>
#include <CtlSimdOp.h>

using namespace std;

#if 0
    #include <iostream>
    #define debug(x) (cout << x << endl)
    #define debug_only(x) x
#else
    #define debug(x)
    #define debug_only(x)
#endif

namespace Ctl {
namespace {

SimdInst *
generateCodeForPath (ExprNodePtr node, SimdLContext &slcontext)
{
    slcontext.newPath();
    node->generateCode (slcontext);
    return slcontext.currentPath().firstInst;
}



SimdInst *
generateCodeForPath (StatementNodePtr node, SimdLContext &slcontext, 
		     const SimdLContext::Path *insertPath = 0,
		     const vector<DataTypePtr> *locals = 0)
{
    if( !node )
    {
	return 0;
    }


    slcontext.newPath();

    if( locals )
    {
	// Insert instructions to hold space for local variables on stack
	for(vector<DataTypePtr>::const_iterator type = locals->begin();
	    type != locals->end(); 
	    type++)
	{
	    (*type)->newAutomaticVariable(node, slcontext);
	}
    }
    
    if(insertPath && insertPath->firstInst)
    {
	slcontext.appendPath(*insertPath);
    }

    while (node)
    {
	node->generateCode (slcontext);
	node = node->next;
    }

    return slcontext.currentPath().firstInst;
}

} //namespace


SimdModuleNode::SimdModuleNode
    (int lineNumber,
     const StatementNodePtr &constants,
     const FunctionNodePtr &functions)
:
    ModuleNode (lineNumber, constants, functions)
{
    // empty
}


void
SimdModuleNode::generateCode (LContext &lcontext)
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    //
    // Generate code for initialization of static constants.
    //

    if (constants)
    {
	debug ("generating module initialization code");

	SimdInst *firstInitInst = generateCodeForPath (constants, slcontext);
	slcontext.simdModule()->setFirstInitInst (firstInitInst);

	debug_only (if (firstInitInst) firstInitInst->printPath (1));
    }

    //
    // Generate code for functions.
    //

    FunctionNodePtr function = functions;

    while (function)
    {
	function->generateCode (lcontext);
	function = function->next;
    }

    //
    // Fixup function call instructions whose callPath was
    // not known when the instructions were generated (see
    // SimdCallNode::generateCode().
    // 

    slcontext.fixCalls();
}


SimdFunctionNode::SimdFunctionNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info,
     const StatementNodePtr &body,
     const std::vector<DataTypePtr> locals)
:
    FunctionNode (lineNumber, name, info, body)
{
    _locals = locals;


}


void
SimdFunctionNode::generateESizeCode(SimdLContext &slcontext, 
				    SimdArrayTypePtr arrayType)
{
    if( arrayType && arrayType->unknownElementSize())
    {
	// push unknownESize
	slcontext.addInst 
	    (new SimdPushRefInst (arrayType->unknownElementSize(), lineNumber));

	// we know sub-type is an array
	SimdArrayTypePtr subArrayType = arrayType->elementType();
	generateESizeCode(slcontext, subArrayType);

	// push sub elementSize : reference to computed or literal
	if( subArrayType->unknownElementSize() )
	    slcontext.addInst 
		(new SimdPushRefInst(subArrayType->unknownElementSize(), 
				     lineNumber));
	else
	    slcontext.addInst 
		(new SimdPushLiteralInst <int> (subArrayType->elementSize(), 
						lineNumber));

	// push sub size : reference to computed or literal
	if( subArrayType->unknownSize() )
	    slcontext.addInst 
		(new SimdPushRefInst (subArrayType->unknownSize(), lineNumber));
	else
	    slcontext.addInst 
		(new SimdPushLiteralInst <int> (subArrayType->size(), 
						lineNumber));
	// multiply
	slcontext.addInst
	    (new SimdBinaryOpInst <int, int, int, TimesOp> (lineNumber));

	// assign
	slcontext.addInst (new SimdAssignInst (sizeof(int), lineNumber));
    }
}


void
SimdFunctionNode::generateCode (LContext &lcontext)
{

    debug ("generating code for function " << name);

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    SimdFunctionTypePtr functionType = info->functionType();

    // step through arguments, if array pointer and unknown element size
    const ParamVector params = functionType->parameters();
    slcontext.newPath();
    slcontext.addInst (new SimdFileNameInst(lcontext.fileName(), lineNumber));
    for(int i = params.size()-1; i >= 0; i--)
    {
	generateESizeCode(slcontext, params[i].type.cast<SimdArrayType>());
    }
    SimdLContext::Path path = slcontext.currentPath();

    SimdInst *firstBodyInst = 
	generateCodeForPath (body, slcontext, &path, &_locals);

    info->setAddr (new SimdInstAddr (firstBodyInst));
    debug_only (if (firstBodyInst) firstBodyInst->printPath (1));
}


SimdVariableNode::SimdVariableNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info,
     const ExprNodePtr &initialValue,
     bool assignInitialValue)
:
    VariableNode (lineNumber, name, info, initialValue, assignInitialValue)
{
    // empty
}


void
SimdVariableNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a variable declaration.
    //
    // If the variable has no initial value, then there is nothing to do.
    //
    // If the variable does have an initial value, then there are two cases:
    //
    //     If the initial value is assigned to the variable,
    //	   then generate code to
    //         - push a reference to the variable onto the stack
    //         - evaluate the expression that computes the initial value
    //         - cast the value to the type of the variable
    //         - assign initial value to the variable.
    //
    //     Otherwise the variable is initialized by a side-effect of
    //     the initialization expression.  In this case we generate
    //     code to
    //         - evaluate the expression that computes the initial value
    //         - pop the expression's value off the stack (unless the
    //           expression's value is of type void).
    // 

    if (initialValue)
    {
	SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
	
	SimdDataAddrPtr dataPtr = info->addr().cast<SimdDataAddr>();
	SimdValueNodePtr valuePtr = initialValue.cast<SimdValueNode>();

	if (assignInitialValue)
	{
	    //
	    // Initial value is assigned to the variable.
	    //

	    if( valuePtr && valuePtr->type && dataPtr  && dataPtr->reg())
	    {
		
		// get sizes & offsets of elements
		SizeVector sizes;
		SizeVector offsets;
		DataTypePtr dataType = valuePtr->type;
		dataType->coreSizes(0, sizes, offsets);

		ExprNodeVector &elements = valuePtr->elements;
		int numElements = elements.size();
		assert((int)sizes.size() == numElements 
		       && (int)offsets.size() == numElements);
		assert(!dataPtr->reg()->isVarying());
		
		char* dest = (*dataPtr->reg())[0];
		
		int eIndex = 0;

		valuePtr->castAndCopyRec(lcontext, dataType, eIndex, 
					 dest, sizes, offsets);

	    }
	    else
	    {
		slcontext.addInst (new SimdPushRefInst (info->addr(), lineNumber));
		initialValue->generateCode (lcontext);
		info->type()->generateCastFrom (initialValue, lcontext);

		info->type()->generateCode (this, lcontext);
	    }
	}
	else
	{
	    //
	    // Variable is initialized via side-effect.
	    //

	    initialValue->generateCode (lcontext);

	    const SimdCallNode *call = 
		dynamic_cast <const SimdCallNode*>(initialValue.pointer());
	    
	    RcPtr<SimdVoidType> pv(new SimdVoidType());

	    if (call == 0 || !call->returnsType (pv))
	    {
		slcontext.addInst (new SimdPopInst (1, lineNumber));
	    }
	}
    }
}




SimdAssignmentNode::SimdAssignmentNode
    (int lineNumber,
     const ExprNodePtr &lhs,
     const ExprNodePtr &rhs)
:
    AssignmentNode (lineNumber, lhs, rhs)
{
    // empty
}


void
SimdAssignmentNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for an assigment statement:
    //
    // Evaluate the left-hand-side and right-hand-side expressions,
    // cast the right-hand-side value to the type of the left-hand-side,
    // assign the value to the left-hand-side.
    // 

    lhs->generateCode (lcontext);
    rhs->generateCode (lcontext);
    lhs->type->generateCastFrom (rhs, lcontext);
    lhs->type->generateCode (this, lcontext);
}


SimdExprStatementNode::SimdExprStatementNode
    (int lineNumber, const ExprNodePtr &expr)
:
    ExprStatementNode (lineNumber, expr)
{
    // empty
}


void
SimdExprStatementNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for an expression statement:
    //
    // Evaluate the expression.
    // Pop its value off the top of the stack (unless it is a void
    // function).
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    expr->generateCode (lcontext);

    const SimdCallNode *call = 
	dynamic_cast <const SimdCallNode*>(expr.pointer()) ;
    
    RcPtr<SimdVoidType> pv(new SimdVoidType());
    if( call == 0 || !call->returnsType(pv))
    {
	slcontext.addInst (new SimdPopInst (1, lineNumber));
    }
}


SimdIfNode::SimdIfNode
    (int lineNumber,
     const ExprNodePtr &condition,
     const StatementNodePtr &truePath,
     const StatementNodePtr &falsePath)
:
    IfNode (lineNumber, condition, truePath, falsePath)
{
    // empty
}


void
SimdIfNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for an if statement:
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    //
    // Append code for the expression that computes the condition to
    // the "main path", that is, to the path that contains the if statement. 
    // Cast value of the expression to type bool.
    //

    condition->generateCode (lcontext);
    SimdLContext::Path mainInstPath = slcontext.currentPath();

    BoolTypePtr boolType = lcontext.newBoolType ();
    boolType->generateCastFrom (condition, lcontext);

    //
    // Generate code for the true and false paths.
    //

    const SimdInst *firstTrueInst = generateCodeForPath (truePath, slcontext);
    const SimdInst *firstFalseInst = generateCodeForPath (falsePath, slcontext);

    //
    // Append a branch instruction to the main path.
    //

    slcontext.setCurrentPath (mainInstPath);
    slcontext.addInst (new SimdBranchInst (firstTrueInst, firstFalseInst, 
					   false,  lineNumber));
}


SimdReturnNode::SimdReturnNode
    (int lineNumber,
     const SymbolInfoPtr &info,
     const ExprNodePtr &returnedValue)
:
    ReturnNode (lineNumber, info, returnedValue)
{
    // empty
}


void
SimdReturnNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a return statement.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (returnedValue)
    {
	//
	// Push a reference to the location for the
	// return value onto the stack.
	//

	slcontext.addInst (new SimdPushRefInst (info->addr(), lineNumber));

	//
	// Generate code for the expression that computes the return value.
	//

	returnedValue->generateCode (lcontext);

	//
	// Cast the return value to the return type of the function
	// that contains this return statement.
	//

	info->type()->generateCastFrom (returnedValue, lcontext);

	//
	// Copy the value into the location for the return value.
	//

	info->type()->generateCode (this, lcontext);
    }

    //
    // Return from the function.
    //

    slcontext.addInst (new SimdReturnInst (lineNumber));
}


SimdWhileNode::SimdWhileNode
    (int lineNumber,
     const ExprNodePtr &condition,
     const StatementNodePtr &loopBody)
:
    WhileNode (lineNumber, condition, loopBody)
{
    // empty
}


void
SimdWhileNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a while statement.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    //
    // Generate a code path for the expression that computes the
    // loop condition.  Cast the value of the expression to type bool.
    // 

    SimdLContext::Path mainInstPath = slcontext.currentPath();

    const SimdInst *firstCondInst =
	generateCodeForPath (condition, slcontext);

    BoolTypePtr boolType = lcontext.newBoolType ();
    boolType->generateCastFrom (condition, lcontext);

    //
    // Generate a code path for the loop body.
    //

    const SimdInst *firstLoopBodyInst =
	generateCodeForPath (loopBody, slcontext);

    //
    // Append a loop instruction to the "main path", that is,
    // the path that contains the while statement.
    //

    slcontext.setCurrentPath (mainInstPath);
    slcontext.addInst (new SimdLoopInst (firstCondInst, firstLoopBodyInst, 
					 lineNumber));
}


SimdBinaryOpNode::SimdBinaryOpNode
    (int lineNumber,
     Token op,
     const ExprNodePtr &leftOperand,
     const ExprNodePtr &rightOperand)
:
    BinaryOpNode (lineNumber, op, leftOperand, rightOperand)
{
    // empty
}


void
SimdBinaryOpNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a binary operator.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (op == TK_AND)
    {
	//
	// The right operand of an "and" is evaluated only if the left
	// operand is true.  We implement the "and" roughly like this:
	//
	//     if (leftOperand)
	//         rightOperand
	//     else
	//         false
	//

	//
	// In the "main path", that is, the path that contains the
	// "and", generate code to evaluate the left operand and
	// to cast the value to type bool.
	//

	BoolTypePtr boolType = lcontext.newBoolType ();

	leftOperand->generateCode (lcontext);
	boolType->generateCastFrom (leftOperand, lcontext);

	SimdLContext::Path mainInstPath = slcontext.currentPath();

	//
	// In a new "true path", generate code to evaluate the right
	// operand and to cast the value to type bool.
	//

	const SimdInst *firstTrueInst =
	    generateCodeForPath (rightOperand, slcontext);

	boolType->generateCastFrom (leftOperand, lcontext);

	//
	// In a new "false path", generate code that produces "false".
	//

	slcontext.newPath();
	slcontext.addInst (new SimdPushLiteralInst <bool> (false, lineNumber));

	const SimdInst *firstFalseInst =
	    slcontext.currentPath().firstInst;

	//
	// Append a branch to the main path.  This selects between
	// the right operand and "false", depending on the value
	// of the left operand.
	//

	slcontext.setCurrentPath (mainInstPath);
	slcontext.addInst (new SimdBranchInst (firstTrueInst, firstFalseInst, 
					       true, lineNumber));
    }
    else if (op == TK_OR)
    {
	//
	// The right operand of an "or" is evaluated only if the left
	// operand is false.  We implement the "or" roughly like this:
	//
	//     if (leftOperand)
	//         true
	//     else
	//         rightOperand
	//

	//
	// In the "main path", that is, the path that contains the
	// "and", generate code to evaluate the left operand and
	// to cast the value to type bool.
	//

	BoolTypePtr boolType = lcontext.newBoolType ();

	leftOperand->generateCode (lcontext);
	boolType->generateCastFrom (leftOperand, lcontext);

	SimdLContext::Path mainInstPath = slcontext.currentPath();

	//
	// In a new "true path", generate code that produces "true".
	//

	slcontext.newPath();
	slcontext.addInst (new SimdPushLiteralInst <bool> (true, lineNumber));

	const SimdInst *firstTrueInst =
	    slcontext.currentPath().firstInst;

	//
	// In a new "false path", generate code to evaluate the right
	// operand and to cast the value to type bool.
	//

	const SimdInst *firstFalseInst =
	    generateCodeForPath (rightOperand, slcontext);

	boolType->generateCastFrom (leftOperand, lcontext);

	//
	// Append a branch to the main path.  This selects between
	// "true" and the right operand, depending on the value
	// of the left operand.
	//

	slcontext.setCurrentPath (mainInstPath);
	slcontext.addInst (new SimdBranchInst (firstTrueInst, firstFalseInst, 
					       true, lineNumber));
    }
    else
    {
	//
	// Operations other than "and" and "or":
	//
	// Evaluate the left and right operands and cast their
	// values to a common type, indicated by operandType.
	// Then perform the binary operation.
	//

	leftOperand->generateCode (lcontext);
	operandType->generateCastFrom (leftOperand, lcontext);
	rightOperand->generateCode (lcontext);
	operandType->generateCastFrom (rightOperand, lcontext);
	operandType->generateCode (this, lcontext);
    }
}


SimdUnaryOpNode::SimdUnaryOpNode
    (int lineNumber,
     Token op,
     const ExprNodePtr &operand)
:
    UnaryOpNode (lineNumber, op, operand)
{
    // empty
}


void
SimdUnaryOpNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a unary operator:
    //
    // Evaluate the operand, cast it to the type that
    // the operation will produce, and perform the operation.
    //

    operand->generateCode (lcontext);
    type->generateCastFrom (operand, lcontext);
    type->generateCode (this, lcontext);
}


SimdArrayIndexNode::SimdArrayIndexNode
    (int lineNumber,
     const ExprNodePtr &array,
     const ExprNodePtr &index)
:
    ArrayIndexNode (lineNumber, array, index)
{
    // empty
}


void
SimdArrayIndexNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for an array indexing operation:
    //
    // Push a reference to the array onto the stack,
    // evaluate the expression for the index, cast the
    // index to type int, and perform the indexing
    // operation.
    //

    array->generateCode (lcontext);
    index->generateCode (lcontext);

    IntTypePtr intType = lcontext.newIntType ();
    intType->generateCastFrom (index, lcontext);

    array->type->generateCode (this, lcontext);
}


SimdMemberNode::SimdMemberNode
    (int lineNumber,
     const ExprNodePtr &obj,
     const std::string &member)
:
	MemberNode (lineNumber, obj, member)
{
    // empty
}


void
SimdMemberNode::generateCode (LContext &lcontext)
{
    //
    //
    //


    obj->generateCode (lcontext);
    obj->type->generateCode(this, lcontext);

}


SimdSizeNode::SimdSizeNode
    (int lineNumber,
     const ExprNodePtr &obj)
:
	SizeNode (lineNumber, obj)
{
    // empty
}


void
SimdSizeNode::generateCode (LContext &lcontext)
{
    //
    //
    //

//    obj->generateCode (lcontext);
    obj->type->generateCode(this, lcontext);

}



SimdNameNode::SimdNameNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info)
:
    NameNode (lineNumber, name, info)
{
    // empty
}


void
SimdNameNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a reference to a variable.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushRefInst (info->addr(), lineNumber));
}


SimdBoolLiteralNode::SimdBoolLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     bool value)
:
    BoolLiteralNode (lineNumber, lcontext, value)
{
    // empty
}


void
SimdBoolLiteralNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a literal value of type bool.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushLiteralInst <bool> (value, lineNumber));
}

char*
SimdBoolLiteralNode::valuePtr()
{
    return (char*)(&value);
}



SimdIntLiteralNode::SimdIntLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     int value)
:
    IntLiteralNode (lineNumber, lcontext, value)
{
    // empty
}


void
SimdIntLiteralNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a literal value of type int.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushLiteralInst <int> (value, lineNumber));
}

char*
SimdIntLiteralNode::valuePtr()
{
    return (char*)(&value);
}


SimdUIntLiteralNode::SimdUIntLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     unsigned int value)
:
    UIntLiteralNode (lineNumber, lcontext, value)
{
    // empty
}


void
SimdUIntLiteralNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a literal value of type int.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushLiteralInst <unsigned> (value, lineNumber));
}

char*
SimdUIntLiteralNode::valuePtr()
{
    return (char*)(&value);
}


SimdHalfLiteralNode::SimdHalfLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     half value)
:
    HalfLiteralNode (lineNumber, lcontext, value)
{
    // empty
}


void
SimdHalfLiteralNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a literal value of type float.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushLiteralInst <half> (value, lineNumber));
}

char*
SimdHalfLiteralNode::valuePtr()
{
    return (char*)(&value);
}

SimdFloatLiteralNode::SimdFloatLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     float value)
:
    FloatLiteralNode (lineNumber, lcontext, value)
{
    // empty
}


void
SimdFloatLiteralNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a literal value of type float.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushLiteralInst <float> (value, lineNumber));
}

char*
SimdFloatLiteralNode::valuePtr()
{
    return (char*)(&value);
}

SimdStringLiteralNode::SimdStringLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     const string &value)
:
    StringLiteralNode (lineNumber, lcontext, value)
{
    // empty
}


void
SimdStringLiteralNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a literal value of type float.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushStringLiteralInst (value, lineNumber));
}

char*
SimdStringLiteralNode::valuePtr()
{
    return 0;
}

SimdCallNode::SimdCallNode
    (int lineNumber,
     const NameNodePtr &function,
     const ExprNodeVector &arguments)
:
    CallNode (lineNumber, function, arguments)
{
    // empty
}

bool
SimdCallNode::returnsType(const TypePtr &t) const
{
    SymbolInfoPtr info = function->info;
    FunctionTypePtr functionType = function->info->functionType();
    DataTypePtr returnType = functionType->returnType();
    if( returnType->isSameTypeAs(t))
	return true;
    return false;
}



void
SimdCallNode::generateCode (LContext &lcontext)
{
    //
    // Generate code for a function call.
    //

    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    SymbolInfoPtr info = function->info;
    FunctionTypePtr functionType = info->functionType();

    //
    // Reserve space on the stack for the function's return value
    //

    functionType->returnType()->generateCode (this, lcontext);
    
    //
    // Push the arguments for the call onto the stack.
    //

    const ParamVector &parameters = functionType->parameters();

    int numParameters = parameters.size();
    for (int i = parameters.size() - 1; i >= 0; --i)
    {
	ExprNodePtr nextArg;

	if (i >= (int)arguments.size())
	{
	    //
	    // Argument is not explicity specified; use the default
	    // value for the corresponding function parameter.
	    //
	    nextArg = parameters[i].defaultValue;
	}
	else
	{
	    //
	    // Argument is explicitly specified.
	    //
	    nextArg = arguments[i];
	}

	nextArg->generateCode (lcontext);
	parameters[i].type->generateCastFrom(nextArg, lcontext);

	// If the size is unknown, push the unknown sizes onto the array
	//
	SimdArrayTypePtr arrayParam = parameters[i].type.cast<SimdArrayType>();
	if(arrayParam)
	{
	    SimdArrayTypePtr arrayArg = nextArg->type.cast<SimdArrayType>();
	    assert(arrayArg);

            SizeVector unknown;
	    arrayParam->sizes(unknown);
	    for(int i = 0; i < (int)unknown.size(); i++)
	    {
                assert(arrayArg);
		if(unknown[i] == 0)
		{
                    if(arrayArg->size() == 0)
                    {
                        slcontext.addInst(new SimdPushRefInst 
                                          (arrayArg->unknownSize(),
                                             lineNumber));
                    }
                    else
                    {
                        slcontext.addInst 
                            (new SimdPushLiteralInst<int>(int(arrayArg->size()), 
                                                          lineNumber));
                    }
		    numParameters++;
		}
                arrayArg = arrayArg->elementType().cast<SimdArrayType>();
	    }
            assert(!arrayArg);
	}
    }

    //
    // Output a SimdCallInst if the called function is written in CTL,
    // or a SimdCCallInst if the called function is written in C++.
    //

    if (SimdCFuncAddrPtr addr = info->addr().cast<SimdCFuncAddr>())
    {
	//
	// Called function is C++ code.
	//

	slcontext.addInst (new SimdCCallInst (addr->func(), 
					      numParameters,
					      lineNumber));
    }
    else if (SimdInstAddrPtr addr = info->addr().cast<SimdInstAddr>())
    {
	//
	// Called function is CTL code, address is known.
	//

	slcontext.addInst (new SimdCallInst (addr->inst(), 
					     numParameters, 
					     lineNumber));
    }
    else
    {
	//
	// Called function is CTL code, address is unknown.
	// Output a function call instruction with a null callPath,
	// and record its location.  We will fix the instruction's
	// callPath later, when we know the address of the called
	// function (see SimdModuleNode::generateCode()).
	//

	SimdCallInst *inst = new SimdCallInst(0, numParameters, lineNumber);
	slcontext.addInst (inst);
	slcontext.mustFixCall (inst, info);
    }

    slcontext.addInst (new SimdFileNameInst(lcontext.fileName(), lineNumber));

}


SimdValueNode::SimdValueNode
    (int lineNumber,
     const ExprNodeVector &elements)
:
    ValueNode (lineNumber, elements)
{
    // empty
}


void
SimdValueNode::generateCode (LContext &lcontext)
{
    int eIndex = 0;
    generateCodeRec(lcontext, type, eIndex);
}


void
SimdValueNode::generateCodeRec (LContext &lcontext, 
				const DataTypePtr &dataType, 
				int& eIndex)
{
    // For each element in the array, 
    //    Evaluate the expression,
    //    cast the value to the type of the array
    // Then assign multiple to the array
    //
    if( StructTypePtr structType = dataType.cast<StructType>())
    {
	for(MemberVectorConstIterator it = structType->members().begin();
	    it != structType->members().end();
	    it++)
	{
	    generateCodeRec(lcontext, it->type, eIndex);
	}
    }
    else if( ArrayTypePtr arrayType = dataType.cast<ArrayType>())
    {
	for (int i = 0; i < arrayType->size(); ++i)
	{
	    generateCodeRec(lcontext, arrayType->elementType(), eIndex);
	}
    }
    else
    {
	assert(eIndex < (int)elements.size());
	elements[eIndex]->generateCode(lcontext);
	dataType->generateCastFrom(elements[eIndex], lcontext);
	eIndex++;
    }
}


void
SimdValueNode::castAndCopyRec(LContext &lcontext,
			      const DataTypePtr &dataType,
			      int &eIndex,
			      char *dest,
			      const SizeVector &sizes,
			      const SizeVector &offsets)
{
    // For each element in the array, 
    //    cast to the correct type
    //    and copy into dest
    // Then assign multiple to the array
    //
    if( StructTypePtr structType = dataType.cast<StructType>())
    {
	for(MemberVectorConstIterator it = structType->members().begin();
	    it != structType->members().end();
	    it++)
	{
	    castAndCopyRec(lcontext, it->type, eIndex, dest, sizes, offsets);
	}
    }
    else if( ArrayTypePtr arrayType = dataType.cast<ArrayType>())
    {
	for (int i = 0; i < arrayType->size(); ++i)
	{
	    castAndCopyRec(lcontext, arrayType->elementType(), eIndex, 
			   dest, sizes, offsets);
	}
    }
    else
    {
	assert(eIndex < (int)elements.size());
	LiteralNodePtr literal = elements[eIndex];
	literal = dataType->castValue(lcontext, literal);
	memcpy(dest+offsets[eIndex], literal->valuePtr(), sizes[eIndex]);
	eIndex++;
    }
}


} // namespace Ctl
