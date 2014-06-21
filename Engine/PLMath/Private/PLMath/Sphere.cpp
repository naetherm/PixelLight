/*********************************************************\
 *  File: Sphere.cpp                                     *
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
#include "PLMath/Sphere.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all position components and the radius to 0
*/
Sphere::Sphere() :
	m_fRadius(0.0f)
{
}

/**
*  @brief
*    Copy constructor
*/
Sphere::Sphere(const Sphere &cSource) :
	m_fRadius(cSource.m_fRadius),
	m_vPos(cSource.m_vPos)
{
}

/**
*  @brief
*    Destructor
*/
Sphere::~Sphere()
{
}

/**
*  @brief
*    Copy operator
*/
Sphere &Sphere::operator =(const Sphere &cSource)
{
	// Copy data
	m_fRadius = cSource.m_fRadius;
	m_vPos    = cSource.m_vPos;

	// Return this
	return *this;
}

/**
*  @brief
*    Returns the radius of the sphere
*/
float Sphere::GetRadius() const
{
	return m_fRadius;
}

/**
*  @brief
*    Sets the radius of the sphere
*/
void Sphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

/**
*  @brief
*    Returns the current position
*/
const Vector3 &Sphere::GetPos() const
{
	// Return position
	return m_vPos;
}

/**
*  @brief
*    Set the current position
*/
void Sphere::SetPos(float fX, float fY, float fZ)
{
	// Set new position
	m_vPos.SetXYZ(fX, fY, fZ);
}

/**
*  @brief
*    Set the current position
*/
void Sphere::SetPos(const Vector3 &vPos)
{
	// Set new position
	m_vPos = vPos;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
