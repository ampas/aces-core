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

#ifndef INCLUDED_CTL_POINT_TREE_H
#define INCLUDED_CTL_POINT_TREE_H

//----------------------------------------------------------------------
//
//	class PointTree<>
//
//	This class implements a binary tree spacial partition
//      for 3D points.  The tree allows two kinds of queries: 
//
//	- find all points within a given sphere
//	- find the n points that are closest to a given location
//
//----------------------------------------------------------------------

#include <ImathBox.h>
#include <vector>
#include <algorithm>

namespace Ctl {

class PointTree
{
  public:
    
    //-----------------------------------------------------
    // Constructor and destructor
    //
    // Warning: the constructor only stores a reference to
    // the points array; the array is not copied.  It is
    // up to the caller to make sure that the array remains
    // vaoid as long as the PointTree exists.
    //-----------------------------------------------------
    
     PointTree (const Imath::V3f *points,
                size_t numPoints,
	        size_t leafSize = 8,
	        size_t maxDepth = 150);

    ~PointTree();


    //-------------------------------------
    // Rebuild the tree after the positions
    // of the points have been changed.
    //-------------------------------------

    void		rebuild();


    //--------------------------------------------------------
    // intersect(p,r,i) finds all points within a sphere with
    // radius r and center p.  The indices of those points are
    // returned in vector i.
    //--------------------------------------------------------

    void		intersect (const Imath::V3f &point,
				   double radius,
				   std::vector <size_t> &indices) const;


    //----------------------------------------------------
    // nearestPoints(p,n,i) finds the n points that are
    // nearest to point p.  The indices of the n nearest
    // points are returned in vector i.
    // If the PointTree contains less than n points, then
    // less than n indices are returned in i.
    // If the PointTree contains n or more points, then
    // the indices in i are partially sorted such that
    // points[i[n-1]] is not closer to p than points[i[j]]
    // for any j from 0 to n-2.
    //----------------------------------------------------

    void		nearestPoints (const Imath::V3f &center,
				       size_t numPoints,
				       std::vector <size_t> &indices) const;

  private:

    struct Node
    {
	 Node ()	{_left = 0; _right = 0; _dataIndex = 0;}
	~Node ()	{delete _left; delete _right;}

	Node *		_left;
	Node *		_right;
	double	        _midValue;
	size_t *	_dataIndex;
	size_t		_dataSize;
    };


    struct IndexComparator
    {
      public:

	int
	operator() (const size_t &a, const size_t &b)
	{
	    return points[a][dimension] < points[b][dimension];
	}

	size_t dimension;
	const Imath::V3f *points;
    };


    class CompareDistance
    {
      private:

	Imath::V3f		_center;
	const Imath::V3f*	_points;

      public:

	CompareDistance (const Imath::V3f &center, const Imath::V3f *points)
	{
	    _center = center;
	    _points = points;
	}

	bool
	operator() (size_t a, size_t b)
	{
	    double al = (_points[a] - _center).length2();
	    double bl = (_points[b] - _center).length2();

	    volatile double delta = fabs (al - bl);
	    const double eps = 2.0 * Imath::limits<double>::epsilon();

    	    //
	    // Impose strict weak ordering... if the lengths are the same,
	    // arbitrarily pick the one with the smallest index.
	    //

    	    if (delta < eps)
	    	return a < b;
	    else
	    	return al < bl;
	}
    };

    void		intersect (Node *node,
				   const Imath::Box3f &box,
				   size_t dimension,
				   const Imath::V3f &point,
				   double radius,
				   std::vector <size_t> &array) const;

    void		split (Node *node,
			       size_t dimension,
			       size_t depth,
			       const Imath::Box3f &box,
			       size_t *array,
			       size_t arraySize);

    static double	boxVolume (const Imath::Box3f &box);
    static double	radiusOfSphereWithVolume (double volume);
    static double	radiusOfSphereWithTwiceVolume (double radius);

    size_t		_numPoints;
    const Imath::V3f *	_points;
    size_t *		_indexArray;
    Imath::Box3f	_bbox;
    size_t		_leafSize;
    size_t		_maxDepth;
    size_t		_depth;
    size_t		_numNodes;
    Node*		_topNode;
};


//---------------
// Implementation
//---------------

PointTree::PointTree
    (const Imath::V3f *points,
     size_t numPoints,
     size_t leafSize,
     size_t maxDepth)
:
    _numPoints (numPoints),
    _points (points),
    _indexArray (new size_t[numPoints]),
    _leafSize (leafSize),
    _maxDepth (maxDepth),
    _depth (0),
    _numNodes (0),
    _topNode (0)
{
    rebuild();
}


PointTree::~PointTree()
{
    delete _topNode;
    delete [] _indexArray;
}


void
PointTree::rebuild()
{
    assert (_numPoints > 0);
    assert (&_points[0] != 0);

    //
    //	Compute bbox
    //

    _bbox.makeEmpty();

    for (size_t i = _numPoints; i--;)
    {
	_bbox.extendBy (_points[i]);
	_indexArray[i] = i;
    }

    _numNodes = 0;
    size_t dimension = _bbox.majorAxis();

    if ( _topNode )
    	delete _topNode;

    _topNode = new Node;
    _numNodes++;
    _depth = 0;

    split (_topNode, dimension, 0, _bbox, _indexArray, _numPoints);
}


void
PointTree::split
    (Node *node,
     size_t dimension,
     size_t depth,
     const Imath::Box3f &box,
     size_t *array,
     size_t arraySize)
{
    if (_depth < depth)
	_depth = depth;

    if (arraySize <= _leafSize || depth == _maxDepth)
    {
	node->_dataIndex = array;
	node->_dataSize	 = arraySize;
	return;
    }

    //
    //	The existing memory is sorted so that all the indexes on
    //	one side of the splitting plane are contiguous.
    //	The two remaining groups will be sent to the next
    //	split box.
    //

    size_t *leftArray     = 0;
    size_t *rightArray    = 0;
    size_t leftArraySize  = 0;
    size_t rightArraySize = 0;

    //
    // Median split
    //

    IndexComparator ic;
    ic.dimension = dimension;
    ic.points    = _points;

    size_t *midElement = array + arraySize / 2;
    std::nth_element (array, midElement, array + arraySize, ic);
    node->_midValue = _points[*midElement][dimension];

    leftArraySize   = arraySize / 2;
    rightArraySize  = arraySize - leftArraySize;
    leftArray	    = array;
    rightArray	    = midElement;

    //
    //	Remaining points are split according to the major axis
    //	of the bounding boxes.
    //

    if (leftArraySize)
    {
	Imath::Box3f leftBox (box);
	leftBox.max[dimension] = node->_midValue;
	size_t nextDimension = leftBox.majorAxis();
	node->_left = new Node;
	_numNodes++;

	split (node->_left,
	       nextDimension,
	       depth + 1,
	       leftBox,
	       leftArray,
	       leftArraySize);
    }

    if (rightArraySize)
    {
	Imath::Box3f rightBox (box);
	rightBox.min[dimension] = node->_midValue;
	size_t nextDimension = rightBox.majorAxis();
	node->_right = new Node;
	_numNodes++;

	split (node->_right,
	       nextDimension,
	       depth + 1,
	       rightBox,
	       rightArray,
	       rightArraySize);
    }

    node->_dataSize = 0;

    if (node->_left)
	node->_dataSize += node->_left->_dataSize;

    if (node->_right)
	node->_dataSize += node->_right->_dataSize;
}


void
PointTree::intersect
    (const Imath::V3f &point,
     double radius,
     std::vector <size_t> &array) const
{
    array.clear();
    intersect (_topNode, _bbox, _bbox.majorAxis(), point, radius, array);
}


void
PointTree::intersect
    (Node *node,
     const Imath::Box3f &box,
     size_t dimension,
     const Imath::V3f &point,
     double radius,
     std::vector <size_t> &array) const
{
    if (node->_dataIndex)
    {
	double radius2 = radius * radius;
	size_t index;

	for (size_t i = 0; i < node->_dataSize; i++)
	{
	    index = node->_dataIndex[i];
	    Imath::V3f vec = _points[index] - point;

	    if (vec.dot(vec) < radius2)
		array.push_back(index);
	}
    }
    else
    {
	Imath::V3f rvec(radius);

	if (node->_left)
	{
	    Imath::Box3f newBox(box);
	    newBox.max[dimension] = node->_midValue;
	    size_t nextDimension = newBox.majorAxis();
	    Imath::Box3f tNewBox(newBox);
	    tNewBox.min -= rvec;
	    tNewBox.max += rvec;

	    if (tNewBox.intersects (point))
	    {
		intersect (node->_left,
			   newBox,
			   nextDimension,
			   point,
			   radius,
			   array);
	    }
	}

	if (node->_right)
	{
	    Imath::Box3f newBox(box);
	    newBox.min[dimension] = node->_midValue;
	    size_t nextDimension = newBox.majorAxis();
	    Imath::Box3f tNewBox(newBox);
	    tNewBox.min -= rvec;
	    tNewBox.max += rvec;

	    if (tNewBox.intersects (point))
	    {
		intersect (node->_right,
			   newBox,
			   nextDimension,
			   point,
			   radius,
			   array);
	    }
	}
    }
}

void
PointTree::nearestPoints
    (const Imath::V3f &center,
     size_t numPoints,
     std::vector <size_t> &pointIndices) const
{
    pointIndices.resize (0);

    if (_topNode && numPoints > 0)
    {
	if (_numPoints < numPoints)
	{
	    //
	    // Special case -- the tree contains less than numPoints points.
	    //

	    for (size_t i = 0; i < _numPoints; i++)
		pointIndices.push_back (i);
	}
	else
	{
	    //
	    // Find a subtree that contains the center and at least numPoints
	    // points.  Based on the volume of the subtree's bounding box,
	    // make an "educated guess" for a search radius.
	    //

	    const Node *node = _topNode;
	    Imath::Box3f bbox = _bbox;
	    
	    while (true)
	    {
		if (node->_dataIndex)
		{
		    //
		    // Node is a leaf.
		    //

		    break;
		}

		//
		// Node is not a leaf.
		//

		size_t dimension = bbox.majorAxis();

		Imath::Box3f leftBbox (bbox);
		leftBbox.max[dimension] = node->_midValue;

		Imath::Box3f rightBbox (bbox);
		rightBbox.min[dimension] = node->_midValue;

		if (node->_left &&
		    leftBbox.intersects (center) &&
		    node->_left->_dataSize >= numPoints)
		{
		    node = node->_left;
		    bbox = leftBbox;
		}
		else if (node->_right &&
			 rightBbox.intersects (center) &&
			 node->_right->_dataSize >= numPoints)
		{
		    node = node->_right;
		    bbox = rightBbox;
		}
		else
		{
		    break;
		}
	    }

	    double nodeVolume = boxVolume (bbox);
	    double searchVolume = 2 * nodeVolume * numPoints / node->_dataSize;
	    double searchRadius = radiusOfSphereWithVolume (searchVolume);

	    //
	    // Find all points within the search radius.
	    // If we find less than numPoints points, increase
	    // the search radius (double the search volume).
	    //

	    intersect (center, searchRadius, pointIndices);

	    while (pointIndices.size() < numPoints)
	    {
		searchRadius = radiusOfSphereWithTwiceVolume (searchRadius);
		intersect (center, searchRadius, pointIndices);
	    }

	    //
	    // Vector pointIndices now contains at least numPoints points,
	    // and probably not too many more.  Partially sort the points
	    // so that the points closest to the center are in the vector's
	    // first numPoints positions.  Then truncate the vector.
	    //

	    std::nth_element (pointIndices.begin(),
			      pointIndices.begin() + (numPoints - 1),
			      pointIndices.end(),
			      CompareDistance (center, _points));

	    pointIndices.resize (numPoints);
	}
    }
}


double
PointTree::boxVolume (const Imath::Box3f &box)
{
    double volume = 1;

    for (size_t i = 0; i < 3; ++i)
    	if (box.max[i] - box.min[i] > 0)
	    volume *= box.max[i] - box.min[i];

    return volume;
}


inline double
PointTree::radiusOfSphereWithVolume (double volume)
{
    #ifdef _WIN32
	if (volume <= 0)
	    return 0;
	else
	    return (double) pow (0.238732 * volume, 1.0 / 3.0);
    #else
	return (double) cbrt (0.238732 * volume);
    #endif
}                          // 3/(4*pi)


inline double
PointTree::radiusOfSphereWithTwiceVolume (double radius)
{
    return (double) (1.25992 * radius);
}                 // cbrt(2)


} // namespace Ctl

#endif
