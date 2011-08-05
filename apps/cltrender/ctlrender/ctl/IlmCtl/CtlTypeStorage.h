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


#ifndef INCLUDED_CTL_TYPE_STORAGE_H
#define INCLUDED_CTL_TYPE_STORAGE_H

//-----------------------------------------------------------------------------
//
//	class TypeStorage
//
//	A TypeStorage object offers a easier to use C++ interface to the input
//  and output arguments of a CTL function.
//
//-----------------------------------------------------------------------------

#include <CtlRcPtr.h>
#include <string>
#include <vector>
#include <half.h>
#include <CtlType.h>

namespace Ctl {

class TypeStorage;
typedef RcPtr<TypeStorage> TypeStoragePtr;

class TypeStorage: public RcObject {
  public:
    //-------------------------------------------------------------
    // Constructor and destructor
    //
    // This is a common base class used for storage of data in a
	// specific type. The two subclasses FunctionArg and DataArg
	// should actually perform useful things. 
    //-------------------------------------------------------------

    TypeStorage(const std::string &name,
		        const DataTypePtr &type);
    virtual ~TypeStorage ();


    //-------------------------
    // The name of the argument
    //-------------------------

    const std::string &		name () const		{return _name;}


    //--------------------
    // The argument's type
    //--------------------

    const DataTypePtr &		type () const		{return _type;}


    //-----------------------------------------------------------------
    // isVarying() returns true if the argument is varying, or false
    // if the argument is uniform.  setVarying() switches the argument
    // between varying and uniform.  The effect of calling setVarying()
    // for a function's output arguments or return value is undefined.
    //-----------------------------------------------------------------

    virtual bool                isVarying() const=0;
    virtual void                setVarying(bool varying)=0;

	virtual size_t              elements(void) const=0;

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

    virtual bool		hasDefaultValue()=0;
    virtual void		setDefaultValue()=0;

	// Can perform a conversion between (almost) any two TypeStorage
	// managed types. It's very efficient if the types are the same, 
	// not so much if some sort of data conversion needed.
	void         copy(const TypeStoragePtr &src, size_t src_offset=0,
	                  size_t dst_offset=0, size_t count=0);

	// Some basic PODish set/get functions (can be used with either varying
	// or non-varying types). The dst_offset and count are used in the case
	// of *varying types*. If you are setting a value in a complex/array type
	// you use the path mechanism described in CtlType.h
	void         set(const bool *src, size_t src_stride=0, size_t dst_offset=0,
	                 size_t count=1, const std::string &path=std::string(),
	                 ...);
	void         setv(const bool *src, size_t src_stride, size_t dst_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         set(const int *src, size_t src_stride=0, size_t dst_offset=0,
	                 size_t count=1, const std::string &path=std::string(),
	                 ...);
	void         setv(const int *src, size_t src_stride, size_t dst_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         set(const unsigned int *src, size_t src_stride=0,
	                 size_t dst_offset=0, size_t count=1,
	                 const std::string &path=std::string(), ...);
	void         setv(const unsigned int *src, size_t src_stride,
	                  size_t dst_offset, size_t count,
	                  const std::string &path, va_list ap);

	void         set(const half *src, size_t src_stride=0, size_t dst_offset=0,
	                 size_t count=1, const std::string &path=std::string(),
	                 ...);
	void         setv(const half *src, size_t src_stride, size_t dst_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         set(const float *src, size_t src_stride=0,
	                 size_t dst_offset=0, size_t count=1,
	                 const std::string &path=std::string(), ...);
	void         setv(const float *src, size_t src_stride, size_t dst_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         set(const std::string *, size_t src_stride=0,
	                 size_t dst_offset=0, size_t count=1,
	                 const std::string &path=std::string(), ...);
	void         setv(const std::string *src, size_t src_stride,
	                  size_t dst_offset, size_t count,
	                  const std::string &path, va_list ap);


	void         get(const bool *dst, size_t dst_stride=0, size_t src_offset=0,
	                 size_t count=1, const std::string &path=std::string(),
	                 ...);
	void         getv(const bool *dst, size_t dst_stride, size_t src_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         get(const int *dst, size_t dst_stride=0, size_t src_offset=0,
	                 size_t count=1, const std::string &path=std::string(),
	                 ...);
	void         getv(const int *dst, size_t dst_stride, size_t src_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         get(const unsigned int *dst, size_t dst_stride=0,
	                 size_t src_offset=0, size_t count=1,
	                 const std::string &path=std::string(), ...);
	void         getv(const unsigned int *dst, size_t dst_stride,
	                  size_t src_offset, size_t count,
	                  const std::string &path, va_list ap);

	void         get(const half *dst, size_t dst_stride=0, size_t src_offset=0,
	                 size_t count=1, const std::string &path=std::string(),
	                 ...);
	void         getv(const half *dst, size_t dst_stride, size_t src_offset,
	                  size_t count, const std::string &path, va_list ap);

	void         get(const float *dst, size_t dst_stride=0,
	                 size_t src_offset=0, size_t count=1,
	                 const std::string &path=std::string(), ...);
	void         getv(const float *dst, size_t dst_stride,
	                  size_t src_offset, size_t count,
	                  const std::string &path, va_list ap);

	void         get(const std::string *, size_t dst_stride=0,
	                 size_t src_offset=0, size_t count=1,
	                 const std::string &path=std::string(), ...);
	void         getv(const std::string *dst, size_t dst_stride,
	                  size_t src_offset, size_t count,
	                  const std::string &path, va_list ap);

  private:
    std::string 		_name;
    DataTypePtr			_type;

	// Does heavy lifting of set / copy functions above.
	void         _set(const char *src, CDataType_e src_type, size_t stride,
	                  size_t dst_offset, size_t count, const std::string &,
	                  va_list ap);
	void         _get(char *dst, CDataType_e dst_type, size_t stride,
	                  size_t src_offset, size_t count, const std::string &,
	                  va_list ap);
};

} // namespace Ctl

#endif

