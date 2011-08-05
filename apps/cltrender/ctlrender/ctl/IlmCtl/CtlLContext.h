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


#ifndef INCLUDED_CTL_L_CONTEXT_H
#define INCLUDED_CTL_L_CONTEXT_H

//-----------------------------------------------------------------------------
//
//	class LContext
//
//	An LContext ("load-time context") keeps track of information
//	that is important while a CTL module is being loaded, such
//	as a pointer to the module that is being constructed or whether
//	any errors have been encountered.  (In order to give good
//	feedback to the user, module loading continues even if errors
//	are found, but a module with errors is unloaded and discarded
//	immediately after loading.)
//
//	The LContext also serves as a factory for syntax tree nodes
//	and type objects.
//
//-----------------------------------------------------------------------------

#include <CtlType.h>
#include <CtlSyntaxTree.h>
#include <CtlErrors.h>
#include <string>
#include <set>

namespace Ctl {

class Module;
class SymbolTable;


class LContext
{
  public:

    virtual ~LContext ();

    //----------------------------------------------------------
    // Access to the file that contains the module's source code
    //----------------------------------------------------------

    std::istream &	file ()			{return _file;}
    const std::string &	fileName () const;

    //------------------------------------
    // Access to the module we are loading
    //------------------------------------

    Module *		module ()		{return _module;}
    const Module *	module () const		{return _module;}


    //---------------------------
    // Access to the symbol table
    //---------------------------

    SymbolTable &	symtab ()		{return _symtab;}
    const SymbolTable &	symtab () const		{return _symtab;}


    //---------------------------------------------------------
    // Error counter, used to keep track of whether errors were
    // encountered while attempting to load the module
    //---------------------------------------------------------

    void		foundError (int lineNumber, Error e);
    void                declareError (int lineNumber, Error e);
    bool                errorDeclared (int lineNumber, Error e);
    void                catchErrors ();
    int 		numErrors () const;
    void                printDeclaredErrors() const;


    //--------------------------------------------------------
    // Methods called by the parser to generate data addresses
    //--------------------------------------------------------

    virtual void	newStackFrame () = 0;
    
    virtual AddrPtr	parameterAddr (const DataTypePtr &parameterType) = 0;
    virtual AddrPtr	returnValueAddr (const DataTypePtr &returnType) = 0;
    virtual AddrPtr	autoVariableAddr (const DataTypePtr &variableType) = 0;


    //-----------------------------------------------
    // Factory for syntax tree nodes and type objects
    //-----------------------------------------------
    
    virtual ModuleNodePtr	newModuleNode
				    (int lineNumber,
				     const StatementNodePtr &constants,
				     const FunctionNodePtr &functions)
				     const = 0;

    virtual FunctionNodePtr	newFunctionNode
				    (int lineNumber,
				     const std::string &name,
				     const SymbolInfoPtr &info,
				     const StatementNodePtr &body)
				     const = 0;

    virtual VariableNodePtr	newVariableNode
				    (int lineNumber,
				     const std::string &name,
				     const SymbolInfoPtr &info,
				     const ExprNodePtr &initialValue,
				     bool assignInitialValue)
				     const = 0;

    virtual AssignmentNodePtr	newAssignmentNode
				    (int lineNumber,
				     const ExprNodePtr &lhs,
				     const ExprNodePtr &rhs)
				     const = 0;

    virtual ExprStatementNodePtr newExprStatementNode
				    (int lineNumber,
				     const ExprNodePtr &expr)
				     const = 0;

    virtual IfNodePtr		newIfNode
				    (int lineNumber,
				     const ExprNodePtr &condition,
				     const StatementNodePtr &truePath,
				     const StatementNodePtr &falsePath)
				     const = 0;

    virtual ReturnNodePtr	newReturnNode
				    (int lineNumber,
				     const SymbolInfoPtr &info,
				     const ExprNodePtr &returnedValue)
				     const = 0;

    virtual WhileNodePtr	newWhileNode
				    (int lineNumber,
				     const ExprNodePtr &condition,
				     const StatementNodePtr &loopBody)
				     const = 0;

    virtual BinaryOpNodePtr	newBinaryOpNode
				    (int lineNumber,
				     Token op,
				     const ExprNodePtr &leftOperand,
				     const ExprNodePtr &rightOperand)
				     const = 0;

    virtual UnaryOpNodePtr	newUnaryOpNode
				    (int lineNumber,
				     Token op,
				     const ExprNodePtr &operand)
				     const = 0;

    virtual ArrayIndexNodePtr	newArrayIndexNode
				    (int lineNumber,
				     const ExprNodePtr &array,
				     const ExprNodePtr &index)
				     const = 0;

    virtual SizeNodePtr      	newSizeNode
				    (int lineNumber,
				     const ExprNodePtr &obj)
				     const = 0;

    virtual MemberNodePtr	newMemberNode
				    (int lineNumber,
				     const ExprNodePtr &obj,
				     const std::string &member)
				     const = 0;

    virtual NameNodePtr		newNameNode
				    (int lineNumber,
				     const std::string &name,
				     const SymbolInfoPtr &info)
				     const = 0;

    virtual BoolLiteralNodePtr	newBoolLiteralNode
				    (int lineNumber, bool value) const = 0;

    virtual IntLiteralNodePtr	newIntLiteralNode
				    (int lineNumber, int value) const = 0;

    virtual UIntLiteralNodePtr	newUIntLiteralNode
				    (int lineNumber, unsigned value) const = 0;

    virtual HalfLiteralNodePtr	newHalfLiteralNode
				    (int lineNumber, half value) const = 0;

    virtual FloatLiteralNodePtr	newFloatLiteralNode
				    (int lineNumber, float value) const = 0;

    virtual StringLiteralNodePtr newStringLiteralNode
				    (int lineNumber,
				     const std::string &value)
				     const = 0;

    virtual CallNodePtr		newCallNode
				    (int lineNumber,
				     const NameNodePtr &function,
				     const ExprNodeVector &arguments)
				     const = 0;

    virtual ValueNodePtr	newValueNode
				    (int lineNumber,
				     const ExprNodeVector &elements)
				     const = 0;

    virtual VoidTypePtr		newVoidType () const = 0;

    virtual BoolTypePtr		newBoolType () const = 0;

    virtual IntTypePtr		newIntType () const = 0;
    
    virtual UIntTypePtr		newUIntType () const = 0;
    
    virtual HalfTypePtr		newHalfType () const = 0;
    
    virtual FloatTypePtr	newFloatType () const = 0;

    virtual StringTypePtr	newStringType () const = 0;

    enum ArrayTypeUsage { PARAMETER, NON_PARAMETER };
    virtual ArrayTypePtr	newArrayType
				    (const DataTypePtr &baseType,
				     int size,
				     ArrayTypeUsage usage = NON_PARAMETER) = 0;

    // Create a multidimensional array
    virtual ArrayTypePtr	newArrayType
				    (const DataTypePtr &baseType,
				     SizeVector sizes,
				     ArrayTypeUsage usage = NON_PARAMETER);

    virtual StructTypePtr	newStructType
				    (const std::string &name,
				     const MemberVector &members) const = 0;

    virtual FunctionTypePtr	newFunctionType
				    (const DataTypePtr &returnType,
				     bool returnVarying,
				     const ParamVector &parameters) const = 0;
  protected:

    LContext (std::istream &file,
	      Module *module,
	      SymbolTable &symtab);

  private:

    std::istream &	_file;
    Module *		_module;
    SymbolTable &	_symtab;

    std::set<LineError> _lineErrors;
    std::set<LineError> _declErrors;
    
};


} // namespace Ctl

#endif
