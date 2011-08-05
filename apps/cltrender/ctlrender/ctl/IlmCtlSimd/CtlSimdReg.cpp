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
//	Registers for the SIMD color transformation engine
//
//-----------------------------------------------------------------------------

#include <CtlSimdReg.h>
#include <sstream>



namespace Ctl {
namespace {

size_t zeroOffsetPlaceholder = 0;

void
throwIndexOutOfRange (int index, int size)
{
    THROW (IndexOutOfRangeExc,
	   "Array index out of range "
	   "(index = " << index << ", "
	   "array size = " << size << ").");
}

} // namespace

size_t *SimdReg::zeroOffset = &zeroOffsetPlaceholder;


SimdReg::SimdReg (bool varying, size_t elementSize)
: _eSize(elementSize), 
  _varying(varying), 
  _oVarying(false),
  _offsets(zeroOffset),
  _data (new char [ varying ? MAX_REG_SIZE * _eSize : _eSize]),
  _ref(0)
{
}


SimdReg::SimdReg
   (SimdReg &r, 
    const SimdReg &indReg, 
    const SimdBoolMask &mask, 
    size_t arrayElementSize,
    size_t arraySize,
    size_t regSize,
    bool transferData /* = false */)

       : _eSize(r._eSize),
	 _varying(r._varying),
	 _oVarying(indReg.isVarying() || r._oVarying),
	 _offsets(new size_t [_oVarying ? MAX_REG_SIZE : 1]),
	 _data(transferData && r._data ? r._data : 0),
         _ref(transferData && r._data ? this : (r._ref ? r._ref : &r))
{
    if( _oVarying )
    {
	if( r._oVarying)
	{
	    for( int i = 0; i < (int)regSize; i++ )
	    {
		int ind = *(int *)(indReg[i]);
		if( ind < 0 || ind >= (int)arraySize )
		    throwIndexOutOfRange (ind, arraySize);

		if( mask[i] )
		    _offsets[i] = r._offsets[i] 
			+ ind*arrayElementSize;
	    }
	}
	else  // !r._oVarying
	{
	    for( int i = 0; i < (int)regSize; i++ )
	    {
		int ind = *(int *)(indReg[i]);
		if( ind < 0 || ind >= (int)arraySize )
		    throwIndexOutOfRange (ind, arraySize);

		if( mask[i] )
		    _offsets[i] = r._offsets[0] 
			+ ind*arrayElementSize;
	    }
	}
    }
    else // ! _oVarying
    {
	int ind = *(int*)(indReg[0]);
	if( ind < 0 || ind >= (int)arraySize )
	    throwIndexOutOfRange (ind, arraySize);
	
	_offsets[0] = r._offsets[0] + ind*arrayElementSize;
    }

    //
    // If we are tranfering the ownership, complete the transfer
    //
    if( transferData && r._data )
    {
	r._data = 0;
    }

}


SimdReg::SimdReg
   (SimdReg &r, 
    const SimdBoolMask &mask, 
    size_t offset,
    size_t regSize,
    bool transferData /* = false */)

       : _eSize(r._eSize),
	 _varying(r._varying),
	 _oVarying(r._oVarying),
	 _offsets(new size_t [_oVarying ? MAX_REG_SIZE : 1]),
	 _data(transferData && r._data ? r._data : 0),
         _ref(transferData && r._data ? this : (r._ref ? r._ref : &r))
{
    if( _oVarying )
    {
	for( int i = 0; i < (int)regSize; i++ )
	{
	    if( mask[i] )
		_offsets[i] = r._offsets[i] + offset;
	}
    }
    else // ! _oVarying
    {
	_offsets[0] = r._offsets[0] + offset;
    }

    //
    // If we are tranfering the ownership, complete the transfer
    //
    if( transferData && r._data )
    {
	r._data = 0;
    }
}




SimdReg::~SimdReg ()
{
    // If this is a reference register, clean up _offsets
    if( _offsets != zeroOffset)
	delete [] _offsets;

    delete [] _data;
}


void
SimdReg::reference(SimdReg &r,
		   bool transferData /* = false */)
{
    _eSize = r._eSize;
    _varying = r._varying;

    if( !_ref )
    {
	_offsets = new size_t [_oVarying ? MAX_REG_SIZE : 1];
    }
    else if(_oVarying != r._oVarying)
    {
	delete [] _offsets;
	_offsets = new size_t [_oVarying ? MAX_REG_SIZE : 1];
    }
    _oVarying = r._oVarying;

    delete [] _data;

    //
    // If we are tranfering the ownership, and the original is not a reference
    //
    if( transferData && r._data )
    {
	_ref =  this;
	_data =  r._data;
	r._data = 0;
    }
    else
    {
	_ref = r._ref ? r._ref : &r;
	_data = 0;
    }

    if( _oVarying )
	memcpy(_offsets, r._offsets, MAX_REG_SIZE*sizeof(*_offsets));
    else 
	_offsets[0] = r._offsets[0];

}


void 
SimdReg::setVarying (bool varying)
{
    if(_ref)
    {
	_ref->setVarying (varying);
    }
    else if (varying != _varying)
    {
        char *data = new char [varying? MAX_REG_SIZE * _eSize: _eSize];

	if (varying)
	{
 	    for (int i = 0; i < MAX_REG_SIZE; i++)
		memcpy (data + (i * _eSize), _data, _eSize);
	}
	else
	{
	    memcpy (data, _data, _eSize);
	}

	delete [] _data;
 	_data = data;
	_varying = varying;
    }
}


void 
SimdReg::setVaryingDiscardData (bool varying)
{
    if(_ref)
    {
	_ref->setVaryingDiscardData (varying);
    }
    else if (varying != _varying)
    {
        char *data = new char [varying? MAX_REG_SIZE * _eSize: _eSize];
	delete [] _data;
 	_data = data;
	_varying = varying;
    }
}

} // namespace Ctl
