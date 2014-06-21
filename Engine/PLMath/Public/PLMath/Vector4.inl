/*********************************************************\
 *  File: Vector4.inl                                    *
 *      4D vector inline implementation
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
#include "PLMath/Math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Special operators                                     ]
//[-------------------------------------------------------]
inline Vector4 operator *(float fS, const Vector4 &vV)
{
	return Vector4(vV.x*fS, vV.y*fS, vV.z*fS, vV.w*fS);
}


//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting the components to (0, 0, 0, 1)
*/
inline Vector4::Vector4() :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
}

inline Vector4::Vector4(float fX, float fY, float fZ, float fW) :
	x(fX),
	y(fY),
	z(fZ),
	w(fW)
{
}

inline Vector4::Vector4(const float fV[]) :
	x(fV[0]),
	y(fV[1]),
	z(fV[2]),
	w(fV[3])
{
}

inline Vector4::Vector4(const Vector4 &vV) :
	x(vV.x),
	y(vV.y),
	z(vV.z),
	w(vV.w)
{
}

inline Vector4::Vector4(const PLCore::String &sString)
{
	FromString(sString);
}


//[-------------------------------------------------------]
//[ Destructor                                            ]
//[-------------------------------------------------------]
inline Vector4::~Vector4()
{
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
inline Vector4 &Vector4::operator =(const Vector4 &vV)
{
	x = vV.x;
	y = vV.y;
	z = vV.z;
	w = vV.w;
	return *this;
}

inline Vector4 &Vector4::operator =(const float fV[])
{
	x = fV[0];
	y = fV[1];
	z = fV[2];
	w = fV[3];
	return *this;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
inline bool Vector4::operator ==(const Vector4 &vV) const
{
	return (x == vV.x && y == vV.y && z == vV.z && w == vV.w);
}

inline bool Vector4::operator !=(const Vector4 &vV) const
{
	return (x != vV.x || y != vV.y || z != vV.z || w != vV.w);
}

inline bool Vector4::operator <(const Vector4 &vV) const
{
	if (x != vV.x) {
		return (x < vV.x);
	} else {
		if (y != vV.y)
			return (y < vV.y);
		else
			return (z != vV.z) ? (z < vV.z) : (w < vV.w);
	}
}

inline bool Vector4::operator >(const Vector4 &vV) const
{
	if (x != vV.x) {
		return (x > vV.x);
	} else {
		if (y != vV.y)
			return (y > vV.y);
		else
			return (z != vV.z) ? (z > vV.z) : (w > vV.w);
	}
}

inline bool Vector4::operator <=(const Vector4 &vV) const
{
	if (x != vV.x) {
		return (x <= vV.x);
	} else {
		if (y != vV.y)
			return (y <= vV.y);
		else
			return (z != vV.z) ? (z <= vV.z) : (w <= vV.w);
	}
}

inline bool Vector4::operator >=(const Vector4 &vV) const
{
	if (x != vV.x) {
		return (x >= vV.x);
	} else {
		if (y != vV.y)
			return (y >= vV.y);
		else
			return (z != vV.z) ? (z >= vV.z) : (w >= vV.w);
	}
}


//[-------------------------------------------------------]
//[ Vector                                                ]
//[-------------------------------------------------------]
inline Vector4 Vector4::operator +(const Vector4 &vV) const
{
	return Vector4(x+vV.x, y+vV.y, z+vV.z, w+vV.w);
}

inline Vector4 Vector4::operator +(float fN) const
{
	return Vector4(x+fN, y+fN, z+fN, w+fN);
}

inline Vector4 &Vector4::operator +=(const Vector4 &vV)
{
	x += vV.x;
	y += vV.y;
	z += vV.z;
	w += vV.w;
	return *this;
}

inline Vector4 &Vector4::operator +=(float fN)
{
	x += fN;
	y += fN;
	z += fN;
	w += fN;
	return *this;
}

inline Vector4 Vector4::operator -() const
{
	return Vector4(-x, -y, -z, -w);
}

inline Vector4 Vector4::operator -(const Vector4 &vV) const
{
	return Vector4(x-vV.x, y-vV.y, z-vV.z, w-vV.w);
}

inline Vector4 Vector4::operator -(float fN) const
{
	return Vector4(x-fN, y-fN, z-fN, w-fN);
}

inline Vector4 &Vector4::operator -=(const Vector4 &vV)
{
	x -= vV.x;
	y -= vV.y;
	z -= vV.z;
	w -= vV.w;
	return *this;
}

inline Vector4 &Vector4::operator -=(float fN)
{
	x -= fN;
	y -= fN;
	z -= fN;
	w -= fN;
	return *this;
}

inline Vector4 Vector4::operator *(const Vector4 &vV) const
{
	return Vector4(x*vV.x, y*vV.y, z*vV.z, w*vV.w);
}

inline Vector4 Vector4::operator *(float fS) const
{
	return Vector4(x*fS, y*fS, z*fS, w*fS);
}

inline Vector4 &Vector4::operator *=(const Vector4 &vV)
{
	x *= vV.x;
	y *= vV.y;
	z *= vV.z;
	w *= vV.w;
	return *this;
}

inline Vector4 &Vector4::operator *=(float fS)
{
	x *= fS;
	y *= fS;
	z *= fS;
	w *= fS;
	return *this;
}

inline Vector4 Vector4::operator /(const Vector4 &vV) const
{
	return Vector4(x/vV.x, y/vV.y, z/vV.z, w/vV.w);
}

inline Vector4 Vector4::operator /(float fS) const
{
	return Vector4(x/fS, y/fS, z/fS, w/fS);
}

inline Vector4 &Vector4::operator /=(const Vector4 &vV)
{
	x /= vV.x;
	y /= vV.y;
	z /= vV.z;
	w /= vV.w;
	return *this;
}

inline Vector4 &Vector4::operator /=(float fS)
{
	x /= fS;
	y /= fS;
	z /= fS;
	w /= fS;
	return *this;
}


//[-------------------------------------------------------]
//[ Get and set                                           ]
//[-------------------------------------------------------]
inline Vector4::operator float *()
{
	return &x;
}

inline Vector4::operator const float *() const
{
	return &x;
}

inline float &Vector4::operator [](int nIndex)
{
	return fV[nIndex];
}

inline const float &Vector4::operator [](int nIndex) const
{
	return fV[nIndex];
}

inline void Vector4::GetXYZW(float &fX, float &fY, float &fZ, float &fW) const
{
	fX = x;
	fY = y;
	fZ = z;
	fW = w;
}

inline float Vector4::GetX() const
{
	return x;
}

inline float Vector4::GetY() const
{
	return y;
}

inline float Vector4::GetZ() const
{
	return z;
}

inline float Vector4::GetW() const
{
	return w;
}

inline void Vector4::SetXYZ(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

inline void Vector4::SetXYZ(const float fV[])
{
	x = fV[0];
	y = fV[1];
	z = fV[2];
}

inline void Vector4::SetXYZW(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}

inline void Vector4::SetXYZW(const float fV[])
{
	x = fV[0];
	y = fV[1];
	z = fV[2];
	w = fV[3];
}

inline void Vector4::SetX(float fX)
{
	x = fX;
}

inline void Vector4::SetY(float fY)
{
	y = fY;
}

inline void Vector4::SetZ(float fZ)
{
	z = fZ;
}

inline void Vector4::SetW(float fW)
{
	w = fW;
}

inline void Vector4::IncXYZW(float fX, float fY, float fZ, float fW)
{
	x += fX;
	y += fY;
	z += fZ;
	w += fW;
}

inline void Vector4::IncXYZW(const float fV[])
{
	x += fV[0];
	y += fV[1];
	z += fV[2];
	w += fV[3];
}

inline void Vector4::IncX(float fX)
{
	x += fX;
}

inline void Vector4::IncY(float fY)
{
	y += fY;
}

inline void Vector4::IncZ(float fZ)
{
	z += fZ;
}

inline void Vector4::IncW(float fW)
{
	w += fW;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether the vector is null or not
*/
inline bool Vector4::IsNull() const
{
	return (!x && !y && !z && !w);
}

/**
*  @brief
*    Returns whether the vector is packed (within range of 0-1) or not
*/
inline bool Vector4::IsPacked() const
{
	return (x >= 0.0f && x <= 1.0f && y >= 0.0f && y <= 1.0f &&
			z >= 0.0f && z <= 1.0f && w >= 0.0f && w <= 1.0f);
}

/**
*  @brief
*    Packs/clamps the vector into a range of 0-1
*/
inline void Vector4::PackTo01()
{
	Normalize();
	x = x*0.5f + 0.5f;
	y = y*0.5f + 0.5f;
	z = z*0.5f + 0.5f;
	w = w*0.5f + 0.5f;
}

/**
*  @brief
*    Returns a vector which is packed/clamped into the range of 0-1
*/
inline Vector4 Vector4::GetPackedTo01() const
{
	Vector4 vTemp(x, y, z, w);
	vTemp.Normalize();
	vTemp.x = vTemp.x*0.5f + 0.5f;
	vTemp.y = vTemp.y*0.5f + 0.5f;
	vTemp.z = vTemp.z*0.5f + 0.5f;
	vTemp.w = vTemp.w*0.5f + 0.5f;
	return vTemp;
}

/**
*  @brief
*    Unpacks the packed vector into a range of -1 to 1
*/
inline void Vector4::UnpackFrom01()
{
	x = x*2 - 1;
	y = y*2 - 1;
	z = z*2 - 1;
	w = w*2 - 1;
}

/**
*  @brief
*    Returns a unpacked vector of the range of -1 to 1
*/
inline Vector4 Vector4::GetUnpackedFrom01() const
{
	return Vector4(x*2 - 1, y*2 - 1, z*2 - 1, w*2 - 1);
}

/**
*  @brief
*    Returns the smallest component
*/
inline Vector4::Component Vector4::GetSmallestComponent() const
{
	if (x < y) {
		if (x < z)
			return (x < w) ? X : W;
		else
			return (z < w) ? Z : W;
	} else {
		if (y < z)
			return (y < w) ? Y : W;
		else
			return (z < w) ? Z : W;
	}
}

/**
*  @brief
*    Returns the value of the smallest component
*/
inline float Vector4::GetSmallestValue() const
{
	if (x < y) {
		if (x < z)
			return (x < w) ? x : w;
		else
			return (z < w) ? z : w;
	} else {
		if (y < z)
			return (y < w) ? y : w;
		else
			return (z < w) ? z : w;
	}
}

/**
*  @brief
*    Returns the greatest component
*/
inline Vector4::Component Vector4::GetGreatestComponent() const
{
	if (x > y) {
		if (x > z)
			return (x > w) ? X : W;
		else
			return (z > w) ? Z : W;
	} else {
		if (y > z)
			return (y > w) ? Y : W;
		else
			return (z > w) ? Z : W;
	}
}

/**
*  @brief
*    Returns the value of the greatest component
*/
inline float Vector4::GetGreatestValue() const
{
	if (x > y) {
		if (x > z)
			return (x > w) ? x : w;
		else
			return (z > w) ? z : w;
	} else {
		if (y > z)
			return (y > w) ? y : w;
		else
			return (z > w) ? z : w;
	}
}

/**
*  @brief
*    Inverts the vector
*/
inline void Vector4::Invert()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
}

/**
*  @brief
*    Returns the inverted vector
*/
inline Vector4 Vector4::GetInverted() const
{
	return Vector4(-x, -y, -z, -w);
}

/**
*  @brief
*    Returns the length of the vector (also called magnitude)
*/
inline float Vector4::GetLength() const
{
	return Math::Sqrt(x*x + y*y + z*z + w*w);
}

/**
*  @brief
*    Returns the squared length of the vector (also called norm)
*/
inline float Vector4::GetSquaredLength() const
{
	return x*x + y*y + z*z + w*w;
}

/**
*  @brief
*    Sets the vector to the given length
*/
inline void Vector4::SetLength(float fLength)
{
	// Avoid division through zero...
	float fU = x*x + y*y + z*z + w*w;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = fLength/fU;
			x *= fScale;
			y *= fScale;
			z *= fScale;
			w *= fScale;
		}
	}
}

/**
*  @brief
*    Normalizes the vector
*/
inline Vector4 &Vector4::Normalize()
{
	// Avoid division through zero...
	float fU = x*x + y*y + z*z + w*w;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = 1.0f/fU;
			x *= fScale;
			y *= fScale;
			z *= fScale;
			w *= fScale;
		}
	}

	return *this;
}

/**
*  @brief
*    Returns the normalized vector
*/
inline Vector4 Vector4::GetNormalized() const
{
	// Avoid division through zero...
	float fU = x*x + y*y + z*z + w*w;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = 1.0f/fU;
			return Vector4(x*fScale, y*fScale, z*fScale, w*fScale);
		}
	}

	// Error!
	return Vector4::Zero;
}

/**
*  @brief
*    Returns the distance to another vector
*/
inline float Vector4::GetDistance(const Vector4 &vV) const
{
	const float fDX = vV.x-x;
	const float fDY = vV.y-y;
	const float fDZ = vV.z-z;
	const float fDW = vV.w-w;
	return Math::Sqrt(fDX*fDX + fDY*fDY + fDZ*fDZ + fDW*fDW);
}

/**
*  @brief
*    Returns the squared distance to another vector
*/
inline float Vector4::GetSquaredDistance(const Vector4 &vV) const
{
	const float fDX = vV.x-x;
	const float fDY = vV.y-y;
	const float fDZ = vV.z-z;
	const float fDW = vV.w-w;
	return fDX*fDX + fDY*fDY + fDZ*fDZ + fDW*fDW;
}

/**
*  @brief
*    Returns the dot product of two vectors
*/
inline float Vector4::DotProduct(const Vector4 &vV) const
{
	return x*vV.x + y*vV.y + z*vV.z + w*vV.w;
}

/**
*  @brief
*    Project the vector onto another vector
*/
inline Vector4 Vector4::ProjectVector(const Vector4 &vA, const Vector4 &vB) const
{
	return vB*(vA.DotProduct(vB)/vB.DotProduct(vB));
}

/**
*  @brief
*    Calculates the angle between two vectors
*/
inline float Vector4::GetAngle(const Vector4 &vV) const
{
	return Math::ACos(DotProduct(vV)/(GetLength()*vV.GetLength()));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
