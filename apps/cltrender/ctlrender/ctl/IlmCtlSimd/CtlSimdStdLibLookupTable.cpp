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
//	The Standard Library of C++ functions that can be called from CTL
//	
//	- interpolated 1D and 3D table lookups
//
//-----------------------------------------------------------------------------

#include <CtlSimdStdLibLookupTable.h>
#include <CtlSimdStdLibTemplates.h>
#include <CtlSimdStdLibrary.h>
#include <CtlSimdStdTypes.h>
#include <CtlSimdCFunc.h>
#include <CtlLookupTable.h>
#include <half.h>
#include <cmath>
#include <cassert>
#include <fstream>

using namespace Imath;
using namespace std;

namespace Ctl {
namespace {

typedef float (*Lookup1DFunc) (const float[], int, float, float, float);


void
simdDoLookup1D
    (const SimdBoolMask &mask,
     SimdXContext &xcontext,
     Lookup1DFunc func)
{
    //
    // float func (float table[], float pMin, float pMax, float p)
    //

    const SimdReg &size  = xcontext.stack().regFpRelative (-1);
    const SimdReg &table = xcontext.stack().regFpRelative (-2);
    const SimdReg &pMin  = xcontext.stack().regFpRelative (-3);
    const SimdReg &pMax  = xcontext.stack().regFpRelative (-4);
    const SimdReg &p     = xcontext.stack().regFpRelative (-5);
    SimdReg &returnValue = xcontext.stack().regFpRelative (-6);

    assert (!size.isVarying());
    int s = *(int *)(size[0]);

    if (table.isVarying() ||
	pMin.isVarying() ||
	pMax.isVarying() ||
	p.isVarying())
    {
	returnValue.setVarying (true);

	if (!mask.isVarying() &&
	    !table.isVarying() &&
	    !pMin.isVarying() &&
	    !pMax.isVarying())
	{
	    //
	    // Fast path -- only p is varying, everything else is uniform.
	    //

	    float *table0 = (float *)(table[0]);
	    float pMin0 = *(float *)(pMin[0]);
	    float pMax0 = *(float *)(pMax[0]);

	    for (int i = xcontext.regSize(); --i >= 0;)
	    {
		*(float *)(returnValue[i]) = func (table0,
						   s,
						   pMin0,
						   pMax0,
						   *(float *)(p[i]));
	    }
	}
	else
	{
	    for (int i = xcontext.regSize(); --i >= 0;)
	    {
		if (mask[i])
		{
		    *(float *)(returnValue[i]) = func ((float *)(table[i]), 
						       s,
						       *(float *)(pMin[i]),
						       *(float *)(pMax[i]),
						       *(float *)(p[i]));
		}
	    }
	}
    }
    else
    {
	returnValue.setVarying (false);

	*(float *)(returnValue[0]) = func ((float *)(table[0]), 
					   s,
					   *(float *)(pMin[0]),
					   *(float *)(pMax[0]),
					   *(float *)(p[0]));
    }
}


void
simdLookup1D (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // float lookup1D (float table[], float pMin, float pMax, float p)
    //

    simdDoLookup1D (mask, xcontext, lookup1D);
}


void
simdLookupCubic1D (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // float lookupCubic1D (float table[], float pMin, float pMax, float p)
    //

    simdDoLookup1D (mask, xcontext, lookupCubic1D);
}


void
simdLookup3D_f3 (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // float[3] lookup3D_f3 (float table[][][][3],
    //			     float pMin[3], float pMax[3],
    //			     float p[3])
    //

    const SimdReg &size2  = xcontext.stack().regFpRelative (-1);
    const SimdReg &size1  = xcontext.stack().regFpRelative (-2);
    const SimdReg &size0  = xcontext.stack().regFpRelative (-3);
    const SimdReg &table  = xcontext.stack().regFpRelative (-4);
    const SimdReg &pMin   = xcontext.stack().regFpRelative (-5);
    const SimdReg &pMax   = xcontext.stack().regFpRelative (-6);
    const SimdReg &p      = xcontext.stack().regFpRelative (-7);
    SimdReg &returnValue  = xcontext.stack().regFpRelative (-8);

    assert (!size0.isVarying() && !size1.isVarying() && !size2.isVarying());

    V3i s (*(int *)(size0[0]),
	   *(int *)(size1[0]),
	   *(int *)(size2[0]));

    if (table.isVarying() ||
	pMin.isVarying() ||
	pMax.isVarying() ||
	p.isVarying())
    {
	returnValue.setVarying (true);

	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    if (mask[i])
	    {
		*(V3f *)(returnValue[i]) = lookup3D ((V3f *)(table[i]), 
						     s,
						     *(V3f *)(pMin[i]),
						     *(V3f *)(pMax[i]),
						     *(V3f *)(p[i]));
	    }
	}
    }
    else
    {
	returnValue.setVarying (false);

	*(V3f *)(returnValue[0]) = lookup3D ((V3f *)(table[0]), 
					     s,
					     *(V3f *)(pMin[0]),
					     *(V3f *)(pMax[0]),
					     *(V3f *)(p[0]));
    }
}


void
simdLookup3D_f (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // void lookup3D_f (float table[][][][3],
    //		        float pMin[3], float pMax[3],
    //		        float p0, float p1, float p2,
    //		        float q0, float q1, float q2)
    //

    const SimdReg &size2  = xcontext.stack().regFpRelative (-1);
    const SimdReg &size1  = xcontext.stack().regFpRelative (-2);
    const SimdReg &size0  = xcontext.stack().regFpRelative (-3);
    const SimdReg &table  = xcontext.stack().regFpRelative (-4);
    const SimdReg &pMin   = xcontext.stack().regFpRelative (-5);
    const SimdReg &pMax   = xcontext.stack().regFpRelative (-6);
    const SimdReg &p0     = xcontext.stack().regFpRelative (-7);
    const SimdReg &p1     = xcontext.stack().regFpRelative (-8);
    const SimdReg &p2     = xcontext.stack().regFpRelative (-9);
    SimdReg &q0           = xcontext.stack().regFpRelative (-10);
    SimdReg &q1           = xcontext.stack().regFpRelative (-11);
    SimdReg &q2           = xcontext.stack().regFpRelative (-12);

    assert (!size0.isVarying() && !size1.isVarying() && !size2.isVarying());

    V3i s (*(int *)(size0[0]),
	   *(int *)(size1[0]),
	   *(int *)(size2[0]));

    if (table.isVarying() ||
	pMin.isVarying() ||
	pMax.isVarying() ||
	p0.isVarying() ||
	p1.isVarying() ||
	p2.isVarying())
    {
	q0.setVarying (true);
	q1.setVarying (true);
	q2.setVarying (true);

	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    if (mask[i])
	    {
		V3f p (*(float *)p0[i], *(float *)p1[i], *(float *)p2[i]);

		V3f q = lookup3D ((V3f *)(table[i]), 
				  s,
				  *(V3f *)(pMin[i]),
				  *(V3f *)(pMax[i]),
				  p);

		*(float *)q0[i] = q[0];
		*(float *)q1[i] = q[1];
		*(float *)q2[i] = q[2];
	    }
	}
    }
    else
    {
	q0.setVarying (false);
	q1.setVarying (false);
	q2.setVarying (false);

	V3f p (*(float *)p0[0], *(float *)p1[0], *(float *)p2[0]);

	V3f q = lookup3D ((V3f *)(table[0]), 
			  s,
			  *(V3f *)(pMin[0]),
			  *(V3f *)(pMax[0]),
			  p);

	*(float *)q0[0] = q[0];
	*(float *)q1[0] = q[1];
	*(float *)q2[0] = q[2];
    }
}


void
simdLookup3D_h (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // void lookup3D_h (float table[][][][3],
    //		        float pMin[3], float pMax[3],
    //		        half p0, half p1, half p2,
    //		        half q0, half q1, half q2)
    //

    const SimdReg &size2  = xcontext.stack().regFpRelative (-1);
    const SimdReg &size1  = xcontext.stack().regFpRelative (-2);
    const SimdReg &size0  = xcontext.stack().regFpRelative (-3);
    const SimdReg &table  = xcontext.stack().regFpRelative (-4);
    const SimdReg &pMin   = xcontext.stack().regFpRelative (-5);
    const SimdReg &pMax   = xcontext.stack().regFpRelative (-6);
    const SimdReg &p0     = xcontext.stack().regFpRelative (-7);
    const SimdReg &p1     = xcontext.stack().regFpRelative (-8);
    const SimdReg &p2     = xcontext.stack().regFpRelative (-9);
    SimdReg &q0           = xcontext.stack().regFpRelative (-10);
    SimdReg &q1           = xcontext.stack().regFpRelative (-11);
    SimdReg &q2           = xcontext.stack().regFpRelative (-12);

    assert (!size0.isVarying() && !size1.isVarying() && !size2.isVarying());

    V3i s (*(int *)(size0[0]),
	   *(int *)(size1[0]),
	   *(int *)(size2[0]));

    if (table.isVarying() ||
	pMin.isVarying() ||
	pMax.isVarying() ||
	p0.isVarying() ||
	p1.isVarying() ||
	p2.isVarying())
    {
	q0.setVarying (true);
	q1.setVarying (true);
	q2.setVarying (true);

	for (int i = xcontext.regSize(); --i >= 0;)
	{
	    if (mask[i])
	    {
		V3f p (*(half *)p0[i], *(half *)p1[i], *(half *)p2[i]);

		V3f q = lookup3D ((V3f *)(table[i]), 
				  s,
				  *(V3f *)(pMin[i]),
				  *(V3f *)(pMax[i]),
				  p);

		*(half *)q0[i] = q[0];
		*(half *)q1[i] = q[1];
		*(half *)q2[i] = q[2];
	    }
	}
    }
    else
    {
	q0.setVarying (false);
	q1.setVarying (false);
	q2.setVarying (false);

	V3f p (*(half *)p0[0], *(half *)p1[0], *(half *)p2[0]);

	V3f q = lookup3D ((V3f *)(table[0]), 
			  s,
			  *(V3f *)(pMin[0]),
			  *(V3f *)(pMax[0]),
			  p);

	*(half *)q0[0] = q[0];
	*(half *)q1[0] = q[1];
	*(half *)q2[0] = q[2];
    }
}


typedef float (*Interpolate1DFunc) (const float[][2], int, float);


void
simdDoInterpolate1D
    (const SimdBoolMask &mask,
     SimdXContext &xcontext,
     Interpolate1DFunc func)
{
    //
    // float func (float table[][2], float p)
    //

    const SimdReg &size  = xcontext.stack().regFpRelative (-1);
    const SimdReg &table = xcontext.stack().regFpRelative (-2);
    const SimdReg &p     = xcontext.stack().regFpRelative (-3);
    SimdReg &returnValue = xcontext.stack().regFpRelative (-4);

    assert (!size.isVarying());
    int s = *(int *)(size[0]);

    if (table.isVarying() ||
	p.isVarying())
    {
	returnValue.setVarying (true);

	if (!mask.isVarying() &&
	    !table.isVarying())
	{
	    //
	    // Fast path -- only p is varying, everything else is uniform.
	    //

	    float (*table0)[2] = (float (*)[2])(table[0]);

	    for (int i = xcontext.regSize(); --i >= 0;)
	    {
		*(float *)(returnValue[i]) = func (table0,
						   s,
						   *(float *)(p[i]));
	    }
	}
	else
	{
	    for (int i = xcontext.regSize(); --i >= 0;)
	    {
		if (mask[i])
		{
		    *(float *)(returnValue[i]) = func ((float (*)[2])(table[i]),
						       s,
						       *(float *)(p[i]));
		}
	    }
	}
    }
    else
    {
	returnValue.setVarying (false);

	*(float *)(returnValue[0]) = func ((float (*)[2])(table[0]), 
					   s,
					   *(float *)(p[0]));
    }
}


void
simdInterpolate1D (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // float interpolate1D (float table[][2], float p)
    //

    simdDoInterpolate1D (mask, xcontext, interpolate1D);
}


void
simdInterpolateCubic1D (const SimdBoolMask &mask, SimdXContext &xcontext)
{
    //
    // float interpolateCubic1D (float table[][2], float p)
    //

    simdDoInterpolate1D (mask, xcontext, interpolateCubic1D);
}

} // namespace

// This will get moved out of here in the near future. The code for
// reading lut files was going to be put in here. This code is currently
// unused.
#include <stdlib.h>
template <class T, int cols>
static void readLut(std::vector<float[cols]> *result,
                    const char *file, const char *section) {
	std::string token;
	std::ifstream s(file);
	unsigned int character;
	unsigned int token_no;
	unsigned int line;
	unsigned int section_no;
	bool at_eof;
	char in;
	bool at_end_of_token;
	bool at_eol;
	bool escape;
	bool in_comment;
	bool in_section;
	char *end;
	float value;
	float new_lutrow[cols];

	result->clear();

	memset(&new_lutrow, 0, sizeof(new_lutrow));

	character=0;
	line=1;
	token_no=0; // What token in the line we are...
	section_no=0;
	escape=0;
	at_end_of_token=0;;
	at_eol=0;
	in_comment=0;
	in_section=(section==NULL || section[0]==0) ? 1 : 0;
	at_eof=0;
	while(!at_eof) {
		if(s.eof()) {
			at_eof=1;
			escape=0;
			in='\n';
		} else {
			s.get(&in, sizeof(in));
		}
		character++;
		if(escape) {
			if(in_comment) {
				continue;
			}
			token.append(&in, sizeof(in));
			escape=0;
			continue;
		} else if(in=='\\') {
			escape=1;
			continue;
		} else if(in_comment) {
			// Yes, an escape characters in comments are honored
			if(in=='\n') {
				in_comment=0;
				at_eol=0;
				at_end_of_token=0;
				token=""; // should be redundant.
				character=0;
				token_no=0;
				line++;
			}
			continue;
		} else if(!isspace(in) && in!='#') {
			if(!at_end_of_token) {
				token.append(&in, sizeof(in));
				continue;
			} else {
				// We hit the end of the token (or rather,
				// got to the end of the token, then gobbled
				// all of the white space. The next character
				// is either the start of a new token or
				// the comment character.
				s.unget();
				character--;
			}
		} else { // is whitespace or comment character
			if(in=='\n') {
				at_eol=1; // end processing of this line. token...
			} else if(in=='#') {
				// will cause everything to get gobbled until CR.
				in_comment=1;
				continue;
			} else {
				at_end_of_token=1;
				continue;
			}
		}

		// At this point we have a token, and we know if we're
		// at the end of line.
		if(token_no==0 && token[0]=='[' && token[token.size()-1]==']') {
			// We have a section header...
			if(section==NULL || section[0]==0) {
				// Didn't specify a section name, but there was
				// a section header anyway. We either ignore it
				// (if it's the first one), or end parsing
				if(section_no!=0) {
					return;
				}
			} else if(in_section) {
				// already found our section (so we leave)
				return;
			} else if(!strncasecmp(section, token.c_str()+1, token.size()-2)) {
				in_section=1;
			}
			if(!at_eol) {
				// trash anything after the section header.
				in_comment=1;
			}
			section_no++;
			continue;
		}
		if(!in_section) {
			continue;
		}
		// OK. So we start parsing values (finally!)
		end=NULL;
		value=strtod(token.c_str(), &end);
		if(end!=NULL || *end!=0) {
			// XXX unable to parse token.c_str() as a floating point value.
		}
		if(token_no<cols) {
			new_lutrow[token_no]=value;
		} else {
			// XXX too many tokens in the row
		}
		if(at_eol) {
			while(token_no<cols) {
				new_lutrow[token_no++]=0;
			}
			result->push_back(new_lutrow);
		}
	}
	if(!in_comment) {
		// XXX no section named section
	}
}

void
declareSimdStdLibLookupTable (SymbolTable &symtab, SimdStdTypes &types)
{
    declareSimdCFunc (symtab, simdLookup1D,
		      types.funcType_f_f0_f_f_f(), "lookup1D");

    declareSimdCFunc (symtab, simdLookupCubic1D,
		      types.funcType_f_f0_f_f_f(), "lookupCubic1D");

    declareSimdCFunc (symtab, simdLookup3D_f3,
		      types.funcType_f3_f0003_f3_f3_f3(), "lookup3D_f3");

    declareSimdCFunc (symtab, simdLookup3D_f,
		      types.funcType_v_f0003_f3_f3_fff_offf(), "lookup3D_f");

    declareSimdCFunc (symtab, simdLookup3D_h,
		      types.funcType_v_f0003_f3_f3_hhh_ohhh(), "lookup3D_h");

    declareSimdCFunc (symtab, simdInterpolate1D,
		      types.funcType_f_f02_f(), "interpolate1D");

    declareSimdCFunc (symtab, simdInterpolateCubic1D,
		      types.funcType_f_f02_f(), "interpolateCubic1D");
}

} // namespace Ctl
