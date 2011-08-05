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

#ifndef INCLUDED_CTL_COLOR_SPACE_H
#define INCLUDED_CTL_COLOR_SPACE_H

//-----------------------------------------------------------------------------
//
//	Color space conversions
//
//	RGB to CIE XYZ:
//
//	    Given a set of chromaticities, c, and the luminance, Y, of the
//	    RGB triple (1,1,1), or "white", RGBtoXYZ(c,Y) computes a matrix,
//	    M, so that multiplying an RGB value, v, with M produces an
//	    equivalent XYZ value, w.  (w == v * M)
//     
//	    If we define that
//     
//	       (Xr, Yr, Zr) == (1, 0, 0) * M
//	       (Xg, Yg, Zg) == (0, 1, 0) * M
//	       (Xb, Yb, Zb) == (0, 0, 1) * M
//	       (Xw, Yw, Zw) == (1, 1, 1) * M,
//     
//	    then the following statements are true:
//     
//	       Xr / (Xr + Yr + Zr) == c.red.x
//	       Yr / (Xr + Yr + Zr) == c.red.y
//     
//	       Xg / (Xg + Yg + Zg) == c.red.x
//	       Yg / (Xg + Yg + Zg) == c.red.y
//     
//	       Xb / (Xb + Yb + Zb) == c.red.x
//	       Yb / (Xb + Yb + Zb) == c.red.y
//     
//	       Xw / (Xw + Yw + Zw) == c.red.x
//	       Yw / (Xw + Yw + Zw) == c.red.y
//     
//	       Yw == Y.
//     
//	CIE XYZ to RGB:
//     
//	    XYZtoRGB(c,Y) returns RGBtoXYZ(c,Y).inverse().
//
//	XYZ to CIE L*u*v*:
//
//	    Given a CIE XYZ tristimulus, XYZ, and a white stimulus,
//	    XYZn, XYZtoLuv(XYZ,XYZn) converts XYZ to an equivalent
//	    CIE L*u*v* triple.
//
//	CIE L*u*v* to XYZ:
//
//	    Given a CIE L*u*v* triple, Luv, and a white stimulus,
//	    XYZn, LuvtoXYZ(Luv,XYZn) converts Luv to an equivalent
//	    CIE XYZ tristimulus.
//
//	XYZ to CIE L*a*b*:
//
//	    Given a CIE XYZ tristimulus, XYZ, and a white stimulus,
//	    XYZn, XYZtoLab(XYZ,XYZn) converts XYZ to an equivalent
//	    CIE L*a*b* triple.
//
//	CIE L*a*b* to XYZ:
//
//	    Given a CIE L*a*b* triple, Lab, and a white stimulus,
//	    XYZn, LabtoXYZ(Lab,XYZn) converts Lab to an equivalent
//	    CIE XYZ tristimulus.
//
//-----------------------------------------------------------------------------

#include <ImathMatrix.h>

namespace Ctl {

struct Chromaticities
{
    Imath::V2f	red;		// CIE xy coordinates of red primary
    Imath::V2f	green;		// CIE xy coordinates of green primary
    Imath::V2f	blue;		// CIE xy coordinates of blue primary
    Imath::V2f	white;		// CIE xy coordinates of white point
};


Imath::M44f	RGBtoXYZ (const Chromaticities &chroma, float Y);
Imath::M44f	XYZtoRGB (const Chromaticities &chroma, float Y);
Imath::V3f	XYZtoLuv (const Imath::V3f &XYZ, const Imath::V3f &XYZn);
Imath::V3f	LuvtoXYZ (const Imath::V3f &Luv, const Imath::V3f &XYZn);
Imath::V3f	XYZtoLab (const Imath::V3f &XYZ, const Imath::V3f &XYZn);
Imath::V3f	LabtoXYZ (const Imath::V3f &Lab, const Imath::V3f &XYZn);


} // namespace Ctl

#endif
