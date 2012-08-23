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


#ifndef INCLUDED_CTL_INTERPRETER_H
#define INCLUDED_CTL_INTERPRETER_H

//-----------------------------------------------------------------------------
//
//	class Interpreter -- represents an instance of
//	the Color Transformation Language interpreter
//
//-----------------------------------------------------------------------------

#include <CtlFunctionCall.h>
#include <iostream>
#include <string>
#include <vector>

namespace Ctl {

class Parser;
class Module;
class LContext;
class SymbolTable;
class SymbolInfo;
typedef RcPtr<SymbolInfo> SymbolInfoPtr;

class Interpreter
{
  public:

    //-------------------------------------------------------
    // Constructor is protected; to get an Interpreter object
    // create an object of a derived class, for example
    // Ctl::SimdInterpreter.
    //-------------------------------------------------------

    virtual ~Interpreter ();


    //--------------------------------------------------------------
    // Load a module, test if a given module has already been loaded
    //--------------------------------------------------------------

    void		loadModule(const std::string &moduleName);
	void        loadFile(const std::string &fileName,
	                     const std::string &moduleName=std::string());
    bool		moduleIsLoaded (const std::string &moduleName) const;


    //--------------------------------------------------
    // Create an object that allows us to a CTL function
    //--------------------------------------------------

   FunctionCallPtr	newFunctionCall (const std::string &functionName);


    //----------------------------------------------------------
    // Get the maximum number of data samples a function call
    // can process in parallel.  Varying arguments to a function
    // call can contain at most maxSamples() data samples.
    //----------------------------------------------------------

    virtual size_t	maxSamples () const = 0;


    //---------------------------------------------------------------------
    // Forced termination of CTL programs:
    //
    // setMaxInstCount() sets the maximum number of instructions a CTL
    // program is allowed to execute.  Any CTL program under the control
    // of this interpreter terminates by throwing a C++ exception
    // (Ctl::MaxInstExc) as soon as it has executed its maximum number
    // of instructions.  What exactly an "instruction" is, and the
    // relationship between CTL source code and instructions depends
    // on how the interpreter is implemented.  A CTL statement typically
    // corresponds to multiple instructions.
    //
    // abortAllPrograms() terminates all CTL programs under the control
    // of this interpreter.  This function is intended to be used by multi-
    // threaded C++ programs.  If one thread calls abortAllPrograms(), all
    // CTL programs running in other C++ threads terminate immediately
    // by throwing a C++ exception (Ctl::AbortExc).
    //
    // Note that it is not safe to call abortAllPrograms() from a signal
    // handler; the call could cause the Interpreter to deadlock.
    //
    //---------------------------------------------------------------------

    virtual void	setMaxInstCount (unsigned long count) = 0;
    virtual void	abortAllPrograms () = 0;

    //---------------------------------------------------------------------
    // Module paths:
    //
    // The interpreter maintains a vector paths that are used to search for
    // ctl modules when loadModule is called.  Initially these paths are
    // loaded from the environment variable CTL_MODULE_PATH, but may be
    // examined and modified by modulePaths() and setModulePaths().
    //
    //---------------------------------------------------------------------
    static std::vector<std::string>  modulePaths();
    static void    setModulePaths(const std::vector<std::string>& newModPaths);


  protected:

    Interpreter ();

    SymbolTable &	symtab ();

  private:

    friend void			loadModuleRecursive
				    (Parser &Parser,
				     const std::string &moduleName);

    void			loadModuleRecursive
				    (const std::string &moduleName);

    bool			moduleIsLoadedInternal
				    (const std::string &moduleName) const;

    virtual FunctionCallPtr	newFunctionCallInternal 
                                    (const SymbolInfoPtr info,
                                     const std::string &functionName) = 0;

    virtual Module *		newModule
				    (const std::string &moduleName,
				     const std::string &fileName) = 0;

    virtual LContext *		newLContext
				    (std::istream &file,
				     Module *module,
				     SymbolTable &symtab) const = 0;
	void        _loadModule(const std::string &moduleName, const std::string &fileName);

    struct Data;

    Data *	_data;
};


} // namespace Ctl

#endif
