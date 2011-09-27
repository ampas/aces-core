///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
//	Copy data between CTL function arguments, OpenEXR
//	file header attributes and OpenEXR frame buffers.
//
//-----------------------------------------------------------------------------

#include <ImfCtlCopyFunctionArg.h>
#include <CtlType.h>
#include <ImfBoxAttribute.h>
#include <ImfChromaticitiesAttribute.h>
#include <ImfDoubleAttribute.h>
#include <ImfFloatAttribute.h>
#include <ImfIntAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfVecAttribute.h>
#include <ImathFun.h>
#include <Iex.h>
#include <cassert>
#include <cstring>

using namespace Imath;
using namespace Imf;
using namespace Ctl;
using namespace Iex;
using namespace std;

namespace ImfCtl {


void
copyFunctionArg
    (size_t numSamples,
     const FunctionArgPtr &src,
     const FunctionArgPtr &dst)
{
    if (!src->type()->isSameTypeAs (dst->type()))
	THROW (TypeExc,
	       "Cannot copy output argument " << src->name() << " of "
	       "CTL function " << src->func()->name() << " into "
	       "input argument " << dst->name() << " of "
	       "function " << dst->func()->name() << ".  The argument "
	       "types, " << src->type()->asString() << " "
	       "and " << dst->type()->asString() << ", are not the same.");

    if (src->isVarying())
    {
	//
	// copy varying to varying
	//

	if (!dst->isVarying())
	    THROW (TypeExc,
		   "Cannot copy varying output argument " << src->name() <<
		   " of CTL function " << src->func()->name() << " into "
		   "uniform input argument " << dst->name() << " of "
		   "function " << dst->func()->name() << ".");

	memcpy (dst->data(),
		src->data(),
		numSamples * src->type()->alignedObjectSize());
    }
    else
    {
	if (dst->isVarying())
	{
	    //
	    // copy uniform to varying
	    //

	    size_t os = src->type()->objectSize();
	    size_t aos = src->type()->alignedObjectSize();
	    char *srcData = src->data();
	    char *dstData = dst->data();

	    for (size_t i = 0; i < numSamples; ++i)
		memcpy (dstData + i * aos, srcData, os);
	}
	else
	{
	    //
	    // copy uniform to uniform
	    //

	    memcpy (dst->data(),
		    src->data(),
		    src->type()->objectSize());
	}
    }
}


namespace {

void
throwSrcSliceTypeMismatch
    (const char srcType[],
     const FunctionArgPtr &dst)
{
    THROW (TypeExc,
	   "Type of frame buffer slice (" << srcType << ") does not "
	   "match type of input argument " << dst->name() << " of "
	   "CTL function " << dst->func()->name() << " "
	   "(" << dst->type()->asString() << ").");
}


void
throwDstSliceTypeMismatch
    (const FunctionArgPtr &src,
     const char dstType[])
{
    THROW (TypeExc,
	   "Type of frame buffer slice (" << dstType << ") does not "
	   "match type of output argument " << src->name() << " of "
	   "CTL function " << src->func()->name() << " "
	   "(" << src->type()->asString() << ").");
}


void
throwSliceSampling ()
{
    THROW (NoImplExc,
	   "Frame buffer slices used as input or output arguments for "
	   "CTL function calls must have x and y sampling rate 1.");
}

} // namespace


void
copyFunctionArg
    (const Box2i transformWindow,
     size_t firstSample,
     size_t numSamples,
     const Slice &src,
     const FunctionArgPtr &dst)
{
    assert (dst->isVarying());

    if (src.xSampling != 1 || src.ySampling != 1)
	throwSliceSampling();

    long w = transformWindow.max.x - transformWindow.min.x + 1;
    long x = transformWindow.min.x + modp (firstSample, w);
    long y = transformWindow.min.y + divp (firstSample, w);
    char *dstData = (dst->data());
    size_t dstStride = dst->type()->alignedObjectSize();

    switch (src.type)
    {
      case HALF:

	if (!dst->type().cast<HalfType>())
	    throwSrcSliceTypeMismatch ("HALF", dst);

	for (size_t i = 0; i < numSamples; ++i)
	{
	    *(half *)dstData =
		*(half *)(src.base + x * src.xStride + y * src.yStride);

	    dstData += dstStride;
	    x += 1;

	    if (x > transformWindow.max.x)
	    {
		y += 1;
		x = transformWindow.min.x;
	    }
	}

	break;

	  case Imf::FLOAT:

	if (!dst->type().cast<FloatType>())
	    throwSrcSliceTypeMismatch ("FLOAT", dst);

	for (size_t i = 0; i < numSamples; ++i)
	{
	    *(float *)dstData =
		*(float *)(src.base + x * src.xStride + y * src.yStride);

	    dstData += dstStride;
	    x += 1;

	    if (x > transformWindow.max.x)
	    {
		y += 1;
		x = transformWindow.min.x;
	    }
	}

	break;

	  case Imf::UINT:

	if (!dst->type().cast<UIntType>())
	    throwSrcSliceTypeMismatch ("UINT", dst);

	for (size_t i = 0; i < numSamples; ++i)
	{
	    *(unsigned int *)dstData =
		*(unsigned int *)(src.base + x * src.xStride + y * src.yStride);

	    dstData += dstStride;
	    x += 1;

	    if (x > transformWindow.max.x)
	    {
		y += 1;
		x = transformWindow.min.x;
	    }
	}

	break;

		default:
		// eat NUM_PIXELTYPES
		break;
    }
}


void
copyFunctionArg
    (const Box2i transformWindow,
     size_t firstSample,
     size_t numSamples,
     const FunctionArgPtr &src,
     const Slice &dst)
{
    assert (src->isVarying());

    if (dst.xSampling != 1 || dst.ySampling != 1)
	throwSliceSampling();

    long w = transformWindow.max.x - transformWindow.min.x + 1;
    long x = transformWindow.min.x + modp (firstSample, w);
    long y = transformWindow.min.y + divp (firstSample, w);
    const char *srcData = (src->data());
    size_t srcStride = src->type()->alignedObjectSize();

    switch (dst.type)
    {
      case HALF:

	if (!src->type().cast<HalfType>())
	    throwDstSliceTypeMismatch (src, "HALF");

	for (size_t i = 0; i < numSamples; ++i)
	{
	    *(half *)(dst.base + x * dst.xStride + y * dst.yStride) =
		*(half *)srcData;

	    srcData += srcStride;
	    x += 1;

	    if (x > transformWindow.max.x)
	    {
		y += 1;
		x = transformWindow.min.x;
	    }
	}

	break;

	  case Imf::FLOAT:

	if (!src->type().cast<FloatType>())
	    throwDstSliceTypeMismatch (src, "FLOAT");

	for (size_t i = 0; i < numSamples; ++i)
	{
	    *(float *)(dst.base + x * dst.xStride + y * dst.yStride) =
		*(float *)srcData;

	    srcData += srcStride;
	    x += 1;

	    if (x > transformWindow.max.x)
	    {
		y += 1;
		x = transformWindow.min.x;
	    }
	}

	break;

	  case Imf::UINT:

	if (!src->type().cast<UIntType>())
	    throwDstSliceTypeMismatch (src, "UINT");

	for (size_t i = 0; i < numSamples; ++i)
	{
	    *(unsigned int *)(dst.base + x * dst.xStride + y * dst.yStride) =
		*(unsigned int *)srcData;

	    srcData += srcStride;
	    x += 1;

	    if (x > transformWindow.max.x)
	    {
		y += 1;
		x = transformWindow.min.x;
	    }
	}

	break;

		default:
		// eat NUM_PIXELTYPES
		break;
    }
}


namespace {

void
throwSrcAttrTypeMismatch
    (const Attribute &src,
     const FunctionArgPtr &dst)
{
    THROW (TypeExc,
	   "Type of image header attribute (" << src.typeName() << ") "
	   "does not match type of input argument " << dst->name() << " of "
	   "CTL function " << dst->func()->name() << " "
	   "(" << dst->type()->asString() << ").");
}


void
throwDstAttrTypeMismatch
    (const FunctionArgPtr &src,
     const Attribute &dst)
{
    THROW (TypeExc,
	   "Type of image header attribute (" << dst.typeName() << ") "
	   "does not match type of output argument " << src->name() << " of "
	   "CTL function " << src->func()->name() << " "
	   "(" << src->type()->asString() << ").");
}


void
throwUnknownAttrType (const Attribute &attr)
{
    THROW (NoImplExc,
	   "Cannot convert image header attribute of "
	   "type " << attr.typeName() << " to CTL data type.");
}


template <class ElementType>
bool
isArrayType1D
    (const TypePtr &type,
     size_t size,
     size_t &stride)
{
    ArrayTypePtr a = type.template cast<ArrayType>();

    if (a && a->size() == (int)size &&
	a->elementType().template cast<ElementType>())
    {
	stride = a->elementSize();
	return true;
    }

    return false;
}


template <class ElementType>
bool
isArrayType2D
    (const TypePtr &type,
     size_t size1,
     size_t size2,
     size_t &stride1,
     size_t &stride2)
{
    ArrayTypePtr a = type.template cast<ArrayType>();

    if (a && a->size() == (int)size1)
    {
	ArrayTypePtr e = a->elementType().template cast<ArrayType>();

	if (e && e->size() == (int)size2 &&
	    e->elementType().template cast<ElementType>())
	{
	    stride1 = a->elementSize();
	    stride2 = e->elementSize();
	    return true;
	}
    }

    return false;
}


TypePtr
findMember (const StructTypePtr st, const string &name, size_t &offset)
{
    const MemberVector members = st->members();

    for (MemberVectorConstIterator i = members.begin(); i != members.end(); ++i)
    {
	if (i->name == name)
	{
	    offset = i->offset;
	    return i->type;
	}
    }

    return 0;
}


} // namespace


void
copyFunctionArg
    (const Attribute &src,
     const FunctionArgPtr &dst)
{
    assert (!dst->isVarying());

    char *dstData = dst->data();
    
    if (const Box2iAttribute *tmp =
	    dynamic_cast <const Box2iAttribute *> (&src))
    {
	StructTypePtr st = dst->type().cast<StructType>();

	if (st && st->name() == "::Box2i")
	{
	    size_t minOffset, minStride;
	    size_t maxOffset, maxStride;

	    TypePtr minType = findMember (st, "min", minOffset);
	    TypePtr maxType = findMember (st, "max", maxOffset);

	    if (isArrayType1D <IntType> (minType, 2, minStride) &&
		isArrayType1D <IntType> (maxType, 2, maxStride))
	    {
		for (size_t i = 0; i < 2; ++i)
		    *(int *)(dstData + minOffset + i * minStride) =
			tmp->value().min[i];

		for (size_t i = 0; i < 2; ++i)
		    *(int *)(dstData + maxOffset + i * maxStride) =
			tmp->value().max[i];
	    }

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }
    
    if (const Box2fAttribute *tmp =
	    dynamic_cast <const Box2fAttribute *> (&src))
    {
	StructTypePtr st = dst->type().cast<StructType>();

	if (st && st->name() == "::Box2f")
	{
	    size_t minOffset, minStride;
	    size_t maxOffset, maxStride;

	    TypePtr minType = findMember (st, "min", minOffset);
	    TypePtr maxType = findMember (st, "max", maxOffset);

	    if (isArrayType1D <FloatType> (minType, 2, minStride) &&
		isArrayType1D <FloatType> (maxType, 2, maxStride))
	    {
		for (size_t i = 0; i < 2; ++i)
		    *(float *)(dstData + minOffset + i * minStride) =
			tmp->value().min[i]; 
		for (size_t i = 0; i < 2; ++i)
		    *(float *)(dstData + maxOffset + i * maxStride) =
			tmp->value().max[i];
	    }

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const ChromaticitiesAttribute *tmp =
	    dynamic_cast <const ChromaticitiesAttribute *> (&src))
    {
	StructTypePtr st = dst->type().cast<StructType>();

	if (st && st->name() == "::Chromaticities")
	{
	    size_t rOffset, rStride;
	    size_t gOffset, gStride;
	    size_t bOffset, bStride;
	    size_t wOffset, wStride;

	    TypePtr redType = findMember (st, "red", rOffset);
	    TypePtr greenType = findMember (st, "green", gOffset);
	    TypePtr blueType = findMember (st, "blue", bOffset);
	    TypePtr whiteType = findMember (st, "white", wOffset);

	    if (isArrayType1D <FloatType> (redType, 2, rStride) &&
		isArrayType1D <FloatType> (greenType, 2, gStride) &&
		isArrayType1D <FloatType> (blueType, 2, bStride) &&
		isArrayType1D <FloatType> (whiteType, 2, wStride))
	    {
		for (size_t i = 0; i < 2; ++i)
		    *(float *)(dstData + rOffset + i * rStride) =
			tmp->value().red[i];

		for (size_t i = 0; i < 2; ++i)
		    *(float *)(dstData + gOffset + i * gStride) =
			tmp->value().green[i];

		for (size_t i = 0; i < 2; ++i)
		    *(float *)(dstData + bOffset + i * bStride) =
			tmp->value().blue[i];

		for (size_t i = 0; i < 2; ++i)
		    *(float *)(dstData + wOffset + i * wStride) =
			tmp->value().white[i];
	    }

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const DoubleAttribute *tmp =
	    dynamic_cast <const DoubleAttribute *> (&src))
    {
	if (dst->type().cast<FloatType>())
	{
	    *(float *)dstData = (float)tmp->value();
	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const FloatAttribute *tmp =
	    dynamic_cast <const FloatAttribute *> (&src))
    {
	if (dst->type().cast<FloatType>())
	{
	    *(float *)dstData = tmp->value();
	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const IntAttribute *tmp =
	    dynamic_cast <const IntAttribute *> (&src))
    {
	if (dst->type().cast<IntType>())
	{
	    *(int *)dstData = tmp->value();
	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const M33fAttribute *tmp =
	    dynamic_cast <const M33fAttribute *> (&src))
    {
	size_t stride1, stride2;

	if (isArrayType2D <FloatType> (dst->type(), 3, 3, stride1, stride2))
	{
	    for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		    *(float *)(dstData + i * stride1 + j * stride2) =
			tmp->value()[i][j];
	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const M44fAttribute *tmp =
	    dynamic_cast <const M44fAttribute *> (&src))
    {
	size_t stride1, stride2;

	if (isArrayType2D <FloatType> (dst->type(), 4, 4, stride1, stride2))
	{
	    for (size_t i = 0; i < 4; ++i)
		for (size_t j = 0; j < 4; ++j)
		    *(float *)(dstData + i * stride1 + j * stride2) =
			tmp->value()[i][j];
	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const V2iAttribute *tmp =
	    dynamic_cast <const V2iAttribute *> (&src))
    {
	size_t stride;

	if (isArrayType1D <IntType> (dst->type(), 2, stride))
	{
	    for (size_t i = 0; i < 2; ++i)
		*(int *)(dstData + i * stride) = tmp->value()[i];

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const V2fAttribute *tmp =
	    dynamic_cast <const V2fAttribute *> (&src))
    {
	size_t stride;

	if (isArrayType1D <FloatType> (dst->type(), 2, stride))
	{
	    for (size_t i = 0; i < 2; ++i)
		*(float *)(dstData + i * stride) = tmp->value()[i];

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const V3iAttribute *tmp =
	    dynamic_cast <const V3iAttribute *> (&src))
    {
	size_t stride;

	if (isArrayType1D <IntType> (dst->type(), 3, stride))
	{
	    for (size_t i = 0; i < 3; ++i)
		*(int *)(dstData + i * stride) = tmp->value()[i];

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    if (const V3fAttribute *tmp =
	    dynamic_cast <const V3fAttribute *> (&src))
    {
	size_t stride;

	if (isArrayType1D <FloatType> (dst->type(), 3, stride))
	{
	    for (size_t i = 0; i < 3; ++i)
		*(float *)(dstData + i * stride) = tmp->value()[i];

	    return;
	}

	throwSrcAttrTypeMismatch (src, dst);
    }

    throwUnknownAttrType (src);
}


void
copyFunctionArg
    (const FunctionArgPtr &src,
     Attribute &dst)
{
    assert (!src->isVarying());

    const char *srcData = src->data();
    
    if (Box2iAttribute *tmp =
	    dynamic_cast <Box2iAttribute *> (&dst))
    {
	StructTypePtr st = src->type().cast<StructType>();

	if (st && st->name() == "::Box2i")
	{
	    size_t minOffset, minStride;
	    size_t maxOffset, maxStride;

	    TypePtr minType = findMember (st, "min", minOffset);
	    TypePtr maxType = findMember (st, "max", maxOffset);

	    if (isArrayType1D <IntType> (minType, 2, minStride) &&
		isArrayType1D <IntType> (maxType, 2, maxStride))
	    {
		for (size_t i = 0; i < 2; ++i)
		    tmp->value().min[i] =
			*(const int *)(srcData + minOffset + i * minStride);

		for (size_t i = 0; i < 2; ++i)
		    tmp->value().max[i] =
			*(const int *)(srcData + maxOffset + i * maxStride);
	    }

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }
    
    if (Box2fAttribute *tmp =
	    dynamic_cast <Box2fAttribute *> (&dst))
    {
	StructTypePtr st = src->type().cast<StructType>();

	if (st && st->name() == "::Box2f")
	{
	    size_t minOffset, minStride;
	    size_t maxOffset, maxStride;

	    TypePtr minType = findMember (st, "min", minOffset);
	    TypePtr maxType = findMember (st, "max", maxOffset);

	    if (isArrayType1D <FloatType> (minType, 2, minStride) &&
		isArrayType1D <FloatType> (maxType, 2, maxStride))
	    {
		for (size_t i = 0; i < 2; ++i)
		    tmp->value().min[i] =
			*(const float *)(srcData + minOffset + i * minStride);

		for (size_t i = 0; i < 2; ++i)
		    tmp->value().max[i] =
			*(const float *)(srcData + maxOffset + i * maxStride);
	    }

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (ChromaticitiesAttribute *tmp =
	    dynamic_cast <ChromaticitiesAttribute *> (&dst))
    {
	StructTypePtr st = src->type().cast<StructType>();

	if (st && st->name() == "::Chromaticities")
	{
	    size_t rOffset, rStride;
	    size_t gOffset, gStride;
	    size_t bOffset, bStride;
	    size_t wOffset, wStride;

	    TypePtr redType = findMember (st, "red", rOffset);
	    TypePtr greenType = findMember (st, "green", gOffset);
	    TypePtr blueType = findMember (st, "blue", bOffset);
	    TypePtr whiteType = findMember (st, "white", wOffset);

	    if (isArrayType1D <FloatType> (redType, 2, rStride) &&
		isArrayType1D <FloatType> (greenType, 2, gStride) &&
		isArrayType1D <FloatType> (blueType, 2, bStride) &&
		isArrayType1D <FloatType> (whiteType, 2, wStride))
	    {
		for (size_t i = 0; i < 2; ++i)
		    tmp->value().red[i] =
			*(const float *)(srcData + rOffset + i * rStride);

		for (size_t i = 0; i < 2; ++i)
		    tmp->value().green[i] =
			*(const float *)(srcData + gOffset + i * gStride);

		for (size_t i = 0; i < 2; ++i)
		    tmp->value().blue[i] =
			*(const float *)(srcData + bOffset + i * bStride);

		for (size_t i = 0; i < 2; ++i)
		    tmp->value().white[i] =
			*(const float *)(srcData + wOffset + i * wStride);
	    }

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (DoubleAttribute *tmp =
	    dynamic_cast <DoubleAttribute *> (&dst))
    {
	if (src->type().cast<FloatType>())
	{
	    tmp->value() = (double)*(const float *)srcData;
	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (FloatAttribute *tmp =
	    dynamic_cast <FloatAttribute *> (&dst))
    {
	if (src->type().cast<FloatType>())
	{
	    tmp->value() = *(const float *)srcData;
	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (IntAttribute *tmp =
	    dynamic_cast <IntAttribute *> (&dst))
    {
	if (src->type().cast<IntType>())
	{
	    tmp->value() = *(const int *)srcData;
	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (M33fAttribute *tmp =
	    dynamic_cast <M33fAttribute *> (&dst))
    {
	size_t stride1, stride2;

	if (isArrayType2D <FloatType> (src->type(), 3, 3, stride1, stride2))
	{
	    for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		    tmp->value()[i][j] =
			*(const float *)(srcData + i * stride1 + j * stride2);
	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (M44fAttribute *tmp =
	    dynamic_cast <M44fAttribute *> (&dst))
    {
	size_t stride1, stride2;

	if (isArrayType2D <FloatType> (src->type(), 4, 4, stride1, stride2))
	{
	    for (size_t i = 0; i < 4; ++i)
		for (size_t j = 0; j < 4; ++j)
		    tmp->value()[i][j] =
			*(const float *)(srcData + i * stride1 + j * stride2);
	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (V2iAttribute *tmp =
	    dynamic_cast <V2iAttribute *> (&dst))
    {
	size_t stride;

	if (isArrayType1D <IntType> (src->type(), 2, stride))
	{
	    for (size_t i = 0; i < 2; ++i)
		tmp->value()[i] = *(const int *)(srcData + i * stride);

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (V2fAttribute *tmp =
	    dynamic_cast <V2fAttribute *> (&dst))
    {
	size_t stride;

	if (isArrayType1D <FloatType> (src->type(), 2, stride))
	{
	    for (size_t i = 0; i < 2; ++i)
		tmp->value()[i] = *(const float *)(srcData + i * stride);

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (V3iAttribute *tmp =
	    dynamic_cast <V3iAttribute *> (&dst))
    {
	size_t stride;

	if (isArrayType1D <IntType> (src->type(), 3, stride))
	{
	    for (size_t i = 0; i < 3; ++i)
		tmp->value()[i] = *(const int *)(srcData + i * stride);

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    if (V3fAttribute *tmp =
	    dynamic_cast <V3fAttribute *> (&dst))
    {
	size_t stride;

	if (isArrayType1D <FloatType> (src->type(), 3, stride))
	{
	    for (size_t i = 0; i < 3; ++i)
		tmp->value()[i] = *(const float *)(srcData + i * stride);

	    return;
	}

	throwDstAttrTypeMismatch (src, dst);
    }

    throwUnknownAttrType (dst);
}

} // namespace ImfCtl
