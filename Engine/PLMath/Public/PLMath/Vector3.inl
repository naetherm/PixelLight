/*********************************************************\
 *  File: Vector3.inl                                    *
 *      3D vector inline implementation
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
inline Vector3 operator *(float fS, const Vector3 &vV)
{
	return Vector3(vV.x*fS, vV.y*fS, vV.z*fS);
}


//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all components to 0
*/
inline Vector3::Vector3() :
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

inline Vector3::Vector3(float fX, float fY, float fZ) :
	x(fX),
	y(fY),
	z(fZ)
{
}

inline Vector3::Vector3(const float fV[]) :
	x(fV[0]),
	y(fV[1]),
	z(fV[2])
{
}

inline Vector3::Vector3(const Vector3 &vV) :
	x(vV.x),
	y(vV.y),
	z(vV.z)
{
}

inline Vector3::Vector3(const PLCore::String &sString)
{
	FromString(sString);
}


//[-------------------------------------------------------]
//[ Destructor                                            ]
//[-------------------------------------------------------]
inline Vector3::~Vector3()
{
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
inline Vector3 &Vector3::operator =(const float fV[])
{
	x = fV[0];
	y = fV[1];
	z = fV[2];
	return *this;
}

inline Vector3 &Vector3::operator =(const Vector3 &vV)
{
	x = vV.x;
	y = vV.y;
	z = vV.z;
	return *this;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
inline bool Vector3::operator ==(const Vector3 &vV) const
{
	return (x == vV.x && y == vV.y && z == vV.z);
}

inline bool Vector3::operator !=(const Vector3 &vV) const
{
	return (x != vV.x || y != vV.y || z != vV.z);
}

inline bool Vector3::operator <(const Vector3 &vV) const
{
	if (x != vV.x)
		return (x < vV.x);
	else
		return (y != vV.y) ? (y < vV.y) : (z < vV.z);
}

inline bool Vector3::operator >(const Vector3 &vV) const
{
	if (x != vV.x)
		return (x > vV.x);
	else
		return (y != vV.y) ? (y > vV.y) : (z > vV.z);
}

inline bool Vector3::operator <=(const Vector3 &vV) const
{
	if (x != vV.x)
		return (x <= vV.x);
	else
		return (y != vV.y) ? (y <= vV.y) : (z <= vV.z);
}

inline bool Vector3::operator >=(const Vector3 &vV) const
{
	if (x != vV.x)
		return (x >= vV.x);
	else
		return (y != vV.y) ? (y >= vV.y) : (z >= vV.z);
}


//[-------------------------------------------------------]
//[ Vector                                                ]
//[-------------------------------------------------------]
inline Vector3 Vector3::operator +(const Vector3 &vV) const
{
	return Vector3(x+vV.x, y+vV.y, z+vV.z);
}

inline Vector3 Vector3::operator +(float fN) const
{
	return Vector3(x+fN, y+fN, z+fN);
}

inline Vector3 &Vector3::operator +=(const Vector3 &vV)
{
	x += vV.x;
	y += vV.y;
	z += vV.z;
	return *this;
}

inline Vector3 &Vector3::operator +=(float fN)
{
	x += fN;
	y += fN;
	z += fN;
	return *this;
}

inline Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator -(const Vector3 &vV) const
{
	return Vector3(x-vV.x, y-vV.y, z-vV.z);
}

inline Vector3 Vector3::operator -(float fN) const
{
	return Vector3(x-fN, y-fN, z-fN);
}

inline Vector3 &Vector3::operator -=(const Vector3 &vV)
{
	x -= vV.x;
	y -= vV.y;
	z -= vV.z;
	return *this;
}

inline Vector3 &Vector3::operator -=(float fN)
{
	x -= fN;
	y -= fN;
	z -= fN;
	return *this;
}

inline Vector3 Vector3::operator *(const Vector3 &vV) const
{
	return Vector3(x*vV.x, y*vV.y, z*vV.z);
}

inline Vector3 Vector3::operator *(float fS) const
{
	return Vector3(x*fS, y*fS, z*fS);
}

inline Vector3 &Vector3::operator *=(const Vector3 &vV)
{
	x *= vV.x;
	y *= vV.y;
	z *= vV.z;
	return *this;
}

inline Vector3 &Vector3::operator *=(float fS)
{
	x *= fS;
	y *= fS;
	z *= fS;
	return *this;
}

inline Vector3 Vector3::operator /(const Vector3 &vV) const
{
	return Vector3(x/vV.x, y/vV.y, z/vV.z);
}

inline Vector3 Vector3::operator /(float fS) const
{
	return Vector3(x/fS, y/fS, z/fS);
}

inline Vector3 &Vector3::operator /=(const Vector3 &vV)
{
	x /= vV.x;
	y /= vV.y;
	z /= vV.z;
	return *this;
}

inline Vector3 &Vector3::operator /=(float fS)
{
	x /= fS;
	y /= fS;
	z /= fS;
	return *this;
}


//[-------------------------------------------------------]
//[ Get and set                                           ]
//[-------------------------------------------------------]
inline Vector3::operator float *()
{
	return &x;
}

inline Vector3::operator const float *() const
{
	return &x;
}

inline float &Vector3::operator [](int nIndex)
{
	return fV[nIndex];
}

inline const float &Vector3::operator [](int nIndex) const
{
	return fV[nIndex];
}

inline void Vector3::GetXYZ(float &fX, float &fY, float &fZ) const
{
	fX = x;
	fY = y;
	fZ = z;
}

inline float Vector3::GetX() const
{
	return x;
}

inline float Vector3::GetY() const
{
	return y;
}

inline float Vector3::GetZ() const
{
	return z;
}

inline void Vector3::SetXYZ(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

inline void Vector3::SetXYZ(const float fV[])
{
	x = fV[0];
	y = fV[1];
	z = fV[2];
}

inline void Vector3::SetX(float fX)
{
	x = fX;
}

inline void Vector3::SetY(float fY)
{
	y = fY;
}

inline void Vector3::SetZ(float fZ)
{
	z = fZ;
}

inline void Vector3::IncXYZ(float fX, float fY, float fZ)
{
	x += fX;
	y += fY;
	z += fZ;
}

inline void Vector3::IncXYZ(const float fV[])
{
	x += fV[0];
	y += fV[1];
	z += fV[2];
}

inline void Vector3::IncX(float fX)
{
	x += fX;
}

inline void Vector3::IncY(float fY)
{
	y += fY;
}

inline void Vector3::IncZ(float fZ)
{
	z += fZ;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether the vector is null or not
*/
inline bool Vector3::IsNull() const
{
	return (!x && !y && !z);
}

/**
*  @brief
*    Returns whether the vector is packed (within range of 0-1) or not
*/
inline bool Vector3::IsPacked() const
{
	return (x >= 0.0f && x <= 1.0f && y >= 0.0f && y <= 1.0f && z >= 0.0f && z <= 1.0f);
}

/**
*  @brief
*    Packs/clamps the vector into a range of 0-1
*/
inline void Vector3::PackTo01()
{
	Normalize();
	x = x*0.5f + 0.5f;
	y = y*0.5f + 0.5f;
	z = z*0.5f + 0.5f;
}

/**
*  @brief
*    Returns a vector which is packed/clamped into the range of 0-1
*/
inline Vector3 Vector3::GetPackedTo01() const
{
	Vector3 vTemp(x, y, z);
	vTemp.Normalize();
	vTemp.x = vTemp.x*0.5f + 0.5f;
	vTemp.y = vTemp.y*0.5f + 0.5f;
	vTemp.z = vTemp.z*0.5f + 0.5f;
	return vTemp;
}

/**
*  @brief
*    Unpacks the packed vector into a range of -1 to 1
*/
inline void Vector3::UnpackFrom01()
{
	x = x*2 - 1;
	y = y*2 - 1;
	z = z*2 - 1;
}

/**
*  @brief
*    Returns a unpacked vector of the range of -1 to 1
*/
inline Vector3 Vector3::GetUnpackedFrom01() const
{
	return Vector3(x*2 - 1, y*2 - 1, z*2 - 1);
}

/**
*  @brief
*    Returns the smallest component
*/
inline Vector3::Component Vector3::GetSmallestComponent() const
{
	if (x < y) {
		if (x < z)
			return X;
		else
			return Z;
	} else {
		if (y < z)
			return Y;
		else
			return Z;
	}
}

/**
*  @brief
*    Returns the value of the smallest component
*/
inline float Vector3::GetSmallestValue() const
{
	if (x < y) {
		if (x < z)
			return x;
		else
			return z;
	} else {
		if (y < z)
			return y;
		else
			return z;
	}
}

/**
*  @brief
*    Returns the greatest component
*/
inline Vector3::Component Vector3::GetGreatestComponent() const
{
	if (x > y) {
		if (x > z)
			return X;
		else
			return Z;
	} else {
		if (y > z)
			return Y;
		else
			return Z;
	}
}

/**
*  @brief
*    Returns the value of the greatest component
*/
inline float Vector3::GetGreatestValue() const
{
	if (x > y) {
		if (x > z)
			return x;
		else
			return z;
	} else {
		if (y > z)
			return y;
		else
			return z;
	}
}

/**
*  @brief
*    Inverts the vector
*/
inline void Vector3::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}

/**
*  @brief
*    Returns the inverted vector
*/
inline Vector3 Vector3::GetInverted() const
{
	return Vector3(-x, -y, -z);
}

/**
*  @brief
*    Returns the length of the vector (also called magnitude)
*/
inline float Vector3::GetLength() const
{
	return Math::Sqrt(x*x + y*y + z*z);
}

/**
*  @brief
*    Returns the squared length of the vector (also called norm)
*/
inline float Vector3::GetSquaredLength() const
{
	return x*x + y*y + z*z;
}

/**
*  @brief
*    Sets the vector to the given length
*/
inline void Vector3::SetLength(float fLength)
{
	// Avoid division through zero...
	float fU = x*x + y*y + z*z;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = fLength/fU;
			x *= fScale;
			y *= fScale;
			z *= fScale;
		}
	}
}

/**
*  @brief
*    Normalizes the vector
*/
inline Vector3 &Vector3::Normalize()
{
	// Avoid division through zero...
	float fU = x*x + y*y + z*z;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = 1.0f/fU;
			x *= fScale;
			y *= fScale;
			z *= fScale;
		}
	}

	return *this;
}

/**
*  @brief
*    Returns the normalized vector
*/
inline Vector3 Vector3::GetNormalized() const
{
	// Avoid division through zero...
	float fU = x*x + y*y + z*z;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = 1.0f/fU;
			return Vector3(x*fScale, y*fScale, z*fScale);
		}
	}

	// Error!
	return Vector3::Zero;
}

/**
*  @brief
*    Returns the distance to another vector
*/
inline float Vector3::GetDistance(const Vector3 &vV) const
{
	const float fDX = vV.x-x;
	const float fDY = vV.y-y;
	const float fDZ = vV.z-z;
	return Math::Sqrt(fDX*fDX + fDY*fDY + fDZ*fDZ);
}

/**
*  @brief
*    Returns the squared distance to another vector
*/
inline float Vector3::GetSquaredDistance(const Vector3 &vV) const
{
	const float fDX = vV.x-x;
	const float fDY = vV.y-y;
	const float fDZ = vV.z-z;
	return fDX*fDX + fDY*fDY + fDZ*fDZ;
}

/**
*  @brief
*    Returns the dot product of two vectors
*/
inline float Vector3::DotProduct(const Vector3 &vV) const
{
	return x*vV.x + y*vV.y + z*vV.z;
}

/**
*  @brief
*    Project the vector onto another vector
*/
inline Vector3 Vector3::ProjectVector(const Vector3 &vA, const Vector3 &vB) const
{
	return vB*(vA.DotProduct(vB)/vB.DotProduct(vB));
}

/**
*  @brief
*    Calculates the angle between two vectors
*/
inline float Vector3::GetAngle(const Vector3 &vV) const
{
	return Math::ACos(DotProduct(vV)/(GetLength()*vV.GetLength()));
}

/**
*  @brief
*    Returns the cross product of this vector with another
*/
inline Vector3 Vector3::CrossProduct(const Vector3 &vV) const
{
	return Vector3(y*vV.z - z*vV.y,
				   z*vV.x - x*vV.z,
				   x*vV.y - y*vV.x);
}

/**
*  @brief
*    Calculates the cross product of two vectors
*/
inline Vector3 &Vector3::CrossProduct(const Vector3 &vV1, const Vector3 &vV2)
{
	x = vV1.y*vV2.z - vV1.z*vV2.y;
	y = vV1.z*vV2.x - vV1.x*vV2.z;
	z = vV1.x*vV2.y - vV1.y*vV2.x;
	return *this;
}

/**
*  @brief
*    Calculates a face/plane normal
*/
inline void Vector3::GetFaceNormal(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3)
{
	CrossProduct(vV1-vV2, vV1-vV3);
	Normalize();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
