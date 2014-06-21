/*********************************************************\
 *  File: SensorAABoundingBox.cpp                        *
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
#include "PLPhysics/SensorAABoundingBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SensorAABoundingBox::~SensorAABoundingBox()
{
}

/**
*  @brief
*    Returns the minimum point
*/
const Vector3 &SensorAABoundingBox::GetMin() const
{
	return m_vMin;
}

/**
*  @brief
*    Sets the minimum point
*/
void SensorAABoundingBox::SetMin(const Vector3 &vMin)
{
	m_vMin = vMin;
}

/**
*  @brief
*    Returns the maximum point
*/
const Vector3 &SensorAABoundingBox::GetMax() const
{
	return m_vMax;
}

/**
*  @brief
*    Sets the maximum point
*/
void SensorAABoundingBox::SetMax(const Vector3 &vMax)
{
	m_vMax = vMax;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SensorAABoundingBox::SensorAABoundingBox(World &cWorld, const Vector3 &vMin, const Vector3 &vMax, uint32 nFlags) : Sensor(cWorld, nFlags),
	m_vMin(vMin),
	m_vMax(vMax)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
