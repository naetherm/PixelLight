/*********************************************************\
 *  File: JointUniversal.cpp                             *
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
#include "PLPhysics/JointUniversal.h"


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
JointUniversal::~JointUniversal()
{
}

/**
*  @brief
*    Returns the origin of the universal in world space
*/
const Vector3 &JointUniversal::GetPivotPoint() const
{
	return m_vPivotPoint;
}

/**
*  @brief
*    Returns the first axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in world space
*/
const Vector3 &JointUniversal::GetPinDir1() const
{
	return m_vPinDir1;
}

/**
*  @brief
*    Returns the second axis of rotation fixed on 'Parent' body and perpendicular to 'PinDir1' in world space
*/
const Vector3 &JointUniversal::GetPinDir2() const
{
	return m_vPinDir2;
}

/**
*  @brief
*    Returns the low range of the first axis
*/
float JointUniversal::GetLowRange1() const
{
	return m_fLowRange1;
}

/**
*  @brief
*    Sets the low range of the first axis
*/
void JointUniversal::SetLowRange1(float fLowRange)
{
	m_fLowRange1 = fLowRange;
}

/**
*  @brief
*    Returns the high range of the first axis
*/
float JointUniversal::GetHighRange1() const
{
	return m_fHighRange1;
}

/**
*  @brief
*    Sets the high range of the first axis
*/
void JointUniversal::SetHighRange1(float fHighRange)
{
	m_fHighRange1 = fHighRange;
}

/**
*  @brief
*    Returns the low range of the second axis
*/
float JointUniversal::GetLowRange2() const
{
	return m_fLowRange2;
}

/**
*  @brief
*    Sets the low range of the second axis
*/
void JointUniversal::SetLowRange2(float fLowRange)
{
	m_fLowRange2 = fLowRange;
}

/**
*  @brief
*    Returns the high range of the second axis
*/
float JointUniversal::GetHighRange2() const
{
	return m_fHighRange2;
}

/**
*  @brief
*    Sets the high range of the second axis
*/
void JointUniversal::SetHighRange2(float fHighRange)
{
	m_fHighRange2 = fHighRange;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
JointUniversal::JointUniversal(World &cWorld, JointImpl &cJointImpl, Body *pParentBody, Body *pChildBody, const Vector3 &vPivotPoint,
							   const Vector3 &vPinDir1, const Vector3 &vPinDir2) :
	Joint(cWorld, cJointImpl, pParentBody, pChildBody),
	m_vPivotPoint(vPivotPoint),
	m_vPinDir1(vPinDir1),
	m_vPinDir2(vPinDir2),
	m_fLowRange1(-180.0f),
	m_fHighRange1(180.0f),
	m_fLowRange2(-180.0f),
	m_fHighRange2(180.0f)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
