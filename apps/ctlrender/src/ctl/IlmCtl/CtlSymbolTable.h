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


#ifndef INCLUDED_CTL_SYMBOL_TABLE_H
#define INCLUDED_CTL_SYMBOL_TABLE_H

//-----------------------------------------------------------------------------
//
//	class SymbolTable -- the symbol table for the color tranformation
//	language parser.
//
//	The symbol table maintains a mapping from names (of type std::string)
//	to SymbolInfo objects.  A SymbolTable object stores the data type
//	and the address of the CTL object that is associated with a name.
//
//	In the symbol table, names are stored as absolute names of the
//	form namespace::namespace::...::name.  Relative names, without
//	explicit name spaces, can be used when entering a name or looking
//	up a name.  The symbol table and the parser maintain a stack of
//	name spaces in order to convert relative names into absolute names.
//
//	Example: 
//
//	    namespace ilm		//  1
//	    {				//  2
//		float foo (int x)	//  3
//		{			//  4
//		    if (x)		//  5
//		    {			//  6
//			int y = x;	//  7
//			...		//  8
//		    }			//  9
//		}			// 10
//	    }				// 11
//
//	By the time the parser has reached line 7, the name space stack
//	contains three nested name spaces, ilm, N0 and N1.  (As far as the
//	CTL program is concerned only the ilm name space has a name; N0 and
//	N1 are anonymous namespaces.)  The relative names x and y in line 7
//	refer to the absolute names ilm::N0::x and ilm::N0::N1::y.
//
//-----------------------------------------------------------------------------

#include <CtlType.h>
#include <CtlSyntaxTree.h>
#include <CtlReadWriteAccess.h>
#include <CtlAddr.h>
#include <string>
#include <vector>
#include <map>

namespace Ctl {

class Module;


class SymbolInfo: public RcObject
{
  public:

    //------------
    // Constructor
    //------------

    SymbolInfo (const Module *module,
		ReadWriteAccess access = RWA_NONE,
		bool isTypeName = false,
		const TypePtr &type = 0,
		const AddrPtr &addr = 0);


    //---------------------------------------------
    // Access to the module that defines the symbol
    //---------------------------------------------

    const Module *		module () const	{return _module;}


    //-------------------
    // Access to the type
    //-------------------

    bool			isData () const;
    bool			isFunction () const;

    const TypePtr &		type () const;
    DataTypePtr			dataType () const;
    FunctionTypePtr 		functionType () const;
    void			setType (const TypePtr &type);


    //-----------------------------------------------------------
    // Access permissions determine if the object that this
    // symbol refers to is readable and/or writable. (Function
    // accessAsString() is for debugging; it returns the object's
    // access permissions as a character string.)
    //-----------------------------------------------------------

    bool			isWritable () const;
    bool			isReadable () const;
    virtual ReadWriteAccess	access () const;
    const char *		accessAsString () const;
    bool                        isTypeName () const;


    //----------------------
    // Access to the address
    //----------------------

    const AddrPtr &		addr () const;
    void			setAddr (const AddrPtr &addr);


    //------------------------------------------
    // Access to the value of symbolic constants
    //------------------------------------------

    const ExprNodePtr &		value () const;
    void			setValue (const ExprNodePtr &value);


    //--------------------------------------------
    // Print the SymbolInfo object (for debugging)
    //--------------------------------------------

    void			print (int indent) const;

  private:

    const Module *	_module;
    TypePtr		_type;
    AddrPtr		_addr;
    ExprNodePtr		_value;
    bool                _isTypeName;

    ReadWriteAccess     _access;
};

typedef RcPtr<SymbolInfo> SymbolInfoPtr;


class SymbolTable
{
  public:

    //---------------------------
    // Constructor and destructor
    //---------------------------

     SymbolTable ();
    ~SymbolTable ();


    //-----------------------------------------------------------
    // Set the name of the outermost name space (in CTL, only the
    // outermost name space has a name.  Nested name spaces are
    // anonymous.
    //-----------------------------------------------------------

    void		setGlobalNamespace (const std::string &name);
    const std::string  &getGlobalNamespace ();


    //-------------------------------------------------------
    // Push an anonymous name space onto the name space stack
    // or pop a name space off the stack.
    //-------------------------------------------------------

    void		pushLocalNamespace ();
    void		popLocalNamespace ();


    //--------------------------------------------------------
    // Return absolute name that would be assigned in a call to
    // defineSymbol
    //--------------------------------------------------------

    std::string         getAbsoluteName (const std::string &name);


    //-------------------------------------------------------
    // Store a (name, SymbolInfo) pair in the symbol table.
    // If the name already exists in the table, return false
    // without changing the contents of the symbol table.
    //-------------------------------------------------------

    bool		defineSymbol (const std::string &name,
				      const SymbolInfoPtr &info);


    //---------------------------------------------------------
    // Lookup a name in the symbol table.  If the name exists,
    // return a pointer to the SymbolInfo associated with the
    // name.  Otherwise return 0.
    // If absName is not 0, then store either a pointer to the
    // absolute version of the name, or 0, in the location that
    // absName points to.
    //---------------------------------------------------------

    SymbolInfoPtr	lookupSymbol (const std::string &name,
				      const std::string **absName = 0) const;


    //------------------------------------------------------
    // Delete all symbols that are defined in a given module
    //------------------------------------------------------

    void		deleteAllSymbols (const Module *module);


    //--------------------------------------------------------
    // Delete all symbols that are in within local name spaces
    // in a given module.  (Those symbols cannot be accessed
    // by other modules.)
    //--------------------------------------------------------

    void		deleteAllLocalSymbols (const Module *module);


  private:

    typedef std::map <std::string, SymbolInfoPtr> SymbolMap;
    typedef std::vector <std::string> StringStack;

    SymbolMap		_symbols;
    StringStack		_localNsStack;
    std::string		_globalNs;
    int			_i;
};


class LocalNamespace
{
  public:

     LocalNamespace (SymbolTable &st): _st (st) {st.pushLocalNamespace();}
    ~LocalNamespace () {_st.popLocalNamespace();}

  private:

    SymbolTable &	_st;
};


} // namespace Ctl

#endif
