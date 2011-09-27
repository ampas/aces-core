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


#ifndef INCLUDED_CTL_TYPE_H
#define INCLUDED_CTL_TYPE_H

//-----------------------------------------------------------------------------
//
//	Types in the color transformation language
//
//	CTL types are represented by the following class hierarchy:
//
//	    Type		base class for all types
//		DataType	base class for types that represent data
//		    VoidType    void
//		    BoolType	bool
//		    IntType	int
//		    UIntType	unsigned int
//		    HalfType	half
//		    FloatType	float
//		    StringType	character string
//		    ArrayType	arrays
//		    StructType	structs
//		FunctionType	function signatures
//
//-----------------------------------------------------------------------------

#include <CtlReadWriteAccess.h>
#include <CtlAddr.h>
#include <string>
#include <vector>
#include <stdarg.h>

namespace Ctl {

struct SyntaxNode;
typedef RcPtr <SyntaxNode> SyntaxNodePtr;

struct StatementNode;
typedef RcPtr <StatementNode> StatementNodePtr;

struct ExprNode;
typedef RcPtr <ExprNode> ExprNodePtr;

class Type;
typedef RcPtr <Type> TypePtr;

class DataType;
typedef RcPtr <DataType> DataTypePtr;

class VoidType;
typedef RcPtr <VoidType> VoidTypePtr;

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

class FunctionType;
typedef RcPtr <FunctionType> FunctionTypePtr;

enum CDataType_e {
	VoidTypeEnum=0,
	BoolTypeEnum=1,
	IntTypeEnum=2,
	UIntTypeEnum=3,
	HalfTypeEnum=4,
	FloatTypeEnum=5,
	StringTypeEnum=6,
	StructTypeEnum=7,
	ArrayTypeEnum=8
};

typedef std::vector<size_t> SizeVector;
typedef std::vector<size_t>::iterator SizeVectorIterator;
typedef std::vector<size_t>::reverse_iterator SizeVectorRIterator;

class LContext;
class XContext;
class Module;

class Type: public RcObject
{
  public:

    virtual ~Type ();


    //----------------------------------------
    // Test if type t is the same as this type
    //----------------------------------------

    virtual bool		isSameTypeAs (const TypePtr &t) const = 0;


    //-------------------------------------------------
    // Test if type t can be "promoted" to this type.
    // For example, int can be promoted to float, but
    // float cannot be promoted to int.
    // (Called by ExprNode::computeType())
    //-------------------------------------------------

    virtual bool		canPromoteFrom (const TypePtr &t) const = 0;


    //-----------------------------------------------------
    // Test if a cast from type t to this type is possible.
    // For example, int can be cast to float, and float can
    // be cast to int.
    // (Called by ExprNode::computeType())
    //-----------------------------------------------------

    virtual bool		canCastFrom (const TypePtr &t) const = 0;



    //-----------------------------------------------------
    // Test if an assignment is allowed between the two types.
    // Same as canCastFrom except returns fals for varrying
    // array types, string, function and void;
    //-----------------------------------------------------

    virtual bool		canAssign (const TypePtr &t) const = 0;


    //---------------------------------------------------
    // Attempt to evaluate a simple constant expression,
    // for example, of the form "operator operand" or
    // "operand operator operand", and produce a result
    // of this type.  Depending on whether evaluation
    // is successful or not, return either the result or
    // the original expression.
    // (Called by ExprNode::evaluate())
    //---------------------------------------------------
    
    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const = 0;


    //---------------------------------------------------
    // Attempt to cast the value of a constant expression
    // to this type.  Depending on whether the cast is
    // successful or not, return either the result or the
    // original expression.
    // (Called by ExprNode::evaluate())
    //---------------------------------------------------
    
    virtual ExprNodePtr		castValue (LContext &lcontext,
					   const ExprNodePtr &expr) const = 0;

	
    //--------------------------------
    // Print this type (for debugging)
    //--------------------------------

    virtual void		print (int indent) const = 0;


    //--------------------------------------------------------
    // Convert this type to a string for use in error messages
    //--------------------------------------------------------

    virtual std::string		asString () const = 0;


    //-------------------------------------------------------------
    // Output code that casts from the result of the given ExprNode
    // to this type
    //-------------------------------------------------------------

    virtual void		generateCastFrom (const ExprNodePtr &expr,
						  LContext &lcontext) const = 0;

    //-------------------------------------
    // Output code for the given SyntaxNode
    //-------------------------------------

    virtual void		generateCode (const SyntaxNodePtr &node,
					      LContext &lcontext) const = 0;

	// This is something of a cheat and mostly for internal use
	// to avoid doing RTTI for determining type conversions.
	virtual CDataType_e cDataType() const { return VoidTypeEnum; };

	// These methods are for dealing with complex and aggregate
	// type introspection. They have the concept of a 'path' where
	// that consists of a '/' separated list of element identifiers.
	// In the case of an array an element identifier is interpreted
	// as a number. In the case of a struct an element identifier is
	// interpreted as a structure item name. The following element
	// identifiers have special meaning (but do not conflict with CTL
	// syntax:
	//
	//   *  A null identifier or "." is a no-op to the parser. 
	//   *  If an identifier is "%s" then the next item on the var-args
	//      list is interpreted as a char * string and used as the element
	//      identifier.
	//   *  If an identifier is "%u", "%d" then the next item on the vargs
	//      list is interpreted as an integer and used as the element
	//      identifier.
	//   *  If an identifier is "%v" then the next item on the vargs list
	//      is interpreted as a SizeVector * and all elements are used to
	//      identify an element in a matrix.
	//
	static void         childElement(size_t *offset, TypePtr *type,
	                                 const std::string &path, ...);

	static void         childElementV(size_t *offset, TypePtr *type,
	                                  const std::string &path, va_list ap);
};


class DataType: public Type
{
  public:
    //------------------------------------------------------------
    // Size and required alignment of objects of this data type:
    //
    // Objects of this type occupy ojectSize() bytes, and the
    // first byte must be at an address that is divisible by
    // objectAlignment().  In an array of objects of this type,
    // the addresses of the array elements are alignedObjectSize()
    // bytes apart.
    //
    // alignObjectAddr(x) computes the smallest address, y,
    // such that y >= x and y is divisible by objectAlignment().
    //------------------------------------------------------------

    virtual size_t  objectSize() const=0;
    virtual size_t  alignedObjectSize() const=0;
    virtual size_t  objectAlignment() const=0;
    char *          alignObjectAddr(char *addr) const;

    //-------------------------------------------------------
    // For basic data types, appends alignedOjectSize.  For arrays 
    // and structs recurses through sub-elements to return number of 
    // non-struct, non-array elements.
    //-------------------------------------------------------
    virtual void    coreSizes(size_t parentOffset,
					          SizeVector &sizes, 
	                          SizeVector &offsets) const;


    //-------------------------------------------------------
    // Generate a new static variable of this type and return
    // the variable's address
    //-------------------------------------------------------

    virtual AddrPtr             newStaticVariable (Module *module) const = 0;
    virtual void                newAutomaticVariable 
                                     (StatementNodePtr node, 
				      LContext &lcontext) const = 0;
};


class VoidType: public DataType
{
  public:

    VoidType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return VoidTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;

    // arrays of void not allowed
    AddrPtr                     newStaticVariable (Module *module) const;
    void                        newAutomaticVariable 
                                     (StatementNodePtr node, 
				      LContext &lcontext) const;
};

class BoolType: public DataType
{
  public:

    BoolType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return IntTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;
};


class IntType: public DataType
{
  public:

    IntType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return IntTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;
};


class UIntType: public DataType
{
  public:

    UIntType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return UIntTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;
};


class HalfType: public DataType
{
  public:

    HalfType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return HalfTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;
};


class FloatType: public DataType
{
  public:

    FloatType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return FloatTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
 					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;
};


class StringType: public DataType
{
  public:

    StringType ();

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return StringTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
 					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;
};


class ArrayType: public DataType
{
  public:
    ArrayType (const DataTypePtr &elementType, int size);

    //---------------------------------------------------------------
    // For array types, objectSize() returns the size of the entire
    // array object.  elementSize() returns the size of an individual
    // array element, including padding for proper alignment
    // (elementSize() returns elementType()->alignedObjectSize()).
    // For example, if an array's elements occypy 10-bytes each, but
    // require four-byte alignment, then each element will be padded
    // with two extra bytes, and elementSize() returns 12.
    //---------------------------------------------------------------

    size_t			elementSize () const;

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return ArrayTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;

    const DataTypePtr &		elementType () const	{return _elementType;}
    int				size () const		{return _size;}

    // utility functions for arrays of arrays
    void			sizes (SizeVector &sizes) const;
    virtual void                coreSizes(size_t parentOffset,
					  SizeVector &sizes, 
					  SizeVector &offsets) const;

    //
    //  The underlying non-array type of an array.  e.g. for 
    //  a type int[3][2], returns IntType
    //
    DataTypePtr                 coreType() const;

  private:

    DataTypePtr _elementType;
    int         _size;
};


struct Member
{
     Member (const std::string &name, const DataTypePtr &type);
    ~Member ();

    std::string	name;
    DataTypePtr	type;
    size_t	offset;		// set by StructType
};


typedef std::vector <Member> MemberVector;
typedef std::vector <Member>::iterator MemberVectorIterator;
typedef std::vector <Member>::const_iterator MemberVectorConstIterator;


class StructType: public DataType
{
  public:

    StructType (const std::string &name,
		const MemberVector &members);

    const std::string &		name () const		{return _name;}
    const MemberVector &	members () const	{return _members;}
    virtual void                coreSizes(size_t parentOffset,
					  SizeVector &sizes, 
					  SizeVector &offsets) const;

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

	virtual CDataType_e cDataType() const { return StructTypeEnum; };

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;


  protected:

    Member &			member (size_t i)	{return _members[i];}

  private:

    const std::string		_name;
    MemberVector		_members;
};


struct Param
{
     Param (const std::string &name,
	    const DataTypePtr &type, 
	    const ExprNodePtr &defaultValue, 
	    ReadWriteAccess access,
	    bool varying);

    ~Param();

    bool isWritable() const;
    bool isReadable() const;

    std::string		name;
    DataTypePtr		type;
    ExprNodePtr		defaultValue;
    ReadWriteAccess	access;
    bool		varying;
};

typedef std::vector <Param> ParamVector;


class FunctionType: public Type
{
  public:

    FunctionType (const DataTypePtr &returnType,
		  bool returnVarying,
	          const ParamVector &parameters);

    const DataTypePtr &		returnType () const	{return _returnType;}
    bool			returnVarying () const	{return _returnVarying;}
    const ParamVector &		parameters () const	{return _parameters;}

    virtual bool		isSameTypeAs (const TypePtr &t) const;
    virtual bool		canAssign (const TypePtr &t) const;
    virtual bool		canPromoteFrom (const TypePtr &t) const;
    virtual bool		canCastFrom (const TypePtr &t) const;

    virtual ExprNodePtr		evaluate (LContext &lcontext,
					  const ExprNodePtr &expr) const;

    virtual ExprNodePtr		castValue (LContext &lcontext,
				           const ExprNodePtr &expr) const;

    virtual void		print (int indent) const;
    virtual std::string		asString () const;

  private:

    DataTypePtr			_returnType;
    bool			_returnVarying;
    ParamVector			_parameters;
};


} // namespace Ctl

#endif
