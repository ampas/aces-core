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


#include <CtlSimdInterpreter.h>
#include <Iex.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <assert.h>
#include <string.h>

using namespace Ctl;
using namespace std;

namespace {

const char testDos[] =
    "int\r\n"
    "testDos()\r\n"
    "{\r\n"
    "    print(\"Dos source file\\n\"); //comment\r\n"
    "    return 1;\r\n"
    "}\r\n"
    "\r\n"
    "const int testDosDummy = testDos();\r\n";


const char testUnix[] =
    "int\n"
    "testUnix()\n"
    "{\n"
    "    print(\"Unix source file\\n\"); //comment\n"
    "    return 1;\n"
    "}\n"
    "\n"
    "const int testUnixDummy = testUnix();\n";


const char testMac[] =
    "int\r"
    "testMac()\r"
    "{\r"
    "    print(\"Mac source file\\n\"); //comment\r"
    "    return 1;\r"
    "}\r"
    "\r"
    "const int testMacDummy = testMac();\r";


void
writeSourceFile (const char fileName[], const char text[])
{
    ofstream file (fileName, ios_base::binary);

    if (!file)
	THROW_ERRNO ("Cannot open file " << fileName << " (%T).");

    file.write (text, strlen (text));

    if (!file)
	THROW_ERRNO ("Write to file " << fileName << " failed (%T).");
}

} //namespace


void
testEndOfLine ()
{
    try
    {
	cout << "Testing end-of-line conventions" << endl;

	writeSourceFile ("testDos.ctl", testDos);
	writeSourceFile ("testUnix.ctl", testUnix);
	writeSourceFile ("testMac.ctl", testMac);

	SimdInterpreter interp;
        interp.loadModule ("testDos");
        interp.loadModule ("testUnix");
        interp.loadModule ("testMac");

	cout << "ok\n" << endl;
    }
    catch (const std::exception &e)
    {
	cerr << "ERROR -- caught exception: " << endl << e.what() << endl;
	assert (false);
    }
}

