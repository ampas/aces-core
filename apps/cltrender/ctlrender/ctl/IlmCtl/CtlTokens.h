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


#ifndef INCLUDED_CTL_TOKENS_H
#define INCLUDED_CTL_TOKENS_H

//-----------------------------------------------------------------------------
//
//	Syntactic tokens for the color transformation language.
//
//-----------------------------------------------------------------------------

namespace Ctl {

enum Token
{
    TK_AND,		// "&&"
    TK_ASSIGN,		// "="
    TK_AT,              // "@error##" where ## is an integer
    TK_BITAND,		// "&"
    TK_BITNOT,		// "~"
    TK_BITOR,		// "|"
    TK_BITXOR,		// "^"
    TK_BOOL,		// "bool"
    TK_BREAK,		// "break"
    TK_CLOSEBRACE,	// "}"
    TK_CLOSEBRACKET,	// "]"
    TK_CLOSEPAREN,	// ")"
    TK_COMMA,		// ","
    TK_CONST,		// "const"
    TK_CONTINUE,	// "continue"
    TK_CTLVERSION,	// "ctlversion"
    TK_DIV,		// "/"
    TK_DOT,             // "." not followed by digits
    TK_ELSE,		// "else"
    TK_END,		// end of input
    TK_EQUAL,		// "=="
    TK_FALSE,		// "false"
    TK_FLOAT,		// "float"
    TK_FLOATLITERAL,	// 32-bit floating point literal, e.g. "123.45"
    TK_FOR,		// "for"
    TK_GREATER,		// ">"
    TK_GREATEREQUAL,	// ">="
    TK_HALF,		// "half"
    TK_HALFLITERAL,	// 16-bit floating-point literal, e.g. "123.4h"
    TK_IF,		// "if"
    TK_IMPORT,		// "import"
    TK_INPUT,		// "input"
    TK_INT,		// "int"
    TK_INTLITERAL,	// integer literal, e.g. "123"
    TK_LEFTSHIFT,	// "<<"
    TK_LESS,		// "<"
    TK_LESSEQUAL,	// "<="
    TK_MINUS,		// "-"
    TK_MOD,		// "%"
    TK_NAME,		// name, e.g. "x"
    TK_NAMESPACE,	// "namespace"
    TK_NOT,		// "!"
    TK_NOTEQUAL,	// "!="
    TK_OPENBRACE,	// "{"
    TK_OPENBRACKET,	// "["
    TK_OPENPAREN,	// "("
    TK_OR,		// "||"
    TK_OUTPUT,		// "output"
    TK_PLUS,		// "+"
    TK_PRINT,		// "print"
    TK_RETURN,		// "return"
    TK_RIGHTSHIFT,	// ">>"
    TK_SCOPE,		// "::"
    TK_SEMICOLON,	// ";"
    TK_STRINGLITERAL,	// string literal
    TK_STRING,		// "string"
    TK_STRUCT,		// "struct"
    TK_TIMES,		// "*"
    TK_TRUE,		// "true"
    TK_UNDEFINED,	// undefined token / undefined value
    TK_UNIFORM,		// "uniform"
    TK_UNSIGNED,	// "unigned"
    TK_VARYING,		// "varying"
    TK_VOID,		// "void"
    TK_WHILE,		// "while"
};


const char *		tokenAsString (Token token);


} // namespace Ctl

#endif
