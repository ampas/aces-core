///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


#ifndef INCLUDED_IMF_CTL_COPY_FUNCTION_ARG_H
#define INCLUDED_IMF_CTL_COPY_FUNCTION_ARG_H

//-----------------------------------------------------------------------------
//
//	Copy data between CTL function arguments, OpenEXR
//	file header attributes and OpenEXR frame buffers.
//
//-----------------------------------------------------------------------------

#include <ImfAttribute.h>
#include <ImfFrameBuffer.h>
#include <CtlFunctionCall.h>
#include <ImathBox.h>

namespace ImfCtl {

//
// varying or uniform FunctionArg <-> FunctionArg
//

void copyFunctionArg (size_t numSamples,
		      const Ctl::FunctionArgPtr &src,
		      const Ctl::FunctionArgPtr &dst);

//
// Frame buffer slice <-> varying FunctionArg
//

void copyFunctionArg (const Imath::Box2i transformWindow,
		      size_t firstSample,
		      size_t numSamples,
		      const Imf::Slice &src,
		      const Ctl::FunctionArgPtr &dst);

void copyFunctionArg (const Imath::Box2i transformWindow,
		      size_t firstSample,
		      size_t numSamples,
		      const Ctl::FunctionArgPtr &src,
		      const Imf::Slice &dst);

//
// Attribute <-> uniform FunctionArg
//

void copyFunctionArg (const Imf::Attribute &src,
		      const Ctl::FunctionArgPtr &dst);

void copyFunctionArg (const Ctl::FunctionArgPtr &src,
		      Imf::Attribute &dst);


} // namespace ImfCtl

#endif
