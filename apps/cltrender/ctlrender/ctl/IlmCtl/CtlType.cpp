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
//	Types in the color transformation language
//
//-----------------------------------------------------------------------------

#include <CtlType.h>
#include <CtlSyntaxTree.h>
#include <CtlMessage.h>
#include <CtlLContext.h>
#include <CtlAlign.h>
#include <CtlExc.h>
#include <Iex.h>
#include <typeinfo>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <stdint.h>
#include <stdlib.h>

using namespace std;

namespace Ctl {


Type::~Type ()
{
    // empty
}

void Type::childElement(size_t *offset, TypePtr *type, const std::string &path,
                        ...) {
	va_list ap;

	va_start(ap, path);
	childElementV(offset, type, path, ap);
	va_end(ap);
}

void Type::childElementV(size_t *offset, TypePtr *type, const std::string &path,
                         va_list ap) {
	std::string segment;
	std::string remainder;
	std::string::size_type start;
	ArrayTypePtr array_type;
	StructTypePtr struct_type;
	SizeVector sizes;	
	SizeVector *offsets;
	uint32_t u, count;

	remainder=path;
	start=remainder.find_first_not_of("/");
	if(start!=(std::string::size_type)-1 && start!=0) {
		remainder=std::string(remainder.begin()+start, remainder.end());
	}

	segment=remainder;
	start=remainder.find_first_of("/");
	if(start!=(std::string::size_type)-1) {
		segment=std::string(remainder.begin(), remainder.begin()+start-1);
		remainder=std::string(remainder.begin()+start, remainder.end());
	} else {
		remainder="";
	}

	if(segment.size()==0 || segment==".") {
		return;
	}

	if((*type)->cDataType()==ArrayTypeEnum) {
		array_type=*type;
		array_type->sizes(sizes);
		if(segment=="%s") {
			segment=std::string(va_arg(ap, char *));
			u=strtoul(segment.c_str(), NULL, 0);
		} else if(segment=="%v") {
			offsets=((SizeVector *)va_arg(ap, SizeVector *));
			while(offsets->size()!=0) {
				if(sizes.size()==0) {
					throw(DatatypeExc("too many dimensions specified for matrix (or array) vector element (at least %d too many)", offsets->size()));
				}
				if(offset[0]>=sizes[0]) {
					throw(DatatypeExc("out of range matrix (or array) vector element (%u>=%u)", offset[0], sizes[0]));
				}
				*offset=*offset+array_type->elementType()->objectSize()*((*offsets)[0]);
				*type=array_type->elementType();
				offsets->erase(offsets->begin());
				sizes.erase(sizes.begin());
			}
			childElementV(offset, type, remainder, ap);
		} else if(segment!="%d" && segment!="%u") {
			u=strtoul(segment.c_str(), NULL, 0);
		} else { // segment=="%d" || segment=="%u"
			u=va_arg(ap, uint32_t);
		}

		if(u>=sizes[0]) {
			throw(DatatypeExc("out of range matrix (or array) element specification (%u>=%u)", u, sizes[0]));
		}

		*offset=*offset+u*array_type->elementType()->objectSize();
		*type=array_type->elementType();
		childElementV(offset, type, remainder, ap);
	} else if((*type)->cDataType()==StructTypeEnum) {
		struct_type=*type;

		if(segment=="%s") {
			segment=va_arg(ap, char *);
		} else if(segment=="%v" || segment=="%d" || segment=="%u") {
			throw(DatatypeExc("The '%s' replacement may not be used when finding the child of a structure", segment.c_str()));
		}

		count=struct_type->members().size();
		for(u=0; u<count; u++) {
			if(struct_type->members()[u].name==segment) {
				*offset=*offset+u*struct_type->objectSize();
				*type=struct_type->members()[u].type;
				childElementV(offset, type, remainder, ap);
				return;
			}
		}
		throw(DatatypeExc("The structure type '%s' does not have an element named '%s'", (*type)->asString().c_str(), segment.c_str()));
	} else {
		throw(DatatypeExc("The type '%s' does not have any children", (*type)->asString().c_str()));
	}
}

char *		
DataType::alignObjectAddr (char *addr) const
{
    return (char *) align ((size_t) addr, objectAlignment());
}

void
DataType::coreSizes(size_t parentOffset,
		    SizeVector &sizes, 
		    SizeVector &offsets) const
{
    sizes.push_back(objectSize());
    offsets.push_back( parentOffset );
}

VoidType::VoidType (): DataType ()
{
    // empty
}


bool
VoidType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
VoidType::canPromoteFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}

bool
VoidType::canAssign (const TypePtr &t) const
{
    return false;
}

bool
VoidType::canCastFrom (const TypePtr &t) const
{
    return true;
}


ExprNodePtr
VoidType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


ExprNodePtr
VoidType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


void
VoidType::print (int indent) const
{
    cout << setw (indent) << "" << "void " << endl;
}


string
VoidType::asString () const
{
    return "void";
};


AddrPtr     
VoidType::newStaticVariable (Module *module) const
{
    // Cannot have a static void or array of voids variable
    assert(false);
    return AddrPtr();
}


void
VoidType::newAutomaticVariable (StatementNodePtr node, 
				LContext &lcontext) const
{
    // Cannot have a void or array of voids
    assert(false);
}


BoolType::BoolType () : DataType ()
{
    // empty
}


bool
BoolType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
BoolType::canPromoteFrom (const TypePtr &t) const
{
    return t.cast<BoolType>();
}


bool
BoolType::canCastFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>() ||
	   t.cast<FloatType>();
}


bool
BoolType::canAssign (const TypePtr &t) const
{
    return canCastFrom(t);
}

ExprNodePtr
BoolType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (UnaryOpNodePtr unOp = expr.cast<UnaryOpNode>())
    {
	BoolLiteralNodePtr x = unOp->operand.cast<BoolLiteralNode>();

	if (x)
	{
	    switch (unOp->op)
	    {
	      case TK_BITNOT:

		//
		// We use the C++ ! operation to evaluate the CTL expression ~x,
		// where x is of type bool.  This ensures that ~true == false
		// and ~false == true.
		//

		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !x->value);

	      case TK_NOT:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !x->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Cannot apply " << tokenAsString (unOp->op) << " "
		    "operator to value of type " << asString() << ".");
	    }
	}
    }

    if (BinaryOpNodePtr binOp = expr.cast<BinaryOpNode>())
    {
	BoolLiteralNodePtr x = binOp->leftOperand.cast<BoolLiteralNode>();
	BoolLiteralNodePtr y = binOp->rightOperand.cast<BoolLiteralNode>();

	if (x && y)
	{
	    switch (binOp->op)
	    {
	      case TK_AND:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value && y->value);

	      case TK_BITAND:

		//
		// For the bit-wise &, | and ^ operators, we normalize bool
		// operands before applying the operators.  This avoids
		// surprises, for example, true^true == true.
		// 

		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !!x->value & !!y->value);

	      case TK_BITOR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !!x->value | !!y->value);

	      case TK_BITXOR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !!x->value ^ !!y->value);

	      case TK_EQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value == y->value);

	      case TK_GREATER:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value > y->value);

	      case TK_GREATEREQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value >= y->value);

	      case TK_LESS:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value < y->value);

	      case TK_LESSEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value <= y->value);

	      case TK_NOTEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value != y->value);

	      case TK_OR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value || y->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Invalid operand types "
		    "for " << tokenAsString (binOp->op) << " operator "
		    "(" << binOp->leftOperand->type->asString() << " " <<
		    tokenAsString (binOp->op) << " " <<
		    binOp->rightOperand->type->asString() << ").");
	    }
	}
    }

    return expr;
}


ExprNodePtr
BoolType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (IntLiteralNodePtr x = expr.cast<IntLiteralNode>())
	return lcontext.newBoolLiteralNode (x->lineNumber, (bool) x->value);

    if (UIntLiteralNodePtr x = expr.cast<UIntLiteralNode>())
	return lcontext.newBoolLiteralNode (x->lineNumber, (bool) x->value);

    if (HalfLiteralNodePtr x = expr.cast<HalfLiteralNode>())
	return lcontext.newBoolLiteralNode (x->lineNumber, (bool) x->value);

    if (FloatLiteralNodePtr x = expr.cast<FloatLiteralNode>())
	return lcontext.newBoolLiteralNode (x->lineNumber, (bool) x->value);

    return expr;
}


void
BoolType::print (int indent) const
{
    cout << setw (indent) << "" << "bool " << endl;
}


string
BoolType::asString () const
{
    return "bool";
};


IntType::IntType () : DataType ()
{
    // empty
}


bool
IntType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
IntType::canPromoteFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<UIntType>() ||
	   t.cast<IntType>();
}


bool
IntType::canCastFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>() ||
	   t.cast<FloatType>();
}


bool
IntType::canAssign (const TypePtr &t) const
{
    return canCastFrom(t);
}

ExprNodePtr
IntType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (UnaryOpNodePtr unOp = expr.cast<UnaryOpNode>())
    {
	IntLiteralNodePtr x = unOp->operand.cast<IntLiteralNode>();

	if (x)
	{
	    switch (unOp->op)
	    {
	      case TK_BITNOT:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, ~x->value);

	      case TK_MINUS:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, -x->value);

	      case TK_NOT:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !x->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Cannot apply " << tokenAsString (unOp->op) << " "
		    "operator to value of type " << asString() << ".");
	    }
	}
    }

    if (BinaryOpNodePtr binOp = expr.cast<BinaryOpNode>())
    {
	IntLiteralNodePtr x =
	    evaluate(lcontext, binOp->leftOperand).cast<IntLiteralNode>();

	IntLiteralNodePtr y =
	    evaluate(lcontext, binOp->rightOperand).cast<IntLiteralNode>();

	if (x && y)
	{
	    switch (binOp->op)
	    {
	      case TK_AND:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value && y->value);

	      case TK_BITAND:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value & y->value);

	      case TK_BITOR:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value | y->value);

	      case TK_BITXOR:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value ^ y->value);

	      case TK_DIV:
		if (y->value != 0)
		{
		    return lcontext.newIntLiteralNode
				(expr->lineNumber, x->value / y->value);
		}
		else
		{
		    MESSAGE_LW (lcontext, ERR_DIV_ZERO, expr->lineNumber,
				"Warning: Division by zero "
				"(" << x->value << "/" << y->value << ").");

		    return lcontext.newIntLiteralNode (expr->lineNumber, 0);
		}

	      case TK_EQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value == y->value);

	      case TK_GREATER:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value > y->value);

	      case TK_GREATEREQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value >= y->value);

	      case TK_LEFTSHIFT:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value << y->value);

	      case TK_LESS:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value < y->value);

	      case TK_LESSEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value <= y->value);

	      case TK_MINUS:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value - y->value);

	      case TK_MOD:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value % y->value);

	      case TK_NOTEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value != y->value);

	      case TK_OR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value || y->value);

	      case TK_PLUS:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value + y->value);

	      case TK_RIGHTSHIFT:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value >> y->value);

	      case TK_TIMES:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, x->value * y->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Invalid operand types "
		    "for " << tokenAsString (binOp->op) << " operator "
		    "(" << binOp->leftOperand->type->asString() << " " <<
		    tokenAsString (binOp->op) << " " <<
		    binOp->rightOperand->type->asString() << ").");
	    }
	}
    }

    return expr;
}


ExprNodePtr
IntType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (BoolLiteralNodePtr x = expr.cast<BoolLiteralNode>())
	return lcontext.newIntLiteralNode (x->lineNumber, (int) x->value);

    if (UIntLiteralNodePtr x = expr.cast<UIntLiteralNode>())
	return lcontext.newIntLiteralNode (x->lineNumber, (int) x->value);

    if (HalfLiteralNodePtr x = expr.cast<HalfLiteralNode>())
	return lcontext.newIntLiteralNode (x->lineNumber, (int) x->value);

    if (FloatLiteralNodePtr x = expr.cast<FloatLiteralNode>())
	return lcontext.newIntLiteralNode (x->lineNumber, (int) x->value);

    return expr;
}


void
IntType::print (int indent) const
{
    cout << setw (indent) << "" << "int " <<  endl;
}


string
IntType::asString () const
{
    return "int";
};


UIntType::UIntType () : DataType ()
{
    // empty
}


bool
UIntType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
UIntType::canPromoteFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>();
}


bool
UIntType::canCastFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>() ||
	   t.cast<FloatType>();
}


bool
UIntType::canAssign (const TypePtr &t) const
{
    return canCastFrom(t);
}

ExprNodePtr
UIntType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (UnaryOpNodePtr unOp = expr.cast<UnaryOpNode>())
    {
	UIntLiteralNodePtr x = unOp->operand.cast<UIntLiteralNode>();

	if (x)
	{
	    switch (unOp->op)
	    {
	      case TK_BITNOT:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, ~x->value);

	      case TK_MINUS:
		return lcontext.newIntLiteralNode
			    (expr->lineNumber, -int (x->value));

	      case TK_NOT:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !x->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Cannot apply " << tokenAsString (unOp->op) << " "
		    "operator to value of type " << asString() << ".");
	    }
	}
    }

    if (BinaryOpNodePtr binOp = expr.cast<BinaryOpNode>())
    {
	UIntLiteralNodePtr x =
	    evaluate(lcontext, binOp->leftOperand).cast<UIntLiteralNode>();

	UIntLiteralNodePtr y =
	    evaluate(lcontext, binOp->rightOperand).cast<UIntLiteralNode>();

	if (x && y)
	{
	    switch (binOp->op)
	    {
	      case TK_AND:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value && y->value);

	      case TK_BITAND:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value & y->value);

	      case TK_BITOR:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value | y->value);

	      case TK_BITXOR:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value ^ y->value);

	      case TK_DIV:
		if (y->value != 0)
		{
		    return lcontext.newUIntLiteralNode
				(expr->lineNumber, x->value / y->value);
		}
		else
		{
		    MESSAGE_LW (lcontext, ERR_DIV_ZERO, expr->lineNumber,
				"Warning: Division by zero "
				"(" << x->value << "/" << y->value << ").");

		    return lcontext.newUIntLiteralNode (expr->lineNumber, 0);
		}

	      case TK_EQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value == y->value);

	      case TK_GREATER:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value > y->value);

	      case TK_GREATEREQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value >= y->value);

	      case TK_LEFTSHIFT:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value << y->value);

	      case TK_LESS:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value < y->value);

	      case TK_LESSEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value <= y->value);

	      case TK_MINUS:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value - y->value);

	      case TK_MOD:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value % y->value);

	      case TK_NOTEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value != y->value);

	      case TK_OR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value || y->value);

	      case TK_PLUS:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value + y->value);

	      case TK_RIGHTSHIFT:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value >> y->value);

	      case TK_TIMES:
		return lcontext.newUIntLiteralNode
			    (expr->lineNumber, x->value * y->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Invalid operand types "
		    "for " << tokenAsString (binOp->op) << " operator "
		    "(" << binOp->leftOperand->type->asString() << " " <<
		    tokenAsString (binOp->op) << " " <<
		    binOp->rightOperand->type->asString() << ").");
	    }
	}
    }

    return expr;
}


ExprNodePtr
UIntType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (BoolLiteralNodePtr x = expr.cast<BoolLiteralNode>())
	return lcontext.newUIntLiteralNode (x->lineNumber, (unsigned) x->value);

    if (IntLiteralNodePtr x = expr.cast<IntLiteralNode>())
	return lcontext.newUIntLiteralNode (x->lineNumber, (unsigned) x->value);

    if (HalfLiteralNodePtr x = expr.cast<HalfLiteralNode>())
	return lcontext.newUIntLiteralNode (x->lineNumber, (unsigned) x->value);

    if (FloatLiteralNodePtr x = expr.cast<FloatLiteralNode>())
	return lcontext.newUIntLiteralNode (x->lineNumber, (unsigned) x->value);

    return expr;
}


void
UIntType::print (int indent) const
{
    cout << setw (indent) << "" << "unsigned int " <<  endl;
}


string
UIntType::asString () const
{
    return "unsigned int";
};


HalfType::HalfType (): DataType ()
{
    // empty
}


bool
HalfType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
HalfType::canPromoteFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>();
}


bool
HalfType::canCastFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>() ||
	   t.cast<FloatType>();
}


bool
HalfType::canAssign (const TypePtr &t) const
{
    return canCastFrom(t);
}

ExprNodePtr
HalfType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (UnaryOpNodePtr unOp = expr.cast<UnaryOpNode>())
    {
	HalfLiteralNodePtr x = unOp->operand.cast<HalfLiteralNode>();

	if (x)
	{
	    switch (unOp->op)
	    {
	      case TK_MINUS:
		return lcontext.newHalfLiteralNode
			    (expr->lineNumber, -x->value);

	      case TK_NOT:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !x->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Cannot apply " << tokenAsString (unOp->op) << " "
		    "operator to value of type " << asString() << ".");
	    }
	}
    }

    if (BinaryOpNodePtr binOp = expr.cast<BinaryOpNode>())
    {
	HalfLiteralNodePtr x =
	    evaluate(lcontext, binOp->leftOperand).cast<HalfLiteralNode>();

	HalfLiteralNodePtr y =
	    evaluate(lcontext, binOp->rightOperand).cast<HalfLiteralNode>();

	if (x && y)
	{
	    switch (binOp->op)
	    {
	      case TK_AND:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value && y->value);

	      case TK_DIV:
		if (y->value == 0)
		{
		    MESSAGE_LW (lcontext, ERR_DIV_ZERO, expr->lineNumber,
				"Warning: Division by zero "
				"(" << x->value << "/" << y->value << ").");
		}

		return lcontext.newHalfLiteralNode
			    (expr->lineNumber, x->value / y->value);

	      case TK_EQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value == y->value);

	      case TK_GREATER:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value > y->value);

	      case TK_GREATEREQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value >= y->value);

	      case TK_LESS:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value < y->value);

	      case TK_LESSEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value <= y->value);

	      case TK_MINUS:
		return lcontext.newHalfLiteralNode
			    (expr->lineNumber, x->value - y->value);

	      case TK_NOTEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value != y->value);

	      case TK_OR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value || y->value);

	      case TK_PLUS:
		return lcontext.newHalfLiteralNode
			    (expr->lineNumber, x->value + y->value);

	      case TK_TIMES:
		return lcontext.newHalfLiteralNode
			    (expr->lineNumber, x->value * y->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Invalid operand types "
		    "for " << tokenAsString (binOp->op) << " operator "
		    "(" << binOp->leftOperand->type->asString() << " " <<
		    tokenAsString (binOp->op) << " " <<
		    binOp->rightOperand->type->asString() << ").");
	    }
	}
    }

    return expr;
}


ExprNodePtr
HalfType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (BoolLiteralNodePtr x = expr.cast<BoolLiteralNode>())
	return lcontext.newHalfLiteralNode (x->lineNumber, (half) x->value);

    if (IntLiteralNodePtr x = expr.cast<IntLiteralNode>())
	return lcontext.newHalfLiteralNode (x->lineNumber, (half) x->value);

    if (UIntLiteralNodePtr x = expr.cast<UIntLiteralNode>())
	return lcontext.newHalfLiteralNode (x->lineNumber, (half) x->value);

    if (FloatLiteralNodePtr x = expr.cast<FloatLiteralNode>())
	return lcontext.newHalfLiteralNode (x->lineNumber, (half) x->value);

    return expr;
}


void
HalfType::print (int indent) const
{
    cout << setw (indent) << "" << "half " << endl;
}


string
HalfType::asString () const
{
    return "half";
};


FloatType::FloatType (): DataType ()
{
    // empty
}


bool
FloatType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
FloatType::canPromoteFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>() ||
	   t.cast<FloatType>();
}


bool
FloatType::canCastFrom (const TypePtr &t) const
{
    return t.cast<BoolType>() ||
	   t.cast<IntType>() ||
	   t.cast<UIntType>() ||
	   t.cast<HalfType>() ||
	   t.cast<FloatType>();
}


bool
FloatType::canAssign (const TypePtr &t) const
{
    return canCastFrom(t);
}

ExprNodePtr
FloatType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (UnaryOpNodePtr unOp = expr.cast<UnaryOpNode>())
    {
	FloatLiteralNodePtr x = unOp->operand.cast<FloatLiteralNode>();

	if (x)
	{
	    switch (unOp->op)
	    {
	      case TK_MINUS:
		return lcontext.newFloatLiteralNode
			    (expr->lineNumber, -x->value);

	      case TK_NOT:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, !x->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Cannot apply " << tokenAsString (unOp->op) << " "
		    "operator to value of type " << asString() << ".");
	    }
	}
    }

    if (BinaryOpNodePtr binOp = expr.cast<BinaryOpNode>())
    {
	FloatLiteralNodePtr x =
	    evaluate(lcontext, binOp->leftOperand).cast<FloatLiteralNode>();

	FloatLiteralNodePtr y =
	    evaluate(lcontext, binOp->rightOperand).cast<FloatLiteralNode>();

	if (x && y)
	{
	    switch (binOp->op)
	    {
	      case TK_AND:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value && y->value);

	      case TK_DIV:
		if (y->value == 0)
		{
		    MESSAGE_LW (lcontext, ERR_DIV_ZERO, expr->lineNumber,
				"Warning: Division by zero "
				"(" << x->value << "/" << y->value << ").");
		}

		return lcontext.newFloatLiteralNode
			    (expr->lineNumber, x->value / y->value);

	      case TK_EQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value == y->value);

	      case TK_GREATER:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value > y->value);

	      case TK_GREATEREQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value >= y->value);

	      case TK_LESS:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value < y->value);

	      case TK_LESSEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value <= y->value);

	      case TK_MINUS:
		return lcontext.newFloatLiteralNode
			    (expr->lineNumber, x->value - y->value);

	      case TK_NOTEQUAL:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value != y->value);

	      case TK_OR:
		return lcontext.newBoolLiteralNode
			    (expr->lineNumber, x->value || y->value);

	      case TK_PLUS:
		return lcontext.newFloatLiteralNode
			    (expr->lineNumber, x->value + y->value);

	      case TK_TIMES:
		return lcontext.newFloatLiteralNode
			    (expr->lineNumber, x->value * y->value);

	      default:

		MESSAGE_LE (lcontext, ERR_OP_TYPE, expr->lineNumber,
		    "Invalid operand types "
		    "for " << tokenAsString (binOp->op) << " operator "
		    "(" << binOp->leftOperand->type->asString() << " " <<
		    tokenAsString (binOp->op) << " " <<
		    binOp->rightOperand->type->asString() << ").");
	    }
	}
    }

    return expr;
}


ExprNodePtr
FloatType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    if (BoolLiteralNodePtr x = expr.cast<BoolLiteralNode>())
	return lcontext.newFloatLiteralNode (x->lineNumber, (float) x->value);

    if (IntLiteralNodePtr x = expr.cast<IntLiteralNode>())
	return lcontext.newFloatLiteralNode (x->lineNumber, (float) x->value);

    if (UIntLiteralNodePtr x = expr.cast<UIntLiteralNode>())
	return lcontext.newFloatLiteralNode (x->lineNumber, (float) x->value);

    if (HalfLiteralNodePtr x = expr.cast<HalfLiteralNode>())
	return lcontext.newFloatLiteralNode (x->lineNumber, (float) x->value);

    return expr;
}


void
FloatType::print (int indent) const
{
    cout << setw (indent) << "" << "float " << endl;
}


string
FloatType::asString () const
{
    return "float";
};


StringType::StringType (): DataType ()
{
    // empty
}


bool
StringType::isSameTypeAs (const TypePtr &t) const
{
    return t && typeid (*this) == typeid (*t);
}


bool
StringType::canPromoteFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}


bool
StringType::canCastFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}


bool
StringType::canAssign (const TypePtr &t) const
{
    return false;
}

ExprNodePtr
StringType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


ExprNodePtr
StringType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


void
StringType::print (int indent) const
{
    cout << setw (indent) << "" << "string" << endl;
}


string
StringType::asString () const
{
    return "string";
};


ArrayType::ArrayType
    (const DataTypePtr &elemType,
     int size)
:
    _elementType (elemType),
    _size (size)
{
    assert (_elementType);
}


size_t
ArrayType::elementSize () const
{
    return _elementType->alignedObjectSize();
}


bool
ArrayType::isSameTypeAs (const TypePtr &t) const
{
    ArrayTypePtr arrayT = t.cast<ArrayType>();

    return arrayT
	&& (size() == arrayT->size() || size() == 0 || arrayT->size() == 0)
	&&  elementType()->isSameTypeAs (arrayT->elementType());
}


bool
ArrayType::canPromoteFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}


bool
ArrayType::canCastFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}

bool
ArrayType::canAssign (const TypePtr &t) const
{
    ArrayTypePtr arrayT = t.cast<ArrayType>();
    return arrayT
	&& size() != 0
	&& arrayT->size() != 0
	&& canCastFrom(t);
}

ExprNodePtr
ArrayType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


ExprNodePtr
ArrayType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


void
ArrayType::print (int indent) const
{
    cout << setw (indent) << "" << "array" << endl;
    elementType()->print (indent + 1);
    cout << setw (indent + 1) << "" << "size " << size() << endl;
}


string
ArrayType::asString () const
{
    stringstream ss;
    ss << elementType()->asString() << "[" << size() << "]";
    return ss.str();
};


void
ArrayType::sizes(SizeVector &sizes) const
{
    sizes.clear();
    sizes.push_back(_size);

    for(ArrayTypePtr subArray = _elementType.cast<ArrayType>();
	subArray;
	subArray = subArray->_elementType.cast<ArrayType>())
    {
	sizes.push_back(subArray->_size);
    }
	
}

void
ArrayType::coreSizes(size_t parentOffset,
		     SizeVector &sizes,
		     SizeVector &offsets) const
{
    for(int i = 0; i < _size; i++)
    {
	_elementType->coreSizes(parentOffset, sizes, offsets);
	parentOffset += _elementType->alignedObjectSize();
    }
}


DataTypePtr
ArrayType::coreType() const
{
    DataTypePtr dataType = elementType();
    ArrayTypePtr arrayType = dataType.cast<ArrayType>();

    while( arrayType )
    {
	dataType = arrayType->elementType();
	arrayType = dataType.cast<ArrayType>();
    }
    return dataType;
}




Member::Member (const string &name, const DataTypePtr &type):
    name (name),
    type (type),
    offset (0)
{
    // empty
}


Member::~Member ()
{
    // empty
}


StructType::StructType
    (const string &name,
     const MemberVector &members)
:
    _name (name),
    _members (members)
{
}


bool	
StructType::isSameTypeAs (const TypePtr &t) const
{
    //
    // Two struct types are the same if they have the same name
    //

    StructTypePtr structT = t.cast<StructType>();

    if (!structT)
	return false;

    return (_name == structT->_name);
}


bool	
StructType::canPromoteFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}


bool	
StructType::canCastFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}


bool
StructType::canAssign (const TypePtr &t) const
{
    return canCastFrom(t);
}

ExprNodePtr	
StructType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


ExprNodePtr	
StructType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


void	
StructType::print (int indent) const
{
    cout << setw (indent) << "" << "struct " << _name << endl;

    for (size_t i = 0; i < _members.size(); ++i)
	_members[i].type->print (indent + 1);
}


std::string	
StructType::asString () const
{
    stringstream ss;
    ss << "struct " << _name;
    return ss.str();
}



void
StructType::coreSizes(size_t parentOffset,
		      SizeVector &sizes, 
		      SizeVector &offsets) const
{
    for( MemberVectorConstIterator it = members().begin();
	 it != members().end();
	 it++ )
    {
	it->type->coreSizes(parentOffset + it->offset, sizes, offsets);
    }
}


Param::Param
    (const string &name,
     const DataTypePtr &type,
     const ExprNodePtr &defaultValue,
     ReadWriteAccess access,
     bool varying)
:
    name (name),
    type (type),
    defaultValue (defaultValue),
    access (access),
    varying (varying)
{
    // empty
}


Param::~Param ()
{
    // empty
}

bool 
Param::isWritable()  const
{ 
    return access == RWA_WRITE  || access == RWA_READWRITE; 
}

bool
Param::isReadable () const
{
    return access == RWA_READ || access == RWA_READWRITE;
}



FunctionType::FunctionType
    (const DataTypePtr &returnType,
     bool returnVarying,
     const ParamVector &parameters)
:
    _returnType (returnType),
    _returnVarying (returnVarying),
    _parameters (parameters)
{
    assert (returnType);

    for (int i = 0; i < (int)parameters.size(); ++i)
	assert (parameters[i].type);
}


bool
FunctionType::isSameTypeAs (const TypePtr &t) const
{
    FunctionTypePtr functionT = t.cast<FunctionType>();

    if (!functionT)
	return false;

    if (!returnType()->isSameTypeAs (functionT->returnType()))
	return false;

    if (parameters().size() != functionT->parameters().size())
	return false;

    for (int i = 0; i < (int)parameters().size(); ++i)
    {
	if (!parameters()[i].type->isSameTypeAs
		    (functionT->parameters()[i].type))
	{
	    return false;
	}
    }

    return true;
}


bool
FunctionType::canPromoteFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}


bool
FunctionType::canCastFrom (const TypePtr &t) const
{
    return isSameTypeAs (t);
}

bool
FunctionType::canAssign (const TypePtr &t) const
{
    return false;
}


ExprNodePtr
FunctionType::evaluate (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


ExprNodePtr
FunctionType::castValue (LContext &lcontext, const ExprNodePtr &expr) const
{
    return expr;
}


void
FunctionType::print (int indent) const
{
    cout << setw (indent) << "" << "function" << endl;

    cout << setw (indent + 1) << "" << "return type" << endl;
    returnType()->print (indent + 2);

    cout << setw (indent + 1) << "" << "parameters" << endl;

    for (int i = 0; i < (int)parameters().size(); ++i)
    {
	cout << setw (indent + 2) << "" << "type" << endl;
	parameters()[i].type->print (indent + 3);

	if (parameters()[i].defaultValue)
	{
	    cout << setw (indent + 2) << "" << "default value" << endl;
	    parameters()[i].defaultValue->print (indent + 3);
	}
    }
}


string
FunctionType::asString () const
{
    stringstream ss;

    ss << (returnType()? returnType()->asString(): "unknown") << "(";

    for (int i = 0; i < (int)parameters().size(); ++i)
	ss << parameters()[i].type->asString() <<
	      (i < (int)parameters().size() - 1? ",": "");

    ss << ")";

    return ss.str();
};


} // namespace Ctl
