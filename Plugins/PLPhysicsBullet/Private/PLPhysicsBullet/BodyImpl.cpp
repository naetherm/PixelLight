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
#include <PLMath/Matrix3x3.h>
#include <PLMath/Matrix3x4.h>
#include <PLMath/AABoundingBox.h>
#include <PLPhysics/Body.h>
#include "PLPhysicsBullet/World.h"
#include "PLPhysicsBullet/BodyImpl.h"
#include "PLPhysicsBullet/Helper.h"
#include "PLPhysicsBullet/BodyMotionState.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the Bullet physics body
*/
btRigidBody *BodyImpl::GetBulletBody() const
{
	return m_pBulletBody;
}

/**
*  @brief
*    Initializes the Bullet physics body
*/
void BodyImpl::InitializeBulletBody(PLPhysics::Body &cBody, btCollisionShape &cBulletCollisionShape, bool bStatic)
{
	float mass = bStatic ? 0.0f : 0.1f;
		
	btVector3 localInertia(0,0,0);
	if (!bStatic)
		cBulletCollisionShape.calculateLocalInertia(mass, localInertia);
	
	BodyMotionState* bodyMotionState = new BodyMotionState(&cBody);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, bodyMotionState, &cBulletCollisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	// Set data
	m_pBulletBody = body;
	
	// Save the pointer to this PL physics body
 	m_pBulletBody->setUserPointer(&cBody);

	// Get the Bullet physics world
	btDynamicsWorld *pBulletWorld = static_cast<World&>(GetBody()->GetWorld()).GetBulletWorld();
	if (pBulletWorld) {
		
		pBulletWorld->addRigidBody(body);
		// Turn auto freeze on by default
		SetBulletBodyFreezeState(true);
	}
}

/**
*  @brief
*    Sets the Bullet physics body freeze state
*/
void BodyImpl::SetBulletBodyFreezeState(bool bFreeze)
{
	// initial zero inertia
	btVector3 localInertia(0,0,0);
	float mass = bFreeze ? 0.0f : m_fMass;
	// Is the mass not null? (null = static body)
	if (mass) {
		// Now we calculate and set the moment of inertia for this body. Note that a correct
		// moment of inertia is CRUCIAL for the CORRECT PHYSICAL BEHAVIOUR of a body, so we
		// use an special equation for calculating it.			
		btCollisionShape *pCollistionShape = m_pBulletBody->getCollisionShape();
		pCollistionShape->calculateLocalInertia(mass, localInertia);
	}
	// Finally, set the mass properties
	m_pBulletBody->setMassProps(mass, localInertia);
//  	m_pBulletBody->setActivationState(bFreeze ? DISABLE_SIMULATION : ACTIVE_TAG);
//  	if (!bFreeze)
//  		m_pBulletBody->activate(true);
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::BodyImpl functions          ]
//[-------------------------------------------------------]
PLPhysics::Body *BodyImpl::GetBody() const
{
	return static_cast<PLPhysics::Body*>(m_pBulletBody->getUserPointer());
}

bool BodyImpl::IsActive() const
{
	return m_bActive;
}

void BodyImpl::SetActive(bool bActive)
{
	// State change?
	if (m_bActive != bActive) {
		m_bActive = bActive;
		SetFrozen(!bActive);
		//m_pBulletBody->activate(true);
	}
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

	// Is there a Bullet physics body?
	if (m_pBulletBody) {
		// initial zero inertia
		btVector3 localInertia(0,0,0);
		// Is the mass not null? (null = static body)
		if (fMass) {
			// Now we calculate and set the moment of inertia for this body. Note that a correct
			// moment of inertia is CRUCIAL for the CORRECT PHYSICAL BEHAVIOUR of a body, so we
			// use an special equation for calculating it.			
			btCollisionShape *pCollistionShape = m_pBulletBody->getCollisionShape();
			pCollistionShape->calculateLocalInertia(m_fMass, localInertia);
		}
		// Finally, set the mass properties
		m_pBulletBody->setMassProps(m_fMass, localInertia);
	}
}

void BodyImpl::GetCenterOfMass(Vector3 &vPosition) const
{
	vPosition = Helper::btVector3ToPLVector3(m_pBulletBody->getCenterOfMassPosition());
}

void BodyImpl::SetCenterOfMass(const Vector3 &vPosition)
{
	btTransform transform(m_pBulletBody->getCenterOfMassTransform());
	btVector3 vPos = Helper::PLVector3TobtVector3(vPosition);
	transform.setOrigin(vPos);
	
	m_pBulletBody->setCenterOfMassTransform(transform);
}

void BodyImpl::GetPosition(Vector3 &vPosition) const
{
	vPosition = m_vPosition;
}

void BodyImpl::SetPosition(const Vector3 &vPosition)
{
	m_vPosition = vPosition;
	
	btTransform transform(m_pBulletBody->getWorldTransform());
	transform.setOrigin(Helper::PLVector3TobtVector3(vPosition));
	
	m_pBulletBody->setWorldTransform(transform);
}

void BodyImpl::GetRotation(Quaternion &qRotation) const
{
	qRotation = m_qRotation;
}

void BodyImpl::SetRotation(const Quaternion &qRotation)
{
	m_qRotation = qRotation;
	
	btTransform transform(m_pBulletBody->getWorldTransform());
	transform.setRotation(Helper::PLQuaternionTobtQuaternion(qRotation));
	
	m_pBulletBody->setWorldTransform(transform);
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
	
	btTransform transform(m_pBulletBody->getWorldTransform());
	transform.setRotation(Helper::PLQuaternionTobtQuaternion(m_qRotation));
	transform.setOrigin(Helper::PLVector3TobtVector3(m_vPosition));
	
	m_pBulletBody->setWorldTransform(transform);
}

void BodyImpl::GetAABoundingBox(AABoundingBox &cAABoundingBox) const
{
	btVector3 vMinAA, vMaxAA;
	m_pBulletBody->getAabb(vMinAA, vMaxAA);
	cAABoundingBox.vMin = Helper::btVector3ToPLVector3(vMinAA);
	cAABoundingBox.vMax = Helper::btVector3ToPLVector3(vMaxAA);
}

void BodyImpl::GetLinearVelocity(Vector3 &vVelocity) const
{
	vVelocity = m_vLinearVelocity;
}

void BodyImpl::SetLinearVelocity(const Vector3 &vVelocity)
{
	m_vLinearVelocity = vVelocity;
	btVector3 linearVelocity(Helper::PLVector3TobtVector3(vVelocity));
	m_pBulletBody->setLinearVelocity(linearVelocity);
}

void BodyImpl::GetAngularVelocity(Vector3 &vVelocity) const
{
	vVelocity = m_vAngularVelocity;
}

void BodyImpl::SetAngularVelocity(const Vector3 &vVelocity)
{
	m_vAngularVelocity = vVelocity;
	
	btVector3 angularVelocity(Helper::PLVector3TobtVector3(vVelocity));
	m_pBulletBody->setLinearVelocity(angularVelocity);
}

void BodyImpl::GetForce(Vector3 &vForce) const
{
	vForce = m_vForce;
}

void BodyImpl::AddForce(const Vector3 &vForce)
{
	m_vForce += vForce;
	
	PLPhysics::Body *pBody = GetBody();
	if (pBody && !m_bChangedByUser) {
		((World&)pBody->GetWorld()).m_lstChangedByUser.Add(pBody);
		m_bChangedByUser = true;
	}
}

void BodyImpl::SetForce(const Vector3 &vForce)
{
	m_vForce = vForce;
	
	PLPhysics::Body *pBody = GetBody();
	if (pBody && !m_bChangedByUser) {
		((World&)pBody->GetWorld()).m_lstChangedByUser.Add(pBody);
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
	
	m_pBulletBody->applyTorque(Helper::PLVector3TobtVector3(vTorque));
	PLPhysics::Body *pBody = GetBody();
	if (pBody && !m_bChangedByUser) {
		((World&)pBody->GetWorld()).m_lstChangedByUser.Add(pBody);
		m_bChangedByUser = true;
	}
}

void BodyImpl::SetTorque(const Vector3 &vTorque)
{
	m_vTorque = vTorque;
	
	m_pBulletBody->applyTorque(Helper::PLVector3TobtVector3(vTorque));
	
	PLPhysics::Body *pBody = GetBody();
	if (pBody && !m_bChangedByUser) {
		((World&)pBody->GetWorld()).m_lstChangedByUser.Add(pBody);
		m_bChangedByUser = true;
	}
}

bool BodyImpl::IsAutoFreeze() const
{
	return m_bAutoFreeze;
}

void BodyImpl::SetAutoFreeze(bool bAutoFreeze)
{
	if (m_bAutoFreeze != bAutoFreeze) {
		m_bAutoFreeze = bAutoFreeze;
		// [TODO] implement me
		//NewtonBodySetAutoSleep(m_pNewtonBody, bAutoFreeze);
	}
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
	if (m_bFrozen != bFrozen) {
		m_bFrozen = bFrozen;
		SetBulletBodyFreezeState(bFrozen);
	}
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
	if(m_pBulletBody)
	{
		m_pBulletBody->setFlags(m_bUseGravity ? 0 : BT_DISABLE_WORLD_GRAVITY);
	}
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
	m_pBulletBody(nullptr),
	m_bActive(true),
	m_fCollisionVolume(0.0f),
	m_fMass(0.0f),
	m_bFrozen(true),
	m_bAutoFreeze(true),
	m_fFreezeLinearVelocity(0.01f),
	m_fFreezeAngularVelocity(0.01f),
	m_nFreezeSteps(10),
	m_nCurrentFreezeSteps(0),
	m_bCollisionActive(true),
	m_bUseGravity(true),
	m_nCollisionGroup(0),
	m_nChangedByUserFlags(0),
	m_nChangedByPhysicsFlags(0),
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

	// Is there a Newton physics body?
	if (m_pBulletBody) {
		// Get the physics world
		World &cWorld = static_cast<World&>(GetBody()->GetWorld());

		// Deactivate the physics simulation if required
		bool bSimulationActive = false;
		PLPhysics::Body *pBody = GetBody();
		if (pBody) {
			bSimulationActive = cWorld.IsSimulationActive();
			if (bSimulationActive)
				cWorld.SetSimulationActive(false);
		}

		// Destroy the Bullet physics body
		btDynamicsWorld* btWorld = cWorld.GetBulletWorld();
		
		btWorld->removeRigidBody(m_pBulletBody);
		
		if (m_pBulletBody->getMotionState())
			delete m_pBulletBody->getMotionState();

		if (m_pBulletBody->getCollisionShape())
			delete m_pBulletBody->getCollisionShape();

		delete m_pBulletBody;

		// Reactivate the physics simulation if required
		if (bSimulationActive)
			cWorld.SetSimulationActive(bSimulationActive);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
