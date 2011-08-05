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

#include <stdio.h>
#include <exception>
#include <list>
#include <sys/stat.h>
#include <sys/param.h>
#include <errno.h>
#include "transform.hh"
#include <Iex.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if !defined(TRUE)
#define TRUE 1
#endif
#if !defined(FALSE)
#define FALSE 0
#endif

double getfloat(const char *param, const char *name, ...)
{
	double result;
	char *end;

	end=NULL;
	result=strtod(param, &end);

	if (end != NULL && *end != 0)
	{
		va_list ap;
		va_start(ap, name);
		fprintf(stderr, "Unable to parse %s as a floating point number. Found as\n", param);
		vfprintf(stderr, name, ap);
		va_end(ap);
		exit(1);
	}

	return result;
}

ctl_parameter_t get_ctl_parameter(const char ***_argv, int *_argc, int start_argc, const char *type, int count)
{
	ctl_parameter_t new_ctl_param;
	const char **argv = *_argv;
	int argc = *_argc;

	memset(&new_ctl_param, 0, sizeof(new_ctl_param));

	argv++;
	argc--;

	new_ctl_param.name = argv[0];

	argv++;
	argc--;

	new_ctl_param.count = count;
	for (int i = 0; i < new_ctl_param.count; i++)
	{
		new_ctl_param.value[i] = getfloat(argv[0], "value %d of %s parameter %s (absolute parameter %d)", i + 1, type, new_ctl_param.name, start_argc - argc);
		argc--;
		argv++;
	}

	*_argv = argv - 1;
	*_argc = argc + 1;

	return new_ctl_param;
}

struct file_format_t
{
	const char *name;
	format_t output_fmt;
};

file_format_t allowed_formats[] =
{
	{ "exr",    format_t("exr",  16) },
	{ "dpx",    format_t("dpx",   0) },
	{ "dpx8",   format_t("dpx",   8) },
	{ "dpx10",  format_t("dpx",  10) },
	{ "dpx12",  format_t("dpx",  12) },
	{ "dpx16",  format_t("dpx",  16) },
	{ "tif",    format_t("tif",   0) },
	{ "tiff",   format_t("tiff",  0) },
	{ "tiff32", format_t("tiff", 32) },
	{ "tiff16", format_t("tiff", 16) },
	{ "tiff8",  format_t("tiff",  8) },
	{ "tif32",  format_t("tif",  32) },
	{ "tif16",  format_t("tif",  16) },
	{ "tif8",   format_t("tif",   8) },
	{ NULL,     format_t()           }
};

const format_t &find_format(const char *fmt, const char *message = NULL)
{
	const file_format_t *current = allowed_formats;

	while (current->name != NULL)
	{
		if (!strcmp(current->name, fmt))
		{
			return current->output_fmt;
		}
		current++;
	}
	fprintf(stderr, "Unrecognized format '%s'%s", fmt, message ? message : ".");
	exit(1);
}

int verbosity = 1;

int main(int argc, const char **argv)
{
	try
	{
		// list of ctl filenames and associated parameters
		CTLOperations ctl_operations;

		CTLParameters global_ctl_parameters;
		ctl_operation_t new_ctl_operation;

		// list of input images on which to operate
		std::list<const char *> input_image_files;
		char output_path[MAXPATHLEN + 1];

		format_t desired_format;
		format_t actual_format;
		float input_scale = 0.0;
		float output_scale = 0.0;
		bool force_overwrite_output_file = FALSE;
		bool noalpha = FALSE;

		int start_argc = argc;

		argc--;
		argv++;

		new_ctl_operation.filename = NULL;
		while (argc > 0)
		{
			if (!strncmp(argv[0], "-help", 2))
			{
				if (argc > 1)
				{
					usage(argv[1]);
				}
				else
				{
					usage(NULL);
				}
				exit(1);
			}
			else if (!strncmp(argv[0], "-input_scale", 2))
			{
				if (argc == 1)
				{
					fprintf(stderr,
							"The -input_scale option requires an "
							"additional option specifying a scale\nvalue for the "
							"input file. see '-help scale' for additional "
							"details.\n");
					exit(1);
				}
				else
				{
					char *end = NULL;
					input_scale = strtof(argv[1], &end);
					if (end != NULL && *end != 0)
					{
						fprintf(stderr,
								"Unable to parse '%s' as a floating "
								"point number for the '-input_scale'\nargument\n",
								argv[1]);
						exit(1);
					}
					argv++;
					argc--;
				}
			}
			else if (!strncmp(argv[0], "-output_scale", 2))
			{
				if (argc == 1)
				{
					fprintf(stderr,
							"The -output_scale option requires an "
							"additional option specifying a scale\nvalue for the "
							"output file. see '-help scale' for additional "
							"details.\n");
					exit(1);
				}
				else
				{
					char *end = NULL;
					output_scale = strtof(argv[1], &end);
					if (end != NULL && *end != 0)
					{
						fprintf(stderr,
								"Unable to parse '%s' as a floating "
								"point number for the '-output_scale'\nargument\n",
								argv[1]);
						exit(1);
					}
					argv++;
					argc--;
				}
			}
			else if (!strncmp(argv[0], "-ctl", 2))
			{
				if (argc == 1)
				{
					fprintf(stderr,
							"the -ctl option requires an additional "
							"option specifying a file containing a\nctl script.\n"
							"see '-help ctl' for more details.\n");
					exit(1);
				}

				if (new_ctl_operation.filename != NULL)
				{
					ctl_operations.push_back(new_ctl_operation);
				}
				new_ctl_operation.local.clear();
				new_ctl_operation.filename = argv[1];

				argv++;
				argc--;
			}
			else if (!strncmp(argv[0], "-format", 5))
			{
				if (argc == 1)
				{
					fprintf(stderr,
							"the -format option requires an addition "
							"argument specifying the destination file\nformat. "
							"this may be one of the following: 'dpx10', 'dpx16', "
							"'aces', 'tiff8',\n'tiff16', or 'exr'.\nSee '-help "
							"format' for more details.\n");
					exit(1);
				}
				desired_format = find_format(argv[1]," for parameter '-format'.\nSee '-help format' for more details.");
				argv++;
				argc--;
			}
			else if (!strcmp(argv[0], "-param1") || !strcmp(argv[0], "-p1"))
			{
				if (argc < 3)
				{
					fprintf(stderr,
							"the -param1 option requires two additional "
							"arguments specifying the\nparameter name and value."
							"\nSee '-help param' for more details.\n");
					exit(1);
				}
				if (new_ctl_operation.filename == NULL)
				{
					THROW(Iex::ArgExc, "the -param1 argument must occur *after* a -ctl option.");
				}
				new_ctl_operation.local.push_back(get_ctl_parameter(&argv, &argc, start_argc, "local", 1));
			}
			else if (!strcmp(argv[0], "-param2") || !strcmp(argv[0], "-p2"))
			{
				if (argc < 4)
				{
					fprintf(stderr,
							"the -param2 option requires three additional "
							"arguments specifying the\nparameter name and value."
							"\nSee '-help param' for more details.\n");
					exit(1);

				}
				if (new_ctl_operation.filename == NULL)
				{
					THROW(Iex::ArgExc, "the -param2 argument must occur *after* a -ctl option.");
				}
				new_ctl_operation.local.push_back(get_ctl_parameter(&argv, &argc, start_argc, "local", 3));
			}
			else if (!strcmp(argv[0], "-param3") || !strcmp(argv[0], "-p3"))
			{
				if (argc < 5)
				{
					fprintf(stderr,
							"the -param3 option requires four additional "
							"arguments specifying the\nparameter name and value."
							"\nSee '-help param' for more details.\n");
					exit(1);
				}
				if (new_ctl_operation.filename == NULL)
				{
					THROW(Iex::ArgExc, "the -param3 argument must occur *after* a -ctl option.");
				}
				new_ctl_operation.local.push_back(get_ctl_parameter(&argv, &argc, start_argc, "local", 3));
			}
			else if (!strcmp(argv[0], "-global_param1") || !strcmp(argv[0], "-gp1"))
			{
				if (argc < 3)
				{
					fprintf(stderr, "the -global_param1 option requires two "
							"additional arguments specifying the\nparameter "
							"name and value.\nSee '-help param' for more "
							"details.\n");
					exit(1);
				}
				global_ctl_parameters.push_back(get_ctl_parameter(&argv, &argc, start_argc, "global", 1));
			}
			else if (!strcmp(argv[0], "-global_param2") || !strcmp(argv[0], "-gp2"))
			{
				if (argc < 4)
				{
					fprintf(stderr, "the -global_param2 option requires three "
							"additional arguments specifying the\nparameter "
							"name and value.\nSee '-help param' for more "
							"details.\n");
					exit(1);
				}
				global_ctl_parameters.push_back(get_ctl_parameter(&argv, &argc, start_argc, "global", 2));
			}
			else if (!strcmp(argv[0], "-global_param3") || !strcmp(argv[0], "-gp3"))
			{
				if (argc < 5)
				{
					fprintf(stderr, "the -global_param3 option requires four "
							"additional arguments specifying the\nparameter "
							"name and value.\nSee '-help param' for more "
							"details.\n");
					exit(1);

				}
				global_ctl_parameters.push_back(get_ctl_parameter(&argv, &argc, start_argc, "global", 2));
			}
			else if (!strncmp(argv[0], "-verbose", 2))
			{
				verbosity++;
			}
			else if (!strncmp(argv[0], "-quiet", 2))
			{
				verbosity--;
			}
			else if (!strncmp(argv[0], "-force", 5))
			{
				force_overwrite_output_file = TRUE;
			}
			else if (!strncmp(argv[0], "-noalpha", 2))
			{
				noalpha = TRUE;
			}
			else if (!strncmp(argv[0], "-", 1))
			{
				fprintf(stderr,
						"unrecognized option %s. see -help for a list "
						"of available options.\n", argv[0]);
				exit(1);
			}
			else
			{
				input_image_files.push_back(argv[0]);
			}
			argv++;
			argc--;

		}  // end while

		if (new_ctl_operation.filename != NULL)
		{
			ctl_operations.push_back(new_ctl_operation);
		}

		if (input_image_files.size() < 2)
		{
			fprintf(stderr,
					"one or more source filenames and a destination "
					"file or directory must be\nprovided. if more than one "
					"source filenames is provided then the last argument\nmust "
					"be a directory. see -help for more details.\n");
			exit(1);
		}

		char *output_slash = NULL;
		const char *outputFile = input_image_files.back();
		input_image_files.pop_back();

		struct stat file_status;
		if (stat(outputFile, &file_status) >= 0)
		{
			if (S_ISDIR(file_status.st_mode))
			{
				memset(output_path, 0, sizeof(output_path));
				strncpy(output_path, outputFile, MAXPATHLEN);
				outputFile = output_path;
				output_slash = output_path + strlen(output_path);
				if (*output_slash != '/')
				{
					*(output_slash++) = '/';
					*output_slash = 0;
				}
			}
			else if (S_ISREG(file_status.st_mode))
			{
				if (input_image_files.size() > 1)
				{
					fprintf(stderr,
							"When providing more than one source "
							"image the destination must be a\ndirectory.\n");
					exit(1);
				}
				else
				{
					if (!force_overwrite_output_file)
					{
						fprintf(stderr,
								"The destination file %s already exists.\n"
								"Cravenly refusing to overwrite unless you supply "
								"the -force option.\n", outputFile);
						exit(1);
					}
					else
					{
						// File exists, but we treat it as if it doesn't (see
						// if(output_slash==NULL) {...} down below...
						output_slash = NULL;
					}
				}
			}
			else
			{
				fprintf(stderr,
						"Specified destination is something other than "
						"a file or directory. That's\nprobably a bad idea.\n");
				exit(1);
			}
		}
		else
		{
			if (errno != ENOENT)
			{
				fprintf(stderr, "Unable to get information about %s (%s).\n", outputFile, strerror(errno));
				exit(1);
			}
			if (input_image_files.size() != 1)
			{
				fprintf(stderr,
						"When specifying more than one source file "
						"you must specify the destination as\na directory "
						"that already exists.\nUnable to stat '%s' (%s)\n",
						outputFile, strerror(errno));
				exit(1);
			}
		}

		if (output_slash == NULL)
		{
			// This is the case when our outputFile is a single file. We do a bunch
			// of sanity checking between the extension of the specified file
			// (if any) and the -format option (if any).
			char *dot = (char *) strrchr(outputFile, '.');
			if (dot == NULL && desired_format.ext == NULL)
			{
				fprintf(stderr,
						"You have not explicitly provided an output "
						"format, and the output file name\ndoes not not contain "
						"an extension. Please add an extension to the output "
						"file\nor use the -format option to specify the desired "
						"output format.\n");
				exit(1);
			}
			else if (dot != NULL)
			{
				if (desired_format.ext == NULL)
				{
					actual_format = find_format(dot + 1,
							                    " specified implicitly (by "
									            "the extension) for\noutput file "
									            "format. Please fix this or use\n"
									            "the -format option to specify "
									            "the desired output format.\n");
				}
				else
				{
					if (strcmp(desired_format.ext, dot + 1) && !force_overwrite_output_file)
					{
						fprintf(stderr,
								"You have specified a destination file "
								"type with the -format option, but the\noutput "
								"file extension does not match the format "
								"specified by the -format option.\nCravenly "
								"refusing to do this unless you specify the "
								"-force option (which\nwill make the -format "
								"option take priority).\n");
						exit(1);
					}
					actual_format = desired_format;
				}
			}
		}

		if (verbosity > 1)
		{
			fprintf(stderr, "global ctl parameters:\n");

			CTLParameters temp_ctl_parameters;
			temp_ctl_parameters = global_ctl_parameters;

			while (temp_ctl_parameters.size() > 0)
			{
				ctl_parameter_t new_ctl_parameter = temp_ctl_parameters.front();
				temp_ctl_parameters.pop_front();
				fprintf(stderr, "%17s:", new_ctl_parameter.name);
				for (int i = 0; i < new_ctl_parameter.count; i++)
				{
					fprintf(stderr, " %f", new_ctl_parameter.value[i]);
				}
				fprintf(stderr, "\n");
			}
			fprintf(stderr, "\n");
		}

		while (input_image_files.size() > 0)
		{
			const char *inputFile = input_image_files.front();

			if (output_slash != NULL)
			{
				const char *input_slash = strrchr(inputFile, '/');
				if (input_slash == NULL)
				{
					input_slash = (char *) inputFile;
				}
				else
				{
					input_slash++;
				}
				strcpy(output_slash, input_slash);
				char *dot = (char *) strrchr(outputFile, '.');
				if (dot != NULL)
				{
					dot++;
					if (desired_format.ext != NULL)
					{
						strcpy(dot, desired_format.ext);
						actual_format = desired_format;
					}
					else
					{
						actual_format = find_format(dot, " (determined from destination file extension).");
					}
				}
			}

			if (force_overwrite_output_file)
			{
				if (unlink(outputFile) < 0)
				{
					if (errno != ENOENT)
					{
						fprintf(stderr, "Unable to remove existing file named "
								"'%s' (%s).\n", outputFile, strerror(errno));
						exit(1);
					}
				}
			}
			if (access(outputFile, F_OK) >= 0)
			{
				fprintf(stderr, "Cravenly refusing to overwrite the file '%s'.\n", outputFile);
				exit(1);
			}
			actual_format.squish = noalpha;
			transform(inputFile, outputFile, input_scale, output_scale, &actual_format, ctl_operations, global_ctl_parameters);
			input_image_files.pop_front();
		}

		return 0;

	} catch (std::exception &e)
	{
		fprintf(stderr, "exception thrown (oops...): %s\n", e.what());
		return 1;
	}
}
