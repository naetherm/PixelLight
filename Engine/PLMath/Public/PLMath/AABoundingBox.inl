/*********************************************************\
 *  File: AABoundingBox.inl                              *
 *      Axis align bounding box inline implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all minimum and maximum components to 0
*/
inline AABoundingBox::AABoundingBox()
{
}

/**
*  @brief
*    Copy constructor
*/
inline AABoundingBox::AABoundingBox(const AABoundingBox &cSource) :
	vMin(cSource.vMin),
	vMax(cSource.vMax)
{
}

/**
*  @brief
*    Constructor
*/
inline AABoundingBox::AABoundingBox(const Vector3 &vMinMax) :
	vMin(vMinMax),
	vMax(vMinMax)
{
}

/**
*  @brief
*    Constructor
*/
inline AABoundingBox::AABoundingBox(const Vector3 &vMinT, const Vector3 &vMaxT) :
	vMin(vMinT),
	vMax(vMaxT)
{
}

/**
*  @brief
*    Constructor
*/
inline AABoundingBox::AABoundingBox(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ) :
	vMin(fMinX, fMinY, fMinZ),
	vMax(fMaxX, fMaxY, fMaxZ)
{
}

/**
*  @brief
*    Destructor
*/
inline AABoundingBox::~AABoundingBox()
{
}

/**
*  @brief
*    Returns the center of the box
*/
inline Vector3 AABoundingBox::GetCenter() const
{
	return Vector3((vMax.x+vMin.x)*0.5f, (vMax.y+vMin.y)*0.5f, (vMax.z+vMin.z)*0.5f);
}

/**
*  @brief
*    Returns the width
*/
inline float AABoundingBox::GetWidth() const
{
	return vMax.x-vMin.x;
}

/**
*  @brief
*    Returns the height
*/
inline float AABoundingBox::GetHeight() const
{
	return vMax.y-vMin.y;
}

/**
*  @brief
*    Returns the depth
*/
inline float AABoundingBox::GetDepth() const
{
	return vMax.z-vMin.z;
}

/**
*  @brief
*    Returns the longest axis
*/
inline AABoundingBox::Axis AABoundingBox::GetLongestAxis() const
{
	// Get differences
	const float fDX = vMax.x - vMin.x;
	const float fDY = vMax.y - vMin.y;
	const float fDZ = vMax.z - vMin.z;

	// Return the longest axis
	if (fDX > fDY && fDX > fDZ)
		return X;
	else if (fDY > fDZ)
		return Y;
	else
		return Z;
}

/**
*  @brief
*    Returns the length of the longest axis
*/
inline float AABoundingBox::GetLongestAxisLength() const
{
	// Get differences
	const float fDX = vMax.x - vMin.x;
	const float fDY = vMax.y - vMin.y;
	const float fDZ = vMax.z - vMin.z;

	// Return the longest axis
	if (fDX > fDY && fDX > fDZ)
		return fDX;
	else if (fDY > fDZ)
		return fDY;
	else
		return fDZ;
}

/**
*  @brief
*    Returns the radius of a sphere placed at the origin (0, 0, 0) enclosing this axis align bounding box
*/
inline float AABoundingBox::GetEnclosingRadius() const
{
	// Get the minimum/maximum squared length
	const float fMinSquaredLength = vMin.GetSquaredLength();
	const float fMaxSquaredLength = vMax.GetSquaredLength();

	// The greater one has to be used for the radius
	return (fMaxSquaredLength > fMinSquaredLength) ? Math::Sqrt(fMaxSquaredLength) : Math::Sqrt(fMinSquaredLength);
}

/**
*  @brief
*    Returns the radius of a sphere placed at the origin (0, 0, 0) inside this axis align bounding box
*/
inline float AABoundingBox::GetInsideRadius() const
{
	// Get differences
	const float fDX = vMax.x - vMin.x;
	const float fDY = vMax.y - vMin.y;
	const float fDZ = vMax.z - vMin.z;

	// Return the longest axis
	if (fDX > fDY && fDX > fDZ)
		return fDX*0.5f;
	else if (fDY > fDZ)
		return fDY*0.5f;
	else
		return fDZ*0.5f;
}

/**
*  @brief
*    Calculates the surface of the box
*/
inline float AABoundingBox::CalculateSurface() const
{
	// Get differences
	const float fDX = vMax.x - vMin.x;
	const float fDY = vMax.y - vMin.y;
	const float fDZ = vMax.z - vMin.z;

	// Calculate the surface
	return (fDX*fDY + fDX*fDZ + fDY*fDZ)*2;
}

/**
*  @brief
*    Calculates the volume of the box
*/
inline float AABoundingBox::CalculateVolume() const
{
	return (vMax.x - vMin.x)*
		   (vMax.y - vMin.y)*
		   (vMax.z - vMin.z);
}

/**
*  @brief
*    Clips this box with another one
*/
inline void AABoundingBox::ClipByAABox(const AABoundingBox &cEnclosed)
{
	// x
	if (vMin.x > cEnclosed.vMax.x) {
		vMin.x = vMax.x = cEnclosed.vMax.x;
	} else {
		if (vMax.x < cEnclosed.vMin.x) {
			vMin.x = vMax.x = cEnclosed.vMin.x;
		} else {
			if (vMin.x < cEnclosed.vMin.x)
				vMin.x = cEnclosed.vMin.x;
			if (vMax.x > cEnclosed.vMax.x)
				vMax.x = cEnclosed.vMax.x;
		}
	}

	// y
	if (vMin.y > cEnclosed.vMax.y) {
		vMin.y = vMax.y = cEnclosed.vMax.y;
	} else {
		if (vMax.y < cEnclosed.vMin.y) {
			vMin.y = vMax.y = cEnclosed.vMin.y;
		} else {
			if (vMin.y < cEnclosed.vMin.y)
				vMin.y = cEnclosed.vMin.y;
			if (vMax.y > cEnclosed.vMax.y)
				vMax.y = cEnclosed.vMax.y;
		}
	}

	// z
	if (vMin.z > cEnclosed.vMax.z) {
		vMin.z = vMax.z = cEnclosed.vMax.z;
	} else {
		if (vMax.z < cEnclosed.vMin.z) {
			vMin.z = vMax.z = cEnclosed.vMin.z;
		} else {
			if (vMin.z < cEnclosed.vMin.z)
				vMin.z = cEnclosed.vMin.z;
			if (vMax.z > cEnclosed.vMax.z)
				vMax.z = cEnclosed.vMax.z;
		}
	}
}

/**
*  @brief
*    Appends a vertex to the cubic hull
*/
inline void AABoundingBox::AppendToCubicHull(const Vector3 &vV)
{
	if (vV.x < vMin.x)
		vMin.x = vV.x;
	if (vV.y < vMin.y)
		vMin.y = vV.y;
	if (vV.z < vMin.z)
		vMin.z = vV.z;
	if (vV.x > vMax.x)
		vMax.x = vV.x;
	if (vV.y > vMax.y)
		vMax.y = vV.y;
	if (vV.z > vMax.z)
		vMax.z = vV.z;
}

/**
*  @brief
*    Combines two axis align boxes
*/
inline void AABoundingBox::CombineAABoxes(const AABoundingBox &cBox)
{
	AppendToCubicHull(cBox.vMin);
	AppendToCubicHull(cBox.vMax);
}

/**
*  @brief
*    Per component addition
*/
inline AABoundingBox AABoundingBox::operator +(const Vector3 &vV) const
{
	return AABoundingBox(vMin+vV, vMax+vV);
}

/**
*  @brief
*    Per component addition
*/
inline AABoundingBox AABoundingBox::operator +(float fS) const
{
	return AABoundingBox(vMin+fS, vMax+fS);
}

/**
*  @brief
*    Per component addition
*/
inline AABoundingBox &AABoundingBox::operator +=(const Vector3 &vV)
{
	vMin += vV;
	vMax += vV;
	return *this;
}

/**
*  @brief
*    Per component addition
*/
inline AABoundingBox &AABoundingBox::operator +=(float fS)
{
	vMin += fS;
	vMax += fS;
	return *this;
}

/**
*  @brief
*    Per component subtraction
*/
inline AABoundingBox AABoundingBox::operator -(const Vector3 &vV) const
{
	return AABoundingBox(vMin-vV, vMax-vV);
}

/**
*  @brief
*    Per component subtraction
*/
inline AABoundingBox AABoundingBox::operator -(float fS) const
{
	return AABoundingBox(vMin-fS, vMax-fS);
}

/**
*  @brief
*    Per component subtraction
*/
inline AABoundingBox &AABoundingBox::operator -=(const Vector3 &vV)
{
	vMin -= vV;
	vMax -= vV;
	return *this;
}

/**
*  @brief
*    Per component subtraction
*/
inline AABoundingBox &AABoundingBox::operator -=(float fS)
{
	vMin -= fS;
	vMax -= fS;
	return *this;
}

/**
*  @brief
*    Per component multiplication
*/
inline AABoundingBox AABoundingBox::operator *(const Vector3 &vV) const
{
	return AABoundingBox(vMin*vV, vMax*vV);
}

/**
*  @brief
*    Per component multiplication
*/
inline AABoundingBox AABoundingBox::operator *(float fS) const
{
	return AABoundingBox(vMin*fS, vMax*fS);
}

/**
*  @brief
*    Per component multiplication
*/
inline AABoundingBox &AABoundingBox::operator *=(const Vector3 &vV)
{
	vMin *= vV;
	vMax *= vV;
	return *this;
}

/**
*  @brief
*    Per component multiplication
*/
inline AABoundingBox &AABoundingBox::operator *=(float fS)
{
	vMin *= fS;
	vMax *= fS;
	return *this;
}

/**
*  @brief
*    Per component division
*/
inline AABoundingBox AABoundingBox::operator /(const Vector3 &vV) const
{
	return AABoundingBox(vMin/vV, vMax/vV);
}

/**
*  @brief
*    Per component division
*/
inline AABoundingBox AABoundingBox::operator /(float fS) const
{
	return AABoundingBox(vMin/fS, vMax/fS);
}

/**
*  @brief
*    Per component division
*/
inline AABoundingBox &AABoundingBox::operator /=(const Vector3 &vV)
{
	vMin /= vV;
	vMax /= vV;
	return *this;
}

/**
*  @brief
*    Per component division
*/
inline AABoundingBox &AABoundingBox::operator /=(float fS)
{
	vMin /= fS;
	vMax /= fS;
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
