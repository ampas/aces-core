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
//	Types in the SIMD implementation of the color transformation language
//
//-----------------------------------------------------------------------------

#include <CtlSimdType.h>
#include <CtlSyntaxTree.h>
#include <CtlSymbolTable.h>
#include <CtlSimdLContext.h>
#include <CtlSimdModule.h>
#include <CtlSimdInst.h>
#include <CtlSimdOp.h>
#include <CtlAlign.h>
#include <CtlMessage.h>
#include <cassert>
#include <CtlErrors.h>

using namespace std;

namespace Ctl {
namespace {

bool
isAssignment (const SyntaxNodePtr &node)
{
    return node.cast<AssignmentNode>() ||
	   node.cast<VariableNode>() ||
	   node.cast<ReturnNode>();
}

} // namespace


SimdVoidType::SimdVoidType(): VoidType ()
{
    // empty
}


size_t
SimdVoidType::objectSize () const
{
    return 0;
}


size_t
SimdVoidType::alignedObjectSize () const
{
    return 0;
}


size_t
SimdVoidType::objectAlignment () const
{
    return 1;
}


void
SimdVoidType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    return;
}


void
SimdVoidType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
}


SimdBoolType::SimdBoolType (): BoolType ()
{
    // empty
}


size_t
SimdBoolType::objectSize () const
{
    return sizeof (bool);
}


size_t
SimdBoolType::alignedObjectSize () const
{
    return sizeof (bool);
}


size_t
SimdBoolType::objectAlignment () const
{
    return sizeof (bool);
}


void
SimdBoolType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (expr->type.cast<BoolType>())
    {
	return;
    }

    if (expr->type.cast<IntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <int, bool, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<UIntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <unsigned, bool, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<HalfType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <half, bool, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<FloatType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <float, bool, CopyOp>
			   (expr->lineNumber));
	return;
    }

    MESSAGE_LE (lcontext, ERR_TYPE, expr->lineNumber,
		"Cannot cast value of type " << expr->type->asString() <<
		" to type " << asString() << ".");
}


void
SimdBoolType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (isAssignment (node))
    {
	slcontext.addInst
	    (new SimdAssignInst (alignedObjectSize(), node->lineNumber));
	return;
    }

    if (UnaryOpNodePtr unOp = node.cast<UnaryOpNode>())
    {
	switch (unOp->op)
	{
	  case TK_BITNOT:

	    //
	    // We use the C++ ! operation to evaluate the CTL expression ~x,
	    // where x is of type bool.  This ensures that ~true == false
	    // and ~false == true.
	    //

	    slcontext.addInst
		(new SimdUnaryOpInst <bool, bool, NotOp>(node->lineNumber));
	    break;

	  case TK_NOT:
	    slcontext.addInst
		(new SimdUnaryOpInst <bool, bool, NotOp>(node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Cannot apply " << tokenAsString (unOp->op) << " "
		"operator to value of type " << 
		unOp->operand->type->asString() << ".");
	}

	return;
    }

    if (BinaryOpNodePtr binOp = node.cast<BinaryOpNode>())
    {
	switch (binOp->op)
	{
	  case TK_BITAND:

	    //
	    // For the bit-wise &, | and ^ operators, we normalize bool
	    // operands before applying the operators.  This avoids
	    // surprises, for example, true^true == true.
	    // 

	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, BoolBitAndOp>
		 (node->lineNumber));
	    break;

	  case TK_BITOR:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, BoolBitOrOp>
		 (node->lineNumber));
	    break;

	  case TK_BITXOR:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, BoolBitXorOp>
		 (node->lineNumber));
	    break;

	  case TK_EQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, EqualOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATER:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, GreaterOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATEREQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, GreaterEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_LESS:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, LessOp>
		 (node->lineNumber));
	    break;

	  case TK_LESSEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, LessEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_NOTEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <bool, bool, bool, NotEqualOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Invalid operand types "
		"for " << tokenAsString (binOp->op) << " operator "
		"(" << binOp->leftOperand->type->asString() << " " <<
		tokenAsString (binOp->op) << " " <<
		binOp->rightOperand->type->asString() << ").");
	}

	return;
    }

    if (node.cast<CallNode>())
    {
	//
	// Push a placeholder for the return value for a call to
	// a function that returns an int.
	//
	slcontext.addInst (new SimdPushPlaceholderInst(alignedObjectSize(),
						       node->lineNumber));
	return;
    }
}

AddrPtr
newStaticVariableGeneric(Module *module, size_t objectSize)
{
    SimdModule *smodule = static_cast <SimdModule *> (module);
    SimdReg* reg = new SimdReg (false, objectSize);
    
    smodule->addStaticData (reg);
    return new SimdDataAddr (reg);
}


AddrPtr
SimdBoolType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, objectSize());
}

void
SimdBoolType::newAutomaticVariable (StatementNodePtr node, 
				    LContext &lcontext) const 
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (objectSize(), node->lineNumber));
}


SimdIntType::SimdIntType (): IntType ()
{
    // empty
}


size_t
SimdIntType::objectSize () const
{
    return sizeof (int);
}


size_t
SimdIntType::alignedObjectSize () const
{
    return sizeof (int);
}


size_t
SimdIntType::objectAlignment () const
{
    return sizeof (int);
}


void
SimdIntType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (expr->type.cast<BoolType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <bool, int, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<IntType>())
    {
	return;
    }

    if (expr->type.cast<UIntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <unsigned, int, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<HalfType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <half, int, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<FloatType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <float, int, CopyOp>
			   (expr->lineNumber));
	return;
    }

    MESSAGE_LE (lcontext, ERR_TYPE, expr->lineNumber,
		"Cannot cast value of type " << expr->type->asString() <<
		" to type " << asString() << ".");
}


void
SimdIntType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (isAssignment (node))
    {
	slcontext.addInst
	    (new SimdAssignInst (alignedObjectSize(), node->lineNumber));
	return;
    }

    if (UnaryOpNodePtr unOp = node.cast<UnaryOpNode>())
    {
	switch (unOp->op)
	{
	  case TK_BITNOT:
	    slcontext.addInst
		(new SimdUnaryOpInst <int, int, BitNotOp>(node->lineNumber));
	    break;

	  case TK_MINUS:
	    slcontext.addInst
		(new SimdUnaryOpInst <int, int, UnaryMinusOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Cannot apply " << tokenAsString (unOp->op) << " "
		"operator to value of type " << 
		unOp->operand->type->asString() << ".");
	}

	return;
    }

    if (BinaryOpNodePtr binOp = node.cast<BinaryOpNode>())
    {
	switch (binOp->op)
	{
	  case TK_BITAND:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, BitAndOp>
		 (node->lineNumber));
	    break;

	  case TK_BITOR:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, BitOrOp>
		 (node->lineNumber));
	    break;

	  case TK_BITXOR:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, BitXorOp>
		 (node->lineNumber));
	    break;

	  case TK_DIV:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, IntDivOp>
		 (node->lineNumber));
	    break;

	  case TK_EQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, bool, EqualOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATER:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, bool, GreaterOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATEREQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, bool, GreaterEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_LEFTSHIFT:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, LeftShiftOp>
		 (node->lineNumber));
	    break;

	  case TK_LESS:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, bool, LessOp>
		 (node->lineNumber));
	    break;

	  case TK_LESSEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, bool, LessEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_MINUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, BinaryMinusOp>
		 (node->lineNumber));
	    break;

	  case TK_MOD:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, ModOp>
		 (node->lineNumber));
	    break;

	  case TK_NOTEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, bool, NotEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_PLUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, PlusOp>
		 (node->lineNumber));
	    break;

	  case TK_RIGHTSHIFT:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, RightShiftOp>
		 (node->lineNumber));
	    break;

	  case TK_TIMES:
	    slcontext.addInst
		(new SimdBinaryOpInst <int, int, int, TimesOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Invalid operand types "
		"for " << tokenAsString (binOp->op) << " operator "
		"(" << binOp->leftOperand->type->asString() << " " <<
		tokenAsString (binOp->op) << " " <<
		binOp->rightOperand->type->asString() << ").");
	}

	return;
    }

    if (node.cast<CallNode>())
    {
	//
	// Push a placeholder for the return value for a call to
	// a function that returns an int.
	//
	slcontext.addInst (new SimdPushPlaceholderInst(alignedObjectSize(),
						       node->lineNumber));
	return;
    }
}



AddrPtr
SimdIntType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, objectSize());
}

void
SimdIntType::newAutomaticVariable (StatementNodePtr node, 
				   LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (objectSize(), node->lineNumber));
}


SimdUIntType::SimdUIntType (): UIntType ()
{
    // empty
}


size_t
SimdUIntType::objectSize () const
{
    return sizeof (unsigned int);
}


size_t
SimdUIntType::alignedObjectSize () const
{
    return sizeof (unsigned int);
}


size_t
SimdUIntType::objectAlignment () const
{
    return sizeof (unsigned int);
}


void
SimdUIntType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (expr->type.cast<BoolType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <bool, unsigned, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<IntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <int, unsigned, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<UIntType>())
    {
	return;
    }

    if (expr->type.cast<HalfType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <half, unsigned, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<FloatType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <float, unsigned, CopyOp>
			   (expr->lineNumber));
	return;
    }

    MESSAGE_LE (lcontext, ERR_TYPE, expr->lineNumber,
		"Cannot cast value of type " << expr->type->asString() <<
		" to type " << asString() << ".");
}


void
SimdUIntType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (isAssignment (node))
    {
	slcontext.addInst
	    (new SimdAssignInst (alignedObjectSize(), node->lineNumber));
	return;
    }

    if (UnaryOpNodePtr unOp = node.cast<UnaryOpNode>())
    {
	switch (unOp->op)
	{
	  case TK_BITNOT:
	    slcontext.addInst
		(new SimdUnaryOpInst <unsigned, unsigned, BitNotOp>
		 (node->lineNumber));
	    break;

	  case TK_MINUS:
	    slcontext.addInst
		(new SimdUnaryOpInst <unsigned, int, UnaryMinusOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Cannot apply " << tokenAsString (unOp->op) << " "
		"operator to value of type " << 
		unOp->operand->type->asString() << ".");
	}

	return;
    }

    if (BinaryOpNodePtr binOp = node.cast<BinaryOpNode>())
    {
	switch (binOp->op)
	{
	  case TK_BITAND:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, BitAndOp>
		 (node->lineNumber));
	    break;

	  case TK_BITOR:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, BitOrOp>
		 (node->lineNumber));
	    break;

	  case TK_BITXOR:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, BitXorOp>
		 (node->lineNumber));
	    break;

	  case TK_DIV:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, IntDivOp>
		 (node->lineNumber));
	    break;

	  case TK_EQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, bool, EqualOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATER:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, bool, GreaterOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATEREQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, bool, GreaterEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_LEFTSHIFT:
	    slcontext.addInst
		(new SimdBinaryOpInst
			<unsigned, unsigned, unsigned, LeftShiftOp>
		 (node->lineNumber));
	    break;

	  case TK_LESS:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, bool, LessOp>
		 (node->lineNumber));
	    break;

	  case TK_LESSEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, bool, LessEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_MINUS:
	    slcontext.addInst
		(new SimdBinaryOpInst
			<unsigned, unsigned, unsigned, BinaryMinusOp>
		 (node->lineNumber));
	    break;

	  case TK_MOD:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, ModOp>
		 (node->lineNumber));
	    break;

	  case TK_NOTEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, bool, NotEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_PLUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, PlusOp>
		 (node->lineNumber));
	    break;

	  case TK_RIGHTSHIFT:
	    slcontext.addInst
		(new SimdBinaryOpInst
			<unsigned, unsigned, unsigned, RightShiftOp>
		 (node->lineNumber));
	    break;

	  case TK_TIMES:
	    slcontext.addInst
		(new SimdBinaryOpInst <unsigned, unsigned, unsigned, TimesOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Invalid operand types "
		"for " << tokenAsString (binOp->op) << " operator "
		"(" << binOp->leftOperand->type->asString() << " " <<
		tokenAsString (binOp->op) << " " <<
		binOp->rightOperand->type->asString() << ").");
	}

	return;
    }

    if (node.cast<CallNode>())
    {
	//
	// Push a placeholder for the return value for a call to
	// a function that returns an int.
	//
	slcontext.addInst
	    (new SimdPushPlaceholderInst (alignedObjectSize(),
					  node->lineNumber));
	return;
    }
}


AddrPtr
SimdUIntType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, objectSize());
}


void
SimdUIntType::newAutomaticVariable (StatementNodePtr node, 
				   LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (objectSize(), node->lineNumber));
}


SimdHalfType::SimdHalfType (): HalfType ()
{
    // empty
}


size_t
SimdHalfType::objectSize () const
{
    return sizeof (half);
}


size_t
SimdHalfType::alignedObjectSize () const
{
    return sizeof (half);
}


size_t
SimdHalfType::objectAlignment () const
{
    return sizeof (half);
}


void
SimdHalfType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (expr->type.cast<BoolType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <bool, half, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<IntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <int, half, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<UIntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <unsigned, half, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<HalfType>())
    {
	return;
    }

    if (expr->type.cast<FloatType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <float, half, CopyOp>
			   (expr->lineNumber));
	return;
    }

    MESSAGE_LE (lcontext, ERR_TYPE, expr->lineNumber,
		"Cannot cast value of type " << expr->type->asString() <<
		" to type " << asString() << ".");
}


void
SimdHalfType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (isAssignment (node))
    {
	slcontext.addInst
	    (new SimdAssignInst (alignedObjectSize(), node->lineNumber));
	return;
    }

    if (UnaryOpNodePtr unOp = node.cast<UnaryOpNode>())
    {
	switch (unOp->op)
	{
	  case TK_MINUS:
	    slcontext.addInst
		(new SimdUnaryOpInst <half, half, UnaryMinusOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Cannot apply " << tokenAsString (unOp->op) << " "
		"operator to value of type " << 
		unOp->operand->type->asString() << ".");
	}

	return;
    }

    if (BinaryOpNodePtr binOp = node.cast<BinaryOpNode>())
    {
	switch (binOp->op)
	{
	  case TK_DIV:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, half, DivOp>
		 (node->lineNumber));
	    break;

	  case TK_EQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, bool, EqualOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATER:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, bool, GreaterOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATEREQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, bool, GreaterEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_LESS:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, bool, LessOp>
		 (node->lineNumber));
	    break;

	  case TK_LESSEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, bool, LessEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_MINUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, half, BinaryMinusOp>
		 (node->lineNumber));
	    break;

	  case TK_NOTEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, bool, NotEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_PLUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, half, PlusOp>
		 (node->lineNumber));
	    break;

	  case TK_TIMES:
	    slcontext.addInst
		(new SimdBinaryOpInst <half, half, half, TimesOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Invalid operand types "
		"for " << tokenAsString (binOp->op) << " operator "
		"(" << binOp->leftOperand->type->asString() << " " <<
		tokenAsString (binOp->op) << " " <<
		binOp->rightOperand->type->asString() << ").");
	}

	return;
    }

    if (node.cast<CallNode>())
    {
	//
	// Push a placeholder for the return value for a call to
	// a function that returns an int.
	//

	slcontext.addInst
	    (new SimdPushPlaceholderInst (alignedObjectSize(),
					  node->lineNumber));
	return;
    }
}


AddrPtr
SimdHalfType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, objectSize());
}


void
SimdHalfType::newAutomaticVariable (StatementNodePtr node, 
				    LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (objectSize(), node->lineNumber));
}


SimdFloatType::SimdFloatType (): FloatType ()
{
    // empty
}


size_t
SimdFloatType::objectSize () const
{
    return sizeof (float);
}


size_t
SimdFloatType::alignedObjectSize () const
{
    return sizeof (float);
}


size_t
SimdFloatType::objectAlignment () const
{
    return sizeof (float);
}


void
SimdFloatType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (expr->type.cast<BoolType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <bool, float, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<IntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <int, float, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<UIntType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <unsigned, float, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<HalfType>())
    {
	slcontext.addInst (new SimdUnaryOpInst <half, float, CopyOp>
			   (expr->lineNumber));
	return;
    }

    if (expr->type.cast<FloatType>())
    {
	return;
    }

    MESSAGE_LE (lcontext, ERR_TYPE, expr->lineNumber,
		"Cannot cast value of type " << expr->type->asString() <<
		" to type " << asString() << ".");
}


void
SimdFloatType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (isAssignment (node))
    {
	slcontext.addInst
	    (new SimdAssignInst(alignedObjectSize(), node->lineNumber));
	return;
    }

    if (UnaryOpNodePtr unOp = node.cast<UnaryOpNode>())
    {
	switch (unOp->op)
	{
	  case TK_MINUS:
	    slcontext.addInst
		(new SimdUnaryOpInst <float, float, UnaryMinusOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Cannot apply " << tokenAsString (unOp->op) << " "
		"operator to value of type " << 
		unOp->operand->type->asString() << ".");
	}

	return;
    }

    if (BinaryOpNodePtr binOp = node.cast<BinaryOpNode>())
    {
	switch (binOp->op)
	{
	  case TK_DIV:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, float, DivOp>
		 (node->lineNumber));
	    break;

	  case TK_EQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, bool, EqualOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATER:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, bool, GreaterOp>
		 (node->lineNumber));
	    break;

	  case TK_GREATEREQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, bool, GreaterEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_LESS:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, bool, LessOp>
		 (node->lineNumber));
	    break;

	  case TK_LESSEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, bool, LessEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_MINUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, float, BinaryMinusOp>
		 (node->lineNumber));
	    break;

	  case TK_NOTEQUAL:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, bool, NotEqualOp>
		 (node->lineNumber));
	    break;

	  case TK_PLUS:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, float, PlusOp>
		 (node->lineNumber));
	    break;

	  case TK_TIMES:
	    slcontext.addInst
		(new SimdBinaryOpInst <float, float, float, TimesOp>
		 (node->lineNumber));
	    break;

	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Invalid operand types "
		"for " << tokenAsString (binOp->op) << " operator "
		"(" << binOp->leftOperand->type->asString() << " " <<
		tokenAsString (binOp->op) << " " <<
		binOp->rightOperand->type->asString() << ").");
	}

	return;
    }

    if (node.cast<CallNode>())
    {
	//
	// Push a placeholder for the return value for a call to
	// a function that returns an int.
	//

	slcontext.addInst (new SimdPushPlaceholderInst (alignedObjectSize(),
						        node->lineNumber));
	return;
    }
}


AddrPtr
SimdFloatType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, objectSize());
}


void
SimdFloatType::newAutomaticVariable (StatementNodePtr node, 
				     LContext &lcontext) const 
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (objectSize(), node->lineNumber));
}


SimdStringType::SimdStringType (): StringType ()
{
    // empty
}


size_t
SimdStringType::objectSize () const
{
    return sizeof (string *);
}


size_t
SimdStringType::alignedObjectSize () const
{
    return sizeof (string *);
}


size_t
SimdStringType::objectAlignment () const
{
    return sizeof (string *);
}


void
SimdStringType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    if (expr->type.cast<StringType>())
    {
	return;
    }

    MESSAGE_LE (lcontext, ERR_TYPE, expr->lineNumber,
		"Cannot cast value of type " << expr->type->asString() <<
		" to type " << asString() << ".");
}


void
SimdStringType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    if (isAssignment (node))
    {
	slcontext.addInst
	    (new SimdAssignInst (alignedObjectSize(), node->lineNumber));
	return;
    }

    if (UnaryOpNodePtr unOp = node.cast<UnaryOpNode>())
    {
	switch (unOp->op)
	{
	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Cannot apply " << tokenAsString (unOp->op) << " "
		"operator to value of type " << 
		unOp->operand->type->asString() << ".");
	}

	return;
    }

    if (BinaryOpNodePtr binOp = node.cast<BinaryOpNode>())
    {
	switch (binOp->op)
	{
	  default:

	    MESSAGE_LE (lcontext, ERR_OP_TYPE, node->lineNumber,
		"Invalid operand types "
		"for " << tokenAsString (binOp->op) << " operator "
		"(" << binOp->leftOperand->type->asString() << " " <<
		tokenAsString (binOp->op) << " " <<
		binOp->rightOperand->type->asString() << ").");
	}

	return;
    }

    if (node.cast<CallNode>())
    {
	//
	// Push a placeholder for the return value for a call to
	// a function that returns an int.
	//

	slcontext.addInst (new SimdPushPlaceholderInst (alignedObjectSize(),
	                                                node->lineNumber));
	return;
    }
}


AddrPtr
SimdStringType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric (module, alignedObjectSize());
}


void
SimdStringType::newAutomaticVariable (StatementNodePtr node, 
				     LContext &lcontext) const 
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    slcontext.addInst (new SimdPushPlaceholderInst
			       (alignedObjectSize(), node->lineNumber));
}


SimdArrayType::SimdArrayType (const DataTypePtr &elementType, int size,
			      SimdLContext *lcontext /* = 0 */):
    ArrayType (elementType, size),
    _unknownSize(0),
    _unknownESize(0)
{
    if( lcontext)
    {
	// If size is not specified, this is a function parameter
	// size will be stored as a parameter
	if( size == 0 )
	{
	    IntTypePtr intType = lcontext->newIntType ();
	    _unknownSize = lcontext->parameterAddr (intType);
	}

	// if the element size is not known, create a local variable
	// to store the element size computed later
	SimdArrayTypePtr at = elementType.cast<SimdArrayType>();
	if( at && (at->unknownElementSize() || at->unknownSize() ))
	{
	    IntTypePtr intType = lcontext->newIntType ();
	    _unknownESize = lcontext->autoVariableAddr (intType);
	}
    }
}


size_t
SimdArrayType::objectSize () const
{
    return elementSize() * size();
}


size_t
SimdArrayType::alignedObjectSize () const
{
    return elementSize() * size();
}


size_t
SimdArrayType::objectAlignment () const
{
    return elementType()->objectAlignment();
}

SimdDataAddrPtr
SimdArrayType::unknownElementSize () const
{
    return _unknownESize;
}

SimdDataAddrPtr
SimdArrayType::unknownSize () const
{
    return _unknownSize;
}

void
SimdArrayType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    assert(isSameTypeAs(expr->type));
    return;
}


void
SimdArrayType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    VariableNodePtr var = node.cast<VariableNode>();
    if( var && var->initialValue.cast<ValueNode>())
    {
	SizeVector sizes;
	SizeVector offsets;
	coreSizes(0, sizes, offsets);
 	slcontext.addInst (new SimdInitializeInst(sizes, 
						  offsets,
						  node->lineNumber));
	return;
    }
    else if (isAssignment(node))  // return or assignment
    {
	slcontext.addInst (new SimdAssignArrayInst
			     (size(), elementSize(), node->lineNumber));
	return;
    }
    else if ( node.cast<ArrayIndexNode>() )
    {
	if(unknownSize() || unknownElementSize())
	{
	    
	    slcontext.addInst (new SimdIndexVSArrayInst(elementSize(),
							unknownElementSize(), 
							size(),
							unknownSize(),
							node->lineNumber));
	}
	else
	{
	    slcontext.addInst (new SimdIndexArrayInst(elementSize(), 
						      node->lineNumber,
						      size()));
	}
	return;
    }
    else if (node.cast<SizeNode>())
    {
	assert(size() == 0);
	slcontext.addInst (new SimdPushRefInst (unknownSize(), 
						node->lineNumber));
    }
    else if (node.cast<CallNode>())
    {
	slcontext.addInst (new SimdPushPlaceholderInst(objectSize(), 
						       node->lineNumber));
	return;
    }
}


AddrPtr
SimdArrayType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, objectSize());
}


void
SimdArrayType::newAutomaticVariable (StatementNodePtr node, 
				      LContext &lcontext ) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (objectSize(), node->lineNumber));
}



SimdStructType::SimdStructType
    (const string &name,
     const MemberVector &members)
:
    StructType (name, members),
    _objectSize (0),
    _alignedObjectSize (0),
    _objectAlignment (1)
{
    for (size_t i = 0; i < this->members().size(); ++i)
    {
	Member &m = member (i);

	m.offset = align (_objectSize, m.type->objectAlignment());
	_objectSize = m.offset + m.type->objectSize();

	_objectAlignment =
	    leastCommonMultiple (_objectAlignment, m.type->objectAlignment());
    }

    _alignedObjectSize = align (_objectSize, _objectAlignment);
}


size_t
SimdStructType::objectSize () const
{
    return _objectSize;
}


size_t
SimdStructType::alignedObjectSize () const
{
    return _alignedObjectSize;
}


size_t
SimdStructType::objectAlignment () const
{
    return _objectAlignment;
}


void
SimdStructType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    return;
}


void
SimdStructType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);

    VariableNodePtr var = node.cast<VariableNode>();
    if( var && var->initialValue.cast<ValueNode>())
    {
	SizeVector sizes;
	SizeVector offsets;
	coreSizes(0, sizes, offsets);
 	slcontext.addInst (new SimdInitializeInst(sizes, 
						  offsets, 
						  node->lineNumber));
	return;
    }

    if( MemberNodePtr mem = node.cast<MemberNode>() )
    {
 	slcontext.addInst (new SimdAccessMemberInst(mem->offset,
 						       (node->lineNumber)));
	return;
    }

    if (isAssignment (node))
    {
	slcontext.addInst (new SimdAssignInst
			     (alignedObjectSize(), node->lineNumber));
	return;
    }

    if (node.cast<CallNode>())
    {
	// Push a placeholder for the return value for a call to
	// a function that returns a struct
	slcontext.addInst (new SimdPushPlaceholderInst (alignedObjectSize(),
						        node->lineNumber));
	return;
    }
}


AddrPtr
SimdStructType::newStaticVariable (Module *module) const
{
    return newStaticVariableGeneric(module, alignedObjectSize());
}


void
SimdStructType::newAutomaticVariable (StatementNodePtr node, 
				      LContext &lcontext ) const
{
    SimdLContext &slcontext = static_cast <SimdLContext &> (lcontext);
    slcontext.addInst (new SimdPushPlaceholderInst
		       (alignedObjectSize(), node->lineNumber));
}


SimdFunctionType::SimdFunctionType
    (const DataTypePtr &returnType,
     bool returnVarying,
     const ParamVector &parameters)
:
    FunctionType (returnType, returnVarying, parameters)
{
    // empty
}


void
SimdFunctionType::generateCastFrom
    (const ExprNodePtr &expr,
     LContext &lcontext) const
{
    // empty
}


void
SimdFunctionType::generateCode
    (const SyntaxNodePtr &node,
     LContext &lcontext) const
{
    // empty
}


} // namespace Ctl

