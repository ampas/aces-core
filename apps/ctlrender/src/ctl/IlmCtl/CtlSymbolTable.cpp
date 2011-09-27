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
//	The symbol table for the color tranformation language parser.
//
//-----------------------------------------------------------------------------

#include <CtlSymbolTable.h>
#include <sstream>
#include <cassert>
#include <iostream>
#include <iomanip>

using namespace std;

#if 0
    #define debug(x) (cout << x << endl)
#else
    #define debug(x)
#endif

namespace Ctl {


SymbolInfo::SymbolInfo
    (const Module *module,
     const ReadWriteAccess access, /* = RWA_NONE */
     bool isTypeName  /* = false */,
     const TypePtr &type /* = 0 */,
     const AddrPtr &addr /* = 0 */)
:
    _module (module),
    _type (type),
    _addr (addr),
    _value (0),
    _isTypeName (isTypeName),
    _access (access)
{
    // empty
}


bool
SymbolInfo::isData () const
{
    return (!_isTypeName && _type.cast<DataType>());
}


bool	
SymbolInfo::isFunction () const
{
    return (!_isTypeName && _type.cast<FunctionType>());
}


const TypePtr &
SymbolInfo::type () const
{
    return _type;
}


DataTypePtr
SymbolInfo::dataType () const
{
    return _type;
}


FunctionTypePtr
SymbolInfo::functionType () const
{
    return _type;
}


void
SymbolInfo::setType (const TypePtr &type)
{
    _type = type;
}


const ExprNodePtr &
SymbolInfo::value () const
{
    assert (isData());
    return _value;
}


void
SymbolInfo::setValue (const ExprNodePtr &value)
{
    assert (isData());
    _value = value;
}


const AddrPtr &
SymbolInfo::addr () const
{
    return _addr;
}


void
SymbolInfo::setAddr (const AddrPtr &addr)
{
    _addr = addr;
}


void		
SymbolInfo::print (int indent) const
{
    if (_type)
	_type->print (indent);
    else
	cout << setw (indent + 1) << "" << "*** type unknown ***" << endl;

    if (_addr)
	_addr->print (indent);

    if (_value)
    {
	cout << setw (indent) << "" << "constant value" << endl;
	_value->print (indent + 1);
    }
}


bool
SymbolInfo::isWritable () const
{
    return access() == RWA_WRITE || access() == RWA_READWRITE;
}


bool
SymbolInfo::isReadable () const
{
    return access() == RWA_READ || access() == RWA_READWRITE;
}


const char *
SymbolInfo::accessAsString () const
{
    switch (access())
    {
      case RWA_READ:
	return "read only";

      case RWA_WRITE:
	return "write only";

      case RWA_READWRITE:
	return "read write";

      default:
	return "no access";
    }
}

bool
SymbolInfo::isTypeName() const
{
    return _isTypeName;
}


ReadWriteAccess
SymbolInfo::access () const
{
    return _access;
}


SymbolTable::SymbolTable ()
{
    _i = 0;
}


SymbolTable::~SymbolTable ()
{
    // empty
}


void	
SymbolTable::setGlobalNamespace (const std::string &name)
{
    debug ("SymbolTable::setGlobalNamespace (name = " << name << ")");

    _globalNs = name;
}


const std::string &
SymbolTable::getGlobalNamespace ()
{
    debug ("SymbolTable::getGlobalNamespace ()");

    return _globalNs;
}

void	
SymbolTable::pushLocalNamespace ()
{
    debug ("SymbolTable::pushLocalNamespace()");

    stringstream ss;
    ss << "N" << _i++;
    _localNsStack.push_back (ss.str());

    debug ("\tname = " << _localNsStack.back());
}


void	
SymbolTable::popLocalNamespace ()
{
    debug ("SymbolTable::popLocalNamespace()");

    assert (!_localNsStack.empty());
    _localNsStack.pop_back();
}



bool
SymbolTable::defineSymbol (const string &name, const SymbolInfoPtr &info)
{
    debug ("SymbolTable::defineSymbol (name = " << name << ")");

    string absName = getAbsoluteName(name);

    if (_symbols.find (absName) != _symbols.end())
	return false;

    _symbols[absName] = info;
    return true;
}


string
SymbolTable::getAbsoluteName (const string &name)
{
    debug ("SymbolTable::getAbsoluteName (name = " << name << ")");

    string absName;

    if (name.find ("::") == string::npos)
    {
	absName = _globalNs;

	absName += "::";

	if (!_localNsStack.empty())
	    absName += _localNsStack.back() + "::";

	absName += name;
    }
    else
    {
	absName = name;
    }

    debug ("\tabsName = " << absName);
    return absName;
}


SymbolInfoPtr
SymbolTable::lookupSymbol (const string &name, const string **absName) const
{
    debug ("SymbolTable::lookupSymbol (name = " << name << ")");

    if (name.find ("::") != string::npos)
    {
	debug ("\ttrying " << name);
	SymbolMap::const_iterator j = _symbols.find (name);

	if (j != _symbols.end())
	{
	    debug ("\tfound");

	    if (absName)
		*absName = &j->first;

	    return j->second;
	}
    }
    else
    {
	for (StringStack::const_reverse_iterator i = _localNsStack.rbegin();
	     i != _localNsStack.rend();
	     ++i)
	{

	    string tmpName = _globalNs + "::" + *i + "::" + name;
	    debug ("\ttrying " << tmpName);
	    SymbolMap::const_iterator j = _symbols.find (tmpName);

	    if (j != _symbols.end())
	    {
		debug ("\tfound");

		if (absName)
		    *absName = &j->first;

		return j->second;
	    }
	}

	{
	    string tmpName = _globalNs + "::" + name;
	    debug ("\ttrying " << tmpName);
	    SymbolMap::const_iterator j = _symbols.find (tmpName);

	    if (j != _symbols.end())
	    {
		debug ("\tfound");

		if (absName)
		    *absName = &j->first;

		return j->second;
	    }
	}

	{
	    string tmpName = "::" + name;
	    debug ("\ttrying " << tmpName);
	    SymbolMap::const_iterator j = _symbols.find (tmpName);

	    if (j != _symbols.end())
	    {
		debug ("\tfound");

		if (absName)
		    *absName = &j->first;

		return j->second;
	    }
	}

	{
	    debug ("\ttrying " << name);
	    SymbolMap::const_iterator j = _symbols.find (name);

	    if (j != _symbols.end())
	    {
		debug ("\tfound");

		if (absName)
		    *absName = &j->first;

		return j->second;
	    }
	}
    }

    debug ("\tnot found");

    if (absName)
	*absName = 0;

    return 0;
}


void	
SymbolTable::deleteAllSymbols (const Module *module)
{
    SymbolMap::iterator i = _symbols.begin();
    
    while (i != _symbols.end())
    {
	SymbolMap::iterator j = i++;

	if (j->second->module() == module)
	    _symbols.erase (j);
    }
}


void	
SymbolTable::deleteAllLocalSymbols (const Module *module)
{
    SymbolMap::iterator i = _symbols.begin();
    
    while (i != _symbols.end())
    {
	SymbolMap::iterator j = i++;

	//
	// If the substring "::" occurs at least twice in a symbol's
	// absolute name then the symbol is defined in a local name space.
	//

	if (j->second->module() == module &&
	    j->first.find ("::") != j->first.rfind ("::"))
	{
	    _symbols.erase (j);
	}
    }
}


} // namespace Ctl
