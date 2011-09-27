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

#include "dpx_bits.hh"

namespace ctl {
namespace dpxi {

const uint64_t max_int_for_bits[]={ 0x0000000000000000ULL,
                                    0x0000000000000001ULL,
                                    0x0000000000000003ULL,
                                    0x0000000000000007ULL,
                                    0x000000000000000fULL,
                                    0x000000000000001fULL,
                                    0x000000000000003fULL,
                                    0x000000000000007fULL,
                                    0x00000000000000ffULL,
                                    0x00000000000001ffULL,
                                    0x00000000000003ffULL,
                                    0x00000000000007ffULL,
                                    0x0000000000000fffULL,
                                    0x0000000000001fffULL,
                                    0x0000000000003fffULL,
                                    0x0000000000007fffULL,
                                    0x000000000000ffffULL,
                                    0x000000000001ffffULL,
                                    0x000000000003ffffULL,
                                    0x000000000007ffffULL,
                                    0x00000000000fffffULL,
                                    0x00000000001fffffULL,
                                    0x00000000003fffffULL,
                                    0x00000000007fffffULL,
                                    0x0000000000ffffffULL,
                                    0x0000000001ffffffULL,
                                    0x0000000003ffffffULL,
                                    0x0000000007ffffffULL,
                                    0x000000000fffffffULL,
                                    0x000000001fffffffULL,
                                    0x000000003fffffffULL,
                                    0x000000007fffffffULL,
                                    0x00000000ffffffffULL,
                                    0x00000001ffffffffULL,
                                    0x00000003ffffffffULL,
                                    0x00000007ffffffffULL,
                                    0x0000000fffffffffULL,
                                    0x0000001fffffffffULL,
                                    0x0000003fffffffffULL,
                                    0x0000007fffffffffULL,
                                    0x000000ffffffffffULL,
                                    0x000001ffffffffffULL,
                                    0x000003ffffffffffULL,
                                    0x000007ffffffffffULL,
                                    0x00000fffffffffffULL,
                                    0x00001fffffffffffULL,
                                    0x00003fffffffffffULL,
                                    0x00007fffffffffffULL,
                                    0x0000ffffffffffffULL,
                                    0x0001ffffffffffffULL,
                                    0x0003ffffffffffffULL,
                                    0x0007ffffffffffffULL,
                                    0x000fffffffffffffULL,
                                    0x001fffffffffffffULL,
                                    0x003fffffffffffffULL,
                                    0x007fffffffffffffULL,
                                    0x00ffffffffffffffULL,
                                    0x01ffffffffffffffULL,
                                    0x03ffffffffffffffULL,
                                    0x07ffffffffffffffULL,
                                    0x0fffffffffffffffULL,
                                    0x1fffffffffffffffULL,
                                    0x3fffffffffffffffULL,
                                    0x7fffffffffffffffULL,
                                    0xffffffffffffffffULL
                                  };

};

};
