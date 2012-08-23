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
//	The parser for the color transformation language.
//
//-----------------------------------------------------------------------------

#include <CtlParser.h>
#include <CtlInterpreter.h>
#include <CtlLContext.h>
#include <CtlSymbolTable.h>
#include <CtlMessage.h>
#include <CtlVersion.h>
#include <Iex.h>
#include <cassert>


using namespace std;

#if 0
    #include <iostream>
    #define debug(x) (cout << x << endl)
    #define debug_only(x) x
#else
    #define debug(x)
    #define debug_only(x)
#endif

#if 0

    #include <iostream>
    #include <iomanip>

    namespace
    {
	int nestingLevel = -1;

	struct NestingLevel
	{
	      NestingLevel () {++nestingLevel;}
	     ~NestingLevel () {--nestingLevel;}
	};
    };

    #define debugSyntax(x) \
	NestingLevel n; cout << setw (nestingLevel) << "" << x << endl

    #define debugSyntax1(x) \
	cout << setw (nestingLevel + 1) << "" << x << endl
#else

    #define debugSyntax(x)
    #define debugSyntax1(x)

#endif

namespace Ctl {
namespace {


template <class Ptr>
void
appendToList (Ptr &head, Ptr &tail, Ptr newElement)
{
    if (tail)
	tail->next = newElement;
    else
	head = newElement;

    while (newElement->next)
	newElement = newElement->next;

    tail = newElement;
}


template <class Ptr>
Ptr
findTailOfList (const Ptr &head)
{
    Ptr tail = head;

    while (tail && tail->next)
	tail = tail->next;

    return tail;
}


} // namespace


Parser::Parser (LContext &lcontext, Interpreter &interpreter):
    _lex (lcontext),
    _lcontext (lcontext),
    _interpreter (interpreter),
    _firstConst (0),
    _lastConst (0)
{
    debug ("Parser::Parser "
	   "(this = " << this << ", "
	   "lcontext = " << &lcontext << ", "
	   "interpreter = " << &interpreter << ")");
}


Parser::~Parser ()
{
    // empty
}


SyntaxNodePtr
Parser::parseInput ()
{
    debug ("Parser::parseInput (this = " << this << ")");

    try
    {
	parseCtlVersion();
	parseImportList();
	SyntaxNodePtr syntaxTree = parseModuleBody();

	debug_only (if (syntaxTree) syntaxTree->printTree());

	_lcontext.catchErrors();
	return syntaxTree;
    }
    catch (const exception &e)
    {
	outputMessage (e.what());
	return 0;
    }
}


void
Parser::parseCtlVersion ()
{
    debugSyntax ("ctlVersion");

    if (token() == TK_CTLVERSION)
    {
	next();

	match (TK_INTLITERAL);
	int ctlVersion = tokenIntValue();

	debugSyntax1 ("ctlversion " << ctlVersion);

	if (ctlVersion > CURRENT_CTL_VERSION)
	{
	    MESSAGE_LW (_lcontext, ERR_CTL_VERSION, currentLineNumber(),
			"Module requires CTL "
			"version " << ctlVersion << ", "
			"interpreter implements "
			"version " << CURRENT_CTL_VERSION << ".");
	}

	next();

	match (TK_SEMICOLON);
	next();
    }
}


void
Parser::parseImportList ()
{
    debugSyntax ("importList");

    while (token() == TK_IMPORT)
    {
	next();

	match (TK_STRINGLITERAL);
	string moduleName = tokenStringValue();
	next();

	match (TK_SEMICOLON);
	next();

	debugSyntax1 ("import " << moduleName);
	loadModuleRecursive (*this, moduleName);
    }
}


SyntaxNodePtr
Parser::parseModuleBody ()
{
    debugSyntax ("moduleBody");

    bool ns = false;
    if( token() == TK_NAMESPACE)
    {
	next();
	match (TK_NAME);

	debugSyntax1 ("namespace " << tokenStringValue());
	symtab().setGlobalNamespace (tokenStringValue());
	

	next();
	match (TK_OPENBRACE);
	next();
	ns = true;
    }
    else
    {
	symtab().setGlobalNamespace ("");
    }

    SyntaxNodePtr syntaxTree = parseFunctionOrConstList();

    if(ns)
    {
	match (TK_CLOSEBRACE);
	next();
    }

    match (TK_END);

    return syntaxTree;
}


ModuleNodePtr
Parser::parseFunctionOrConstList ()
{
    debugSyntax ("functionOrConstList");

    FunctionNodePtr firstFunc = 0;
    FunctionNodePtr lastFunc = 0;

    while (token() != TK_CLOSEBRACE && token() != TK_END)
    {
	if (token() == TK_CONST)
	{
	    if (StatementNodePtr constant = parseVariableDefinition(AM_STATIC))
		appendToList (_firstConst, _lastConst, constant);
	}
	else if(token() == TK_STRUCT)
	{
	    parseStructDefinition(AM_AUTO);
	}
	else
	{
	    if (FunctionNodePtr func = parseFunction())
		appendToList (firstFunc, lastFunc, func);
	}
    }

    return _lcontext.newModuleNode (currentLineNumber(), 
				    _firstConst, 
				    firstFunc);
}


FunctionNodePtr
Parser::parseFunction ()
{
    debugSyntax ("function");

    //
    // Parse return type
    //

    bool returnVarying = parseVaryingHint();

    VoidTypePtr voidType = _lcontext.newVoidType();
    DataTypePtr returnType = voidType;

    if (token() == TK_VOID)
    {
	next();
    }
    else
    {
	DataTypePtr baseType = parseBaseType();
	SizeVector arraySizes;
	parseArraySize(arraySizes);

	for(int i = 0; i < (int)arraySizes.size(); i++ )
	{
	    if(arraySizes[i] == 0)
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_ARR_LEN, 
			    currentLineNumber(),
			    "Cannot use variable size array as return value.");
		
	    }
	}


	if (arraySizes.size() > 0)
	    returnType = _lcontext.newArrayType (baseType, arraySizes);
	else
	    returnType = baseType;
    }

    //
    // Parse the function's name, enter the name into the symbol table.
    //

    match (TK_NAME);
    string name = tokenStringValue();
    int lineNumber = currentLineNumber();
    debugSyntax1 ("name " << name);
    next();

    //
    // The current token must be an open parenthesis; if it is not,
    // the program most likely attempted to declare a static variable,
    // which is not allowed (CTL supports only static constants).
    //

    if (token() != TK_OPENPAREN)
    {
	staticVariableError();
	recover (TK_SEMICOLON);
	next();
	return 0;
    }

    SymbolInfoPtr info = new SymbolInfo (module(), RWA_WRITE);

    if (!symtab().defineSymbol (name, info))
	duplicateName (name, lineNumber, fileName());

    //
    // Create a local name space for the function's parameter list and body.
    // Start a new stack frame layout for this function.
    //

    LocalNamespace localNs (symtab());
    _lcontext.newStackFrame();

    //
    // Parse the function's parameter list.
    //

    ParamVector parameters;
    parseParameterList (parameters, name);

    //
    // Now that we know the function's signature,
    // store it in the symbol table.
    //

    info->setType (_lcontext.newFunctionType
			    (returnType, returnVarying, parameters));

    //
    // Allocate a stack frame location for the function's
    // return value and store it in the symbol table.
    //

    AddrPtr returnAddr = _lcontext.returnValueAddr (returnType);

    symtab().defineSymbol
	("$return", new SymbolInfo 
                       (module(), RWA_WRITE, false, returnType, returnAddr));

    //
    // Parse the function's body.
    //

    StatementNodePtr body = parseCompoundStatement();

    //
    // If the function's return type is not void, then every
    // path through the function must end in a return statement.
    // "Falling off the end" of the function is not allowed.
    // 

    if (returnType && 
	!returnType->isSameTypeAs (voidType) &&
	(!body || !body->pathEndsWithReturn()))
    {
	MESSAGE_PLE (_lex, _lcontext, ERR_RETURN_REQ, lineNumber,
	    "Non-void function can terminate without returning a value.");
    }

    return _lcontext.newFunctionNode (lineNumber, name, info, body);
}


void
Parser::parseParameterList (ParamVector &parameters, const string &funcName)
{
    debugSyntax ("parameterList");

    try
    {
	match (TK_OPENPAREN);
	next();

	bool foundDefaults = false;
	while (token() != TK_CLOSEPAREN && token() != TK_END)
	{
	    parseParameter (parameters, funcName, foundDefaults);

            if (token() == TK_COMMA)
            {
                next();
                if(token() == TK_CLOSEPAREN)
                {
                    MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
                                 "Extra comma.\n");
                }
            }
            else if(token() != TK_CLOSEPAREN)
            {
                MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
                             "Expected a comma.\n");
            }
	}

	match (TK_CLOSEPAREN);
	next();
    }
    catch (...)
    {
	recover (TK_OPENBRACE);
    }


}


void
Parser::parseParameter (ParamVector &parameters, 
			const string &funcName,
			bool &foundDefaults)
{
    debugSyntax ("parameter");

    try
    {
	//
	// Parse the parameter's type and name
	//

	ReadWriteAccess access = RWA_READ;

	switch (token())
	{
	  case TK_INPUT:
	    access = RWA_READ;
	    next();
	    break;

	  case TK_OUTPUT:
	    access = RWA_WRITE;
	    next();
	    break;

	  default:
	    break;
	}

	bool varying = parseVaryingHint();
	DataTypePtr baseType = parseBaseType ();

	match (TK_NAME);
	string name = tokenStringValue();
	debugSyntax1 ("name " << name);
	next();

	SizeVector declArraySizes;
	parseArraySize(declArraySizes);
	SizeVector defaultSizes = declArraySizes;

	DataTypePtr paramType = 0;

	if (declArraySizes.size() > 0)
	    paramType = _lcontext.newArrayType (baseType, declArraySizes, 
					   LContext::PARAMETER);
	else
	    paramType = baseType;

	//
	// Parse the parameter's default value
	//

	bool initError = false;
	ExprNodePtr defaultValue = 0;

	if (access == RWA_READ  && token() == TK_ASSIGN)
	{
	    foundDefaults = true;
	    next();
	
	    if (token() == TK_OPENBRACE)
	    {
		initError = !parseInitializer(defaultValue, 
					      paramType, 
					      defaultSizes);
		if( !initError )
		{
		    TypePtr defaultType;
		    if( declArraySizes.size() > 0)
			defaultType = _lcontext.newArrayType (baseType, 
							      defaultSizes);
		    else
			defaultType = baseType;
		    
		    ValueNodePtr value = defaultValue;
		    
		    if (value)
			value->computeType (_lcontext);

		    if (value 
			&& !value->checkElementTypes (baseType, _lcontext))
			value = 0;
		    
		    if( value )
		    {
			value->type = defaultType;
			value->evaluate(_lcontext);
		    }
		}
	    }
	    else
	    {
		defaultValue = parseExpression();
		if (defaultValue)
		{
		    defaultValue->computeType (_lcontext);

		    if (paramType && defaultValue->type &&
			!paramType->canCastFrom (defaultValue->type))
		    {
			MESSAGE_PLE (_lex, _lcontext, ERR_FUNC_PARAM, 
				   defaultValue->lineNumber,
				   "Cannot convert default value for function "
				   "parameter " << name << " from type " <<
				   defaultValue->type->asString() << " to type "
				   <<  paramType->asString() << ".");

			initError = true;
		    }
		    else
		    {
			defaultValue = evaluateExpression (defaultValue, 
							   paramType);
		    }
		}
	    }
	}
	else
	{
	    if(foundDefaults)
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_DEFAULT_ORDER, 
			     currentLineNumber(),
			   "Parameters with defaults must appear at the end"
			   "of the parameter list.");
	    }
	    if( token() == TK_ASSIGN )
	    {
		next();
		MESSAGE_PLE (_lex, _lcontext, ERR_DEFAULT_WRITE, 
			    currentLineNumber(),
			    "Cannot set a default value for an output "
			    "parameter  (parameter " << name << ").");

		// now throw away everything
		if(token() == TK_OPENBRACE)
		{
		    initError = !parseInitializer(defaultValue, 
						  paramType, 
						  defaultSizes);
		}
		else
		{
		    parseExpression();
		}
		initError = true;
	    }
	}
	if( defaultValue && defaultValue->type )
	{
	    //
	    // create a static variable for the default value
	    // so that it can be used easily in c++ func calls
	    DataTypePtr defaultType = defaultValue->type;

	    AddrPtr addr = defaultType->newStaticVariable (_lcontext.module());

	    SymbolInfoPtr info = new SymbolInfo (module(), RWA_READ, false, 
				defaultType, addr);

	    info->setValue(defaultValue);
	    string paramName = 
		symtab().getGlobalNamespace() + "::" 
		+  funcName + "$" + name;
	    if (!symtab().defineSymbol (paramName, info))
	    {
		duplicateName (name, currentLineNumber(), fileName());
	    }


	    // create default value initialization code
	    StatementNodePtr constant = 
		_lcontext.newVariableNode(defaultValue->lineNumber,
					  paramName, info,
					  defaultValue, true);
	    appendToList(_firstConst, _lastConst, constant);


	    // replace default value with a reference to the new
	    // static variable
	    defaultValue =     
		_lcontext.newNameNode (defaultValue->lineNumber, 
				       paramName, 
				       info);
	    defaultValue->computeType (_lcontext);
	} 
	if( initError )
	{
	    defaultValue = 0;
	}


	//
	// Add the parameter's type and default value to the function's
	// parameter list; enter the parameter's name, type and frame-
	// pointer relative address into the symbol table.
	//

	parameters.push_back
	    (Param (name, paramType, defaultValue, access, varying));

	AddrPtr paramAddr = _lcontext.parameterAddr (paramType);

	if (!symtab().defineSymbol
	      (name, new SymbolInfo (module(), access, false, 
				     paramType, paramAddr)))
	{
	    duplicateName (name, currentLineNumber(), fileName());
	}
    }
    catch (...)
    {
	recover (TK_CLOSEPAREN);
    }
}


StatementNodePtr
Parser::parseCompoundStatement ()
{
    debugSyntax ("compoundStatement");

    try
    {
	LocalNamespace localNs (symtab());

	match (TK_OPENBRACE);
	next();

	StatementNodePtr firstStmt = 0;
	StatementNodePtr lastStmt = 0;
	bool discard = false;

	while (token() != TK_CLOSEBRACE && token() != TK_END)
	{
	    StatementNodePtr stmt = parseStatement();

	    if (stmt && !discard)
	    {
		appendToList (firstStmt, lastStmt, stmt);

		if (stmt.cast<ReturnNode>())
		{
		    //
		    // Discard statements following a return
		    // statement; they will never be executed.
		    //

		    discard = true;
		}
	    }
	}

	match (TK_CLOSEBRACE);
	next();

	return firstStmt;
    }
    catch (...)
    {
	recover (TK_CLOSEBRACE);
	next();
	return 0;
    }
}


StatementNodePtr
Parser::parseStatement ()
{
    debugSyntax ("statement");

    try
    {
	switch (token())
	{
	  case TK_STRUCT:
	    return parseStructDefinition(AM_AUTO);

	  case TK_OPENBRACE:
	    return parseCompoundStatement();

	  case TK_FOR:
	    return parseForStatement();

	  case TK_IF:
	    return parseIfStatement();

	  case TK_PRINT:
	    return parsePrintStatement();

	  case TK_RETURN:
	    return parseReturnStatement();

	  case TK_WHILE:
	    return parseWhileStatement();

	  case TK_SEMICOLON:
	    return parseNullStatement();

	  default:
	      return parseExprVariableDefinitionOrAssign();
	}
    }
    catch (...)
    {
	recover (TK_SEMICOLON);
	next();
	return 0;
    }
}


StatementNodePtr
Parser::parseVariableDefinition (AllocationMode mode, 
				  DataTypePtr baseType /* = 0 */)
{
    debugSyntax ("variableDefinition");

    //
    // Parse the variable's type and name
    //

    bool isConst = false;
    int lineNumber = currentLineNumber();

    if (!baseType)
    {
	if (token() == TK_CONST)
	{
	    isConst = true;
	    next();
	}
	
	baseType = parseBaseType();
    }

    if (!baseType)
	return 0;

    match (TK_NAME);
    string name = tokenStringValue();
    debugSyntax1 ("name " << name);
    next();

    SizeVector declArraySizes;
    parseArraySize (declArraySizes);

    //
    // Parse the variable's initial value.
    //

    VariableNodePtr node = 0;

    if (token() == TK_ASSIGN)
    {
	next();

#if 0
	if (token() == TK_IMPORT)
	{
	    //
	    // "int x[] = import(...);"
	    //
	    
	    DataTypePtr type = baseType;

	    if (declArraySizes.size() > 0)
		type = _lcontext.newArrayType (baseType, declArraySizes);
	    
	    ExprNodePtr initialValue = 0;

	    if (parseInitializer (initialValue, type, declArraySizes))
	    {
		node = variableDefinitionImport (mode,
						 lineNumber,
						 name,
						 isConst,
		                                 baseType,
		                                 declArraySizes,
						 initialValue);
	    }
	}
	else
#endif
	if (token() == TK_OPENBRACE)
	{
	    //
	    // "int x[] = {1, 2, 3};"
	    //
	    
	    DataTypePtr type = baseType;

	    if (declArraySizes.size() > 0)
		type = _lcontext.newArrayType (baseType, declArraySizes);
	    
	    ExprNodePtr initialValue = 0;

	    if (parseInitializer (initialValue, type, declArraySizes))
	    {
		node = variableDefinitionCurlyBraces (mode,
						       lineNumber,
						       name,
						       isConst,
						       baseType,
						       declArraySizes,
						       initialValue);
	    }
	}
	else
	{
	    //
	    // "int x = y + 2;"
	    //

	    ExprNodePtr initialValue = parseExpression();

	    node = variableDefinitionAssignExpr (mode,
						  lineNumber,
						  name,
						  isConst,
						  baseType,
						  declArraySizes,
						  initialValue);
	}
    }
    else if (token() == TK_COMMA)
    {
	//
	// "int x[3], f(x);"
	//
	// Initial value for the new variable is generated by calling
	// a function, passing the variable as an output parameter.
	// The called function is expected to assign a value.
	//

	next();

	node = variableDefinitionExprSideEffect (mode,
						  lineNumber,
						  name,
						  isConst,
						  baseType,
						  declArraySizes);

	ExprNodePtr initialValue = parseExpression();

	if (node && initialValue)
	{
	    initialValue->computeType (_lcontext, node->info);
	    initialValue = evaluateExpression (initialValue, 0);

	}

	if (node)
	    node->initialValue = initialValue;
    }
    else
    {
	//
	// No initial value for the new variable is given.
	//

	if (isConst)
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_CONST, lineNumber,
		"No value given for constant " << name << ".");
	}
	else
	{
	    node = variableDefinitionNoInit
		       (mode, lineNumber, name, baseType, declArraySizes);
	}
    }

    match (TK_SEMICOLON);
    next();

    return node;
}


StatementNodePtr
Parser::parseStructDefinition (AllocationMode mode)
{
    next();  // TK_STRUCT
    string structName = "";
    debugSyntax ("structDefinition");

    int lineNumber = currentLineNumber();

    match(TK_NAME);

    structName = symtab().getAbsoluteName(tokenStringValue());
    debugSyntax1 ("name " << structName);
    next();

    match( TK_OPENBRACE );
    next();

    MemberVector members;

    while( token() != TK_CLOSEBRACE  && token() != TK_END)
    {
	if (token() == TK_CONST)
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_STRUCT_CONST,
			 currentLineNumber(),
			"Cannot have a const member variable.");
	    next();
	}

	DataTypePtr baseType = parseBaseType ();

	match (TK_NAME);
	string name = tokenStringValue();
	debugSyntax1 ("name " << name);
	next();

	if( name == "size" )
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_MEMBER_NAME, currentLineNumber(),
			"Member name \"size\" is reserved."
			<< name);
	}


	SizeVector declArraySizes;
	parseArraySize(declArraySizes);

	for( int i = 0; i < (int)declArraySizes.size(); i++)
	{
	    if(declArraySizes[i] == 0)
		MESSAGE_PLE (_lex, _lcontext, ERR_STRUCT_ARR_LEN, 
			     currentLineNumber(),
			    "Struct member array size must be specified: " 
			    << name);
	}

	match (TK_SEMICOLON);
	next();

	DataTypePtr memberType = 0;
	if (declArraySizes.size() > 0)
	    memberType = _lcontext.newArrayType (baseType, declArraySizes);
	else
	    memberType = baseType;

	// create new memeber of this type
	members.push_back(Member (name, memberType));
    }
    
    StructTypePtr stype = _lcontext.newStructType(structName, members);

    // create new type name
    SymbolInfoPtr info = new SymbolInfo (module(), RWA_NONE, true, stype);

    if (!symtab().defineSymbol (structName, info))
	duplicateName (structName, lineNumber, fileName());

    next();
    match( TK_SEMICOLON );
    next();
    return 0;
}


StatementNodePtr
Parser::parseExprVariableDefinitionOrAssign()
{
    debugSyntax ("exprVariableDefinitionOrAssign");
    
    //
    // This is either a variable definition, an expression
    // statement or an assignment.
    //

    if (token() == TK_CONST ||
        token() == TK_BOOL ||
	token() == TK_INT ||
	token() == TK_UNSIGNED ||
	token() == TK_HALF ||
	token() == TK_FLOAT)
    {
	return parseVariableDefinition (AM_AUTO);
    }

    ExprNodePtr lhs = parseExpression();
    if( token() == TK_NAME )
    {
	NameNodePtr name = lhs.cast<NameNode>();
	DataTypePtr dataType;
	if(!name || !name->info->isTypeName() || !name->info->type())
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_UNKNOWN_TYPE, lhs->lineNumber,
			"Definition with unknown type: " << name->name 
			<< "\n");
	    MemberVector members;
	    dataType = _lcontext.newStructType("",members);
	}
	else
	    dataType = name->info->type();

	return parseVariableDefinition(AM_AUTO, dataType);
    }

    if( token() == TK_ASSIGN )
	return parseAssignment(lhs);

    return parseExprStatement(lhs);
}


StatementNodePtr
Parser::parseAssignment (ExprNodePtr lhs)
{
    debugSyntax ("assignment");
    StatementNodePtr statement = parseSimpleAssignment (lhs);
    match (TK_SEMICOLON);
    next();
    return statement;
}


StatementNodePtr
Parser::parseSimpleAssignment (ExprNodePtr lhs)
{
    debugSyntax ("simpleAssignment");

    //
    // Parse the right-hand side of the assignment.
    //

    match (TK_ASSIGN);
    next();
    ExprNodePtr rhs = parseExpression();

    //
    // Type-check and pre-evaluate the left and right-hand side expressions.
    //

    lhs->computeType (_lcontext);
    rhs->computeType (_lcontext);

    lhs = evaluateExpression (lhs, 0);
    rhs = evaluateExpression (rhs, lhs->type);

    //
    // Verify that the left-hand side of the assignment is an lvalue.
    // 

    if (!lhs->isLvalue())
    {
	MESSAGE_PLE (_lex, _lcontext, ERR_LVAL, lhs->lineNumber,
	    "Left-hand side of assignment is not an lvalue.");

	lhs->type = 0;
	return 0;
    }

    //
    // Verify that the right can be casted to the left
    // 
    else if(lhs->type && rhs->type  && !lhs->type->canAssign(rhs->type))
    {
	MESSAGE_PLE (_lex, _lcontext, ERR_ASSIGN_CONV,  rhs->lineNumber,
		    "Cannot convert assignment value of type " <<
		    rhs->type->asString() << " to type " <<
		    lhs->type->asString() << ".");
	
	lhs->type = 0;
	return 0;
    }


    //
    // Create a syntax tree node for the assignment.
    //
    return _lcontext.newAssignmentNode (lhs->lineNumber, lhs, rhs);
}


StatementNodePtr
Parser::parseExprStatement (ExprNodePtr expr)
{
    debugSyntax ("exprStatement");
    StatementNodePtr statement = parseSimpleExprStatement (expr);
    match (TK_SEMICOLON);
    next();
    return statement;
}


StatementNodePtr
Parser::parseSimpleExprStatement (ExprNodePtr expr)
{
    debugSyntax ("simpleExprStatement");

    //
    // Compute the type of the expression.
    //

    expr->computeType (_lcontext);
    expr = evaluateExpression (expr, 0);

    //
    // Create a syntax tree node for the expression statement.
    //

    return _lcontext.newExprStatementNode (expr->lineNumber, expr);
}


StatementNodePtr
Parser::parseForStatement()
{
    debugSyntax ("forStatement");

    //
    // Parse the condition for the for statement.
    //

    LocalNamespace localNs (symtab());

    match (TK_FOR);
    next();

    match (TK_OPENPAREN);
    next();

    debugSyntax1 ("initialization");
    StatementNodePtr init = parseExprVariableDefinitionOrAssign();

    debugSyntax1 ("condition");
    ExprNodePtr condition = parseExpression();
    match (TK_SEMICOLON);
    next();

    debugSyntax1 ("update");
    StatementNodePtr update = parseForUpdateStatement();

    match (TK_CLOSEPAREN);
    next();

    //
    // Verify that the condition can be converted to a bool value.
    //

    condition->computeType (_lcontext);

    if (condition->type)
    {
	BoolTypePtr boolType = _lcontext.newBoolType ();

	if (!boolType->canCastFrom (condition->type))
	{
	    MESSAGE_LE (_lcontext, ERR_WHILE_CONV, condition->lineNumber,
		"Cannot convert condition for 'for' statement to type bool.");

	    condition->type = 0;
	}

	condition = evaluateExpression (condition, boolType);
    }

    //
    // Parse the body of the for statement
    //

    debugSyntax1 ("loop body");
    StatementNodePtr loopBody = parseStatement();

    //
    // If the condition can be evaluated at compile time, and it is
    // false, then we can discard the condition and the loop body.
    // We keep only the loop initialization.
    //

    if (BoolLiteralNodePtr constCondition = condition.cast<BoolLiteralNode>())
    {
	if (!constCondition->value)
	    return init;
    }

    //
    // If we didn't discarded the loop body, and the loop body does not
    // end in a return statement, then we append the condition update
    // statement to the end of the loop body.
    //

    StatementNodePtr loopBodyTail = findTailOfList (loopBody);

    if (!loopBodyTail.cast<ReturnNode>())
	appendToList (loopBody, loopBodyTail, update);

    //
    // Generate the loop and append it to the initialization.
    //

    StatementNodePtr loop =
	_lcontext.newWhileNode (condition->lineNumber, condition, loopBody);

    StatementNodePtr initTail = findTailOfList (init);
    appendToList (init, initTail, loop);
    return init;
}


StatementNodePtr
Parser::parseForUpdateStatement()
{
    debugSyntax ("forUpdateStatement");

    ExprNodePtr lhs = parseExpression();

    if( token() == TK_ASSIGN )
	return parseSimpleAssignment(lhs);

    return parseSimpleExprStatement(lhs);
}


StatementNodePtr
Parser::parseIfStatement()
{
    debugSyntax ("ifStatement");

    //
    // Parse the condition of the if statement
    //

    match (TK_IF);
    next();

    debugSyntax1 ("condition");

    match (TK_OPENPAREN);
    next();
    ExprNodePtr condition = parseExpression();
    match (TK_CLOSEPAREN);
    next();
    
    //
    // Verify that the condition can be converted to a bool value.
    //

    condition->computeType (_lcontext);

    if (condition->type)
    {
	BoolTypePtr boolType = _lcontext.newBoolType ();

	if (!boolType->canCastFrom (condition->type))
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_IF_CONV, condition->lineNumber,
		"Cannot convert condition for 'if' statement to type bool.");

	    condition->type = 0;
	}

	condition = evaluateExpression (condition, boolType);
    }

    //
    // Parse the body of the if statement
    //

    debugSyntax1 ("true path");
    StatementNodePtr truePath = parseStatement();

    StatementNodePtr falsePath = 0;

    if (token() == TK_ELSE)
    {
	debugSyntax1 ("false path");
	next();
	falsePath = parseStatement();
    }

    //
    // If the condition can be evaluated at compile time,
    // then we can discard either the "true" or the "false" path.
    //

    if (BoolLiteralNodePtr constCondition = condition.cast<BoolLiteralNode>())
    {
	if (constCondition->value)
	    return truePath;
	else
	    return falsePath;
    }

    //
    // Create a syntax tree node for the if statement.
    //

    return _lcontext.newIfNode
		(condition->lineNumber, condition, truePath, falsePath);
}


StatementNodePtr
Parser::parsePrintStatement()
{
    debugSyntax ("printStatement");

    //
    // Parse the expression list for the print statement.
    //

    match (TK_PRINT);
    next();
    match (TK_OPENPAREN);
    next();
    ExprNodeVector exprs;
    parseExprList (exprs);
    match (TK_CLOSEPAREN);
    next();
    match (TK_SEMICOLON);
    next();

    //
    // Convert each of the expressions in the list into a function call.
    //

    StatementNodePtr firstStmt = 0;
    StatementNodePtr lastStmt = 0;

    for (int i = 0; i < (int)exprs.size(); ++i)
    {
	ExprNodePtr expr = exprs[i];
	expr->computeType (_lcontext);

	if (!expr->type)
	    continue;

	expr = evaluateExpression (expr, 0);

	const char *functionName = "";

	if (expr->type.cast<BoolType>())
	{
	    functionName = "::print_bool";
	}
	else if (expr->type.cast<IntType>())
	{
	    functionName = "::print_int";
	}
	else if (expr->type.cast<UIntType>())
	{
	    functionName = "::print_unsigned_int";
	}
	else if (expr->type.cast<HalfType>())
	{
	    functionName = "::print_half";
	}
	else if (expr->type.cast<FloatType>())
	{
	    functionName = "::print_float";
	}
	else if (expr->type.cast<StringType>())
	{
	    functionName = "::print_string";
	}
	else
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_PRINT_TYPE, expr->lineNumber,
			"Cannot print expression of "
			"type " << expr->type->asString() << ".");
	    continue;
	}

	SymbolInfoPtr info = symtab().lookupSymbol (functionName);

	if (!info)
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_PRINT_TYPE, expr->lineNumber,
			"Cannot print expression; "
			"function" << functionName << " is missing.");
	    continue;
	}

	ExprNodeVector arguments;
	arguments.push_back (expr);

	ExprNodePtr functionCall = _lcontext.newCallNode
	    (expr->lineNumber,
	     _lcontext.newNameNode (expr->lineNumber, functionName, info),
	     arguments);

	StatementNodePtr stmt = _lcontext.newExprStatementNode
	    (expr->lineNumber,
	     functionCall);

	appendToList (firstStmt, lastStmt, stmt);
    }

    return firstStmt;
}


StatementNodePtr
Parser::parseNullStatement()
{
    debugSyntax ("nullStatement");

    match (TK_SEMICOLON);
    next();

    return 0;
}


StatementNodePtr
Parser::parseReturnStatement()
{
    debugSyntax ("returnStatement");

    //
    // Parse the expression for the return value.
    //

    match (TK_RETURN);
    next();

    ExprNodePtr returnedValue = 0;

    if (token() == TK_SEMICOLON)
    {
	next();
    }
    else
    {
	returnedValue = parseExpression();
	match (TK_SEMICOLON);
	next();
    }

    //
    // Verify that the return value can be converted to the
    // return type of the function that contains the return
    // statement.
    //

    SymbolInfoPtr info = symtab().lookupSymbol("$return");
    DataTypePtr returnType = info->dataType();

    if (returnedValue == 0)
    {
	VoidTypePtr voidType = _lcontext.newVoidType();

	if (!voidType->isSameTypeAs (returnType))
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_RETURN_REQ, currentLineNumber(),
		"No value returned from non-void function.");
	}
    }
    else
    {
	returnedValue->computeType (_lcontext);

	if (returnedValue->type &&
	    !returnType->canAssign (returnedValue->type))
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_RETURN_REQ, 
			 returnedValue->lineNumber,
			 "Cannot convert return value of type " <<
			 returnedValue->type->asString() << " to type " <<
			 returnType->asString() << ".");

	    returnedValue->type = returnType;
	}

	returnedValue = evaluateExpression (returnedValue, returnType);
    }

    //
    // Create a syntax tree node for the return statement.
    //

    return _lcontext.newReturnNode (currentLineNumber(), info, returnedValue);
}


StatementNodePtr
Parser::parseWhileStatement()
{
    debugSyntax ("whileStatement");

    //
    // Parse the condition for the while statement.
    //

    match (TK_WHILE);
    next();

    debugSyntax1 ("condition");

    match (TK_OPENPAREN);
    next();
    ExprNodePtr condition = parseExpression();
    match (TK_CLOSEPAREN);
    next();

    //
    // Verify that the condition can be converted to a bool value.
    //

    condition->computeType (_lcontext);

    if (condition->type)
    {
	BoolTypePtr boolType = _lcontext.newBoolType ();

	if (!boolType->canCastFrom (condition->type))
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_WHILE_CONV, condition->lineNumber,
		"Cannot convert condition for 'while' statement to type bool.");

	    condition->type = 0;
	}

	condition = evaluateExpression (condition, boolType);
    }

    //
    // Parse the body of the while statement
    //

    debugSyntax1 ("loop body");
    StatementNodePtr loopBody = parseStatement();

    //
    // If the condition can be evaluated at compile time,
    // and it is false, then we can discard the entire loop.
    //

    if (BoolLiteralNodePtr constCondition = condition.cast<BoolLiteralNode>())
    {
	if (!constCondition->value)
	    return 0;
    }

    //
    // Create a syntax tree node for the while statement.
    //

    return _lcontext.newWhileNode (condition->lineNumber, condition, loopBody);
}


ExprNodePtr
Parser::parseExpression ()
{
    debugSyntax ("expression");
    return parseOrExpression();
}


ExprNodePtr
Parser::parseOrExpression ()
{
    debugSyntax ("orExpression");

    ExprNodePtr leftOperand = parseAndExpression();

    while (token() == TK_OR)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseAndExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseAndExpression ()
{
    debugSyntax ("andExpression");

    ExprNodePtr leftOperand = parseBitOrExpression();

    while (token() == TK_AND)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseBitOrExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseBitOrExpression ()
{
    debugSyntax ("bitOrExpression");

    ExprNodePtr leftOperand = parseBitXorExpression();

    while (token() == TK_BITOR)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseBitXorExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseBitXorExpression ()
{
    debugSyntax ("bitXorExpression");

    ExprNodePtr leftOperand = parseBitAndExpression();

    while (token() == TK_BITXOR)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseBitAndExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseBitAndExpression ()
{
    debugSyntax ("bitAndExpression");

    ExprNodePtr leftOperand = parseEqualityExpression();

    while (token() == TK_BITAND)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseEqualityExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseEqualityExpression ()
{
    debugSyntax ("equalityExpression");

    ExprNodePtr leftOperand = parseRelationalExpression();

    while (token() == TK_EQUAL || token() == TK_NOTEQUAL)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseRelationalExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseRelationalExpression ()
{
    debugSyntax ("relationalExpression");

    ExprNodePtr leftOperand = parseShiftExpression();

    while (token() == TK_LESS || token() == TK_GREATER ||
	   token() == TK_LESSEQUAL || token() == TK_GREATEREQUAL)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseShiftExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseShiftExpression ()
{
    debugSyntax ("shiftExpression");

    ExprNodePtr leftOperand = parseAdditiveExpression();

    while (token() == TK_LEFTSHIFT || token() == TK_RIGHTSHIFT)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseAdditiveExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseAdditiveExpression ()
{
    debugSyntax ("additiveExpression");

    ExprNodePtr leftOperand = parseMultiplicativeExpression();

    while (token() == TK_PLUS || token() == TK_MINUS)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseMultiplicativeExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseMultiplicativeExpression ()
{
    debugSyntax ("multiplicativeExpression");

    ExprNodePtr leftOperand = parseUnaryExpression();

    while (token() == TK_TIMES || token() == TK_DIV || token() == TK_MOD)
    {
	Token op = token();
	next();

	ExprNodePtr rightOperand = parseUnaryExpression();

	leftOperand = _lcontext.newBinaryOpNode
			  (currentLineNumber(), op, leftOperand, rightOperand);
    }

    return leftOperand;
}


ExprNodePtr
Parser::parseUnaryExpression ()
{
    debugSyntax ("unaryExpression");

    if (token() == TK_MINUS || token() == TK_BITNOT || token() == TK_NOT)
    {
	Token op = token();
	next();

	ExprNodePtr operand = parseUnaryExpression();

	return _lcontext.newUnaryOpNode (currentLineNumber(), op, operand);
    }

    return parsePrimaryExpression();
}


ExprNodePtr
Parser::parsePrimaryExpression ()
{
    debugSyntax ("primaryExpression");

    NameNodePtr name;

    if (token() == TK_TRUE)
    {
	debugSyntax1 ("true");
	next();
	return _lcontext.newBoolLiteralNode (currentLineNumber(), true);
    }

    if (token() == TK_FALSE)
    {
	debugSyntax1 ("false");
	next();
	return _lcontext.newBoolLiteralNode (currentLineNumber(), false);
    }

    if (token() == TK_INTLITERAL)
    {
	int value = tokenIntValue();
	debugSyntax1 ("int literal " << value);
	next();
	return _lcontext.newIntLiteralNode (currentLineNumber(), value);
    }

    if (token() == TK_FLOATLITERAL)
    {
	float value = tokenFloatValue();
	debugSyntax1 ("32-bit float literal " << value);
	next();
	return _lcontext.newFloatLiteralNode (currentLineNumber(), value);
    }

    if (token() == TK_HALFLITERAL)
    {
	float value = tokenFloatValue();
	debugSyntax1 ("16-bit float literal " << value);
	next();
	return _lcontext.newHalfLiteralNode (currentLineNumber(), value);
    }

    if (token() == TK_STRINGLITERAL)
    {
        // Adjacent string literals are treated as a single string literal
	string value = "";
        while(token() == TK_STRINGLITERAL)
        {
            value += tokenStringValue();
            next();
        }
        debugSyntax1 ("string literal " << value);
	return _lcontext.newStringLiteralNode (currentLineNumber(), value);
    }

    if (token() == TK_OPENPAREN)
    {
	next();
	ExprNodePtr expr = parseExpression();
	match (TK_CLOSEPAREN);
	next();

	return expr;
    }

    name = parseScopedName();
    ExprNodePtr lhs = name;

    if( name && name->info && name->info->isTypeName())
	return lhs;

    if (token() == TK_OPENPAREN)
    {
	debugSyntax1 ("function call");
	next();
	ExprNodeVector arguments;
	parseExprList (arguments);
	match (TK_CLOSEPAREN);
	next();
	
	lhs =  _lcontext.newCallNode (currentLineNumber(), name, arguments);
    }

    return parseMemberArrayExpression(lhs);
}



ExprNodePtr
Parser::parseMemberArrayExpression (ExprNodePtr lhs)
{
    debugSyntax ("MemberArrayExpression");

    while( token() == TK_DOT || token() == TK_OPENBRACKET)
    {
        if (token() == TK_DOT)
	{
	    debugSyntax1 ("struct member access");
	    next();
	    if( token() == TK_NAME)
	    {
		string member = tokenStringValue();
		if( member == "size")
		{
		    debugSyntax1 ("size " << member);
		    lhs = _lcontext.newSizeNode (currentLineNumber(), lhs);
		    next();
		}
		else
		{
		    debugSyntax1 ("member " << member);
		    lhs = _lcontext.newMemberNode (currentLineNumber(), 
						   lhs, member);
		    next();
		}
	    }
	    else
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_MEMBER_NAME, 
			   currentLineNumber(),
			   "Error parsing member name.");
	    }
	}
	else if (token() == TK_OPENBRACKET)
	{
	    debugSyntax1 ("index into array");
	    next();
	    ExprNodePtr index = parseExpression();
	    match (TK_CLOSEBRACKET);
	    next();

	    lhs = _lcontext.newArrayIndexNode 
			(currentLineNumber(), lhs, index);
	}
    }
    
    return lhs;

}


string
Parser::parseScopedString()
{
    debugSyntax ("scopedString");
    string name;

    if( token() == TK_NAME)
    {
	name = tokenStringValue();
	next();
    }
    else if (token() != TK_SCOPE)
    {
	syntaxError();
	return "";
    }

    while (token() == TK_SCOPE)
    {
	next();
	name += "::";
	match (TK_NAME);
	name += tokenStringValue();
	next();
    }

    return name;
}



NameNodePtr
Parser::parseScopedName ()
{
    debugSyntax ("scopedName");

    string name = parseScopedString();

    SymbolInfoPtr info = symtab().lookupSymbol (name);

    if (!info)
	undefinedName (name);

    debugSyntax1 ("name " << name);
    return _lcontext.newNameNode (currentLineNumber(), name, info);
}


int
Parser::parseExprList (ExprNodeVector &arguments)
{
    debugSyntax ("exprList");

    int count = 0;
    while (token() != TK_CLOSEPAREN &&
	   token() != TK_CLOSEBRACE &&
	   token() != TK_END)
    {
	arguments.push_back (parseExpression());
	count++;

        if (token() == TK_COMMA)
        {
            next();
            if(token() == TK_CLOSEPAREN)
            {
                MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
                             "Extra comma.\n");
            }
        }
        else if(token() != TK_CLOSEPAREN)
        {
            MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
                         "Expected a comma.\n");
        }
    }
    return count;
}



bool
Parser::parseInitializer (ExprNodePtr &initialValue,
			  DataTypePtr dataType,
			  SizeVector &sizes)
{
    //
    //  Parse an array or struct initializer, set initialValue
    //  to the resulting valueNode, sizes comes in with declared
    //  array indices and goes out with actual sizes.  
    //  dataType is the type declared using sizes

    ExprNodeVector elements;
    bool success = parseInitializerRecursive(dataType, elements, sizes, 0);
    if( success )
	initialValue = _lcontext.newValueNode (currentLineNumber(), elements);
    return success;
	
}


bool
Parser::parseInitializerRecursive (DataTypePtr dataType, 
				   ExprNodeVector& elements,
				   SizeVector& sizes,
				   int depth)
{
    debugSyntax1 ("initializer");

    bool success = true;
    //
    // Parses curly brace initializers, including structs and arrays
    // (and nested structs and arrays).  Recursively runs
    // on sub-elements.

    if( StructTypePtr structType = dataType.cast<StructType>())
    {
	match (TK_OPENBRACE);
	next();

	for(MemberVectorConstIterator it = structType->members().begin();
	    it != structType->members().end();
	    it++)
	{
	    SizeVector sub;
	    if(ArrayTypePtr arrayType = it->type.cast<ArrayType>())
		arrayType->sizes(sub);
	    parseInitializerRecursive(it->type, elements, sub, 0);

            if (token() == TK_COMMA)
            {
                next();
                if(token() == TK_CLOSEBRACE)
                {
                    MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, 
                                 currentLineNumber(),
                                 "Extra comma after struct initializer.\n");
                }
            }
            else if(token() != TK_CLOSEBRACE)
            {
                MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
                          "Expected a comma separating struct initializers.\n");
            }
	}

	match (TK_CLOSEBRACE);
	next();
    }
    else if( ArrayTypePtr arrayType = dataType.cast<ArrayType>())
    {
	match (TK_OPENBRACE);
	next();

	if( (int)sizes.size() == depth )
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_INIT_TYPE,  currentLineNumber(),
			"The dimension of the initial array value is higher "
			"than the declared dimension.");
	    sizes.push_back(0);
	    success = false;
	}
	
	int count = 0;
	while (token() != TK_CLOSEBRACE &&
	       token() != TK_END)
	{
	    count ++;
	    parseInitializerRecursive( arrayType->elementType(), 
				       elements, sizes, depth+1 );

            if (token() == TK_COMMA)
            {
                next();
                if(token() == TK_CLOSEBRACE)
                {
                    MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, 
                                 currentLineNumber(),
                                 "Extra comma after array initializer.\n");
                }
            }
            else if(token() != TK_CLOSEBRACE)
            {
                MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
                             "Expected a comma separating struct elements.\n");
            }
	}

	if( sizes[depth] == 0 )
	    sizes[depth] = count;
	else if( (int)sizes[depth] != count )
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_ARR_INIT_READ, 
			 currentLineNumber(),
			"Array contains wrong number of elements.  "
			"Expected " << sizes[depth] << ", got " << count <<
			".\n");
	    success = false;
	}

	match (TK_CLOSEBRACE);
	next();
    }
    else
    {
	if( (int)sizes.size() > depth )
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_INIT_TYPE,  currentLineNumber(),
			"The dimension of the initial array value is lower "
			"than the declared dimension.");
	    success = false;
	}
	elements.push_back (parseExpression());
    }
    return success;
}


DataTypePtr
Parser::parseBaseType ()
{
    debugSyntax ("baseType");

    DataTypePtr baseType = 0;

    switch (token())
    {
      case TK_BOOL:
	baseType = DataTypePtr (_lcontext.newBoolType ());
	next();
	break;

      case TK_INT:
	baseType = DataTypePtr (_lcontext.newIntType ());
	next();
	break;

      case TK_UNSIGNED:

	//
	// For compatibility with C and C++, "unsigned"
	// and "unsigned int" are equivalent.
	//

	baseType = DataTypePtr (_lcontext.newUIntType ());
	next();

	if (token() == TK_INT)
	    next();
	break;

      case TK_FLOAT:
	baseType = DataTypePtr (_lcontext.newFloatType ());
	next();
	break;

      case TK_HALF:
	baseType = DataTypePtr (_lcontext.newHalfType ());
	next();
	break;

      case TK_NAME:
	  {
	      string name = parseScopedString();
	      SymbolInfoPtr info = symtab().lookupSymbol (name);
	  
	      if( info  && info->isTypeName())
	      {
		  baseType = info->type();
	      }
	      else
	      {
		  if( info )
		  {
		      MESSAGE_PLE (_lex, _lcontext, ERR_UNKNOWN_TYPE, 
				  currentLineNumber(), 
				  "Name " << name << " is not a type name.");
		  }
		  else
		  {
		      undefinedName(name);
		  }
		  baseType = DataTypePtr (_lcontext.newIntType ());
	      }
	  }
	  break;
	  
      default:
	  syntaxError();
    }

    return baseType;
}


bool
Parser::parseVaryingHint ()
{
    debugSyntax ("varyingHint");

    if (token() == TK_VARYING)
    {
	next();
	return true;
    }

    if (token() == TK_UNIFORM)
    {
	next();
	return false;
    }

    return false;
}


void
Parser::parseArraySize (SizeVector &sizes)
{
    debugSyntax ("arraySize");


    //
    // Array size -1 means "not an array".
    //

    if (token() != TK_OPENBRACKET)
	return;

    while( token() == TK_OPENBRACKET )
    {

	//
	// Parse the array size expression.
	//

	next();

	if (token() == TK_CLOSEBRACKET)
	{
	    next();
	    sizes.push_back(0);
	    continue;
	}

	ExprNodePtr expression = parseExpression();
	match (TK_CLOSEBRACKET);
	next();

	//
	// Compute the type of the array size expression,
	// and verify that it can be converted to an int.
	//

	expression->computeType (_lcontext);

	IntTypePtr intType = _lcontext.newIntType ();

	if (expression->type && !intType->canPromoteFrom (expression->type))
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_ARR_IND, expression->lineNumber,
			"Non-integer array size (array size is "
			"of type " << expression->type->asString() << ".");

	    sizes.push_back(0);
	    continue;
	}

	expression = evaluateExpression (expression, intType);

	if (IntLiteralNodePtr size = expression.cast<IntLiteralNode>())
	{
	    if (size->value <= 0)
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_ARR_SIZE, 
			     expression->lineNumber,
			    "Invalid array size (" << size->value << ".");
		sizes.push_back(0);
		continue;
	    }
	    sizes.push_back(size->value);
	    continue;
	}
	else
	{
	    MESSAGE_PLE (_lex, _lcontext, ERR_ARR_SZ_UNKNOWN, expression->lineNumber,
			"Cannot determine array size at compile time.");
	    sizes.push_back(0);
	    continue;
	}
    }
}


ExprNodePtr	
Parser::evaluateExpression (ExprNodePtr expr, TypePtr targetType)
{
    expr = expr->evaluate (_lcontext);

    if (targetType)
	expr = targetType->castValue (_lcontext, expr);

    return expr;
}


VariableNodePtr
Parser::variableDefinitionNoInit
    (AllocationMode mode,
     int lineNumber,
     const string &name,
     const DataTypePtr &baseType,
     const SizeVector &declArraySizes)
{
    debugSyntax ("variableDefinition, no initial value");

    //
    // Variable definition, no initial value given.
    //

    DataTypePtr type = baseType;

    if (declArraySizes.size() > 0)
    {
	//
	// Variable is an array.  Check if the sizes for
	// all array dimensions are known, and if they
	// are, create a new array type.
	//

	bool sizeError = false;

	for (int i = 0; i < (int)declArraySizes.size(); i++)
	{
	    if (declArraySizes[i] == 0)
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_ARR_LEN, lineNumber,
			    "Cannot determine array length.");

		sizeError = true;
		break;
	    }
	}

	if (!sizeError)
	    type = _lcontext.newArrayType (baseType, declArraySizes);
    }

    //
    // Store the variable's name, type and address in the symbol table.
    // Create a syntax tree node for the variable.
    //

    AddrPtr addr;

    if (mode == AM_STATIC)
	addr = type->newStaticVariable (_lcontext.module());
    else
	addr = _lcontext.autoVariableAddr (type);

    SymbolInfoPtr info =
	new SymbolInfo (module(), RWA_READWRITE, false, type, addr);

    if (!symtab().defineSymbol (name, info))
	duplicateName (name, lineNumber, fileName());

    return _lcontext.newVariableNode (lineNumber, name, info, 0, true);
}


VariableNodePtr
Parser::variableDefinitionCurlyBraces
    (AllocationMode mode,
     int lineNumber,
     const string &name,
     bool isConst,
     const DataTypePtr &baseType,
     const SizeVector &declArraySizes,
     ExprNodePtr &initialValue)
{
    debugSyntax ("variableDefinitionCurlyBraces");

    //
    // Variable definition, initial value is defined by
    // by list of expressions in curly braces.
    //

    DataTypePtr type = baseType;

    if (declArraySizes.size() > 0)
	type = _lcontext.newArrayType (baseType, declArraySizes);
	
    if (initialValue)
	initialValue->computeType (_lcontext);

    ValueNodePtr value = initialValue;

    if (value && !value->checkElementTypes (baseType, _lcontext))
    {
	value = 0;
    }

    if( value )
    {
	value->type = type;
	value->evaluate(_lcontext);
    }
    
    ExprNodePtr constValue = 0;
    
    if (isConst && value && value->elementsAreLiterals())
	constValue = value;

    //
    // Store the variable's name, type and address in the symbol table.
    // Create a syntax tree node for the variable.
    //

    AddrPtr addr;

    if (mode == AM_STATIC)
	addr = type->newStaticVariable (_lcontext.module());
    else
	addr = _lcontext.autoVariableAddr (type);

    SymbolInfoPtr info = new SymbolInfo (module(), 
					 isConst? RWA_READ: RWA_READWRITE, 
					 false, type, addr);
    if (constValue)
	info->setValue (constValue);

    if (!symtab().defineSymbol (name, info))
	duplicateName (name, lineNumber, fileName());

    return _lcontext.newVariableNode
	(lineNumber, name, info, initialValue, true);
}

VariableNodePtr
Parser::variableDefinitionImport
    (AllocationMode mode,
     int lineNumber,
     const string &name,
     bool isConst,
     const DataTypePtr &baseType,
     const SizeVector &declArraySizes,
     ExprNodePtr &initialValue)
{
    debugSyntax ("variableDefinitionImport");

    //
    // Variable definition, initial value is imported from
    // a file
    //

    DataTypePtr type = baseType;

    if (declArraySizes.size() > 0)
	type = _lcontext.newArrayType (baseType, declArraySizes);
	
    if (initialValue)
	initialValue->computeType (_lcontext);

    ValueNodePtr value = initialValue;

    if (value && !value->checkElementTypes (baseType, _lcontext))
    {
	value = 0;
    }

    if( value )
    {
	value->type = type;
	value->evaluate(_lcontext);
    }
    
    ExprNodePtr constValue = 0;
    
    if (isConst && value && value->elementsAreLiterals())
	constValue = value;

    //
    // Store the variable's name, type and address in the symbol table.
    // Create a syntax tree node for the variable.
    //

    AddrPtr addr;

    if (mode == AM_STATIC)
	addr = type->newStaticVariable (_lcontext.module());
    else
	addr = _lcontext.autoVariableAddr (type);

    SymbolInfoPtr info = new SymbolInfo (module(), 
					 isConst? RWA_READ: RWA_READWRITE, 
					 false, type, addr);
    if (constValue)
	info->setValue (constValue);

    if (!symtab().defineSymbol (name, info))
	duplicateName (name, lineNumber, fileName());

    return _lcontext.newVariableNode
	(lineNumber, name, info, initialValue, true);
}


VariableNodePtr
Parser::variableDefinitionAssignExpr
    (AllocationMode mode,
     int lineNumber,
     const string &name,
     bool isConst,
     const DataTypePtr &baseType,
     const SizeVector &declArraySizes,
     ExprNodePtr &initialValue)
{
    debugSyntax ("variableDefinitionAssignExpr");

    //
    // Variable definition, initial value is defined by
    // expression that is assigned to the variable.
    //

    ExprNodePtr constValue = 0;

    if (initialValue)
	initialValue->computeType (_lcontext);

    DataTypePtr type = baseType;

    if (declArraySizes.size() > 0)
    {
	SizeVector arraySizes = declArraySizes;
	bool initError = false;

	if (initialValue)
	{
	    //
	    //  Check array value initializer
	    //

	    ArrayTypePtr rhsType = initialValue->type.cast<ArrayType>();
	    SizeVector initSizes;

	    if (!rhsType)
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_ARR_LEN, 
			    initialValue->lineNumber,
			    "Right hand side of array assignment is "
			    "not an array: " << name );

		initError = true;
	    }
	    else
	    {
		rhsType->sizes (initSizes);
	    }

	    for (int i = 0; i < (int)arraySizes.size() && !initError ; i++)
	    {
		if (initSizes[i] == 0)
		{
		    MESSAGE_PLE (_lex, _lcontext, ERR_ARR_LEN, 
				lineNumber,
				"Cannot use variable size array as "
				"initial value.");

		    initError = true;
		}

		if(arraySizes[i] == 0)
		{
		    arraySizes[i] = initSizes[i];
		}
		else if (arraySizes[i] != initSizes[i])
		{
		    MESSAGE_PLE (_lex, _lcontext, ERR_ARR_INIT_CONV, 
				initialValue->lineNumber,
				"Cannot convert initial value for array " 
				<< name << " from size " 
				<< initSizes[i] 
				<< " to size " << arraySizes[i] << ".");

		    initError = true;
		}
	    }
	}
	else
	{
	    for (int i = 0; i < (int)arraySizes.size() && !initError; i++)
	    {
		if (arraySizes[i] == 0)
		{
		    MESSAGE_PLE (_lex, _lcontext, ERR_ARR_LEN, 
				lineNumber,
				"Cannot determine array length.");

		    initError = true;
		}
	    }
	}

	if (initError)
	{
	    initialValue = 0;
	    type = baseType;
	}
	else
	{
	    type = _lcontext.newArrayType (baseType, arraySizes);
	}
    }

    if (initialValue && initialValue->type &&
	!type->canCastFrom (initialValue->type))
    {
	MESSAGE_PLE (_lex, _lcontext, ERR_INIT_TYPE, 
		     initialValue->lineNumber,
		    "Cannot convert initial value for " << name << " "
		    "from type " << initialValue->type->asString() << " "
		    "to type " << type->asString() << ".");
	
	initialValue = 0;
    }
    
    if (initialValue)
	initialValue = evaluateExpression (initialValue, type);
    
    if (isConst && initialValue && initialValue.cast<LiteralNode>())
	constValue = initialValue;

    //
    // Store the variable's name, type and address in the symbol table.
    // Create a syntax tree node for the variable.
    //

    AddrPtr addr;

    if (mode == AM_STATIC)
	addr = type->newStaticVariable (_lcontext.module());
    else
	addr = _lcontext.autoVariableAddr (type);

    SymbolInfoPtr info = new SymbolInfo (module(), 
					 isConst? RWA_READ: RWA_READWRITE, 
					 false, type, addr);
    if (constValue)
	info->setValue (constValue);

    if (!symtab().defineSymbol (name, info))
	duplicateName (name, lineNumber, fileName());

    return _lcontext.newVariableNode
	(lineNumber, name, info, initialValue, true);
}


VariableNodePtr
Parser::variableDefinitionExprSideEffect
    (AllocationMode mode,
     int lineNumber,
     const string &name,
     bool isConst,
     const DataTypePtr &baseType,
     const SizeVector &declArraySizes)
{
    debugSyntax ("variableDefinitionExprSideEffect");

    //
    // Variable definition, initial value is generated by calling
    // a function, passing the variable as an output parameter.
    //

    ExprNodePtr constValue = 0;
    DataTypePtr type = baseType;

    if (declArraySizes.size() > 0)
    {
	bool initError = false;

	for (int i = 0; i < (int)declArraySizes.size(); i++ )
	{
	    if (declArraySizes[i] == 0)
	    {
		MESSAGE_PLE (_lex, _lcontext, ERR_ARR_LEN, 
			    lineNumber,
			    "Cannot determine array length.");

		initError = true;
		break;
	    }
	}

	if (!initError)
	    type = _lcontext.newArrayType (baseType, declArraySizes);
    }
	
    //
    // Store the variable's name, type and address in the symbol table.
    // Create a syntax tree node for the variable.
    //

    AddrPtr addr;

    if (mode == AM_STATIC)
	addr = type->newStaticVariable (_lcontext.module());
    else
	addr = _lcontext.autoVariableAddr (type);

    SymbolInfoPtr info = new SymbolInfo (module(), 
					 isConst? RWA_READ: RWA_READWRITE, 
					 false, type, addr);

    if (!symtab().defineSymbol (name, info))
	duplicateName (name, lineNumber, fileName());

    return _lcontext.newVariableNode
	(lineNumber, name, info, 0, false);
}


int		
Parser::currentLineNumber () const
{
    return _lex.currentLineNumber();
}


int
Parser::tokenIntValue () const
{
    return _lex.tokenIntValue();
}


float
Parser::tokenFloatValue () const
{
    return _lex.tokenFloatValue();
}


const std::string &
Parser::tokenStringValue () const
{
    return _lex.tokenStringValue();
}


void	
Parser::match (Token expectedToken)
{
    if (token() != expectedToken)
	syntaxError();
}


void	
Parser::recover (Token expectedToken)
{
    while (token() != expectedToken && token() != TK_END)
	next();
}


void	
Parser::syntaxError ()
{
    MESSAGE_PLE (_lex, _lcontext, ERR_SYNTAX, currentLineNumber(),
		 "Syntax Error.");

    throw Iex::InputExc ("Cannot continue parsing.  Giving up.");
}


void	
Parser::staticVariableError ()
{
    MESSAGE_PLE (_lex, _lcontext, ERR_STATIC_VAR, currentLineNumber(),
                 "Static data must be const.");
}


void	
Parser::foundError (int lineNumber, Error error)
{
    _lcontext.foundError (lineNumber, error);
}


void
Parser::duplicateName (const string &name, 
		       int lineNumber, 
		       const string &fileName)
{
    MESSAGE_PLE (_lex, _lcontext, ERR_NAME_DUP, lineNumber,
		"Name " << name << " has already been defined "
		"in current scope.");
}


void
Parser::undefinedName (const string &name)
{
    MESSAGE_PLE (_lex, _lcontext, ERR_NAME_UNDEF, currentLineNumber(), 
		"Name " << name << " is not defined.");
}


void
loadModuleRecursive (Parser &parser, const string &moduleName)
{
    //
    // This function is a friend of class Interpreter; it allows us
    // to call Interpreter::loadModuleRecursive(), which is private.
    //

    parser.interpreter().loadModuleRecursive (moduleName);
}

} // namespace Ctl

// vim: ts=8:sts=4
