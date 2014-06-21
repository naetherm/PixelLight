/*********************************************************\
 *  File: Vector2i.inl                                   *
 *      2D integer vector inline implementation
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
inline Vector2i::Vector2i() :
	x(0),
	y(0)
{
}

/**
*  @brief
*    Constructor
*/
inline Vector2i::Vector2i(int nX, int nY) :
	x(nX),
	y(nY)
{
}

/**
*  @brief
*    Constructor
*/
inline Vector2i::Vector2i(const PLCore::String &sString)
{
	// The following method sets all components, so we don't need the initializer list in here
	FromString(sString);
}

/**
*  @brief
*    Destructor
*/
inline Vector2i::~Vector2i()
{
}

inline Vector2i::operator int *()
{
	return &x;
}

inline Vector2i::operator const int *() const
{
	return &x;
}

inline int &Vector2i::operator [](int nIndex)
{
	return nV[nIndex];
}

inline const int &Vector2i::operator [](int nIndex) const
{
	return nV[nIndex];
}

/**
*  @brief
*    Set X and Y value
*/
inline void Vector2i::Set(int nX, int nY)
{
	x = nX;
	y = nY;
}

/**
*  @brief
*    Set x
*/
inline void Vector2i::SetX(int nX)
{
	x = nX;
}

/**
*  @brief
*    Set y
*/
inline void Vector2i::SetY(int nY)
{
	y = nY;
}

/**
*  @brief
*    Assignment operator
*/
inline Vector2i &Vector2i::operator =(const Vector2i &vV)
{
	x = vV.x;
	y = vV.y;
	return *this;
}

/**
*  @brief
*    Compare operator
*/
inline bool Vector2i::operator ==(const Vector2i &vV) const
{
	return (x == vV.x && y == vV.y);
}

/**
*  @brief
*    Compare operator
*/
inline bool Vector2i::operator !=(const Vector2i &vV) const
{
	return (x != vV.x || y != vV.y);
}

/**
*  @brief
*    Compare operator lexicographically
*/
inline bool Vector2i::operator <(const Vector2i &vV) const
{
	return (x != vV.x) ? (x < vV.x) : (y < vV.y);
}

/**
*  @brief
*    Compare operator lexicographically
*/
inline bool Vector2i::operator >(const Vector2i &vV) const
{
	return (x != vV.x) ? (x > vV.x) : (y > vV.y);
}

/**
*  @brief
*    Compare operator lexicographically
*/
inline bool Vector2i::operator <=(const Vector2i &vV) const
{
	return (x != vV.x) ? (x <= vV.x) : (y <= vV.y);
}

/**
*  @brief
*    Compare operator lexicographically
*/
inline bool Vector2i::operator >=(const Vector2i &vV) const
{
	return (x != vV.x) ? (x >= vV.x) : (y >= vV.y);
}

/**
*  @brief
*    Addition operator
*/
inline Vector2i Vector2i::operator +(const Vector2i &vV) const
{
	return Vector2i(x + vV.x, y + vV.y);
}

/**
*  @brief
*    Addition operator
*/
inline Vector2i &Vector2i::operator +=(const Vector2i &vV)
{
	x += vV.x;
	y += vV.y;
	return *this;
}

/**
*  @brief
*    Subtraction operator
*/
inline Vector2i Vector2i::operator -(const Vector2i &vV) const
{
	return Vector2i(x - vV.x, y - vV.y);
}

/**
*  @brief
*    Subtraction operator
*/
inline Vector2i &Vector2i::operator -=(const Vector2i &vV)
{
	x -= vV.x;
	y -= vV.y;
	return *this;
}

/**
*  @brief
*    Multiplication operator
*/
inline Vector2i Vector2i::operator *(float fFactor) const
{
	return Vector2i(static_cast<int>(x*fFactor), static_cast<int>(y*fFactor));
}

/**
*  @brief
*    Multiplication operator
*/
inline Vector2i &Vector2i::operator *=(float fFactor)
{
	x = static_cast<int>(x*fFactor);
	y = static_cast<int>(y*fFactor);
	return *this;
}

/**
*  @brief
*    Division operator
*/
inline Vector2i Vector2i::operator /(float fFactor) const
{
	return Vector2i(static_cast<int>(x/fFactor), static_cast<int>(y/fFactor));
}

/**
*  @brief
*    Division operator
*/
inline Vector2i &Vector2i::operator /=(float fFactor)
{
	x = static_cast<int>(x/fFactor);
	y = static_cast<int>(y/fFactor);
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
