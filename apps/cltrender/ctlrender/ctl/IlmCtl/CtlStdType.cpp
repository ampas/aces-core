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

#include <CtlStdType.h>
#include <cassert>
#include <CtlErrors.h>
#include <half.h>
#include <CtlExc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

namespace Ctl {

StdBoolType::StdBoolType() : BoolType() {
}

size_t StdBoolType::objectSize() const {
	return sizeof(unsigned char);
}

size_t StdBoolType::alignedObjectSize() const {
	return sizeof(unsigned char);
}

size_t StdBoolType::objectAlignment() const {
	return sizeof(unsigned char);;
}

void StdBoolType::generateCastFrom(const ExprNodePtr &expr,
	                               LContext &lcontext) const {
}

void StdBoolType::generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const {
}

AddrPtr StdBoolType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdBoolType::newAutomaticVariable(StatementNodePtr node, 
	                                   LContext &lcontext) const {
}

StdIntType::StdIntType() : IntType() {
}

size_t StdIntType::objectSize() const {
	return sizeof(int);
}

size_t StdIntType::alignedObjectSize() const {
	return sizeof(int);
}

size_t StdIntType::objectAlignment() const {
	return sizeof(int);
}

void StdIntType::generateCastFrom(const ExprNodePtr &expr,
	                              LContext &lcontext) const {
}

void StdIntType::generateCode(const SyntaxNodePtr &node,
	                          LContext &lcontext) const {
}

AddrPtr StdIntType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdIntType::newAutomaticVariable(StatementNodePtr node, 
	                                  LContext &lcontext) const {
}

StdUIntType::StdUIntType() : UIntType() {
}

size_t StdUIntType::objectSize() const {
	return sizeof(unsigned int);
}

size_t StdUIntType::alignedObjectSize() const {
	return sizeof(unsigned int);
}

size_t StdUIntType::objectAlignment() const {
	return sizeof(unsigned int);
}

void StdUIntType::generateCastFrom(const ExprNodePtr &expr,
	                               LContext &lcontext) const {
}

void StdUIntType::generateCode(const SyntaxNodePtr &node,
	                           LContext &lcontext) const {
}

AddrPtr StdUIntType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdUIntType::newAutomaticVariable(StatementNodePtr node, 
	                                    LContext &lcontext) const {
}

StdHalfType::StdHalfType() : HalfType() {
}

size_t StdHalfType::objectSize() const {
	return sizeof(half);
}

size_t StdHalfType::alignedObjectSize() const {
	return sizeof(half);
}

size_t StdHalfType::objectAlignment() const {
	return sizeof(half);
}

void StdHalfType::generateCastFrom(const ExprNodePtr &expr,
	                               LContext &lcontext) const {
}

void StdHalfType::generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const {
}

AddrPtr StdHalfType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdHalfType::newAutomaticVariable(StatementNodePtr node, 
	                                   LContext &lcontext) const {
}

StdFloatType::StdFloatType() : FloatType() {
}

size_t StdFloatType::objectSize() const {
	return sizeof(float);
}

size_t StdFloatType::alignedObjectSize() const {
	return sizeof(float);
}

size_t StdFloatType::objectAlignment() const {
	return sizeof(float);
}

void StdFloatType::generateCastFrom(const ExprNodePtr &expr,
	                                LContext &lcontext) const {
}

void StdFloatType::generateCode(const SyntaxNodePtr &node,
	                            LContext &lcontext) const {
}

AddrPtr StdFloatType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdFloatType::newAutomaticVariable(StatementNodePtr node, 
	                                    LContext &lcontext) const {
}

StdStringType::StdStringType() : StringType() {
}

size_t StdStringType::objectSize() const {
	return sizeof(std::string);
}

size_t StdStringType::alignedObjectSize() const {
	return sizeof(std::string);
}

size_t StdStringType::objectAlignment() const {
	return 8;
}

void StdStringType::generateCastFrom(const ExprNodePtr &expr,
	                                LContext &lcontext) const {
}

void StdStringType::generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const {
}

AddrPtr StdStringType::newStaticVariable(Module *module) const {
	return AddrPtr();
}

void StdStringType::newAutomaticVariable(StatementNodePtr node, 
	                                    LContext &lcontext) const {
}

StdArrayType::StdArrayType(const DataTypePtr &elementType, int size) :
      ArrayType(elementType, size) {
}

size_t StdArrayType::objectSize() const {
	return size()*elementSize();
}

size_t StdArrayType::alignedObjectSize() const {
	return elementSize();
}

size_t StdArrayType::objectAlignment() const {
	return elementSize();
}

void StdArrayType::generateCastFrom(const ExprNodePtr &expr,
	                                LContext &lcontext) const {
}

void StdArrayType::generateCode(const SyntaxNodePtr &node,
	                            LContext &lcontext) const {
}

AddrPtr StdArrayType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdArrayType::newAutomaticVariable(StatementNodePtr node, 
	                                    LContext &lcontext) const {
}

StdStructType::StdStructType(const std::string &name,
                             const MemberVector &members) :
                            StructType(name, members) {
}

size_t StdStructType::objectSize() const {
	return sizeof(unsigned char);
}

size_t StdStructType::alignedObjectSize() const {
	return sizeof(unsigned char);
}

size_t StdStructType::objectAlignment() const {
	return 4;
}

void StdStructType::generateCastFrom(const ExprNodePtr &expr,
	                                 LContext &lcontext) const {
}

void StdStructType::generateCode(const SyntaxNodePtr &node,
	                             LContext &lcontext) const {
}

AddrPtr StdStructType::newStaticVariable (Module *module) const {
	return AddrPtr();
}

void StdStructType::newAutomaticVariable(StatementNodePtr node, 
	                                   LContext &lcontext) const {
}

DataArg::DataArg(const std::string &name, const DataTypePtr &type,
	             size_t elements) : TypeStorage(name, type) {
	_elements=elements;
	_data=(char *)malloc(type->objectSize()*_elements);
//	fprintf(stderr, "malloc %p %s/(%d of %s (%d))\n", _data,
//	        name.c_str(), elements, type->asString().c_str(),
//	        type->objectSize());
	memset(_data, 0, type->objectSize()*_elements);
}

DataArg::~DataArg() {
	// This is kind of a problem. Causes strings to leak...
	if(_data!=NULL) {
		free(_data);
	}
	_data=NULL;
	_elements=0;
}

bool DataArg::isVarying() const {
	return _elements>1;
}

void DataArg::setVarying(bool varying) {
	throw(RuntimeExc("unable to set 'varying' flag on DataArg storage"));
}

char *DataArg::data() {
	return _data;
}

};
