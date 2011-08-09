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


#include "dpx_rw.hh"
#include <string.h>

namespace ctl {
namespace dpxi {

rwinfo::rwinfo() {
	clear();
}

rwinfo::rwinfo(const dpx *that, uint8_t e, float64_t _scale,
               dpx::intmode_e _mode, bool is_integer) {
	set(that, e, _scale, _mode, is_integer);
}

void rwinfo::clear(void) {
	bps=0;
	datatype=0;
	pack=0;
	descriptor=0;
	offset_to_data=0;
	width=0;
	height=0;
	channels=0;
	bytes_per_swap=0;
	aligned=0;
	direct=0;
	scale=0;
	need_byteswap=0;
	mode=dpx::normal;
}

void rwinfo::set(const dpx *that, uint8_t e, float64_t _scale,
                 dpx::intmode_e _mode, bool is_integer) {
	bps=that->elements[e].bits_per_sample;
	pack=that->elements[e].actual_packing;
	descriptor=that->elements[e].descriptor;
	offset_to_data=that->elements[e].offset_to_data;
	need_byteswap=that->_need_byteswap;
	width=that->pixels_per_line;
	height=that->lines_per_element;
	scale=_scale;

	datatype=that->elements[e].data_sign;

	if(descriptor<50) {
		// I don't quite get how the Color difference is supposed to work.
		channels=1;
	} else if(descriptor==50 || descriptor==53 || descriptor==157) {
		channels=3;
	} else if(descriptor==51 || descriptor==52) {
		channels=4;
	} else if(descriptor<100) {
		channels=3; // Probably... There can be RGBA variants in this space.
	} else if(descriptor==102) {
		channels=3;
	} else if(descriptor==101 || descriptor==103) {
		channels=4;
	} else if(descriptor==101) {
		channels=5;
	} else if(descriptor<150) {
		channels=3; // Probably... There can be YCrCbA variants in this space.
	} else if(descriptor>=150 && descriptor<=156) {
		channels=2+descriptor-150;
	} else if(descriptor==157) {
		channels=3;
	} else if(descriptor==158) {
		channels=2;
	} else {
		channels=0;
		// who knows...
	}

	if(!strncasecmp(that->header_version, "V3.0", 2)) {
		version=0x30;
	} else if(!strncasecmp(that->header_version, "V2.0", 2)) {
		version=0x20;
	} else {
		// If we don't recognize the header version string we assume
		// That we're version 0x10. Note that we put the version number
		// in the top 4 bits, in case we have to later add some other
		// subtype information.
		version=0x10;
	}

	if(dpx::isnull(pack)) {
		// This happens on read (since on write the actual_packing is set
		// by the validate function for writes)
		if(version==0x30) {
			pack=that->elements[e].packing;
		} else {
			packing_for_bps(that->elements[e].bits_per_sample, &pack, NULL);
			// We need to maintain the same lower 2 bits as the packing
			// in the file so that we don't shift in the wrong direction.
			pack=(pack&~0x3)|(that->elements[e].packing&0x3);
		}
	}

	aligned=FALSE;
	if((pack&0x7)==0) {
		aligned=TRUE;
	}
	if(pack<8) {
		bytes_per_swap=4;
	} else if(pack<16) {
		bytes_per_swap=2;
	} else if(pack<24) {
		bytes_per_swap=1;
	} else if(pack<32) {
		bytes_per_swap=8;
	} else {
		// XXX
	}


	direct=FALSE;
	if((scale==0.0 || scale==1.0) && (bytes_per_swap*8)==bps) {
		if((is_integer && datatype==0) || (!is_integer && datatype==2)) {
			direct=TRUE;
		}
	}

	scale=_scale;
	mode=_mode;
}

uint64_t rwinfo::words_for_raw(uint8_t swap_boundary) const {
	uint32_t samples_per_word;
	uint64_t samples;

	samples_per_word=(swap_boundary*8)/bps;
	samples=channels*width;
	return ((samples+samples_per_word-1)/samples_per_word)*height;
}

uint64_t rwinfo::bytes_for_raw(void) const {
	return words_for_raw(bytes_per_swap)*bytes_per_swap;
}

void rwinfo::write_init(std::ostream *o, dpx *h) {
	uint32_t foo;
	bool arch_is_little_endian;

	foo=0x01020304;
	arch_is_little_endian=FALSE;
	if(((uint8_t *)&foo)[0]==0x04) {
		arch_is_little_endian=TRUE;
	}

	if(o==NULL) {
		h->current_ostream=NULL;
		return;
	}

	if(h->current_ostream==NULL) {
		h->current_ostream=o;
		if(h->compliance==dpx::automatic) {
			h->compliance=dpx::dpx1;
		}
		if(h->endian_mode==dpx::default_endian_mode) {
			h->endian_mode=dpx::big_endian;
		}
		h->current_compliance=h->compliance;
		h->current_endian_mode=h->endian_mode;
		h->_need_byteswap=FALSE;
		switch(h->endian_mode) {
			case dpx::default_endian_mode:
				h->endian_mode=dpx::big_endian;
				// FALL THROUGH
			
			case dpx::big_endian:
				if(arch_is_little_endian) {
					h->_need_byteswap=TRUE;
				}
				break;

			case dpx::little_endian:
				if(!arch_is_little_endian) {
					h->_need_byteswap=TRUE;
				}
				break;

			case dpx::native:
				break;

			case dpx::swapped:
				h->_need_byteswap=TRUE;
				break;
		}
		// WHY OH WHY OH WHY DO THEY HAVE THESE FIELDS BUT MARK THEM AS
		// NON-CORE?
		h->total_file_size=0;
		h->generic_header_length=1664;
		h->industry_header_length=384;
		dpx::nullify(&(h->data_offset));
	}

	if(h->current_ostream!=o) {
		// XXX very, very bad... Wrote a plane, then tried to write another
		// plane on a different ostream.
	}
	if(h->current_compliance!=h->compliance) {
		// XXX changed validation level... This will probably end in tears...
	}
	if(h->current_endian_mode!=h->current_endian_mode) {
		// XXX byteswap changed. this will probably end in tears...
	}
}

void rwinfo::find_home(dpx *h, uint8_t element, uint64_t size) {
	uint8_t i;
	rwinfo info;
	uint64_t eof;
	uint64_t actual_eof;
	uint64_t element_eod;
	uint64_t actual_element_eod;
	uint64_t actual_lengths[8];
	uint64_t lengths[8];

	memset(actual_lengths, 0, sizeof(actual_lengths));
	memset(lengths, 0, sizeof(lengths));

	if(!dpx::isnull(h->elements[element].offset_to_data) &&
	   h->elements[element].offset_to_data!=0) {
		// The user set this himself... He gets to slit his own throat...
		return;
	}

	// If the offset to data has not been set. Same as above. The user
	// should only set this if he *really* knows what he's doing.
	if(dpx::isnull(h->data_offset)) {
		h->data_offset=1<<14;
	}

	for(i=0; i<h->number_of_elements; i++) {
		// The last three parameters are only used for determining
		// conversion attributes. We just want to get the size.
		info.set(h, element, 0.0, dpx::normal, 0);

		actual_lengths[i]=info.bytes_for_raw();
		// And round things up to 16k boundaries.
		lengths[i]=actual_lengths[i]+(1<<14)-1;
		lengths[i]=lengths[i]&(~((1<<14)-1));
	}

	eof=h->data_offset;
	actual_eof=h->data_offset;
	for(i=0; i<h->number_of_elements; i++) {
		if(i==element) {
			continue;
		}
		element_eod=h->elements[i].offset_to_data+lengths[i];
		actual_element_eod=h->elements[i].offset_to_data+actual_lengths[i];

		if(element_eod>eof) {
			eof=element_eod;
		}

		if(actual_element_eod>actual_eof) {
			actual_eof=actual_element_eod;
		}
	}

	h->elements[element].offset_to_data=eof;
	actual_eof=eof+actual_lengths[element];
	h->total_file_size=actual_eof;
}

};

};
