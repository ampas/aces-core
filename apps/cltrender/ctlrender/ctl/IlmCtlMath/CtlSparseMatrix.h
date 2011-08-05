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

#ifndef INCLUDED_CTL_SPARSE_MATRIX_H
#define INCLUDED_CTL_SPARSE_MATRIX_H

//--------------------------------------------------------------------------
//
//	Sparse Matrices
//
//	The code in this file is derived from
//	code written by Francesco Callari at ILM.
//
//--------------------------------------------------------------------------

#include <IexBaseExc.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cassert>

namespace Ctl {

//---------------------------------------------------------------------
// 
// OPERATOR: CRSOperator
// MODEL OF: LinearTransposableOperator
//
// CRSOperator represents a sparse matrix in the Compressed Row
// Storage format, following R. Barret et al, "Templates for the
// Solution of Linear Systems: Building Blocks for Iterative Methods",
// pag 64. 
//
// A sparse matrix M of m rows and n columns, containing nnz nonzero
//    elements, is represented by the integer N == n, and three vectors:
//    * val, of size nnz, containing the nonzero elements of M,
//      ordered row-major. 
//    * col_ind, of size nnz, containing the column indices of the
//      nonzero elements. That is, if val[k] == M[i][j], then
//      col_ind[k] == j.
//    * row_ptr, of size m + 1, containing the indices of the elements
//      in col_ind that start a row. That is, if val[k] == M[i][j], then
//      row_ptr[i] <= k <= row_ptr[i+1]. As a special case, if the
//      i-th row of M does not have nonzero elements, then
//      row_ptr[i] == -1. Further, row_ptr[m] == nnz.
// 
//---------------------------------------------------------------------

template<typename T>
struct CRSOperator
{
    std::vector<T>      val;
    std::vector<size_t> col_ind;
    std::vector<size_t> row_ptr;
    size_t N;

    //---------------------------------------------------------
    // LinearTransposableOperator methods.
    //---------------------------------------------------------
    size_t numRows() const    { return row_ptr.size() -1; }
    size_t numColumns() const { return N;                 }
	
    template<typename xit, typename yit>
    void apply(xit xi, xit xe, yit yi, yit ye) const;

    template<typename xit, typename yit>
    void applyT(xit xi, xit xe, yit yi, yit ye) const;

    //---------------------------------------------------------
    // Default constructor and destructor.
    //---------------------------------------------------------
    CRSOperator() : N(0u) {}
    ~CRSOperator() {}

    //---------------------------------------------------------
    // Construct from basic blocks.
    //---------------------------------------------------------
    template<typename U>
    CRSOperator(const std::vector<U> & v,
		const std::vector<size_t> & c,
		const std::vector<size_t> & r,
		size_t n);
    
    //---------------------------------------------------------
    // Copy.
    //---------------------------------------------------------
    template<typename U>
    CRSOperator(const CRSOperator<U> & o);
    
    template<typename U>
    CRSOperator<T> & operator = (const CRSOperator<U> & o);

};


//---------------
// Implementation
//---------------


#if defined(DBGASSERT)
#   error "Macro DBGASSERT already defined"
#endif
#if defined(DEBUG)
#   define DBGASSERT(x) assert(x)
#else
#   define DBGASSERT(x) 
#endif
	
template<typename T>
template<typename U>
CRSOperator<T>::
CRSOperator(const std::vector<U> & v,
	    const std::vector<size_t> & c,
	    const std::vector<size_t> & r,
	    size_t n)
    : val(v.begin(), v.end()),
      col_ind(c),
      row_ptr(r),
      N(n)
{
    assert(! row_ptr.empty());
    assert(val.size() == col_ind.size());
    assert(val.size() == row_ptr.back());	
}

template<typename T>
template<typename U>
CRSOperator<T>::
CRSOperator(const CRSOperator<U> & o)
    : val(o.val.begin(), o.val.end()),
      col_ind(o.col_in),
      row_ptr(o.row_ptr),
      N(o.N)
{}

template<typename T>
template<typename U>
CRSOperator<T> & CRSOperator<T>::
operator = (const CRSOperator<U> & o)
{
    val.resize(o.val.size());
    std::copy(o.val.begin(), o.val.end(), val.begin());
    col_ind = o.col.ind;
    row_ptr = o.row_ptr;
    N = o.N;
    return *this;
}


template<typename T>
template<typename xit, typename yit>
void CRSOperator<T>::
apply(xit xi, xit xe, yit yi, yit ye) const
{
    DBGASSERT(std::distance(xi, xe) == numColumns());
    DBGASSERT(std::distance(yi, ye) == numRows());
	
    typedef typename std::iterator_traits<yit>::value_type YValue;
    typename std::vector<T>::const_iterator vi = val.begin();
    typename std::vector<size_t>::const_iterator ci, ce;
    typename std::vector<size_t>::const_iterator ri, re = row_ptr.end() -1;
	
    for (ri = row_ptr.begin(); ri < re; ++ri, ++yi)
    {
	*yi = YValue(0);

	if (*ri < 0)
	    continue;
	
	ce = ri + 1;
	while (*ce < 0 && ce <= re)
	    ++ce;
	
	ce = col_ind.begin() + *ce;
	DBGASSERT(ce <= col_ind.end());
	
	for (ci = col_ind.begin() + *ri; ci < ce; ++ci, ++vi)
	    *yi += (*vi) * (*(xi + *ci));
    }
}

template<typename T>
template<typename xit, typename yit>
void CRSOperator<T>::
applyT(xit xi, xit xe, yit yi, yit ye) const
{
    DBGASSERT(std::distance(xi, xe) == numRows());
    DBGASSERT(std::distance(yi, ye) == numColumns());
	
    typedef typename std::iterator_traits<yit>::value_type XValue;
    typedef typename std::iterator_traits<yit>::value_type YValue;
    typename std::vector<T>::const_iterator vi = val.begin();
    typename std::vector<size_t>::const_iterator ci, ce;
    typename std::vector<size_t>::const_iterator ri, re = row_ptr.end() -1;

    std::fill(yi, yi + numColumns(), YValue(0));

    for (ri = row_ptr.begin(); ri < re; ++ri, ++xi)
    {
	if (*ri < 0)
	    continue;

	ce = ri + 1;
	while (*ce < 0 && ce <= re)
	    ++ce;
	
	ce = col_ind.begin() + *ce;
	DBGASSERT(ce <= col_ind.end());
	
	XValue x = *xi;

	vi = val.begin() + *ri;
	DBGASSERT(vi < val.end());
	    
	for (ci = col_ind.begin() + *ri; ci < ce; ++ci, ++vi)
	{
	    *(yi + *ci) += (*vi) * x;
	}
    }
}

} // namespace Ctl

#undef DBGASSERT

#endif
