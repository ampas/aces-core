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
//	Syntactic tokens for the color transformation language.
//
//-----------------------------------------------------------------------------

#include <CtlTokens.h>

namespace Ctl {


const char *
tokenAsString (Token token)
{
    switch (token)
    {
      case TK_AND:
	return "&&";
      case TK_ASSIGN:
	return "=";
      case TK_AT:
	return "@error";
      case TK_BITAND:
	return "&";
      case TK_BITNOT:
	return "~";
      case TK_BITOR:
	return "|";
      case TK_BITXOR:
	return "^";
      case TK_BOOL:
	return "bool";
      case TK_BREAK:
	return "break";
      case TK_CLOSEBRACE:
	return "}";
      case TK_CLOSEBRACKET:
	return "]";
      case TK_CLOSEPAREN:
	return ")";
      case TK_COMMA:
	return ",";
      case TK_CONST:
	return "const";
      case TK_CONTINUE:
	return "continue";
      case TK_CTLVERSION:
	return "ctlversion";
      case TK_DIV:
	return "/";
      case TK_DOT:
	return ".";
      case TK_ELSE:
	return "else";
      case TK_END:
	return "(end of input)";
      case TK_EQUAL:
	return "==";
      case TK_FALSE:
	return "false";
      case TK_FLOAT:
	return "float";
      case TK_FLOATLITERAL:
	return "float literal";
      case TK_FOR:
	return "for";
      case TK_GREATER:
	return ">";
      case TK_GREATEREQUAL:
	return ">=";
      case TK_HALF:
	return "half";
      case TK_HALFLITERAL:
	return "half literal";
      case TK_IF:
	return "if";
      case TK_IMPORT:
	return "import";
      case TK_INPUT:
	return "input";
      case TK_INT:
	return "int";
      case TK_INTLITERAL:
	return "int literal";
      case TK_LEFTSHIFT:
	return "<<";
      case TK_LESS:
	return "<";
      case TK_LESSEQUAL:
	return "<=";
      case TK_MINUS:
	return "-";
      case TK_MOD:
	return "%";
      case TK_NAME:
	return "name";
      case TK_NAMESPACE:
	return "namespace";
      case TK_NOT:
	return "!";
      case TK_NOTEQUAL:
	return "!=";
      case TK_OPENBRACE:
	return "{";
      case TK_OPENBRACKET:
	return "[";
      case TK_OPENPAREN:
	return "(";
      case TK_OR:
	return "||";
      case TK_OUTPUT:
	return "output";
      case TK_PLUS:
	return "+";
      case TK_PRINT:
	return "print";
      case TK_RETURN:
	return "return";
      case TK_RIGHTSHIFT:
	return ">>";
      case TK_SCOPE:
	return "::";
      case TK_SEMICOLON:
	return ";";
      case TK_STRING:
	return "string";
      case TK_STRINGLITERAL:
	return "string literal";
      case TK_STRUCT:
	return "struct";
      case TK_TIMES:
	return "*";
      case TK_TRUE:
	return "true";
      case TK_UNDEFINED:
	return "undefined";
      case TK_UNIFORM:
	return "uniform";
      case TK_UNSIGNED:
	return "unsigned";
      case TK_VARYING:
	return "varying";
      case TK_VOID:
	return "void";
      case TK_WHILE:
	return "while";
    }

    return "(invalid token)";
}


} // namespace Ctl
