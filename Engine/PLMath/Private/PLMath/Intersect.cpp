/*********************************************************\
 *  File: Intersect.cpp                                  *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Ray.h"
#include "PLMath/Line.h"
#include "PLMath/Plane.h"
#include "PLMath/Sphere.h"
#include "PLMath/PlaneSet.h"
#include "PLMath/BoundingBox.h"
#include "PLMath/AABoundingBox.h"
#include "PLMath/Intersect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Sphere intersection                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check whether there's a sphere/point intersection
*/
bool Intersect::SpherePoint(const Sphere &cSphere, const Vector3 &cPoint)
{
	const Vector3 vDifference = cSphere.GetPos() - cPoint;
	return (vDifference.DotProduct(vDifference) <= cSphere.GetRadius()*cSphere.GetRadius());
}

bool Intersect::SpherePoint(const Sphere &cSphere, const Vector4 &cPoint)
{
	const Vector3 vDifference = cSphere.GetPos() - cPoint;
	return (vDifference.DotProduct(vDifference) <= cSphere.GetRadius()*cSphere.GetRadius());
}

/**
*  @brief
*    Check whether there's a sphere/ray intersection
*/
float Intersect::SphereRay(const Sphere &cSphere, const Vector3 &vRayOrigin, const Vector3 &vRayDirection)
{
	const Vector3 vDelta = cSphere.GetPos() - vRayOrigin;

	const float fC = vDelta.GetSquaredLength();
	const float fV = vDelta.DotProduct(vRayDirection);
	const float fD = cSphere.GetRadius()*cSphere.GetRadius() - (fC - fV*fV);

	// Return the distance to the (first) intersecting point
	return (fD >= 0.0f) ? (fV - Math::Sqrt(fD)) : -1.0f;
}

/**
*  @brief
*    Check whether there's a sphere/ray intersection
*/
// Ray-sphere intersection test from Graphics Gems p.388
// **NOTE** There is a bug in this Graphics Gem. If the origin
// of the ray is *inside* the sphere being tested, it reports the
// wrong intersection location. This code has a fix for the bug.
bool Intersect::SphereRay(const Sphere &cSphere, const Vector3 &vRayOrigin,
						  const Vector3 &vRayDirection, Vector3 *pvIntersect)
{
	const Vector3 &vPos    = cSphere.GetPos();
	const float	   fRadius = cSphere.GetRadius();

	// Notation:
	// point E = ray origin
	// point O = sphere center

	const Vector3 vEO = vPos - vRayOrigin;
		  Vector3 vV  = vRayDirection;
	const float fDist2 = vEO.x*vEO.x + vEO.y*vEO.y + vEO.z*vEO.z;

	// Bug Fix For Gem, if origin is *inside* the sphere, invert the
	// direction vector so that we get a valid intersection location
	if (fDist2 < fRadius*fRadius)
		vV.Invert();

	const float v = vEO.DotProduct(vV);

	const float fDisc = fRadius*fRadius - (vEO.GetSquaredLength() - v*v);
	if (fDisc > 0.0f) {
		if (pvIntersect) {
			const float d = Math::Sqrt(fDisc);
			*pvIntersect = vRayOrigin + vV*(v-d);
		}
		return true;
	} else {
		return false;
	}
}

/**
*  @brief
*    Check whether there's a sphere/ray intersection
*/
bool Intersect::SphereRay(const Sphere &cSphere, const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
						  float fDistance, Vector3 *pvIntersect)
{
	Vector3 vSect;
	const bool bHit = SphereRayInFront(cSphere, vRayOrigin, vRayDirection, &vSect);
	if (bHit) {
		const float d = vRayOrigin.GetSquaredDistance(vSect);
		if (d <= fDistance*fDistance) {
			if (pvIntersect)
				*pvIntersect = vSect;
			return true;
		}
	}

	return false;
}

/**
*  @brief
*    Check whether there's a sphere/ray intersection
*/
bool Intersect::SphereRayInFront(const Sphere &cSphere, const Vector3 &vRayOrigin,
								 const Vector3 &vRayDirection, Vector3 *pvIntersect)
{
	Vector3 vSect;
	const bool bHit = SphereRay(cSphere, vRayOrigin, vRayDirection, &vSect);
	if (bHit) {
		const Vector3 vDir = vSect - vRayOrigin;
		const float fDot = vDir.DotProduct(vRayDirection);
		if (fDot >= 0) { // Then it's in front!
			if (pvIntersect)
				*pvIntersect = vSect;
			return true;
		}
	}

	return false;
}

/**
*  @brief
*    Check whether there's a sphere/line intersection
*/
bool Intersect::SphereLine(const Sphere &cSphere, const Vector3 &vStart, const Vector3 &vEnd)
{
	const Vector3 vDelta		   = cSphere.GetPos() - vStart;
		  Vector3 vDir			   = vEnd - vStart;
	const float   fSquaredDistance = vDir.GetSquaredLength();

	vDir.Normalize();
	const float fC = vDelta.GetSquaredLength();
		  float fV = vDelta.DotProduct(vDir);
	fV *= fV;
	const float fD = cSphere.GetRadius()*cSphere.GetRadius() - (fC - fV);

	// Return if the intersection point is within the line
	return (fD >= 0.0f && (fV - fD) <= fSquaredDistance);
}

/**
*  @brief
*    Check whether there's a sphere/sphere intersection
*/
bool Intersect::SphereSphere(const Sphere &cSphere, const Vector3 &vPos, float fRadius)
{
	const float fComposedRadius = cSphere.GetRadius()+fRadius;
	return ((vPos-cSphere.GetPos()).GetSquaredLength() <= fComposedRadius*fComposedRadius);
}

/**
*  @brief
*    Check whether there's a sphere/sphere intersection
*/
bool Intersect::SphereSphere(const Sphere &cSphere, const Sphere &cSphere2)
{
	const float fComposedRadius = cSphere.GetRadius()+cSphere2.GetRadius();
	return ((cSphere2.GetPos()-cSphere.GetPos()).GetSquaredLength() <= fComposedRadius*fComposedRadius);
}

/**
*  @brief
*    Check whether a sphere intersects with this sphere while moving
*/
bool Intersect::SphereSphere(const Sphere &cSphere, const Sphere &cSphere2, const Vector3 &vMove1, const Vector3 &vMove2)
{
	// Note: A = this, B = the other
	const Vector3 vMoveVec = vMove2 - vMove1;

	// Early escape test: If the length of the move vector is less
	// than distance between the centers of these spheres minus
	// their radii, there's no way they can hit.
	const Vector3 vDelta  = cSphere2.GetPos() - cSphere.GetPos();
	const float fSumRadii = cSphere2.GetRadius() + cSphere.GetRadius();
	const float fMoveDot  = vMoveVec.DotProduct(vMoveVec);

	if (fMoveDot >= vDelta.DotProduct(vDelta) - fSumRadii*fSumRadii) {
		// Another early escape: Make sure that A is moving
		// towards B! If the dot product between the move vector and
		// the delta position is less that or equal to 0,
		// A isn't isn't moving towards B!
		const Vector3 vN = vDelta.GetNormalized();
		float fD = vN.DotProduct(vDelta);
		if (fD > 0) {
			// Square 'fD', we will need it twice
			fD *= fD;

			// Escape test: If the closest that A will get to B
			// is more than the sum of their radii, there's no
			// way they are going collide
			const float fSquaredLength   = vDelta.GetSquaredLength();
			const float fF			     = fSquaredLength - fD;
			const float fSumRadiiSquared = fSumRadii*fSumRadii;
			if (fF < fSumRadiiSquared) {
				// We now have fF and fSumRadii, two sides of a right triangle.
				// Use these to find the third side, sqrt(fT)
				const float fT = fSumRadiiSquared - fF;

				// If there is no such right triangle with sides length of
				// fSumRadii and sqrt(fF), fT will probably be less than 0.
				// Better to check now than perform a square root of a
				// negative number.
				if (fT >= 0) {
					// Therefore the distance the spheres has to travel along
					// the move vector is:
					const float fSquaredDistance = fD - fT;

					// Finally, make sure that the distance A has to move
					// to touch B is not greater than the magnitude of the
					// movement vector.
					return (fMoveDot*fMoveDot >= fSquaredDistance);
				}
			}
		}
	}

	// No intersection
	return false;
}

/**
*  @brief
*    Check whether there's a sphere/axis aligned box intersection
*/
bool Intersect::SphereAABox(const Vector3 &vSpherePos, float fSphereRadius, const AABoundingBox &cAABox)
{
	return AABoxSphere(cAABox.vMin, cAABox.vMax, vSpherePos, fSphereRadius);
}

/**
*  @brief
*    Check whether there's a sphere/axis aligned box intersection
*/
bool Intersect::SphereAABox(const Sphere &cSphere, const AABoundingBox &cAABox)
{
	return SphereAABox(cSphere.GetPos(), cSphere.GetRadius(), cAABox);
}

/**
*  @brief
*    Check whether there's a sphere/box intersection
*/
bool Intersect::SphereBox(const Vector3 &vSpherePos, float fSphereRadius, const BoundingBox &cBox)
{
	// Check sphere radius
	if (fSphereRadius > 0) {
		// Transform first corner into world space coordinates
		const Vector3 vC1 = cBox.m_mFinalRot*cBox.m_vCorner1*cBox.m_vScale + cBox.m_vPos;

		float d = cBox.m_vX.DotProduct(vSpherePos - vC1);
		if (Math::Abs(d) <= fSphereRadius) { // Possible collision
			const Vector3 a = vSpherePos - cBox.m_vX*d;
			const float   x =  cBox.m_vX.DotProduct(a - vC1);
			const float   y =  cBox.m_vY.DotProduct(a - vC1);
			const float   z = -cBox.m_vZ.DotProduct(a - vC1);
			if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
				return true;
		}

		d = cBox.m_vY.DotProduct(vSpherePos - vC1);
		if (Math::Abs(d) <= fSphereRadius) { // Possible collision
			const Vector3 a = vSpherePos - cBox.m_vY*d;
			const float   x =  cBox.m_vX.DotProduct(a - vC1);
			const float   y =  cBox.m_vY.DotProduct(a - vC1);
			const float   z = -cBox.m_vZ.DotProduct(a - vC1);
			if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
				return true;
		}

		d = -cBox.m_vZ.DotProduct(vSpherePos - vC1);
		if (Math::Abs(d) <= fSphereRadius) { // Possible collision
			const Vector3 a = vSpherePos + cBox.m_vZ*d;
			const float   x =  cBox.m_vX.DotProduct(a - vC1);
			const float   y =  cBox.m_vY.DotProduct(a - vC1);
			const float   z = -cBox.m_vZ.DotProduct(a - vC1);
			if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
				return true;
		}

		// Transform second corner into world space coordinates
		const Vector3 vC2 = cBox.m_mFinalRot*cBox.m_vCorner2*cBox.m_vScale + cBox.m_vPos;

		d = cBox.m_vX.DotProduct(vSpherePos - vC2);
		if (Math::Abs(d) <= fSphereRadius) { // Possible collision
			const Vector3 a = vSpherePos - cBox.m_vX*d;
			const float   x =  cBox.m_vX.DotProduct(a - vC1);
			const float   y =  cBox.m_vY.DotProduct(a - vC1);
			const float   z = -cBox.m_vZ.DotProduct(a - vC1);
			if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
				return true;
		}

		d = cBox.m_vY.DotProduct(vSpherePos - vC2);
		if (Math::Abs(d) <= fSphereRadius) { // Possible collision
			const Vector3 a = vSpherePos - cBox.m_vY*d;
			const float   x =  cBox.m_vX.DotProduct(a - vC1);
			const float   y =  cBox.m_vY.DotProduct(a - vC1);
			const float   z = -cBox.m_vZ.DotProduct(a - vC1);
			if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
				return true;
		}

		d = -cBox.m_vZ.DotProduct(vSpherePos - vC2);
		if (Math::Abs(d) <= fSphereRadius) { // Possible collision
			const Vector3 a = vSpherePos + cBox.m_vZ*d;
			const float   x =  cBox.m_vX.DotProduct(a - vC1);
			const float   y =  cBox.m_vY.DotProduct(a - vC1);
			const float   z = -cBox.m_vZ.DotProduct(a - vC1);
			if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
				return true;
		}
	}

	return false;
}

/**
*  @brief
*    Check whether there's a sphere/box intersection
*/
bool Intersect::SphereBox(const Sphere &cSphere, const BoundingBox &cBox)
{
	return SphereBox(cSphere.GetPos(), cSphere.GetRadius(), cBox);
}


//[-------------------------------------------------------]
//[ Axis aligned box intersection                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check whether a point is inside the axis aligned box
*/
bool Intersect::AABoxPoint(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Vector3 &vPos)
{
	return (vAABoxMin.x <= vPos.x && vAABoxMin.y <= vPos.y && vAABoxMin.z <= vPos.z &&
		    vAABoxMax.x >= vPos.x && vAABoxMax.y >= vPos.y && vAABoxMax.z >= vPos.z);
}

/**
*  @brief
*    Check whether there's a axis aligned box/line intersection
*/
bool Intersect::AABoxLine(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax,
						  const Vector3 &vStart, const Vector3 &vEnd, float *pfIntersection)
{
	if (pfIntersection)
		*pfIntersection = -1.0f;
	if (vAABoxMin == vAABoxMax)
		return false;

	// Check origin inside first
	const Vector3 &vRayOrigin = vStart;
	if (AABoxPoint(vAABoxMin, vAABoxMax, vRayOrigin)) {
		// The ray origin is inside the bounding box
		return true;
	}

	// Check each face in turn, only check closest 3
	const Vector3 vRayDirection = (vEnd - vStart).Normalize();
	bool  bHit  = false;
	float fLowt = 0.0f;

	// Min x
	if (vRayOrigin.x < vAABoxMin.x && vRayDirection.x > 0) {
		const float t = (vAABoxMin.x - vRayOrigin.x)/vRayDirection.x;
		if (t > 0) {
			// Substitute t back into ray and check bounds and dist
			const Vector3 vHitpoint = vRayOrigin + vRayDirection*t;
			if (vHitpoint.y >= vAABoxMin.y && vHitpoint.y <= vAABoxMax.y &&
				vHitpoint.z >= vAABoxMin.z && vHitpoint.z <= vAABoxMax.z &&
				(!bHit || t < fLowt)) {
				bHit  = true;
				fLowt = t;
			}
		}
	}
	// Max x
	if (vRayOrigin.x > vAABoxMax.x && vRayDirection.x < 0) {
		const float t = (vAABoxMax.x - vRayOrigin.x)/vRayDirection.x;
		if (t > 0) {
			// Substitute t back into ray and check bounds and dist
			const Vector3 vHitpoint = vRayOrigin + vRayDirection*t;
			if (vHitpoint.y >= vAABoxMin.y && vHitpoint.y <= vAABoxMax.y &&
				vHitpoint.z >= vAABoxMin.z && vHitpoint.z <= vAABoxMax.z &&
				(!bHit || t < fLowt)) {
				bHit  = true;
				fLowt = t;
			}
		}
	}
	// Min y
	if (vRayOrigin.y < vAABoxMin.y && vRayDirection.y > 0) {
		const float t = (vAABoxMin.y - vRayOrigin.y)/vRayDirection.y;
		if (t > 0) {
			// Substitute t back into ray and check bounds and dist
			const Vector3 vHitpoint = vRayOrigin + vRayDirection*t;
			if (vHitpoint.x >= vAABoxMin.x && vHitpoint.x <= vAABoxMax.x &&
				vHitpoint.z >= vAABoxMin.z && vHitpoint.z <= vAABoxMax.z &&
				(!bHit || t < fLowt)) {
				bHit  = true;
				fLowt = t;
			}
		}
	}
	// Max y
	if (vRayOrigin.y > vAABoxMax.y && vRayDirection.y < 0) {
		const float t = (vAABoxMax.y - vRayOrigin.y)/vRayDirection.y;
		if (t > 0) {
			// Substitute t back into ray and check bounds and dist
			const Vector3 vHitpoint = vRayOrigin + vRayDirection*t;
			if (vHitpoint.x >= vAABoxMin.x && vHitpoint.x <= vAABoxMax.x &&
				vHitpoint.z >= vAABoxMin.z && vHitpoint.z <= vAABoxMax.z &&
				(!bHit || t < fLowt)) {
				bHit  = true;
				fLowt = t;
			}
		}
	}
	// Min z
	if (vRayOrigin.z < vAABoxMin.z && vRayDirection.z > 0) {
		const float t = (vAABoxMin.z - vRayOrigin.z)/vRayDirection.z;
		if (t > 0) {
			// Substitute t back into ray and check bounds and dist
			const Vector3 vHitpoint = vRayOrigin + vRayDirection*t;
			if (vHitpoint.x >= vAABoxMin.x && vHitpoint.x <= vAABoxMax.x &&
				vHitpoint.y >= vAABoxMin.y && vHitpoint.y <= vAABoxMax.y &&
				(!bHit || t < fLowt)) {
				bHit  = true;
				fLowt = t;
			}
		}
	}
	// Max z
	if (vRayOrigin.z > vAABoxMax.z && vRayDirection.z < 0) {
		const float t = (vAABoxMax.z - vRayOrigin.z)/vRayDirection.z;
		if (t > 0) {
			// Substitute t back into ray and check bounds and dist
			const Vector3 vHitpoint = vRayOrigin + vRayDirection*t;
			if (vHitpoint.x >= vAABoxMin.x && vHitpoint.x <= vAABoxMax.x &&
				vHitpoint.y >= vAABoxMin.y && vHitpoint.y <= vAABoxMax.y &&
				(!bHit || t < fLowt)) {
				bHit  = true;
				fLowt = t;
			}
		}
	}

	// Return the result
	if (bHit && fLowt <= (vEnd-vStart).GetLength()) {
		if (pfIntersection)
			*pfIntersection = fLowt;

		// Intersection
		return true;
	} else {
		// No intersection
		return false;
	}
}

/**
*  @brief
*    Check whether there's a axis aligned box/line intersection
*/
bool Intersect::AABoxLine(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Line &cLine, float *pfIntersection)
{
	return AABoxLine(vAABoxMin, vAABoxMax, cLine.vStart, cLine.vEnd, pfIntersection);
}

/**
*  @brief
*    Check whether there's a axis aligned box/sphere intersection
*/
bool Intersect::AABoxSphere(const Vector3 &vAABoxMin, const Vector3 &vAABoxMax, const Vector3 &vSpherePos, float fSphereRadius)
{
	// Check each axis
	return ((vSpherePos.x >= vAABoxMin.x || vAABoxMin.x - vSpherePos.x  <= fSphereRadius) &&
		    (vSpherePos.x <= vAABoxMax.x || vSpherePos.x  - vAABoxMax.x <= fSphereRadius) &&
		    (vSpherePos.y >= vAABoxMin.y || vAABoxMin.y - vSpherePos.y  <= fSphereRadius) &&
		    (vSpherePos.y <= vAABoxMax.y || vSpherePos.y  - vAABoxMax.y <= fSphereRadius) &&
		    (vSpherePos.z >= vAABoxMin.z || vAABoxMin.z - vSpherePos.z  <= fSphereRadius) &&
		    (vSpherePos.z <= vAABoxMax.z || vSpherePos.z  - vAABoxMax.z <= fSphereRadius));
}

/**
*  @brief
*    Check whether there's a axis aligned box/axis aligned box intersection
*/
bool Intersect::AABoxAABox(const Vector3 &vAABoxMin1, const Vector3 &vAABoxMax1,
						   const Vector3 &vAABoxMin2, const Vector3 &vAABoxMax2)
{
	return (vAABoxMax1.x >= vAABoxMin2.x && vAABoxMax1.y >= vAABoxMin2.y && vAABoxMax1.z >= vAABoxMin2.z &&
		    vAABoxMin1.x <= vAABoxMax2.x && vAABoxMin1.y <= vAABoxMax2.y && vAABoxMin1.z <= vAABoxMax2.z);
}

/**
*  @brief
*    Check whether there's a axis aligned box/axis aligned box intersection
*/
bool Intersect::AABoxAABox(const AABoundingBox &cBox1, const AABoundingBox &cBox2)
{
	return (cBox1.vMax.x >= cBox2.vMin.x && cBox1.vMax.y >= cBox2.vMin.y && cBox1.vMax.z >= cBox2.vMin.z &&
			cBox1.vMin.x <= cBox2.vMax.x && cBox1.vMin.y <= cBox2.vMax.y && cBox1.vMin.z <= cBox2.vMax.z);
}


//[-------------------------------------------------------]
//[ Box                                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check whether a point is inside the box
*/
bool Intersect::BoxPoint(const BoundingBox &cBox, const Vector3 &vPos)
{
	// Transform corners into world space coordinates
	const Vector3 vC1 = cBox.m_mFinalRot*cBox.m_vCorner1*cBox.m_vScale + cBox.m_vPos;
	const Vector3 vC2 = cBox.m_mFinalRot*cBox.m_vCorner2*cBox.m_vScale + cBox.m_vPos;

	// Check plane pair #1, #2 and #3
	return (Math::Sign(cBox.m_vX.DotProduct(vPos - vC1)) != Math::Sign(cBox.m_vX.DotProduct(vPos - vC2)) &&
		    Math::Sign(cBox.m_vY.DotProduct(vPos - vC1)) != Math::Sign(cBox.m_vY.DotProduct(vPos - vC2)) &&
		    Math::Sign(cBox.m_vZ.DotProduct(vPos - vC1)) != Math::Sign(cBox.m_vZ.DotProduct(vPos - vC2)));
}

/**
*  @brief
*    Check whether a line intersects with the box
*/
bool Intersect::BoxLine(const BoundingBox &cBox, const Vector3 &vA, const Vector3 &vB)
{
	// Transform corners into world space coordinates
	const Vector3 vC1 = cBox.m_mFinalRot*cBox.m_vCorner1*cBox.m_vScale + cBox.m_vPos;
	const Vector3 vC2 = cBox.m_mFinalRot*cBox.m_vCorner2*cBox.m_vScale + cBox.m_vPos;

	// Check with each plane
	const Vector3 r = vB - vA;
	Vector3 n = cBox.m_vX;
	float   t = (n.DotProduct(vC1) - n.DotProduct(vA))/n.DotProduct(r);
	if (t>=0 && t<=1) { // Point in plane, but where in plane?
		const Vector3 q = vA + r*t;

		// Get position in local axis coordinates
		const float x =  cBox.m_vX.DotProduct(q - vC1);
		const float y =  cBox.m_vY.DotProduct(q - vC1);
		const float z = -cBox.m_vZ.DotProduct(q - vC1);
		if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
			return true;
	}

	n = cBox.m_vY;
	t = (n.DotProduct(vC1) - n.DotProduct(vA))/n.DotProduct(r);
	if (t>=0 && t<=1) { // Point in plane, but where in plane?
		const Vector3 q = vA + r*t;

		// Get position in local axis coordinates
		const float x =  cBox.m_vX.DotProduct(q - vC1);
		const float y =  cBox.m_vY.DotProduct(q - vC1);
		const float z = -cBox.m_vZ.DotProduct(q - vC1);
		if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
			return true;
	}

	n = -cBox.m_vZ;
	t = (n.DotProduct(vC1) - n.DotProduct(vA))/n.DotProduct(r);
	if (t>=0 && t<=1) { // Point in plane, but where in plane?
		const Vector3 q = vA + r*t;

		// Get position in local axis coordinates
		const float x =  cBox.m_vX.DotProduct(q - vC1);
		const float y =  cBox.m_vY.DotProduct(q - vC1);
		const float z = -cBox.m_vZ.DotProduct(q - vC1);
		if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
			return true;
	}

	n = cBox.m_vX;
	t = (n.DotProduct(vC2) - n.DotProduct(vA))/n.DotProduct(r);
	if (t>=0 && t<=1) { // Point in plane, but where in plane?
		const Vector3 q = vA + r*t;

		// Get position in local axis coordinates
		const float x =  cBox.m_vX.DotProduct(q - vC1);
		const float y =  cBox.m_vY.DotProduct(q - vC1);
		const float z = -cBox.m_vZ.DotProduct(q - vC1);
		if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
			return true;
	}

	n = cBox.m_vY;
	t = (n.DotProduct(vC2) - n.DotProduct(vA))/n.DotProduct(r);
	if (t>=0 && t<=1) { // Point in plane, but where in plane?
		const Vector3 q = vA + r*t;

		// Get position in local axis coordinates
		const float x =  cBox.m_vX.DotProduct(q - vC1);
		const float y =  cBox.m_vY.DotProduct(q - vC1);
		const float z = -cBox.m_vZ.DotProduct(q - vC1);
		if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
			return true;
	}

	n = -cBox.m_vZ;
	t = (n.DotProduct(vC2) - n.DotProduct(vA))/n.DotProduct(r);
	if (t>=0 && t<=1) { // Point in plane, but where in plane?
		const Vector3 q = vA + r*t;

		// Get position in local axis coordinates
		const float x =  cBox.m_vX.DotProduct(q - vC1);
		const float y =  cBox.m_vY.DotProduct(q - vC1);
		const float z = -cBox.m_vZ.DotProduct(q - vC1);
		if (x>=0 && x<=cBox.m_fW && y>=0 && y<=cBox.m_fH && z>=0 && z<=cBox.m_fD)
			return true;
	}

	return false;
}

/**
*  @brief
*    Check whether another box intersects with this box
*/
bool Intersect::BoxBox(const BoundingBox &cBox1, const BoundingBox &cBox2)
{
	// Definition
	static const float ParallelApprox = 0.999999f;

	// Get distance of the boxes
	Vector3 vDist = cBox2.m_vCenter - cBox1.m_vCenter;

	// Compute dot products XX, XY, XZ
	bool bParallelAxis = false;
	float fDotXX = Math::Abs(cBox1.m_vX.DotProduct(cBox2.m_vX));
	if (fDotXX > ParallelApprox)
		bParallelAxis = true;
	float fDotXY = Math::Abs(cBox1.m_vX.DotProduct(cBox2.m_vY));
	if (fDotXY > ParallelApprox)
		bParallelAxis = true;
	float fDotXZ = Math::Abs(cBox1.m_vX.DotProduct(cBox2.m_vZ));
	if (fDotXZ > ParallelApprox)
		bParallelAxis = true;

	// Test for m_vX
	float fR1 = cBox2.m_fExtX*fDotXX + cBox2.m_fExtY*fDotXY + cBox2.m_fExtZ*fDotXZ;
	float fR  = Math::Abs(cBox1.m_vX.DotProduct(vDist));
	if (fR > cBox1.m_fExtX + fR1)
		return false;

	// Compute dot products YX, YY, YZ
	float fDotYX = Math::Abs(cBox1.m_vY.DotProduct(cBox2.m_vX));
	if (fDotYX > ParallelApprox)
		bParallelAxis = true;
	float fDotYY = Math::Abs(cBox1.m_vY.DotProduct(cBox2.m_vY));
	if (fDotYY > ParallelApprox)
		bParallelAxis = true;
	float fDotYZ = Math::Abs(cBox1.m_vY.DotProduct(cBox2.m_vZ));
	if (fDotYZ > ParallelApprox)
		bParallelAxis = true;

	// Test for m_vY
	fR1 = cBox2.m_fExtX*fDotYX + cBox2.m_fExtY*fDotYY + cBox2.m_fExtZ*fDotYZ;
	fR  = Math::Abs(cBox1.m_vY.DotProduct(vDist));
	if (fR > cBox1.m_fExtY + fR1)
		return false;

	// Compute dot products ZX, ZY, ZZ
	float fDotZX = Math::Abs(cBox1.m_vZ.DotProduct(cBox2.m_vX));
	if (fDotZX > ParallelApprox)
		bParallelAxis = true;
	float fDotZY = Math::Abs(cBox1.m_vZ.DotProduct(cBox2.m_vY));
	if (fDotZY > ParallelApprox)
		bParallelAxis = true;
	float fDotZZ = Math::Abs(cBox1.m_vZ.DotProduct(cBox2.m_vZ));
	if (fDotZZ > ParallelApprox)
		bParallelAxis = true;

	// Test for m_vZ
	fR1 = cBox2.m_fExtX*fDotZX + cBox2.m_fExtY*fDotZY + cBox2.m_fExtZ*fDotZZ;
	fR  = Math::Abs(cBox1.m_vZ.DotProduct(vDist));
	if (fR > cBox1.m_fExtZ + fR1)
		return false;

	// Test for cBox2.m_vX
	float fR0 = cBox1.m_fExtX*fDotXX + cBox1.m_fExtY*fDotYX + cBox1.m_fExtZ*fDotZX;
	fR = Math::Abs(cBox2.m_vX.DotProduct(vDist));
	if (fR > fR0 + cBox2.m_fExtX)
		return false;

	// Test for cBox2.m_vY
	fR0 = cBox1.m_fExtX*fDotXY + cBox1.m_fExtY*fDotYY + cBox1.m_fExtZ*fDotZY;
	fR  = Math::Abs(cBox2.m_vY.DotProduct(vDist));
	if (fR > fR0 + cBox2.m_fExtY)
		return false;

	// Test for cBox2.m_vZ
	fR0 = cBox1.m_fExtX*fDotXZ + cBox1.m_fExtY*fDotYZ + cBox1.m_fExtZ*fDotZZ;
	fR  = Math::Abs(cBox2.m_vZ.DotProduct(vDist));
	if (fR > fR0 + cBox2.m_fExtZ)
		return false;

	// If a pair of axis is parallel and none of the local axis separates the boxes,
	// we don't need to check the cross product axis, it's already sure that a
	// collision has occurred
	if (bParallelAxis)
		return true;

	// Test for m_vX x cBox2.m_vX
	fR0 = cBox1.m_fExtY*fDotZX + cBox1.m_fExtZ*fDotYX;
	fR1 = cBox2.m_fExtY*fDotXZ + cBox2.m_fExtZ*fDotXY;
	fR  = Math::Abs(fDotYX*cBox1.m_vZ.DotProduct(vDist) - fDotZX*cBox1.m_vY.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vX x cBox2.m_vY
	fR0 = cBox1.m_fExtY*fDotZY + cBox1.m_fExtZ*fDotYY;
	fR1 = cBox2.m_fExtX*fDotXZ + cBox2.m_fExtZ*fDotXX;
	fR  = Math::Abs(fDotYY*cBox1.m_vZ.DotProduct(vDist) - fDotZY*cBox1.m_vY.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vX x cBox2.m_vZ
	fR0 = cBox1.m_fExtY*fDotZZ + cBox1.m_fExtZ*fDotYZ;
	fR1 = cBox2.m_fExtX*fDotXY + cBox2.m_fExtY*fDotXX;
	fR  = Math::Abs(fDotYZ*cBox1.m_vZ.DotProduct(vDist) - fDotZZ*cBox1.m_vY.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vY x cBox2.m_vX
	fR0 = cBox1.m_fExtX*fDotZX + cBox1.m_fExtZ*fDotXX;
	fR1 = cBox2.m_fExtY*fDotYZ + cBox2.m_fExtZ*fDotYY;
	fR  = Math::Abs(fDotZX*cBox1.m_vX.DotProduct(vDist) - fDotXX*cBox1.m_vZ.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vY x cBox2.m_vY
	fR0 = cBox1.m_fExtX*fDotZY + cBox1.m_fExtZ*fDotXY;
	fR1 = cBox2.m_fExtX*fDotYZ + cBox2.m_fExtZ*fDotYX;
	fR  = Math::Abs(fDotZY*cBox1.m_vX.DotProduct(vDist) - fDotXY*cBox1.m_vZ.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vY x cBox2.m_vZ
	fR0 = cBox1.m_fExtX*fDotZZ + cBox1.m_fExtZ*fDotXZ;
	fR1 = cBox2.m_fExtX*fDotYY + cBox2.m_fExtY*fDotYX;
	fR  = Math::Abs(fDotZZ*cBox1.m_vX.DotProduct(vDist) - fDotXZ*cBox1.m_vZ.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vZ x cBox2.m_vX
	fR0 = cBox1.m_fExtX*fDotYX + cBox1.m_fExtY*fDotXX;
	fR1 = cBox2.m_fExtY*fDotZZ + cBox2.m_fExtZ*fDotZY;
	fR  = Math::Abs(fDotXX*cBox1.m_vY.DotProduct(vDist) - fDotYX*cBox1.m_vX.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vZ x cBox2.m_vY
	fR0 = cBox1.m_fExtX*fDotYY + cBox1.m_fExtY*fDotXY;
	fR1 = cBox2.m_fExtX*fDotZZ + cBox2.m_fExtZ*fDotZX;
	fR  = Math::Abs(fDotXY*cBox1.m_vY.DotProduct(vDist) - fDotYY*cBox1.m_vX.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// Test for m_vZ x cBox2.m_vZ
	fR0 = cBox1.m_fExtX*fDotYZ + cBox1.m_fExtY*fDotXZ;
	fR1 = cBox2.m_fExtX*fDotZY + cBox2.m_fExtY*fDotZX;
	fR  = Math::Abs(fDotXZ*cBox1.m_vY.DotProduct(vDist) - fDotYZ*cBox1.m_vX.DotProduct(vDist));
	if (fR > fR0 + fR1)
		return false;

	// None of the axis separated the boxes, so there's a collision
	return true;
}

/**
*  @brief
*    Check whether another box intersects with this box while moving
*/
bool Intersect::BoxBox(const BoundingBox &cBox1, const BoundingBox &cBox2, const Vector3 &vMove1, const Vector3 &vMove2)
{
	// Get distance of the boxes and relative movement vector
	Vector3 vMove  = vMove2 - vMove1;
	Vector3 vDist0 = cBox2.m_vCenter - cBox1.m_vCenter;
	Vector3 vDist1 = vDist0 + vMove;

	// Test for m_vX
	float fDotXX = cBox1.m_vX.DotProduct(cBox2.m_vX); float fAbsXX = Math::Abs(fDotXX);
	float fDotXY = cBox1.m_vX.DotProduct(cBox2.m_vY); float fAbsXY = Math::Abs(fDotXY);
	float fDotXZ = cBox1.m_vX.DotProduct(cBox2.m_vZ); float fAbsXZ = Math::Abs(fDotXZ);
	float fDotXDist0 = cBox1.m_vX.DotProduct(vDist0);
	float fDotXDist1 = cBox1.m_vX.DotProduct(vDist1);
	float fR1  = cBox2.m_fExtX*fAbsXX + cBox2.m_fExtY*fAbsXY + cBox2.m_fExtZ*fAbsXZ;
	float fR01 = cBox1.m_fExtX + fR1;
	if ((fDotXDist0 > fR01 && fDotXDist1 > fR01) || (fDotXDist0 < -fR01 && fDotXDist1 < -fR01))
		return false;

	// Test for m_vY
	float fDotYX = cBox1.m_vY.DotProduct(cBox2.m_vX); float fAbsYX = Math::Abs(fDotYX);
	float fDotYY = cBox1.m_vY.DotProduct(cBox2.m_vY); float fAbsYY = Math::Abs(fDotYY);
	float fDotYZ = cBox1.m_vY.DotProduct(cBox2.m_vZ); float fAbsYZ = Math::Abs(fDotYZ);
	float fDotYDist0 = cBox1.m_vY.DotProduct(vDist0);
	float fDotYDist1 = cBox1.m_vY.DotProduct(vDist1);
	fR1 = cBox2.m_fExtX*fAbsYX + cBox2.m_fExtY*fAbsYY + cBox2.m_fExtZ*fAbsYZ;
	fR01 = cBox1.m_fExtY + fR1;
	if ((fDotYDist0 > fR01 && fDotYDist1 > fR01) || (fDotYDist0 < -fR01 && fDotYDist1 < -fR01))
		return false;

	// Test for m_vZ
	float fDotZX = cBox1.m_vZ.DotProduct(cBox2.m_vX); float fAbsZX = Math::Abs(fDotZX);
	float fDotZY = cBox1.m_vZ.DotProduct(cBox2.m_vY); float fAbsZY = Math::Abs(fDotZY);
	float fDotZZ = cBox1.m_vZ.DotProduct(cBox2.m_vZ); float fAbsZZ = Math::Abs(fDotZZ);
	float fDotZDist0 = cBox1.m_vZ.DotProduct(vDist0);
	float fDotZDist1 = cBox1.m_vZ.DotProduct(vDist1);
	fR1 = cBox2.m_fExtX*fAbsZX + cBox2.m_fExtY*fAbsZY + cBox2.m_fExtZ*fAbsZZ;
	fR01 = cBox1.m_fExtZ + fR1;
	if ((fDotZDist0 > fR01 && fDotZDist1 > fR01) || (fDotZDist0 < -fR01 && fDotZDist1 < -fR01))
		return false;

	// Test for cBox2.m_vX
	float fR  = cBox2.m_vX.DotProduct(vDist0);
	float fR0 = cBox1.m_fExtX*fAbsXX + cBox1.m_fExtY*fAbsYX + cBox1.m_fExtZ*fAbsZX;
	fR01 = fR0 + cBox2.m_fExtX;
	if (fR > fR01) {
		fR = cBox2.m_vX.DotProduct(vDist1);
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = cBox2.m_vX.DotProduct(vDist1);
		if (fR < -fR01)
			return false;
	}

	// Test for cBox2.m_vY
	fR  = cBox2.m_vY.DotProduct(vDist0);
	fR0 = cBox1.m_fExtX*fAbsXY + cBox1.m_fExtY*fAbsYY + cBox1.m_fExtZ*fAbsZY;
	fR01 = fR0 + cBox2.m_fExtY;
	if (fR > fR01) {
		fR = cBox2.m_vY.DotProduct(vDist1);
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = cBox2.m_vY.DotProduct(vDist1);
		if (fR < -fR01)
			return false;
	}

	// Test for cBox2.m_vZ
	fR  = cBox2.m_vZ.DotProduct(vDist0);
	fR0 = cBox1.m_fExtX*fAbsXZ + cBox1.m_fExtY*fAbsYZ + cBox1.m_fExtZ*fAbsZZ;
	fR01 = fR0 + cBox2.m_fExtZ;
	if (fR > fR01) {
		fR = cBox2.m_vZ.DotProduct(vDist1);
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = cBox2.m_vZ.DotProduct(vDist1);
		if (fR < -fR01)
			return false;
	}

	// Test for m_vX x cBox2.m_vX
	fR  = fDotZDist0*fDotYX - fDotYDist0*fDotZX;
	fR0 = cBox1.m_fExtY*fAbsZX + cBox1.m_fExtZ*fAbsYX;
	fR1 = cBox2.m_fExtY*fAbsXZ + cBox2.m_fExtZ*fAbsXY;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotZDist1*fDotYX - fDotYDist1*fDotZX;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotZDist1*fDotYX - fDotYDist1*fDotZX;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vX x cBox2.m_vY
	fR  = fDotZDist0*fDotYY - fDotYDist0*fDotZY;
	fR0 = cBox1.m_fExtY*fAbsZY + cBox1.m_fExtZ*fAbsYY;
	fR1 = cBox2.m_fExtX*fAbsXZ + cBox2.m_fExtZ*fAbsXX;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotZDist1*fDotYY - fDotYDist1*fDotZY;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotZDist1*fDotYY - fDotYDist1*fDotZY;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vX x cBox2.m_vZ
	fR  = fDotZDist0*fDotYZ - fDotYDist0*fDotZZ;
	fR0 = cBox1.m_fExtY*fAbsZZ + cBox1.m_fExtZ*fAbsYZ;
	fR1 = cBox2.m_fExtX*fAbsXY + cBox2.m_fExtY*fAbsXX;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotZDist1*fDotYZ - fDotYDist1*fDotZZ;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotZDist1*fDotYZ - fDotYDist1*fDotZZ;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vY x cBox2.m_vX
	fR  = fDotXDist0*fDotZX - fDotZDist0*fDotXX;
	fR0 = cBox1.m_fExtX*fAbsZX + cBox1.m_fExtZ*fAbsXX;
	fR1 = cBox2.m_fExtY*fAbsYZ + cBox2.m_fExtZ*fAbsYY;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotXDist1*fDotZX - fDotZDist1*fDotXX;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotXDist1*fDotZX - fDotZDist1*fDotXX;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vY x cBox2.m_vY
	fR  = fDotXDist0*fDotZY - fDotZDist0*fDotXY;
	fR0 = cBox1.m_fExtX*fAbsZY + cBox1.m_fExtZ*fAbsXY;
	fR1 = cBox2.m_fExtX*fAbsYZ + cBox2.m_fExtZ*fAbsYX;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotXDist1*fDotZY - fDotZDist1*fDotXY;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotXDist1*fDotZY - fDotZDist1*fDotXY;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vY x cBox2.m_vZ
	fR  = fDotXDist0*fDotZZ - fDotZDist0*fDotXZ;
	fR0 = cBox1.m_fExtX*fAbsZZ + cBox1.m_fExtZ*fAbsXZ;
	fR1 = cBox2.m_fExtX*fAbsYY + cBox2.m_fExtY*fAbsYX;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotXDist1*fDotZZ - fDotZDist1*fDotXZ;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotXDist1*fDotZZ - fDotZDist1*fDotXZ;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vZ x cBox2.m_vX
	fR  = fDotYDist0*fDotXX - fDotXDist0*fDotYX;
	fR0 = cBox1.m_fExtX*fAbsYX + cBox1.m_fExtY*fAbsXX;
	fR1 = cBox2.m_fExtY*fAbsZZ + cBox2.m_fExtZ*fAbsZY;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotYDist1*fDotXX - fDotXDist1*fDotYX;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotYDist1*fDotXX - fDotXDist1*fDotYX;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vZ x cBox2.m_vY
	fR  = fDotYDist0*fDotXY - fDotXDist0*fDotYY;
	fR0 = cBox1.m_fExtX*fAbsYY + cBox1.m_fExtY*fAbsXY;
	fR1 = cBox2.m_fExtX*fAbsZZ + cBox2.m_fExtZ*fAbsZX;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotYDist1*fDotXY - fDotXDist1*fDotYY;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotYDist1*fDotXY - fDotXDist1*fDotYY;
		if (fR < -fR01)
			return false;
	}

	// Test for m_vZ x cBox2.m_vZ
	fR  = fDotYDist0*fDotXZ - fDotXDist0*fDotYZ;
	fR0 = cBox1.m_fExtX*fAbsYZ + cBox1.m_fExtY*fAbsXZ;
	fR1 = cBox2.m_fExtX*fAbsZY + cBox2.m_fExtY*fAbsZX;
	fR01 = fR0 + fR1;
	if (fR > fR01) {
		fR = fDotYDist1*fDotXZ - fDotXDist1*fDotYZ;
		if (fR > fR01)
			return false;
	} else if (fR < -fR01) {
		fR = fDotYDist1*fDotXZ - fDotXDist1*fDotYZ;
		if (fR < -fR01)
			return false;
	}

	// In addition to these 15 axis, we have to test each box against the movement axis
	Vector3 vCrossMoveDist = vMove.CrossProduct(vDist0);

	// Test for vMove x m_vX
	float fDotMoveY = vMove.DotProduct(cBox1.m_vY);
	float fDotMoveZ = vMove.DotProduct(cBox1.m_vZ);
	fR  = Math::Abs(cBox1.m_vX.DotProduct(vCrossMoveDist));
	fR0 = cBox1.m_fExtY*Math::Abs(fDotMoveZ) + cBox1.m_fExtZ*Math::Abs(fDotMoveY);
	fR1 = cBox2.m_fExtX*Math::Abs(fDotYX*fDotMoveZ - fDotZX*fDotMoveY) +
		  cBox2.m_fExtY*Math::Abs(fDotYY*fDotMoveZ - fDotZY*fDotMoveY) +
		  cBox2.m_fExtZ*Math::Abs(fDotYZ*fDotMoveZ - fDotZZ*fDotMoveY);
	if (fR > fR0 + fR1)
		return false;

	// Test for vMove x m_vY
	float fDotMoveX = vMove.DotProduct(cBox1.m_vX);
	fR  = Math::Abs(cBox1.m_vY.DotProduct(vCrossMoveDist));
	fR0 = cBox1.m_fExtZ*Math::Abs(fDotMoveX) + cBox1.m_fExtX*Math::Abs(fDotMoveZ);
	fR1 = cBox2.m_fExtX*Math::Abs(fDotZX*fDotMoveX - fDotXX*fDotMoveZ) +
		  cBox2.m_fExtY*Math::Abs(fDotZY*fDotMoveX - fDotXY*fDotMoveZ) +
		  cBox2.m_fExtZ*Math::Abs(fDotZZ*fDotMoveX - fDotXZ*fDotMoveZ);
	if (fR > fR0 + fR1)
		return false;

	// Test for vMove x m_vZ
	fR  = Math::Abs(cBox1.m_vZ.DotProduct(vCrossMoveDist));
	fR0 = cBox1.m_fExtX*Math::Abs(fDotMoveY) + cBox1.m_fExtY*Math::Abs(fDotMoveX);
	fR1 = cBox2.m_fExtX*Math::Abs(fDotXX*fDotMoveY - fDotYX*fDotMoveX) +
		  cBox2.m_fExtY*Math::Abs(fDotXY*fDotMoveY - fDotYY*fDotMoveX) +
		  cBox2.m_fExtZ*Math::Abs(fDotXZ*fDotMoveY - fDotYZ*fDotMoveX);
	if (fR > fR0 + fR1)
		return false;

	// Test for vMove x cBox2.m_vX
	float fDotMoveY2 = vMove.DotProduct(cBox2.m_vY);
	float fDotMoveZ2 = vMove.DotProduct(cBox2.m_vZ);
	fR  = Math::Abs(cBox2.m_vX.DotProduct(vCrossMoveDist));
	fR0 = cBox1.m_fExtX*Math::Abs(fDotXY*fDotMoveZ2 - fDotXZ*fDotMoveY2) +
		  cBox1.m_fExtY*Math::Abs(fDotYY*fDotMoveZ2 - fDotYZ*fDotMoveY2) +
		  cBox1.m_fExtZ*Math::Abs(fDotZY*fDotMoveZ2 - fDotZZ*fDotMoveY2);
	fR1 = cBox2.m_fExtY*Math::Abs(fDotMoveZ2) + cBox2.m_fExtZ*Math::Abs(fDotMoveY2);
	if (fR > fR0 + fR1)
		return false;

	// Test for vMove x cBox2.m_vY
	float fDotMoveX2 = vMove.DotProduct(cBox2.m_vX);
	fR  = Math::Abs(cBox2.m_vY.DotProduct(vCrossMoveDist));
	fR0 = cBox1.m_fExtX*Math::Abs(fDotXZ*fDotMoveX2 - fDotXX*fDotMoveZ2) +
		  cBox1.m_fExtY*Math::Abs(fDotYZ*fDotMoveX2 - fDotYX*fDotMoveZ2) +
		  cBox1.m_fExtZ*Math::Abs(fDotZZ*fDotMoveX2 - fDotZX*fDotMoveZ2);
	fR1 = cBox2.m_fExtZ*Math::Abs(fDotMoveX2) + cBox2.m_fExtX*Math::Abs(fDotMoveZ2);
	if (fR > fR0 + fR1)
		return false;

	// Test for vMove x cBox2.m_vZ
	fR  = Math::Abs(cBox2.m_vZ.DotProduct(vCrossMoveDist));
	fR0 = cBox1.m_fExtX*Math::Abs(fDotXX*fDotMoveY2 - fDotXY*fDotMoveX2) +
		  cBox1.m_fExtY*Math::Abs(fDotYX*fDotMoveY2 - fDotYY*fDotMoveX2) +
		  cBox1.m_fExtZ*Math::Abs(fDotZX*fDotMoveY2 - fDotZY*fDotMoveX2);
	fR1 = cBox2.m_fExtX*Math::Abs(fDotMoveY2) + cBox2.m_fExtY*Math::Abs(fDotMoveX2);
	if (fR > fR0 + fR1)
		return false;

	// None of the axis separated the boxes, so there's a collision
	return true;
}

/**
*  @brief
*    Check whether the box is in the plane set or not
*/
bool Intersect::BoxPlaneSet(const BoundingBox &cBox, const PlaneSet &cPlaneSet)
{
	// Check frustum itself
	return PlaneSetAABox(cPlaneSet, cBox.m_vPos+cBox.m_mFinalRot*cBox.m_vCorner1*cBox.m_vScale,
						 cBox.m_vPos+cBox.m_mFinalRot*cBox.m_vCorner2*cBox.m_vScale);
}


//[-------------------------------------------------------]
//[ Plane intersection                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Checks if there's a plane/ray intersection
*/
bool Intersect::IsPlaneRay(const Plane &cPlane, const Vector3 &vRayDirection)
{
	// Ray parallel to plane?
	return ((cPlane.fN[0]*vRayDirection.x + cPlane.fN[1]*vRayDirection.y + cPlane.fN[2]*vRayDirection.z) != 0.0f);
}

/**
*  @brief
*    Returns the plane/ray intersection point
*/
bool Intersect::PlaneRay(const Plane &cPlane, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 &vIntersectionPointPos)
{
	// Line parallel to plane?
	const float *fN = &cPlane.fN[0];
	float fA = fN[0]*vRayDirection.x + fN[1]*vRayDirection.y + fN[2]*vRayDirection.z;
	if (fA) {
		// Get a point on the plane
		const Vector3 vPointOnPlane = cPlane.GetPointOnPlane();

		// Not optimized
//		vIntersectionPointPos = vRayOrigin-vRayDirection*((Vector3(cPlane.fN).DotProduct(vRayOrigin)-vPointOnPlane.DotProduct(Vector3(cPlane.fN)))/fA);

		// Optimized (no temporal vector objects on the runtime stack)
		fA = ((fN[0]*vRayOrigin.x + fN[1]*vRayOrigin.y + fN[2]*vRayOrigin.z) - (vPointOnPlane.x*fN[0] + vPointOnPlane.y*fN[1] + vPointOnPlane.z*fN[2]))/fA;
		vIntersectionPointPos.x = vRayOrigin.x - vRayDirection.x*fA;
		vIntersectionPointPos.y = vRayOrigin.y - vRayDirection.y*fA;
		vIntersectionPointPos.z = vRayOrigin.z - vRayDirection.z*fA;

		// Intersection
		return true;
	}

	// Error line parallel to plane
	return false;
}

bool Intersect::PlaneRayPositive(const Plane &cPlane, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 &vIntersectionPointPos)
{
	// Line parallel to plane?
	const float *fN = &cPlane.fN[0];
	float fA = fN[0]*vRayDirection.x + fN[1]*vRayDirection.y + fN[2]*vRayDirection.z;
	if (fA) {
		// Get a point on the plane
		const Vector3 vPointOnPlane = cPlane.GetPointOnPlane();

		// Not optimized
//		vIntersectionPointPos = vRayOrigin-vRayDirection*((Vector3(cPlane.fN).DotProduct(vRayOrigin)-vPointOnPlane.DotProduct(Vector3(cPlane.fN)))/fA);

		// Optimized (no temporal vector objects on the runtime stack)
		fA = ((fN[0]*vRayOrigin.x + fN[1]*vRayOrigin.y + fN[2]*vRayOrigin.z) - (vPointOnPlane.x*fN[0] + vPointOnPlane.y*fN[1] + vPointOnPlane.z*fN[2]))/fA;
		if (fA > 0.0f) {
			vIntersectionPointPos.x = vRayOrigin.x - vRayDirection.x*fA;
			vIntersectionPointPos.y = vRayOrigin.y - vRayDirection.y*fA;
			vIntersectionPointPos.z = vRayOrigin.z - vRayDirection.z*fA;

			// Intersection
			return true;
		}
	}

	// Error line parallel to plane or distance not positive
	return false;
}

bool Intersect::PlaneRayNegative(const Plane &cPlane, const Vector3 &vRayOrigin, const Vector3 &vRayDirection, Vector3 &vIntersectionPointPos)
{
	// Line parallel to plane?
	const float *fN = &cPlane.fN[0];
	float fA = fN[0]*vRayDirection.x + fN[1]*vRayDirection.y + fN[2]*vRayDirection.z;
	if (fA) {
		// Get a point on the plane
		const Vector3 vPointOnPlane = cPlane.GetPointOnPlane();

		// Not optimized
//		vIntersectionPointPos = vRayOrigin-vRayDirection*((Vector3(cPlane.fN).DotProduct(vRayOrigin)-vPointOnPlane.DotProduct(Vector3(cPlane.fN)))/fA);

		// Optimized (no temporal vector objects on the runtime stack)
		fA = ((fN[0]*vRayOrigin.x + fN[1]*vRayOrigin.y + fN[2]*vRayOrigin.z) - (vPointOnPlane.x*fN[0] + vPointOnPlane.y*fN[1] + vPointOnPlane.z*fN[2]))/fA;
		if (fA < 0.0f) {
			vIntersectionPointPos.x = vRayOrigin.x - vRayDirection.x*fA;
			vIntersectionPointPos.y = vRayOrigin.y - vRayDirection.y*fA;
			vIntersectionPointPos.z = vRayOrigin.z - vRayDirection.z*fA;

			// Intersection
			return true;
		}
	}

	// Error line parallel to plane or distance not negative
	return false;
}

/**
*  @brief
*    Checks if there's a plane/line intersection
*/
float Intersect::PlaneLine(const Plane &cPlane, const Vector3 &vStartPos,
						   const Vector3 &vEndPos, Vector3 *pvPos)
{
	const float *fN = cPlane.fN;
	float		 fD = cPlane.fD;
	Vector3 vM = vEndPos - vStartPos;
	float f0 = fN[0]*vStartPos.x + fN[1]*vStartPos.y + fN[2]*vStartPos.z + fD;
	float f1 = fN[0]*-vM.x       + fN[1]*-vM.y       + fN[2]*-vM.z;
	if ((f1 < -0.0001f) || (f1 > 0.0001f)) {
		float t = f0/f1;
		if (pvPos)
			*pvPos = vStartPos+vM*t;
		return t;
	} else {
		return -1.0f;
	}
}

/**
*  @brief
*    Checks if there's a plane/plane intersection and calculates the intersection ray
*/
bool Intersect::PlanePlane(const Plane &cPlane, const Plane &cPlane2, Ray &cRay)
{
	const float *fN = cPlane.fN;
	float		 fD = cPlane.fD;
	const float *fPN = cPlane2.fN;								// Get plane 2 normal reference
	float fN00 = fN[0]*fN[0] + fN[1]*fN[1] + fN[2]*fN[2];		// Squared length of fN
	float fN01 = fN[0]*fPN[0] + fN[1]*fPN[1] + fN[2]*fPN[2];	// Dot product fN*fPN
	float fN11 = fPN[0]*fPN[0] + fPN[1]*fPN[1] + fPN[2]*fPN[2];	// Squared length of fPN
	float fDet = fN00*fN11 - fN01*fN01;

	if (!Math::Abs(fDet))
		return false;

	float fInvDet = 1.0f/fDet;
	float fC0	  = (fN11*fD - fN01*cPlane2.fD)*fInvDet;
	float fC1	  = (fN00*cPlane2.fD - fN01*fD)*fInvDet;

	// Ray direction = fNxfPN
	cRay.SetDir(fN[1]*fPN[2] - fN[2]*fPN[1],
				fN[2]*fPN[0] - fN[0]*fPN[2],
				fN[0]*fPN[1] - fN[1]*fPN[0]);
	cRay.SetPos(fN[0]*fC0 + fPN[0]*fC1,
				fN[1]*fC0 + fPN[1]*fC1,
				fN[2]*fC0 + fPN[2]*fC1);

	return true;
}

/**
*  @brief
*    Checks if there's a plane/plane intersection
*/
bool Intersect::PlanePlane(const Plane &cPlane, const Plane &cPlane2)
{
	const float *fN = cPlane.fN;
	return (fN[0]*cPlane2.fN[0] + fN[1]*cPlane2.fN[1] + fN[2]*cPlane2.fN[2] > Math::Epsilon);
}

/**
*  @brief
*    Checks if there's an intersection between the three planes and returns the intersection point
*/
bool Intersect::PlanePlanePlane(const Plane &cP1, const Plane &cP2, const Plane &cP3, Vector3 &vRes)
{
	const float *fN = cP1.fN;
	Vector3 vN1 = fN;
	Vector3 vN2 = cP2.fN;
	Vector3 vN3 = cP3.fN;

	float fDenominator = vN1.DotProduct((vN2).CrossProduct(vN3)); // Scalar triple product of normals
	if (!fDenominator)	// If zero
		return false;	// No intersection

	float fD = cP1.fD;
	Vector3 vTemp1, vTemp2, vTemp3;
	vTemp1 = (vN2.CrossProduct(vN3))*fD;
	vTemp2 = (vN3.CrossProduct(vN1))*cP2.fD;
	vTemp3 = (vN1.CrossProduct(vN2))*cP3.fD;

	vRes = (vTemp1+vTemp2+vTemp3)/(-fDenominator);

	return true;
}


//[-------------------------------------------------------]
//[ Plane set                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Test whether a point is in the plane set
*/
bool Intersect::PlaneSetPoint(const PlaneSet &cPlaneSet, const Vector3 &vPoint)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	// Go through all the sides of the plane set
	for (uint32 i=0; i<lstPlane.GetNumOfElements(); i++) {
		// Calculate the plane equation and check if the point is behind a side of the plane set
		if (lstPlane[i].GetDistance(vPoint) <= 0)
			return false; // Not in plane set!
	}

	// The point is inside of the plane set (In front of ALL the sides of the plane set)
	return true;
}

/**
*  @brief
*    Test whether a point is in the plane set
*/
bool Intersect::PlaneSetPoint(const PlaneSet &cPlaneSet, const Vector4 &vPoint)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	// Go through all the sides of the plane set
	for (uint32 i=0; i<lstPlane.GetNumOfElements(); i++) {
		// Calculate the plane equation and check if the point is behind a side of the plane set
		if (lstPlane[i].GetDistance(vPoint) <= 0)
			return false; // Not in plane set!
	}

	// The point is inside of the plane set (In front of ALL the sides of the plane set)
	return true;
}

/**
*  @brief
*    Test whether the given points are in the plane set
*/
bool Intersect::PlaneSetPoints(const PlaneSet &cPlaneSet, const Array<Vector3> &lstPoints)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	// See if there is one plane for which all
	// of the vertices are on the wrong side
	for (uint32 nPlane=0; nPlane<lstPlane.GetNumOfElements(); nPlane++) {
		const Plane &cPlane = lstPlane[nPlane];

		// Assume this plane culls all points. See if there is a point
		// not culled by the plane.
		uint32 nInside = 0;
		for (uint32 i=0; i<lstPoints.GetNumOfElements(); i++) {
			// Calculate the plane equation and check if the point is behind a side of the plane set
			if (cPlane.GetDistance(lstPoints[i]) > 0)
				nInside++;
		}

		// Are all points outside the plane set?
		if (!nInside)
			return false;
	}

	// All points are inside the plane set
	return true;
}

/**
*  @brief
*    Test whether the given points are in the plane set
*/
bool Intersect::PlaneSetPoints(const PlaneSet &cPlaneSet, const Array<Vector4> &lstPoints)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	// See if there is one plane for which all
	// of the vertices are on the wrong side
	for (uint32 nPlane=0; nPlane<lstPlane.GetNumOfElements(); nPlane++) {
		const Plane &cPlane = lstPlane[nPlane];

		// Assume this plane culls all points. See if there is a point
		// not culled by the plane.
		uint32 nInside = 0;
		for (uint32 i=0; i<lstPoints.GetNumOfElements(); i++) {
			// Calculate the plane equation and check if the point is behind a side of the plane set
			if (cPlane.GetDistance(lstPoints[i]) > 0)
				nInside++;
		}

		// Are all points outside the plane set?
		if (!nInside)
			return false;
	}

	// All points are inside the plane set
	return true;
}

/**
*  @brief
*    Test whether a sphere is in the plane set
*/
bool Intersect::PlaneSetSphere(const PlaneSet &cPlaneSet, const Vector3 &vSphereOrigin, float fSphereRadius)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	// Go through all the sides of the plane set
	for (uint32 i=0; i<lstPlane.GetNumOfElements(); i++) {
		// Calculate the plane equation and check if the sphere is behind a side of the plane set
		if (lstPlane[i].GetDistance(vSphereOrigin) <= -fSphereRadius)
			return false; // Not in plane set!
	}

	// The sphere is inside of the plane set (In front of ALL the sides of the plane set)
	return true;
}

/**
*  @brief
*    Test whether a triangle is in the plane set
*/
bool Intersect::PlaneSetTriangle(const PlaneSet &cPlaneSet, const Vector3 &vV1,
								 const Vector3 &vV2, const Vector3 &vV3)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	// Go through all the sides of the plane set
	for (uint32 i=0; i<lstPlane.GetNumOfElements(); i++) {
		const Plane &cPlane = lstPlane[i];
		const float &fN     = cPlane.fN[0];
		if ((&fN)[0]*vV1.x + (&fN)[1]*vV1.y + (&fN)[2]*vV1.z + cPlane.fD <= 0) {
			if ((&fN)[0]*vV2.x + (&fN)[1]*vV2.y + (&fN)[2]*vV2.z + cPlane.fD <= 0) {
				if ((&fN)[0]*vV3.x + (&fN)[1]*vV3.y + (&fN)[2]*vV3.z + cPlane.fD <= 0)
					return false; // Not in plane set!
			}
		}
	}

	// The triangle is inside of the plane set (In front of ALL the sides of the plane set)
	return true;
}

/**
*  @brief
*    Tests whether a box is within the plane set or not
*/
bool Intersect::PlaneSetAABox(const PlaneSet &cPlaneSet, const Vector3 &vMin, const Vector3 &vMax, uint32 *pnOutClipMask)
{
	// Get planes list
	const Array<Plane> &lstPlane = cPlaneSet.GetList();

	// Are there any planes?
	if (!lstPlane.GetNumOfElements())
		return false;

	Vector3  m = (vMin + vMax)*0.5f;	// Center of AABB
	Vector3  d = vMax - m;				// Half-diagonal
	uint32 mk = 1;
	if (pnOutClipMask) {
		*pnOutClipMask = 0;	// Init outclip mask
		// Go through all the sides of the plane set
		for (uint32 i=0; i<lstPlane.GetNumOfElements(); i++) {
			const Plane &cPlane = lstPlane[i];
			const float &fN     = cPlane.fN[0];
			float NP = d.x*Math::Abs((&fN)[Vector3::X])+d.y*Math::Abs((&fN)[Vector3::Y])+d.z*Math::Abs((&fN)[Vector3::Z]);
			float MP = m.x*(&fN)[Vector3::X]+m.y*(&fN)[Vector3::Y]+m.z*(&fN)[Vector3::Z]+cPlane.fD;
			if ((MP+NP) < 0.0f)
				return false;	// Behind clip plane
			if ((MP-NP) < 0.0f)
				*pnOutClipMask |= mk;
			mk += mk;	// mk = (1<<iter)
		}
	} else {
		// Go through all the sides of the plane set
		for (uint32 i=0; i<lstPlane.GetNumOfElements(); i++) {
			const Plane &cPlane = lstPlane[i];
			const float &fN     = cPlane.fN[0];
			float NP = d.x*Math::Abs((&fN)[Vector3::X])+d.y*Math::Abs((&fN)[Vector3::Y])+d.z*Math::Abs((&fN)[Vector3::Z]);
			float MP = m.x*(&fN)[Vector3::X]+m.y*(&fN)[Vector3::Y]+m.z*(&fN)[Vector3::Z]+cPlane.fD;
			if ((MP+NP) < 0.0f)
				return false;	// Behind clip plane
			mk += mk;	// mk = (1<<iter)
		}
	}

	// AABB intersects the plane set
	return true;
}


//[-------------------------------------------------------]
//[ Triangle                                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Checks if there's a triangle/ray intersection
*/
bool Intersect::TriangleRay(const Vector3 &vV1, const Vector3 &vV2,
							const Vector3 &vV3, const Vector3 &vN,
							const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
							Vector3 &vIntersectionPointPos)
{
	// Calculate plane intersection point
	float fA = vN.DotProduct(vRayDirection);
	if (fA) {
		// Not optimized
//		vIntersectionPointPos = vRayOrigin-vRayDirection*((vN.DotProduct(vRayOrigin)-vV1.DotProduct(vN))/fA);

		// Optimized (no temporal vector objects on the runtime stack)
		fA = (vN.DotProduct(vRayOrigin) - vV1.DotProduct(vN))/fA;
		vIntersectionPointPos.x = vRayOrigin.x - vRayDirection.x*fA;
		vIntersectionPointPos.y = vRayOrigin.y - vRayDirection.y*fA;
		vIntersectionPointPos.z = vRayOrigin.z - vRayDirection.z*fA;

		// Check whether the intersection point is in the triangle
		return vIntersectionPointPos.IsPointInTriangle(vV1, vV2, vV3);
	}

	// Error line parallel to plane
	return false;
}

bool Intersect::TriangleRayPositive(const Vector3 &vV1, const Vector3 &vV2,
									const Vector3 &vV3, const Vector3 &vN,
									const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
									Vector3 &vIntersectionPointPos)
{
	// Calculate plane intersection point
	float fA = vN.DotProduct(vRayDirection);
	if (fA) {
		// Not optimized
//		vIntersectionPointPos = vRayOrigin-vRayDirection*((vN.DotProduct(vRayOrigin)-vV1.DotProduct(vN))/fA);

		// Optimized (no temporal vector objects on the runtime stack)
		fA = (vN.DotProduct(vRayOrigin) - vV1.DotProduct(vN))/fA;
		if (fA > 0.0f) {
			vIntersectionPointPos.x = vRayOrigin.x - vRayDirection.x*fA;
			vIntersectionPointPos.y = vRayOrigin.y - vRayDirection.y*fA;
			vIntersectionPointPos.z = vRayOrigin.z - vRayDirection.z*fA;

			// Check whether the intersection point is in the triangle
			return vIntersectionPointPos.IsPointInTriangle(vV1, vV2, vV3);
		}
	}

	// Error line parallel to plane or distance not positive
	return false;
}

bool Intersect::TriangleRayNegative(const Vector3 &vV1, const Vector3 &vV2,
									const Vector3 &vV3, const Vector3 &vN,
									const Vector3 &vRayOrigin, const Vector3 &vRayDirection,
									Vector3 &vIntersectionPointPos)
{
	// Calculate plane intersection point
	float fA = vN.DotProduct(vRayDirection);
	if (fA) {
		// Not optimized
//		vIntersectionPointPos = vRayOrigin-vRayDirection*((vN.DotProduct(vRayOrigin)-vV1.DotProduct(vN))/fA);

		// Optimized (no temporal vector objects on the runtime stack)
		fA = (vN.DotProduct(vRayOrigin) - vV1.DotProduct(vN))/fA;
		if (fA < 0.0f) {
			vIntersectionPointPos.x = vRayOrigin.x - vRayDirection.x*fA;
			vIntersectionPointPos.y = vRayOrigin.y - vRayDirection.y*fA;
			vIntersectionPointPos.z = vRayOrigin.z - vRayDirection.z*fA;

			// Check whether the intersection point is in the triangle
			return vIntersectionPointPos.IsPointInTriangle(vV1, vV2, vV3);
		}
	}

	// Error line parallel to plane or distance not negative
	return false;
}

/**
*  @brief
*    Checks if there's a triangle/ray intersection
*/
bool Intersect::TriangleRay(const Vector3 &vV1, const Vector3 &vV2,
							const Vector3 &vV3, const Vector3 &vN,
							const Ray &cRay, Vector3 &vIntersectionPointPos)
{
	return TriangleRay(vV1, vV2, vV3, vN, cRay.GetPos(), cRay.GetDir(), vIntersectionPointPos);
}

bool Intersect::TriangleRayPositive(const Vector3 &vV1, const Vector3 &vV2,
									const Vector3 &vV3, const Vector3 &vN,
									const Ray &cRay, Vector3 &vIntersectionPointPos)
{
	return TriangleRayPositive(vV1, vV2, vV3, vN, cRay.GetPos(), cRay.GetDir(), vIntersectionPointPos);
}

bool Intersect::TriangleRayNegative(const Vector3 &vV1, const Vector3 &vV2,
									const Vector3 &vV3, const Vector3 &vN,
									const Ray &cRay, Vector3 &vIntersectionPointPos)
{
	return TriangleRayNegative(vV1, vV2, vV3, vN, cRay.GetPos(), cRay.GetDir(), vIntersectionPointPos);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
