/*********************************************************\
 *  File: AABoundingBox.h                                *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLMATH_AABOUNDINGBOX_H__
#define __PLMATH_AABOUNDINGBOX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Vector3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Axis align bounding box class
*
*  @remarks
*    @code
*        3+------+2  y
*        /|     /|   |
*       / |    / |   |
*      / 0+---/--+1  *---x
*    7+------+6 /   /
*     | /    | /   z
*     |/     |/
*    4+------+5
*    @endcode
*
*  @note
*    - It's highly recommended (but not enforced by this class) that the minimum is
*      really the minimum and the maximum is really the maximum
*/
class AABoundingBox {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Axis
		*/
		enum Axis {
			X = 0,	/**< X axis */
			Y = 1,	/**< Y axis */
			Z = 2	/**< Z axis */
		};


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		Vector3 vMin;	/**< Minimum position */
		Vector3 vMax;	/**< Maximum position */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all minimum and maximum components to 0
		*/
		inline AABoundingBox();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline AABoundingBox(const AABoundingBox &cSource);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vMinMax
		*    Minimum and maximum position
		*/
		inline AABoundingBox(const Vector3 &vMinMax);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vMin
		*    Minimum position
		*  @param[in] vMax
		*    Maximum position
		*/
		inline AABoundingBox(const Vector3 &vMin, const Vector3 &vMax);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] fMinX
		*    X component of the minimum position
		*  @param[in] fMinY
		*    Y component of the minimum position
		*  @param[in] fMinZ
		*    Z component of the minimum position
		*  @param[in] fMaxX
		*    X component of the maximum position
		*  @param[in] fMaxY
		*    Y component of the maximum position
		*  @param[in] fMaxZ
		*    Z component of the maximum position
		*/
		inline AABoundingBox(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~AABoundingBox();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		PLMATH_API AABoundingBox &operator =(const AABoundingBox &cSource);

		/**
		*  @brief
		*    Returns the center of the box
		*
		*  @return
		*    Center of the box ((vMax+vMin)/2)
		*/
		inline Vector3 GetCenter() const;

		/**
		*  @brief
		*    Returns the width
		*
		*  @return
		*    Width (vMax.x - vMin.x)
		*/
		inline float GetWidth() const;

		/**
		*  @brief
		*    Returns the height
		*
		*  @return
		*    Height (vMax.y - vMin.y)
		*/
		inline float GetHeight() const;

		/**
		*  @brief
		*    Returns the depth
		*
		*  @return
		*    Depth (vMax.z - vMin.z)
		*/
		inline float GetDepth() const;

		/**
		*  @brief
		*    Returns the longest axis
		*
		*  @return
		*    The longest axis
		*/
		inline Axis GetLongestAxis() const;

		/**
		*  @brief
		*    Returns the length of the longest axis
		*
		*  @return
		*    The length of the longest axis
		*/
		inline float GetLongestAxisLength() const;

		/**
		*  @brief
		*    Returns the radius of a sphere placed at the origin (0, 0, 0) enclosing this axis align bounding box
		*
		*  @return
		*    The radius of a sphere placed at the origin (0, 0, 0) enclosing this axis align bounding box
		*/
		inline float GetEnclosingRadius() const;

		/**
		*  @brief
		*    Returns the radius of a sphere placed at the origin (0, 0, 0) inside this axis align bounding box
		*
		*  @return
		*    The radius of a sphere placed at the origin (0, 0, 0) inside this axis align bounding box
		*
		*  @note
		*    - Smaller than GetEnclosingRadius()
		*    - Same as GetLongestAxisLength()/2
		*/
		inline float GetInsideRadius() const;

		/**
		*  @brief
		*    Calculates the surface of the box
		*
		*  @return
		*    Surface of the box
		*/
		inline float CalculateSurface() const;

		/**
		*  @brief
		*    Calculates the volume of the box
		*
		*  @return
		*    Volume of the box
		*/
		inline float CalculateVolume() const;

		/**
		*  @brief
		*    Clips this box with another one
		*
		*  @param[in] cEnclosed
		*    Axis align box to clip this box with
		*/
		inline void ClipByAABox(const AABoundingBox &cEnclosed);

		/**
		*  @brief
		*    Appends a vertex to the cubic hull
		*
		*  @param[in] vV
		*    Vertex to append
		*/
		inline void AppendToCubicHull(const Vector3 &vV);

		/**
		*  @brief
		*    Combines two axis align boxes
		*
		*  @param[in] cBox
		*    Axis align box to combine with this box
		*/
		inline void CombineAABoxes(const AABoundingBox &cBox);

		/**
		*  @brief
		*    Returns the 8 corner vertices
		*
		*  @param[out] vVertex
		*    This array will receive the 8 corner vertices
		*
		*  @see
		*    - Remarks of this class for the positions of the 8 corner vertices
		*/
		PLMATH_API void GetVertices(Vector3 vVertex[8]) const;

		/**
		*  @brief
		*    Returns one of the 8 corner vertices
		*
		*  @param[in] nIndex
		*    Index of the corner vertex to return
		*
		*  @return
		*    The requested corner vertex
		*
		*  @see
		*    - GetVertices()
		*/
		PLMATH_API Vector3 GetVertex(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Calculates the index of the nearest vertex in the AAB according to the normal vector of a clip plane
		*
		*  @param[in] vClipPlaneNormal
		*    Clip plane normal
		*
		*  @return
		*    Index of the nearest vertex in the AAB according to the normal vector of a clip plane
		*/
		PLMATH_API PLCore::uint32 GetNearestVertexIndex(const Vector3 &vClipPlaneNormal) const;

		/**
		*  @brief
		*    Calculates the index of the furthest vertex in the AAB according to the normal vector of a clip plane
		*
		*  @param[in] vClipPlaneNormal
		*    Clip plane normal
		*
		*  @return
		*    Index of the furthest vertex in the AAB according to the normal vector of a clip plane
		*/
		PLMATH_API PLCore::uint32 GetFurthestVertexIndex(const Vector3 &vClipPlaneNormal) const;

		/**
		*  @brief
		*    Ensures that the minimum is really the minimum and the maximum is really the maximum
		*/
		PLMATH_API void ValidateMinMax();

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] vV
		*    Vector to add
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator +(const Vector3 &vV) const;

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] fS
		*    Scalar to add
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator +(float fS) const;

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] vV
		*    Vector to add
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator +=(const Vector3 &vV);

		/**
		*  @brief
		*    Per component addition
		*
		*  @param[in] fS
		*    Scalar to add
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator +=(float fS);

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] vV
		*    Vector to subtract
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator -(const Vector3 &vV) const;

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] fS
		*    Scalar to subtract
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator -(float fS) const;

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] vV
		*    Vector to subtract
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator -=(const Vector3 &vV);

		/**
		*  @brief
		*    Per component subtraction
		*
		*  @param[in] fS
		*    Scalar to subtract
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator -=(float fS);

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] vV
		*    Vector to multiplicate with
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator *(const Vector3 &vV) const;

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] fS
		*    Scalar to multiplicate with
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator *(float fS) const;

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] vV
		*    Vector to multiplicate with
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator *=(const Vector3 &vV);

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] fS
		*    Scalar to multiplicate with
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator *=(float fS);

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] vV
		*    Vector to divide through
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator /(const Vector3 &vV) const;

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] fS
		*    Scalar to divide through
		*
		*  @return
		*    The resulting axis aligned bounding box
		*/
		inline AABoundingBox operator /(float fS) const;

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] vV
		*    Vector to divide through
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator /=(const Vector3 &vV);

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] fS
		*    Scalar to divide through
		*
		*  @return
		*    Reference to this instance
		*/
		inline AABoundingBox &operator /=(float fS);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/AABoundingBox.inl"


#endif // __PLMATH_AABOUNDINGBOX_H__
