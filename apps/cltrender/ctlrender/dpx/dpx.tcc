///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 Academy of Motion Picture Arts and Sciences
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


template <class T>
dpx::fb<T>::fb() {
	_data=NULL;
	_width=0;
	_height=0;
	_depth=0;
	_length=0;
}

template <class T>
dpx::fb<T>::~fb() {
	delete [] _data;
	_data=NULL;
	_width=0;
	_height=0;
	_depth=0;
	_length=0;
}

template <class T>
void dpx::fb<T>::init(uint32_t width, uint32_t height, uint32_t depth) {
	_width=width;
	_height=height;
	_depth=depth; 

	delete [] _data;

	_length=_width*_height*_depth*sizeof(T);

	_data=new T[width*height*depth];
}

template <class T>
uint32_t dpx::fb<T>::width(void) const {
	return _width;
}

template <class T>
uint32_t dpx::fb<T>::height(void) const {
	return _height;
}

template <class T>
uint32_t dpx::fb<T>::depth(void) const {
	return _depth;
}

template <class T>
uint64_t dpx::fb<T>::length(void) const {
	return _length;
}

template <class T>
uint64_t dpx::fb<T>::count(void) const {
	return _width*_height*_depth;
}

template <class T>
uint64_t dpx::fb<T>::pixels(void) const {
	return _width*_height;
}

template <class T>
T *dpx::fb<T>::ptr(void) {
	return _data;
}

template <class T>
const T *dpx::fb<T>::ptr(void) const {
	return _data;
}

template <class T>
dpx::fb<T>::operator T *() {
	return _data;
}

template <class T>
dpx::fb<T>::operator const T *() const {
	return _data;
}

template <class T>
void dpx::fb<T>::swizzle(uint8_t descriptor, bool squish_alpha) {
	uint64_t u, count;
	T *i, *o;
	T t;

	count=width()*height();

	i=_data;
	o=_data;

	switch(descriptor) {
		case 0:
			if(alpha() && squish_alpha) {
				if(_depth==2) {
					for(u=0; u<count; u++) {
						*(o++)=*(i++); i++;		
					}
					_depth=1;					
				} else if(_depth==4) {
					for(u=0; u<count; u++) {
						*(o++)=*(i++);
						*(o++)=*(i++);
						*(o++)=*(i++);
						i++;
					}
					_depth=3;
				}
			}
			break;

		case 6:   // luminance
		case 50:  // RGB
		case 157: // XYZ
			break;

		case 158: // luminance, alpha
			if(squish_alpha) {
				for(u=0; u<count; u++) {
					*(o++)=*(i++); i++;		
				}
				_depth=1;
			}
			break;

		case 51: // RGBA
			if(squish_alpha) {
				for(u=0; u<count; u++) {
					*(o++)=*(i++);
					*(o++)=*(i++);
					*(o++)=*(i++);
					i++;
				}
				_depth=3;
			}
			break;

		case 53: //BGR
			for(u=0; u<count; u++) {
				t=i[0];
				i[0]=i[2];
				i[2]=t;
				t=t+3;
				i=i+3;
			}
			break;

		case 52: //ABGR
			if(squish_alpha) {
				for(u=0; u<count; u++) {
					o[0]=i[3];
					t=i[2];
					o[2]=i[1];
					o[1]=t;
				}
				_depth=3;
			} else {
				for(u=0; u<count; u++) {
					t=i[0];
					o[0]=i[3];
					o[3]=t;
					t=i[1];
					o[1]=i[2];
					o[2]=t;
					i=i+4;
					o=o+4;
				}
			}
			break;
	}
}

template <class T>
bool ctl::dpx::fb<T>::alpha() const {
	return (_depth==2 || _depth==4);
}

template <class T>
void ctl::dpx::fb<T>::alpha(const T &value) {
	uint64_t u;
	fb<T> new_fb;
	T *i, *o;

	if(alpha()) {
		i=_data+_depth-1;
		for(u=0; u<count(); u++) {
			*i=value;
			i=i+_depth;
		}
		return;
	}

	new_fb.init(width(), height(), depth()+1);
	if(depth()==1) {
		i=_data;
		o=new_fb.ptr();
		for(u=0; u<count(); u++) {
			*(o++)=*(i++);
			*(o++)=value;
		}
	} else if(depth()==3) {	
		i=_data;
		o=new_fb.ptr();
		for(u=0; u<count(); u++) {
			*(o++)=*(i++);
			*(o++)=*(i++);
			*(o++)=*(i++);
			*(o++)=value;
		}
	}
}


namespace dpxi {

extern const uint64_t max_int_for_bits[];

//
// Various functions to perform conversions between different types and
// significant bit counts. This is the seedy underbelly of the dpx
// library. Hopefully you haven't found yourself here because of a bug.
//
// This code preferrs accuracy over speed. This is not so much a problem
// with 8 or 16 bit input types (because a lookup table is used), but with
// a 32 or 64 bit type, you end up with a fair bit of code that often
// unavoidably has branches statements in it.
//
// This code is highly templatized (and therefore inlined), and there
// are certain assumptions made on what types are provided.
//
// Repurposing the template code for other types is done at your own
// risk. This has been developed to deal with conversions between the
// following types:
//
//     uint8_t
//     uint16_t
//     uint32_t
//     uint64_t
//     float16_t (half)
//     float32_t (float)
//     float64_t (double)
//
//
// Because of various foibles of the template system, we still need to
// explicitly have some functions specify what their general input
// and output types are.
//

// This method is the interface to *all* of the methods included in this
// this file. The O and I classes can be any combination of the above
// types, and if O or I is an integer type, then the osb and isb values
// are used to specify if 
template <class O, class I>
void convert(O *out, uint8_t osb, const I *in, uint8_t isb,
             float64_t scale);

template <class O, class I>
void convert(O *out, uint8_t osb, const I *in, uint8_t isb,
             float64_t scale, uint64_t count);

template <class O, class I>
void convertfb(dpx::fb<O> *out, uint8_t osb, const I *in, uint8_t isb,
               float64_t scale);


// Given input and output types, input and output signifcant bits, and
// a scale value, returns a function pointer that (effectively) matches
// the convert signature above. The function returned *may* be an optimized
// version the above with the osb, isb, and scale values 'cooked' into it.
// When using the returned function you *must* still supply the osb, isb
// and scale values (in the event that the returned type was not optimized).
// There is probably a better way to handle this optimization, but it's
// left as an excercise to the user.
typedef void (*convert_fn)(void *out, const uint8_t &osb, const void *in,
                           const uint8_t &isb, const float64_t &scale);

template <class O, class I> 
convert_fn find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale);

// The lower level functions:

// Convert float to integer, optimized for scale==1.0
template<class O, class I>
void ftu_one(O *out, const uint8_t &osb, const I *_in, const uint8_t &isb,
             const float64_t &scale) {
	I in=*_in;
	float64_t fmax;
	uint64_t imax;
	uint64_t result;
	uint64_t msb;

	if(osb==64) {
		imax=dpxi::max_int_for_bits[osb-1];
		in=in/2.0;
	} else {
		imax=dpxi::max_int_for_bits[osb];
	}
	fmax=imax;
	// We get to do this hinky scale by half thing here when dealing with
	// 64 bit numbers because the only double to int conversion presumes
	// the return int is signed (and there is no support for return values
	// greater than 64 bits.

	if(in<0) {
		*out=0;
		return;
	} else if(in>fmax) {
		*out=imax;
		return;
	}

	result=llrint(in);
	if(osb==64) {
		msb=(result&(1ULL<<62))>>62;
		result=result<<1 | msb;
	}

	*out=result;
}

// Convert float to integer, optimized for scale==0
template<class O, class I>
void ftu_zero(O *out, const uint8_t &osb, const I *_in, const uint8_t &isb,
              const float64_t &scale) {
	float64_t in=*_in;
	float64_t fmax;

	fmax=dpxi::max_int_for_bits[osb];

	if(in<0) {
		*out=0;
		return;
	} else if(in>1.0) {
		*out=dpxi::max_int_for_bits[osb];
		return;
	}

	in=in*fmax;

	ftu_one(out, osb, &in, isb, scale);
}

// convert float to integer non-optimized
template<class O, class I>
void ftu(O *out, const uint8_t &osb, const I *_in, const uint8_t &isb,
         const float64_t &scale) {
	I in=*_in;

	in=in*scale;

	ftu_one(out, osb, &in, isb, scale);
}

// convert float to float, with scale==0 or scale==1
template<class O, class I>
void ftf_one(O *out, const uint8_t &osb, const I *in, const uint8_t &isb,
             const float64_t &scale) {
	*out=*in;
}

// convert float to float, any scale
template<class O, class I>
void ftf(O *out, const uint8_t &osb, const I *in, const uint8_t &isb,
         const float64_t &scale) {
	*out=*in*scale;
}

// convert int to float, scale==0.0
template<class O, class I>
void utf_zero(O *out, const uint8_t &osb, const I *in, const uint8_t &isb,
              const float64_t &scale) {
	uint64_t imax;
	float64_t fmax;

	imax=dpxi::max_int_for_bits[isb];
	fmax=imax;

	if((*in)>imax) {
		*out=1.0;
	} 

	*out=((float64_t)(*in))/fmax;
}

// convert int to float, scale==1.0
template<class O, class I>
void utf_one(O *out, const uint8_t &osb, const I *in, const uint8_t &isb,
             const float64_t &scale) {
	uint64_t imax;
	float64_t fmax;

	imax=dpxi::max_int_for_bits[isb];
	fmax=imax;

	if((*in)>imax) {
		*out=fmax;
	} 

	*out=*in;
}

// convert from float to int
template<class O, class I>
void utf(O *out, const uint8_t &osb, const I *_in, const uint8_t &isb,
         const float64_t &scale) {
	float64_t in;

	in=(*_in)*scale;

	utf_one(out, osb, &in, isb, scale);
}

// convert integer to integer, scale=0.0
template<class O, class I>
void utu_zero(O *out, const uint8_t &osb, const I *in, const uint8_t &isb,
               const float64_t &scale) {
	uint8_t count;
	uint8_t sigbits;
	uint8_t deltabits;
	uint8_t shift;
	O result;
	O bits;

	if(osb<isb) {
		shift=osb-isb;
		*out=((*in)>>shift);
		if((*out)>=dpxi::max_int_for_bits[osb]) {
			*out=dpxi::max_int_for_bits[osb];
		}
		return;
	} else {
		sigbits=isb;
		count=sigbits-osb;

		result=*in;
		if(result>=dpxi::max_int_for_bits[isb]) {
			result=dpxi::max_int_for_bits[isb];
		}
		// When we scale up we do it in such a way that we don't leave
		// all of the LSB zero by copying parts out of the MSB. For some
		// conversions this needs to be iteratively done (since there
		// aren't enough MSB to copy if the conversion is large). We
		// do a quickie pass that the optimizer will take care of
		// since it can be determined at compile time, failing that
		// the looping version is used.
		if(count<=sigbits) {
			deltabits=count;
			bits=dpxi::max_int_for_bits[deltabits];
			bits=bits<<(sigbits-deltabits);
			bits=result&bits;
			bits=bits>>(sigbits-deltabits);
			result=result<<deltabits;
			result=result|bits;
		} else {
			while(count!=0) {
				deltabits=count;
				if(count>sigbits) {
					deltabits=sigbits;
				}
	
				bits=dpxi::max_int_for_bits[deltabits];
				bits=bits<<(sigbits-deltabits);
				bits=result&bits;
				bits=bits>>(sigbits-deltabits);
				result=result<<deltabits;
				result=result|bits;
				count=count-deltabits;
			}
		}
		*out=result;
	} 
}

// convert integer to integer scale==1.0
template<class O, class I>
void utu_one(O *out, const uint8_t &osb, const I *in, const uint8_t &isb,
              const float64_t &scale) {
	uint8_t shift;

	if(osb<isb) {
		shift=osb-isb;
	} else {
		shift=0;
	}

	*out=(*in)>>shift;

	if(*out>=dpxi::max_int_for_bits[osb]) {
		*out=dpxi::max_int_for_bits[osb];
	}
}

// convert integer to integer 
template<class O, class I>
void utu(O *out, const uint8_t &osb, const I *_in, const uint8_t &isb,
         const float64_t &scale) {
	float64_t in;

	in=*_in*scale;

	ftu_one(out, osb, &in, isb, scale);
}

#define VOIDIFY(X)  \
template<class O, class I> \
void v_##X(void *out, const uint8_t &osb, const void *in, \
           const uint8_t &isb, const float64_t &scale) { \
	X((O *)out, osb, (const I *)in, isb, scale); \
}

VOIDIFY(ftu_one);
VOIDIFY(ftu_zero);
VOIDIFY(ftu);
VOIDIFY(utf_one);
VOIDIFY(utf_zero);
VOIDIFY(utf);
VOIDIFY(utu_one);
VOIDIFY(utu_zero);
VOIDIFY(utu);
VOIDIFY(ftf_one);
VOIDIFY(ftf);

struct true_type {};
struct false_type {};

template <class T>
struct isfloatish : false_type {};

template <>
struct isfloatish<float16_t> : true_type {};
template <>
struct isfloatish<float32_t> : true_type {};
template <>
struct isfloatish<float64_t> : true_type {};


//template <class O, class I, class it, class ot> 
//convert_fn _find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale);

template <class O, class I>
convert_fn _find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale,
                            const O &_o, const I &_i, const false_type &_it,
                            const false_type &_ot) {
	if(scale==0.0) {
		return v_utu_zero<O, I>;
	} else if(scale==1.0) {
		return v_utu_one<O, I>;
	} else {
		return v_utu<O, I>;
	}
}

template <class O, class I>
convert_fn _find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale,
                            const O &_o, const I &_i, const true_type &_it,
                            const false_type &_ot) {
	if(scale==0.0) {
		return v_ftu_zero<O, I>;
	} else if(scale==1.0) {
		return v_ftu_one<O, I>;
	} else {
		return v_ftu<O, I>;
	} 
}

template <class O, class I>
convert_fn _find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale,
                            const O &_o, const I &_i, const false_type &_it,
                            const true_type &_ot) {
	if(scale==0.0) {
		return v_utf_zero<O, I>;
	} else if(scale==1.0) {
		return v_utf_one<O, I>;
	} else {
		return v_utf<O, I>;
	}
}

template <class O, class I>
convert_fn _find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale,
                            const O &_o, const I &_i, const true_type &_it,
                            const true_type &_ot) {
	if(scale==0.0 || scale==1.0) {
		return v_ftf_one<O, I>;
	} else {
		return v_ftf<O, I>;
	}
}

template <class O, class I> 
convert_fn find_convert_fn(uint8_t osb, uint8_t isb, float64_t scale) {
	convert_fn fn;

	fn=_find_convert_fn(osb, isb, scale, O(), I(),
	                    isfloatish<I>(), isfloatish<O>());
	return fn;
}

template <class O, class I>
void convertlut(O *out, uint8_t osb, const I *in, uint8_t isb,
                float64_t scale, uint64_t count);

template <class O, class I>
void _convertfblut(O *out, uint8_t osb, const I *in, uint8_t isb,
                   float64_t scale, uint64_t count);

template <class O, class I>
void _convertlut(O *o, uint8_t osb, const I *in, uint8_t isb,
                 float64_t scale, uint64_t count) {
	convert_fn fn;
	O lut[1<<16];
	uint32_t u;

	fn=find_convert_fn<O, uint32_t>(osb, isb, scale);
	for(u=0; u<(1<<16); u++) {
		fn(lut+u, osb, &u, isb, scale);	
	}
	for(u=0; u<count; u++) {
		*(o++)=lut[*(in++)];
	}
}

template <class O>
void convertlut(O *o, uint8_t osb, const uint8_t *in, uint8_t isb,
                float64_t scale, uint64_t count) {
	_convertlut<O, uint8_t>(o, osb, in, isb, scale, count);
}

template <class O>
void convertlut(O *o, uint8_t osb, const uint16_t *in, uint8_t isb,
                float64_t scale, uint64_t count) {
	_convertlut<O, uint16_t>(o, osb, in, isb, scale, count);
}

template <class O>
void convertlut(O *o, uint8_t osb, const float16_t *in, uint8_t isb,
                float64_t scale, uint64_t count) {
	convert_fn fn;
	O lut[1<<16];
	uint64_t u;
	uint16_t b;
	half h;

	fn=find_convert_fn<O, half>(osb, isb, scale);
	for(u=0; u<((1<<16)-1); u++) {
		h.setBits(u);
		// Just running everything as-is for right now...
		if(h.isNan()) {
			lut[u]=0;
		} else if(h.isInfinity() &&  h.isNegative()) {
			lut[u]=dpxi::max_int_for_bits[osb];
		} else if(h.isInfinity() && !h.isNegative()) {
			lut[u]=0;
		} else {
			fn(lut+u, osb, &h, isb, scale);
		}
	}

	for(u=0; u<count; u++) {
		b=in->bits();
		*o=lut[b];
		in++; o++;
	}
}

template <class O, class I>
void convertlut(O *out, uint8_t osb, const I *in, uint8_t isb,
                float64_t scale, uint64_t count) {
	// Do nothing unless we have a partial implimentation;
	// XXX
}

template <class O, class I>
void convert(O *o, uint8_t osb, const I *in, uint8_t isb, float64_t scale,
             uint64_t count) {
	convert_fn fn;
	uint64_t u;

	if(sizeof(I)<=2) {
		convertlut(o, osb, in, isb, scale, count);
	} else {
		fn=find_convert_fn<O, I>(osb, isb, scale);
		for(u=0; u<count; u++) {
			fn(o++, osb, in++, isb, scale);
		}
	}
}

template <class O, class I>
void convert(O *out, uint8_t osb, const I *in, uint8_t isb,
             float64_t scale) {
	convert_fn fn;

	fn=find_convert_fn<O, I>(osb, isb, scale);

	fn(out, osb, in, isb, scale);
}

template <class O, class I>
void convertfb(dpx::fb<O> *out, uint8_t osb, const I *in,
               uint8_t isb, float64_t scale) {
	convert(out->ptr(), osb, in, isb, scale, out->count());
}
 
}

template <class O, class I>
void dpx::convert(O *o, uint8_t osb, const I *i, uint8_t isb,
                  float64_t scale, uint64_t count) {
	dpxi::convert(o, osb, i, isb, scale, count);
}

template <class O, class I>
void dpx::convert(O *o, const I *i, uint64_t count) {
	dpxi::convert(o, sizeof(O)*8, i, sizeof(I)*8, 0.0, count);
}

template <class O, class I>
void dpx::convert(O *o, const I *i, float64_t scale, uint64_t count) {
	dpxi::convert(o, sizeof(O)*8, i, sizeof(I)*8, scale, count);
}

template <class O, class I>
void dpx::convert(O *o, uint8_t osb, const I *i, uint64_t count) {
	dpxi::convert(o, osb, i, sizeof(I)*8, 0.0, count);
}

template <class O, class I>
void dpx::convert(O *o, uint8_t osb, const I *i, float64_t scale,
                  uint64_t count) {
	dpxi::convert(o, osb, i, sizeof(I)*8, scale, count);
}

template <class O, class I>
void dpx::convert(O *o, const I *i, uint8_t isb, uint64_t count) {
	dpxi::convert(o, sizeof(O)*8, i, isb, 0.0, count);
}

template <class O, class I>
void dpx::convert(O *o, const I *i, uint8_t isb, float64_t scale,
                  uint64_t count) {
	dpxi::convert(o, sizeof(O)*8, i, isb, scale, count);
}

template <class O, class I>
void dpx::convert(O *o, uint8_t osb, const I *i, uint8_t isb,
                  uint64_t count) {
	dpxi::convert(o, osb, i, isb, 0.0, count);
}

