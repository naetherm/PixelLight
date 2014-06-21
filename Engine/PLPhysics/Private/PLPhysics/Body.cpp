/*********************************************************\
 *  File: Body.cpp                                       *
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
#include "PLPhysics/BodyImpl.h"
#include "PLPhysics/Body.h"


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
Body::~Body()
{
	// Destroy the physics API body implementation
	delete m_pBodyImpl;
}

/**
*  @brief
*    Returns a reference to the physics API specific body implementation
*/
BodyImpl &Body::GetBodyImpl() const
{
	return *m_pBodyImpl;
}

/**
*  @brief
*    Returns whether the body is currently active or not
*/
bool Body::IsActive() const
{
	return m_pBodyImpl->IsActive();
}

/**
*  @brief
*    Sets whether the body is currently active or not
*/
void Body::SetActive(bool bActive)
{
	return m_pBodyImpl->SetActive(bActive);
}

/**
*  @brief
*    Returns the collision volume of the physics body
*/
float Body::GetCollisionVolume() const
{
	return m_pBodyImpl->GetCollisionVolume();
}

/**
*  @brief
*    Returns the mass of the physics body
*/
float Body::GetMass() const
{
	return m_pBodyImpl->GetMass();
}

/**
*  @brief
*    Gets the relative center of the mass
*/
void Body::GetCenterOfMass(Vector3 &vPosition) const
{
	m_pBodyImpl->GetCenterOfMass(vPosition);
}

/**
*  @brief
*    Sets the relative center of the mass
*/
void Body::SetCenterOfMass(const Vector3 &vPosition)
{
	m_pBodyImpl->SetCenterOfMass(vPosition);
}

/**
*  @brief
*    Returns whether the body is frozen automatically or not
*/
bool Body::IsAutoFreeze() const
{
	return m_pBodyImpl->IsAutoFreeze();
}

/**
*  @brief
*    Sets whether the body is frozen automatically or not
*/
void Body::SetAutoFreeze(bool bAutoFreeze)
{
	m_pBodyImpl->SetAutoFreeze(bAutoFreeze);
}

/**
*  @brief
*    Returns the freeze threshold
*/
void Body::GetFreezeThreshold(float &fLinearVelocity, float &fAngularVelocity, uint32 &nSteps) const
{
	return m_pBodyImpl->GetFreezeThreshold(fLinearVelocity, fAngularVelocity, nSteps);
}

/**
*  @brief
*    Sets the freeze threshold
*/
void Body::SetFreezeThreshold(float fLinearVelocity, float fAngularVelocity, uint32 nSteps)
{
	return m_pBodyImpl->SetFreezeThreshold(fLinearVelocity, fAngularVelocity, nSteps);
}

/**
*  @brief
*    Returns whether the body is currently frozen or not
*/
bool Body::IsFrozen() const
{
	return m_pBodyImpl->IsFrozen();
}

/**
*  @brief
*    Sets whether the body is currently frozen or not
*/
void Body::SetFrozen(bool bFrozen)
{
	return m_pBodyImpl->SetFrozen(bFrozen);
}

/**
*  @brief
*    Returns whether the body can collide or not
*/
bool Body::IsCollisionActive() const
{
	return m_pBodyImpl->IsCollisionActive();
}

/**
*  @brief
*    Sets whether the body can collide or not
*/
void Body::SetCollisionActive(bool bActive)
{
	return m_pBodyImpl->SetCollisionActive(bActive);
}

/**
*  @brief
*    Returns whether the body ignores the gravity or not
*/
bool Body::GetUseGravity() const
{
	return m_pBodyImpl->GetUseGravity();
}

/**
*  @brief
*    Sets whether the body ignores the gravity or not
*/
void Body::SetUseGravity(bool bUseGravity)
{
	m_pBodyImpl->SetUseGravity(bUseGravity);
}

/**
*  @brief
*    Gets the body position
*/
void Body::GetPosition(Vector3 &vPosition) const
{
	m_pBodyImpl->GetPosition(vPosition);
}

/**
*  @brief
*    Sets the body position
*/
void Body::SetPosition(const Vector3 &vPosition)
{
	m_pBodyImpl->SetPosition(vPosition);
}

/**
*  @brief
*    Gets the body rotation
*/
void Body::GetRotation(Quaternion &qRotation) const
{
	m_pBodyImpl->GetRotation(qRotation);
}

/**
*  @brief
*    Sets the body rotation
*/
void Body::SetRotation(const Quaternion &qRotation)
{
	m_pBodyImpl->SetRotation(qRotation);
}

/**
*  @brief
*    Gets the body transform matrix
*/
void Body::GetTransformMatrix(Matrix3x4 &mTrans) const
{
	m_pBodyImpl->GetTransformMatrix(mTrans);
}

/**
*  @brief
*    Sets the body transform matrix
*/
void Body::SetTransformMatrix(const Matrix3x4 &mTrans)
{
	m_pBodyImpl->SetTransformMatrix(mTrans);
}

/**
*  @brief
*    Gets the global space axis aligned bounding box of the body
*/
void Body::GetAABoundingBox(AABoundingBox &cAABoundingBox) const
{
	m_pBodyImpl->GetAABoundingBox(cAABoundingBox);
}

/**
*  @brief
*    Gets the linear body velocity
*/
void Body::GetLinearVelocity(Vector3 &vVelocity) const
{
	m_pBodyImpl->GetLinearVelocity(vVelocity);
}

/**
*  @brief
*    Sets the linear body velocity
*/
void Body::SetLinearVelocity(const Vector3 &vVelocity)
{
	m_pBodyImpl->SetLinearVelocity(vVelocity);
}

/**
*  @brief
*    Gets the angular body velocity
*/
void Body::GetAngularVelocity(Vector3 &vVelocity) const
{
	m_pBodyImpl->GetAngularVelocity(vVelocity);
}

/**
*  @brief
*    Sets the angular body velocity
*/
void Body::SetAngularVelocity(const Vector3 &vVelocity)
{
	m_pBodyImpl->SetAngularVelocity(vVelocity);
}

/**
*  @brief
*    Gets the force applied to the body at the next simulation update
*/
void Body::GetForce(Vector3 &vForce) const
{
	m_pBodyImpl->GetForce(vForce);
}

/**
*  @brief
*    Adds a force to the body
*/
void Body::AddForce(const Vector3 &vForce)
{
	m_pBodyImpl->AddForce(vForce);
}

/**
*  @brief
*    Sets the force applied to the body at the next simulation update
*/
void Body::SetForce(const Vector3 &vForce)
{
	m_pBodyImpl->SetForce(vForce);
}

/**
*  @brief
*    Gets the torque applied to the body at the next simulation update
*/
void Body::GetTorque(Vector3 &vTorque) const
{
	m_pBodyImpl->GetTorque(vTorque);
}

/**
*  @brief
*    Adds a torque to the body
*/
void Body::AddTorque(const Vector3 &vTorque)
{
	m_pBodyImpl->AddTorque(vTorque);
}

/**
*  @brief
*    Sets the torque applied to the body at the next simulation update
*/
void Body::SetTorque(const Vector3 &vTorque)
{
	m_pBodyImpl->SetTorque(vTorque);
}

/**
*  @brief
*    Returns the collision group the body is in
*/
uint8 Body::GetCollisionGroup() const
{
	return m_pBodyImpl->GetCollisionGroup();
}

/**
*  @brief
*    Sets the collision group the body is in
*/
void Body::SetCollisionGroup(uint8 nCollisionGroup)
{
	return m_pBodyImpl->SetCollisionGroup(nCollisionGroup);
}

/**
*  @brief
*    Returns the number of partner bodies
*/
uint32 Body::GetNumOfPartners() const
{
	return m_pBodyImpl->GetNumOfPartners();
}

/**
*  @brief
*    Returns a partner body
*/
Body *Body::GetPartner(uint32 nIndex) const
{
	return m_pBodyImpl->GetPartner(nIndex);
}


//[-------------------------------------------------------]
//[ Public virtual Body functions                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the mass of the physics body
*/
void Body::SetMass(float fMass)
{
	m_pBodyImpl->SetMass(fMass);
}


//[-------------------------------------------------------]
//[ Public virtual Element functions                      ]
//[-------------------------------------------------------]
bool Body::IsBody() const
{
	return true;
}

bool Body::IsJoint() const
{
	return false;
}

bool Body::IsSensor() const
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
Body::Body(World &cWorld, BodyImpl &cBodyImpl) : Element(cWorld),
	m_pBodyImpl(&cBodyImpl)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
