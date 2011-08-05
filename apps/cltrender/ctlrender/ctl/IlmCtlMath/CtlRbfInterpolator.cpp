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

//----------------------------------------------------------------------------
//
//	class RbfInterpolator -- performs scattered data
//
//----------------------------------------------------------------------------

#include <CtlRbfInterpolator.h>
#include <CtlPointTree.h>
#include <CtlSparseMatrix.h>
#include <CtlLinearSolver.h>

using namespace std;
using namespace Imath;

//#define DEBUG_RBF

namespace Ctl {


inline double 
RbfInterpolator::kernel (double val, double sigma) const
{
    assert(sigma > .0f);

    if (val > 2.*sigma)
	return .0f;
    else
    {
	double r = val/sigma;
	  
	if (r > 1.)
	{
	    r -= 2.f;	    
	    return (-.25*r*r*r) / (M_PI*sigma);
	}
	else 
	{
	    double r2 = r*r;

	    return (1 - 1.5*r2 + .75*r*r2) / (M_PI*sigma);
	}
    }
}


inline double 
RbfInterpolator::kernelGrad (double val, double sigma) const
{
    assert(sigma > .0f);

    if (val > 2.*sigma)
	return .0f;
    else
    {
	double r = val/sigma;
	  
	if (val > sigma)
	{
	    r -= 2.f;	    
	    return (-.75*r*r) / (M_PI*sigma);
	}
	else 
	{
	    return (-3*r + 2.25*r*r) / (M_PI*sigma);
	}
    }
}


RbfInterpolator::RbfInterpolator (size_t n, const Imath::V3f p[/*n*/][2]):
    _samplePts (n),
    _numSamples (n),
    _lambdas (3*_numSamples),
    _sigmas (_numSamples),
    _affine (12),
    _pointTree (0)
{
    for ( size_t s = 0; s < _numSamples; s++)
	_samplePts[s] = p[s][0];

    std::vector<double> b(3*_numSamples);
    std::vector<double> bX(_numSamples);
    std::vector<double> bY(_numSamples);
    std::vector<double> bZ(_numSamples);
    std::vector<double> valSparse;
    std::vector<size_t> colInd;
    std::vector<size_t> rowPts;
    
    // Solve for affine term    
    rowPts.push_back(0);	    

    size_t endRow = 0;
    for ( size_t s = 0; s < _numSamples; s++)
    {
	valSparse.push_back (p[s][0][0]);
	colInd.push_back (0);
	valSparse.push_back (p[s][0][1]);
	colInd.push_back (1);
	valSparse.push_back (p[s][0][2]);
	colInd.push_back (2);
	valSparse.push_back (1.);
	colInd.push_back (3);
	
	endRow += 4;	
	rowPts.push_back(endRow);	    
	b[3*s+0] = p[s][1][0];

	valSparse.push_back (p[s][0][0]);
	colInd.push_back (4);
	valSparse.push_back (p[s][0][1]);
	colInd.push_back (5);
	valSparse.push_back (p[s][0][2]);
	colInd.push_back (6);
	valSparse.push_back (1.);
	colInd.push_back (7);
	
	endRow += 4;	
	rowPts.push_back(endRow);	    
	b[3*s+1] = p[s][1][1];

	valSparse.push_back (p[s][0][0]);
	colInd.push_back (8);
	valSparse.push_back (p[s][0][1]);
	colInd.push_back (9);
	valSparse.push_back (p[s][0][2]);
	colInd.push_back (10);
	valSparse.push_back (1.);
	colInd.push_back (11);
	
	endRow += 4;	
	rowPts.push_back(endRow);	 
	b[3*s+2] = p[s][1][2];	
    }
    CRSOperator<double> OMAAff(valSparse, colInd, rowPts, 12);
    
    LSSCG<double, CRSOperator<double> > lssAffine(OMAAff);

    fill(_affine.begin(), _affine.end(), 0.0);
    
    lssAffine.solver.maxNumIterations = 12;
    lssAffine.solver.tolerance = 1.e-7;
        
    lssAffine(b.begin(), b.end(), _affine.begin(), _affine.end());

    // Fit residual with Rbf
	
    // Use k-d tree for fast spatial queries
    _pointTree = new PointTree (&_samplePts[0], _numSamples);

    // Compute spread for each kernel
    size_t numNeighbor = 4;

    _maxSigma = .0;    
    for ( size_t i = 0; i < _numSamples; i++) 
    {	
	std::vector <size_t> indices;

	_pointTree->nearestPoints (_samplePts[i], numNeighbor, indices);
	
	double sum = .0;
	for (size_t n = 0; n < indices.size(); n++)
	{
	    size_t nidx = indices[n];
	    double delta[3];
	    
	    delta[0] = _samplePts[i][0] - _samplePts[nidx][0];
	    delta[1] = _samplePts[i][1] - _samplePts[nidx][1];
	    delta[2] = _samplePts[i][2] - _samplePts[nidx][2];

	    sum += delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
	}
	
	_sigmas[i] = .5f * sqrt(sum);

	if ( _sigmas[i] > _maxSigma)
	    _maxSigma = _sigmas[i];
    }      

#ifdef DEBUG_RBF
    std::cout << "Sigmas" << std::endl;
    for ( size_t i = 0; i < _numSamples; i++) 
    {	
	std::cout << _sigmas[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "MaxSigma = " << _maxSigma << std::endl;
#endif

    // Solve for the weights (lambdas) using a sparse solver
    valSparse.clear();
    colInd.clear();
    rowPts.clear();
    
    rowPts.push_back(0);	    

    endRow = 0;
    for ( size_t s = 0; s < _numSamples; s++)
    {
	std::vector <size_t> indices;
	
	Imath::V3f center = _samplePts[s];
	_pointTree->intersect(center, 2.*_maxSigma, indices);
		
	size_t nonZero = 0;
	for (size_t n = 0; n < indices.size(); n++)
	{
	    size_t nidx = indices[n];
	    double dist = (center - _samplePts[nidx]).length();
	    double weight = kernel (dist, _sigmas[nidx]);
	    
	    if (weight > .0) 
	    {
		valSparse.push_back (weight);
		colInd.push_back (nidx);
		nonZero++;
	    }
	}

	// fit data minus estimated affine function
	bX[s] = p[s][1][0] - (_affine[0]*p[s][0][0] + _affine[1]*p[s][0][1] + _affine[2]*p[s][0][2] + _affine[3]);
	bY[s] = p[s][1][1] - (_affine[4]*p[s][0][0] + _affine[5]*p[s][0][1] + _affine[6]*p[s][0][2] + _affine[7]);
	bZ[s] = p[s][1][2] - (_affine[8]*p[s][0][0] + _affine[9]*p[s][0][1] + _affine[10]*p[s][0][2] + _affine[11]);
	
	endRow += nonZero;
	
	if (endRow > 0)
	    rowPts.push_back(endRow);	    
    }

    CRSOperator<double> OMA(valSparse, colInd, rowPts, _numSamples);
    
    LSSCG<double, CRSOperator<double> > lss(OMA);

    std::vector<double> solX(_numSamples, 0.0);
    std::vector<double> solY(_numSamples, 0.0);
    std::vector<double> solZ(_numSamples, 0.0);
    
    lss.solver.maxNumIterations = 30*_numSamples;
    lss.solver.tolerance = 1.e-7;
    
#ifndef DEBUG_RBF
    lss(bX.begin(), bX.end(), solX.begin(), solX.end());
    lss(bY.begin(), bY.end(), solY.begin(), solY.end());
    lss(bZ.begin(), bZ.end(), solZ.begin(), solZ.end());        
#else
    std::vector<double> s(_numSamples);
    double tolX = lss(bX.begin(), bX.end(), solX.begin(), solX.end());
    double tolY = lss(bY.begin(), bY.end(), solY.begin(), solY.end());
    double tolZ = lss(bZ.begin(), bZ.end(), solZ.begin(), solZ.end());    
    
    std::cout << "bX\n";
    copy(bX.begin(), bX.end(), ostream_iterator<double>(std::cout, " "));
    cout << "\nsX\n";
    OMA.apply(solX.begin(), solX.end(), s.begin(), s.end());    
    copy(s.begin(), s.end(), ostream_iterator<double>(cout, " "));

    std::cout << "\n\nbY\n";
    copy(bY.begin(), bY.end(), ostream_iterator<double>(std::cout, " "));
    cout << "\nsY\n";
    OMA.apply(solY.begin(), solY.end(), s.begin(), s.end());    
    copy(s.begin(), s.end(), ostream_iterator<double>(cout, " "));

    std::cout << "\n\nbZ\n";
    copy(bZ.begin(), bZ.end(), ostream_iterator<double>(std::cout, " "));
    cout << "\nsZ\n";
    OMA.apply(solZ.begin(), solZ.end(), s.begin(), s.end());    
    copy(s.begin(), s.end(), ostream_iterator<double>(cout, " "));


    std::cout << "\n\nTolerance " << tolX << " " << tolY << " " << tolZ <<
    std::endl;
#endif

    for (size_t s = 0; s < _numSamples; s++) 
    {
	_lambdas[3*s+0] = solX[s];
	_lambdas[3*s+1] = solY[s];
	_lambdas[3*s+2] = solZ[s];
    }
}


RbfInterpolator::~RbfInterpolator ()
{
    delete _pointTree;
}


Imath::V3f
RbfInterpolator::value (const Imath::V3f &x) const
{
    std::vector <size_t> indices;
    _pointTree->intersect(x, 2.*_maxSigma, indices);
    
    double sumX = .0;
    double sumY = .0;
    double sumZ = .0;

    for (size_t n = 0; n < indices.size(); n++)
    {
	size_t nidx = indices[n];
	
	double weight = kernel((_samplePts[nidx] - x).length(), 
			       _sigmas[nidx]);
	
	sumX += weight*_lambdas[3*nidx];
	sumY += weight*_lambdas[3*nidx+1];
	sumZ += weight*_lambdas[3*nidx+2];
    }    

    sumX += _affine[0]*x[0] + _affine[1]*x[1] + _affine[2]*x[2] + _affine[3];
    sumY += _affine[4]*x[0] + _affine[5]*x[1] + _affine[6]*x[2] + _affine[7];
    sumZ += _affine[8]*x[0] + _affine[9]*x[1] + _affine[10]*x[2] + _affine[11];

    return Imath::V3f(sumX, sumY, sumZ);
}


Imath::V3f
RbfInterpolator::gradient (const Imath::V3f &x) const
{
    std::vector <size_t> indices;
    _pointTree->intersect(x, 2.*_maxSigma, indices);
    
    double sumX = .0;
    double sumY = .0;
    double sumZ = .0;
    
    for (size_t n = 0; n < indices.size(); n++)
    {
	size_t nidx = indices[n];
	
	double weight = kernelGrad((_samplePts[nidx] - x).length(), 
				   _sigmas[nidx]);
	
	sumX += weight*_lambdas[3*nidx];
	sumY += weight*_lambdas[3*nidx+1];
	sumZ += weight*_lambdas[3*nidx+2];
    }    
    
    return Imath::V3f(sumX, sumY, sumZ);
}

} // namespace Ctl
