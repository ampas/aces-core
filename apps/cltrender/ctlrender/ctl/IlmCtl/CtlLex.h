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


#ifndef INCLUDED_CTL_LEX_H
#define INCLUDED_CTL_LEX_H

//-----------------------------------------------------------------------------
//
//	class Lex -- the lexical analyzer for the color transformation
//	language.  Eliminates white space and comments from the program
//	text and splits the rest of the text into syntactic tokens.
//
//-----------------------------------------------------------------------------

#include <CtlLContext.h>
#include <CtlTokens.h>
#include <string>

namespace Ctl {


class Lex
{
  public:

    //------------------------------------------------------------
    // Constructor, opens the file that contains the program text,
    // and calls next(), below.  The name of the file to be opened
    // is found by querying the ErrorCound object.
    //------------------------------------------------------------

     Lex (LContext &lcontext);


    //-----------------------------------------------------------------
    // Advance to the text token:
    //
    // next() reads characters from the program text until the end of
    // the next syntactic token has been found.  This token becomes
    // the "current token".  The line in the program text that contains
    // the current token becomes the "current line".
    //
    // Some tokens have an associated "value".  This value becomes
    // the "token value".  For example if the program text contains
    // the character sequence "3.0", TK_FLOATLITERAL becomes the current
    // token, and the floating-point number 3.0 becomes the token value.
    //
    // If reading characters from the program text encounters the end
    // of the program before another token is found, an "end of input"
    // token (TK_END) is generated and becomes the current token.
    //-----------------------------------------------------------------

    void		next ();


    //----------------------------
    // Access to the current token
    //----------------------------

    const Token &	token () const		   {return _token;}


    //--------------------------
    // Access to the token value
    //--------------------------

    int			tokenIntValue () const	   {return _tokenIntValue;}
    float		tokenFloatValue () const   {return _tokenFloatValue;}
    const std::string &	tokenStringValue () const  {return _tokenStringValue;}


    //-----------------------------------------------------------------
    // Access to the current line, for error message generation:
    //
    // currentLineNumber() returns the line number of the current line.
    // lines are counted starting with 1.
    //
    // printCurrentLine() prints the current line, along with the name
    // of the input file, a line number, and an indication of how far
    // the program text has already been read.  The output looks roughly
    // like this:
    //
    //	foo.ctl, line 61:
    //	return pow ((x + 0.099) / 1.099, 1 / 0.45);
    //	              ^
    //-----------------------------------------------------------------

    int			currentLineNumber () const {return _currentLineNumber;}
    void		printCurrentLine () const;

  private:

    bool		nextLine ();
    bool		atEndOfLine () const;
    char		currentChar () const;
    void		nextChar ();
    void		skipWhiteSpace ();
    bool		nextToken ();
    bool		getNameOrKeyword ();
    bool                getAtKeyword();
    bool		getIntOrFloatLiteral (bool decimalPointSeen);
    bool		getStringLiteral ();
    void		badToken (char c);

    LContext &		_lcontext;
    std::string		_currentLine;
    char		_currentChar;
    int			_currentCharIndex;
    int			_currentLineNumber;
    Token		_token;
    int			_tokenIntValue;
    float		_tokenFloatValue;
    std::string		_tokenStringValue;
};


} // namespace Ctl

#endif

