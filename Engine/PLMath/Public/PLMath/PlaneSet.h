/*********************************************************\
 *  File: PlaneSet.h                                     *
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


#ifndef __PLMATH_PLANESET_H__
#define __PLMATH_PLANESET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include "PLMath/Plane.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Sphere;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A set of planes defines a convex volume (e.g. the frustum, an event area...)
*
*  @note
*    - If there are no planes the tests whether something is inside the planes or not
*      will return always 'false'
*/
class PlaneSet {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default view plane order
		*/
		enum EViewPlaneOrder {
			VPNear   = 0,	/**< Near plane */
			VPRight  = 1,	/**< Right plane */
			VPLeft   = 2,	/**< Left plane */
			VPBottom = 3,	/**< Bottom plane */
			VPTop    = 4,	/**< Top plane */
			VPFar    = 5	/**< Far plane */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMATH_API PlaneSet();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLMATH_API PlaneSet(const PlaneSet &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API virtual ~PlaneSet();

		/**
		*  @brief
		*    Creates and adds a new plane to the plane set
		*
		*  @return
		*    The created and added new plane, a null pointer if there was an error
		*/
		PLMATH_API Plane *Create();

		/**
		*  @brief
		*    Creates 6 planes which enclose a box area
		*
		*  @param[in] vMin
		*    Minimum position of the box
		*  @param[in] vMax
		*    Maximum position of the box
		*
		*  @remarks
		*    Plane order:\n
		*    0 = Left (min x)\n
		*    1 = Right (max x)\n
		*    2 = Bottom (min y)\n
		*    3 = Top (max y)\n
		*    4 = Back (min z)\n
		*    5 = Front (max z)
		*
		*  @note
		*    - The six enclosing planes corresponding to the box are created automatically
		*/
		PLMATH_API void CreateBox(const Vector3 &vMin, const Vector3 &vMax);

		/**
		*  @brief
		*    Creates view planes using a view projection matrix
		*
		*  @param[in] mViewProjection
		*    View projection matrix
		*  @param[in] bInfProj
		*    Is the projection matrix part infinite? In this case, there's no far plane.
		*
		*  @note
		*    - If there are not enough planes this function will create it. There are
		*      6 view planes of the projection matrix is not infinite, else there are
		*      5 planes. (no far plane)
		*    - See EViewPlaneOrder for plane order
		*/
		PLMATH_API void CreateViewPlanes(const Matrix4x4 &mViewProjection, bool bInfProj = false);

		/**
		*  @brief
		*    Creates view planes using vertices and a view position
		*
		*  @param[in] lstVertices
		*    Vertices (xyz, counterclockwise) defining a 'portal'-polygon (there must be at least 3 vertices)
		*  @param[in] vViewPosition
		*    View position
		*
		*  @return
		*    'true' if all went fine, else 'false' (not enough vertices provided?)
		*
		*  @remarks
		*    You can for example use this function to create a 'frustum of a portal'.
		*
		*  @note
		*    - If there are not enough planes this function will create it. There are 5 planes. (no far plane)
		*/
		PLMATH_API bool CreateViewPlanes(const PLCore::Array<Vector3> &lstVertices, const Vector3 &vViewPosition);

		/**
		*  @brief
		*    Creates selection planes
		*
		*  @param[in] vStartPos
		*    Selection rectangle start position
		*  @param[in] vEndPos
		*    Selection rectangle end position
		*  @param[in] nX
		*    X component of the pixel coordinate of the upper-left corner of the viewport
		*  @param[in] nY
		*    Y component of the pixel coordinate of the upper-left corner of the viewport
		*  @param[in] nWidth
		*    Width of the clip volume, in pixels
		*  @param[in] nHeight
		*    Height of the clip volume, in pixels
		*  @param[in] mProjection
		*    Projection matrix
		*  @param[in] mView
		*    View matrix
		*  @param[in] bInfProj
		*    Is the projection matrix infinite? In this case, there's no far plane.
		*
		*  @note
		*    - Use this function for instance when dragging a selection rectangle via
		*      mouse to receive the scene nodes within it. The start and end positions
		*      are within the given viewport.
		*    - If there are not enough planes this function will create it. There are
		*      6 selection planes.
		*    - See EViewPlaneOrder for plane order
		*/
		PLMATH_API void CreateSelectionPlanes(const Vector2 &vStartPos, const Vector2 &vEndPos,
											  int nX, int nY, int nWidth, int nHeight,
											  const Matrix4x4 &mProjection, const Matrix4x4 &mView,
											  bool bInfProj = false);
		PLMATH_API void CreateSelectionPlanes(const Vector2 &vStartPos, const Vector2 &vEndPos,
											  int nX, int nY, int nWidth, int nHeight,
											  const Matrix4x4 &mProjection, const Matrix3x4 &mView,
											  bool bInfProj = false);

		/**
		*  @brief
		*    Returns the number of planes
		*
		*  @return
		*    Number of planes in the set
		*/
		PLMATH_API PLCore::uint32 GetNumOfPlanes() const;

		/**
		*  @brief
		*    Returns a plane
		*
		*  @param[in] nID
		*    ID of the plane which should be returned
		*
		*  @return
		*    Plane with the given ID, a null pointer if there was an error
		*/
		PLMATH_API const Plane *operator [](int nID) const;

		/**
		*  @brief
		*    Returns a plane
		*
		*  @param[in] nID
		*    ID of the plane which should be returned
		*
		*  @return
		*    Plane with the given ID, a null pointer if there was an error
		*/
		PLMATH_API Plane *operator [](int nID);

		/**
		*  @brief
		*    Returns the plane list
		*
		*  @return
		*    Plane list
		*/
		PLMATH_API PLCore::Array<Plane> &GetList();

		/**
		*  @brief
		*    Returns the plane list
		*
		*  @return
		*    Plane list
		*/
		PLMATH_API const PLCore::Array<Plane> &GetList() const;

		/**
		*  @brief
		*    Removes a plane
		*
		*  @param[in] nID
		*    ID of the plane which should be removed
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMATH_API bool RemovePlane(PLCore::uint32 nID = 0);

		/**
		*  @brief
		*    Clears the plane set
		*/
		PLMATH_API void Clear();

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
		PLMATH_API PlaneSet &operator =(const PlaneSet &cSource);

		/**
		*  @brief
		*    Plane set transformation
		*
		*  @param[in] mRot
		*    3x3 rotation matrix
		*
		*  @return
		*    This instance
		*/
		PLMATH_API PlaneSet &operator *=(const Matrix3x3 &mRot);

		/**
		*  @brief
		*    Plane set transformation
		*
		*  @param[in] mTrans
		*    3x4 transformation matrix
		*
		*  @return
		*    This instance
		*/
		PLMATH_API PlaneSet &operator *=(const Matrix3x4 &mTrans);

		/**
		*  @brief
		*    Plane set transformation
		*
		*  @param[in] mTrans
		*    4x4 transformation matrix
		*
		*  @return
		*    This instance
		*/
		PLMATH_API PlaneSet &operator *=(const Matrix4x4 &mTrans);

		/**
		*  @brief
		*    Checks whether the plane set is convex or not
		*
		*  @return
		*    'true' if the plane set is convex, else 'false'
		*
		*  @remarks
		*    A plane set is a convex volume if each plane has at least one 'neighbour'
		*    plane 'facing' it.
		*/
		PLMATH_API bool IsConvex() const;

		/**
		*  @brief
		*    Calculates the sphere surrounding the enclosed area
		*
		*  @param[out] cSphere
		*    Will receive the sphere surrounding the enclosed area
		*
		*  @see
		*    - GetSphere()
		*/
		PLMATH_API void CalculateSphere(Sphere &cSphere) const;

		/**
		*  @brief
		*    Calculates the box surrounding the enclosed area
		*
		*  @param[out] vMin
		*    Will receive the minimum position of the box
		*  @param[out] vMax
		*    Will receive the maximum position of the box
		*/
		PLMATH_API void CalculateBox(Vector3 &vMin, Vector3 &vMax) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::Array<Plane> m_lstPlane;	/**< Plane list */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_PLANESET_H__
