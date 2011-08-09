///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Academy of Motion Picture Arts and Sciences
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

//-------------------------------------------------------------------------
//
//	A program that generates header files for table-based,
//	fast 16-bit floating-point functions that compute logarithm,
//	exponential and power functions.
//
//-------------------------------------------------------------------------

#include <half.h>
#include <ImathFun.h>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <cstdlib>

using namespace Imath;
using namespace std;


void
genHeaders (const char tablesFileName[], const char functionsFileName[])
{
    ofstream tables (tablesFileName);
    ofstream functions (functionsFileName);

    if (!tables || !functions)
    {
	cerr << "Could not open output files" << endl;
	exit (1);
    }

    tables.precision (9);
    tables.setf (ios_base::hex, ios_base::basefield);

    tables << "//\n"
	      "// This is an automatically generated file.\n"
	      "// Do not edit.\n"
	      "//\n\n";

    functions.precision (9);
    functions.setf (ios_base::hex, ios_base::basefield);

    functions << "//\n"
		 "// This is an automatically generated file.\n"
		 "// Do not edit.\n"
		 "//\n\n";

    //
    // Generate a base-10 logarithm table for every possible
    // half value, h: if h is finite and positive, then store
    // float(log10(h)); otherwise store a NaN or an infinity.
    //

    const unsigned int iMax = (1 << 16);

    tables.setf (ios_base::dec, ios_base::basefield);

    tables << "const unsigned int log10Table[" << iMax << "] =\n"
	      "{\n    ";

    tables.setf (ios_base::hex, ios_base::basefield);

    for (unsigned int i = 0; i < iMax; ++i)
    {
	half h;
	h.setBits (i);

	half::uif u;

	if (h.isNan() || h < 0)
	{
	    u.i = 0x7f800001;	// quiet NaN
	}
	else if (h == 0)
	{
	    u.i = 0xff800000;	// negative infinity
	}
	else if (h.isInfinity())
	{
	    assert (h > 0);
	    u.i = 0x7f800000;	// positive infinity
	}
	else
	{
	    assert (h.isFinite() && h > 0);
	    double l = log10 (double (h));
	    u.f = float (l);
	}

	tables << "0x" << setfill ('0') << setw (8) << u.i << ", ";

	if (i % 4 == 3)
	{
	    tables << "\n";

	    if (i < iMax - 1)
		tables << "    ";
	}
    }

    tables << "};\n\n";

    //
    // Generate a natural logarithm table for every possible
    // half value, h: if h is finite and positive, then store
    // float(log(h)); otherwise store a NaN or an infinity.
    //

    tables.setf (ios_base::dec, ios_base::basefield);

    tables << "const unsigned int logTable[" << iMax << "] =\n"
	      "{\n    ";

    tables.setf (ios_base::hex, ios_base::basefield);

    double lMin = 0;
    double lMax = 0;
    double lPrevious = -numeric_limits<double>::max();
    double lStepMin  =  numeric_limits<double>::max();

    for (unsigned int i = 0; i < iMax; ++i)
    {
	half h;
	h.setBits (i);

	half::uif u;

	if (h.isNan() || h < 0)
	{
	    u.i = 0x7f800001;	// quiet NaN
	}
	else if (h == 0)
	{
	    u.i = 0xff800000;	// negative infinity
	}
	else if (h.isInfinity())
	{
	    assert (h > 0);
	    u.i = 0x7f800000;	// positive infinity
	}
	else
	{
	    assert (h.isFinite() && h > 0);
	    double l = log (double (h));
	    u.f = float (l);

	    lMin = min (l, lMin);
	    lMax = max (l, lMax);

	    double lStep = l - lPrevious;
	    lStepMin = min (lStep, lStepMin);
	    lPrevious = l;
	}

	tables << "0x" << setfill ('0') << setw (8) << u.i << ", ";

	if (i % 4 == 3)
	{
	    tables << "\n";

	    if (i < iMax - 1)
		tables << "    ";
	}
    }

    tables << "};\n\n";

    //
    // Tabulate half(exp(x)) for log(HALF_MIN) < x < log(HALF_MAX).
    // The step size for x is roughly half the smallest difference
    // between adjacent table entries in the logarithm table.
    // This step size allows us to approximate half(exp(x)) to within
    // 0.75 units in the last place with a single table lookup.
    //

    int expTableSize = Imath::ceil ((lMax - lMin) / lStepMin) * 2;
    float expTableScale = (expTableSize - 1) / (lMax - lMin);
    float expTableOffset = -lMin * expTableScale + 0.5f;

    tables.setf (ios_base::dec, ios_base::basefield);

    tables << "const unsigned short expTable[" << expTableSize << "] =\n"
	      "{\n    ";

    tables.setf (ios_base::hex, ios_base::basefield);

    for (int i = 0; i < expTableSize; ++i)
    {
	double x = lMin + i / double (expTableScale);
	half h = exp (x);

	tables << "0x" << setfill ('0') << setw (4) << h.bits() << ", ";

	if (i % 4 == 3)
	{
	    tables << "\n";

	    if (i < expTableSize - 1)
		tables << "    ";
	}
    }

    tables << "};\n\n";

    functions <<
	"\n"
	"extern const unsigned int log10Table[];\n"
	"extern const unsigned int logTable[];\n"
	"extern const unsigned short expTable[];\n"
	"\n"
	"\n"
	"inline float\n"
	"log10_h (half x)\n"
	"{\n"
	"    return *(float *)(&log10Table[x.bits()]);\n"
	"}\n"
	"\n"
	"\n"
	"inline float\n"
	"log_h (half x)\n"
	"{\n"
	"    return *(float *)(&logTable[x.bits()]);\n"
	"}\n"
	"\n"
	"\n"
	"inline half\n"
	"exp_h (float x)\n"
	"{\n"
	"    if (x >= " << lMin << "f)\n"
	"    {\n"
	"        if (x <= " << lMax << "f)\n"
	"        {\n"
	"            //\n"
	"            // If we arrive here, then x is finite and between\n"
	"            // log(HALF_MIN) and log(HALF_MAX); indexing into\n"
	"            // expTable will not fail.\n"
	"            //\n"
	"\n"
	"            return *(half *)"
			 "(&expTable[int "
			 "(x * " << expTableScale << "f "
			 "+ " << expTableOffset << "f)]);\n"
	"        }\n"
	"        else\n"
	"        {\n"
	"            return half::posInf();\n"
	"        }\n"
	"    }\n"
	"    else if (x < " << lMin << "f)\n"
	"    {\n"
	"        return 0;\n"
	"    }\n"
	"    else\n"
	"    {\n"
	"        //\n"
	"        // If we arrive here, then x is a NaN.\n"
	"        //\n"
	"\n"
	"        return half::qNan();\n"
	"    }\n"
	"}\n"
	"\n"
	"\n"
	"inline half\n"
	"pow10_h (float x)\n"
	"{\n"
	"    return exp_h (x * " << log (10.0) << "f);\n"
	"}\n"
	"\n"
	"\n"
	"inline half\n"
	"pow_h (half x, float y)\n"
	"{\n"
	"    return exp_h (y * log_h (x));\n"
	"}\n"
	"\n";

    if (!tables || !functions)
    {
	cerr << "Error writing output files" << endl;
	exit (1);
    }
}


int
main (int argc, char *argv[])
{
    if (argc < 3)
    {
	cerr << "usage: " << argv[0] << " tablesFile.h functionsFile.h" << endl;
	return 1;
    }

    genHeaders (argv[1], argv[2]);
    return 0;
}
