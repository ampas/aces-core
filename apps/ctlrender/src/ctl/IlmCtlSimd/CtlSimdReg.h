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


#ifndef INCLUDED_CTL_SIMD_REG_H
#define INCLUDED_CTL_SIMD_REG_H

#include <typeinfo>
#include <CtlExc.h>
#include <Iex.h>
#include <string.h>

//-----------------------------------------------------------------------------
//
//	Registers for the SIMD color transformation engine
//
//      Registers can contain a single element or MAX_REG_SIZE elements
//      if the register is varying.  SimdReg encapsulates the logic to
//      handle varying or non-varying-ness of the register.
//
//      A register is either a "value" register or a "reference" register.
//      A reference register points to values in another register.  SimdReg
//      also handles the logic to access elements of a register regardless
//      of whether it is a value or ref register.
//
//-----------------------------------------------------------------------------

namespace Ctl {

const int MAX_REG_SIZE = 4096;


class SimdBoolMask
{
  public:
    SimdBoolMask(const SimdBoolMask &copy, int copyLen);

    explicit SimdBoolMask(bool varying)
	: _varying(varying),
	  _data (new bool [ varying ? MAX_REG_SIZE : 1]) {}

    ~SimdBoolMask() { delete [] _data; }

    void		setVarying (bool varying);
    bool                isVarying() const         {return _varying;}

    bool                &operator [] (int i) const 
	                               {return _varying ? _data[i] : _data[0]; }
  private:    
    bool		_varying;
    bool              * _data;

};


class SimdReg
{
  public:

    // Value constructor
    explicit SimdReg (bool varying, size_t elementSize);

    //
    // Reference constructor for array indexing.
    // If transferData is true and original is not a reference
    // this register takes over ownership of the original data,
    // rendering the original register useless
    //
    explicit SimdReg(SimdReg &original, const SimdReg &indices, 
	    const SimdBoolMask &mask, 
	    size_t arrayElementSize, 
	    size_t arraySize,
	    size_t regSize,
	    bool transferData = false);

    // Reference constructor for struct member access
    explicit SimdReg(SimdReg &r, 
	    const SimdBoolMask &mask, 
	    size_t offset,
	    size_t regSize,
	    bool transferData = false);

    ~SimdReg ();

    //
    // Similar to the reference constructors above, but creates a reference
    // out of an existing simdReg.
    // 
    void reference(SimdReg &r, bool transferData = false);


    void		setVarying (bool varying);
    void		setVaryingDiscardData (bool varying);
    bool                isVarying () const { return _varying || _oVarying; }
    size_t              elementSize () const { return _eSize; }
    bool		isReference () const {return _ref != 0;}


    const char*	operator [] (int i) const 
    {
	return _ref ? _ref->_data + (_ref->_varying ? offset(i) + i *_eSize 
				                    : offset(i) )
                    :               (_varying       ? _data + i *_eSize 
                                                   : _data);
    }
    char*	operator [] (int i)
    {
	return _ref ? _ref->_data + (_ref->_varying ? offset(i) + i *_eSize 
				                    : offset(i) )
                    :               (_varying       ? _data + i *_eSize 
                                                   : _data);
    }

  protected:

    size_t              offset(int i) const
    { return _oVarying ? _offsets[i] : _offsets[0]; }

    //  A register has four ownership states:
    // 
    //  1) A register created from scratch:  
    //        _ref = 0, 
    //        _data = the register's data
    //        _offsets = 0
    //        _oVarying = false
    //        _Varying = true | false
    // 
    //  2) A reference register created by array index or struct member
    //  access, referencing a static register or temporary lower on the
    //  stack
    //        _ref = the referenced register
    //        _data = 0
    //        _offsets = offsets into reference register's data
    //        _oVarying = true | false
    //        _Varying = true | false
    //
    //  3) A reference register created by array index or struct member
    //  access, referencing a temporary register about to be poped from
    //  the stack (and deleted).
    //        _ref = this
    //        _data = data from the original register
    //        _offsets = offsets into this register's data
    //        _oVarying = true | false        
    //        _Varying = true | false
    //
    //  4) A register whose data ownership has been transfered to a
    //  a register of type 3).  This register can not be accessed
    //  and can only be deleted.
    //        _ref = any:  not valid
    //        _data = 0
    //        _offsets = 0 or not valid anymore - to be deleted
    //        _oVarying = true | false
    //        _Varying = true | false


    size_t              _eSize;        // Size of element in varying array
    bool		_varying;
    bool                _oVarying;     // Ref Register Offsets varying?
    size_t*             _offsets;      // indexed offsets into a _data block
    char*               _data;
    SimdReg*            _ref;          // If a reference, points to original

  private:
    static size_t *zeroOffset;  // for reference registers,_offsets = zeroOffset
};


inline void
SimdBoolMask::setVarying(bool varying)
{
    if(varying != _varying)
    {
        bool* data = new bool [varying? MAX_REG_SIZE : 1];

	if (varying)
	    memset(data, _data[0], MAX_REG_SIZE);
	else
	    data[0] = _data[0];

	delete [] _data;
 	_data = data;
	_varying = varying;
    }
}


inline
SimdBoolMask::SimdBoolMask(const SimdBoolMask &copy, int copyLen)
    : _varying(copy.isVarying()),
      _data (new bool [ copy.isVarying() ? MAX_REG_SIZE : 1])
{
    if(_varying)
	memcpy(_data, copy._data, copyLen*sizeof(bool));
    else
	_data[0] = copy._data[0];

}

} // namespace Ctl

#endif
