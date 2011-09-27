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


#ifndef INCLUDED_CTL_ERRORS_H
#define INCLUDED_CTL_ERRORS_H

//-----------------------------------------------------------------------------
//
//	Compile errors for the color transformation language.
//
//-----------------------------------------------------------------------------

namespace Ctl {


enum Error
{
    ERR_AT_SYNTAX     = 0,      // array out of range
    ERR_FLOAT_SYNTAX  = 1,      // invalid floating-point literal
    ERR_INT_SYNTAX    = 2,      // invalid int literal
    ERR_STRING_SYNTAX = 3,      // string parsing error
    ERR_SYNTAX        = 4,      // general syntax error

    ERR_RETURN_REQ    = 5,      // Non-void function must return value
    ERR_RETURN_CONV   = 6,      // Can't convert return val
    ERR_FUNC_PARAM    = 7,      // can't convert func parameter
    ERR_CONST         = 8,      // No value given for const

    ERR_ARR_INIT_CONV = 9,      // cannot convert initial value for array 
    ERR_ARR_LEN       = 10,     // cannot determine array length
    ERR_ARR_IND       = 11,     // non-integer array size
    ERR_ARR_SIZE      = 12,     // invalid array size
    ERR_ARR_SZ_UNKNOWN= 13,     // array size unkown
    ERR_ARR_IND_TYPE  = 14,     // non integer index
    ERR_ARR_EL_TYPE   = 15,     // wrong type array element

    ERR_NON_ARR_IND   = 16,     // applied [] operator to non-array

    ERR_INIT_TYPE     = 17,     // cannot convert init value to type
    ERR_OP_TYPE       = 18,     // invalid operand type
    ERR_TYPE          = 19,     // can not cast value to requied type

    ERR_LVAL          = 20,     // lhs of assignment is not lvalue
    ERR_ASSIGN_CONV   = 21,     // rhs can not be casted to lhs
    ERR_IF_CONV       = 22,     // can't convert condition for 'if' to bool.
    ERR_WHILE_CONV    = 23,     // can't convert condition for 'while' to bool.

    ERR_NON_FUNC      = 24,     // func call to non-function
    ERR_FUNC_ARG_NUM  = 25,     // too many function arguments
    ERR_FUNC_ARG_LVAL = 26,     // output func not an lval
    ERR_FUNC_ARG_TYPE = 27,     // wrong func arg type

    ERR_NAME_DUP      = 28,     // duplicate name definition
    ERR_NAME_UNDEF    = 29,     // referenced name not defined

    ERR_STRUCT_ARR_LEN= 30,     // unspecified len of struct array member
    ERR_STRUCT_CONST  = 31,     // tried to declare const struct member

    ERR_UNKNOWN_TYPE  = 32,     // tried to declare variable using unknown type
    ERR_MEMBER_NAME   = 33,     // error parsing member name
    ERR_NON_STRUCT    = 34,     // tried to access member of non-struct type
    ERR_MEMBER_ACCESS = 35,     // unable to find member with given name
    
    ERR_ARR_INIT_READ = 36,     // error parsing array initialization

    ERR_PRINT_TYPE    = 37,     // cannot print expression of given type

    ERR_NON_ARRAY     = 38,     // applied size() to non-arraytype
    ERR_SIZE_SYNTAX   = 39,     // didn't have "()" after size operator

    ERR_DEFAULT_ORDER = 40,     // defaults were not the last arguments
    ERR_DEFAULT_WRITE = 41,     // tried to set default on non-input value

    ERR_STATIC_VAR    = 42,	// attempt to define a static variable

    ERR_DIV_ZERO      = 43,	// division by zero

    ERR_CTL_VERSION   = 44,	// unsupported CTL version

    ERR_MAX             ,
};

struct              LineError
{
    int             lineNumber;
    Error           error;

    bool            operator < (const LineError &le) const
    {
	return (lineNumber == le.lineNumber ? error < le.error : 
		lineNumber < le.lineNumber);
    }
};



} // namespace Ctl

#endif
