/*********************************************************\
 *  File: Intersect.h                                    *
 *      Intersection tools
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


#ifndef __PLMATH_INTERSECT_H__
#define __PLMATH_INTERSECT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/PLMath.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	template <class AType> class Array;
}
namespace PLMath {
	class Ray;
	class Line;
	class Plane;
	class Sphere;
	class Vector3;
	class Vector4;
	class PlaneSet;
	class BoundingBox;
	class AABoundingBox;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static class with some useful intersection tools
*/
class Intersect {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Sphere                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check whether there's a sphere/point intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] cPoint
		*    Point to check
		*
		*  @return
		*    'true' if there's a sphere/axis aligned box intersection, else 'false'
		*/
		static PLMATH_API bool SpherePoint(const Sphere &cSphere, const Vector3 &cPoint);
		static PLMATH_API bool SpherePoint(const Sphere &cSphere, const Vector4 &cPoint);

		/**
		*  @brief
		*    Check whether there's a sphere/ray intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] vRayOrigin
		*    Ray origin
		*  @param[in] vRayDirection
		*    Ray direction vector
		*
		*  @return
		*    Distance to intersection point, -1.0 it there was no intersection
		*/
		static PLMATH_API float SphereRay(const Sphere &cSphere, const Vector3 &vRayOrigin, const Vector3 &vRayDirection);

		/**
		*  @brief
		*    Check whether there's a sphere/ray intersection
		*
		*  @param[in]  cSphere
		*    Sphere to check
		*  @param[in]  vRayOrigin
		*    Ray origin
		*  @param[in]  vRayDirection
		*    Ray direction vector
		*  @param[out] pvIntersect
		*    If not a null pointer, will receive the intersection point (if there's one)
		*
		*  @return
		*    'true' if there's a sphere/ray intersection, else 'false'
		*/
		static PLMATH_API bool SphereRay(const Sphere &cSphere, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 *pvIntersect = nullptr);

		/**
		*  @brief
		*    Check whether there's a sphere/ray intersection
		*
		*  @param[in]  cSphere
		*    Sphere to check
		*  @param[in]  vRayOrigin
		*    Ray origin
		*  @param[in]  vRayDirection
		*    Ray direction vector
		*  @param[in]  fDistance
		*    Ray 'length'
		*  @param[out] pvIntersect
		*    If not a null pointer, will receive the intersection point (if there's one)
		*
		*  @return
		*    'true' if there's a sphere/ray intersection, else 'false'
		*/
		static PLMATH_API bool SphereRay(const Sphere &cSphere, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, float fDistance, Vector3 *pvIntersect = nullptr);

		/**
		*  @brief
		*    Check whether there's a sphere/ray intersection
		*
		*  @param[in]  cSphere
		*    Sphere to check
		*  @param[in]  vRayOrigin
		*    Ray origin
		*  @param[in]  vRayDirection
		*    Ray direction vector
		*  @param[out] pvIntersect
		*    If not a null pointer, will receive the intersection point (if there's one)
		*
		*  @return
		*    'true' if there's a sphere/ray intersection, else 'false'
		*/
		static PLMATH_API bool SphereRayInFront(const Sphere &cSphere, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 *pvIntersect = nullptr);

		/**
		*  @brief
		*    Check whether there's a sphere/line intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] vStart
		*    Line start point
		*  @param[in] vEnd
		*    Line end point
		*
		*  @return
		*    'true' if there's a sphere/line intersection, else 'false'
		*/
		static PLMATH_API bool SphereLine(const Sphere &cSphere, const Vector3 &vStart, const Vector3 &vEnd);

		/**
		*  @brief
		*    Check whether there's a sphere/sphere intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] vPos
		*    Second sphere position
		*  @param[in] fRadius
		*    Second sphere radius
		*
		*  @return
		*    'true' if there's a sphere/sphere intersection, else 'false'
		*/
		static PLMATH_API bool SphereSphere(const Sphere &cSphere, const Vector3 &vPos, float fRadius);

		/**
		*  @brief
		*    Check whether there's a sphere/sphere intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] cSphere2
		*    Second sphere to check
		*
		*  @return
		*    'true' if there's a sphere/sphere intersection, else 'false'
		*/
		static PLMATH_API bool SphereSphere(const Sphere &cSphere, const Sphere &cSphere2);

		/**
		*  @brief
		*    Check whether a sphere intersects with this sphere while moving
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] cSphere2
		*    Second sphere to test
		*  @param[in] vMove1
		*    Movement vector of this sphere
		*  @param[in] vMove2
		*    Movement vector of the other sphere box
		*
		*  @return
		*    'true' if there's an intersection, else 'false'
		*/
		static PLMATH_API bool SphereSphere(const Sphere &cSphere, const Sphere &cSphere2, const Vector3 &vMove1, const Vector3 &vMove2);

		/**
		*  @brief
		*    Check whether there's a sphere/axis aligned box intersection
		*
		*  @param[in] vSpherePos
		*    Position of the sphere to check
		*  @param[in] fSphereRadius
		*    Radius of the sphere to check
		*  @param[in] cAABox
		*    Axis aligned box to check
		*
		*  @return
		*    'true' if there's a sphere/axis aligned box intersection, else 'false'
		*/
		static PLMATH_API bool SphereAABox(const Vector3 &vSpherePos, float fSphereRadius, const AABoundingBox &cAABox);

		/**
		*  @brief
		*    Check whether there's a sphere/axis aligned box intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] cAABox
		*    Axis aligned box to check
		*
		*  @return
		*    'true' if there's a sphere/axis aligned box intersection, else 'false'
		*/
		static PLMATH_API bool SphereAABox(const Sphere &cSphere, const AABoundingBox &cAABox);

		/**
		*  @brief
		*    Check whether there's a sphere/box intersection
		*
		*  @param[in] vSpherePos
		*    Position of the sphere to check
		*  @param[in] fSphereRadius
		*    Radius of the sphere to check
		*  @param[in] cBox
		*    Box to check
		*
		*  @return
		*    'true' if there's a sphere/box intersection, else 'false'
		*/
		static PLMATH_API bool SphereBox(const Vector3 &vSpherePos, float fSphereRadius, const BoundingBox &cBox);

		/**
		*  @brief
		*    Check whether there's a sphere/box intersection
		*
		*  @param[in] cSphere
		*    Sphere to check
		*  @param[in] cBox
		*    Box to check
		*
		*  @return
		*    'true' if there's a sphere/box intersection, else 'false'
		*/
		static PLMATH_API bool SphereBox(const Sphere &cSphere, const BoundingBox &cBox);

		//[-------------------------------------------------------]
		//[ Axis aligned box                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check whether a point is inside the axis aligned box
		*
		*  @param[in] vAABoxMin
		*    Axis aligned box minimum position
		*  @param[in] vAABoxMax
		*    Axis aligned box box maximum position
		*  @param[in] vPos
		*    Point to test
		*
		*  @return
		*    'true' if the given point is inside the axis aligned box, else 'false'
		*/
		static PLMATH_API bool AABoxPoint(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Vector3 &vPos);

		/**
		*  @brief
		*    Check whether there's a axis aligned box/line intersection
		*
		*  @param[in]  vAABoxMin
		*    Axis aligned box minimum position
		*  @param[in]  vAABoxMax
		*    Axis aligned box box maximum position
		*  @param[in]  vStart
		*    Line start point
		*  @param[in]  vEnd
		*    Line end point
		*  @param[out] pfIntersection
		*    Receives the intersection distance if not null, <0 if there's no intersection or when the start point is inside the axis aligned box or when the distance is just negative
		*
		*  @return
		*    'true' if there's a axis aligned box/line intersection, else 'false'
		*/
		static PLMATH_API bool AABoxLine(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Vector3 &vStart, const Vector3 &vEnd, float *pfIntersection = nullptr);

		/**
		*  @brief
		*    Check whether there's a axis aligned box/line intersection
		*
		*  @param[in]  vAABoxMin
		*    Axis aligned box minimum position
		*  @param[in]  vAABoxMax
		*    Axis aligned box box maximum position
		*  @param[in]  cLine
		*    Line to check
		*  @param[out] pfIntersection
		*    Receives the intersection distance if not null, <0 if there's no intersection or when the start point is inside the axis aligned box or when the distance is just negative
		*
		*  @return
		*    'true' if there's a axis aligned box/line intersection, else 'false'
		*/
		static PLMATH_API bool AABoxLine(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Line &cLine, float *pfIntersection = nullptr);

		/**
		*  @brief
		*    Check whether there's a axis aligned box/sphere intersection
		*
		*  @param[in] vAABoxMin
		*    Axis aligned box minimum position
		*  @param[in] vAABoxMax
		*    Axis aligned box box maximum position
		*  @param[in] vSpherePos
		*    Position of the sphere to check
		*  @param[in] fSphereRadius
		*    Radius of the sphere to check
		*
		*  @return
		*    'true' if there's a sphere/axis aligned box intersection, else 'false'
		*/
		static PLMATH_API bool AABoxSphere(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Vector3 &vSpherePos, float fSphereRadius);

		/**
		*  @brief
		*    Check whether there's a axis aligned box/axis aligned box intersection
		*
		*  @param[in] vAABoxMin1
		*    Axis aligned box minimum position 1
		*  @param[in] vAABoxMax1
		*    Axis aligned box box maximum position 1
		*  @param[in] vAABoxMin2
		*    Axis aligned box minimum position 2
		*  @param[in] vAABoxMax2
		*    Axis aligned box box maximum position 2
		*
		*  @return
		*    'true' if there's a axis aligned box/axis aligned box intersection, else 'false'
		*/
		static PLMATH_API bool AABoxAABox(const Vector3 &vAABoxMin1, const Vector3 &vAABoxMax1, const Vector3 &vAABoxMin2, const Vector3 &vAABoxMax2);

		/**
		*  @brief
		*    Check whether there's a axis aligned box/axis aligned box intersection
		*
		*  @param[in] cBox1
		*    Axis aligned box 1
		*  @param[in] cBox2
		*    Axis aligned box 2
		*
		*  @return
		*    'true' if there's a axis aligned box/axis aligned box intersection, else 'false'
		*/
		static PLMATH_API bool AABoxAABox(const AABoundingBox &cBox1, const AABoundingBox &cBox2);

		//[-------------------------------------------------------]
		//[ Box                                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check whether a point is inside the box
		*
		*  @param[in] cBox
		*    Box to check
		*  @param[in] vPos
		*    Point to test
		*
		*  @return
		*    'true' if the given point is inside the box, else 'false'
		*/
		static PLMATH_API bool BoxPoint(const BoundingBox &cBox, const Vector3 &vPos);

		/**
		*  @brief
		*    Check whether a line intersects with the box
		*
		*  @param[in] cBox
		*    Box to check
		*  @param[in] vA
		*    Line start position
		*  @param[in] vB
		*    Line end position
		*
		*  @return
		*    'true' if the given line intersects with the box, else 'false'
		*/
		static PLMATH_API bool BoxLine(const BoundingBox &cBox, const Vector3 &vA, const Vector3 &vB);

		/**
		*  @brief
		*    Check whether another box intersects with this box
		*
		*  @param[in] cBox1
		*    First box to check
		*  @param[in] cBox2
		*    Second box to test
		*
		*  @return
		*    'true' if the given box intersects with this box, else 'false'
		*/
		static PLMATH_API bool BoxBox(const BoundingBox &cBox1, const BoundingBox &cBox2);

		/**
		*  @brief
		*    Check whether another box intersects with this box while moving
		*
		*  @param[in] cBox1
		*    First box to check
		*  @param[in] cBox2
		*    Second box to test
		*  @param[in] vMove1
		*    Movement vector of first box
		*  @param[in] vMove2
		*    Movement vector of second box
		*
		*  @return
		*    'true' if the given box intersects with this box while moving, else 'false'
		*/
		static PLMATH_API bool BoxBox(const BoundingBox &cBox1, const BoundingBox &cBox2, const Vector3 &vMove1, const Vector3 &vMove2);

		/**
		*  @brief
		*    Check whether the box is in the plane set or not
		*
		*  @param[in] cBox
		*    Box to check
		*  @param[in] cPlaneSet
		*    Plane set to check
		*
		*  @return
		*    'true' if the box is in the plane set, else 'false'
		*/
		static PLMATH_API bool BoxPlaneSet(const BoundingBox &cBox, const PlaneSet &cPlaneSet);

		//[-------------------------------------------------------]
		//[ Plane                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Checks if there's a plane/ray intersection
		*
		*  @param[in] cPlane
		*    Plane to check
		*  @param[in] vRayDirection
		*    Ray direction (must be normalized)
		*
		*  @return
		*    'true' if the ray intersects the plane, else 'false' (ray is parallel to plane)
		*/
		static PLMATH_API bool IsPlaneRay(const Plane &cPlane, const Vector3 &vRayDirection);

		/**
		*  @brief
		*    Returns the plane/ray intersection point
		*
		*  @param[in] cPlane
		*    Plane to check
		*  @param[in] vRayOrigin
		*    Ray origin position
		*  @param[in] vRayDirection
		*    Ray direction (must be normalized)
		*  @param[out] vIntersectionPointPos
		*    Will receive the position of the intersection point, do only use the result when this method returned 'true'
		*
		*  @return
		*    'true' if the ray intersects the plane, else 'false'
		*
		*  @note
		*    - By definition, a ray has an infinite length, in both directions
		*    - "PlaneRayPositive()" only returns "true" for intersection with positive distance to plane
		*    - "PlaneRayNegative()" only returns "true" for intersection with negative distance to plane
		*/
		static PLMATH_API bool PlaneRay(const Plane &cPlane, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 &vIntersectionPointPos);
		static PLMATH_API bool PlaneRayPositive(const Plane &cPlane, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 &vIntersectionPointPos);
		static PLMATH_API bool PlaneRayNegative(const Plane &cPlane, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 &vIntersectionPointPos);

		/**
		*  @brief
		*    Checks if there's a plane/line intersection
		*
		*  @param[in]  cPlane
		*    Plane to check
		*  @param[in]  vStartPos
		*    Line start position
		*  @param[in]  vEndPos
		*    Line end direction
		*  @param[out] pvPos
		*    If not a null pointer this will receive the intersection point (if there is any :)
		*
		*  @return
		*    Factor between 0-1 if there was an intersection, -1.0f if no intersection
		*/
		static PLMATH_API float PlaneLine(const Plane &cPlane, const Vector3 &vStartPos, const Vector3 &vEndPos, Vector3 *pvPos = nullptr);

		/**
		*  @brief
		*    Checks if there's a plane/plane intersection
		*
		*  @param[in] cPlane
		*    Plane to check
		*  @param[in] cPlane2
		*    Plane2 to test with
		*
		*  @return
		*    'true' if the plane intersects the plane, else 'false'
		*/
		static PLMATH_API bool PlanePlane(const Plane &cPlane, const Plane &cPlane2);

		/**
		*  @brief
		*    Checks if there's a plane/plane intersection and calculates the intersection ray
		*
		*  @param[in]  cPlane
		*    Plane to check
		*  @param[in]  cPlane2
		*    Second plane
		*  @param[out] cRay
		*    Receives the intersection ray/line
		*
		*  @return
		*    'true' if theres a plane intersection, else 'false'
		*/
		static PLMATH_API bool PlanePlane(const Plane &cPlane, const Plane &cPlane2, Ray &cRay);

		/**
		*  @brief
		*    Checks if there's an intersection between the three planes and returns the
		*    intersection point
		*
		*  @param[in]  cP1
		*    Plane to check
		*  @param[in]  cP2
		*    Plane to intersect with
		*  @param[in]  cP3
		*    Plane to intersect with
		*  @param[out] vRes
		*    Receives the intersection point
		*
		*  @return
		*    'true' if there's an intersection between the three planes, else 'false'
		*/
		static PLMATH_API bool PlanePlanePlane(const Plane &cP1, const Plane &cP2, const Plane &cP3, Vector3 &vRes);

		//[-------------------------------------------------------]
		//[ Plane set                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Test whether a point is in the plane set
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in] vPoint
		*    Position of the point
		*
		*  @return
		*    'true' if the point is in plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetPoint(const PlaneSet &cPlaneSet, const Vector3 &vPoint);

		/**
		*  @brief
		*    Test whether a point is in the plane set
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in] vPoint
		*    Position of the point
		*
		*  @return
		*    'true' if the point is in plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetPoint(const PlaneSet &cPlaneSet, const Vector4 &vPoint);

		/**
		*  @brief
		*    Test whether the given points are in the plane set
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in] lstPoints
		*    Array of all points to test
		*
		*  @return
		*    'true' if the points are in plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetPoints(const PlaneSet &cPlaneSet, const PLCore::Array<Vector3> &lstPoints);

		/**
		*  @brief
		*    Test whether the given points are in the plane set
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in] lstPoints
		*    Array of all points to test
		*
		*  @return
		*    'true' if the points are in plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetPoints(const PlaneSet &cPlaneSet, const PLCore::Array<Vector4> &lstPoints);

		/**
		*  @brief
		*    Test whether a sphere is in the plane set
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in] vSphereOrigin
		*    Middle of the sphere
		*  @param[in] fSphereRadius
		*    Sphere radius
		*
		*  @return
		*    'true' if the sphere is in plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetSphere(const PlaneSet &cPlaneSet, const Vector3 &vSphereOrigin, float fSphereRadius);

		/**
		*  @brief
		*    Test whether a triangle is in the plane set
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in] vV1
		*    First triangle point
		*  @param[in] vV2
		*    Second triangle point
		*  @param[in] vV3
		*    Third triangle point
		*
		*  @return
		*    'true' if the triangle is in plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetTriangle(const PlaneSet &cPlaneSet, const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);

		/**
		*  @brief
		*    Tests whether an axis aligned box is within the plane set or not
		*
		*  @param[in] cPlaneSet
		*    Plane set to check
		*  @param[in]  vMin
		*    Minimum position
		*  @param[in]  vMax
		*    Maximum position
		*  @param[out] pnOutClipMask
		*    If not a null pointer, this clip mask will receive the intersection state of a
		*    maximum number of 32 planes if the box intersects the plane set. If this mask is
		*    0, there was no plane intersection at all. (= complete inside/outside the plane set)
		*
		*  @return
		*    'true' if the axis aligned box is within plane set, else 'false'
		*/
		static PLMATH_API bool PlaneSetAABox(const PlaneSet &cPlaneSet, const Vector3 &vMin, const Vector3 &vMax, PLCore::uint32 *pnOutClipMask = nullptr);

		//[-------------------------------------------------------]
		//[ Triangle                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Checks if there's a triangle/ray intersection
		*
		*  @param[in]  vV1
		*    First triangle point
		*  @param[in]  vV2
		*    Second triangle point
		*  @param[in]  vV3
		*    Third triangle point
		*  @param[in]  vN
		*    Triangle plane normal
		*  @param[in]  vRayOrigin
		*    Ray origin
		*  @param[in]  vRayDirection
		*    Ray direction vector
		*  @param[out] vIntersectionPointPos
		*    Will receive the position of the intersection point, do only use the result when this method returned 'true'
		*
		*  @return
		*    'true' if the ray intersects the triangle, else 'false'
		*
		*  @note
		*    - By definition, a ray has an infinite length, in both directions
		*    - "TriangleRayPositive()" only returns "true" for intersection with positive distance to triangle plane
		*    - "TriangleRayNegative()" only returns "true" for intersection with negative distance to triangle plane
		*/
		static PLMATH_API bool TriangleRay(const Vector3 &vV1, const Vector3 &vV2,
										   const Vector3 &vV3, const Vector3 &vN,
										   const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
										   Vector3 &vIntersectionPointPos);
		static PLMATH_API bool TriangleRayPositive(const Vector3 &vV1, const Vector3 &vV2,
												   const Vector3 &vV3, const Vector3 &vN,
												   const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
												   Vector3 &vIntersectionPointPos);
		static PLMATH_API bool TriangleRayNegative(const Vector3 &vV1, const Vector3 &vV2,
												   const Vector3 &vV3, const Vector3 &vN,
												   const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
												   Vector3 &vIntersectionPointPos);

		/**
		*  @brief
		*    Checks if there's a triangle/ray intersection
		*
		*  @param[in]  vV1
		*    First triangle point
		*  @param[in]  vV2
		*    Second triangle point
		*  @param[in]  vV3
		*    Third triangle point
		*  @param[in]  vN
		*    Triangle plane normal
		*  @param[in]  cRay
		*    Ray to check
		*  @param[out] vIntersectionPointPos
		*    Will receive the position of the intersection point, do only use the result when this method returned 'true'
		*
		*  @return
		*    'true' if the ray intersects the triangle, else 'false'
		*
		*  @note
		*    - By definition, a ray has an infinite length, in both directions
		*    - "TriangleRayPositive()" only returns "true" for intersection with positive distance to triangle plane
		*    - "TriangleRayNegative()" only returns "true" for intersection with negative distance to triangle plane
		*/
		static PLMATH_API bool TriangleRay(const Vector3 &vV1, const Vector3 &vV2,
										   const Vector3 &vV3, const Vector3 &vN,
										   const Ray &cRay, Vector3 &vIntersectionPointPos);
		static PLMATH_API bool TriangleRayPositive(const Vector3 &vV1, const Vector3 &vV2,
												   const Vector3 &vV3, const Vector3 &vN,
												   const Ray &cRay, Vector3 &vIntersectionPointPos);
		static PLMATH_API bool TriangleRayNegative(const Vector3 &vV1, const Vector3 &vV2,
												   const Vector3 &vV3, const Vector3 &vN,
												   const Ray &cRay, Vector3 &vIntersectionPointPos);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_INTERSECT_H__
