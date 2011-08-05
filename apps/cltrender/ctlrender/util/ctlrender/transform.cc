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

#include "transform.hh"
#include "dpx_file.hh"
#include "tiff_file.hh"
#include "exr_file.hh"
#include <dpx.hh>
#include <CtlRcPtr.h>
#include <CtlFunctionCall.h>
#include <CtlSimdInterpreter.h>
#include <CtlStdType.h>
#include <exception>
#include <Iex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

class CTLResult;
typedef Ctl::RcPtr<CTLResult> CTLResultPtr;

// class which holds the data resulting from a CTL transform
class CTLResult: public Ctl::RcObject
{
public:
	CTLResult();
	virtual ~CTLResult();

	Ctl::TypeStoragePtr data;
	bool external;
	std::string alt_name;
};

CTLResult::CTLResult() :
		Ctl::RcObject()
{
	external = FALSE;
}

CTLResult::~CTLResult()
{
}

typedef std::list<CTLResultPtr> CTLResults;


// This function is used to add to the result list parameters that
// are specified on the command line. A parameter that has been returned
// by a CTL function takes precedence
void add_parameter_value_to_ctl_results(CTLResults *ctl_results, const ctl_parameter_t &ctl_parameter)
{
	CTLResults::iterator results_iter;
	Ctl::ArrayTypePtr array;
	Ctl::DataTypePtr type;
	CTLResultPtr ctl_result;

	// lookup a named data element that matches the parameter name.
	for (results_iter = ctl_results->begin(); results_iter != ctl_results->end(); results_iter++)
	{
		if ((*results_iter)->data->name() == ctl_parameter.name)
		{
			break;
		}
	}

	if (results_iter != ctl_results->end())
	{
		// result data found
		ctl_result = *results_iter;

		// if result data was set by a CTL function (i.e. external == false), we preserve its value
		if (!ctl_result->external)
		{
			// Set by a CTL function. No way are we over writing this...
			return;
		}
		// It's a command line argument, so we (re)set it (this happens
		// if the user has specified the same parameter more than once
		// (either due to user error or by overriding a global value with
		// a local one).
	}
	else
	{
		// ctl result data not found, so we create a new ctl result and add it to the list (the ctl data value is added below)
		ctl_result = CTLResultPtr(new CTLResult);
		ctl_result->external = TRUE;
		ctl_results->push_back(ctl_result);
	}

	if (ctl_parameter.count == 1)
	{
		ctl_result->data = new Ctl::DataArg(ctl_parameter.name, new Ctl::StdFloatType(), 1);
		ctl_result->data->set(&(ctl_parameter.value[0]));
	}
	else
	{
		type = new Ctl::StdArrayType(new Ctl::StdFloatType(), ctl_parameter.count);
		ctl_result->data = new Ctl::DataArg(ctl_parameter.name, type, 1);
		for (uint8_t i = 0; i < ctl_parameter.count; i++)
		{
			ctl_result->data->set(&(ctl_parameter.value[i]), 0, 0, 1, "%d", i);
		}
	}
	ctl_results->push_back(ctl_result);
}

void set_ctl_function_argument_from_ctl_results(Ctl::FunctionArgPtr *arg, const CTLResults &ctl_results, size_t offset, size_t count)
{
	CTLResults::const_iterator results_iter;
	Ctl::TypeStoragePtr src;
	Ctl::FunctionArgPtr dst;

	dst = *arg;

	for (results_iter = ctl_results.begin(); results_iter != ctl_results.end(); results_iter++)
	{
		if ((*results_iter)->data->name() == dst->name() || (*results_iter)->alt_name == dst->name())
		{
			break;
		}
	}

	if (results_iter == ctl_results.end())
	{
		if (dst->hasDefaultValue())
		{
			dst->setDefaultValue();
		}
		else
		{
			THROW(Iex::ArgExc, "CTL parameter '" << dst->name() << "' not specified on the command line and does not have a default value.");
			throw(std::exception());
		}
		return;
	}

	src = (*results_iter)->data;
	if (!dst->isVarying())
	{
		if (offset == 0)
		{
			dst->copy(src, 0, 0, 1);
		}
		return;
	}
	else
	{
		if (!src->isVarying())
		{
			for (size_t i = 0; i < dst->elements(); i++)
			{
				dst->copy(src, 0, i, 1);
			}
		}
		else
		{
			dst->copy(src, offset, 0, count);
		}
	}
}

void set_ctl_results_from_ctl_function_argument(CTLResults *ctl_results, const Ctl::FunctionArgPtr &arg, size_t offset, size_t count, size_t total)
{
	CTLResults::iterator results_iter;
	CTLResultPtr ctl_result;

	for (results_iter = ctl_results->begin(); results_iter != ctl_results->end(); results_iter++)
	{
		if ((*results_iter)->data->name() == arg->name())
		{
			break;
		}
	}

//	fprintf(stderr, "copying %d@%d (total %d) of %s\n", count, offset, total, arg->name().c_str());
	if (!arg->isVarying())
	{
		// For constant return arguments we only do this the first time
		// through
		if (offset != 0)
		{
			return;
		}
		total = 1;
		count = 1;
	}

	// Since the ctl_results list gets rebuilt after the end of ctl_run_transform
	// we don't have to worry about having this function getting called
	// twice with the FunctionArgPtr having different types (for a given
	// output argument name).
	if (results_iter != ctl_results->end())
	{
		ctl_result = *results_iter;

		std::string inputName;
		if (arg->name() == "rOut")
		{
			inputName = "rIn";
		}
		else if (arg->name() == "gOut")
		{
			inputName = "gIn";
		}
		else if (arg->name() == "bOut")
		{
			inputName = "bIn";
		}
		else if (arg->name() == "aOut")
		{
			inputName = "aIn";
		}

		if (!inputName.empty())
		{
			for (results_iter = ctl_results->begin(); results_iter != ctl_results->end(); results_iter++)
			{
				if ((*results_iter)->data->name() == inputName)
				{
					break;
				}
			}

			if (results_iter != ctl_results->end())
			{
				CTLResultPtr ctl_result_output_to_input;
				ctl_result_output_to_input = *results_iter;
				ctl_result_output_to_input->data->copy(arg, 0, offset, count);
			}

		}
	}
	else
	{
		ctl_result = CTLResultPtr(new CTLResult());
		ctl_result->data = new Ctl::DataArg(arg->name(), arg->type(), total);
		ctl_results->push_back(ctl_result);

		std::string inputName;
		if (arg->name() == "rOut")
		{
			inputName = "rIn";
		}
		else if (arg->name() == "gOut")
		{
			inputName = "gIn";
		}
		else if (arg->name() == "bOut")
		{
			inputName = "bIn";
		}
		else if (arg->name() == "aOut")
		{
			inputName = "aIn";
		}

		if (!inputName.empty())
		{
			CTLResultPtr ctl_result_input;

			ctl_result_input = CTLResultPtr(new CTLResult());
			ctl_result_input->data = new Ctl::DataArg(inputName, arg->type(), total);
			ctl_results->push_back(ctl_result_input);
			ctl_result_input->data->copy(arg, 0, offset, count);
		}
	}

	ctl_result->data->copy(arg, 0, offset, count);
}

void run_ctl_transform(const ctl_operation_t &ctl_operation, CTLResults *ctl_results, size_t count)
{
	Ctl::SimdInterpreter interpreter;
	Ctl::FunctionCallPtr fn;
	Ctl::FunctionArgPtr arg;
	CTLResults::iterator results_iter;
	char *name = NULL;
	char *module;
	char *slash;
	char *dot;
	CTLResults new_ctl_results;

	try
	{

		name = (char *) alloca(strlen(ctl_operation.filename)+1);
		memset(name, 0, strlen(ctl_operation.filename) + 1);
		strcpy(name, ctl_operation.filename);

		// XXX probably not windows friendly
		slash = strrchr(name, '/');
		if (slash == NULL)
		{
			module = name;
		}
		else
		{
			module = slash + 1;
		}

		dot = strrchr(module, '.');
		if (dot != NULL)
		{
			*dot = 0;
		}

		try
		{
			interpreter.loadFile(ctl_operation.filename);
			try
			{
				// It's probably broken that you can't get a list of the function
				// calls from a file. It's an article of faith that the primary
				// function of a ctl script is named the same as the base ctl script
				// name (without '.ctl' extension). We deal with this by looking
				// for a 'main' function, and failing that, a function named whatever
				// the ctl file is named. This is probably not ideal. The 'main'
				// function convention is used by 'toxik'
				fn = interpreter.newFunctionCall(std::string("main"));
			}
			catch (const Iex::ArgExc &e)
			{
				// XXX CTL library needs to be changed so that we have a better
				// XXX 'function not exists' exception.
			}

			if (fn.refcount() == 0)
			{
				fn = interpreter.newFunctionCall(std::string(module));
			}
		}
		catch (...)
		{

		}

		if (fn->returnValue()->type().cast<Ctl::VoidType>().refcount() == 0)
		{
			THROW(Iex::ArgExc, "CTL main (or <module_name>) function must return a 'void'");
		}

		if (verbosity > 1)
		{
			fprintf(stderr, "   ctl script file: %s\n", ctl_operation.filename);
			fprintf(stderr, "     function name: %s\n", fn->name().c_str());

			for (size_t i = 0; i < fn->numInputArgs(); i++)
			{
				arg = fn->inputArg(i);
				if (i == 0)
				{
					fprintf(stderr, "   input arguments:\n");
				}
				fprintf(stderr, "%18s: %s", arg->name().c_str(), arg->type()->asString().c_str());

				if (arg->isVarying())
				{
					fprintf(stderr, " (varying)");
				}

				if (arg->hasDefaultValue())
				{
					fprintf(stderr, " (defaulted)");
				}

				fprintf(stderr, "\n");
			}

			for (size_t i = 0; i < fn->numOutputArgs(); i++)
			{
				arg = fn->outputArg(i);
				if (i == 0)
				{
					fprintf(stderr, "  output arguments:\n");
				}

				fprintf(stderr, "%18s: %s", arg->name().c_str(), arg->type()->asString().c_str());

				if (arg->isVarying())
				{
					fprintf(stderr, " (varying)");
				}

				if (arg->hasDefaultValue())
				{
					fprintf(stderr, " (defaulted)");
				}

				fprintf(stderr, "\n");
			}
			fprintf(stderr, "\n");
		}

		//	fprintf(stderr, "%d samples to go.\n", count);

		size_t offset = 0;
		while (offset < count)
		{
			size_t pass = interpreter.maxSamples();
			if (pass > (count - offset))
			{
				pass = (count - offset);
			}
//		fprintf(stderr, "at offset %d doing %d samples\n", offset, pass);
			for (size_t i = 0; i < fn->numInputArgs(); i++)
			{
				arg = fn->inputArg(i);
				set_ctl_function_argument_from_ctl_results(&arg, *ctl_results, offset, pass);
			}

			fn->callFunction(pass);

			for (size_t i = 0; i < fn->numOutputArgs(); i++)
			{
				//printf("setting results from function argument\n");
				set_ctl_results_from_ctl_function_argument(&new_ctl_results, fn->outputArg(i), offset, pass, count);
			}

			offset = offset + pass;
		}
		*ctl_results = new_ctl_results;
	}
	catch (...)
	{
//		if(name!=NULL) {
//			free(name);
//		}
		throw;
	}
}


// Creates a new ctl result object from the image buffer (fb parameter) passed in
// Copies a new CTL result (block of data) from the framebuffer fb.
CTLResultPtr mkresult(const char *name, const char *alt_name, const ctl::dpx::fb<half> &fb, size_t offset)
{
	CTLResultPtr new_result = CTLResultPtr(new CTLResult());

	// DataArg represents a raw block of data
	// XXX - seems like last argument to Ctl::DataArg should be (fb.pixels() + offset) because the argument represents the data size (i.e. number of bytes)
	new_result->data = Ctl::DataArgPtr(new Ctl::DataArg(name, Ctl::DataTypePtr(new Ctl::StdHalfType()), fb.pixels()));

	if (alt_name != NULL)
	{
		new_result->alt_name = alt_name;
	}

	new_result->data->set(fb.ptr() + offset, sizeof(half) * fb.depth(), 0, fb.pixels());

	return new_result;
}

void mkimage(ctl::dpx::fb<half> *image_buffer, const CTLResults &ctl_results, format_t *image_format)
{
	enum have_channel_e
	{
		// These need to be in the order that you want them in the file...
		have_x = 0,
		have_y = 1,
		have_z = 2,
		have_r = 3,
		have_g = 4,
		have_b = 5,
		have_a = 6,
		have_xout = 8,
		have_yout = 9,
		have_zout = 10,
		have_rout = 11,
		have_gout = 12,
		have_bout = 13,
		have_aout = 14,
		mask_x = 1 << have_x,
		mask_y = 1 << have_y,
		mask_z = 1 << have_z,
		mask_r = 1 << have_r,
		mask_g = 1 << have_g,
		mask_b = 1 << have_b,
		mask_a = 1 << have_a,
		mask_xout = 1 << have_xout,
		mask_yout = 1 << have_yout,
		mask_zout = 1 << have_zout,
		mask_rout = 1 << have_rout,
		mask_gout = 1 << have_gout,
		mask_bout = 1 << have_bout,
		mask_aout = 1 << have_aout,
		have_none = 33,
	};

	CTLResults::const_iterator results_iter;
	CTLResultPtr channels[16];
	int channels_mask;
	have_channel_e channel;
	const char *channel_name;
	uint8_t on_channel;
	uint8_t channel_count;
	uint8_t c;
	CTLResultPtr ctl_result;

	// These need to be in the order for preferred output formats...
	// The DPX colorimetric is in the top 8 bits
	int tests[] =
	{
			(51 << 24) | (mask_rout | mask_gout | mask_bout | mask_aout), (50 << 24)
			| (mask_rout | mask_gout | mask_bout), (158 << 24)
			| (mask_xout | mask_yout | mask_zout | mask_aout), (157 << 24)
			| (mask_xout | mask_yout | mask_zout), (159 << 24)
			| (mask_yout | mask_aout), (6 << 24) | (mask_yout), (162 << 24)
			| (mask_gout | mask_aout), (2 << 24) | (mask_gout), (161 << 24)
			| (mask_bout | mask_aout), (3 << 24) | (mask_bout), (160 << 24)
			| (mask_rout | mask_aout), (1 << 24) | (mask_rout), (4 << 24)
			| (mask_aout),

	        (51 << 24) | (mask_r | mask_g | mask_b | mask_a), (50 << 24)
			| (mask_r | mask_g | mask_b), (158 << 24)
			| (mask_x | mask_y | mask_z | mask_a), (159 << 24)
			| (mask_y | mask_a), (6 << 24) | (mask_y), (162 << 24)
			| (mask_g | mask_a), (2 << 24) | (mask_g), (161 << 24)
			| (mask_b | mask_a), (3 << 24) | (mask_b), (160 << 24)
			| (mask_r | mask_a), (1 << 24) | (mask_r), (4 << 24) | (mask_a),

	        0
	};

	channels_mask = 0;
	for (results_iter = ctl_results.begin(); results_iter != ctl_results.end(); results_iter++)
	{
		ctl_result = *results_iter;
		if (!ctl_result->data->isVarying() && ctl_result->data->elements() != image_buffer->pixels())
		{
			continue;
		}
		channel = have_none;
		channel_name = ctl_result->data->name().c_str();
		if (0)
		{
		}
//		else if(!strcasecmp("X", channel_name)) { channel=have_x; } 
//		else if(!strcasecmp("Y", channel_name)) { channel=have_y; } 
//		else if(!strcasecmp("Z", channel_name)) { channel=have_z; } 
//		else if(!strcasecmp("A", channel_name)) { channel=have_a; } 
//		else if(!strcasecmp("R", channel_name)) { channel=have_r; } 
//		else if(!strcasecmp("G", channel_name)) { channel=have_g; } 
//		else if(!strcasecmp("B", channel_name)) { channel=have_b; } 
//		else if(!strcasecmp("outX", channel_name)) { channel=have_xout; } 
//		else if(!strcasecmp("outY", channel_name)) { channel=have_yout; } 
//		else if(!strcasecmp("outZ", channel_name)) { channel=have_zout; } 
		else if (!strcasecmp("aOut", channel_name))
		{
			channel = have_aout;
		}
		else if (!strcasecmp("rOut", channel_name))
		{
			channel = have_rout;
		}
		else if (!strcasecmp("gOut", channel_name))
		{
			channel = have_gout;
		}
		else if (!strcasecmp("bOut", channel_name))
		{
			channel = have_bout;
		}

		if (channel == have_none)
		{
			continue;
		}

		if (ctl_result->data->type().cast<Ctl::HalfType>().refcount() == 0
				&& ctl_result->data->type().cast<Ctl::FloatType>().refcount() == 0)
		{
			THROW(Iex::ArgExc, "CTL script not providing half or float as the output data type.");
		}
		channels[channel] = ctl_result;
		channels_mask = channels_mask | (1 << channel);
	}

	for (c = 0; tests[c] != 0; c++)
	{
		if ((channels_mask & tests[c] & 0x00ffffff) == (tests[c] & 0x00ffffff))
		{
			channels_mask = tests[c];
			break;
		}
	}

	if (tests[c] == 0)
	{
		THROW(Iex::ArgExc, "Unable to determine what channels from the CTL script output should be saved.");
	}

	channel_count = 0;
	for (c = 0; c < 24; c++)
	{
		if (channels_mask & (1 << c))
		{
			channel_count++;
		}
	}

	if (image_buffer->depth() != channel_count)
	{
		image_buffer->init(image_buffer->width(), image_buffer->height(), channel_count);
	}

	on_channel = 0;
	for (c = 0; c < 24; c++)
	{
		if (channels_mask & (1 << c))
		{
			channels[c]->data->get(image_buffer->ptr() + on_channel, sizeof(half) * channel_count, 0, image_buffer->pixels());
			on_channel++;
		}
	}

	if (image_format->descriptor == 0)
	{
		image_format->descriptor = (channels_mask & 0xff000000) >> 24;
	}
}

// Currently we have no thread support. This would be nice but we will
// deal with it on a per-input-file basis. The format is passed in as 
// a pointer since there are fields in it that may be filled out by the
// reader / writer and those will probably want to migrate back to the
// calling function.
void transform(const char *inputFile, const char *outputFile,
		       float input_scale, float output_scale,
		       format_t *image_format,
		       const CTLOperations &ctl_operations,
		       const CTLParameters &global_parameters)
{
	CTLOperations::const_iterator operations_iter;
	ctl_operation_t ctl_operation;
	CTLParameters::const_iterator parameters_iter;
	uint8_t i;
	std::string error;
	ctl::dpx::fb<half> image_buffer;

	if (verbosity > 1)
	{
		fprintf(stdout, "       source file: %s\n", inputFile);
		fprintf(stdout, "  destination file: %s\n", outputFile);
		fprintf(stderr, "destination format: %s\n", image_format->ext);
		fprintf(stderr, "       input scale: ");
		if (input_scale == 0.0)
		{
			fprintf(stderr, "default\n");
		}
		else
		{
			fprintf(stderr, "%f\n", input_scale);
		}
		fprintf(stderr, "      output scale: ");
		if (output_scale == 0.0)
		{
			fprintf(stderr, "default\n");
		}
		else
		{
			fprintf(stderr, "%f\n", output_scale);
		}
		if (verbosity > 2)
		{
			i = 0;
			for (parameters_iter = global_parameters.begin(); parameters_iter != global_parameters.end(); parameters_iter++)
			{
				if (i == 0)
				{
					fprintf(stderr, " global parameters:\n");
					i++;
				}
				fprintf(stderr, "%18s:", parameters_iter->name);
				for (i = 0; i < parameters_iter->count; i++)
				{
					fprintf(stderr, " %f", parameters_iter->value[i]);
				}
				fprintf(stderr, "\n");
			}
			for (operations_iter = ctl_operations.begin(); operations_iter != ctl_operations.end(); operations_iter++)
			{
				ctl_operation = *operations_iter;
				fprintf(stderr, "   ctl script file: %s\n", ctl_operation.filename);
				if (verbosity > 3)
				{
					i = 0;
					for (parameters_iter = ctl_operation.local.begin(); parameters_iter != ctl_operation.local.end(); parameters_iter++)
					{
						if (i == 0)
						{
							fprintf(stderr, "  local parameters:\n");
							i++;
						}
						fprintf(stderr, "%18s:", parameters_iter->name);
						for (i = 0; i < parameters_iter->count; i++)
						{
							fprintf(stderr, " %f", parameters_iter->value[i]);
						}
						fprintf(stderr, "\n");
					}
				}
			}
		}
		fprintf(stderr, "\n");
	}

	if (!dpx_read(inputFile, input_scale, &image_buffer, image_format) &&
		!exr_read(inputFile, input_scale, &image_buffer, image_format) &&
		!tiff_read(inputFile, input_scale, &image_buffer, image_format))
	{
		fprintf(stderr, "unable to read file %s (unknown format).\n", inputFile);
		exit(1);
	}

	if (output_scale != 0.0)
	{
		output_scale = output_scale / 1.0;
	}
	if (image_format->bps == 0)
	{
		image_format->bps = image_format->src_bps;
	}

	CTLResults ctl_results;

	if (image_buffer.depth() > 0)
	{
		ctl_results.push_back(mkresult("rIn", "c00In", image_buffer, 0));
	}
	if (image_buffer.depth() > 1)
	{
		ctl_results.push_back(mkresult("gIn", "c01In", image_buffer, 1));
	}
	if (image_buffer.depth() > 2)
	{
		ctl_results.push_back(mkresult("bIn", "c02In", image_buffer, 2));
	}
	if (image_buffer.depth() > 3)
	{
		ctl_results.push_back(mkresult("aIn", "c03In", image_buffer, 3));
	}

	char name[16];

	for (i = 4; i < image_buffer.depth(); i++)
	{
		memset(name, 0, sizeof(name));
		snprintf(name, sizeof(name) - 1, "c%02dIn", i);
		ctl_results.push_back(mkresult(name, NULL, image_buffer, i));
	}

	for (operations_iter = ctl_operations.begin(); operations_iter != ctl_operations.end(); operations_iter++)
	{
		ctl_operation = *operations_iter;
		for (parameters_iter = global_parameters.begin(); parameters_iter != global_parameters.end(); parameters_iter++)
		{
			add_parameter_value_to_ctl_results(&ctl_results, *parameters_iter);
		}
		for (parameters_iter = ctl_operation.local.begin(); parameters_iter != ctl_operation.local.end(); parameters_iter++)
		{
			add_parameter_value_to_ctl_results(&ctl_results, *parameters_iter);
		}

		// Output is used to pass output parameters from script to the next.
		run_ctl_transform(*operations_iter, &ctl_results, image_buffer.pixels());
	}

	mkimage(&image_buffer, ctl_results, image_format);

	if (output_scale != 0.0)
	{
		output_scale = output_scale / 1.0;
	}
	if (image_format->squish)
	{
		image_buffer.swizzle(0, TRUE);
	}

	if (!strncmp(image_format->ext, "exr", 3))
	{
		exr_write(outputFile, output_scale, image_buffer, image_format);
	}
	else if (!strncmp(image_format->ext, "adx", 3))
	{
		dpx_write(outputFile, output_scale, image_buffer, image_format);
	}
	else if (!strncmp(image_format->ext, "dpx", 3))
	{
		dpx_write(outputFile, output_scale, image_buffer, image_format);
	}
	else if (!strncmp(image_format->ext, "tiff", 3))
	{
		tiff_write(outputFile, output_scale, image_buffer, image_format);
	}
	else
	{
		fprintf(stderr, "unable to write a %s file (unknown format).\n", image_format->ext);
		exit(1);
	}
}
