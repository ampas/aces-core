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

#include <dpx.hh>
#include "dpx_raw.hh"
#include <half.h>
#include <math.h>
#include "dpx_bits.hh"
#include "dpx_rw.hh"
#include <string.h>

namespace ctl {
namespace dpxi {

template <class O, class I>
void pack(dpx::fb<O> *out, const dpx::fb<I> &in, const rwinfo &ri) {
	uint64_t u, v;
	uint8_t pad, onbit;
	O mask;
	O t;
	O *o;
	const I *i=in.ptr();

	out->init(ri.words_for_raw<O>(), 1, 1);

	o=out->ptr();

#if 1
	if(ri.bps==10) {
		if(ri.pack==1) {
			for(u=0; u<out->pixels(); u++) {
				*o=i[0]&0x3ff;
				*o=*o<<10;	
				*o=*o|i[1]&0x3ff;
				*o=*o<<10;	
				*o=*o|i[2]&0x3ff;
				*o=*o<<2;	
				i=i+3;
				o++;
			}
			return;
		} else if(ri.pack==2) {
			for(u=0; u<out->pixels(); u++) {
				*o=i[0]&0x3ff;
				*o=*o<<10;	
				*o=*o|i[1]&0x3ff;
				*o=*o<<10;	
				*o=*o|i[2]&0x3ff;
				i=i+3;
				o++;
			}
			return;
		}
	}
#endif

	mask=max_int_for_bits[ri.bps];

	if((ri.pack&0x7)==0) {
		// XXX not supported
	} else if((ri.pack&0x7)==1) {
		pad=(sizeof(I)*8)%ri.bps;
	} else if((ri.pack&0x7)==2) {
		pad=0;
	} else {
		// XXX not supported
	}

	onbit=sizeof(O)*8;
	t=0;
	for(u=0; u<out->height(); u++) {
		if(onbit!=sizeof(O)*8) {
			t=t<<pad;
			*(o++)=t;
			onbit=sizeof(O)*8;
			t=0;
		}
		for(v=0; v<out->width(); v++) {
			if(onbit<ri.bps) {
				t=t<<pad;
				*(o++)=t;
				onbit=sizeof(O)*8;
				t=0;
			}
			t=t<<ri.bps;
			t=t|((*(i++))&mask);
			onbit=onbit-ri.bps;
		}
	}
	if(onbit!=sizeof(O)*8) {
		t=t<<pad;
		*(o++)=t;
	}
}

template <class T>
void write_fb(std::ostream *o, const dpx::fb<T> &buf, const rwinfo &wi) {
	dpx::fb<uint64_t>    fbu64;
	dpx::fb<uint32_t>    fbu32;
	dpx::fb<uint16_t>    fbu16;
	dpx::fb<uint8_t>     fbu8;
		
	if(wi.pack<8) {
		if(wi.bps==32) {
			write_ptr(o, buf.ptr(), buf.count(), wi.need_byteswap);
		} else if(wi.datatype==0 && (wi.pack==1 || wi.pack==2)) {
			pack(&fbu32, buf, wi);
			write_ptr(o, fbu32.ptr(), fbu32.count(), wi.need_byteswap);
		} else {
			// XXX
		}
	} else if(wi.pack<16) {
		if(wi.bps==16) {
			write_ptr(o, buf.ptr(), buf.count(), wi.need_byteswap);
		} else if(wi.datatype==0 && (wi.pack==9 || wi.pack==10)) {
			pack(&fbu16, buf, wi);
			write_ptr(o, fbu16.ptr(), fbu16.count(), wi.need_byteswap);
		} else {
			// XXX
		}
	} else if(wi.pack<24) {
		if(wi.bps==8) {
			write_ptr(o, buf.ptr(), buf.count(), wi.need_byteswap);
		} else if(wi.datatype==0 && (wi.pack==17 || wi.pack==18)) {
			pack(&fbu32, buf, wi);
			write_ptr(o, buf.ptr(), buf.count(), wi.need_byteswap);
		} else {
			// XXX
		}
	} else if(wi.pack<32) {
		if(wi.bps==64) {
			write_ptr(o, buf.ptr(), buf.count(), wi.need_byteswap);
		} else if(wi.datatype==0 && (wi.pack==25 || wi.pack==26)) {
			pack(&fbu64, buf, wi);
			write_ptr(o, buf.ptr(), buf.count(), wi.need_byteswap);
		} else {
			// XXX
		}
	} else {
		// XXX
	}
}

template <class T>
void write(std::ostream *o, const dpx::fb<T> &buf, const rwinfo &wi) {
	dpx::fb<uint64_t>    fbu64;
	dpx::fb<uint32_t>    fbu32;
	dpx::fb<uint16_t>    fbu16;
	dpx::fb<uint8_t>     fbu8;
	dpx::fb<float16_t>   fbf16;
	dpx::fb<float32_t>   fbf32;
	dpx::fb<float64_t>   fbf64;

	if(wi.datatype==0) {
		if(wi.bps<=8) {
			fbu8.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbu8, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			write_fb(o, fbu8, wi);
		} else if(wi.bps<=16) {
			fbu16.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbu16, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			write_fb(o, fbu16, wi);
		} else if(wi.bps<=32) {
			fbu32.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbu32, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			write_fb(o, fbu32, wi);
		} else if(wi.bps<=64) {
			fbu64.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbu64, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			write_fb(o, fbu64, wi);
		} else {
			// XXX
		}
	} else if(wi.datatype==2) {
		if(wi.bps==16) {
			fbf16.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbf16, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			if(wi.pack>=8 && wi.pack<=10) {
				write_ptr(o, fbf64.ptr(), fbf64.count(), wi.need_byteswap);
			}
		} else if(wi.bps==32) {
			fbf32.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbf32, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			if(/*wi.pack>=0 &&*/ wi.pack<=2) {
				write_ptr(o, fbf32.ptr(), fbf32.count(), wi.need_byteswap);
			}
		} else if(wi.bps==64) {
			fbf64.init(buf.width(), buf.height(), buf.depth());
			convertfb(&fbf64, wi.bps, buf.ptr(), sizeof(T)*8, wi.scale);
			if(wi.pack>=24 && wi.pack<=26) {
				write_ptr(o, fbf64.ptr(), fbf64.count(), wi.need_byteswap);
			}
		} else {
			// XXX
		}
	} else {
		// XXX
	}
}

template <class T>
void write(std::ostream *o, dpx *h, uint8_t element, const dpx::fb<T> &buffer,
           float64_t scale, dpx::intmode_e mode) {
	std::ostream::pos_type start;
	rwinfo wi;

	start=o->tellp();

	rwinfo::write_init(o, h);

	if(mode!=dpx::unformatted) {
		rwinfo::validate(h, element, 2*!(std::numeric_limits<T>::is_integer),
		                   sizeof(T)*8, buffer.depth(), buffer.width(),
		                   buffer.height());
	}

	wi.set(h, element, scale, mode, FALSE);

	if(mode==dpx::unformatted || wi.direct) {
		rwinfo::find_home(h, element, buffer.length());
		o->seekp(start+((std::streamoff)h->elements[element].offset_to_data));
		dpxi::write_ptr(o, buffer.ptr(), buffer.count(), wi.need_byteswap);
	} else {
		rwinfo::find_home(h, element, wi.bytes_for_raw());
		o->seekp(start+((std::streamoff)h->elements[element].offset_to_data));
		dpxi::write(o, buffer, wi);
	}

	o->seekp(start);
}

};

void dpx::write(std::ostream *o, uint8_t element, const fb<half> &buffer,
                float64_t scale) {
	dpxi::write(o, this, element, buffer, scale, dpx::normal);
}

void dpx::write(std::ostream *o, uint8_t element, const fb<float32_t> &buffer,
                float64_t scale) {
	dpxi::write(o, this, element, buffer, scale, dpx::normal);
}

void dpx::write(std::ostream *o, uint8_t element, const fb<float64_t> &buffer,
                float64_t scale) {
	dpxi::write(o, this, element, buffer, scale, dpx::normal);
}

void dpx::write(std::ostream *o, uint8_t element, const fb<uint8_t> &buffer,
                float64_t scale, intmode_e mode) {
	dpxi::write(o, this, element, buffer, scale, mode);
}

void dpx::write(std::ostream *o, uint8_t element, const fb<uint16_t> &buffer,
                float64_t scale, intmode_e mode) {
	dpxi::write(o, this, element, buffer, scale, mode);
}

void dpx::write(std::ostream *o, uint8_t element, const fb<uint32_t> &buffer,
                float64_t scale, intmode_e mode) {
	dpxi::write(o, this, element, buffer, scale, mode);
}

void dpx::write(std::ostream *o, uint8_t element, const fb<uint64_t> &buffer,
                float64_t scale, intmode_e mode) {
	dpxi::write(o, this, element, buffer, scale, mode);
}

};
