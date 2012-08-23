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
//	The lexical analyzer for the color transformation language.
//
//-----------------------------------------------------------------------------

#include <CtlLex.h>
#include <CtlMessage.h>
#include <CtlLContext.h>
#include <iomanip>
#include <cassert>
#include <stdlib.h>

#if 0
    #include <iostream>
    #define debug(x) (cout << x << endl)
#else
    #define debug(x)
#endif


#if 0
    #include <iostream>
    #define debug1(x) (cout << x << endl)
#else
    #define debug1(x)
#endif


using namespace std;

namespace Ctl {
namespace {


void
getLine (istream &is, string &str)
{
    //
    // Read the next line from the input file and place in it str.
    // The end of the line line may be represented according to
    // Unix/Linux, Dos/Windows or Macintosh conventions:
    //
    // 	Unix	\n	(line feed)
    // 	Dos	\r\n	(carriage return, line feed)
    // 	Mac	\r	(carriage return)
    //

    str.clear();

    char c;

    while (is.get (c))
    {
	if (c == '\r')
	{
	    //
	    // Mac or Dos end-of-line.
	    //

	    char d;

	    if (is.get (d) && d != '\n')
	    {
		//
		// Mac end-of-line.  We've just read the first
		// character of the following line; we must put
		// it back into the input stream.
		//

		is.unget();			
	    }

	    break;
	}

	if (c == '\n')
	{
	    //
	    // Unix end-of-line
	    //
	    
	    break;
	}

	str += c;
    }
}


} // namespace


Lex::Lex (LContext &lcontext):
    _lcontext (lcontext),
    _currentLine (""),
    _currentChar (0),
    _currentCharIndex (0),
    _currentLineNumber (0),
    _token (TK_END),
    _tokenIntValue (0),
    _tokenFloatValue (0),
    _tokenStringValue ("")
{
    debug ("Lex::Lex "
	   "(this = " << this << ", "
	   "lcontext = \"" << &lcontext << "\")");

    next();
}


void	
Lex::next ()
{
    debug ("Lex::next (this = " << this << ")");

    do
    {
	while (!nextToken())
	{
	    if (!nextLine())
	    {
		_token = TK_END;
		return;
	    }
	}
    }
    while (token() == TK_AT);
}


bool
Lex::nextLine ()
{
    debug ("Lex::nextLine (this = " << this << ")");

    if (_lcontext.file())
    {
	//
	// Read the next line, that is, all characters up to
	// the next newline or up to the end of the input stream.
	//

	_currentLineNumber += 1;
	_currentChar = 0;
	_currentCharIndex = 0;

	getLine (_lcontext.file(), _currentLine);

	if (_currentLine.size() > 0)
	    _currentChar = _currentLine[0];

	debug1 ("\t" << _currentLineNumber << "\t" << _currentLine);

	debug ("\t\"" << _currentLine << "\"");
	debug ("\treturn true");

	return true;
    }
    else
    {
	debug ("\treturn false");
	return false;
    }
}


inline bool
Lex::atEndOfLine () const
{
    return _currentCharIndex >= (int)_currentLine.size();
}


inline char
Lex::currentChar () const
{
    return _currentChar;
}


inline void
Lex::nextChar ()
{
    if (!atEndOfLine())
	_currentCharIndex += 1;

    _currentChar = atEndOfLine()? 0: _currentLine[_currentCharIndex];
}


void
Lex::skipWhiteSpace ()
{
    while (isspace (currentChar()))
	nextChar();
}


bool
Lex::nextToken ()
{
    debug ("Lex::nextToken (this = " << this << ")");

    skipWhiteSpace();

    if (atEndOfLine())
    {
	debug ("\treached end of current line\n");
	return false;
    }

    if (isalpha (currentChar()) || currentChar() == '_')
    {
	debug ("\tname or keyword");
	return getNameOrKeyword();
    }

    if (isdigit (currentChar()))
    {
	debug ("\tinteger or floating-point literal");
	return getIntOrFloatLiteral (false);
    }
    
    if (currentChar() == '.')
    {
	nextChar();
	
	if(isdigit (currentChar()))
	{
	    return getIntOrFloatLiteral (true);
	}
	else
	{
	    _token = TK_DOT;
	    return true;
	}

    }

    if (currentChar() == '"')
    {
	debug ("\tstring literal");
	return getStringLiteral();
    }

    if (currentChar() == '/')
    {
	debug ("\tdivision operator or comment");

	nextChar();

	if (currentChar() == '/')
	{
	    debug ("\t// comment; skipping rest of current line\n");
	    return false;
	}
	else if (currentChar() == '*')
	{
	    debug ("\t/* ... */ comment\n");

	    //
	    // Skip everything until we see either a * followed
	    // by a /, or the end of the input.
	    //

	    nextChar();

	    do
	    {
		while (!atEndOfLine())
		{
		    if (currentChar() == '*')
		    {
			nextChar();

			if (currentChar() == '/')
			{
			    nextChar();
			    return nextToken();
			}
		    }
		    else
		    {
			nextChar();
		    }
		}
	    }
	    while (nextLine());

	    return false;
	}
	else
	{
	    _token = TK_DIV;
	    return true;
	}
    }

    if (currentChar() == '=')
    {
	debug ("\tassignment or equality operator");
	
	nextChar();

	if (currentChar() == '=')
	{
	    nextChar();
	    _token = TK_EQUAL;
	}
	else
	{
	    _token = TK_ASSIGN;
	}

	return true;
    }

    if (currentChar() == '!')
    {
	debug ("\tlogical not or inequality operator");
	
	nextChar();

	if (currentChar() == '=')
	{
	    nextChar();
	    _token = TK_NOTEQUAL;
	}
	else
	{
	    _token = TK_NOT;
	}

	return true;
    }

    if (currentChar() == '<')
    {
	debug ("\tleft shift, less than, or less-or-equal operator");

	nextChar();

	if (currentChar() == '<')
	{
	    nextChar();
	    _token = TK_LEFTSHIFT;
	}
	else if (currentChar() == '=')
	{
	    nextChar();
	    _token = TK_LESSEQUAL;
	}
	else
	{
	    _token = TK_LESS;
	}

	return true;
    }

    if (currentChar() == '>')
    {
	debug ("\tright shift, greater than, or greater-or-equal operator");

	nextChar();

	if (currentChar() == '>')
	{
	    nextChar();
	    _token = TK_RIGHTSHIFT;
	}
	else if (currentChar() == '=')
	{
	    nextChar();
	    _token = TK_GREATEREQUAL;
	}
	else
	{
	    _token = TK_GREATER;
	}

	return true;
    }

    if (currentChar() == '&')
    {
	debug ("\tsequential and, or bit-wise and operator");

	nextChar();

	if (currentChar() == '&')
	{
	    nextChar();
	    _token = TK_AND;
	}
	else
	{
	    _token = TK_BITAND;
	}

	return true;
    }

    if (currentChar() == '|')
    {
	debug ("\tsequential or, or bit-wise or operator");

	nextChar();

	if (currentChar() == '|')
	{
	    nextChar();
	    _token = TK_OR;
	}
	else
	{
	    _token = TK_BITOR;
	}

	return true;
    }

    if (currentChar() == ':')
    {
	debug ("\tscope operator");

	nextChar();

	if (currentChar() == ':')
	{
	    nextChar();
	    _token = TK_SCOPE;
	}
	else
	{
	    badToken (currentChar());
	}

	return true;
    }

    if (currentChar() == '@')
    {
	debug ("\t@Error operator");

	nextChar();
	_token = TK_AT;
	return getAtKeyword();
    }


    debug ("\tsingle character \"" << currentChar() << "\"");

    char c = currentChar();
    nextChar();

    switch (c)
    {
      case '~':
	_token = TK_BITNOT;
	break;

      case '^':
	_token = TK_BITXOR;
	break;

      case '}':
	_token = TK_CLOSEBRACE;
	break;

      case ']':
	_token = TK_CLOSEBRACKET;
	break;

      case ')':
	_token = TK_CLOSEPAREN;
	break;

      case ',':
	_token = TK_COMMA;
	break;

      case '-':
	_token = TK_MINUS;
	break;

      case '%':
	_token = TK_MOD;
	break;

      case '{':
	_token = TK_OPENBRACE;
	break;

      case '[':
	_token = TK_OPENBRACKET;
	break;

      case '(':
	_token = TK_OPENPAREN;
	break;

      case '+':
	_token = TK_PLUS;
	break;

      case ';':
	_token = TK_SEMICOLON;
	break;

      case '*':
	_token = TK_TIMES;
	break;

      default:
	badToken (c);
	break;
    }

    return true;
}


bool
Lex::getAtKeyword()
{
    _tokenStringValue.clear();

    while (isalpha (currentChar()))
    {
	_tokenStringValue += currentChar();
	nextChar();
    }

    if (_tokenStringValue != "error")
    {
	printCurrentLine();
	MESSAGE_LE (_lcontext, ERR_AT_SYNTAX, _currentLineNumber, 
		    "Error parsing @error directive.");
	return true;
    }

    _tokenStringValue.clear();

    while (isdigit (currentChar()))
    {
	_tokenStringValue += currentChar();
	nextChar();
    }

    if (_tokenStringValue == "")
    {
	printCurrentLine();
	MESSAGE_LE (_lcontext, ERR_AT_SYNTAX, _currentLineNumber, 
		    "@error directive without error number.");
	return true;
    }

    const char *b = _tokenStringValue.c_str();
    char *e;

    int errorValue = -1;
    errorValue = strtol (b, &e, 0);

    if (e - b != (int)_tokenStringValue.size())
    {
	printCurrentLine();
	MESSAGE_LE (_lcontext, ERR_AT_SYNTAX, _currentLineNumber, 
		    "Invalid @error directive.");
    }
    else if( errorValue < 0 || errorValue >= ERR_MAX)
    {
	printCurrentLine();
	MESSAGE_LE (_lcontext, ERR_AT_SYNTAX, _currentLineNumber, 
		    "@error value \"" << errorValue << "\" is not valid.");

    }

    _lcontext.declareError(_currentLineNumber, Error(errorValue));
    
    debug ("\t@error(" << errorValue << "," << _currentLineNumber << ")");
    return true;
}


bool
Lex::getNameOrKeyword ()
{
    _tokenStringValue.clear();

    while (isalnum (currentChar()) || currentChar() == '_')
    {
	_tokenStringValue += currentChar();
	nextChar();
    }
    
    if (_tokenStringValue == "bool")
	_token = TK_BOOL;
    else if (_tokenStringValue == "break")
	_token = TK_BREAK;
    else if (_tokenStringValue == "const")
	_token = TK_CONST;
    else if (_tokenStringValue == "continue")
	_token = TK_CONTINUE;
    else if (_tokenStringValue == "ctlversion")
	_token = TK_CTLVERSION;
    else if (_tokenStringValue == "else")
	_token = TK_ELSE;
    else if (_tokenStringValue == "false")
	_token = TK_FALSE;
    else if (_tokenStringValue == "float")
	_token = TK_FLOAT;
    else if (_tokenStringValue == "for")
	_token = TK_FOR;
    else if (_tokenStringValue == "half")
	_token = TK_HALF;
    else if (_tokenStringValue == "if")
	_token = TK_IF;
    else if (_tokenStringValue == "input")
	_token = TK_INPUT;
    else if (_tokenStringValue == "import")
	_token = TK_IMPORT;
    else if (_tokenStringValue == "int")
	_token = TK_INT;
    else if (_tokenStringValue == "namespace")
	_token = TK_NAMESPACE;
    else if (_tokenStringValue == "output")
	_token = TK_OUTPUT;
    else if (_tokenStringValue == "print")
	_token = TK_PRINT;
    else if (_tokenStringValue == "return")
	_token = TK_RETURN;
    else if (_tokenStringValue == "string")
	_token = TK_STRING;
    else if (_tokenStringValue == "struct")
	_token = TK_STRUCT;
    else if (_tokenStringValue == "true")
	_token = TK_TRUE;
    else if (_tokenStringValue == "uniform")
	_token = TK_UNIFORM;
    else if (_tokenStringValue == "unsigned")
	_token = TK_UNSIGNED;
    else if (_tokenStringValue == "varying")
	_token = TK_VARYING;
    else if (_tokenStringValue == "void")
	_token = TK_VOID;
    else if (_tokenStringValue == "while")
	_token = TK_WHILE;
    else
    {
	_token = TK_NAME;
    }

    debug ("\t" << ((_token == TK_NAME)? "name": "keyword"));
    return true;
}


bool
Lex::getIntOrFloatLiteral (bool decimalPointSeen)
{
    _tokenStringValue.clear();
    bool isFloat = false;

    if (decimalPointSeen)
    {
	//
	// The caller has already seen, and advanced past, a decimal point.
	//

	_tokenStringValue += '.';
	isFloat = true;
    }

    if (!isFloat && currentChar() == '0')
    {
	_tokenStringValue += currentChar();
	nextChar();

	if (currentChar() == 'x' || currentChar() == 'X')
	{
	    //
	    // Base-16 integer, starts with 0x or 0X.
	    //

	    _tokenStringValue += currentChar();
	    nextChar();

	    while (isxdigit (currentChar()))
	    {
		_tokenStringValue += currentChar();
		nextChar();
	    }

	    const char *b = _tokenStringValue.c_str();
	    char *e;

	    _tokenIntValue = strtol (b, &e, 0);

	    if (e - b != (int)_tokenStringValue.size())
	    {
		_tokenIntValue = 0;

		printCurrentLine();

		MESSAGE_LE (_lcontext, ERR_INT_SYNTAX, _currentLineNumber, 
			    "Invalid base-16 integer literal.");
	    }

	    _token = TK_INTLITERAL;
	    debug ("\tbase-16 integer literal " << _tokenIntValue);
	    return true;
	}
    }

    //
    // Floating-point or decimal integer 
    //

    while (isdigit (currentChar()))
    {
	_tokenStringValue += currentChar();
	nextChar();
    }

    if (currentChar() == '.' && !decimalPointSeen)
    {
	//
	// Floating-point, get digits after the decimal point
	//

	_tokenStringValue += currentChar();
	nextChar();
	isFloat = true;

	while (isdigit (currentChar()))
	{
	    _tokenStringValue += currentChar();
	    nextChar();
	}
    }

    if (currentChar() == 'e' || currentChar() == 'E')
    {
	//
	// Floating-point number in "scientific" notation, get exponent.
	//

	_tokenStringValue += currentChar();
	nextChar();
	isFloat = true;

	if (currentChar() == '+' || currentChar() == '-')
	{
	    _tokenStringValue += currentChar();
	    nextChar();
	    isFloat = true;
	}

	while (isdigit (currentChar()))
	{
	    _tokenStringValue += currentChar();
	    nextChar();
	}
    }

    if (isFloat)
    {
	const char *b = _tokenStringValue.c_str();
	char *e;

	_tokenFloatValue = strtod (b, &e);

	if (e - b != (int)_tokenStringValue.size())
	{
	    _tokenFloatValue = 0;

	    printCurrentLine();

	    MESSAGE_LE (_lcontext, ERR_FLOAT_SYNTAX, _currentLineNumber, 
			"Invalid floating-point literal.");
	}

	if (currentChar() == 'h' || currentChar() == 'H')
	{
	    //
	    // 16-bit half
	    //

	    nextChar();
	    _token = TK_HALFLITERAL;
	}
	else
	{
	    //
	    // 32-bit float
	    //

	    _token = TK_FLOATLITERAL;
	}

	debug ("\tfloating-point literal " << _tokenFloatValue);
    }
    else
    {
	const char *b = _tokenStringValue.c_str();
	char *e;

	_tokenIntValue = strtol (b, &e, 0);

	if (e - b != (int)_tokenStringValue.size())
	{
	    _tokenIntValue = 0;

	    printCurrentLine();

	    MESSAGE_LE (_lcontext, ERR_INT_SYNTAX, _currentLineNumber, 
			"Invalid decimal integer literal.");
	}

	_token = TK_INTLITERAL;
	debug ("\tdecimal integer literal " << _tokenIntValue);
    }

    return true;
}


bool
Lex::getStringLiteral ()
{
    _tokenStringValue.clear();

    assert (currentChar() == '"');
    nextChar();

    while (currentChar() != '"')
    {
	if (atEndOfLine())
	{
	    debug ("\treached end of current line\n");

	    printCurrentLine();

	    MESSAGE_LE (_lcontext, ERR_STRING_SYNTAX,  _currentLineNumber, 
			"Unterminated string literal.");

	    return false;
	}

	if (currentChar() == '\\')
	{
	    nextChar();

	    switch (currentChar())
	    {
	      case 'n':
		_tokenStringValue += '\n';
		break;

	      case 't':
		_tokenStringValue += '\t';
		break;

	      case 'v':
		_tokenStringValue += '\v';
		break;

	      case 'b':
		_tokenStringValue += '\b';
		break;

	      case 'r':
		_tokenStringValue += '\r';
		break;

	      case 'f':
		_tokenStringValue += '\f';
		break;

	      case 'a':
		_tokenStringValue += '\a';
		break;

	      case '\\':
		_tokenStringValue += '\\';
		break;

	      case '?':
		_tokenStringValue += '\?';
		break;

	      case '\'':
		_tokenStringValue += '\'';
		break;

	      case '"':
		_tokenStringValue += '\"';
		break;

	      default:

		printCurrentLine();

		MESSAGE_LE (_lcontext, ERR_STRING_SYNTAX, _currentLineNumber,
			      "Unrecognized escape sequence "
			      "in string literal.");
		return false;
	    }
	}
	else
	{
	    _tokenStringValue += currentChar();
	}

	nextChar();
    }

    nextChar();

    debug ("\tstring literal \"" << _tokenStringValue << "\"");
    _token = TK_STRINGLITERAL;

    return true;
}


void
Lex::printCurrentLine () const
{
    string where;

    for (int i = 0; i < _currentCharIndex; ++i)
	where += (_currentLine[i] == '\t')? '\t': ' ';

    where += '^';

    MESSAGE (_currentLine);
    MESSAGE (where);
}


void
Lex::badToken (char c)
{
    if (c & 0x80 || iscntrl (c))
    {
	MESSAGE_LN (_lcontext.fileName(), _currentLineNumber,
	            "Unexpected character "
		    "0x" << setw (2) << setfill ('0') << setbase (16) <<
		    (int)(unsigned char)c << " in program.");
    }

    _token = TK_UNDEFINED;
    _lcontext.foundError (_currentLineNumber, ERR_SYNTAX);
}

} // namespace Ctl
