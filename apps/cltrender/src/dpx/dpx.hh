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

#if !defined(AMPAS_CTL_DPX_INCLUDE)
#define AMPAS_CTL_DPX_INCLUDE

#include <string>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <stdint.h>

#include <half.h>

#if !defined(TRUE)
#define TRUE 1
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

typedef half float16_t;
typedef float float32_t;
typedef double float64_t;

namespace ctl {
// The dpxi namespace is for various internals that are not actually
// class methods to keep the dpx header down to a sane size...
namespace dpxi {
struct rwinfo;
};

struct dpx {
	// This class can read and write DPX 1.0, 2.0, and 3.0 files (in so
	// far as the DPX 3.0 specification is complete). It should have no
	// difficulty reading any extant DPX file, and can write all common
	// variants of DPX 1.0/2.0 and all reasonable variants of DPX 3.0.
	// Please see the 'compliance' member (below) for details on how to
	// change this behavior.
	//
	// Fields marked with a '*' have some additional notes on them towards
	// the end of the include file.
	//
	// Fields below marked with a '**' indicate fields that are
	// synthesized (on reads) from the character array header field
	// they are associated with. If the associated character array field
	// has not been set (i.e. first character is a numeric 0) on write,
	// then the field is decomposed back into a character array.
	public:                                  // specification field number
		uint32_t magic;                      // 1 *
		uint32_t data_offset;                // 2 *
		char header_version[9];              // 3 *
		uint32_t total_file_size;            // 4
		uint32_t ditto_key;                  // 5 *
		uint32_t generic_header_length;      // 6
		uint32_t industry_header_length;     // 7
		uint32_t user_header_length;         // 8

		char filename[101];                  // 9
		char creation_time[25];              // 10
		struct tm creation_time_tm;          // 10a **
		char creator[101];                   // 12 (no field 11 in dpx spec)
		char project_name[201];              // 13
		char copyright[201];                 // 14
		uint32_t encryption_key;             // 15 *

		// skip 104 bytes                    // 16

		uint16_t image_orientation;          // 17
		uint16_t number_of_elements;         // 18
		uint32_t pixels_per_line;            // 19
		uint32_t lines_per_element;          // 20

		struct element_t {
			friend class dpxi::rwinfo;
			friend class dpx;
			uint32_t data_sign;              // 21.1 *
			uint32_t ref_low_data_code;      // 21.2 *
			float32_t ref_low_quantity;      // 21.3 *
			uint32_t ref_high_data_code;     // 21.4 *
			float32_t ref_high_quantity;     // 21.5 *
			uint8_t descriptor;              // 21.6 *
			uint8_t transfer_characteristic; // 21.7 *
			uint8_t colorimetric_characteristic; // 21.8 *
			uint8_t bits_per_sample;         // 21.9  *
			uint16_t packing;                // 21.10 *
			uint16_t encoding;               // 21.11 *
			uint32_t offset_to_data;         // 21.12
			uint32_t eol_padding;            // 21.13 *
			uint32_t eoi_padding;            // 21.14 *
			char description[33];            // 21.15

			private:
				uint16_t actual_packing;         // see dpx_validate.cc
		} elements[8];                       // 21-28

		// skip 52 bytes                     // 29

		uint32_t x_offset;                   // 30 *
		uint32_t y_offset;                   // 31 *
		float32_t x_center;                  // 32 *
		float32_t y_center;                  // 33 *
		uint32_t x_origional_size;           // 34 *
		uint32_t y_origional_size;           // 35 *

		char source_filename[101];           // 36
		char source_creation_time[25];       // 37
		struct tm source_creation_time_tm;   // 37a **
		char input_device[33];               // 38
		char input_device_serial_number[33]; // 39

		uint16_t xl_border_validity;         // 40a
		uint16_t xr_border_validity;         // 40b
		uint16_t yt_border_validity;         // 40c
		uint16_t yb_border_validity;         // 40d
		uint32_t horizonal_par;              // 41a 
		uint32_t vertical_par;               // 41b 

		float32_t x_scanned_size;            // 42.1
		float32_t y_scanned_size;            // 42.2

		// skip 20 bytes                     // 42.3

		// film information header...
		char keycode_film_id[3];             // 43
		char keycode_film_type_char[3];      // 44
		char keycode_perf_offset_char[3];    // 45 *
		char keycode_prefix[7];              // 47 (no field 46 in dpx)
		char keycode_count_char[5];          // 48

		char format[33];                     // 49
		uint32_t frame_position_in_sequence; // 50
		uint32_t sequence_length;            // 51
		uint32_t held_count;                 // 52
		float32_t frame_rate;                // 53
		float32_t shutter_angle;             // 54
		char frame_attribute[33];            // 55
		char frame_slate_info[101];          // 56
		float32_t film_gague;                // 57.0 *
		uint8_t film_orientation;            // 57.1 *
		uint8_t film_perf_per_foot;          // 57.2 *
		uint8_t film_perf_per_frame;         // 57.3 *
		char film_manufacturer[17];          // 57.4 *
		char film_stock_name[17];            // 57.5 *
		struct keycode {
			//enum film_id_type_kind {
			//	numeric=0,
			//	alpha=1
			//} film_id_type_kind;
			uint8_t id;
			uint8_t type;
			float32_t film_gague;
			enum film_orientation_e {
				vistavision=1,
				horizontal=1,
				normal=0,
			    vertical=0
			};
			uint8_t perf_per_frame;
			uint8_t perf_per_foot;
			std::string manufacturer;
			std::string stock_number;
			uint32_t prefix;
			uint16_t foot;
			uint8_t perf;
			// Add perf arithmatic operations...
		} keycode;                           // 57a **

		// skip 21 bytes (less than original, taken up by 57.x above)

		// video information header...
		uint32_t smpte_timecode;             // 58
		uint32_t smpte_userbits;             // 59
		uint8_t interlace;                   // 60
		uint8_t field_number;                // 61
		uint8_t video_standard;              // 62
		// skip 1 byte                       // 63
		float32_t horizontal_sampling_rate;  // 64
		float32_t vertical_sampling_rate;    // 65
		float32_t temporal_sampling_rate;    // 66
		float32_t time_offset_sync_to_first_pixel; // 67
		float32_t gamma;                     // 68
		float32_t black_level_code;          // 69
		float32_t black_gain;                // 70
		float32_t breakpoint;                // 71
		float32_t white_level_code;          // 72
		float32_t integration_time;          // 73
		// skip 76 bytes                     // 74

		// we should be at byte 2048 at this point...
		char user_id[33];                    // 75

		// end of header

		// Set on read() to either little_endian or big_endian based on
		// the file byte order. Used on write to determine if bytes
		// are to be written either in the native mode (0), always swapped (1),
		// swapped if not on a little endian architecture (2), swapped if
		// not on a big endian architecture (3).
		//
		// It is strongly suggested that this value be set to 'big_endian'
		// for all writes, since Cinepaint can only deal with network order
		// (big endian) dpx files.
		enum endian_mode_e {
			default_endian_mode=0,
			native=1,
			swapped=2,
			little_endian=3,
			big_endian=4,
		} endian_mode;

		// This field is used to control any alternate interpretation
		// of fields that may need to happen to read a file (or write a 
		// compatible file). On class construction this has a value of
		// 0 (automatic).
		//
		enum compliance_e {
			automatic =0,
			dpx1      =0x10000000,
			dpx3      =0x20000000,
		} compliance;

		// Some helper functions to determine if a field is 'NULL' or
		// to set a field to NULL.
		static bool isnull(uint64_t v);
		static bool isnull(uint32_t v);
		static bool isnull(uint16_t v);
		static bool isnull(uint8_t v);
		static bool isnull(float32_t v);

		static void nullify(uint16_t *v);
		static void nullify(uint8_t *v);
		static void nullify(uint32_t *v);
		static void nullify(uint64_t *v);
		static void nullify(float32_t *v);

		// mark all fields as NULL.
		void clear(void);

	private:
		bool _need_byteswap;
		friend class dpxi::rwinfo;
		std::ostream *current_ostream;
		compliance_e current_compliance;
		endian_mode_e current_endian_mode;

		std::istream::streampos header_start;

	public:
		virtual ~dpx();
		dpx();

		// read and write only the header...
		void read(std::istream *io);
		void write(std::ostream *io);

		static bool check_magic(std::istream *io);

		// The framebuffer (fb) class that we use for putting image
		// information into (and pulling out of) the file using the
		// read(...) and write(...) methods. We can't use Imf::Array2D
		// because that is part of the openexr libary, and we don't
		// want a dependency on openexr for ctl (we *do* have a
		// dependency on ilmbase, but that's OK).
		// 
		template <class T>
		class fb {
			private:
				T *_data;
				uint32_t _width;
				uint32_t _height;
				uint32_t _depth;
				uint64_t _length;

			public:	
				fb();
				~fb();

				void init(uint32_t width, uint32_t height, uint32_t depth);

				uint32_t width(void) const;
				uint32_t height(void) const;
				uint32_t depth(void) const;

				// the total number of bytes of storage
				uint64_t length(void) const;

				// The total number of pixels
				uint64_t pixels(void) const;

				// The total number of samples
				uint64_t count(void) const;

				// Takes the stored pixels and treats them as if they
				// were in the format described by the descriptor parameter
				// (i.e. header field 21.6), and then move things about so
				// the pixels are then ordered in a standard RGB(A)
				// (or Y(A)) format. Optionally this will remove the alpha
				// channel (set descriptor==0 if the data has already been
				// formatted into RGBA or YA).
				void swizzle(uint8_t descriptor, bool squish_alpha);
		
				// Indicates if the data has an alpha channel.	
				bool alpha() const;
				// Set the alpha channel to the specified value (creates
				// alpha channel if it does not exist).
				void alpha(const T &value);

				T *ptr(void);
				const T *ptr(void) const;
				operator T *();
				operator const T *() const;
		};

		// Exception classes...
		class exception : std::exception {};
		class invalid : exception {};
		class outofrange : exception {};
		class badtemplate : exception {};

		// 
		// These implicitly call read(std::istream *i); to get a
		// copy of the header.
		//
		// If the file's packing or bpp (or whatever...) is sufficiently
		// foreign that we can't handle it, we will thow an 'invalid'
		// exception.
		//
		// If the parameter element is out of range for the image (i.e.
		// greater than or equal to 'number_of_elements' then an
		// exception will be thrown.
		//
		enum intmode_e {
			normal=0,
			unformatted=1
		};
		void read(std::istream *io, uint8_t element, fb<half> *buffer,
		          float64_t scale=0.0);
		void read(std::istream *io, uint8_t element, fb<float32_t> *buffer,
		          float64_t scale=0.0);
		void read(std::istream *io, uint8_t element, fb<float64_t> *buffer,
		          float64_t scale=0.0);
		void read(std::istream *io, uint8_t element, fb<uint8_t> *buffer,
		          float64_t scale=0.0, intmode_e mode=normal);
		void read(std::istream *io, uint8_t element, fb<uint16_t> *buffer,
		          float64_t scale=0.0, intmode_e mode=normal);
		void read(std::istream *io, uint8_t element, fb<uint32_t> *buffer,
		          float64_t scale=0.0, intmode_e mode=normal);
		void read(std::istream *io, uint8_t element, fb<uint64_t> *buffer,
		          float64_t scale=0.0, intmode_e mode=normal);

		//
		// If not filled out ahead of time (i.e. have 'null' values associated
		// with them), then the following fields will be set based on
		// the version of the write method called. The defaults chosen are
		// affected by the value of the 'compliance' field (described above).
		// If compliance is set to 'automatic', then it is silently changed
		// on first write to 'dpx1'.
		//
		// Common across all values of 'compliance':
		//
		// field   default
		// ------  ----------------------------------------------------------
		// 17      0 (pixels arranged left to right, top to bottom).
		// 19      width of provided framebuffer
		// 20      height of provided framebuffer
		// 21.9    sizeof(<framebuffer type>)*8
		// 21.1    (see below)
		// 21.6    based on depth of framebuffer:
		//             depth  descriptor
		//             -----  -----------------------------
		//                 1  6 (Luminance)
		//                 2  158 (Luminance Alpha) (proposed)  (*)
		//                 3  50 (RGB)
		//                 4  51 (RGBA)
		//                 5  153 (user defined 5 channel image)
		//                 6  154 (user defined 6 channel image)
		//                 7  155 (user defined 7 channel image)
		//                 8  156 (user defined 8 channel image)
		// 21.7    1 if bits_per_sample==10, otherwise 2
		// 21.8    set equal to 21.7 (transfer characteristic)
		// 21.10   (**)(packing)
		// vaheader field valubits_per_sample, packing,
		// descriptor, colorimetric_characteristic, transfer_characteristic,
		// and offset_to_data will have reasonable default values placed
		// in them by the write(...) method. The default values are determined
		// on via the following scheme:
		//
		//
		// IMPORTANT NOTE: Unlike the read(...) methods, the write methods
		// presume that the interleaved data in the framebuffer is in the
		// same order as the descriptor.
		//
		//  All of these fields do not need
		// to be filled out prior to calling write(...), , defaults will only be used as needed, colorimetric, and and elements[n].offset_to_data will have
		// reasonable defaults set based on the number of channels in the
		// framebuffer to write, and the type of the framebuffer.
		// 
		// If there is a reasonable disconnect between the are not specified (i.e.
		// are not all 0xff) then appropriate values will be selected
		// based on the type provided in the frame buffer. 
		// If values are provided and do not match the framebuffer type
		// then a warning will be posted (see below) and an attempt will
		// be made to perform a conversion. If no conversion can be
		// performed then an exception is thrown.
		// Specifying the elements[n].offset_to_data is particularly
		// dangerous as it will will be used without any sort of sanity
		// checking and you can easily overwrite critical parts of your
		// file.
		//
		// The number_of_elements value *must* be set prior to the first
		// call to write. Failure to do this will cause an exception.
		// 
		// The compliance field (above) must also be set prior to the
		// first call/ to write. 
		//
		// Elements should be written (preferably in order), after the
		// last element has been written the header should be written.
		//
		// Like all of the read(...) methods, this will rewind the osream
		// to the point it was at when the write(...) method was called.
		//
		void write(std::ostream *o, uint8_t element,
		           const fb<half> &buffer, float64_t scale=0.0);
		void write(std::ostream *o, uint8_t element,
		           const fb<float32_t> &buffer, float64_t scale=0.0);
		void write(std::ostream *o, uint8_t element,
		           const fb<float64_t> &buffer, float64_t scale=0.0);
		void write(std::ostream *o, uint8_t element,
		           const fb<uint8_t> &buffer, float64_t scale=0.0,
		           intmode_e mode=normal);
		void write(std::ostream *o, uint8_t element,
		           const fb<uint16_t> &buffer, float64_t scale=0.0,
		           intmode_e mode=normal);
		void write(std::ostream *o, uint8_t element,
		           const fb<uint32_t> &buffer, float64_t scale=0.0,
		           intmode_e mode=normal);
		void write(std::ostream *o, uint8_t element,
		           const fb<uint64_t> &buffer, float64_t scale=0.0,
		           intmode_e mode=normal);

		// Can perform pretty much perform every imaginable conversion
		// between two types. While there are a number of functions, they
		// all eventually call the same method at the base of things, and
		// the variants are to give slightly cleaner interfaces. 
		//
		// The current interface only supports the following types (as either
		// inputs and outputs):
		//       uint8_t    (unsigned char)
		//       uint16_t   (unsigned short)
		//       uint32_t   (unsigned int)
		//       uint64_t   (unsigned long long int)
		//       float16_t  (half)
		//       float32_t  (float)
		//       float64_t  (double)
		//
		// Adding support for signed types is left as an excercise to the
		// user.
		//
		// The parameter meanings are identical across all functions. If
		// you are calling a function variant *without* the parameter, it
		// defaults to a zero value (the behavior is described below).
		//
		// O        template parameter that is the output type.
		// I        template parameter that is the input type.
		// isb      input significant bits, only used when then input type
		//          is an integer type. Will cause the input to be clipped
		//          from 0 to (1<<isb)-1. If set to zero then it will be
		//          treated as sizeof(I)*8.
		// osb      output significant bits, only used when the output type
		//          is an integer type. Will cause the output to be clipped
		//          from 0 to (1<<osb)-1. If set the zero then it will be
		//          treated as sizeof(O)*8.
		// count    number of samples to convert.
		// scale    a parameter whose behavior is highly dependent on the
		//          input and output types. There are two variable that
		//          are (effectively) treated as flags to the conversion
		//          functions. These two values are 0.0 (which will
		//          generally do what you want (DWYW)), and 1.0 (which
		//          will generally do what you expect (DWYE)). In a few
		//          cases what you want and what you expect are the same
		//          thing. The behavior of this parameter is:
		//              for float to float conversions:
		//                  DWYW   input is directly copied to the output
		//                  DWYE   input is directly copied to the output
		//                  other  input is multiplied by the scale
		//                         and placed in the output
		//              for integer to float conversions:
		//                  DWYW   clip the input to the range 0 to
		//                         (1<<isb)-1, convert the value to a
		//                         float, and divide the result by 
		//                         (1<<isb)-1. In short: converts the
		//                         input to a clipped range to 0.0 to 1.0.
		//                  DWYE   clip the input to the range 0 to 
		//                         (1<<isb)-1, store the value in the
		//                         floating point type.
		//                  other  clip the input to the range 0 to
		//                         (1<<isb)-1, converts to a float and
		//                         then divides that value by the scale.
		//              for float to integer conversions:
		//                  DWYW   clip input range to [0.0, 1.0], multiply
		//                         input by (1<<isb)-1 and then convert to
		//                         the integer type.
		//                  DWYE   convert input to float, clip output
		//                         range to [0, (1<<isb)-1]
		//                  other  multiply the input by the scale, clip to
		//                         [0, (1<<isb)-1], convert to integer.
		//              for integer to integer conversions:
		//                  DWYW   the input is clipped to the range
		//                         [0, (1<<isb)-1]. The input is scaled up
		//                         (or down) to the number of significant
		//                         bits. When scaling down this is done
		//                         simply by shifting, When scaling up,
		//                         significant bits of the input are copied
		//                         to the less significant bits to maintain
		//                         linearity.
		//                  DWYE   the input is clipped to the range
		//                         [0, (1<<isb)-1]. If the number of output
		//                         bits is less than the number of input
		//                         bits then the input value is shifted down.
		//                         If the number of output bits is greater
		//                         than the input, the value is simply
		//                         copied over.
		//                  other  convert the input to a float, multiply
		//                         by the scale, and then convert to an
		//                         integer by the rules in the DWYE float
		//                         to integer conversion. A float64_t is
		//                         used for the intermediate floating
		//                         point values.
		//
		// IMPORTANT NOTE:
		//    While these functions do everything possible to prevent loss
		// of bit accuracy, when dealing with large types (i.e. uint64_t and
		// uint32_t) it is inevitable that you will run into the loss of
		// significant bits.
		// 
		template <class O, class I>
		static void convert(O *o, const I *i, uint64_t count);
		template <class O, class I>
		static void convert(O *o, const I *i, float64_t scale, uint64_t count);
		template <class O, class I>
		static void convert(O *o, uint8_t osb, const I *i, uint64_t count);
		template <class O, class I>
		static void convert(O *o, uint8_t osb, const I *i, float64_t scale,
		                    uint64_t count);
		template <class O, class I>
		static void convert(O *o, const I *i, uint8_t isb, uint64_t count);
		template <class O, class I>
		static void convert(O *o, const I *i, uint8_t isb, float64_t scale,
		                    uint64_t count);
		template <class O, class I>
		static void convert(O *o, uint8_t osb, const I *i, uint8_t isb,
		                    uint64_t count);
		// This is the actual function that does the work. All of the others
		// just fill out default values...
		template <class O, class I>
		static void convert(O *o, uint8_t osb, const I *i, uint8_t isb,
		                    float64_t scale, uint64_t count);


		// Some static methods to take de-enumerate some of the
		// header values.
		static std::string smpte_timecode_to_string(uint32_t tc);
		static std::string colormetric_to_string(uint8_t id);
		static std::string transfer_to_string(uint8_t id);
		static std::string video_to_string(uint8_t id);
		static std::string encoding_to_string(uint8_t id);
		static std::string packing_to_string(uint8_t id);
		static std::string bits_per_sample_to_string(uint8_t id);
		static std::string orientation_to_string(uint8_t id);
		static std::string descriptor_to_string(uint8_t id);
};

#include <dpx.tcc>

}


#endif
