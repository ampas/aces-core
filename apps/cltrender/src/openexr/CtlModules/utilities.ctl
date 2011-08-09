///////////////////////////////////////////////////////////////////////////////
// 
// Copyright (c) 2007 Academy of Motion Picture Arts and Sciences
// ("A.M.P.A.S."). Portions contributed by others as indicated.
// All rights reserved.
// 
// A world-wide, royalty-free, non-exclusive right to distribute, copy,
// modify, create derivatives, and use, in source and binary forms, is
// hereby granted, subject to acceptance of this license. Performance of
// any of the aforementioned acts indicates acceptance to be bound by the
// following terms and conditions:
// 
//   * Redistributions of source code, in whole or in part, must
//     retain the above copyright notice, this list of conditions and
//     the Disclaimer of Warranty.
// 
//   * Redistributions in binary form must retain the above copyright
//     notice, this list of conditions and the Disclaimer of Warranty
//     in the documentation and/or other materials provided with the
//     distribution.
// 
//   * Nothing in this license shall be deemed to grant any rights to
//     trademarks, copyrights, patents, trade secrets or any other
//     intellectual property of A.M.P.A.S. or any contributors, except
//     as expressly stated herein.  Neither the name of. A.M.P.A.S. nor
//     any other contributors to this software may be used to endorse or
//     promote products derivative or, or based on this software without
//     express prior written permission of A.M.P.A.S. or contributor, as
//     appropriate.
// 
//   * This license shall be construed pursuant to the laws of the State
//     of California, and any disputes related thereto shall be subject
//     to the jurisdiction of the courts therein.
// 
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT ARE DISCLAIMED. IN NO
// EVENT SHALL A.M.P.A.S., OR ANY CONTRIBUTORS OR DISTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
// RESITUTIONARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
///////////////////////////////////////////////////////////////////////////////

//
// Data and functions that may be useful for writing CTL programs.
//

const Chromaticities rec709Chromaticities =
{
    {0.6400, 0.3300},
    {0.3000, 0.6000},
    {0.1500, 0.0600},
    {0.3172, 0.3290}
};

float[3]
convertRGBtoXYZ
    (Chromaticities chromaticities,
     float whiteLuminance,
     float R,
     float G,
     float B)
{
    float M[4][4] = RGBtoXYZ (chromaticities, whiteLuminance);
    float RGB[3] = {R, G, B};
    return mult_f3_f44 (RGB, M);
}

void
convertRGBtoXYZ_f
    (Chromaticities chromaticities,
     float whiteLuminance,
     float RGB[3],
     output float X,
     output float Y,
     output float Z)
{
    float M[4][4] = RGBtoXYZ (chromaticities, whiteLuminance);
    float XYZ[3] = mult_f3_f44 (RGB, M);
    X = XYZ[0];
    Y = XYZ[1];
    Z = XYZ[2];
}

void
convertRGBtoXYZ_h
    (Chromaticities chromaticities,
     float whiteLuminance,
     float RGB[3],
     output half X,
     output half Y,
     output half Z)
{
    float M[4][4] = RGBtoXYZ (chromaticities, whiteLuminance);
    float XYZ[3] = mult_f3_f44 (RGB, M);
    X = XYZ[0];
    Y = XYZ[1];
    Z = XYZ[2];
}

void
convertXYZtoRGB_f
    (Chromaticities chromaticities,
     float whiteLuminance,
     float XYZ[3],
     output float R,
     output float G,
     output float B)
{
    float M[4][4] = XYZtoRGB (chromaticities, whiteLuminance);
    float RGB[3] = mult_f3_f44 (XYZ, M);
    R = RGB[0];
    G = RGB[1];
    B = RGB[2];
}

void
convertXYZtoRGB_h
    (Chromaticities chromaticities,
     float whiteLuminance,
     float XYZ[3],
     output half R,
     output half G,
     output half B)
{
    float M[4][4] = XYZtoRGB (chromaticities, whiteLuminance);
    float RGB[3] = mult_f3_f44 (XYZ, M);
    R = RGB[0];
    G = RGB[1];
    B = RGB[2];
} 
