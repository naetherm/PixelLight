/*********************************************************\
 *  File: AABoundingBox.cpp                              *
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
#include "PLMath/AABoundingBox.h"


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
*    Copy operator
*/
AABoundingBox &AABoundingBox::operator =(const AABoundingBox &cSource)
{
	// Copy data
	vMin = cSource.vMin;
	vMax = cSource.vMax;

	// Return this
	return *this;
}

/**
*  @brief
*    Returns the 8 corner vertices
*/
void AABoundingBox::GetVertices(Vector3 vVertex[8]) const
{
	vVertex[0].SetXYZ(vMin.x, vMin.y, vMin.z);
	vVertex[1].SetXYZ(vMax.x, vMin.y, vMin.z);
	vVertex[2].SetXYZ(vMax.x, vMax.y, vMin.z);
	vVertex[3].SetXYZ(vMin.x, vMax.y, vMin.z);
	vVertex[4].SetXYZ(vMin.x, vMin.y, vMax.z);
	vVertex[5].SetXYZ(vMax.x, vMin.y, vMax.z);
	vVertex[6].SetXYZ(vMax.x, vMax.y, vMax.z);
	vVertex[7].SetXYZ(vMin.x, vMax.y, vMax.z);
}

/**
*  @brief
*    Returns one of the 8 corner vertices
*/
Vector3 AABoundingBox::GetVertex(uint32 nIndex) const
{
	switch (nIndex) {
		case 0:  return Vector3(vMin.x, vMin.y, vMin.z);
		case 1:  return Vector3(vMax.x, vMin.y, vMin.z);
		case 2:  return Vector3(vMax.x, vMax.y, vMin.z);
		case 3:  return Vector3(vMin.x, vMax.y, vMin.z);
		case 4:  return Vector3(vMin.x, vMin.y, vMax.z);
		case 5:  return Vector3(vMax.x, vMin.y, vMax.z);
		case 6:  return Vector3(vMax.x, vMax.y, vMax.z);
		case 7:  return Vector3(vMin.x, vMax.y, vMax.z);
		default: return Vector3::Zero; // Error!
	}
}

/**
*  @brief
*    Calculates the index of the nearest vertex in the AAB according to the normal vector of a clip plane
*/
uint32 AABoundingBox::GetNearestVertexIndex(const Vector3 &vClipPlaneNormal) const
{
	uint32 nIndex;

	// Get vertex index
	if (vClipPlaneNormal.x <= 0.0f)
		nIndex = (vClipPlaneNormal.y <= 0.0f) ? 0 : 3;
	else
		nIndex = (vClipPlaneNormal.y <= 0.0f) ? 1 : 2;
	return (vClipPlaneNormal.z <= 0.0f) ? nIndex : 4 + nIndex;
}

/**
*  @brief
*    Calculates the index of the furthest vertex in the AAB according to the normal vector of a clip plane
*/
uint32 AABoundingBox::GetFurthestVertexIndex(const Vector3 &vClipPlaneNormal) const
{
	uint32 nIndex;

	// Get vertex index
	if (vClipPlaneNormal.x > 0.0f)
		nIndex = (vClipPlaneNormal.y > 0.0f) ? 0 : 3;
	else
		nIndex = (vClipPlaneNormal.y > 0.0f) ? 1 : 2;
	return (vClipPlaneNormal.z > 0.0f) ? nIndex : 4 + nIndex;
}

/**
*  @brief
*    Ensures that the minimum is really the minimum and the maximum is really the maximum
*/
void AABoundingBox::ValidateMinMax()
{
	// x
	if (vMin.x > vMax.x) {
		// Swap
		const float fTemp = vMax.x;
		vMax.x = vMin.x;
		vMin.x = fTemp;
	}

	// y
	if (vMin.y > vMax.y) {
		// Swap
		const float fTemp = vMax.y;
		vMax.y = vMin.y;
		vMin.y = fTemp;
	}

	// z
	if (vMin.z > vMax.z) {
		// Swap
		const float fTemp = vMax.z;
		vMax.z = vMin.z;
		vMin.z = fTemp;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
