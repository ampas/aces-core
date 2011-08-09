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
//	1D and 3D table lookups with linear and trilinear interpolation.
//
//-----------------------------------------------------------------------------

#include <CtlLookupTable.h>
#include <ImathFun.h>

using namespace Imath;

namespace Ctl {
namespace {

inline void
indicesAndWeights (float r, int iMax, int &i, int &i1, float &u, float &u1)
{
    if (r >= 0)
    {
	if (r < iMax)
	{
	    //
	    // r is finite and in the interval [0, iMax[
	    //

	    i = int (r);
	    i1 = i + 1;
	    u  = r - i;
	}
	else
	{
	    //
	    // r is greater than or equal to iMax
	    //

	    i = i1 = iMax;
	    u = 1;
	}
    }
    else
    {
	//
	// r is either NaN or less than 0
	//

	i = i1 = 0;
	u = 1;
    }

    u1 = 1 - u;
}

} // namespace


float
lookup1D
    (const float table[],
     int size,
     float pMin,
     float pMax,
     float p)
{
    int iMax = size - 1;
    float r = (clamp (p, pMin, pMax) - pMin) / (pMax - pMin) * iMax;

    int i, i1;
    float u, u1;
    indicesAndWeights (r, iMax, i, i1, u, u1);

    return table[i] * u1 + table[i1] * u;
}


float
lookupCubic1D
    (const float table[],
     int size,
     float pMin,
     float pMax,
     float p)
{
    if (size < 3)
	return lookup1D (table, size, pMin, pMax, p);

    int iMax = size - 1;
    float r = (clamp (p, pMin, pMax) - pMin) / (pMax - pMin) * iMax;
    int i;

    if (r >= 0 && r < iMax)
    {
	//
	// r is finite and in the interval [0, iMax[
	//

	i = int (r);
    }
    else if (r >= iMax)
    {
	//
	// r is greater than or equal to iMax
	//

	return table[iMax];
    }
    else
    {
	//
	// r is either NaN or less than 0
	//

	return table[0];
    }

    float dy = (table[i+1] - table[i]);
    float m0, m1;

    if (i > 0)
	m0 = (dy + (table[i] - table[i-1])) * 0.5f;

    if (i < size - 2)
	m1 = (dy + (table[i+2] - table[i+1])) * 0.5f;

    if (i <= 0)
	m0 = (3 * dy - m1) * 0.5f;

    if (i >= size - 2)
	m1 = (3 * dy - m0) * 0.5f;

    float t = r - i;
    float t2 = t * t;
    float t3 = t2 * t;

    return table[i] * (2 * t3 - 3 * t2 + 1) +
           m0 * (t3 - 2 * t2 + t) +
	   table[i+1] * (-2 * t3 + 3 * t2) +
	   m1 * (t3 - t2);
}


V3f
lookup3D
    (const V3f table[],
     const V3i &size,
     const V3f &pMin,
     const V3f &pMax,
     const V3f &p)
{
    int iMax = size.x - 1;
    float r = (clamp (p.x, pMin.x, pMax.x) - pMin.x) / (pMax.x - pMin.x) * iMax;

    int i, i1;
    float u, u1;
    indicesAndWeights (r, iMax, i, i1, u, u1);

    int jMax = size.y - 1;
    float s = (clamp (p.y, pMin.y, pMax.y) - pMin.y) / (pMax.y - pMin.y) * jMax;

    int j, j1;
    float v, v1;
    indicesAndWeights (s, jMax, j, j1, v, v1);

    int kMax = size.z - 1;
    float t = (clamp (p.z, pMin.z, pMax.z) - pMin.z) / (pMax.z - pMin.z) * kMax;

    int k, k1;
    float w, w1;
    indicesAndWeights (t, kMax, k, k1, w, w1);

    const V3f &a = table[(i  * size.y + j ) * size.z + k ];
    const V3f &b = table[(i1 * size.y + j ) * size.z + k ];
    const V3f &c = table[(i  * size.y + j1) * size.z + k ];
    const V3f &d = table[(i1 * size.y + j1) * size.z + k ];
    const V3f &e = table[(i  * size.y + j ) * size.z + k1];
    const V3f &f = table[(i1 * size.y + j ) * size.z + k1];
    const V3f &g = table[(i  * size.y + j1) * size.z + k1];
    const V3f &h = table[(i1 * size.y + j1) * size.z + k1];

    return w1 * (v1 * (u1 * a + u * b) + v * (u1 * c + u * d)) +
	   w  * (v1 * (u1 * e + u * f) + v * (u1 * g + u * h));
}


float	
interpolate1D
    (const float table[][2],
     int size,
     float p)
{
    if (size < 1)
	return 0;

    if (p < table[0][0])
	return table[0][1];

    if (p >= table[size-1][0])
	return table[size-1][1];

    int i = 0;
    int j = size;

    while (i < j - 1)
    {
	int k = (i + j) / 2;

	if (table[k][0] == p)
	    return table[k][1];
	else if (table[k][0] < p)
	    i = k;
	else
	    j = k;
    }

    float t = (p - table[i][0]) / (table[i+1][0] - table[i][0]);
    float s = 1 - t;

    return s * table[i][1] + t * table[i+1][1];
}


float	
interpolateCubic1D
    (const float table[][2],
     int size,
     float p)
{
    if (size < 3)
	return interpolate1D (table, size, p);

    if (p < table[0][0])
	return table[0][1];

    if (p >= table[size-1][0])
	return table[size-1][1];

    size_t i = 0;
    size_t j = size;

    while (i < j - 1)
    {
	size_t k = (i + j) / 2;

	if (table[k][0] == p)
	    return table[k][1];
	else if (table[k][0] < p)
	    i = k;
	else
	    j = k;
    }

    float dx = (table[i+1][0] - table[i][0]);
    float dy = (table[i+1][1] - table[i][1]);
    float m0, m1;

    if (i > 0)
    {
	m0 = 0.5f * (dy + dx * (table[i][1] - table[i-1][1]) /
			       (table[i][0] - table[i-1][0]));
    }

    if ((int)i < size - 2)
    {
	m1 = 0.5f * (dy + dx * (table[i+2][1] - table[i+1][1]) /
			       (table[i+2][0] - table[i+1][0]));
    }

    if (i <= 0)
    {
	m0 = (3 * dy - m1) * 0.5f;
    }

    if ((int)i >= size - 2)
    {
	m1 = (3 * dy - m0) * 0.5f;
    }

    float t = (p - table[i][0]) / dx;
    float t2 = t * t;
    float t3 = t2 * t;

    return table[i][1] * (2 * t3 - 3 * t2 + 1) +
           m0 * (t3 - 2 * t2 + t) +
	   table[i+1][1] * (-2 * t3 + 3 * t2) +
	   m1 * (t3 - t2);
}

} // namespace Ctl
