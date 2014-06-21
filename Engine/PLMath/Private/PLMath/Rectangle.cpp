/*********************************************************\
 *  File: Rectangle.cpp                                  *
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
#include "PLMath/Rectangle.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Calculates the screen-space bounding rectangle of the given array of points
*/
bool Rectangle::ScreenRectangle(const Array<Vector3> &lstPoints, bool bZCull, const Matrix4x4 &mMVP,
								uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight)
{
	// Convert it to window space
	const Vector3 *pPoint = &lstPoints[0];
	uint32 nZNear = 0, nZFar = 0;
	Vector3 vPoint;
	for (uint32 i=0; i<lstPoints.GetNumOfElements(); i++, pPoint++) {
		// Multiply with concatenated modelview/projection matrix
		vPoint = *pPoint;
		vPoint *= mMVP;

		// Convert to window coordinates
		// Map x, y and z to range 0-1
		vPoint.x =  vPoint.x*0.5f + 0.5f;
		vPoint.y = -vPoint.y*0.5f + 0.5f;
		vPoint.z =  vPoint.z*0.5f + 0.5f;
		// Map x,y to viewport
		vPoint.x = vPoint.x*nWidth  + nX;
		vPoint.y = vPoint.y*nHeight + nY;

		// Perform z cull?
		if (bZCull) {
			if (vPoint.z < -Math::Epsilon)
				nZFar++;
			else if (vPoint.z > (1+Math::Epsilon))
				nZNear++;
		}

		// Clamp x
		vPoint.x = Math::Max(static_cast<float>(nX),        vPoint.x);
		vPoint.x = Math::Min(static_cast<float>(nX+nWidth), vPoint.x);

		// Clamp y
		vPoint.y = Math::Max(static_cast<float>(nY),         vPoint.y);
		vPoint.y = Math::Min(static_cast<float>(nY+nHeight), vPoint.y);

		// Update min/max
		if (i) {
			if (vMin.x > vPoint.x)
				vMin.x = vPoint.x;
			if (vMin.y > vPoint.y)
				vMin.y = vPoint.y;
			if (vMax.x < vPoint.x)
				vMax.x = vPoint.x;
			if (vMax.y < vPoint.y)
				vMax.y = vPoint.y;
		} else {
			vMin.x = vMax.x = vPoint.x;
			vMin.y = vMax.y = vPoint.y;
		}
	}

	// Return z culling result
	if (nZFar == lstPoints.GetNumOfElements() || nZNear == lstPoints.GetNumOfElements())
		return false; // Z culled
	if (nZNear > 0 && nZNear < lstPoints.GetNumOfElements()) {
		vMin.x = static_cast<float>(nX);
		vMin.y = static_cast<float>(nY);
		vMax.x = static_cast<float>(nX + nWidth);
		vMax.y = static_cast<float>(nY + nHeight);
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates the screen-space bounding rectangle of the given array of points
*/
bool Rectangle::ScreenRectangle(const Array<Vector4> &lstPoints, bool bZCull, const Matrix4x4 &mMVP,
								uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight)
{
	// Convert it to window space
	Array<Vector4> lstP(lstPoints);
	const Vector4 *pPoint = &lstP[0];
	uint32 nZNear = 0, nZFar = 0;
	Vector4 vPoint;
	for (uint32 i=0; i<lstP.GetNumOfElements(); i++, pPoint++) {
		// Multiply with concatenated modelview/projection matrix
		vPoint = *pPoint;
		vPoint *= mMVP;
		vPoint.x /= vPoint.w;
		vPoint.y /= vPoint.w;
		vPoint.z /= vPoint.w;
		vPoint.w = 1.0f;

		// Convert to window coordinates
		// Map x, y and z to range 0-1
		vPoint.x =  vPoint.x*0.5f + 0.5f;
		vPoint.y = -vPoint.y*0.5f + 0.5f;
		vPoint.z =  vPoint.z*0.5f + 0.5f;
		// Map x,y to viewport
		vPoint.x = vPoint.x*nWidth  + nX;
		vPoint.y = vPoint.y*nHeight + nY;

		// Perform z cull?
		if (bZCull) {
			if (vPoint.z < -Math::Epsilon)
				nZFar++;
			else if (vPoint.z > (1+Math::Epsilon))
				nZNear++;
		}

		// Clamp x
		vPoint.x = Math::Max(static_cast<float>(nX),        vPoint.x);
		vPoint.x = Math::Min(static_cast<float>(nX+nWidth), vPoint.x);

		// Clamp y
		vPoint.y = Math::Max(static_cast<float>(nY),         vPoint.y);
		vPoint.y = Math::Min(static_cast<float>(nY+nHeight), vPoint.y);

		// Update min/max
		if (i) {
			if (vMin.x > vPoint.x)
				vMin.x = vPoint.x;
			if (vMin.y > vPoint.y)
				vMin.y = vPoint.y;
			if (vMax.x < vPoint.x)
				vMax.x = vPoint.x;
			if (vMax.y < vPoint.y)
				vMax.y = vPoint.y;
		} else {
			vMin.x = vMax.x = vPoint.x;
			vMin.y = vMax.y = vPoint.y;
		}
	}

	// Return z culling result
	if (nZFar == lstP.GetNumOfElements() || nZNear == lstP.GetNumOfElements())
		return false; // Z culled
	if (nZNear > 0 && nZNear < lstP.GetNumOfElements()) {
		vMin.x = static_cast<float>(nX);
		vMin.y = static_cast<float>(nY);
		vMax.x = static_cast<float>(nX + nWidth);
		vMax.y = static_cast<float>(nY + nHeight);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
