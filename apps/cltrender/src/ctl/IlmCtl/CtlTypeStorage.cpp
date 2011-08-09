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
//	class FunctionCall, class FunctionArg
//
//-----------------------------------------------------------------------------

#include <CtlTypeStorage.h>
#include <string>
#include <half.h>
#include <CtlExc.h>
#include <string.h>
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

using namespace std;

namespace Ctl {

TypeStorage::TypeStorage(const string &name,
     const DataTypePtr &type) : _name (name), _type (type) {
}

TypeStorage::~TypeStorage() {
}

static std::string strprintf(const char *fmt, ...) {
	char *ptr;
	int length=1024;
	int need_len;
	va_list ap;

	while(1) {
		va_start(ap, fmt);
		ptr=(char *)alloca(length);
		memset(ptr, 0, length);
		need_len=vsnprintf(ptr, length, fmt, ap);
		va_end(ap);
		if(need_len<length && need_len!=-1) {
			break;
		}
		if(need_len==-1) {
			length=length*2;
		} else {
			length=need_len+2;
		}
	}

	return std::string(ptr);
}

void _copy(char *out, const char *in, const DataTypePtr &out_type,
           const DataTypePtr &in_type);

// _convert performs conversions amoung PODish types. Not terribly fast.
void _convert(void *out, const void *in, CDataType_e out_type,
              CDataType_e in_type) {
	if(in_type==BoolTypeEnum) {
		if(out_type==BoolTypeEnum) {
			*((bool *)out)=!!*((bool *)in);
		} else if(out_type==IntTypeEnum) {
			*((bool *)out)=*((bool *)in);
		} else if(out_type==UIntTypeEnum) {
			*((bool *)out)=*((bool *)in);
		} else if(out_type==HalfTypeEnum) {
			*((bool *)out)=*((bool *)in);
		} else if(out_type==FloatTypeEnum) {
			*((bool *)out)=*((bool *)in);
		} else if(out_type==StringTypeEnum) {
			*((std::string *)out)=*((bool *)in) ? "true" : "false";
		}
	} else if(in_type==IntTypeEnum) {
		if(out_type==BoolTypeEnum) {
			*((bool *)out)=!!*((int *)in);
		} else if(out_type==IntTypeEnum) {
			*((int *)out)=*((int *)in);
		} else if(out_type==UIntTypeEnum) {
			*((unsigned int *)out)=*((int *)in);
		} else if(out_type==HalfTypeEnum) {
			*((half  *)out)=*((int *)in);
		} else if(out_type==FloatTypeEnum) {
			*((float *)out)=*((int *)in);
		} else if(out_type==StringTypeEnum) {
			*((std::string *)out)=strprintf("%d", *((int *)in));
		}
	} else if(in_type==UIntTypeEnum) {
		if(out_type==BoolTypeEnum) {
			*((bool *)out)=!!*((unsigned int *)in);
		} else if(out_type==IntTypeEnum) {
			*((int *)out)=*((unsigned int *)in);
		} else if(out_type==UIntTypeEnum) {
			*((unsigned int *)out)=*((unsigned int *)in);
		} else if(out_type==HalfTypeEnum) {
			*((half *)out)=*((unsigned int *)in);
		} else if(out_type==FloatTypeEnum) {
			*((float *)out)=*((unsigned int *)in);
		} else if(out_type==StringTypeEnum) {
			*((std::string *)out)=strprintf("%u", *((unsigned int *)in));
		}
	} else if(in_type==HalfTypeEnum) {
		if(out_type==BoolTypeEnum) {
			*((bool *)out)=!!*((half *)in);
		} else if(out_type==IntTypeEnum) {
			*((int *)out)=*((half *)in);
		} else if(out_type==UIntTypeEnum) {
			*((unsigned int *)out)=*((half *)in);
		} else if(out_type==HalfTypeEnum) {
			*((half *)out)=*((half *)in);
		} else if(out_type==FloatTypeEnum) {
			*((float *)out)=*((half *)in);
		} else if(out_type==StringTypeEnum) {
			*((std::string *)out)=strprintf("%f",
			                                (float)(*((unsigned int *)in)));
		}
	} else if(in_type==FloatTypeEnum) {
		if(out_type==BoolTypeEnum) {
			*((bool *)out)=!!*((float *)in);
		} else if(out_type==IntTypeEnum) {
			*((int *)out)=(int)*((float *)in);
		} else if(out_type==UIntTypeEnum) {
			*((unsigned int *)out)=(unsigned int)*((float *)in);
		} else if(out_type==HalfTypeEnum) {
			*((half *)out)=*((float *)in);
		} else if(out_type==FloatTypeEnum) {
			*((float *)out)=*((float *)in);
		} else if(out_type==StringTypeEnum) {
			*((std::string *)out)=strprintf("%f", *((float *)in));
		} 
	} else if(in_type==StringTypeEnum) {
		if(out_type==in_type) {
			*((std::string *)out)=*((const std::string *)in);
		} else {
			CDataType_e intermediate;
			int int_i;
			unsigned int int_u;
			float int_f;
			const char *str;

			str=((const std::string *)in)->c_str();
			switch(out_type) {
				case BoolTypeEnum:
					intermediate=IntTypeEnum;
					if(strchr("yYtT", str[0])!=NULL) {
						int_i=1;
						break;
					} else if(strchr("nNfF", str[0])!=NULL) {
						int_i=0;
						break;
					}
					// FALL THROUGH
		
				case IntTypeEnum:
					int_i=strtol(str, NULL, 0);
					intermediate=IntTypeEnum;
					break;
					
		
				case UIntTypeEnum:
					intermediate=UIntTypeEnum;
					int_u=strtoul(str, NULL, 0);
					break;
		
				case HalfTypeEnum:
				case FloatTypeEnum:
					intermediate=FloatTypeEnum;
					int_f=strtof(str, NULL);
					break;

				default:
					// should never reach here.
					break;
			}
		
			switch(intermediate) {
				case IntTypeEnum:
					_convert(out, &int_i, out_type, intermediate);
					break;
		
				case UIntTypeEnum:
					_convert(out, &int_u, out_type, intermediate);
					break;
		
				case FloatTypeEnum:
					_convert(out, &int_f, out_type, intermediate);
					break;

				default:
					// should never reach here.
					break;
			}
		}
	}
}

// Zeros a type. Scalars are set to 0, Strings are set to '',
// Arrays / structs have all of their entries zero'd
void _clear(char *out, const DataTypePtr &type) {
	StructTypePtr struct_type;
	ArrayTypePtr array_type;
	SizeVector sizes;
	uint32_t i, count;

	if(type->cDataType()==ArrayTypeEnum) {
		array_type=type;
		array_type->sizes(sizes);
		for(i=0; i<sizes[0]; i++) {
			_clear(out, array_type->elementType());
			out=out+type->alignedObjectSize();
		}
	} else if(type->cDataType()==StructTypeEnum) {
		struct_type=type;

		count=struct_type->members().size();
		for(i=0; i<count; i++) {
			_clear(out+struct_type->members()[i].offset,
			       struct_type->members()[i].type);
		}
	} else if(type->cDataType()==VoidTypeEnum) {
		// just skip this one...
	} else if(type->cDataType()==StructTypeEnum) {
		// Probably the right thing to do...
		if(out==NULL) {
			out=(char *)(new std::string());
		} else {
			*((std::string *)out)="";
		}
	} else {
		memset(out, 0, type->alignedObjectSize());
	}
}

// Copies two arrays / matricies. Can handle arrays / matricies that are not
// of equal size (in the case of copying into a larger array, the 'extra'
// samples are set to '0' if the type is numeric, or .
void _copy_array(char *out, const char *in, const DataTypePtr &_dst_type,
                 const DataTypePtr &_src_type) {
	ArrayTypePtr dst_type=_dst_type;
	ArrayTypePtr src_type=_src_type;
	SizeVector src_sizes;
	SizeVector dst_sizes;
	uint32_t i;

	dst_type->sizes(dst_sizes);
	src_type->sizes(src_sizes);
	for(i=0; i<dst_sizes[0]; i++) {
		if(i>=src_sizes[0]) {
//			fprintf(stderr, "clearning element %d\n", i);
			_clear(out, _dst_type); 
		} else {
//			fprintf(stderr, "copying element %d\n", i);
			_copy(out, in, dst_type->elementType(), src_type->elementType());
			in=in+src_type->elementSize();
		}
		out=out+dst_type->elementSize();
	}
}

// We can only copy structs that have identical types. So we don't actually
// check on the type here (_copy takes care of that). Note that the types
// can be identical but stored differently, so we need both the src and
// dst types.
void _copy_struct(char *out, const char *in, const DataTypePtr &dst_type,
                  const DataTypePtr &src_type) {
	StructTypePtr type=dst_type;
	uint32_t i, count;

	count=type->members().size();
	for(i=0; i<count; i++) {
		_copy(out+type->members()[i].offset, in+type->members()[i].offset,
		      type->members()[i].type, type->members()[i].type);
	}
}

// Spam a value across an entire array...
void _spam_array(char *out, const char *in, const DataTypePtr &_dst_type,
                const DataTypePtr &src_type) {
	ArrayTypePtr dst_type=_dst_type;
	SizeVector dst_sizes;
	uint32_t i;

	dst_type->sizes(dst_sizes);
	for(i=0; i<dst_sizes[0]; i++) {
		_copy(out, in, dst_type->elementType(), src_type);
		out=out+dst_type->objectSize();
	}
}

void _copy(char *out, const char *in, const DataTypePtr &out_type,
           const DataTypePtr &in_type) {
	ArrayTypePtr array;

//	fprintf(stderr, "copy %s@%p -> %s@%p\n", in_type->asString().c_str(), in, out_type->asString().c_str(), out);
	if(out_type->cDataType()==ArrayTypeEnum) {
		if(in_type->cDataType()!=ArrayTypeEnum) {
			_spam_array(out, in, out_type, in_type);
		} else {
			_copy_array(out, in, out_type, in_type);
		}
	} else if(out_type->cDataType()==StructTypeEnum) {
		if(out_type->isSameTypeAs(in_type)) {
			_copy_struct(out, in, out_type, in_type);
		} else {
			throw(DatatypeExc("unable to copy between structures of type '%s' and '%s'", out_type->asString().c_str(), in_type->asString().c_str()));
		}
	} else if(out_type->cDataType()==VoidTypeEnum) {
		throw(DatatypeExc("unable to copy into a void type"));
	} else {
		if(in_type->cDataType()==StructTypeEnum) {
			throw(DatatypeExc("unable convert a structure '%s' into a '%s'", out_type->asString().c_str(), in_type->asString().c_str()));
		} else if(in_type->cDataType()==VoidTypeEnum) {
			_clear(out, out_type);
		} else if(in_type->cDataType()==ArrayTypeEnum) {
			array=in_type;
			_copy(out, in, out_type, array->elementType());
		} else {
			// It's a POD.
			_convert(out, in, out_type->cDataType(), in_type->cDataType());
		}
	}
}

void _set_spam(char *out, const char *in,
               const DataTypePtr &out_type, const DataTypePtr &in_type,
               uint32_t count) {
	uint32_t i;

	for(i=0; i<count; i++) {
		_copy(out, in, out_type, in_type);
		out=out+out_type->alignedObjectSize();
	}
}

void TypeStorage::_set(const char *src, CDataType_e src_type,
	                   size_t src_stride, size_t dst_offset, size_t count,
                       const std::string &path, va_list ap) {
	const char *in;
	char *out;
	size_t u;
	size_t path_offset;
	TypePtr _data_type;
	DataTypePtr data_type;

	_data_type=type();
	path_offset=0;
	Type::childElementV(&path_offset, &_data_type, path, ap);
	data_type=_data_type;
	if(data_type->cDataType()!=BoolTypeEnum &&
	   data_type->cDataType()!=FloatTypeEnum &&
	   data_type->cDataType()!=IntTypeEnum &&
	   data_type->cDataType()!=UIntTypeEnum &&
	   data_type->cDataType()!=HalfTypeEnum &&
	   data_type->cDataType()!=StringTypeEnum) {
		throw(DatatypeExc("unable to set type %s via direct C++ interface (bad element path perhaps?)", data_type->asString().c_str()));
	}

	in=src;
	out=data()+dst_offset*type()->objectSize()+path_offset;

	// All of the simple copies which simply revolve around (possibly)
	// just changing the stride.
	if(data_type->cDataType()==src_type) {
		if(src_type==StringTypeEnum) {
			for(u=0; u<count; u++) {
				*((std::string *)out)=*((const std::string *)in);
				in=in+src_stride;
				out=out+type()->objectSize();
			}
			return;
		}
		if(data_type->objectSize()==src_stride &&
		   data_type->alignedObjectSize()==src_stride) {
			memcpy(out, in, data_type->objectSize()*count);
		} else if(data_type->objectSize()==sizeof(char)) {
			for(u=0; u<count; u++) {
				*((char *)out)=*((const char *)in);
				in=in+src_stride;
				out=out+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(short)) {
			for(u=0; u<count; u++) {
				*((short *)out)=*((const short *)in);
				in=in+src_stride;
				out=out+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(int)) {
			for(u=0; u<count; u++) {
				*((int *)out)=*((const int *)in);
				in=in+src_stride;
				out=out+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(long int)) {
			for(u=0; u<count; u++) {
				*((long int *)out)=*((const long int *)in);
				in=in+src_stride;
				out=out+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(long long int)) {
			for(u=0; u<count; u++) {
				*((long long int *)out)=*((const long long int *)in);
				in=in+src_stride;
				out=out+type()->objectSize();
			}
		} else {
			throw(DatatypeExc("unexpected data objectSize (%d)", data_type->objectSize()));
		}
		return;
	}

	for(u=0; u<count; u++) {
		_convert(out, in, type()->cDataType(),
		         src_type);
		in=in+src_stride;
		out=out+type()->objectSize();
	}
}

void TypeStorage::_get(char *dst, CDataType_e dst_type,
	                   size_t dst_stride, size_t src_offset, size_t count,
                       const std::string &path, va_list ap) {
	const char *in;
	char *out;
	size_t u;
	size_t path_offset;
	TypePtr _data_type;
	DataTypePtr data_type;

	_data_type=type();
	path_offset=0;
	Type::childElementV(&path_offset, &_data_type, path, ap);
	data_type=_data_type;
	if(data_type->cDataType()!=BoolTypeEnum &&
	   data_type->cDataType()!=FloatTypeEnum &&
	   data_type->cDataType()!=IntTypeEnum &&
	   data_type->cDataType()!=UIntTypeEnum &&
	   data_type->cDataType()!=HalfTypeEnum &&
	   data_type->cDataType()!=StringTypeEnum) {
		throw(DatatypeExc("unable to get type %s via direct C++ interface (bad element path perhaps?)", data_type->asString().c_str()));
	}

	out=dst;
	in=data()+src_offset*type()->objectSize()+path_offset;

	// All of the simple copies which simply revolve around (possibly)
	// just changing the stride.
	if(data_type->cDataType()==dst_type) {
		if(dst_type==StringTypeEnum) {
			for(u=0; u<count; u++) {
				*((std::string *)out)=*((const std::string *)in);
				out=out+dst_stride;
				in=in+type()->objectSize();
			}
			return;
		}
		if(data_type->objectSize()==dst_stride &&
		   data_type->alignedObjectSize()==dst_stride) {
			memcpy(out, in, data_type->objectSize()*count);
		} else if(data_type->objectSize()==sizeof(char)) {
			for(u=0; u<count; u++) {
				*((char *)out)=*((const char *)in);
				out=out+dst_stride;
				in=in+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(short)) {
			for(u=0; u<count; u++) {
				*((short *)out)=*((const short *)in);
				out=out+dst_stride;
				in=in+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(int)) {
			for(u=0; u<count; u++) {
				*((int *)out)=*((const int *)in);
				out=out+dst_stride;
				in=in+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(long int)) {
			for(u=0; u<count; u++) {
				*((long int *)out)=*((const long int *)in);
				out=out+dst_stride;
				in=in+type()->objectSize();
			}
		} else if(data_type->objectSize()==sizeof(long long int)) {
			for(u=0; u<count; u++) {
				*((long long int *)out)=*((const long long int *)in);
				out=out+dst_stride;
				in=in+type()->objectSize();
			}
		} else {
			throw(DatatypeExc("unexpected data objectSize (%d)", data_type->objectSize()));
		}
		return;
	}

	for(u=0; u<count; u++) {
		_convert(out, in, dst_type, type()->cDataType());
		out=out+dst_stride;
		in=in+type()->objectSize();
	}
}

// Determines if our types and alignment match well enough that we can
// do memcpy.
bool _can_fastcopy(const DataTypePtr &a, const DataTypePtr &b) {
	ArrayTypePtr a_array, b_array;
	SizeVector a_size, b_size;
	StructTypePtr a_struct, b_struct;
	uint32_t u, count;

	if(a->cDataType()==StringTypeEnum || b->cDataType()==StringTypeEnum ||
	   a->cDataType()!=b->cDataType() ||
	   a->cDataType()==VoidTypeEnum ||
	   a->alignedObjectSize()!=b->alignedObjectSize() ||
	   a->objectSize()!=b->objectSize() ||
	   a->objectSize()!=a->alignedObjectSize()) {
		return 0;
	}

	if(a->cDataType()==BoolTypeEnum ||
	   a->cDataType()==FloatTypeEnum ||
	   a->cDataType()==IntTypeEnum ||
	   a->cDataType()==UIntTypeEnum ||
	   a->cDataType()==HalfTypeEnum) {
		return 1;
	}

	if(a->cDataType()==ArrayTypeEnum) {
		a_array=a;
		b_array=b;
		a_array->sizes(a_size);
		b_array->sizes(b_size);
		if(a_size[0]==a_size[0]) {
			return _can_fastcopy(a_array->elementType(),
			                     b_array->elementType());
		}
		return 0;
	}
	if(a->cDataType()==StructTypeEnum) {
		if(!a->isSameTypeAs(b)) {
			return 0;
		}	

		a_struct=a;
		b_struct=b;

		count=a_struct->members().size();
		if(count!=b_struct->members().size()) {
			return 0;
		}
		for(u=0; u<count; u++) {
			if(!_can_fastcopy(a_struct->members()[u].type,
			                  b_struct->members()[u].type)) {
				return 0;
			}
		}
		return 1;
	}
	throw(DatatypeExc("unexpectedly found ourselves at the end of fastcopy"));
}

void TypeStorage::copy(const TypeStoragePtr &src, size_t src_offset,
	                   size_t dst_offset, size_t count) {
	const char *in;
	char *out;
	size_t u;

	in=src->data()+src_offset*src->type()->objectSize();
	out=data()+dst_offset*type()->objectSize();

//	fprintf(stderr, "copy from: %s@%p (%s/%d) to %s@%p (%s/%d) (%d/%d/%d)\n",
//	src->name().c_str(),
//	in,
//	src->type()->asString().c_str(),
//	src->type()->objectSize(),
//	name().c_str(),
//	out,
//	type()->asString().c_str(),
//	type()->objectSize(),
//	src_offset,
//	dst_offset,
//	count);

	if(_can_fastcopy(type(), src->type())) {
//		fprintf(stderr, "fastcopy %p %p %d\n", out, in, count*type()->objectSize());
		memcpy(out, in, count*type()->objectSize());
		return;
	}

	if((type()->cDataType()==BoolTypeEnum ||
	    type()->cDataType()==FloatTypeEnum ||
	    type()->cDataType()==IntTypeEnum ||
	    type()->cDataType()==UIntTypeEnum ||
	    type()->cDataType()==HalfTypeEnum ||
	    type()->cDataType()==StringTypeEnum) && 
	   (src->type()->cDataType()==BoolTypeEnum ||
	    src->type()->cDataType()==FloatTypeEnum ||
	    src->type()->cDataType()==IntTypeEnum ||
	    src->type()->cDataType()==UIntTypeEnum ||
	    src->type()->cDataType()==HalfTypeEnum ||
	    src->type()->cDataType()==StringTypeEnum)) {
        va_list empty;
//		fprintf(stderr, "_set %p %d %d %d %d %d\n", out, src->type()->cDataType(), type()->cDataType(), type()->objectSize(), dst_offset, count);
		_set(in, src->type()->cDataType(), src->type()->objectSize(),
		     dst_offset, count, "", empty);
		return;
	}

	for(u=0; u<count; u++) {
//		fprintf(stderr, "copy %s@%p -> %s@%p %d of %d\n", in, src->type()->asString().c_str(), out, type()->asString().c_str(), u, count);
		_copy(out, in, type(), src->type());
		in=in+src->type()->objectSize();
		out=out+type()->objectSize();
	}
}

void TypeStorage::set(const bool *src, size_t src_stride, size_t dst_offset,
	                 size_t count, const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	setv(src, src_stride, dst_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::setv(const bool *src, size_t src_stride, size_t dst_offset,
                       size_t count, const std::string &path, va_list ap) {
	_set((const char *)src, BoolTypeEnum, src_stride, dst_offset, count, path,
	     ap);
}

void TypeStorage::set(const int *src, size_t src_stride, size_t dst_offset,
	                 size_t count, const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	setv(src, src_stride, dst_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::setv(const int *src, size_t src_stride, size_t dst_offset,
                       size_t count, const std::string &path, va_list ap) {
	_set((const char *)src, IntTypeEnum, src_stride, dst_offset, count, path,
	     ap);
}

void TypeStorage::set(const unsigned int *src, size_t src_stride,
	                 size_t dst_offset, size_t count,
	                 const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	setv(src, src_stride, dst_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::setv(const unsigned int *src, size_t src_stride,
                       size_t dst_offset, size_t count,
                       const std::string &path, va_list ap) {
	_set((const char *)src, UIntTypeEnum, src_stride, dst_offset, count, path,
	     ap);
}

void TypeStorage::set(const half *src, size_t src_stride, size_t dst_offset,
	                 size_t count, const std::string &path,
	                 ...) {
	va_list ap;

	va_start(ap, path);
	setv(src, src_stride, dst_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::setv(const half *src, size_t src_stride, size_t dst_offset,
                       size_t count, const std::string &path,
                       va_list ap) {
	_set((const char *)src, HalfTypeEnum, src_stride, dst_offset, count, path,
	     ap);
}

void TypeStorage::set(const float *src, size_t src_stride,
	                 size_t dst_offset, size_t count,
	                 const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	setv(src, src_stride, dst_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::setv(const float *src, size_t src_stride,
                       size_t dst_offset, size_t count,
                       const std::string &path, va_list ap) {
	_set((const char *)src, FloatTypeEnum, src_stride, dst_offset, count,
	     path, ap);
}

void TypeStorage::set(const std::string *src, size_t src_stride,
	                 size_t dst_offset, size_t count,
	                 const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	setv(src, src_stride, dst_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::setv(const std::string *src, size_t src_stride,
                       size_t dst_offset, size_t count,
                       const std::string &path, va_list ap) {
	_set((const char *)src, StringTypeEnum, src_stride, dst_offset, count,
	     path, ap);
}


void TypeStorage::get(const bool *dst, size_t dst_stride, size_t src_offset,
	                 size_t count, const std::string &path,
	                 ...) {
	va_list ap;

	va_start(ap, path);
	getv(dst, dst_stride, src_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::getv(const bool *dst, size_t dst_stride, size_t src_offset,
                       size_t count, const std::string &path,
                       va_list ap) {
	_get((char *)dst, BoolTypeEnum, dst_stride, src_offset, count, path, ap);
}

void TypeStorage::get(const int *dst, size_t dst_stride, size_t src_offset,
	                 size_t count, const std::string &path,
	                 ...) {
	va_list ap;

	va_start(ap, path);
	getv(dst, dst_stride, src_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::getv(const int *dst, size_t dst_stride, size_t src_offset,
                       size_t count, const std::string &path,
                       va_list ap) {
	_get((char *)dst, IntTypeEnum, dst_stride, src_offset, count, path, ap);
}

void TypeStorage::get(const unsigned int *dst, size_t dst_stride,
	                 size_t src_offset, size_t count,
	                 const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	getv(dst, dst_stride, src_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::getv(const unsigned int *dst, size_t dst_stride,
                       size_t src_offset, size_t count,
                       const std::string &path, va_list ap) {
	_get((char *)dst, UIntTypeEnum, dst_stride, src_offset, count, path, ap);
}

void TypeStorage::get(const half *dst, size_t dst_stride, size_t src_offset,
	                 size_t count, const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	getv(dst, dst_stride, src_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::getv(const half *dst, size_t dst_stride, size_t src_offset,
                       size_t count, const std::string &path, va_list ap) {
	_get((char *)dst, HalfTypeEnum, dst_stride, src_offset, count, path, ap);
}

void TypeStorage::get(const float *dst, size_t dst_stride,
	                 size_t src_offset, size_t count,
	                 const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	getv(dst, dst_stride, src_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::getv(const float *dst, size_t dst_stride,
                       size_t src_offset, size_t count,
                       const std::string &path, va_list ap) {
	_get((char *)dst, FloatTypeEnum, dst_stride, src_offset, count, path, ap);
}

void TypeStorage::get(const std::string *dst, size_t dst_stride,
                      size_t src_offset, size_t count,
                      const std::string &path, ...) {
	va_list ap;

	va_start(ap, path);
	getv(dst, dst_stride, src_offset, count, path, ap);
	va_end(ap);
}

void TypeStorage::getv(const std::string *dst, size_t dst_stride,
                       size_t src_offset, size_t count,
                       const std::string &path, va_list ap) {
	_get((char *)dst, StringTypeEnum, dst_stride, src_offset, count, path, ap);
}

} // namespace Ctl
