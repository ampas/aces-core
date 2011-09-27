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

#if !defined(CTL_DPX_RAW_INTERNAL_INCLUDE)
#define CRL_DPX_RAW_INTERNAL_INCLUDE

namespace ctl {

namespace dpxi {

inline void swap64(void *_s) {
	uint64_t s=*((uint64_t*)_s);

	s=(((s&0xffffffff00000000LL)>>32) | ((s&0x00000000ffffffffLL)<<32));
	s=(((s&0xffff0000ffff0000LL)>>16) | ((s&0x0000ffff0000ffffLL)<<16));
	s=(((s&0xff00ff00ff00ff00LL)>>8)  | ((s&0x00ff00ff00ff00ffLL)<<8));

	*((uint64_t*)_s)=s;
}
	
inline void swap32(void *_s) {
	uint32_t s=*((uint32_t*)_s);

	s=(((s&0xffff0000)>>16) | ((s&0x0000ffff)<<16));
	s=(((s&0xff00ff00)>>8)  | ((s&0x00ff00ff)<<8));

	*((uint32_t*)_s)=s;
}
	
inline void swap16(void *_s) {
	uint16_t s=*((uint16_t*)_s);

	s=(((s&0xff00)>>8)  | ((s&0x00ff)<<8));

	*((uint16_t*)_s)=s;
}

template <class T>
void read_ptr(std::istream *i, T *data, uint64_t count, bool swap) {
	uint64_t u;
	T *s;

	s=data;

	i->read((char *)s, sizeof(T)*count);

	if(swap && sizeof(T)==2) {
		for(u=0; u<count; u++) {
			swap16(s++);
		}
	} else if(swap && sizeof(T)==4) {
		for(u=0; u<count; u++) {
			swap32(s++);
		}
	} else if(swap && sizeof(T)==8) {
		for(u=0; u<count; u++) {
			swap64(s++);
		}
	}
}

template <class T>
void write_ptr(std::ostream *o, const T *_data, uint64_t count, bool swap) {
	uint64_t u;
	dpx::fb<uint8_t> data;
	T *s;

	data.init(count*sizeof(T), 1, 1);

	if(swap && sizeof(T)!=1) {
		data.init(count*sizeof(T), 1, 1);
		memcpy(data, _data, count*sizeof(T));
		s=(T *)data.ptr();
		switch(sizeof(T)) {
			case 2:
				for(u=0; u<count; u++) {
					swap16(s++);	
				}
				break;

			case 4:
				for(u=0; u<count; u++) {
					swap32(s++);	
				}
				break;

			case 8:
				for(u=0; u<count; u++) {
					swap64(s++);	
				}
				break;
		}
		_data=(const T *)data.ptr();
	}

	o->write((const char *)_data, count*sizeof(T));
}

uint8_t read_uint8(std::istream *is, bool need_byteswap);
uint16_t read_uint16(std::istream *is, bool need_byteswap);
uint32_t read_uint32(std::istream *is, bool need_byteswap);
float32_t read_float32(std::istream *is, bool need_byteswap);
void read_string(std::istream *is, char *bytes, int len_plus_one);

void write_uint8(std::ostream *os, uint8_t i, bool need_byteswap);
void write_uint16(std::ostream *os, uint16_t i, bool need_byteswap);
void write_uint32(std::ostream *os, uint32_t i, bool need_byteswap);
void write_float32(std::ostream *os, float32_t f, bool need_byteswap);
void write_string(std::ostream *os, char *bytes, int len_plus_one);
void write_fill(std::ostream *os, char b, int count);

}

}

#endif
