/*********************************************************\
 *  File: BodyImpl.cpp                                   *
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
#include <PLMath/Matrix3x4.h>
#include <PLMath/AABoundingBox.h>
#include <PLPhysics/Body.h>
#include "PLPhysicsNull/World.h"
#include "PLPhysicsNull/BodyImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the null physics body
*/
void BodyImpl::InitializeNullBody(PLPhysics::Body &cBody)
{
	// Set data
	m_pBody = &cBody;
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::BodyImpl functions          ]
//[-------------------------------------------------------]
PLPhysics::Body *BodyImpl::GetBody() const
{
	return m_pBody;
}

bool BodyImpl::IsActive() const
{
	return m_bActive;
}

void BodyImpl::SetActive(bool bActive)
{
	m_bActive = bActive;
}

float BodyImpl::GetCollisionVolume() const
{
	return m_fCollisionVolume;
}

float BodyImpl::GetMass() const
{
	return m_fMass;
}

void BodyImpl::SetMass(float fMass)
{
	m_fMass = fMass;
}

void BodyImpl::GetCenterOfMass(Vector3 &vPosition) const
{
	vPosition = m_vCenterOfMass;
}

void BodyImpl::SetCenterOfMass(const Vector3 &vPosition)
{
	m_vCenterOfMass = vPosition;
}

void BodyImpl::GetPosition(Vector3 &vPosition) const
{
	vPosition = m_vPosition;
}

void BodyImpl::SetPosition(const Vector3 &vPosition)
{
	m_vPosition = vPosition;
}

void BodyImpl::GetRotation(Quaternion &qRotation) const
{
	qRotation = m_qRotation;
}

void BodyImpl::SetRotation(const Quaternion &qRotation)
{
	m_qRotation = qRotation;
}

void BodyImpl::GetTransformMatrix(Matrix3x4 &mTrans) const
{
	m_qRotation.ToRotationMatrix(mTrans);
	mTrans.SetTranslation(m_vPosition);
}

void BodyImpl::SetTransformMatrix(const Matrix3x4 &mTrans)
{
	mTrans.GetTranslation(m_vPosition);
	m_qRotation.FromRotationMatrix(mTrans);
}

void BodyImpl::GetAABoundingBox(AABoundingBox &cAABoundingBox) const
{
	// Not implemented
	cAABoundingBox.vMin = cAABoundingBox.vMax = Vector3::Zero;
}

void BodyImpl::GetLinearVelocity(Vector3 &vVelocity) const
{
	vVelocity = m_vLinearVelocity;
}

void BodyImpl::SetLinearVelocity(const Vector3 &vVelocity)
{
	m_vLinearVelocity = vVelocity;
}

void BodyImpl::GetAngularVelocity(Vector3 &vVelocity) const
{
	vVelocity = m_vAngularVelocity;
}

void BodyImpl::SetAngularVelocity(const Vector3 &vVelocity)
{
	m_vAngularVelocity = vVelocity;
}

void BodyImpl::GetForce(Vector3 &vForce) const
{
	vForce = m_vForce;
}

void BodyImpl::AddForce(const Vector3 &vForce)
{
	m_vForce += vForce;
	if (!m_bChangedByUser) {
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
		m_bChangedByUser = true;
	}
}

void BodyImpl::SetForce(const Vector3 &vForce)
{
	m_vForce = vForce;
	if (!m_bChangedByUser) {
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
		m_bChangedByUser = true;
	}
}

void BodyImpl::GetTorque(Vector3 &vTorque) const
{
	vTorque = m_vTorque;
}

void BodyImpl::AddTorque(const Vector3 &vTorque)
{
	m_vTorque += vTorque;
	if (!m_bChangedByUser) {
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
		m_bChangedByUser = true;
	}
}

void BodyImpl::SetTorque(const Vector3 &vTorque)
{
	m_vTorque = vTorque;
	if (!m_bChangedByUser) {
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
		m_bChangedByUser = true;
	}
}

bool BodyImpl::IsAutoFreeze() const
{
	return m_bAutoFreeze;
}

void BodyImpl::SetAutoFreeze(bool bAutoFreeze)
{
	m_bAutoFreeze = bAutoFreeze;
}

void BodyImpl::GetFreezeThreshold(float &fLinearVelocity, float &fAngularVelocity, uint32 &nSteps) const
{
	fLinearVelocity  = m_fFreezeLinearVelocity;
	fAngularVelocity = m_fFreezeAngularVelocity;
	nSteps			 = m_nFreezeSteps;
}

void BodyImpl::SetFreezeThreshold(float fLinearVelocity, float fAngularVelocity, uint32 nSteps)
{
	m_fFreezeLinearVelocity  = fLinearVelocity;
	m_fFreezeAngularVelocity = fAngularVelocity;
	m_nFreezeSteps			 = nSteps;
}

bool BodyImpl::IsFrozen() const
{
	return m_bFrozen;
}

void BodyImpl::SetFrozen(bool bFrozen)
{
	m_bFrozen = bFrozen;
}

bool BodyImpl::IsCollisionActive() const
{
	return m_bCollisionActive;
}

void BodyImpl::SetCollisionActive(bool bActive)
{
	m_bCollisionActive = bActive;
}

bool BodyImpl::GetUseGravity() const
{
	return m_bUseGravity;
}

void BodyImpl::SetUseGravity(bool bUseGravity)
{
	m_bUseGravity = bUseGravity;
}

uint8 BodyImpl::GetCollisionGroup() const
{
	return m_nCollisionGroup;
}

void BodyImpl::SetCollisionGroup(uint8 nCollisionGroup)
{
	if (nCollisionGroup <= 31)
		m_nCollisionGroup = nCollisionGroup;
}

uint32 BodyImpl::GetNumOfPartners() const
{
	return m_lstPartnerBodies.GetNumOfElements();
}

PLPhysics::Body *BodyImpl::GetPartner(uint32 nIndex) const
{
	return m_lstPartnerBodies[nIndex];
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BodyImpl::BodyImpl() :
	m_pBody(nullptr),
	m_bActive(true),
	m_fCollisionVolume(0.0f),
	m_fMass(0.0f),
	m_bAutoFreeze(true),
	m_bFrozen(true),
	m_bCollisionActive(true),
	m_bUseGravity(true),
	m_fFreezeLinearVelocity(0.0f),
	m_fFreezeAngularVelocity(0.0f),
	m_nFreezeSteps(0),
	m_nCollisionGroup(0),
	m_bChangedByUser(false)
{
}

/**
*  @brief
*    Destructor
*/
BodyImpl::~BodyImpl()
{
	// Remove all relationships of this body
	while (m_lstPartnerBodies.GetNumOfElements())
		GetBody()->GetWorld().SetBodyPairFlags(*GetBody(), *m_lstPartnerBodies[0], 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNull
