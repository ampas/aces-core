//////////////////////////////////////////////////////////////////////////
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


#ifndef INCLUDED_CTL_SYNTAX_TREE_H
#define INCLUDED_CTL_SYNTAX_TREE_H

//-----------------------------------------------------------------------------
//
//	The syntax tree for the color transformation language.
//
//-----------------------------------------------------------------------------

#include <CtlTokens.h>
#include <CtlRcPtr.h>
#include <half.h>
#include <vector>
#include <string>

namespace Ctl {


struct SyntaxNode;
typedef RcPtr<SyntaxNode> SyntaxNodePtr;

struct ModuleNode;
typedef RcPtr<ModuleNode> ModuleNodePtr;

struct FunctionNode;
typedef RcPtr<FunctionNode> FunctionNodePtr;

struct StatementNode;
typedef RcPtr<StatementNode> StatementNodePtr;

struct VariableNode;
typedef RcPtr<VariableNode> VariableNodePtr;

struct AssignmentNode;
typedef RcPtr<AssignmentNode> AssignmentNodePtr;

struct ExprStatementNode;
typedef RcPtr<ExprStatementNode> ExprStatementNodePtr;

struct IfNode;
typedef RcPtr<IfNode> IfNodePtr;

struct ReturnNode;
typedef RcPtr<ReturnNode> ReturnNodePtr;

struct WhileNode;
typedef RcPtr<WhileNode> WhileNodePtr;

struct ExprNode;
typedef RcPtr<ExprNode> ExprNodePtr;

struct BinaryOpNode;
typedef RcPtr<BinaryOpNode> BinaryOpNodePtr;

struct UnaryOpNode;
typedef RcPtr<UnaryOpNode> UnaryOpNodePtr;

struct ArrayIndexNode;
typedef RcPtr<ArrayIndexNode> ArrayIndexNodePtr;

struct MemberNode;
typedef RcPtr<MemberNode> MemberNodePtr;

struct SizeNode;
typedef RcPtr<SizeNode> SizeNodePtr;

struct NameNode;
typedef RcPtr<NameNode> NameNodePtr;

struct LiteralNode;
typedef RcPtr<LiteralNode> LiteralNodePtr;

struct BoolLiteralNode;
typedef RcPtr<BoolLiteralNode> BoolLiteralNodePtr;

struct IntLiteralNode;
typedef RcPtr<IntLiteralNode> IntLiteralNodePtr;

struct UIntLiteralNode;
typedef RcPtr<UIntLiteralNode> UIntLiteralNodePtr;

struct HalfLiteralNode;
typedef RcPtr<HalfLiteralNode> HalfLiteralNodePtr;

struct FloatLiteralNode;
typedef RcPtr<FloatLiteralNode> FloatLiteralNodePtr;

struct StringLiteralNode;
typedef RcPtr<StringLiteralNode> StringLiteralNodePtr;

struct CallNode;
typedef RcPtr<CallNode> CallNodePtr;

struct ValueNode;
typedef RcPtr<ValueNode> ValueNodePtr;

class SymbolInfo;
typedef RcPtr<SymbolInfo> SymbolInfoPtr;

class Type;
typedef RcPtr<Type> TypePtr;

class DataType;
typedef RcPtr<DataType> DataTypePtr;

typedef std::vector<size_t> SizeVector;
typedef std::vector<size_t>::iterator SizeVectorIterator;

class LContext;


struct SyntaxNode: public RcObject
{
    //-------------------------------------------------------------
    // class SyntaxNode is the base class for all syntax tree nodes
    //-------------------------------------------------------------

    SyntaxNode (int lineNumber);
    virtual ~SyntaxNode ();

    void		printTree () const;
    virtual void	print (int indent) const = 0;


    //----------------------------
    // Generate code for this node
    //----------------------------

    virtual void	generateCode (LContext &lcontext) = 0;


    int			lineNumber;
};
    

struct ModuleNode: public SyntaxNode
{
    ModuleNode (int lineNumber,
	        const StatementNodePtr &constants,
		const FunctionNodePtr &functions);

    virtual void	print (int indent) const;

    StatementNodePtr	constants;
    FunctionNodePtr	functions;
};
    

struct FunctionNode: public SyntaxNode
{
    FunctionNode (int lineNumber,
	          const std::string &name,
		  const SymbolInfoPtr &info,
		  const StatementNodePtr &body);

    virtual void	print (int indent) const;

    std::string		name;
    SymbolInfoPtr	info;
    StatementNodePtr	body;
    FunctionNodePtr	next;
};


struct StatementNode: public SyntaxNode
{
    //---------------------------------------------------------
    // class StatementNode is the base class for all nodes that
    // represent CTL statements
    //---------------------------------------------------------

    StatementNode (int lineNumber);

    //--------------------------------------------------------
    // Check if all paths through the code that begin at this
    // node end with a ReturnNode.  Used to check if execution
    // of a non-void function can "fall off the end" of the
    // function without returning a value.
    //--------------------------------------------------------

    virtual bool	pathEndsWithReturn () const = 0;


    StatementNodePtr	next;
};


struct LinearStatementNode: public StatementNode
{
    //---------------------------------------------------------
    // class LinearStatementNode is the base class for all
    // CTL statements that represent a "linear" control flow,
    // without branching, looping or returning from a function.
    //---------------------------------------------------------

    LinearStatementNode (int lineNumber);

    virtual bool	pathEndsWithReturn () const;
};


struct VariableNode: public LinearStatementNode
{
    VariableNode (int lineNumber,
	          const std::string &name,
		  const SymbolInfoPtr &info,
		  const ExprNodePtr &initialValue,
		  bool assignInitialValue);

    virtual void	print (int indent) const;

    std::string		name;
    SymbolInfoPtr	info;
    ExprNodePtr		initialValue;
    bool		assignInitialValue;
};


struct AssignmentNode: public LinearStatementNode
{
    AssignmentNode (int lineNumber,
		    const ExprNodePtr &lhs,
		    const ExprNodePtr &rhs);

    virtual void	print (int indent) const;

    ExprNodePtr		lhs;
    ExprNodePtr		rhs;
};


struct ExprStatementNode: public LinearStatementNode
{
    ExprStatementNode (int lineNumber, const ExprNodePtr &expr);

    virtual void	print (int indent) const;

    ExprNodePtr		expr;
};


struct IfNode: public StatementNode
{
    IfNode (int lineNumber,
	    const ExprNodePtr &condition,
	    const StatementNodePtr &truePath,
	    const StatementNodePtr &falsePath);

    virtual void	print (int indent) const;
    virtual bool	pathEndsWithReturn () const;

    ExprNodePtr		condition;
    StatementNodePtr	truePath;
    StatementNodePtr	falsePath;
};


struct ReturnNode: public StatementNode
{
    ReturnNode (int lineNumber,
	        const SymbolInfoPtr &info,
		const ExprNodePtr &returnedValue);

    virtual void	print (int indent) const;
    virtual bool	pathEndsWithReturn () const;

    SymbolInfoPtr	info;
    ExprNodePtr		returnedValue;
};


struct WhileNode: public StatementNode
{
    WhileNode (int lineNumber,
	       const ExprNodePtr &condition,
	       const StatementNodePtr &loopBody);

    virtual void	print (int indent) const;
    virtual bool	pathEndsWithReturn () const;

    ExprNodePtr		condition;
    StatementNodePtr	loopBody;
};


struct ExprNode: public SyntaxNode
{
    //----------------------------------------------------
    // class ExprNode is the base class for all node types
    // that represent parts of a CTL expression.
    //----------------------------------------------------
    
    ExprNode (int lineNumber);


    //--------------------------------------------------------
    // Compute the type of the value that will be generated
    // by evaluating this expression.  If initInfo is non-null,
    // then this expression is expected to initialize the
    // object to which initInfo refers via a side effect,
    // and writing to the object is allowed even if the
    // object is marked read-only.
    //--------------------------------------------------------

    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0) = 0;

    //------------------------------------------------------
    // Simplifiy this expression by evaluating any constant
    // subexpressions.  Return a pointer to the root node of
    // the simplified expression.  (The returned pointer may
    // point to this expression.)
    //------------------------------------------------------

    virtual ExprNodePtr	evaluate (LContext &lcontext) = 0;


    //-----------------------------------------------------
    // Test if this expression is represents an lvalue.
    // If initInfo is non-null then we are in the middle
    // of initializing the object to which initInfo refers,
    // and writing to the object is allowed, and the object
    // may be an lvalue, even if it is marked read-only.
    //-----------------------------------------------------

    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const = 0;


    TypePtr		type;
};


struct BinaryOpNode: public ExprNode
{
    BinaryOpNode (int lineNumber,
	          Token op,
		  const ExprNodePtr &leftOperand,
		  const ExprNodePtr &rightOperand);

    virtual void	print (int indent) const;

    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);

    virtual ExprNodePtr	evaluate (LContext &lcontext);

    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;

    Token		op;
    ExprNodePtr		leftOperand;
    ExprNodePtr		rightOperand;
    TypePtr		operandType;
};


struct UnaryOpNode: public ExprNode
{
    UnaryOpNode (int lineNumber,
	         Token op,
		 const ExprNodePtr &operand);

    virtual void	print (int indent) const;

    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;

    Token		op;
    ExprNodePtr		operand;
};


struct ArrayIndexNode: public ExprNode
{
    ArrayIndexNode (int lineNumber,
		    const ExprNodePtr &array,
		    const ExprNodePtr &index);

    virtual void	print (int indent) const;
    
    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;
    
    ExprNodePtr		array;
    ExprNodePtr		index;
};

struct MemberNode: public ExprNode
{
    MemberNode (int lineNumber,
		const ExprNodePtr &obj,
		const std::string &member);

    virtual void	print (int indent) const;
    
    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;
    
    ExprNodePtr		obj;
    std::string          member;
    size_t              offset;
};


struct SizeNode: public ExprNode
{
    SizeNode (int lineNumber,
	      const ExprNodePtr &obj);

    virtual void	print (int indent) const;
    
    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;
    
    ExprNodePtr		obj;
};

struct NameNode: public ExprNode
{
    NameNode (int lineNumber,
	      const std::string &name,
	      const SymbolInfoPtr &info);

    virtual void	print (int indent) const;
    
    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;

    std::string		name;
    SymbolInfoPtr	info;
};


struct LiteralNode: public ExprNode
{
    //----------------------------------------
    // base class for all nodes that represent
    // literals in a CTL program
    //----------------------------------------

    LiteralNode (int lineNumber);

    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;
    
    virtual char*       valuePtr() = 0;
    virtual void	printLiteral () const = 0;

};


struct BoolLiteralNode: public LiteralNode
{
    BoolLiteralNode (int lineNumber, const LContext &lcontext, bool value);

    virtual void	print (int indent) const;
    virtual void	printLiteral () const;

    bool		value;
};


struct IntLiteralNode: public LiteralNode
{
    IntLiteralNode (int lineNumber, const LContext &lcontext, int value);

    virtual void	print (int indent) const;
    virtual void	printLiteral () const;

    int			value;
};


struct UIntLiteralNode: public LiteralNode
{
    UIntLiteralNode (int lineNumber, const LContext &lcontext, unsigned value);

    virtual void	print (int indent) const;
    virtual void	printLiteral () const;

    unsigned int	value;
};


struct HalfLiteralNode: public LiteralNode
{
    HalfLiteralNode (int lineNumber, const LContext &lcontext, half value);

    virtual void	print (int indent) const;
    virtual void	printLiteral () const;

    half		value;
};


struct FloatLiteralNode: public LiteralNode
{
    FloatLiteralNode (int lineNumber, const LContext &lcontext, float value);

    virtual void	print (int indent) const;
    virtual void	printLiteral () const;

    float		value;
};


struct StringLiteralNode: public LiteralNode
{
    StringLiteralNode (int lineNumber,
		       const LContext &lcontext,
		       const std::string &value);

    virtual void	print (int indent) const;
    virtual void	printLiteral () const;

    std::string		value;
};


typedef std::vector <ExprNodePtr> ExprNodeVector;
typedef std::vector <ExprNodePtr>::iterator ExprNodeVectorIterator;

struct CallNode: public ExprNode
{
    CallNode (int lineNumber,
	      const NameNodePtr &function,
	      const ExprNodeVector &arguments);

    virtual void	print (int indent) const;
    
    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);
				     
    virtual ExprNodePtr	evaluate (LContext &lcontext);
    
    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;

    NameNodePtr		function;
    ExprNodeVector	arguments;
};



struct ValueNode: public ExprNode
{
    //------------------------------------------------------------
    //  class ValueNode represents a set of expressions for the initial
    //  values of the elements of a struct or array.  (In CTL an
    //  ValueNode corresponds to an struct initializer, for example 
    //  {1, 3, x/4, 2}.)
    //------------------------------------------------------------

    ValueNode (int lineNumber, const ExprNodeVector &elements);

    virtual void	print (int indent) const;


    //----------------------------------------------------------
    // Compute the types of the values that will be generated by
    // evalutating the expressions for the struct elements.
    //----------------------------------------------------------

    virtual void	computeType (LContext &lcontext,
				     const SymbolInfoPtr &initInfo = 0);

    //---------------------------------------------------
    // Simplify the expressions for the struct elements by
    // evaluating any constant subexpressions.
    //---------------------------------------------------

    virtual ExprNodePtr	evaluate (LContext &lcontext);


    //------------------------------------------------------------
    // Verify that the values of the expressions for the struct
    // elements can be converted to the type struct's element type.
    //------------------------------------------------------------

    bool		checkElementTypes (const DataTypePtr &dataType,
					   LContext &lcontext) const;

    bool		checkElementTypesRec (const DataTypePtr &dataType,
					      LContext &lcontext,
					      int &eIndex) const;

    //-----------------------------------------------------------------
    // Check if the expressions for the struct elements are all literals
    //-----------------------------------------------------------------

    bool		elementsAreLiterals () const;


    virtual bool	isLvalue (const SymbolInfoPtr &initInfo = 0) const;


    ExprNodeVector	elements;
};


} // namespace Ctl

#endif
