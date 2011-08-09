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


//-----------------------------------------------------------------------------
//
//	class SimdInterpreter
//
//-----------------------------------------------------------------------------

#include <CtlSimdInterpreter.h>
#include <CtlSimdLContext.h>
#include <CtlSimdModule.h>
#include <CtlSimdStdLibrary.h>
#include <CtlSimdReg.h>
#include <CtlSimdFunctionCall.h>
#include <IlmThreadMutex.h>
#include <Iex.h>
#include <cassert>

using namespace std;
using namespace Iex;
using namespace IlmThread;

namespace Ctl {


struct SimdInterpreter::Data
{
    Mutex		mutex;
    unsigned long	maxInstCount;
    unsigned long	abortCount;
};


SimdInterpreter::SimdInterpreter():
    Interpreter(),
    _data (new Data)
{
    _data->maxInstCount = 10000000;
    _data->abortCount = 0;

    //
    // Create a dummy LContext and load the CTL standard library
    //

    SimdModule module (*this, "none", "none");
    stringstream file;
    SimdLContext lcontext (file, &module, symtab());

    declareSimdStdLibrary (lcontext);
}


SimdInterpreter::~SimdInterpreter()
{
    delete _data;
}


size_t
SimdInterpreter::maxSamples () const
{
    return MAX_REG_SIZE;
}


void	
SimdInterpreter::setMaxInstCount (unsigned long count)
{
    Lock lock (_data->mutex);
    _data->maxInstCount = count;
}


void	
SimdInterpreter::abortAllPrograms ()
{
    Lock lock (_data->mutex);
    _data->abortCount += 1;
}

unsigned long	
SimdInterpreter::abortCount()
{
    Lock lock (_data->mutex);
    return _data->abortCount;
}


unsigned long	
SimdInterpreter::maxInstCount()
{
    Lock lock (_data->mutex);
    return _data->maxInstCount;
}


Module *
SimdInterpreter::newModule
    (const string &moduleName,
     const string &fileName)
{
    return new SimdModule (*this, moduleName, fileName);
}


FunctionCallPtr
SimdInterpreter::newFunctionCallInternal 
    (const SymbolInfoPtr info, 
     const string& functionName)
{
    assert(info);

    return new SimdFunctionCall
	(*this, functionName, info->type(), info->addr(), symtab());
}


LContext *
SimdInterpreter::newLContext
    (istream &file,
     Module *module,
     SymbolTable &symtab) const
{
    return new SimdLContext (file, module, symtab);
}

} // namespace Ctl
