/*********************************************************\
 *  File: Vector3i.inl                                   *
 *      3D integer vector inline implementation
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
*    Default constructor setting all components to 0
*/
inline Vector3i::Vector3i() :
	x(0),
	y(0),
	z(0)
{
}

/**
*  @brief
*    Constructor
*/
inline Vector3i::Vector3i(int nX, int nY, int nZ) :
	x(nX),
	y(nY),
	z(nZ)
{
}

/**
*  @brief
*    Constructor
*/
inline Vector3i::Vector3i(const PLCore::String &sString)
{
	// The following method sets all components, so we don't need the initializer list in here
	FromString(sString);
}

/**
*  @brief
*    Destructor
*/
inline Vector3i::~Vector3i()
{
}

inline Vector3i::operator int *()
{
	return &x;
}

inline Vector3i::operator const int *() const
{
	return &x;
}

inline int &Vector3i::operator [](int nIndex)
{
	return nV[nIndex];
}

inline const int &Vector3i::operator [](int nIndex) const
{
	return nV[nIndex];
}

/**
*  @brief
*    Set X, Y and Z value
*/
inline void Vector3i::Set(int nX, int nY, int nZ)
{
	x = nX;
	y = nY;
	z = nZ;
}

/**
*  @brief
*    Set x
*/
inline void Vector3i::SetX(int nX)
{
	x = nX;
}

/**
*  @brief
*    Set y
*/
inline void Vector3i::SetY(int nY)
{
	y = nY;
}

/**
*  @brief
*    Set z
*/
inline void Vector3i::SetZ(int nZ)
{
	z = nZ;
}

/**
*  @brief
*    Assignment operator
*/
inline Vector3i &Vector3i::operator =(const Vector3i &vV)
{
	x = vV.x;
	y = vV.y;
	z = vV.z;
	return *this;
}

/**
*  @brief
*    Compare operator
*/
inline bool Vector3i::operator ==(const Vector3i &vV) const
{
	return (x == vV.x && y == vV.y && z == vV.z);
}

/**
*  @brief
*    Compare operator
*/
inline bool Vector3i::operator !=(const Vector3i &vV) const
{
	return (x != vV.x || y != vV.y || z != vV.z);
}

/**
*  @brief
*    Compares two vectors lexicographically
*/
inline bool Vector3i::operator <(const Vector3i &vV) const
{
	if (x != vV.x)
		return (x < vV.x);
	else
		return (y != vV.y) ? (y < vV.y) : (z < vV.z);
}

/**
*  @brief
*    Compares two vectors lexicographically
*/
inline bool Vector3i::operator >(const Vector3i &vV) const
{
	if (x != vV.x)
		return (x > vV.x);
	else
		return (y != vV.y) ? (y > vV.y) : (z > vV.z);
}

/**
*  @brief
*    Compares two vectors lexicographically
*/
inline bool Vector3i::operator <=(const Vector3i &vV) const
{
	if (x != vV.x)
		return (x <= vV.x);
	else
		return (y != vV.y) ? (y <= vV.y) : (z <= vV.z);
}

/**
*  @brief
*    Compares two vectors lexicographically
*/
inline bool Vector3i::operator >=(const Vector3i &vV) const
{
	if (x != vV.x)
		return (x >= vV.x);
	else
		return (y != vV.y) ? (y >= vV.y) : (z >= vV.z);
}

/**
*  @brief
*    Addition operator
*/
inline Vector3i Vector3i::operator +(const Vector3i &vV) const
{
	return Vector3i(x + vV.x, y + vV.y, z + vV.z);
}

/**
*  @brief
*    Addition operator
*/
inline Vector3i &Vector3i::operator +=(const Vector3i &vV)
{
	x += vV.x;
	y += vV.y;
	z += vV.z;
	return *this;
}

/**
*  @brief
*    Subtraction operator
*/
inline Vector3i Vector3i::operator -(const Vector3i &vV) const
{
	return Vector3i(x - vV.x, y - vV.y, z - vV.z);
}

/**
*  @brief
*    Subtraction operator
*/
inline Vector3i &Vector3i::operator -=(const Vector3i &vV)
{
	x -= vV.x;
	y -= vV.y;
	z -= vV.z;
	return *this;
}

/**
*  @brief
*    Multiplication operator
*/
inline Vector3i Vector3i::operator *(float fFactor) const
{
	return Vector3i(static_cast<int>(x*fFactor), static_cast<int>(y*fFactor), static_cast<int>(z*fFactor));
}

/**
*  @brief
*    Multiplication operator
*/
inline Vector3i &Vector3i::operator *=(float fFactor)
{
	x = static_cast<int>(x*fFactor);
	y = static_cast<int>(y*fFactor);
	z = static_cast<int>(z*fFactor);
	return *this;
}

/**
*  @brief
*    Division operator
*/
inline Vector3i Vector3i::operator /(float fFactor) const
{
	return Vector3i(static_cast<int>(x/fFactor), static_cast<int>(y/fFactor), static_cast<int>(z/fFactor));
}

/**
*  @brief
*    Division operator
*/
inline Vector3i &Vector3i::operator /=(float fFactor)
{
	x = static_cast<int>(x/fFactor);
	y = static_cast<int>(y/fFactor);
	z = static_cast<int>(z/fFactor);
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
