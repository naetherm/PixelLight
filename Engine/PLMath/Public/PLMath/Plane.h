/*********************************************************\
 *  File: Plane.h                                        *
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


#ifndef __PLMATH_PLANE_H__
#define __PLMATH_PLANE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Vector4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Plane class
*
*  @remarks
*    A plane is defined in 3D space by the equation: Ax + By + Cz - D = 0\n
*    This equates to a vector - the normal of the plane, whose x, y
*    and z components equate to the coefficients A, B and C
*    respectively, and a constant D which is the distance along
*    the normal you have to go to move the plane back to the origin.
*/
class Plane {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Component
		*/
		enum Component {
			X = 0,	/**< X component */
			Y = 1,	/**< Y component */
			Z = 2,	/**< Z component */
			W = 3	/**< Z component */
		};

		/**
		*  @brief
		*    Plane/point relation
		*/
		enum ESide {
			Behind     = -1,	/**< Behind the plane (distance < 0) */
			Coinciding =  0,	/**< Lies on the plane (distance = 0) */
			InFront    =  1		/**< In font of the plane (distance > 0) */
		};


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		union {
			float fVector[4];		/**< Coefficients */
			struct {
				float a, b, c, d;	/**< Coefficients */
			};
			struct {
				float fN[3];		/**< Plane normal */
				float fD;			/**< Distance to origin */
			};
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all components to 0
		*/
		inline Plane();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] fA
		*    Plane equation value A
		*  @param[in] fB
		*    Plane equation value B
		*  @param[in] fC
		*    Plane equation value C
		*  @param[in] fD
		*    Plane equation value D
		*/
		inline Plane(float fA, float fB, float fC, float fD);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vOrigin
		*    Plane origin
		*  @param[in] vNormal
		*    Plane normal (will be normalized automatically)
		*/
		inline Plane(const Vector3 &vOrigin, const Vector3 &vNormal);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vV1
		*    First vertex on the plane
		*  @param[in] vV2
		*    Second vertex on the plane
		*  @param[in] vV3
		*    Third vertex on the plane
		*/
		inline Plane(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Plane();

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
		inline Plane &operator =(const Plane &cSource);

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		inline bool operator ==(const Plane &cPlane) const;
		inline bool operator !=(const Plane &cPlane) const;

		//[-------------------------------------------------------]
		//[ Transformation                                        ]
		//[-------------------------------------------------------]
		PLMATH_API Plane  operator *(const Matrix3x3 &mRot) const;
		PLMATH_API Plane  operator *(const Matrix3x4 &mTrans) const;
		PLMATH_API Plane  operator *(const Matrix4x4 &mTrans) const;
		PLMATH_API Plane &operator *=(const Matrix3x3 &mRot);
		PLMATH_API Plane &operator *=(const Matrix3x4 &mTrans);
		PLMATH_API Plane &operator *=(const Matrix4x4 &mTrans);

		/**
		*  @brief
		*    Calculates the plane
		*
		*  @param[in] vOrigin
		*    Plane origin
		*  @param[in] vNormal
		*    Plane normal (will be normalized automatically)
		*
		*  @return
		*    This instance
		*/
		inline Plane &ComputeND(const Vector3 &vOrigin, const Vector3 &vNormal);

		/**
		*  @brief
		*    Calculates the plane
		*
		*  @param[in] vV1
		*    First vertex on the plane
		*  @param[in] vV2
		*    Second vertex on the plane
		*  @param[in] vV3
		*    Third vertex on the plane
		*
		*  @return
		*    This instance
		*/
		PLMATH_API Plane &ComputeND(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);

		/**
		*  @brief
		*    Computes the tangent plane of an ellipsoid
		*
		*  @param[in] vPointPos
		*    Point on the plane
		*  @param[in] vEllipsoidPos
		*    Ellipsoid position
		*  @param[in] vEllipsoidRadius
		*    Ellipsoid radius
		*
		*  @return
		*    This instance
		*/
		PLMATH_API Plane &ComputeTangentPlaneOfEllipsoid(const Vector3 &vPointPos, const Vector3 &vEllipsoidPos,
														 const Vector3 &vEllipsoidRadius);

		/**
		*  @brief
		*    Normalizes the plane
		*
		*  @return
		*    This instance
		*/
		inline Plane &Normalize();

		/**
		*  @brief
		*    Calculates the interpolated plane from two other planes
		*
		*  @param[in] cP2
		*    Other plane to interpolate with
		*  @param[in] fFactor
		*    Interpolation factor. 0.0 = this plane, 1.0 = cP2
		*
		*  @return
		*    The resulting interpolated plane
		*/
		PLMATH_API Plane Lerp(const Plane &cP2, float fFactor);

		/**
		*  @brief
		*    Returns the side of the plane the given point is on
		*
		*  @param[in] vPoint
		*    Point to check
		*
		*  @return
		*    The side of the plane the given point is on
		*/
		inline ESide GetSide(const Vector3 &vPoint) const;

		/**
		*  @brief
		*    Calculates the distance from a point to the plane
		*
		*  @param[in] vPoint
		*    Point the distance should be calculated
		*
		*  @return
		*    The distance from the point to the plane
		*/
		inline float GetDistance(const Vector3 &vPoint) const;

		/**
		*  @brief
		*    Calculates the distance from a point to the plane
		*
		*  @param[in] vPoint
		*    Point the distance should be calculated
		*
		*  @return
		*    The distance from the point to the plane
		*/
		inline float GetDistance(const Vector4 &vPoint) const;

		/**
		*  @brief
		*    Calculates the distance to the ray/intersection point
		*
		*  @param[in] vRayPos
		*    Ray position
		*  @param[in] vRayDir
		*    Ray direction (must be normalized)
		*
		*  @return
		*    Distance to ray/plane intersection point (-1.0 if there's no intersection)
		*/
		inline float GetDistance(const Vector3 &vRayPos, const Vector3 &vRayDir) const;

		/**
		*  @brief
		*    Returns a point on the plane
		*
		*  @return
		*    A point on the plane (-D*N)
		*/
		inline Vector3 GetPointOnPlane() const;

		/**
		*  @brief
		*    Clips an edge by this plane
		*
		*  @param[in] vV1
		*    The first vertex of the edge
		*  @param[in] vV2
		*    The second vertex of the edge
		*
		*  @return
		*    The clipped vertex of the edge on this plane
		*/
		inline Vector3 ClipEdge(const Vector3 &vV1, const Vector3 &vV2) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Plane.inl"


#endif // __PLMATH_PLANE_H__
