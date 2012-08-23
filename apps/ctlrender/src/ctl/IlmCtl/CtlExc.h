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


#ifndef INCLUDED_CTL_EXC_H
#define INCLUDED_CTL_EXC_H

//-----------------------------------------------------------------------------
//
//	Exception classes related to the
//	color transformation language interpreter.
//
//-----------------------------------------------------------------------------

#include <IexBaseExc.h>
#include <stdarg.h>

namespace Ctl {

class CtlExc : public Iex::BaseExc {
	public:
		void _explain(const char *text, va_list ap);
		CtlExc(const char *format=0, ...) throw();
		CtlExc(const std::string &text) throw(): Iex::BaseExc(text) {};
		CtlExc(std::stringstream &text) throw(): Iex::BaseExc(text) {};
};

#define CTL_DEFINE_EXC(name, base)                              \
    class name : public base                                    \
    {                                                           \
      public:                                                   \
        name (const char* text=0, ...) throw() {                \
            va_list ap;                                         \
            va_start(ap, text);                                 \
            _explain(text, ap);                                 \
            va_end(ap);                                         \
        };                                                      \
        name (const std::string &text) throw(): base (text) {}  \
        name (std::stringstream &text) throw(): base (text) {}  \
    };


CTL_DEFINE_EXC (MaxInstExc, CtlExc)         // program terminated because
                                            // it reached its maximum
                                            // instruction count

CTL_DEFINE_EXC (AbortExc, CtlExc)           // program was aborted

CTL_DEFINE_EXC (LoadModuleExc, CtlExc)      // failed to load module

CTL_DEFINE_EXC (StackUnderflowExc, CtlExc)  // run-time stack underflow

CTL_DEFINE_EXC (StackOverflowExc, CtlExc)   // run-time stack overflow

CTL_DEFINE_EXC (StackLogicExc, CtlExc)      // run-time stack error

CTL_DEFINE_EXC (IndexOutOfRangeExc, CtlExc) // array index out of range

CTL_DEFINE_EXC (InvalidSizeExc, CtlExc)	    // invalid array size specified

CTL_DEFINE_EXC (ArrayMismatchExc, CtlExc)   // array assignment mismatch

CTL_DEFINE_EXC (StructAccessExc, CtlExc)    // inconsistent struct access
                                            // permissions

CTL_DEFINE_EXC (RuntimeExc, CtlExc)         // Some compile time issue was
                                            // not caught until runtime

CTL_DEFINE_EXC (DatatypeExc, CtlExc)        // A compile time type conversion
                                            // error has been found in the
                                            // C++ <-> CTL data structure
                                            // interface code.

}; // namespace Ctl

#endif
