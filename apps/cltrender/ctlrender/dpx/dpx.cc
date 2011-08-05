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

#include "dpx.hh"
#include "dpx_raw.hh"
#include "dpx_bits.hh"
#include <string.h>

#if !defined(TRUE)
#define TRUE 1
#endif
#if !defined(FALSE)
#define FALSE 0
#endif

namespace ctl
{

dpx::dpx() {
	endian_mode=default_endian_mode;
	compliance=automatic;

	clear();

	header_start=0;
	_need_byteswap=FALSE;
	current_ostream=FALSE;
	current_endian_mode=default_endian_mode;
	current_compliance=automatic;
}

dpx::~dpx() {
}

void dpx::clear(void) {
	uint8_t i;

	nullify(&magic);
	magic=0x53445058;
	nullify(&data_offset);
	memset(header_version, 0, sizeof(header_version));
	nullify(&total_file_size);
	nullify(&ditto_key);
	nullify(&generic_header_length);
	nullify(&industry_header_length);
	nullify(&user_header_length);

	memset(filename, 0, sizeof(filename));
	memset(creation_time, 0, sizeof(creation_time));
	memset(creator, 0, sizeof(creator));
	memset(project_name, 0, sizeof(project_name));
	memset(copyright, 0, sizeof(copyright));
	nullify(&encryption_key);

	nullify(&image_orientation);
	nullify(&number_of_elements);
	nullify(&pixels_per_line);
	nullify(&lines_per_element);

	for(i=0; i<8; i++) {
		nullify(&(elements[i].data_sign));
		nullify(&(elements[i].ref_low_data_code));
		nullify(&(elements[i].ref_low_quantity));
		nullify(&(elements[i].ref_high_data_code));
		nullify(&(elements[i].ref_high_quantity));
		nullify(&(elements[i].descriptor));
		nullify(&(elements[i].transfer_characteristic));
		nullify(&(elements[i].colorimetric_characteristic));
		nullify(&(elements[i].bits_per_sample));
		nullify(&(elements[i].packing));
		nullify(&(elements[i].actual_packing));
		nullify(&(elements[i].encoding));
		nullify(&(elements[i].offset_to_data));
		nullify(&(elements[i].eol_padding));
		nullify(&(elements[i].eoi_padding));
		memset(elements[i].description, 0, sizeof(elements[i].description));
	};

	nullify(&x_offset);
	nullify(&y_offset);
	nullify(&x_center);
	nullify(&y_center);
	nullify(&x_origional_size);
	nullify(&y_origional_size);

	memset(source_filename, 0, sizeof(source_filename));
	memset(source_creation_time, 0, sizeof(source_creation_time));
	memset(input_device, 0, sizeof(input_device));
	memset(input_device_serial_number, 0, sizeof(input_device_serial_number));

	nullify(&xl_border_validity);
	nullify(&xr_border_validity);
	nullify(&yt_border_validity);
	nullify(&yb_border_validity);
	nullify(&horizonal_par);
	nullify(&vertical_par);

	nullify(&x_scanned_size);
	nullify(&y_scanned_size);

	memset(keycode_film_id, 0, sizeof(keycode_film_id));
	memset(keycode_film_type_char, 0, sizeof(keycode_film_type_char));
	memset(keycode_perf_offset_char, 0, sizeof(keycode_perf_offset_char));
	memset(keycode_prefix, 0, sizeof(keycode_prefix));
	memset(keycode_count_char, 0, sizeof(keycode_count_char));

	memset(format, 0, sizeof(format));
	nullify(&frame_position_in_sequence);
	nullify(&sequence_length);
	nullify(&held_count);
	nullify(&frame_rate);
	nullify(&shutter_angle);
	memset(frame_attribute, 0, sizeof(frame_attribute));
	memset(frame_slate_info, 0, sizeof(frame_slate_info));
//	nullify(&film_gague);
// 	nullify(&film_orientation);
//	nullify(&film_perf_per_foot);
//	nullify(&film_perf_per_frame);
//	memset(film_manufacturer, 0, sizeof(film_manufacturer));
//	memset(film_stock_name, 0, sizeof(film_stock_name));

	nullify(&smpte_timecode);
	nullify(&smpte_userbits);
	nullify(&interlace);
	nullify(&field_number);
	nullify(&video_standard);
	nullify(&horizontal_sampling_rate);
	nullify(&vertical_sampling_rate);
	nullify(&temporal_sampling_rate);
	nullify(&time_offset_sync_to_first_pixel);
	nullify(&gamma);
	nullify(&black_level_code);
	nullify(&black_gain);
	nullify(&breakpoint);
	nullify(&white_level_code);
	nullify(&integration_time);

	memset(user_id, 0, sizeof(user_id));
}


bool dpx::check_magic(std::istream *io) {
	uint32_t magic;
	std::istream::pos_type start;

	start=io->tellg();
	io->read((char *)&magic, sizeof(magic));
	io->seekg(start);

	if(magic==0x53445058 || magic==0x58504453) {
		return TRUE;
	}
	return FALSE;
}

void dpx::read(std::istream *is) {
	std::istream::pos_type start;
	bool cpu_is_little_endian;
	int i;

	magic=0x01020304;
	cpu_is_little_endian=FALSE;
	if(((const char *)&magic)[0]==0x04) {
		cpu_is_little_endian=TRUE;
	}

	clear();

	header_start=is->tellg();
	magic=dpxi::read_uint32(is, 0);
	if(magic==0x53445058) {
		_need_byteswap=FALSE;
		if(cpu_is_little_endian) {
			endian_mode=little_endian;
		} else {
			endian_mode=big_endian;
		}
	} else {
		is->seekg(header_start);
		magic=dpxi::read_uint32(is, 1);
		if(magic!=0x53445058) {
			// XXX This is bad... Need to fail nicely..
		}
		_need_byteswap=TRUE;
		if(cpu_is_little_endian) {
			endian_mode=big_endian;
		} else {
			endian_mode=little_endian;
		}
	}

	data_offset=dpxi::read_uint32(is, _need_byteswap);
	dpxi::read_string(is, header_version, sizeof(header_version));
	total_file_size=dpxi::read_uint32(is, _need_byteswap);
	ditto_key=dpxi::read_uint32(is, _need_byteswap);
	generic_header_length=dpxi::read_uint32(is, _need_byteswap);
	industry_header_length=dpxi::read_uint32(is, _need_byteswap);
	user_header_length=dpxi::read_uint32(is, _need_byteswap);

	dpxi::read_string(is, filename, sizeof(filename));
	dpxi::read_string(is, creation_time, sizeof(creation_time));
	dpxi::read_string(is, creator, sizeof(creator));
	dpxi::read_string(is, project_name, sizeof(project_name));
	dpxi::read_string(is, copyright, sizeof(copyright));
	encryption_key=dpxi::read_uint32(is, _need_byteswap);

	is->ignore(104);

	image_orientation=dpxi::read_uint16(is, _need_byteswap);
	number_of_elements=dpxi::read_uint16(is, _need_byteswap);
	pixels_per_line=dpxi::read_uint32(is, _need_byteswap);
	lines_per_element=dpxi::read_uint32(is, _need_byteswap);

	for(i=0; i<8; i++) {
		elements[i].data_sign=dpxi::read_uint32(is, _need_byteswap);
		elements[i].ref_low_data_code=dpxi::read_uint32(is, _need_byteswap);
		elements[i].ref_low_quantity=dpxi::read_float32(is, _need_byteswap);
		elements[i].ref_high_data_code=dpxi::read_uint32(is, _need_byteswap);
		elements[i].ref_high_quantity=dpxi::read_float32(is, _need_byteswap);
		elements[i].descriptor=dpxi::read_uint8(is, _need_byteswap);
		elements[i].transfer_characteristic=dpxi::read_uint8(is, _need_byteswap);
		elements[i].colorimetric_characteristic=dpxi::read_uint8(is, _need_byteswap);
		elements[i].bits_per_sample=dpxi::read_uint8(is, _need_byteswap);
		elements[i].packing=dpxi::read_uint16(is, _need_byteswap);
		elements[i].encoding=dpxi::read_uint16(is, _need_byteswap);
		elements[i].offset_to_data=dpxi::read_uint32(is, _need_byteswap);
		if(i<number_of_elements &&
		   (elements[i].offset_to_data==(uint32_t)-1 ||
		    elements[i].offset_to_data==0)) {
			// Both of the above have been seen in the wild. And both
			// are non-spec (since offset_to_data is a core field it canot
			// be marked as UNDEF, and the spec states that offset_to_data
			// is relative to file start, not the data_offset field). The
			// second (arguably more problematic) is generated by cinepaint.
			elements[i].offset_to_data=data_offset;
		}
		elements[i].eol_padding=dpxi::read_uint32(is, _need_byteswap);
		elements[i].eoi_padding=dpxi::read_uint32(is, _need_byteswap);
		dpxi::read_string(is, elements[i].description,
		            sizeof(elements[i].description));
	};

	is->ignore(52);

	x_offset=dpxi::read_uint32(is, _need_byteswap);
	y_offset=dpxi::read_uint32(is, _need_byteswap);
	x_center=dpxi::read_float32(is, _need_byteswap);
	y_center=dpxi::read_float32(is, _need_byteswap);
	x_origional_size=dpxi::read_uint32(is, _need_byteswap);
	y_origional_size=dpxi::read_uint32(is, _need_byteswap);

	dpxi::read_string(is, source_filename, sizeof(source_filename));
	dpxi::read_string(is, source_creation_time, sizeof(source_creation_time));
	dpxi::read_string(is, input_device, sizeof(input_device));
	dpxi::read_string(is, input_device_serial_number,
	                  sizeof(input_device_serial_number));

	xl_border_validity=dpxi::read_uint16(is, _need_byteswap);
	xr_border_validity=dpxi::read_uint16(is, _need_byteswap);
	yt_border_validity=dpxi::read_uint16(is, _need_byteswap);
	yb_border_validity=dpxi::read_uint16(is, _need_byteswap);
	horizonal_par=dpxi::read_uint32(is, _need_byteswap);
	vertical_par=dpxi::read_uint32(is, _need_byteswap);

	x_scanned_size=dpxi::read_float32(is, _need_byteswap);
	y_scanned_size=dpxi::read_float32(is, _need_byteswap);

	is->ignore(20);

	dpxi::read_string(is, keycode_film_id, sizeof(keycode_film_id));
	dpxi::read_string(is, keycode_film_type_char,
	                  sizeof(keycode_film_type_char));
	dpxi::read_string(is, keycode_perf_offset_char,
	                  sizeof(keycode_perf_offset_char));
	dpxi::read_string(is, keycode_prefix, sizeof(keycode_prefix));
	dpxi::read_string(is, keycode_count_char, sizeof(keycode_count_char));

	dpxi::read_string(is, format, sizeof(format));
	frame_position_in_sequence=dpxi::read_uint32(is, _need_byteswap);
	sequence_length=dpxi::read_uint32(is, _need_byteswap);
	held_count=dpxi::read_uint32(is, _need_byteswap);
	frame_rate=dpxi::read_float32(is, _need_byteswap);
	shutter_angle=dpxi::read_float32(is, _need_byteswap);
	dpxi::read_string(is, frame_attribute, sizeof(frame_attribute));
	dpxi::read_string(is, frame_slate_info, sizeof(frame_slate_info));
	film_gague=dpxi::read_float32(is, _need_byteswap);
	film_orientation=dpxi::read_uint8(is, _need_byteswap);
	film_perf_per_foot=dpxi::read_uint8(is, _need_byteswap);
	film_perf_per_frame=dpxi::read_uint8(is, _need_byteswap);
	dpxi::read_string(is, film_manufacturer, sizeof(film_manufacturer));
	dpxi::read_string(is, film_stock_name, sizeof(film_stock_name));

	is->ignore(21);

	smpte_timecode=dpxi::read_uint32(is, _need_byteswap);
	smpte_userbits=dpxi::read_uint32(is, _need_byteswap);
	interlace=dpxi::read_uint8(is, _need_byteswap);
	field_number=dpxi::read_uint8(is, _need_byteswap);
	video_standard=dpxi::read_uint8(is, _need_byteswap);
	is->ignore(1);
	horizontal_sampling_rate=dpxi::read_float32(is, _need_byteswap);
	vertical_sampling_rate=dpxi::read_float32(is, _need_byteswap);
	temporal_sampling_rate=dpxi::read_float32(is, _need_byteswap);
	time_offset_sync_to_first_pixel=dpxi::read_float32(is, _need_byteswap);
	gamma=dpxi::read_float32(is, _need_byteswap);
	black_level_code=dpxi::read_float32(is, _need_byteswap);
	black_gain=dpxi::read_float32(is, _need_byteswap);
	breakpoint=dpxi::read_float32(is, _need_byteswap);
	white_level_code=dpxi::read_float32(is, _need_byteswap);
	integration_time=dpxi::read_float32(is, _need_byteswap);

	is->ignore(76);
	dpxi::read_string(is, user_id, sizeof(user_id));

	is->seekg(header_start);
	// struct tm source_creation_time_tm;
	// time_t source_creation_time_time;
	// uint8_t keycode_perf_offset_int;
	// uint16_t keycode_film_type_int;
	// uint8_t keycode_prefix_int;
	// uint16_t keycode_count_int;
	// uint64_t keycode_absolute_perf_offset;
	// struct tm creation_time_tm;
	// time_t creation_time_time;
}	

void dpx::write(std::ostream *os)
{
	uint32_t _magic;
	bool cpu_is_little_endian;
	int i;

	_magic=0x01020304;
	cpu_is_little_endian=FALSE;
	if(((const char *)&_magic)[0]==0x04) {
		cpu_is_little_endian=TRUE;
	}

	_need_byteswap=FALSE;
	if(cpu_is_little_endian!=little_endian) {
		_need_byteswap=TRUE;
	}

	// struct tm source_creation_time_tm;
	// time_t source_creation_time_time;
    // uint8_t keycode_perf_offset_int;
	// uint16_t keycode_film_type_int;
    // uint8_t keycode_prefix_int;
    // uint16_t keycode_count_int;
	// uint64_t keycode_absolute_perf_offset;
	// struct tm creation_time_tm;
	// time_t creation_time_time;

	os->seekp(header_start);

	if(header_version[0]==0) {
		if(compliance==dpx1) {
			strcpy(header_version, "v1.0");
		} else if(compliance==dpx3) {
			strcpy(header_version, "v3.0");
		} else {
 			// XXX invalid validation_level
		}
	}

	magic=0x53445058;
	dpxi::write_uint32(os, magic, _need_byteswap);
	dpxi::write_uint32(os, data_offset, _need_byteswap);
	dpxi::write_string(os, header_version, sizeof(header_version));
	dpxi::write_uint32(os, total_file_size, _need_byteswap);
	dpxi::write_uint32(os, ditto_key, _need_byteswap);
	dpxi::write_uint32(os, generic_header_length, _need_byteswap);
	dpxi::write_uint32(os, industry_header_length, _need_byteswap);
	dpxi::write_uint32(os, user_header_length, _need_byteswap);

	dpxi::write_string(os, filename, sizeof(filename));
	dpxi::write_string(os, creation_time, sizeof(creation_time));
	dpxi::write_string(os, creator, sizeof(creator));
	dpxi::write_string(os, project_name, sizeof(project_name));
	dpxi::write_string(os, copyright, sizeof(copyright));
	dpxi::write_uint32(os, encryption_key, _need_byteswap);

	dpxi::write_fill(os, 0x00, 104);

	dpxi::write_uint16(os, image_orientation, _need_byteswap);
	dpxi::write_uint16(os, number_of_elements, _need_byteswap);
	dpxi::write_uint32(os, pixels_per_line, _need_byteswap);
	dpxi::write_uint32(os, lines_per_element, _need_byteswap);

	for(i=0; i<8; i++) {
		if(i>=number_of_elements) {
			nullify(&(elements[i].data_sign));
			nullify(&(elements[i].ref_low_data_code));
			nullify(&(elements[i].ref_low_quantity));
			nullify(&(elements[i].ref_high_data_code));
			nullify(&(elements[i].ref_high_quantity));
			nullify(&(elements[i].descriptor));
			nullify(&(elements[i].transfer_characteristic));
			nullify(&(elements[i].colorimetric_characteristic));
			nullify(&(elements[i].bits_per_sample));
			nullify(&(elements[i].packing));
			nullify(&(elements[i].encoding));
			nullify(&(elements[i].offset_to_data));
			nullify(&(elements[i].eol_padding));
			nullify(&(elements[i].eoi_padding));
			memset(elements[i].description, 0,
			       sizeof(elements[i].description));
		}

		dpxi::write_uint32(os, elements[i].data_sign, _need_byteswap);
		dpxi::write_uint32(os, elements[i].ref_low_data_code, _need_byteswap);
		dpxi::write_float32(os, elements[i].ref_low_quantity, _need_byteswap);
		dpxi::write_uint32(os, elements[i].ref_high_data_code, _need_byteswap);
		dpxi::write_float32(os, elements[i].ref_high_quantity, _need_byteswap);
		dpxi::write_uint8(os, elements[i].descriptor, _need_byteswap);
		dpxi::write_uint8(os, elements[i].transfer_characteristic,
		                  _need_byteswap);
		dpxi::write_uint8(os, elements[i].colorimetric_characteristic,
		                  _need_byteswap);
		dpxi::write_uint8(os, elements[i].bits_per_sample, _need_byteswap);
		// This is kind of sticky... The two commonly found DPX 1.0/2.0 files
		// are either the 10 bit samples in 32 bit words, swapped on 32 bit
		// words (i.e. cineon) or 16 bit samples in 16 bit words swapped on
		// 16 bit words (i.e. fimlight linear). We silently limit the range
		// of the packing to DPX 1.0 values.
		if(compliance==dpx1 && elements[i].packing>=8) {
			dpxi::write_uint16(os, (elements[i].packing)&0x7, _need_byteswap);
		} else {
			dpxi::write_uint16(os, elements[i].packing, _need_byteswap);
		}
		dpxi::write_uint16(os, elements[i].encoding, _need_byteswap);
		dpxi::write_uint32(os, elements[i].offset_to_data, _need_byteswap);
		dpxi::write_uint32(os, elements[i].eol_padding, _need_byteswap);
		dpxi::write_uint32(os, elements[i].eoi_padding, _need_byteswap);
		dpxi::write_string(os, elements[i].description,
		            sizeof(elements[i].description));
	};

	dpxi::write_fill(os, 0x00, 52);

	dpxi::write_uint32(os, x_offset, _need_byteswap);
	dpxi::write_uint32(os, y_offset, _need_byteswap);
	dpxi::write_float32(os, x_center, _need_byteswap);
	dpxi::write_float32(os, y_center, _need_byteswap);
	dpxi::write_uint32(os, x_origional_size, _need_byteswap);
	dpxi::write_uint32(os, y_origional_size, _need_byteswap);

	dpxi::write_string(os, source_filename, sizeof(source_filename));
	dpxi::write_string(os, source_creation_time, sizeof(source_creation_time));
	dpxi::write_string(os, input_device, sizeof(input_device));
	dpxi::write_string(os, input_device_serial_number,
	                  sizeof(input_device_serial_number));

	dpxi::write_uint16(os, xl_border_validity, _need_byteswap);
	dpxi::write_uint16(os, xr_border_validity, _need_byteswap);
	dpxi::write_uint16(os, yt_border_validity, _need_byteswap);
	dpxi::write_uint16(os, yb_border_validity, _need_byteswap);
	dpxi::write_uint32(os, horizonal_par, _need_byteswap);
	dpxi::write_uint32(os, vertical_par, _need_byteswap);

	dpxi::write_float32(os, x_scanned_size, _need_byteswap);
	dpxi::write_float32(os, y_scanned_size, _need_byteswap);

	dpxi::write_fill(os, 0x00, 20);

	dpxi::write_string(os, keycode_film_id, sizeof(keycode_film_id));
	dpxi::write_string(os, keycode_film_type_char,
	                   sizeof(keycode_film_type_char));
	dpxi::write_string(os, keycode_perf_offset_char,
	                   sizeof(keycode_perf_offset_char));
	dpxi::write_string(os, keycode_prefix, sizeof(keycode_prefix));
	dpxi::write_string(os, keycode_count_char, sizeof(keycode_count_char));

	dpxi::write_string(os, format, sizeof(format));
	dpxi::write_uint32(os, frame_position_in_sequence, _need_byteswap);
	dpxi::write_uint32(os, sequence_length, _need_byteswap);
	dpxi::write_uint32(os, held_count, _need_byteswap);
	dpxi::write_float32(os, frame_rate, _need_byteswap);
	dpxi::write_float32(os, shutter_angle, _need_byteswap);
	dpxi::write_string(os, frame_attribute, sizeof(frame_attribute));
	dpxi::write_string(os, frame_slate_info, sizeof(frame_slate_info));
//	dpxi::write_float32(os, film_gague, _need_byteswap);
// 	dpxi::write_uint8(os, film_orientation, _need_byteswap);
//	dpxi::write_uint8(os, film_perf_per_foot, _need_byteswap);
//	dpxi::write_uint8(os, film_perf_per_frame, _need_byteswap);
//	dpxi::write_string(os, film_manufacturer, sizeof(film_manufacturer));
//	dpxi::write_string(os, film_stock_name, sizeof(film_stock_name));
	dpxi::write_fill(os, 0x00, 108);

	dpxi::write_fill(os, 0x00, 21);

	dpxi::write_uint32(os, smpte_timecode, _need_byteswap);
	dpxi::write_uint32(os, smpte_userbits, _need_byteswap);
	dpxi::write_uint8(os, interlace, _need_byteswap);
	dpxi::write_uint8(os, field_number, _need_byteswap);
	dpxi::write_uint8(os, video_standard, _need_byteswap);
	dpxi::write_fill(os, 0x00, 1);
	dpxi::write_float32(os, horizontal_sampling_rate, _need_byteswap);
	dpxi::write_float32(os, vertical_sampling_rate, _need_byteswap);
	dpxi::write_float32(os, temporal_sampling_rate, _need_byteswap);
	dpxi::write_float32(os, time_offset_sync_to_first_pixel, _need_byteswap);
	dpxi::write_float32(os, gamma, _need_byteswap);
	dpxi::write_float32(os, black_level_code, _need_byteswap);
	dpxi::write_float32(os, black_gain, _need_byteswap);
	dpxi::write_float32(os, breakpoint, _need_byteswap);
	dpxi::write_float32(os, white_level_code, _need_byteswap);
	dpxi::write_float32(os, integration_time, _need_byteswap);

	dpxi::write_fill(os, 0x00, 76);
	dpxi::write_string(os, user_id, sizeof(user_id));

	current_ostream=NULL;
}	

}
