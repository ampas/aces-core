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

//---------------------------------------------------------------------
//
// 	Routines to solve linear algebra problems
//	involving large sparse matrices.
//
//	The code in this file is derived from
//	code written by Francesco Callari at ILM.
//
//---------------------------------------------------------------------

#ifndef INCLUDED_CTL_LINEAR_SOLVER_H
#define INCLUDED_CTL_LINEAR_SOLVER_H

#include <IexMacros.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cassert>

#if defined(DBGASSERT)
#    error "Macro DBGASSERT already defined"
#endif
#if defined(DEBUG)
#    define DBGASSERT(x) assert(x)
#else
#    define DBGASSERT(x)
#endif

namespace Ctl {

//---------------------------------------------------------------------
// OPERATOR: NullLinearOperator
// MODEL OF: LinearOperator
// 
// A NullLinearOperator is a dummy model of LinearOperator, used to
// specify defaults for some of the  templates below.  
//---------------------------------------------------------------------
struct NullLinearOperator
{
    size_t numRows()    const { return 0; }
    size_t numColumns() const { return 0; }

    template<typename x_iterator, typename y_iterator>
    void apply(x_iterator x_first, x_iterator x_last,
               y_iterator y_first, y_iterator y_last) const
    { assert(0 && "Attempt to use NullLinearOperator::apply()."); }
};

//---------------------------------------------------------------------
// CONCEPT: LinearSolver
// 
// A LinearSolver is a functor that solves linear system defined by
// a LinearOperator and a range of constants.
//
// Requirements on types:
//     * Operator must be a model of LinearOperator
//     * b_iterator must be a model of Input Iterator.
//     * x_iterator must be a model of Output Iterator.
//     * The value type of the iterators must be numeric.
// 
// Preconditions:
//     * [b_first, b_last) is a valid range.
//     * distance(b_first, b_last) == A.numRows()
//     * [x_first, x_last) is a valid range.
//     * distance(x_first, x_last) == A.numColumns()
//
// Postconditions
//     * T returns a measure of the accuracy of the solution
//     
//---------------------------------------------------------------------
template<typename T, typename Operator>
struct LinearSolver
{
    const Operator & op() const;
    
    template<typename b_iterator, typename x_iterator>
    T operator () (b_iterator b_first, b_iterator b_last,
                   x_iterator x_first, x_iterator x_last) const;
};

//---------------------------------------------------------------------
// FUNCTOR: CG
// MODEL OF: LinearSolver
// 
// CG computes the solution through the conjugate gradient algorithm
// with optional preconditioning of linear systems of the form A x = b, with
// A square, symmetric, positive-definite and non-singular matrices.
// 
// Requirements on types:
//     * T must be an arithmetic type.
//     * Operator must be a model of LinearOperator.
//     * Preconditioner must be a model of LinearOperator.
//     * b_iterator must be a model of Input Iterator.
//     * x_iterator must be a model of Output Iterator.
//     * The value type of the iterators must be numeric.
//     
// Preconditions:
//     * A.numRows() > 0
//     * A.numRows() == A.numColumns().
//     * A implies a symmetric, positive-definite, non-singular
//       matrix.
//     * [b_first, b_last) is a valid range.
//     * distance(b_first, b_last) == A.numRows()
//     * [x_first, x_last) is a valid range.
//     * distance(x_first, x_last) == A.numColumns()
//     * For the computation with preconditioning:
//       * M.numColumns() == A.numColumns().
//       * M.numRows() == A.numRows().
//       * M implies a symmetric, positive-definite, non-singular
//         matrix.
//
// On input the range [x_first, x_last) contains an
// initial estimate for the solution.
// 
// On output, the same range contains a refined estimate, and the
// routine returns the L2 norm of the residual when the tolerance is
// reached or after the maximum number of iterations has been
// performed, whichever comes first.  
// 
//---------------------------------------------------------------------
template<typename T, typename Operator, 
         typename Preconditioner = NullLinearOperator> 
struct CG
{
    unsigned maxNumIterations;
    T tolerance;
    const Preconditioner * M;
    const Operator & A;
    
    CG(const Operator & a, const Preconditioner * m = 0);

    const Operator & op() const { return A; }

    template<typename b_iterator, typename x_iterator>
    T operator () (b_iterator b_first, b_iterator b_last,
                   x_iterator x_first, x_iterator x_last) const;

  private:

    template<typename b_iterator, typename x_iterator>
    T cg(b_iterator b_first, b_iterator b_last,
         x_iterator x_first, x_iterator x_last) const;

    template<typename b_iterator, typename x_iterator>
    T cgp(b_iterator b_first, b_iterator b_last,
          x_iterator x_first, x_iterator x_last) const;

    T dot(const std::vector<T> & x, const std::vector<T> & y) const;

    // Computes z = x - y
    template<typename iterator>
    void sub(iterator x_first, iterator x_last,
             const std::vector<T> & y, std::vector<T> & z) const;

    // Computes z = alpha * x + y
    template<typename iterator>
    void saxpy(T alpha, const std::vector<T> &x,
               iterator y_first, iterator y_last,
               iterator z_first, iterator z_last) const;
};

//---------------------------------------------------------------------
// FUNCTOR: LSSOperator
// MODEL OF: LinearOperator
// 
// LSSOperator is a LinearOperator that computes y = A' A x  from A
// (without actually forming A' A) 
// 
// Requirements on types:
//     * T must be an arithmetic type.
//     * Operator must be a model of LinearTransposableOperator.
//     
//---------------------------------------------------------------------
template<typename T, typename Operator> 
struct LSSOperator
{
    const Operator & A;
    mutable std::vector<T> aux;
    
    LSSOperator(const Operator & a) : A(a) {}

    size_t numRows()    const { return A.numColumns(); }
    size_t numColumns() const { return numRows();      }
    
    template<typename i_iterator, typename o_iterator>
    void apply(i_iterator x_first, i_iterator x_last,
	       o_iterator y_first, o_iterator y_last) const
    {
	DBGASSERT(distance(x_first, x_last) == A.numColumns());
	DBGASSERT(distance(y_first, y_last) == A.numColumns());

	aux.resize(A.numRows());
	A.apply(x_first, x_last, aux.begin(), aux.end());
	A.applyT(aux.begin(), aux.end(), y_first, y_last);
    }
};


//---------------------------------------------------------------------
// FUNCTOR: LSS
// MODEL OF: LinearSolver
// 
// Minimum L2 norm solution of an overconstrained linear system of the
// form A x = b through the normal equations  A' A x = A' b. These are
// solved in turn using the LinearSolver specified as template argument.
// 
// Requirements on types:
//     * T must be an arithmetic type.
//     * Operator must be a model of LinearTransposableOperator.
//     * Solver must be a model of LinearSolver.
//     * Preconditioner must be a model of LinearOperator.
//     * Applicator must be a model of LinearApplicator
//     * b_iterator must be a model of Input Iterator.
//     * x_iterator must be a model of Output Iterator.
//     * The value type of the iterators must be numeric.
//     
// Preconditions:
//     * A.numColumns() > 0
//     * [b_first, b_last) is a valid range.
//     * distance(b_first, b_last) == A.numRows()
//     * [x_first, x_last) is a valid range.
//     * distance(x_first, x_last) == A.numColumns()
//     * For the computation with preconditioning:
//       * M.numColumns() == M.numRows() == A.numColumns().
//       * M implies a symmetric, positive-definite, non-singular
//         matrix.
//     
// On input the range [x_first, x_first + A.numRows()) contains an
// initial estimate for the solution.
// 
// On output, the same range contains a refined estimate, and the
// routine returns the L2 norm of the residual when the tolerance is
// reached or after the maximum number of iterations has been
// performed, whichever comes first.  
//
//---------------------------------------------------------------------
template<typename T, typename Operator, typename Solver,
         typename Preconditioner = NullLinearOperator>
struct LSS
{
    typedef LSSOperator<T, Operator> LSSOP;

  protected:

    LSSOP _op;

  public:

    Solver solver;
    
    LSS(const Operator & a, const Preconditioner * m = 0)
	: _op(a), solver(_op, m)
    {}

    const LSSOP & op() const { return _op; }
    
    template<typename b_iterator, typename x_iterator>
    T operator () (b_iterator b_first, b_iterator b_last,
                   x_iterator x_first, x_iterator x_last) const
    {
	std::vector<T> c(op().A.numColumns());
	constants(b_first, b_last, c.begin(), c.end());
	return solver(c.begin(), c.end(), x_first, x_last);
    }

  protected:

    // Computes c = A' b
    template<typename i_iterator, typename o_iterator>
    void constants(i_iterator b_first, i_iterator b_last,
		   o_iterator c_first, o_iterator c_last) const;
};

//---------------------------------------------------------------------
// FUNCTOR: LSSCG
// MODEL OF: LinearSolver
// 
// LSSCG is an LSS that uses CG as the Solver.
//---------------------------------------------------------------------
template<typename T, typename Operator,
         typename Preconditioner = NullLinearOperator>
struct LSSCG : public LSS<T, Operator,
			  CG<T, LSSOperator<T, Operator>, Preconditioner>,
			  Preconditioner>
{
    typedef LSS<T, Operator,
		CG<T, LSSOperator<T, Operator>, Preconditioner>,
		Preconditioner> parent;

    LSSCG(const Operator & a, const Preconditioner * m = 0)
	: parent(a, m)
    {}
};


//---------------
// Implementation
//---------------

//
// CG
//

template<typename T, typename Operator, typename Preconditioner> 
CG<T, Operator, Preconditioner>::
CG(const Operator & a, const Preconditioner * m)
    : maxNumIterations(400u),
      tolerance(T(1.0e-10)),
      M(m),
      A(a)
{}

// Algo from Shewchuck 1994
template<typename T, typename Operator, typename Preconditioner>
template<typename b_iterator, typename x_iterator>
inline T CG<T, Operator, Preconditioner>::
operator () (b_iterator b_first, b_iterator b_last,
	     x_iterator x_first, x_iterator x_last) const
{
    ASSERT(op().numRows() > 0, Iex::ArgExc,
	   "Ctl::CG() requires that "
	   "A.numRows() > 0.");
    ASSERT(op().numRows() == op().numColumns(), Iex::ArgExc,
	   "Ctl::CG() requires that "
	   "A.numRows() == A.numColumns().");
    ASSERT(!M || M->numRows() == op().numRows(), Iex::ArgExc,
	   "Ctl::CG() requires that "
	   "M->numRows() == A.numRows().");
    ASSERT(!M || M->numColumns() == op().numColumns(), Iex::ArgExc,
	   "Ctl::CG() requires that "
	   "M->numColumns() == A.numColumns().");
    ASSERT(std::distance(b_first, b_last) == (int)op().numRows(), Iex::ArgExc,
	   "Ctl::CG() requires that "
	   "distance(b_first, b_last) == A.numRows().");
    ASSERT(std::distance(x_first, x_last) == (int)op().numColumns(), Iex::ArgExc,
	   "Ctl::CG() requires that "
	   "distance(x_first, x_last) == A.numColumns().");

    if (! M)
	return cg(b_first, b_last, x_first, x_last);
    else
	return cgp(b_first, b_last, x_first, x_last);
}

template<typename T, typename Operator, typename Preconditioner>
template<typename b_iterator, typename x_iterator>
T CG<T, Operator, Preconditioner>::
cg(b_iterator b_first, b_iterator b_last,
   x_iterator x_first, x_iterator x_last) const
{
    size_t n = op().numColumns();
    std::vector<T> d(n), q(n), r(n), t(n), xx(n);

    std::copy(x_first, x_last, xx.begin());   

    A.apply(x_first, x_last, t.begin(), t.end()); // t = A * x
    sub(b_first, b_last, t, r);                   // r = b - t;
    std::copy(r.begin(), r.end(), d.begin());     // d = r

    T deltaNew = dot(r, r);
    T deltaBest = sqrt(deltaNew);
    for (unsigned i = 0; i < maxNumIterations && sqrt(deltaBest) > tolerance; ++i)
    {
	A.apply(d.begin(), d.end(), q.begin(), q.end()); // q = A * d
	
	T alpha = deltaNew / dot(d, q);

	saxpy(alpha, d, x_first, x_last, x_first, x_last); // x += d * alpha;

	if (0 == i % 50)
	{
	    A.apply(x_first, x_last, t.begin(), t.end());   // t = A * x
	    sub(b_first, b_last, t, r);                   // r = b - t;
	}
	else
	{
	    // r -= q * alpha;	    
	    saxpy(-alpha, q, r.begin(), r.end(), r.begin(), r.end()); 
	}
	
	T deltaOld = deltaNew;
	deltaNew = dot(r, r);

	T beta = deltaNew / deltaOld;

	// d = d * beta + r;
	saxpy(beta, d, r.begin(), r.end(), d.begin(), d.end()); 

	if (deltaNew < deltaBest)
	{
	    deltaBest = deltaNew;
	    std::copy(x_first, x_last, xx.begin());
	}
    }

    std::copy(xx.begin(), xx.end(), x_first);
    return deltaBest;
}

template<typename T, typename Operator, typename Preconditioner>	
template<typename b_iterator, typename x_iterator>
T CG<T, Operator, Preconditioner>::
cgp(b_iterator b_first, b_iterator b_last,
    x_iterator x_first, x_iterator x_last) const
{
    assert(M);

    size_t n = op().numColumns();
    std::vector<T> d(n), q(n), r(n), s(n), t(n), xx(n);

    std::copy(x_first, x_last, xx.begin()); 
    
    A.apply(x_first, x_last, t.begin(), t.end());        // t = A * x
    sub(b_first, b_last, t, r);                        // r = b - t
    M->apply(r.begin(), r.end(), d.begin(), d.end());  // d = M * r

    T deltaNew = dot(r, r);
    T deltaBest = deltaNew;
    T delta0 = tolerance * deltaNew;
    T thetaNew = dot(r, d);
    
    for (unsigned i = 0; i < maxNumIterations && deltaBest > delta0; ++i)
    {
	// std::cout << i << ' ' << deltaBest << std::endl;
	
	A.apply(d.begin(), d.end(), q.begin(), q.end()); // q = A * d
	
	T alpha = thetaNew / dot(d, q);

	saxpy(alpha, d, x_first, x_last, x_first, x_last); // x += d * alpha;

	if (0 == i % 50)
	{
	    A.apply(x_first, x_last, t.begin(), t.end());   // t = A * x
	    sub(b_first, b_last, t, r);                   // r = b - t;
	}
	else
	{
	    // r -= q * alpha;	    
	    saxpy(-alpha, q, r.begin(), r.end(), r.begin(), r.end()); 
	}
	
	M->apply(r.begin(), r.end(), s.begin(), s.end()); // s = M * r
	    
	T thetaOld = thetaNew;
	thetaNew = dot(r, s);

	T beta = thetaNew / thetaOld;

	// d = d * beta + s;
	saxpy(beta, d, s.begin(), s.end(), d.begin(), d.end()); 

	deltaNew = dot(r, r);
	if (deltaNew < deltaBest)
	{
	    deltaBest = deltaNew;
	    std::copy(x_first, x_last, xx.begin());
	}
    }

    std::copy(xx.begin(), xx.end(), x_first);
    return deltaBest;
}

template<typename T, typename Operator, typename Preconditioner>
inline T CG<T, Operator, Preconditioner>::
dot(const std::vector<T> & x, const std::vector<T> & y) const
{
    DBGASSERT(x.size() == y.size());

    return std::inner_product(x.begin(), x.end(), y.begin(), T(0));
}

template<typename T, typename Operator, typename Preconditioner>
template<typename iterator>	 
inline void CG<T, Operator, Preconditioner>::
sub(iterator x_first, iterator x_last,
    const std::vector<T> & y, std::vector<T> & z) const
{
    DBGASSERT(distance(x_first, x_last) == y.size());
    DBGASSERT(y.size() == z.size());
    
    typename std::vector<T>::const_iterator yi = y.begin();
    typename std::vector<T>::iterator zi = z.begin();
    for (; yi < y.end(); ++x_first, ++yi, ++zi)
    {
	*zi = *x_first;
	*zi -= *yi;
    }
}

template<typename T, typename Operator, typename Preconditioner>
template<typename iterator>	 
inline void CG<T, Operator, Preconditioner>::
saxpy(T alpha, const std::vector<T> & x,
      iterator y_first, iterator y_last,
      iterator z_first, iterator z_last) const
{
    DBGASSERT(distance(y_first, y_last) == x.size());
    DBGASSERT(distance(z_first, z_last) == x.size());

    typename std::vector<T>::const_iterator xi;
    typedef typename std::iterator_traits<iterator>::value_type YValue;
    YValue y;
    for (xi = x.begin(); xi < x.end(); ++xi, ++y_first, ++z_first)
    {
	y = *y_first; // this is needed in case y_first == z_first
	*z_first = *xi;
	*z_first *= alpha;
	*z_first += y;
    }
}

//
// LSS
//

template<typename T, typename Operator,
	 typename Solver, typename Preconditioner>
template<typename i_iterator, typename o_iterator>
void LSS<T, Operator, Solver, Preconditioner>::
constants(i_iterator b_first, i_iterator b_last,
	  o_iterator c_first, o_iterator c_last) const
{
    DBGASSERT(std::distance(b_first, b_last) == op().A.numRows());
    DBGASSERT(std::distance(c_first, c_last) == op().A.numColumns());

    op().A.applyT(b_first, b_last, c_first, c_last);
}

} // namespace Ctl

#undef DBGASSERT

#endif
