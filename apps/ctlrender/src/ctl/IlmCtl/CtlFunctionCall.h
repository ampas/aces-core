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


#ifndef INCLUDED_CTL_FUNCTION_CALL_H
#define INCLUDED_CTL_FUNCTION_CALL_H

//-----------------------------------------------------------------------------
//
//	class FunctionCall, class FunctionArg
//
//	A FunctionCall object allows a C++ program to set up input arguments
//	for a CTL function, to call the CTL function, and to access the
//	function's output arguments and return value.
//
//	The FunctionCall object points to FunctionArg objects that represent
//	the function's arguments and return value.  Each FunctionArg object
//	contains a pointer to the argument's data type, and a pointer to a
//	buffer for the argument's value.
//
//-----------------------------------------------------------------------------

#include <CtlRcPtr.h>
#include <string>
#include <vector>
#include <half.h>
#include <CtlType.h>
#include <CtlTypeStorage.h>

namespace Ctl {

class FunctionCall;
typedef RcPtr<FunctionCall> FunctionCallPtr;

class FunctionArg;
typedef RcPtr<FunctionArg> FunctionArgPtr;

class DataType;
typedef RcPtr<DataType> DataTypePtr;

class FunctionCall: public RcObject {
  public:

    //-----------------------------------------------------------------
    // Constructor and destructor
    //
    // The constructor and destructor are not meant to be called
    // directly by an application program.  In order to create
    // a new FunctionCall object, the application should call
    // Interpreter::newFunctionCall(), which returns a FunctionCallPtr
    // that points to the new FunctionCall object.  The FunctionCallPtr
    // is a reference-counting pointer; the FunctionCall object will
    // be deleted automatically when there are no more references to
    // the object.
    //-----------------------------------------------------------------

    FunctionCall (const std::string &name);
    virtual ~FunctionCall ();


    //------------------------------
    // Access to the function's name
    //------------------------------

    const std::string &		name () const		{return _name;}

    //---------------------------------------------------------------------
    // Access to the arguments for the function call
    //
    // inputArg(i) and outputArg(i) return a pointer to the ith input or
    // output argument of the CTL function represented by this FunctionCall
    // object; i must be less than numInputArgs() and numOutputArgs(),
    // respectively.
    //
    // returnValue() returns a pointer to the CTL function's return value.
    //
    // findInputArg(n) and findOutputArg(n) return a pointer to the input
    // or output argument with name n, or FunctionArgPtr(0) if the CTL
    // function has no argument with name n.
    //---------------------------------------------------------------------

    size_t		numInputArgs () const	{return _inputArgs.size();}
    size_t		numOutputArgs () const	{return _outputArgs.size();}

    const FunctionArgPtr &	inputArg (size_t i)	{return _inputArgs[i];}
    const FunctionArgPtr &	outputArg (size_t i)	{return _outputArgs[i];}
    const FunctionArgPtr &	returnValue ()		{return _returnValue;}

    FunctionArgPtr		findInputArg (const std::string &name);
    FunctionArgPtr		findOutputArg (const std::string &name);


    //---------------------------------------------------------------
    // Call the CTL function: The function reads the input parameters
    // and stores its results in the output parameters.
    // callFunction(n) concurrently processes n data samples.  Before
    // calling callFunction(n), the caller must have initialized each
    // varying input argument with n samples.  When the call returns,
    // all varying output arguments will contain n samples.  Uniform
    // input arguments must be initialized with a single sample, and
    // uniform output arguments will contain a single sample when the
    // call returns.
    //---------------------------------------------------------------

    virtual void		callFunction (size_t numSamples) = 0;

  protected:

    void		setInputArg (size_t i, const FunctionArgPtr &arg);
    void		setOutputArg (size_t i, const FunctionArgPtr &arg);
    void		setReturnValue (const FunctionArgPtr &rval);

  private:

    typedef std::vector<FunctionArgPtr> ArgVector;

    std::string		_name;
    ArgVector		_inputArgs;
    ArgVector		_outputArgs;
    FunctionArgPtr	_returnValue;
};

class FunctionArg: public TypeStorage {
  public:
    //-------------------------------------------------------------
    // Constructor and destructor
    //
    // The constructor and destructor are not meant to be called
    // directly by an application program.  Creating a FunctionCall
    // object automatically creates FunctionArg objects for the
    // function's arguments and return value.  Destroying the
    // FunctionCall object destroys the corresponding FunctionArg
    // objects.
    //-------------------------------------------------------------

    FunctionArg(const std::string &name,
	            const FunctionCallPtr &func,
	            const DataTypePtr &type,
	            bool varying);

    virtual ~FunctionArg ();


    //----------------------------------------------------------
    // The FunctionCall object to which this FunctionArg belongs
    //----------------------------------------------------------

    const FunctionCallPtr &	func () const		{return _func;}


    //-----------------------------------------------------------------
    // isVarying() returns true if the argument is varying, or false
    // if the argument is uniform.  setVarying() switches the argument
    // between varying and uniform.  The effect of calling setVarying()
    // for a function's output arguments or return value is undefined.
    //-----------------------------------------------------------------

    virtual bool                isVarying () const	{return _varying;}
    virtual void                setVarying (bool varying) {_varying = varying;}

    //----------------------------------------------------------------
    // A pointer to a buffer for the argument's value.  Before calling
    // a CTL function, an application program must store values for
    // the function's input arguments in the corresponding buffers.
    // When the CTL function returns, the application can read the
    // of the output argument values and the return value from the
    // corresponding buffers.
    //----------------------------------------------------------------
    virtual char *		data ()	= 0;


    //-----------------------------------------------------------
    // hasDefaultValue() returns true if an input argument has a
    // default value.  setDefaultValue() makes the input argument
    // equal to the default value.
    //-----------------------------------------------------------

    virtual bool		hasDefaultValue () = 0;
    virtual void		setDefaultValue () = 0;

  private:
    FunctionCallPtr		_func;
    bool                _varying;
};


} // namespace Ctl

#endif

