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
//	Color space conversions
//
//	For an explanation of how the RGB to XYZ conversion matrix is derived,
//	see Roy Hall, "Illumination and Color in Computer Generated Imagery",
//	Springer-Verlag, 1989, chapter 3, "Perceptual Response"; and 
//	Charles A. Poynton, "A Technical Introduction to Digital Video",
//	John Wiley & Sons, 1996, chapter 7, "Color science for video".
//
//	The conversions between XYZ and L*u*v* and between XYZ and L*a*b*
//	are taken from chapter 1 of "Digital Color Imaging Handbook", by
//	Gaurav Sharma (ed.), CRC Press, 2003.
//
//-----------------------------------------------------------------------------

#include <CtlColorSpace.h>

using namespace Imath;

namespace Ctl {

M44f
RGBtoXYZ (const Chromaticities &chroma, float Y)
{
    //
    // X and Z values of RGB value (1, 1, 1), or "white"
    //

    float X = chroma.white.x * Y / chroma.white.y;
    float Z = (1 - chroma.white.x - chroma.white.y) * Y / chroma.white.y;

    //
    // Scale factors for matrix rows
    //

    float d = chroma.red.x   * (chroma.blue.y  - chroma.green.y) +
	      chroma.blue.x  * (chroma.green.y - chroma.red.y) +
	      chroma.green.x * (chroma.red.y   - chroma.blue.y);

    float Sr = (X * (chroma.blue.y - chroma.green.y) -
	        chroma.green.x * (Y * (chroma.blue.y - 1) +
		chroma.blue.y  * (X + Z)) +
		chroma.blue.x  * (Y * (chroma.green.y - 1) +
		chroma.green.y * (X + Z))) / d;

    float Sg = (X * (chroma.red.y - chroma.blue.y) +
		chroma.red.x   * (Y * (chroma.blue.y - 1) +
		chroma.blue.y  * (X + Z)) -
		chroma.blue.x  * (Y * (chroma.red.y - 1) +
		chroma.red.y   * (X + Z))) / d;

    float Sb = (X * (chroma.green.y - chroma.red.y) -
		chroma.red.x   * (Y * (chroma.green.y - 1) +
		chroma.green.y * (X + Z)) +
		chroma.green.x * (Y * (chroma.red.y - 1) +
		chroma.red.y   * (X + Z))) / d;

    //
    // Assemble the matrix
    //

    Imath::M44f M;

    M[0][0] = Sr * chroma.red.x;
    M[0][1] = Sr * chroma.red.y;
    M[0][2] = Sr * (1 - chroma.red.x - chroma.red.y);

    M[1][0] = Sg * chroma.green.x;
    M[1][1] = Sg * chroma.green.y;
    M[1][2] = Sg * (1 - chroma.green.x - chroma.green.y);

    M[2][0] = Sb * chroma.blue.x;
    M[2][1] = Sb * chroma.blue.y;
    M[2][2] = Sb * (1 - chroma.blue.x - chroma.blue.y);

    return M;
}


M44f
XYZtoRGB (const Chromaticities &chroma, float Y)
{
    return RGBtoXYZ (chroma, Y).inverse();
}

namespace {

float
f (float x)
{
    if (x > 0.008856f)
	return pow (x, 1.f / 3.f);
    else
	return 7.787f * x + 16.f / 116.f;
}


float
fInverse (float t)
{
    if (t > 0.206893f)
	return t * t * t;
    else
	return 1.f / 7.787f * (t - 16.f / 116.f);
}


float
uprime (const V3f &XYZ)
{
    return (4 * XYZ.x) / (XYZ.x + 15 * XYZ.y + 3 * XYZ.z);
}


float
vprime (const V3f &XYZ)
{
    return (9 * XYZ.y) / (XYZ.x + 15 * XYZ.y + 3 * XYZ.z);
}


} // namespace


Imath::V3f
XYZtoLuv (const Imath::V3f &XYZ, const Imath::V3f &XYZn)
{
    float Lstar = 116 * f (XYZ.y / XYZn.y) - 16;
    float ustar = 13 * Lstar * (uprime (XYZ) - uprime (XYZn));
    float vstar = 13 * Lstar * (vprime (XYZ) - vprime (XYZn));

    return V3f (Lstar, ustar, vstar);
}


Imath::V3f
LuvtoXYZ (const Imath::V3f &Luv, const Imath::V3f &XYZn)
{
    float Lstar = Luv.x;
    float ustar = Luv.y;
    float vstar = Luv.z;
    float unprime = uprime (XYZn);
    float vnprime = vprime (XYZn);

    float fY = (Lstar + 16) / 116;
    float Y = XYZn.y * fInverse (fY);
    float d = 4 * (13 * Lstar * vnprime + vstar);
    float X = 9 * (13 * Lstar * unprime + ustar) * Y / d;
    float Z = -(3 * ustar + 13 * Lstar *
	      (-12 + 3 * unprime + 20 * vnprime) + 20 * vstar) * Y / d;

    return V3f (X, Y, Z);
}


Imath::V3f
XYZtoLab (const Imath::V3f &XYZ, const Imath::V3f &XYZn)
{
    float Lstar = 116 * f (XYZ.y / XYZn.y) - 16;
    float astar = 500 * (f (XYZ.x / XYZn.x) - f (XYZ.y / XYZn.y));
    float bstar = 200 * (f (XYZ.y / XYZn.y) - f (XYZ.z / XYZn.z));

    return V3f (Lstar, astar, bstar);
}


Imath::V3f
LabtoXYZ (const Imath::V3f &Lab, const Imath::V3f &XYZn)
{
    float Lstar = Lab.x;
    float astar = Lab.y;
    float bstar = Lab.z;

    float fY = (Lstar + 16) / 116;
    float fX = astar / 500 + fY;
    float fZ = fY - bstar / 200;

    float X = XYZn.x * fInverse (fX);
    float Y = XYZn.y * fInverse (fY);
    float Z = XYZn.z * fInverse (fZ);

    return V3f (X, Y, Z);
}

} // namespace Ctl
