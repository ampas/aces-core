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

#include "tiff_file.hh"
#include <stdarg.h>
#include <dpx.hh>
#if defined(HAVE_LIBTIFF)
#include <tiff.h>
#include <tiffio.h>
#include <sys/param.h>
#include <math.h>
#include <Iex.h>
#include <alloca.h>

void tiff_read_multiplane(TIFF *t, float scale, ctl::dpx::fb<half> * pixels);
void tiff_read_interleaved(TIFF *t, float scale, ctl::dpx::fb<half> * pixels);
void tiff_read_failsafe(TIFF *t, float scale, ctl::dpx::fb<half> * pixels);

void tiff_interleave_int8(half *row, int offset, float scale,
                          uint8_t *r, int r_stride, uint8_t *g, int g_stride,
                          uint8_t *b, int b_stride, uint8_t *a, int a_stride,
                          uint32_t width);
                           
void tiff_interleave_int16(half *row, int offset, float scale,
                           uint16_t *r, int r_stride, uint16_t *g, int g_stride,
                           uint16_t *b, int b_stride, uint16_t *a, int a_stride,
                           uint32_t width);

void tiff_interleave_float(half *row, float scale,
                           float *r, int r_stride, float *g, int g_stride,
                           float *b, int b_stride, float *a, int a_stride,
                           uint32_t width);

void ErrorHandler(const char *module, const char *fmt, va_list ap) {
	fprintf(stderr, "Unable to read tiff file: ");
	vfprintf(stderr, fmt, ap);
}

void WarningHandler(const char *module, const char *fmt, va_list ap) {
//	fprintf(stderr, "tiff wrn: %s - ");
//	vfprintf(stderr, fmt, ap);
}

bool tiff_read(const char *name, float scale, ctl::dpx::fb<half> *pixels,
               format_t *format) {
	TIFF *t;
	uint16_t samples_per_pixel;
	uint16_t bits_per_sample;
	uint16_t sample_format;
	uint16_t planar_config;
	uint16_t photometric;
	uint16_t orientation;

	TIFFSetErrorHandler(ErrorHandler);
	TIFFSetWarningHandler(WarningHandler);

	t=TIFFOpen(name, "r");
	if(t==NULL) {
		// This is set if the file is not a tiff, we just sort of punt.
		return FALSE;
	}

	TIFFGetFieldDefaulted(t, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
	TIFFGetFieldDefaulted(t, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	format->src_bps=bits_per_sample;
	TIFFGetFieldDefaulted(t, TIFFTAG_SAMPLEFORMAT, &sample_format);
	TIFFGetFieldDefaulted(t, TIFFTAG_PHOTOMETRIC, &photometric);
	TIFFGetFieldDefaulted(t, TIFFTAG_ORIENTATION, &orientation);

//	tiff_read_failsafe(t, scale, pixels);
//	return TRUE;

	if(!(bits_per_sample==16 && sample_format<3) &&
	   !(bits_per_sample==32 && sample_format==3) &&
	   photometric!=PHOTOMETRIC_RGB &&
	   orientation!=ORIENTATION_TOPLEFT &&
	   orientation!=ORIENTATION_BOTLEFT) {

		if(bits_per_sample!=8) {
			fprintf(stderr, "falling back to failsafe TIFF reader. Reading "
			        "as \n8 bits per sample RGBA.\n");
		}
		tiff_read_failsafe(t, scale, pixels);
		TIFFClose(t);
		return TRUE;
	}

	TIFFGetField(t, TIFFTAG_PLANARCONFIG, &planar_config);
	if(planar_config==PLANARCONFIG_CONTIG) {
		tiff_read_interleaved(t, scale, pixels);
	} else if(planar_config==PLANARCONFIG_SEPARATE) {
		tiff_read_multiplane(t, scale, pixels);
	}

	TIFFClose(t);

	return TRUE;
}

void tiff_interleave_int8(half *o, int offset, float scale,
                          uint8_t *r, int r_stride, uint8_t *g, int g_stride,
                          uint8_t *b, int b_stride, uint8_t *a, int a_stride,
                          uint32_t width) {
	uint32_t i;
	float f;

	if(scale==0) {
		scale=255.0;
	}

	for(i=0; i<width; i++) {
		if(r!=NULL) {
			f=*r+offset;
			r=r+r_stride;
			*(o++)=f/scale;
		}
		if(g!=NULL) {
			f=*g+offset;
			g=g+g_stride;
			*(o++)=f/scale;
		}
		if(b!=NULL) {
			f=*b+offset;
			b=b+b_stride;
			*(o++)=f/scale;
		}
		if(a!=NULL) {
			f=*a+offset;
			a=a+a_stride;
			*(o++)=f/scale;
		}
	}
}

                           
void tiff_interleave_int16(half *o, uint16_t offset, float scale,
                           uint16_t *r, int r_stride, uint16_t *g, int g_stride,
                           uint16_t *b, int b_stride, uint16_t *a, int a_stride,
                           uint32_t width) {
	uint32_t i;
	float f;

	if(scale==0) {
		scale=65535.0;
	}

	for(i=0; i<width; i++) {
		if(r!=NULL) {
			f=*r+offset;
			r=r+r_stride;
			*(o++)=f/scale;
		}
		if(g!=NULL) {
			f=*g+offset;
			g=g+g_stride;
			*(o++)=f/scale;
		}
		if(b!=NULL) {
			f=*b+offset;
			b=b+b_stride;
			*(o++)=f/scale;
		}
		if(a!=NULL) {
			f=*a+offset;
			a=a+a_stride;
			*(o++)=f/scale;
		}
	}
}

void tiff_interleave_float(half *o, float scale,
                           float *r, int r_stride, float *g, int g_stride,
                           float *b, int b_stride, float *a, int a_stride,
                           uint32_t width) {
	uint32_t i;
	float f;

	if(scale==0) {
		scale=1.0;
	}

	for(i=0; i<width; i++) {
		if(r!=NULL) {
			f=*r;
			r=r+r_stride;
			*(o++)=f/scale;
		}
		if(g!=NULL) {
			f=*g;
			g=g+g_stride;
			*(o++)=f/scale;
		}
		if(b!=NULL) {
			f=*b;
			b=b+b_stride;
			*(o++)=f/scale;
		}
		if(a!=NULL) {
			f=*a;
			a=a+a_stride;
			*(o++)=f/scale;
		}
	}
}

void tiff_read_multiplane(TIFF *t, float scale, ctl::dpx::fb<half> * pixels) {
	uint8_t *scanline_buffer_uint8[4];
	uint16_t *scanline_buffer_uint16[4];
	float *scanline_buffer_float[4];
	uint16_t samples_per_pixel;
	uint16_t bits_per_sample;
	uint32_t w;
	uint32_t h;
	uint16_t sample_format;
	uint16_t offset;
	uint16_t orientation;
	tsize_t scanline_size;
	half *row_ptr;
	uint32_t row;
	uint32_t orientation_offset;
	uint16_t d;

	TIFFGetFieldDefaulted(t, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetFieldDefaulted(t, TIFFTAG_IMAGELENGTH, &h);
	TIFFGetFieldDefaulted(t, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
	TIFFGetFieldDefaulted(t, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	TIFFGetFieldDefaulted(t, TIFFTAG_SAMPLEFORMAT, &sample_format);
	TIFFGetFieldDefaulted(t, TIFFTAG_ORIENTATION, &orientation);

	pixels->init(w, h, samples_per_pixel);

	orientation_offset=0;
	if(orientation==ORIENTATION_LEFTTOP) {
		// We only deal with the bottom->top flip, not the other orientation
		// modes (the actual check for this is in tiff_read).
		orientation_offset=(uint32_t)-h;
	}

	scanline_size=TIFFScanlineSize(t);
	if(bits_per_sample==8) {
		for(row=0; row<4; row++) {
			if(row<samples_per_pixel) {
				scanline_buffer_uint8[row]=(uint8_t *)alloca(scanline_size);
				for(d=0; d<w; d++) {
					scanline_buffer_uint8[row][d]= row==3 ? 255 : 0;
				}
			} else {
				scanline_buffer_uint8[row]=NULL;
			}
		}
		for(;row<4; row++) {
		}
		offset=0;
		if(sample_format==2) {
			offset=1<<7;
		}
		for(row=0; row<h; row++) {
			for(d=0; d<samples_per_pixel; d++) {
				TIFFReadScanline(t, scanline_buffer_uint8[d],
				                 row+orientation_offset, d);
			}
			row_ptr=pixels->ptr()+row*pixels->width()*pixels->depth();
			tiff_interleave_int8(row_ptr, offset, scale,
			                     scanline_buffer_uint8[0], 1,
			                     scanline_buffer_uint8[1], 1,
			                     scanline_buffer_uint8[2], 1,
			                     scanline_buffer_uint8[3], 1,
			                     w);
		}
	} else if(bits_per_sample==16) {
		for(row=0; row<4; row++) {
			if(row<samples_per_pixel) {
				scanline_buffer_uint16[row]=(uint16_t *)alloca(scanline_size);
				for(d=0; d<w; d++) {
					scanline_buffer_uint16[row][d]= row==3 ? 65535 : 0;
				}
			} else {
				scanline_buffer_uint16[row]=NULL;
			}
		}
		offset=0;
		if(sample_format==2) {
			offset=1<<15;
		}
		for(row=0; row<h; row++) {
			for(d=0; d<samples_per_pixel; d++) {
				TIFFReadScanline(t, scanline_buffer_uint16[d],
				                 row+orientation_offset, d);
			}
			row_ptr=pixels->ptr()+row*pixels->width()*pixels->depth();
			tiff_interleave_int16(row_ptr, offset, scale,
			                      scanline_buffer_uint16[0], 1,
			                      scanline_buffer_uint16[1], 1,
			                      scanline_buffer_uint16[2], 1,
			                      scanline_buffer_uint16[3], 1,
			                      w);
		}
	} else if(sample_format==3) {
		for(row=0; row<4; row++) {
			if(row<samples_per_pixel) {
				scanline_buffer_float[row]=(float *)alloca(scanline_size);
				for(d=0; d<w; d++) {
					scanline_buffer_float[row][d]= row==3 ? 1.0 : 0.0;
				}
			} else {
				scanline_buffer_float[row]=NULL;
			}
		}
		for(row=0; row<h; row++) {
			for(d=0; d<samples_per_pixel; d++) {
				TIFFReadScanline(t, scanline_buffer_float[d],
				                 row+orientation_offset, d);
			}
			row_ptr=pixels->ptr()+row*pixels->width()*pixels->depth();
			tiff_interleave_float(row_ptr, scale,
			                      scanline_buffer_float[0], 1,
			                      scanline_buffer_float[1], 1,
			                      scanline_buffer_float[2], 1,
			                      scanline_buffer_float[3], 1,
			                      w);
		}
	}
}

void tiff_read_interleaved(TIFF *t, float scale, ctl::dpx::fb<half> * pixels) {
	uint8_t *scanline_buffer_uint8;
	uint16_t *scanline_buffer_uint16;
	float *scanline_buffer_float;
	uint16_t samples_per_pixel;
	uint16_t bits_per_sample;
	uint32_t w;
	uint32_t h;
	uint16_t sample_format;
	uint16_t offset;
	uint32_t row;
	half *row_ptr;

	TIFFGetFieldDefaulted(t, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetFieldDefaulted(t, TIFFTAG_IMAGELENGTH, &h);
	TIFFGetFieldDefaulted(t, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
	TIFFGetFieldDefaulted(t, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	TIFFGetFieldDefaulted(t, TIFFTAG_SAMPLEFORMAT, &sample_format);
	pixels->init(w, h, samples_per_pixel);

	if(bits_per_sample==8) {
		scanline_buffer_uint8=(uint8_t *)alloca(TIFFScanlineSize(t));
		offset=0;
		if(sample_format==2) {
			offset=127;
		}
		for(row=0; row<h; row++) {
			TIFFReadScanline(t, scanline_buffer_uint8, row, 0);
			row_ptr=pixels->ptr()+row*pixels->width()*pixels->depth();
			if(samples_per_pixel==3) {
				tiff_interleave_int8(row_ptr, offset, scale,
				                     scanline_buffer_uint8+0, 3,
				                     scanline_buffer_uint8+1, 3,
				                     scanline_buffer_uint8+2, 3,
				                     NULL, 0,
				                     w);
			} else {
				tiff_interleave_int8(row_ptr, offset, scale,
				                     scanline_buffer_uint8+0, 4,
				                     scanline_buffer_uint8+1, 4,
				                     scanline_buffer_uint8+2, 4,
				                     scanline_buffer_uint8+3, 4,
				                     w);
			}
		}
	} else if(bits_per_sample==16) {
		scanline_buffer_uint16=(uint16_t *)alloca(TIFFScanlineSize(t));
		offset=0;
		if(sample_format==2) {
			offset=32767;
		}
		for(row=0; row<h; row++) {
			TIFFReadScanline(t, scanline_buffer_uint16, row, 0);
			row_ptr=pixels->ptr()+row*pixels->width()*pixels->depth();
			if(samples_per_pixel==3) {
				tiff_interleave_int16(row_ptr, offset, scale,
				                      scanline_buffer_uint16+0, 3,
				                      scanline_buffer_uint16+1, 3,
				                      scanline_buffer_uint16+2, 3,
				                      NULL, 0,
				                      w);
			} else {
				tiff_interleave_int16(row_ptr, offset, scale,
				                      scanline_buffer_uint16+0, 4,
				                      scanline_buffer_uint16+1, 4,
				                      scanline_buffer_uint16+2, 4,
				                      scanline_buffer_uint16+3, 4,
				                      w);
			}
		}
	} else if(sample_format==3) {
		scanline_buffer_float=(float *)alloca(TIFFScanlineSize(t));
		for(row=0; row<h; row++) {
			TIFFReadScanline(t, scanline_buffer_float, row, 0);
			row_ptr=pixels->ptr()+row*pixels->width()*pixels->depth();
			if(samples_per_pixel==3) {
				tiff_interleave_float(row_ptr, scale,
				                      scanline_buffer_float+0, 3,
				                      scanline_buffer_float+1, 3,
				                      scanline_buffer_float+2, 3,
				                      NULL, 0,
				                      w);
			} else {
				tiff_interleave_float(row_ptr, scale,
				                      scanline_buffer_float+0, 4,
				                      scanline_buffer_float+1, 4,
				                      scanline_buffer_float+2, 4,
				                      scanline_buffer_float+3, 4,
				                      w);
			}
		}
	}
}

void tiff_read_failsafe(TIFF *t, float scale, ctl::dpx::fb<half> *pixels) {
	uint8_t *temp_buffer;
	uint8_t *flip;
	uint32_t i;
	uint32_t w, h;

	TIFFGetFieldDefaulted(t, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetFieldDefaulted(t, TIFFTAG_IMAGELENGTH, &h);
	pixels->init(w, h, 4);

	temp_buffer=(uint8_t *)alloca(w*h*4);
	TIFFReadRGBAImage(t, w, h, (uint32 *)temp_buffer, 0);

	for(i=0; i<h; i++) {
		flip=temp_buffer+sizeof(uint32_t)*w*(h-i-1);
		tiff_interleave_int8(pixels->ptr()+w*i*4, 0, scale,
		                     flip+0, 4, flip+1, 4, flip+2, 4, flip+3, 4, w);
	}
}

void tiff_convert_uint8(uint8_t *data, const half *in,
                        float scale, uint32_t width) {
#if 1
	ctl::dpx::convert(data, in, 0.0, width);
#else
	uint32_t i;
	float f;

	if(scale==0.0) {
		scale=1ULL<<15;
	}

	for(i=0; i<width; i++) {
		f=*in;
		f=f*scale;
		if(f<0.0) { f=0.0; }
		if(f>254.999) { f=254.999; }
		*(data++)=lrint(f);
		in++;
	}
#endif
}

void tiff_convert_uint16(uint16_t *data, const half *in,
                         float scale, uint32_t width) {
#if 1
	ctl::dpx::convert(data, in, 0.0, width);
#else
	uint32_t i;
	float f;

	if(scale==0.0) {
		scale=1ULL<<15;
	}

	// Yes... I know... You can do this with a lookup table...
	// And it would be much faster... And you should round these...
	for(i=0; i<width; i++) {
		f=*in;
		f=f*scale;
		if(f<0.0) { f=0.0; }
		if(f>65534.999) { f=65534.999; }
		*(data++)=lrint(f);
		in++;
	}
#endif
}

void tiff_convert_float(float *out, const half *in,
                        float scale, uint32_t width) {
#if 1
	ctl::dpx::convert(out, in, 1.0, width);
#else
	uint32_t i;

	if(scale==0.0) {
		scale=1.0;
	}
	for(i=0; i<width; i++) {
		*(out++)=*in*scale;
		in++;
	}
#endif
}

void tiff_write(const char *name, float scale,
                const ctl::dpx::fb<half> &pixels,
                format_t *format) {
	TIFF *t;
	uint16_t bits_per_sample;
	tdata_t scanline_buffer;
	uint32_t y;
	uint8_t channel;
	const half *row;

	TIFFSetErrorHandler(ErrorHandler);
	TIFFSetWarningHandler(WarningHandler);

	bits_per_sample=format->bps;
	if(format->bps<=8) {
		bits_per_sample=8;
	} else if(format->bps<=16) {
		bits_per_sample=16;
	} else if(format->bps!=32) {
		THROW(Iex::ArgExc, "TIFF files can only support files with <=16 bps "
		      "(integer) or 32 bps (float).");
	}

	t=TIFFOpen(name, "w");
	if(t==NULL) {
		// What went wrong
	}

	TIFFSetField(t, TIFFTAG_SAMPLESPERPIXEL, pixels.depth());
	TIFFSetField(t, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
	TIFFSetField(t, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(t, TIFFTAG_IMAGEWIDTH, pixels.width());
	TIFFSetField(t, TIFFTAG_IMAGELENGTH, pixels.height());
	TIFFSetField(t, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(t, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	// Worst case...
	scanline_buffer=alloca(sizeof(float)*pixels.depth()*pixels.width());

	if(bits_per_sample==8) {
		TIFFSetField(t, TIFFTAG_SAMPLEFORMAT, 1);
		for(y=0; y<pixels.height(); y++) {
			row=pixels.ptr()+y*pixels.width()*pixels.depth();
			tiff_convert_uint8((uint8_t *)scanline_buffer, row,
			                   scale, pixels.depth()*pixels.width());
			TIFFWriteScanline(t, scanline_buffer, y, 0);
		}
	} else if(bits_per_sample==16) {
		TIFFSetField(t, TIFFTAG_SAMPLEFORMAT, 1);
		for(y=0; y<pixels.height(); y++) {
			row=pixels.ptr()+y*pixels.width()*pixels.depth();
			tiff_convert_uint16((uint16_t *)scanline_buffer, row,
			                    scale, pixels.depth()*pixels.width());
			TIFFWriteScanline(t, scanline_buffer, y, channel);
		}
	} else if(bits_per_sample==32) {
		TIFFSetField(t, TIFFTAG_SAMPLEFORMAT, 3);
		for(y=0; y<pixels.height(); y++) {
			row=pixels.ptr()+y*pixels.width()*pixels.depth();
			tiff_convert_float((float *)scanline_buffer, row,
			                   scale, pixels.depth()*pixels.width());
			TIFFWriteScanline(t, scanline_buffer, y, channel);
		}
	}

	TIFFClose(t);
}

#else
bool tiff_read(const char *name, float scale, ctl::dpx::fb<half> *pixels,
               format_t *bps) {
	return FALSE;
}
void tiff_write(const char *name, float scale,
                const ctl::dpx::fb<half> &pixels, format_t *format) {
	// thow tiff is unsupported message.
}
#endif
