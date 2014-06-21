/*********************************************************\
 *  File: Joint.cpp                                      *
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
#include <PLMath/Vector3.h>
#include "PLPhysics/ElementHandler.h"
#include "PLPhysics/JointImpl.h"
#include "PLPhysics/Joint.h"


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
Joint::~Joint()
{
	// Destroy the physics body handlers
	delete m_pParentBodyHandler;
	delete m_pChildBodyHandler;

	// Destroy the physics API joint implementation
	delete m_pJointImpl;
}

/**
*  @brief
*    Returns a reference to the physics API specific joint implementation
*/
JointImpl &Joint::GetJointImpl() const
{
	return *m_pJointImpl;
}

/**
*  @brief
*    Returns a pointer to the parent rigid body
*/
Body *Joint::GetParentBody() const
{
	return reinterpret_cast<Body*>(m_pParentBodyHandler->GetElement());
}

/**
*  @brief
*    Returns a pointer to the attached rigid body
*/
Body *Joint::GetChildBody() const
{
	return reinterpret_cast<Body*>(m_pChildBodyHandler->GetElement());
}


//[-------------------------------------------------------]
//[ Public virtual Joint functions                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Gets the current pivot point of the joint
*/
void Joint::GetCurrentPivotPoint(Vector3 &vPosition) const
{
	// Nothing do to in here
}

/**
*  @brief
*    Returns whether the joint is breakable or not
*/
bool Joint::IsBreakable() const
{
	return m_pJointImpl->IsBreakable();
}

/**
*  @brief
*    Sets whether the joint is breakable or not
*/
void Joint::SetBreakable(bool bBreakable)
{
	m_pJointImpl->SetBreakable(bBreakable);
}

/**
*  @brief
*    Returns the break force
*/
void Joint::GetBreakForce(Vector3 &vForce) const
{
	m_pJointImpl->GetBreakForce(vForce);
}

/**
*  @brief
*    Sets the break force
*/
void Joint::SetBreakForce(const Vector3 &vForce)
{
	m_pJointImpl->SetBreakForce(vForce);
}

/**
*  @brief
*    Returns the break torque
*/
void Joint::GetBreakTorque(Vector3 &vTorque) const
{
	m_pJointImpl->GetBreakTorque(vTorque);
}

/**
*  @brief
*    Sets the break torque
*/
void Joint::SetBreakTorque(const Vector3 &vTorque)
{
	m_pJointImpl->SetBreakTorque(vTorque);
}


//[-------------------------------------------------------]
//[ Public virtual Element functions                      ]
//[-------------------------------------------------------]
bool Joint::IsBody() const
{
	return false;
}

bool Joint::IsJoint() const
{
	return true;
}

bool Joint::IsSensor() const
{
	return false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Joint::Joint(World &cWorld, JointImpl &cJointImpl, Body *pParentBody, Body *pChildBody) : Element(cWorld),
	m_pJointImpl(&cJointImpl),
	m_pParentBodyHandler(new ElementHandler()),
	m_pChildBodyHandler(new ElementHandler())
{
	// Set the physics bodies this joint is attached to
	m_pParentBodyHandler->SetElement(reinterpret_cast<Element*>(pParentBody));
	m_pChildBodyHandler ->SetElement(reinterpret_cast<Element*>(pChildBody));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
