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

namespace ctl {
namespace dpxi {

void rwinfo::packing_for_bps(uint8_t bps, uint16_t *actual, uint16_t *header) {
	if(bps==8) {
		if(header!=NULL) {
			*header=0;
		} 
		if(actual!=NULL) {
			*actual=16;
		}
	} else if(bps==16) {
		if(header!=NULL) {
			*header=0;
		}
		if(actual!=NULL) {
			*actual=8;
		}
	} else if(bps>=11 && bps<17) {
		if(header!=NULL) {
			*header=1;
		}
		if(actual!=NULL) {
			*actual=9;
		}
	} else if(bps==10) {
		if(header!=NULL) {
			*header=1;
		}
		if(actual!=NULL) {
			*actual=1;
		}
	} else {
		// XXX
	}
}

void rwinfo::validate(dpx *h, uint8_t element, uint8_t datatype,
                      uint8_t bps, uint8_t channels, uint64_t width,
                      uint64_t height) {

	// Make sure that all of the fields are filled out correctly...
	if(dpx::isnull(h->image_orientation)) {
		h->image_orientation=0;
	}

	if(dpx::isnull(h->pixels_per_line)) {
		h->pixels_per_line=width;
	}

	if(dpx::isnull(h->lines_per_element)) {
		h->lines_per_element=height;
	}

	if(dpx::isnull(h->number_of_elements) || h->number_of_elements<=element) {
		h->number_of_elements=element+1;
	}

	if(h->number_of_elements>8) {
		// XXX too many elements...
	}

	if(dpx::isnull(h->elements[element].bits_per_sample)) {
		h->elements[element].bits_per_sample=bps;
	}

	if(dpx::isnull(h->elements[element].data_sign)) {
		h->elements[element].data_sign=datatype;
	}

	if(dpx::isnull(h->elements[element].descriptor)) {
		switch(channels) {
			case 1:
				h->elements[element].descriptor=6; // Luminance
				break;

			case 2:
				if(h->compliance==dpx::dpx1) {
					h->elements[element].descriptor=150; // user def 2 channels
				} else {
					h->elements[element].descriptor=158; // Luminance + Alpha
				}
				break;

			case 3:
				h->elements[element].descriptor=50; // RGB
				break;

			case 4:
				h->elements[element].descriptor=51; // RGBA
				break;

			case 5:
				h->elements[element].descriptor=153; // user def 5 channels
				break;

			case 6:
				h->elements[element].descriptor=154; // user def 6 channels
				break;

			case 7:
				h->elements[element].descriptor=155; // user def 7 channels
				break;

			case 8:
				h->elements[element].descriptor=156; // user def 8 channels
				break;

			default:
				// XXX unsupported
				break;
		}
	}

	if(dpx::isnull(h->elements[element].transfer_characteristic)) {
		if(h->elements[element].bits_per_sample==10) {
			h->elements[element].transfer_characteristic=1;
		} else {
			h->elements[element].transfer_characteristic=2;
		}
	}

	if(dpx::isnull(h->elements[element].colorimetric_characteristic)) {
		h->elements[element].colorimetric_characteristic=
		 h->elements[element].transfer_characteristic;
	}

	// We have an 'actual_packing' field which drives the writer to
	// specify where the byte swaps occur, and a 'packing' field which
	// is what is put in the header. We can thank the excessively vague
	// DPX 1.0 standard for this.
	if(dpx::isnull(h->elements[element].packing)) {
		if(h->compliance==dpx::dpx1) {
			packing_for_bps(h->elements[element].bits_per_sample,
			                &(h->elements[element].actual_packing),
			                &(h->elements[element].packing));
		} else {
			if(h->elements[element].bits_per_sample==10) {
				// historic reasons... If we don't do this we'll
				// break every DPX 1.0 reader known to man
				h->elements[element].packing=1;
			} else if(h->elements[element].bits_per_sample<8) {
				h->elements[element].packing=17;
			} else if(h->elements[element].bits_per_sample==8) {
				h->elements[element].packing=16;
			} else if(h->elements[element].bits_per_sample<16) {
				h->elements[element].packing=9;
			} else if(h->elements[element].bits_per_sample==16) {
				h->elements[element].packing=8;
			} else if(h->elements[element].bits_per_sample<32) {
				h->elements[element].packing=1;
			} else if(h->elements[element].bits_per_sample==32) {
				h->elements[element].packing=0;
			} else if(h->elements[element].bits_per_sample<64) {
				h->elements[element].packing=25;
			} else if(h->elements[element].bits_per_sample==64) {
				h->elements[element].packing=24;
			} else {
				// XXX
			}
		}
	}

	if(dpx::isnull(h->elements[element].actual_packing)) {
		h->elements[element].actual_packing=h->elements[element].packing;
	}

	if(dpx::isnull(h->elements[element].encoding)) {
		h->elements[element].encoding=0;
	}

	if(dpx::isnull(h->elements[element].eol_padding)) {
		h->elements[element].eol_padding=0;
	}

	if(dpx::isnull(h->elements[element].eoi_padding)) {
		h->elements[element].eoi_padding=0;
	}

	// Now we start doing general validation (i.e. things that won't
	// possibly fly no mater what the validation_level is)
	rwinfo info(h, element, 0, dpx::normal, FALSE);

	if(h->image_orientation>=8) {
		// XXX invalid image_orientation
	}

	if(h->pixels_per_line!=width) {
		// XXX invalid width
	}

	if(h->lines_per_element!=height) {
		// XXX invalid height
	}

	if(info.channels!=channels) {
		// XXX descriptor doesn't match channel count.
	}

	if(bps>(info.bytes_per_swap*8)) {
		// sample doesn't fit in swap boundary.
	}

	if((info.bytes_per_swap*8)%bps) {
		if((h->elements[element].packing&0x7)==0) {
			// Invalid packing
		}
	} else {
		if((h->elements[element].packing&0x7)>=3) {
			// Invalid packing
		}
	}

	if(h->elements[element].eol_padding!=0) {
		// EOL padding must be 0
	}

	if(h->elements[element].eoi_padding!=0) {
		// EOI padding must be 0
	}

	if(h->elements[element].encoding!=0) {
		// encoding must be 0
	}

	if(h->elements[element].data_sign!=0 &&
	   h->elements[element].data_sign!=2) {
		// invalid data_sign
	}
}

void rwinfo::validate_adx_strict(dpx *h, uint8_t element) {
	// We don't do full validation yet...
	validate_adx(h, element);
}

void rwinfo::validate_adx(dpx *h, uint8_t element) {
}

void rwinfo::validate_dpx(dpx *h, uint8_t element) {
}

void rwinfo::validate_dpx3(dpx *h, uint8_t element) {
}

}; 

};
