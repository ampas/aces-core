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
//	The syntax tree for the color transformation language.
//
//-----------------------------------------------------------------------------

#include <CtlSyntaxTree.h>
#include <CtlSymbolTable.h>
#include <CtlMessage.h>
#include <CtlLContext.h>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <CtlErrors.h>

using namespace std;

namespace Ctl {


SyntaxNode::SyntaxNode (int lineNumber):
    lineNumber (lineNumber)
{
    // empty
}


SyntaxNode::~SyntaxNode ()
{
    // empty
}


void
SyntaxNode::printTree () const
{
    print (0);
}


ModuleNode::ModuleNode
    (int lineNumber,
     const StatementNodePtr &constants,
     const FunctionNodePtr &functions)
:
    SyntaxNode (lineNumber),
    constants (constants),
    functions (functions)
{
    // empty
}


void
ModuleNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " module " << endl;

    if (constants)
	constants->print (indent + 1);

    if (functions)
	functions->print (indent + 1);
}


FunctionNode::FunctionNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info,
     const StatementNodePtr &body)
:
    SyntaxNode (lineNumber),
    name (name),
    info (info),
    body (body),
    next (0)
{
    // empty
}


void
FunctionNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " function " << name << endl;

    if (info)
	info->print (indent + 1);

    if (body)
	body->print (indent + 1);

    if (next)
	next->print (indent);
}


StatementNode::StatementNode (int lineNumber):
    SyntaxNode (lineNumber),
    next (0)
{
    // empty
}


LinearStatementNode::LinearStatementNode (int lineNumber):
    StatementNode (lineNumber)
{
    // empty
}


bool
LinearStatementNode::pathEndsWithReturn () const
{
    return next && next->pathEndsWithReturn();
}


VariableNode::VariableNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info,
     const ExprNodePtr &initialValue,
     bool assignInitialValue)
:
    LinearStatementNode (lineNumber),
    name (name),
    info (info),
    initialValue (initialValue),
    assignInitialValue (assignInitialValue)
{
    // empty
}


void
VariableNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " variable " << name << endl;

    if (info)
	info->print (indent + 1);

    if (initialValue && !(info && info->value()))
    {
	cout << setw (indent + 1) << "" << "initial value" << endl;
	initialValue->print (indent + 2);
	cout << setw (indent + 1) << "" << "assign initial value" << endl;
	cout << setw (indent + 2) << "" << assignInitialValue << endl;
    }

    if (next)
	next->print (indent);
}


AssignmentNode::AssignmentNode
    (int lineNumber,
     const ExprNodePtr &lhs,
     const ExprNodePtr &rhs)
:
    LinearStatementNode (lineNumber),
    lhs (lhs),
    rhs (rhs)
{
    // empty
}


void
AssignmentNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " assignment" << endl;

    if (lhs)
	lhs->print (indent + 1);

    if (rhs)
	rhs->print (indent + 1);

    if (next)
	next->print (indent);
}


ExprStatementNode::ExprStatementNode
    (int lineNumber,
     const ExprNodePtr &expr)
:
    LinearStatementNode (lineNumber),
    expr (expr)
{
    // empty
}


void
ExprStatementNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " expr statement" << endl;

    if (expr)
	expr->print (indent + 1);

    if (next)
	next->print (indent);
}


IfNode::IfNode
    (int lineNumber,
     const ExprNodePtr &condition,
     const StatementNodePtr &truePath,
     const StatementNodePtr &falsePath)
:
    StatementNode (lineNumber),
    condition (condition),
    truePath (truePath),
    falsePath (falsePath)
{
    // empty
}


void
IfNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " if" << endl;

    if (condition)
    {
	cout << setw (indent + 1) << "" << "condition" << endl;
	condition->print (indent + 2);
    }

    if (truePath)
    {
	cout << setw (indent + 1) << "" << "true path" << endl;
	truePath->print (indent + 2);
    }

    if (falsePath)
    {
	cout << setw (indent + 1) << "" << "false path" << endl;
	falsePath->print (indent + 2);
    }

    if (next)
	next->print (indent);
}


bool
IfNode::pathEndsWithReturn () const
{
    return (truePath  && truePath->pathEndsWithReturn() &&
	    falsePath && falsePath->pathEndsWithReturn()) ||
	   (next && next->pathEndsWithReturn());
}


ReturnNode::ReturnNode
    (int lineNumber,
     const SymbolInfoPtr &info,
     const ExprNodePtr &returnedValue)
:
    StatementNode (lineNumber),
    info (info),
    returnedValue (returnedValue)
{
    // empty
}


void
ReturnNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " return" << endl;

    if (info)
	info->print (indent + 1);

    if (returnedValue)
	returnedValue->print (indent + 1);

    if (next)
	next->print (indent);
}


bool
ReturnNode::pathEndsWithReturn () const
{
    return true;
}


WhileNode::WhileNode
    (int lineNumber,
     const ExprNodePtr &condition,
     const StatementNodePtr &loopBody)
:
    StatementNode (lineNumber),
    condition (condition),
    loopBody (loopBody)
{
    // empty
}


void
WhileNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " while" << endl;

    if (condition)
    {
	cout << setw (indent + 1) << "" << "condition" << endl;
	condition->print (indent + 2);
    }

    if (loopBody)
    {
	cout << setw (indent + 1) << "" << "loop body" << endl;
	loopBody->print (indent + 2);
    }

    if (next)
	next->print (indent);
}


bool
WhileNode::pathEndsWithReturn () const
{
    BoolLiteralNodePtr constCondition = condition.cast<BoolLiteralNode>();

    if (constCondition && constCondition->value)
    {
	//
	// The loop condition has already been evaluated, and it is true.
	// The only way to get out of the loop is via a return statement.
	// Code after the loop is unreachable.
	//

	return true;
    }
    else
    {
	//
	// The loop condition must be evaluated at run time.  Code after
	// the loop is reachable unless all paths through the loop body
	// end with a return statement.
	//

	return (loopBody && loopBody->pathEndsWithReturn()) ||
	       (next && next->pathEndsWithReturn());
    }
}


ExprNode::ExprNode (int lineNumber):
    SyntaxNode (lineNumber),
    type (0)
{
    // empty
}


BinaryOpNode::BinaryOpNode
    (int lineNumber,
     Token op,
     const ExprNodePtr &leftOperand,
     const ExprNodePtr &rightOperand)
:
    ExprNode (lineNumber),
    op (op),
    leftOperand (leftOperand),
    rightOperand (rightOperand),
    operandType (0)
{
    // empty
}


void
BinaryOpNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " binary operator " <<
	    tokenAsString (op) << endl;

    if (type)
	type->print (indent + 1);
    else
	cout << setw (indent + 1) << "" << "*** type unknown ***" << endl;

    if (leftOperand)
	leftOperand->print (indent + 1);

    if (rightOperand)
	rightOperand->print (indent + 1);
}


void
BinaryOpNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    if (!leftOperand || !rightOperand)
	return;

    leftOperand->computeType (lcontext, initInfo);
    rightOperand->computeType (lcontext, initInfo);

    if (!leftOperand->type || !rightOperand->type)
	return;

    ArrayTypePtr arrayType = leftOperand->type.cast<ArrayType>();
    StructTypePtr structType = leftOperand->type.cast<StructType>();

    if( arrayType || structType )
    {
	MESSAGE_LE (lcontext, ERR_OP_TYPE, lineNumber,
	    "Invalid operand types for " << tokenAsString (op) << " "
	    "operator (" << leftOperand->type->asString() << " " <<
	    tokenAsString (op) << " " <<
	    rightOperand->type->asString() << ").");
    }

    if (op == TK_AND || op == TK_OR)
    {
	//
	// It must be possible to cast the operands to type bool,
	// and the operator produces a result of type bool.
	//

	BoolTypePtr boolType = lcontext.newBoolType ();

	if (boolType->canCastFrom (leftOperand->type) &&
	    boolType->canCastFrom (rightOperand->type))
	{
	    operandType = boolType;
	    type = boolType;
	    return;
	}
    }
    else if (op == TK_EQUAL || op == TK_GREATER || op == TK_GREATEREQUAL ||
	     op == TK_LESS || op == TK_LESSEQUAL || op == TK_NOTEQUAL)
    {
	//
	// It must be possible to promote the operands to a common type.
	// The operator produces a result of type bool.
	//

	BoolTypePtr boolType = lcontext.newBoolType ();

	if (leftOperand->type->isSameTypeAs (rightOperand->type))
	{
	    operandType = leftOperand->type;
	    type = boolType;
	    return;
	}
	else if (leftOperand->type->canPromoteFrom (rightOperand->type))
	{
	    operandType = leftOperand->type;
	    type = boolType;
	    return;
	}
	else if (rightOperand->type->canPromoteFrom (leftOperand->type))
	{
	    operandType = rightOperand->type;
	    type = boolType;
	    return;
	}
    }
    else
    {
	//
	// It must be possible to promote the operands to a common type.
	// The operator produces a result of this common type.
	//

	if (leftOperand->type->isSameTypeAs (rightOperand->type))
	{
	    operandType = leftOperand->type;
	    type = operandType;
	    return;
	}
	else if (leftOperand->type->canPromoteFrom (rightOperand->type))
	{
	    operandType = leftOperand->type;
	    type = operandType;
	    return;
	}
	else if (rightOperand->type->canPromoteFrom (leftOperand->type))
	{
	    operandType = rightOperand->type;
	    type = operandType;
	    return;
	}
    }

    MESSAGE_LE (lcontext, ERR_OP_TYPE, lineNumber,
	"Invalid operand types for " << tokenAsString (op) << " "
	"operator (" << leftOperand->type->asString() << " " <<
	tokenAsString (op) << " " <<
	rightOperand->type->asString() << ").");
}


ExprNodePtr
BinaryOpNode::evaluate (LContext &lcontext)
{
    leftOperand = leftOperand->evaluate (lcontext);
    rightOperand = rightOperand->evaluate (lcontext);

    if (!type || !operandType)
	return this;

    if (!operandType->isSameTypeAs (leftOperand->type))
	leftOperand = operandType->castValue (lcontext, leftOperand);

    if (!operandType->isSameTypeAs (rightOperand->type))
	rightOperand = operandType->castValue (lcontext, rightOperand);

    return operandType->evaluate (lcontext, this);
}


bool
BinaryOpNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return false;
}


UnaryOpNode::UnaryOpNode
    (int lineNumber,
     Token op,
     const ExprNodePtr &operand)
:
    ExprNode (lineNumber),
    op (op),
    operand (operand)
{
    // empty
}


void
UnaryOpNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " unary operator " <<
	    tokenAsString (op) << endl;

    if (type)
	type->print (indent + 1);
    else
	cout << setw (indent + 1) << "" << "*** type unknown ***" << endl;

    if (operand)
	operand->print (indent + 1);
}


void
UnaryOpNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    if (!operand)
	return;

    operand->computeType (lcontext, initInfo);

    if (!operand->type)
	return;

    if (op == TK_BITNOT)
    {
	//
	// Operator ~ can only be applied to operands of type bool int, or
	// unsigned int.  The operator produces a result of the same type as
	// the operand.
	//

	BoolTypePtr boolType = lcontext.newBoolType ();

	if (boolType->isSameTypeAs (operand->type))
	{
	    type = boolType;
	    return;
	}

	IntTypePtr intType = lcontext.newIntType ();

	if (intType->isSameTypeAs (operand->type))
	{
	    type = intType;
	    return;
	}

	UIntTypePtr uIntType = lcontext.newUIntType ();

	if (uIntType->isSameTypeAs (operand->type))
	{
	    type = uIntType;
	    return;
	}
    }
    else if (op == TK_MINUS)
    {
	//
	// Operator - can only be applied to operands of type int, unsigned
	// int, half or float.  The operator produces a result of the same
	// type as the operand, except for unsigned int where it produces a
	// result of type int.
	//

	IntTypePtr intType = lcontext.newIntType ();

	if (intType->isSameTypeAs (operand->type))
	{
	    type = intType;
	    return;
	}

	UIntTypePtr uIntType = lcontext.newUIntType ();

	if (uIntType->isSameTypeAs (operand->type))
	{
	    type = intType;  // not uIntType!
	    return;
	}

	HalfTypePtr halfType = lcontext.newHalfType ();

	if (halfType->isSameTypeAs (operand->type))
	{
	    type = halfType;
	    return;
	}

	FloatTypePtr floatType = lcontext.newFloatType ();

	if (floatType->isSameTypeAs (operand->type))
	{
	    type = floatType;
	    return;
	}
    }
    else if (op == TK_NOT)
    {
	//
	// Operator ! can be applied to operands that can be converted
	// to type bool.  The operator produces a result of type bool.
	//

	BoolTypePtr boolType = lcontext.newBoolType ();

	if (boolType->canCastFrom (operand->type))
	{
	    type = boolType;
	    return;
	}
    }

    MESSAGE_LE (lcontext, ERR_OP_TYPE, lineNumber,
	"Invalid operand type for " << tokenAsString (op) << " operator "
	"(" << tokenAsString (op) << operand->type->asString() << ").");
}


ExprNodePtr
UnaryOpNode::evaluate (LContext &lcontext)
{
    operand = operand->evaluate (lcontext);

    if (!type)
	return this;

    if (!type->isSameTypeAs (operand->type))
	operand = type->castValue (lcontext, operand);

    return type->evaluate (lcontext, this);
}


bool
UnaryOpNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return false;
}


MemberNode::MemberNode
    (int lineNumber,
     const ExprNodePtr &obj,
     const std::string &member)
:
    ExprNode (lineNumber),
    obj (obj),
    member (member),
    offset (0)
{
    // empty
}


void
MemberNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " access of member " 
	 << member << endl;

    if (type)
	type->print (indent + 1);
    else
	cout << setw (indent + 1) << "" << "*** type unknown ***" << endl;

    if (obj)
	obj->print (indent + 1);

}


void
MemberNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    obj->computeType (lcontext, initInfo);

    if (!obj->type)
    {
	type = lcontext.newIntType();
	return;
    }

    StructTypePtr structType = obj->type.cast<StructType>();

    if (!structType)
    {
	MESSAGE_LE (lcontext, ERR_NON_STRUCT, lineNumber,
		    "Applied member access operator to non-struct "
		    "of type " << obj->type->asString() << ".");

	type = lcontext.newIntType();
	return;
    }

    //
    // check that the member exists and gets its type
    //

    for (MemberVectorConstIterator it = structType->members().begin(); 
 	 it != structType->members().end(); 
	 it++)
    {
	if (it->name == member)
	{
	    type = it->type;
	    offset = it->offset;
	    return;
	}
    }

    if (!type)
    {
	MESSAGE_LE (lcontext, ERR_MEMBER_ACCESS, lineNumber,
		    "Unable to find member \"" << member << "\".");

	type = lcontext.newIntType();
    }
}


ExprNodePtr
MemberNode::evaluate (LContext &lcontext)
{
    obj = obj->evaluate(lcontext);
    return this;
}


bool
MemberNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return obj && obj->isLvalue (initInfo);
}


SizeNode::SizeNode
    (int lineNumber,
     const ExprNodePtr &obj)
:
    ExprNode (lineNumber),
    obj (obj)
{
    // empty
}


void
SizeNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " array size of " 
	 << endl;

    if (obj)
	obj->print (indent + 1);
}


void
SizeNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    obj->computeType (lcontext, initInfo);

    ArrayTypePtr arrayType = obj->type.cast<ArrayType>();

    if( !arrayType)
    {
	string type = "unknown";
	if( obj && obj->type)
	    type = obj->type->asString();
	MESSAGE_LE (lcontext, ERR_NON_ARRAY, lineNumber,
		    "Applied size operator to non-array "
		    " of type " << type << ".");
    }
    type = lcontext.newIntType ();
}


ExprNodePtr
SizeNode::evaluate (LContext &lcontext)
{
    obj = obj->evaluate(lcontext);
    ArrayTypePtr arrayType = obj->type.cast<ArrayType>();
    if( !arrayType)
    {
	return lcontext.newIntLiteralNode (lineNumber, 1);
    }
    else if( arrayType->size() != 0 )
    {
	return lcontext.newIntLiteralNode (lineNumber, arrayType->size());
    }
    return this;
}


bool
SizeNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return false;
}



ArrayIndexNode::ArrayIndexNode
    (int lineNumber,
     const ExprNodePtr &array,
     const ExprNodePtr &index)
:
    ExprNode (lineNumber),
    array (array),
    index (index)
{
    // empty
}


void
ArrayIndexNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " array index " << endl;

    if (type)
	type->print (indent + 1);
    else
	cout << setw (indent + 1) << "" << "*** type unknown ***" << endl;

    if (array)
	array->print (indent + 1);

    if (index)
	index->print (indent + 1);
}


void
ArrayIndexNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    if (!array || !index)
	return;

    array->computeType (lcontext, initInfo);
    index->computeType (lcontext, initInfo);

    if (!array->type || !index->type)
	return;

    ArrayTypePtr arrayType = array->type.cast<ArrayType>();

    if (!arrayType)
    {
	string name = "";
	if( NameNodePtr arrayName =  array.cast<NameNode>() )
	{
	    name = arrayName->name;
	    MESSAGE_LE (lcontext, ERR_NON_ARR_IND, array->lineNumber,
			"Applied [] operator to non-array (" << name << " "
			"is of type " << array->type->asString() << ").");
	}
	else
	{
	    MESSAGE_LE (lcontext, ERR_NON_ARR_IND, array->lineNumber,
			"Applied [] operator to non-array of type " 
			<< array->type->asString() << ".");
	}
	type = lcontext.newIntType();
	return;
    }

    IntTypePtr intType = lcontext.newIntType ();

    if (!intType->canPromoteFrom (index->type))
    {
	string name = "";
	if( NameNodePtr arrayName =  array.cast<NameNode>() )
	    name = arrayName->name;

	MESSAGE_LE (lcontext, ERR_ARR_IND_TYPE, array->lineNumber,
	    "Index into array " << name << " is not an iteger "
	    "(index is of type " << index->type->asString() << ").");

	type = lcontext.newIntType();
	return;
    }

    type = arrayType->elementType();
}


ExprNodePtr
ArrayIndexNode::evaluate (LContext &lcontext)
{
    IntTypePtr intType = lcontext.newIntType ();

    array = array->evaluate (lcontext);
    index = index->evaluate (lcontext);


    if( IntLiteralNodePtr literal = index.cast<IntLiteralNode>())
    {
	if(literal->value < 0)
	{
	    string name = "";
	    if( NameNodePtr arrayName =  array.cast<NameNode>() )
		name = arrayName->name;
	    
	    MESSAGE_LE (lcontext, ERR_ARR_IND_TYPE, array->lineNumber,
			"Index into array " << name << " is negative "
			"(" << literal->value << ").");
	}

	ArrayTypePtr arrayType = array->type.cast<ArrayType>();
	if(!arrayType)
	    return this;
	if( literal->value >= arrayType->size() && arrayType->size() != 0)
	{
	    string name = "";
	    if( NameNodePtr arrayName =  array.cast<NameNode>() )
		name = arrayName->name;
	    
	    MESSAGE_LE (lcontext, ERR_ARR_IND_TYPE, array->lineNumber,
			"Index into array " << name << " is out of range "
			"(index = " << literal->value << ", "
			"array size = " << arrayType->size() << ").");
	}


    }

    if (index->type && !intType->isSameTypeAs (index->type))
	index = intType->castValue (lcontext, index);

    return this;
}


bool
ArrayIndexNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return array && array->isLvalue (initInfo);
}


NameNode::NameNode
    (int lineNumber,
     const std::string &name,
     const SymbolInfoPtr &info)
:
     ExprNode (lineNumber),
     name (name),
     info (info)
{
    // empty
}


void
NameNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " name " << name << endl;

    if (info)
	info->print (indent + 1);
    else
	cout << setw (indent + 1) << "" << "*** no info ***" << endl;
}


void
NameNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    if (info)
	type = info->type();
}


ExprNodePtr
NameNode::evaluate (LContext &lcontext)
{
    if (info && info->isData() && info->value())
    {
	ValueNodePtr valueNode = info->value().cast<ValueNode>();
	if(valueNode)
	    info->value()->evaluate (lcontext);
	else
	    return info->value()->evaluate (lcontext);
    }
    return this;
}


bool
NameNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    if (!type)
	return false;

    if (type.cast<VoidType>())
	return false;

    DataTypePtr dataType = type.cast<DataType>();

    if (!dataType)
	return false;

    if (initInfo && initInfo.pointer() == info.pointer())
	return true;
    else
	return info->isWritable();
}


LiteralNode::LiteralNode (int lineNumber):
    ExprNode (lineNumber)
{
    // empty
}


void
LiteralNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    // empty
}


ExprNodePtr
LiteralNode::evaluate (LContext &lcontext)
{
    return this;
}


bool
LiteralNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return false;
}


BoolLiteralNode::BoolLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     bool value)
:
    LiteralNode (lineNumber),
    value (value)
{
    type = lcontext.newBoolType ();
}


void
BoolLiteralNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " bool " << value << endl;
}

void
BoolLiteralNode::printLiteral () const
{
    cout << value;
}


IntLiteralNode::IntLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     int value)
:
    LiteralNode (lineNumber),
    value (value)
{
    type = lcontext.newIntType ();
}


void
IntLiteralNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " int " << value << endl;
}
void
IntLiteralNode::printLiteral () const
{
    cout << value;
}


UIntLiteralNode::UIntLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     unsigned int value)
:
    LiteralNode (lineNumber),
    value (value)
{
    type = lcontext.newUIntType ();
}


void
UIntLiteralNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber <<
	    " unsigned int " << value << endl;
}
void
UIntLiteralNode::printLiteral () const
{
    cout << value;
}


HalfLiteralNode::HalfLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     half value)
:
    LiteralNode (lineNumber),
    value (value)
{
    type = lcontext.newHalfType ();
}


void
HalfLiteralNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " half " << value << endl;
}
void
HalfLiteralNode::printLiteral () const
{
    cout << value;
}


FloatLiteralNode::FloatLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     float value)
:
    LiteralNode (lineNumber),
    value (value)
{
    type = lcontext.newFloatType ();
}


void
FloatLiteralNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " float " << value << endl;
}
void
FloatLiteralNode::printLiteral () const
{
    cout << value;
}


StringLiteralNode::StringLiteralNode
    (int lineNumber,
     const LContext &lcontext,
     const string &value)
:
    LiteralNode (lineNumber),
    value (value)
{
    type = lcontext.newStringType ();
}


void
StringLiteralNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber <<
	    " float \"" << value << "\"" << endl;
}
void
StringLiteralNode::printLiteral () const
{
    cout << value;
}


CallNode::CallNode
    (int lineNumber,
     const NameNodePtr &function,
     const ExprNodeVector &arguments)
:
    ExprNode (lineNumber),
    function (function),
    arguments (arguments)
{
    // empty
}


void
CallNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " function call" << endl;
    
    if (function)
	function->print (indent + 1);

    for (int i = 0; i < (int)arguments.size(); ++i)
	arguments[i]->print (indent + 1);
}


void
CallNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    //
    // Compute the type of a function call.  This is the same type
    // as the function's return type, provided the call is valid.
    //

    //
    // Verify that what we are trying to call is actually a function.
    //

    if (!function)
	return;

    function->computeType (lcontext, initInfo);

    if (!function->type)
	return;

    FunctionTypePtr functionType = function->type.cast <FunctionType>();

    if (!functionType)
    {
	MESSAGE_LE (lcontext, ERR_NON_FUNC, function->lineNumber,
	    "Invalid function call to call non-function "
	    "(" << function->name << " is of type " <<
	    function->type->asString() << ").");

	return;
    }

    //
    // We shouldn't have more function call arguments than parameters.
    //

    const ParamVector &parameters = functionType->parameters();

    if (arguments.size() > parameters.size())
    {
	MESSAGE_LE (lcontext, ERR_FUNC_ARG_NUM, function->lineNumber,
	    "Too many arguments in call to function " << function->name << ".");

	return;
    }

    //
    // Compare the types of the arguments to the call with
    // the types of the function's parameters.
    //

    for (int i = 0; i < (int)parameters.size(); ++i)
    {
	if (i < (int)arguments.size())
	{
	    //
	    // We have a function call argument for parameter i.
	    //

	    arguments[i]->computeType (lcontext, initInfo);

	    TypePtr argType = arguments[i]->type;

	    if (!argType)
		return;

	    DataTypePtr paramType = parameters[i].type;

	    if (parameters[i].isWritable())
	    {
		//
		// output parameter -- argument must be an lvalue
		// of the same type as the parameter
		//

		if (!arguments[i]->isLvalue (initInfo))
		{
		    MESSAGE_LE (lcontext, ERR_FUNC_ARG_LVAL, arguments[i]->lineNumber,
			"Argument " << i+1 << " in call to function " <<
			function->name << " corresponds to an output "
			"parameter but it is not an lvalue.");

		    return;
		}

		if (!paramType->isSameTypeAs (argType))
		{
		    MESSAGE_LE (lcontext,
			ERR_FUNC_ARG_TYPE, arguments[i]->lineNumber,
			"Type of argument " << i+1 << " in call to "
			"function " << function->name << " is not the "
			"same as the type of the function's parameter "
			"(" << argType->asString() << " value "
			"for " << paramType->asString() << " parameter.)");
		    return;
		}
	    }
	    else
	    {
		//
		// input parameter -- it must be possible to cast
		// the argument type to the parameter type
		//

		if (!paramType->canCastFrom (argType))
		{
		    MESSAGE_LE (lcontext, ERR_FUNC_ARG_TYPE,
			arguments[i]->lineNumber,
			"Cannot convert the type of argument " << i+1 << " "
			"in call to function " << function->name << " "
			"to the type of the function's parameter "
			"(" << argType->asString() << " value "
			"for " << paramType->asString() << " parameter.)");
		    return;
		}
	    }
	}
	else
	{
	    //
	    // We have no function call argument for parameter i.
	    // The parameter must have a default value.
	    //

	    if (!parameters[i].defaultValue)
	    {
		MESSAGE_LE (lcontext, ERR_FUNC_ARG_NUM,
		    function->lineNumber,
		    "Not enough arguments in call to "
		    "function " << function->name << ".");
		return;
	    }
	}
    }

    //
    // If we get here, then the call is valid.
    //

    type = functionType->returnType();
}


ExprNodePtr
CallNode::evaluate (LContext &lcontext)
{
    for (int i = 0; i < (int)arguments.size(); ++i)
	arguments[i] = arguments[i]->evaluate (lcontext);

    return this;
}


bool
CallNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return false;
}


ValueNode::ValueNode
    (int lineNumber,
     const ExprNodeVector &elements)
:
    ExprNode (lineNumber),
    elements (elements)
{
    // empty
}


void
ValueNode::print (int indent) const
{
    cout << setw (indent) << "" << lineNumber << " value initializer" << endl;

    if(elements.size() <= 20)
    {
	for (int i = 0; i < (int)elements.size(); ++i)
	    elements[i]->print (indent + 1);
    }
    else
    {
	cout << setw (indent) << "[";
	for (int i = 0; i < (int)elements.size(); ++i)
	{
	    LiteralNodePtr literal = elements[i].cast<LiteralNode>();
	    if(literal)
	    {
		literal->printLiteral();

		if( i == (int)elements.size() -1)
		    cout << "]\n";
		else
		{
		    cout << ", ";

		    if( i >= 199 && elements.size() - i > 10)
		    {
			cout << "\n";
			cout  << setw (indent) << "" << "...< " 
			      << elements.size() - i - 11
			      << " elements omitted > \n";
			cout  << setw (indent) << "";
			i = elements.size() - 11;
		    }
		    else if( (i+1)%10 == 0)
		    {
			cout << "\n";
			cout << setw(indent) << "";
		    }
		}
	    }
	    else
		elements[i]->print (indent + 1);

	}
    }

}


void
ValueNode::computeType (LContext &lcontext, const SymbolInfoPtr &initInfo)
{
    for (int i = 0; i < (int)elements.size(); ++i)
    {
	elements[i]->computeType (lcontext, initInfo);

	if (!elements[i]->type)
	    return;
    }
}


ExprNodePtr
ValueNode::evaluate (LContext &lcontext)
{
    for (int i = 0; i < (int)elements.size(); ++i)
	elements[i] = elements[i]->evaluate (lcontext);

    return this;
}


bool
ValueNode::checkElementTypes (const DataTypePtr &dataType,
			      LContext &lcontext) const
{
    int eIndex = 0;
    return checkElementTypesRec(dataType, lcontext, eIndex);
}

bool
ValueNode::checkElementTypesRec (const DataTypePtr &dataType,
				 LContext &lcontext,
				 int &eIndex) const
{
    //
    //  recursively iterate through nested structs and arrays
    //  in this data type, checking element types along the way
    //

    assert(eIndex < (int)elements.size());
    if( StructTypePtr structType = dataType.cast<StructType>() )
    {
	for(MemberVectorConstIterator it = structType->members().begin();
	    it != structType->members().end();
	    it++)
	{
	    if(!checkElementTypesRec(it->type, lcontext, eIndex))
		return false;
	}
    }
    else if( ArrayTypePtr arrayType = dataType.cast<ArrayType>() )
    {
	for (int i = 0; i < arrayType->size(); ++i)
	{
	    if( !checkElementTypesRec(arrayType->elementType(), lcontext, eIndex) )
		return false;
	}
    }
    else if (!dataType->canCastFrom (elements[eIndex++]->type))
    {
	string fromType = "unknown";
	if( elements[eIndex-1]->type)
	    fromType = elements[eIndex-1]->type->asString();
	MESSAGE_LE (lcontext, ERR_ARR_EL_TYPE, 
		    elements[eIndex-1]->lineNumber,
		    "Cannot convert the type of value element " 
		    << eIndex-1 << " "
		    "(" << fromType << ") "
		    "to type " << dataType->asString() << ".");
	return false;
    }
    return true;
}


bool	
ValueNode::elementsAreLiterals () const
{
    for (int i = 0; i < (int)elements.size(); ++i)
	if (!elements[i].cast<LiteralNode>())
	    return false;

    return true;
}


bool
ValueNode::isLvalue (const SymbolInfoPtr &initInfo) const
{
    return false;
}

} // namespace Ctl
