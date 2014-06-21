/*********************************************************\
 *  File: Vector2.inl                                    *
 *      2D vector inline implementation
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
inline Vector2 operator *(float fS, const Vector2 &vV)
{
	return Vector2(vV.x*fS, vV.y*fS);
}


//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all components to 0
*/
inline Vector2::Vector2() :
	x(0.0f),
	y(0.0f)
{
}

inline Vector2::Vector2(float fX, float fY) :
	x(fX),
	y(fY)
{
}

inline Vector2::Vector2(const float fV[]) :
	x(fV[0]),
	y(fV[1])
{
}

inline Vector2::Vector2(const Vector2 &vV) :
	x(vV.x),
	y(vV.y)
{
}

inline Vector2::Vector2(const PLCore::String &sString)
{
	FromString(sString);
}


//[-------------------------------------------------------]
//[ Destructor                                            ]
//[-------------------------------------------------------]
inline Vector2::~Vector2()
{
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
inline Vector2 &Vector2::operator =(const float fV[])
{
	x = fV[0];
	y = fV[1];
	return *this;
}

inline Vector2 &Vector2::operator =(const Vector2 &vV)
{
	x = vV.x;
	y = vV.y;
	return *this;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
inline bool Vector2::operator ==(const Vector2 &vV) const
{
	return (x == vV.x && y == vV.y);
}

inline bool Vector2::operator !=(const Vector2 &vV) const
{
	return (x != vV.x ||y != vV.y);
}

inline bool Vector2::operator <(const Vector2 &vV) const
{
	return (x != vV.x) ? (x < vV.x) : (y < vV.y);
}

inline bool Vector2::operator >(const Vector2 &vV) const
{
	return (x != vV.x) ? (x > vV.x) : (y > vV.y);
}

inline bool Vector2::operator <=(const Vector2 &vV) const
{
	return (x != vV.x) ? (x <= vV.x) : (y <= vV.y);
}

inline bool Vector2::operator >=(const Vector2 &vV) const
{
	return (x != vV.x) ? (x >= vV.x) : (y >= vV.y);
}


//[-------------------------------------------------------]
//[ Vector                                                ]
//[-------------------------------------------------------]
inline Vector2 Vector2::operator +(const Vector2 &vV) const
{
	return Vector2(x+vV.x, y+vV.y);
}

inline Vector2 Vector2::operator +(float fN) const
{
	return Vector2(x+fN, y+fN);
}

inline Vector2 &Vector2::operator +=(const Vector2 &vV)
{
	x += vV.x;
	y += vV.y;
	return *this;
}

inline Vector2 &Vector2::operator +=(float fN)
{
	x += fN;
	y += fN;
	return *this;
}

inline Vector2 Vector2::operator -() const
{
	return Vector2(-x, -y);
}

inline Vector2 Vector2::operator -(float fN) const
{
	return Vector2(x-fN, y-fN);
}

inline Vector2 Vector2::operator -(const Vector2 &vV) const
{
	return Vector2(x-vV.x, y-vV.y);
}

inline Vector2 &Vector2::operator -=(const Vector2 &vV)
{
	x -= vV.x;
	y -= vV.y;
	return *this;
}

inline Vector2 &Vector2::operator -=(float fN)
{
	x -= fN;
	y -= fN;
	return *this;
}

inline Vector2 Vector2::operator *(const Vector2 &vV) const
{
	return Vector2(x*vV.x, y*vV.y);
}

inline Vector2 Vector2::operator *(float fS) const
{
	return Vector2(x*fS, y*fS);
}

inline Vector2 &Vector2::operator *=(const Vector2 &vV)
{
	x *= vV.x;
	y *= vV.y;
	return *this;
}

inline Vector2 &Vector2::operator *=(float fS)
{
	x *= fS;
	y *= fS;
	return *this;
}

inline Vector2 Vector2::operator /(const Vector2 &vV) const
{
	return Vector2(x/vV.x, y/vV.y);
}

inline Vector2 Vector2::operator /(float fS) const
{
	return Vector2(x/fS, y/fS);
}

inline Vector2 &Vector2::operator /=(const Vector2 &vV)
{
	x /= vV.x;
	y /= vV.y;
	return *this;
}

inline Vector2 &Vector2::operator /=(float fS)
{
	x /= fS;
	y /= fS;
	return *this;
}


//[-------------------------------------------------------]
//[ Get and set                                           ]
//[-------------------------------------------------------]
inline Vector2::operator float *()
{
	return &x;
}

inline Vector2::operator const float *() const
{
	return &x;
}

inline float &Vector2::operator [](int nIndex)
{
	return fV[nIndex];
}

inline const float &Vector2::operator [](int nIndex) const
{
	return fV[nIndex];
}

inline void Vector2::GetXY(float &fX, float &fY) const
{
	fX = x;
	fY = y;
}

inline float Vector2::GetX() const
{
	return x;
}

inline float Vector2::GetY() const
{
	return y;
}

inline void Vector2::SetXY(float fX, float fY)
{
	x = fX;
	y = fY;
}

inline void Vector2::SetXY(const float fV[])
{
	x = fV[0];
	y = fV[1];
}

inline void Vector2::SetX(float fX)
{
	x = fX;
}

inline void Vector2::SetY(float fY)
{
	y = fY;
}

inline void Vector2::IncXY(float fX, float fY)
{
	x += fX;
	y += fY;
}

inline void Vector2::IncXY(const float fV[])
{
	x += fV[0];
	y += fV[1];
}

inline void Vector2::IncX(float fX)
{
	x += fX;
}

inline void Vector2::IncY(float fY)
{
	y += fY;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether the vector is null or not
*/
inline bool Vector2::IsNull() const
{
	return (!x && !y);
}

/**
*  @brief
*    Returns whether the vector is packed (within range of 0-1) or not
*/
inline bool Vector2::IsPacked() const
{
	return (x >= 0.0f && x <= 1.0f && y >= 0.0f && y <= 1.0f);
}

/**
*  @brief
*    Packs/clamps the vector into a range of 0-1
*/
inline void Vector2::PackTo01()
{
	Normalize();
	x = x*0.5f + 0.5f;
	y = y*0.5f + 0.5f;
}

/**
*  @brief
*    Returns a vector which is packed/clamped into the range of 0-1
*/
inline Vector2 Vector2::GetPackedTo01() const
{
	Vector2 vTemp(x, y);
	vTemp.Normalize();
	vTemp.x = vTemp.x*0.5f + 0.5f;
	vTemp.y = vTemp.y*0.5f + 0.5f;
	return vTemp;
}

/**
*  @brief
*    Unpacks the packed vector into a range of -1 to 1
*/
inline void Vector2::UnpackFrom01()
{
	x = x*2 - 1;
	y = y*2 - 1;
}

/**
*  @brief
*    Returns a unpacked vector of the range of -1 to 1
*/
inline Vector2 Vector2::GetUnpackedFrom01() const
{
	return Vector2(x*2 - 1, y*2 - 1);
}

/**
*  @brief
*    Returns the smallest component
*/
inline Vector2::Component Vector2::GetSmallestComponent() const
{
	return (x < y) ? X : Y;
}

/**
*  @brief
*    Returns the value of the smallest component
*/
inline float Vector2::GetSmallestValue() const
{
	return (x < y) ? x : y;
}

/**
*  @brief
*    Returns the greatest component
*/
inline Vector2::Component Vector2::GetGreatestComponent() const
{
	return (x > y) ? X : Y;
}

/**
*  @brief
*    Returns the value of the greatest component
*/
inline float Vector2::GetGreatestValue() const
{
	return (x > y) ? x : y;
}

/**
*  @brief
*    Inverts the vector
*/
inline void Vector2::Invert()
{
	x = -x;
	y = -y;
}

/**
*  @brief
*    Returns the inverted vector
*/
inline Vector2 Vector2::GetInverted() const
{
	return Vector2(-x, -y);
}

/**
*  @brief
*    Returns the length of the vector (also called magnitude)
*/
inline float Vector2::GetLength() const
{
	return Math::Sqrt(x*x + y*y);
}

/**
*  @brief
*    Returns the squared length of the vector (also called norm)
*/
inline float Vector2::GetSquaredLength() const
{
	return x*x + y*y;
}

/**
*  @brief
*    Sets the vector to the given length
*/
inline void Vector2::SetLength(float fLength)
{
	// Avoid division through zero...
	float fU = x*x + y*y;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = fLength/fU;
			x *= fScale;
			y *= fScale;
		}
	}
}

/**
*  @brief
*    Normalizes the vector
*/
inline Vector2 &Vector2::Normalize()
{
	// Avoid division through zero...
	float fU = x*x + y*y;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = 1.0f/fU;
			x *= fScale;
			y *= fScale;
		}
	}

	return *this;
}

/**
*  @brief
*    Returns the normalized vector
*/
inline Vector2 Vector2::GetNormalized() const
{
	// Avoid division through zero...
	float fU = x*x + y*y;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fScale = 1.0f/fU;
			return Vector2(x*fScale, y*fScale);
		}
	}

	// Error!
	return Vector2::Zero;
}

/**
*  @brief
*    Returns the distance to another vector
*/
inline float Vector2::GetDistance(const Vector2 &vV) const
{
	const float fDX = vV.x-x;
	const float fDY = vV.y-y;
	return Math::Sqrt(fDX*fDX + fDY*fDY);
}

/**
*  @brief
*    Returns the squared distance to another vector
*/
inline float Vector2::GetSquaredDistance(const Vector2 &vV) const
{
	const float fDX = vV.x-x;
	const float fDY = vV.y-y;
	return fDX*fDX + fDY*fDY;
}

/**
*  @brief
*    Returns the dot product of two vectors
*/
inline float Vector2::DotProduct(const Vector2 &vV) const
{
	return x*vV.x + y*vV.y;
}

/**
*  @brief
*    Project the vector onto another vector
*/
inline Vector2 Vector2::ProjectVector(const Vector2 &vA, const Vector2 &vB) const
{
	return vB*(vA.DotProduct(vB)/vB.DotProduct(vB));
}

/**
*  @brief
*    Calculates the angle between two vectors
*/
inline float Vector2::GetAngle(const Vector2 &vV) const
{
	return Math::ACos(DotProduct(vV)/(GetLength()*vV.GetLength()));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
