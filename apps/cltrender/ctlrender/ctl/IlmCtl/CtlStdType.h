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


#ifndef INCLUDED_CTL_STD_TYPE_H
#define INCLUDED_CTL_STD_TYPE_H

// These are a more C friendly set of types and a generic data storage element
// in the style of FunctionArg. All of the code generation methods are no-ops,
// since these all operate outside of the LContext space.

#include <CtlRcPtr.h>
#include <CtlType.h>
#include <CtlFunctionCall.h>

namespace Ctl {

class StdDataAddr;
typedef RcPtr<StdDataAddr> StdDataAddrPtr;

class StdDataType;
typedef RcPtr<StdDataType> StdDataTypePtr;

class StdArrayType;
typedef RcPtr<StdArrayType> StdArrayTypePtr;

class StdFunctionType;
typedef RcPtr<StdFunctionType> StdFunctionTypePtr;

class BoolType;
typedef RcPtr <BoolType> BoolTypePtr;

class IntType;
typedef RcPtr <IntType> IntTypePtr;

class UIntType;
typedef RcPtr <UIntType> UIntTypePtr;

class HalfType;
typedef RcPtr <HalfType> HalfTypePtr;

class FloatType;
typedef RcPtr <FloatType> FloatTypePtr;

class StringType;
typedef RcPtr <StringType> StringTypePtr;

class ArrayType;
typedef RcPtr <ArrayType> ArrayTypePtr;

class StructType;
typedef RcPtr <StructType> StructTypePtr;

class DataArg;
typedef RcPtr <DataArg> DataArgPtr;

class StdBoolType: public BoolType
{
  public:

    StdBoolType ();

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};


class StdIntType: public IntType
{
  public:
    StdIntType ();

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};

class StdUIntType: public UIntType
{
  public:
    StdUIntType ();

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};


class StdHalfType: public HalfType
{
  public:
    StdHalfType ();

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};


class StdFloatType: public FloatType
{
  public:
    StdFloatType ();

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};


class StdStringType: public StringType
{
  public:
    StdStringType ();

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};


class StdArrayType: public ArrayType
{
  public:
    StdArrayType(const DataTypePtr &elementType, int size);

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};


class StdStructType: public StructType {
  public:
	StdStructType(const std::string &name,
	              const MemberVector &members);

	virtual size_t	objectSize () const;
	virtual size_t	alignedObjectSize () const;
	virtual size_t	objectAlignment () const;

	virtual void	generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const;

	virtual void	generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const;

    AddrPtr         newStaticVariable (Module *module) const;

	void            newAutomaticVariable(StatementNodePtr node, 
	                                     LContext &lcontext) const;
};

class DataArg : public TypeStorage {
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
	DataArg(const std::string &name, const DataTypePtr &type,
	        size_t elements);

	virtual ~DataArg();

    //-----------------------------------------------------------------
    // isVarying() returns true if the argument is varying, or false
    // if the argument is uniform.  setVarying() switches the argument
    // between varying and uniform.  The effect of calling setVarying()
    // for a function's output arguments or return value is undefined.
    //-----------------------------------------------------------------
	virtual bool           isVarying() const;
    virtual void           setVarying(bool varying);

	virtual size_t         elements(void) const { return _elements; };

    //----------------------------------------------------------------
    // A pointer to a buffer for the argument's value.  Before calling
    // a CTL function, an applickkation program must store values for
    // the function's input arguments in the corresponding buffers.
    // When the CTL function returns, the application can read the
    // of the output argument values and the return value from the
    // corresponding buffers.
    //----------------------------------------------------------------
    virtual char *          data();

    virtual bool		hasDefaultValue () { return 0; };
    virtual void		setDefaultValue () {};

  private:
	size_t _elements;
	char *_data;
};

};

#endif
