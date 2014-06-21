/*********************************************************\
 *  File: JointHinge.cpp                                 *
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
#include "PLPhysics/JointHinge.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
JointHinge::~JointHinge()
{
}

/**
*  @brief
*    Returns the origin of the hinge in world space
*/
const Vector3 &JointHinge::GetPivotPoint() const
{
	return m_vPivotPoint;
}

/**
*  @brief
*    Returns the line of action of the hinge in world space
*/
const Vector3 &JointHinge::GetPinDir() const
{
	return m_vPinDir;
}

/**
*  @brief
*    Returns the low range
*/
float JointHinge::GetLowRange() const
{
	return m_fLowRange;
}

/**
*  @brief
*    Sets the low range
*/
void JointHinge::SetLowRange(float fLowRange)
{
	m_fLowRange = fLowRange;
}

/**
*  @brief
*    Returns the high range
*/
float JointHinge::GetHighRange() const
{
	return m_fHighRange;
}

/**
*  @brief
*    Sets the high range
*/
void JointHinge::SetHighRange(float fHighRange)
{
	m_fHighRange = fHighRange;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
JointHinge::JointHinge(World &cWorld, JointImpl &cJointImpl, Body *pParentBody, Body *pChildBody,
					   const Vector3 &vPivotPoint, const Vector3 &vPinDir) :
	Joint(cWorld, cJointImpl, pParentBody, pChildBody),
	m_vPivotPoint(vPivotPoint),
	m_vPinDir(vPinDir),
	m_fLowRange(-180.0f),
	m_fHighRange(180.0f)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
