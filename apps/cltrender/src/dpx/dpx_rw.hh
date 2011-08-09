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


#if !defined(AMPAS_CTL_DPX_RW_INCLUDE)
#define AMPAS_CTL_DPX_RW_INCLUDE

#include <dpx.hh>

namespace ctl {
namespace dpxi {

// The story behind readinfo_t: Because there large number of auxilliary
// functions to handle all of the potential conversions in reads and writes,
// and we don't want to have a gigantic friend section of the dpx header,
// we build this (which only contains the required parameters, as well as
// a little bit of pre-processing) and pass it to all of the myriad functions
// that actually read and write things.
struct rwinfo {
	rwinfo();
	rwinfo(const dpx *that, uint8_t e, float64_t scale, dpx::intmode_e mode,
	         bool is_integer);

	void clear(void);
	void set(const dpx *that, uint8_t e, float64_t scale, dpx::intmode_e mode,
	         bool is_integer);

	// from the header
	uint8_t bps; 
	uint8_t datatype;
	uint16_t pack;
	uint8_t descriptor;
	uint32_t offset_to_data;
	uint32_t width;
	uint32_t height;

	// synthesized.
	uint8_t channels;
	uint8_t bytes_per_swap;
	bool aligned;
	uint8_t version;

	// True if our buffer is of the same type and alignment that we
	// can just read the bits off the disk and put them in our buffer.
	bool direct;

	// user options to top level read functions.
	float64_t scale;
	bool need_byteswap;
	dpx::intmode_e mode;

	// If writing into type 'T', given the bps, width, and height
	// how many words of type 'T' will be needed to actually store
	// with packing.
	template <class T>
	uint64_t words_for_raw(void) const;

	uint64_t words_for_raw(uint8_t swap_boundary) const;
	uint64_t bytes_for_raw(void) const;

	// Some various helper functions. We might as well put them
	// here because this struct is a friend of dpx, and they
	// have to modify the header.
	//
	// 1) If not already set in the header, fill out some basically sane
	//    parameters to be able to fit .
	// 2) Makes sure that the user hasn't specified something completely out
	//    of whack in the header (we can't possibly write).
	//
	static void validate(dpx *h, uint8_t element, uint8_t datatype,
	                     uint8_t bps, uint8_t channels, uint64_t width,
	                     uint64_t height);

	void validate_adx(dpx *h, uint8_t element);
	void validate_adx_strict(dpx *h, uint8_t element);
	void validate_dpx(dpx *h, uint8_t element);
	void validate_dpx3(dpx *h, uint8_t element);

	// Used for DPX 1.0 fields to set up the packing in the header vs
	// what the actual packing is.
	static void packing_for_bps(uint8_t bps, uint16_t *actual,
	                            uint16_t *header);

	// Set up / check state variables when we start writing to try
	// to prevent the user from doing anything *too* foolish when
	// writing files.
	static void write_init(std::ostream *o, dpx *h);

	// Given an block of data of bytes 'size', find a place in the
	// file that it will fit. Not the smartest search in the world.
	static void find_home(dpx *h, uint8_t element, uint64_t size);
};

template <class T>
uint64_t rwinfo::words_for_raw(void) const {
	return words_for_raw(sizeof(T));
}

};

};

#endif
