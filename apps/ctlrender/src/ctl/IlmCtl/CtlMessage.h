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

#ifndef INCLUDED_CTL_MESSAGE_H
#define INCLUDED_CTL_MESSAGE_H

//-----------------------------------------------------------------------------
//
//	Error/warning message output for the color transformation engine.
//	
//	The color transformation language compiler and run-time support
//	output error messages via the MESSAGE() and MESSAGE_LN() macros,
//	below.  By default, error messages are printed to std::cerr, but
//	error message output can be redirected by supplying an alternate
//	message output function.
//
//-----------------------------------------------------------------------------

#include <string>
#include <sstream>
#include <CtlErrors.h>

namespace Ctl {

//----------------------------------------------------
// Type declaration for error message output functions
//----------------------------------------------------

typedef void (*MessageOutputFunction) (const std::string &);


//-----------------------------------------------------------------------
// setMessageOutputFunction(f) makes function f the current error message
// output function, and returns the previous message output function.
//
// outputMessage(m) outputs error message m by calling the current error
// message function
//
//-----------------------------------------------------------------------

MessageOutputFunction	setMessageOutputFunction (MessageOutputFunction f);
void			outputMessage (const std::string &message);


//----------------------------------------------------------------------------
// Macros to format and output error messages.
//
// Examples:
//
// Type
//
//	MESSAGE ("Name " << name << " is not defined.");
//
// to print
//
//	Name glorp is not defined.
//
// Type
//
//	MESSAGE_LN (fileName, line, "Name " << name << " is not defined.");
//
// to print
//
//	foo.ctl:123: Name glorp is not defined.
//
// Type
//
//	MESSAGE_LN (lcontext, ERR_NAME_UNDER, line,
//		    "Name " << name << " is not defined.");
//
// to print
//
//	foo.ctl:123: Name glorp is not defined. (@error29)
//
//
// MESSAGE_LE  is a version that not print out a message 
// or stop the compilation if the line also contains an "@error"
// comment before the error.  If the @error message matches the 
// error number (err), no message will be printed and the error
// will be cleared at the end of the compilation.
//
// MESSAGE_LW  is a version of MESSAGE_LE, but for messages
// that are warnings, not errors.  If the line where the warning
// occurs also contains an "@error" comment before the error, and
// the @error message matches the error number (err), no message
// will be printed.
//
// MESSAGE_PLE  is a version to be used when parsing, when the lex
// object is available.  It prints out a pointer under the
// current line indicating where the error occurred.
//
//----------------------------------------------------------------------------

#define MESSAGE_NO_ENDL(text)					\
    do								\
    {								\
	std::stringstream ss;					\
	ss << text;						\
	Ctl::outputMessage (ss.str());				\
    }								\
    while (0)

#define MESSAGE(text)						\
    MESSAGE_NO_ENDL (text << std::endl)


#define MESSAGE_LN(fileName, lineNumber, text)			\
    MESSAGE (fileName << ":" << lineNumber << ": " << text);	\


#define MESSAGE_LE(lcontext, err, lineNumber, text)		\
    do								\
    {								\
        lcontext.foundError (lineNumber, err);			\
								\
        if (!lcontext.errorDeclared (lineNumber, err))		\
        {							\
            MESSAGE_LN (lcontext.fileName(), lineNumber,	\
			text << " (@error" << err << ")");	\
								\
        }							\
    }								\
    while(0)


#define MESSAGE_LW(lcontext, err, lineNumber, text)		\
    do								\
    {								\
        if (!lcontext.errorDeclared (lineNumber, err))		\
        {							\
            MESSAGE_LN (lcontext.fileName(), lineNumber,	\
			text << " (@error" << err << ")");	\
        }							\
	else							\
	{							\
	    lcontext.foundError (lineNumber, err);		\
	}							\
    }								\
    while(0)


#define MESSAGE_PLE(lex, lcontext, err, lineNumber, text)       \
    do								\
    {								\
        lcontext.foundError (lineNumber, err);			\
								\
        if (!lcontext.errorDeclared (lineNumber, err))		\
        {							\
            lex.printCurrentLine();				\
            MESSAGE_LN (lcontext.fileName(), lineNumber,	\
			text << " (@error" << err << ")");	\
								\
        }							\
    }								\
    while(0)





} // namespace Ctl

#endif
