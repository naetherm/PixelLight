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
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/BodyImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the Newton physics body
*/
Newton::NewtonBody *BodyImpl::GetNewtonBody() const
{
	return m_pNewtonBody;
}

/**
*  @brief
*    Flushes the body
*/
void BodyImpl::Flush()
{
	// Flush required?
	if (m_nChangedByUserFlags) {
		// Get the physics world
		PLPhysics::World &cWorld = GetBody()->GetWorld();

		// Deactivate the physics simulation if required
		const bool bSimulationActive = cWorld.IsSimulationActive();
		if (bSimulationActive)
			cWorld.SetSimulationActive(false);

		// Set the transform matrix of the Newton physics body
		if ((m_nChangedByUserFlags & Position) &&  m_nChangedByUserFlags & Rotation) {
			// Construct the 4x4 matrix
			float fMatrix[16];
			Matrix3x3 mRot;
			m_qRotation.ToRotationMatrix(mRot);
			fMatrix[0] = mRot.xx; fMatrix[4] = mRot.xy; fMatrix[ 8] = mRot.xz; fMatrix[12] = m_vPosition.x;
			fMatrix[1] = mRot.yx; fMatrix[5] = mRot.yy; fMatrix[ 9] = mRot.yz; fMatrix[13] = m_vPosition.y;
			fMatrix[2] = mRot.zx; fMatrix[6] = mRot.zy; fMatrix[10] = mRot.zz; fMatrix[14] = m_vPosition.z;
			fMatrix[3] = 0.0f;    fMatrix[7] = 0.0f;    fMatrix[11] = 0.0f;    fMatrix[15] = 1.0f;

			// Set the new physics body matrix
			NewtonBodySetMatrix(m_pNewtonBody, fMatrix);

			// Restore the previous freeze state, it looks like NewtonBodySetMatrix() not also wakes up bodies -
			// this wasn't the case in previous Newton Game Dynamics versions
			SetNewtonBodyFreezeState(m_bFrozen);
		} else {
			// Only position
			if (m_nChangedByUserFlags & Position) {
				// Construct the 4x4 matrix
				float fMatrix[16];
				NewtonBodyGetMatrix(m_pNewtonBody, fMatrix);
				fMatrix[12] = m_vPosition.x;
				fMatrix[13] = m_vPosition.y;
				fMatrix[14] = m_vPosition.z;

				// Set the new physics body matrix
				NewtonBodySetMatrix(m_pNewtonBody, fMatrix);

				// Restore the previous freeze state, it looks like NewtonBodySetMatrix() not also wakes up bodies -
				// this wasn't the case in previous Newton Game Dynamics versions
				SetNewtonBodyFreezeState(m_bFrozen);
			} else {
				// Only rotation
				if (m_nChangedByUserFlags & Rotation) {
					// Construct the 4x4 matrix
					float fMatrix[16];
					Matrix3x3 mRot;
					m_qRotation.ToRotationMatrix(mRot);
					NewtonBodyGetMatrix(m_pNewtonBody, fMatrix);
					fMatrix[0] = mRot.xx; fMatrix[4] = mRot.xy; fMatrix[ 8] = mRot.xz;
					fMatrix[1] = mRot.yx; fMatrix[5] = mRot.yy; fMatrix[ 9] = mRot.yz;
					fMatrix[2] = mRot.zx; fMatrix[6] = mRot.zy; fMatrix[10] = mRot.zz;

					// Set the new physics body matrix
					NewtonBodySetMatrix(m_pNewtonBody, fMatrix);

					// Restore the previous freeze state, it looks like NewtonBodySetMatrix() not also wakes up bodies -
					// this wasn't the case in previous Newton Game Dynamics versions
					SetNewtonBodyFreezeState(m_bFrozen);
				}
			}
		}

		// Set the linear and angular velocity of the Newton physics body
		if (m_nChangedByUserFlags & LinearVelocity)
			NewtonBodySetVelocity(m_pNewtonBody, m_vLinearVelocity);
		if (m_nChangedByUserFlags & AngularVelocity)
			NewtonBodySetOmega(m_pNewtonBody, m_vAngularVelocity);

		// Set force and torque of the Newton physics body
		if (m_nChangedByUserFlags & Force)
			m_vNewtonForce = m_vForce;
		if (m_nChangedByUserFlags & Torque)
			m_vNewtonTorque = m_vTorque;

		// Freeze/unfreeze
		if (m_nChangedByUserFlags & Freeze)
			SetNewtonBodyFreezeState(true);
		if (m_nChangedByUserFlags & Unfreeze)
			SetNewtonBodyFreezeState(false);

		// Reactivate the physics simulation if required
		if (bSimulationActive)
			cWorld.SetSimulationActive(bSimulationActive);

		// Reset the changed by user flags
		m_nChangedByUserFlags = 0;

		// And DON'T forget to remove this one from the 'I need an update' list of the world!
		PLPhysics::Body *pBody = GetBody();
		if (pBody)
			static_cast<World&>(cWorld).m_lstChangedByUser.Remove(pBody);
	}
}

/**
*  @brief
*    Initializes the Newton physics body
*/
void BodyImpl::InitializeNewtonBody(PLPhysics::Body &cBody, Newton::NewtonBody &cNewtonBody, float fCollisionVolume)
{
	// Set data
	m_pNewtonBody      = &cNewtonBody;
	m_fCollisionVolume = fCollisionVolume;

	// Get the Newton physics world
	Newton::NewtonWorld *pNewtonWorld = NewtonBodyGetWorld(m_pNewtonBody);
	if (pNewtonWorld) {
		// Save the pointer to this PL physics body
		NewtonBodySetUserData(m_pNewtonBody, &cBody);

		// Set a destructor for this rigid body
		NewtonBodySetDestructorCallback(m_pNewtonBody, World::PhysicsBodyDestructor);

		// Set the force and torque callback function
		NewtonBodySetForceAndTorqueCallback(m_pNewtonBody, World::PhysicsApplyGravityForce);

		// Set the transform callback function
		NewtonBodySetTransformCallback(m_pNewtonBody, World::PhysicsSetTransform);

		// Turn auto freeze on by default
		SetNewtonBodyFreezeState(true);
		NewtonBodySetAutoSleep(m_pNewtonBody, 1);
	}
}

/**
*  @brief
*    Sets the Newton physics body freeze state
*/
void BodyImpl::SetNewtonBodyFreezeState(bool bFreeze)
{
	NewtonBodySetFreezeState(m_pNewtonBody, bFreeze || !m_bActive);
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::BodyImpl functions          ]
//[-------------------------------------------------------]
PLPhysics::Body *BodyImpl::GetBody() const
{
	return static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(m_pNewtonBody));
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

	// Is there a Newton physics body?
	if (m_pNewtonBody) {
		// Is the mass not null? (null = static body)
		if (fMass) {
			// Now we calculate and set the moment of inertia for this body. Note that a correct
			// moment of inertia is CRUCIAL for the CORRECT PHYSICAL BEHAVIOUR of a body, so we
			// use an special equation for calculating it.
			float fInertia[3], fCentre[3];
			NewtonConvexCollisionCalculateInertialMatrix(NewtonBodyGetCollision(m_pNewtonBody), fInertia, fCentre);

			// Finally, set the mass matrix
			NewtonBodySetMassMatrix(m_pNewtonBody, fMass, fMass*fInertia[0], fMass*fInertia[1], fMass*fInertia[2]);
		} else {
			// Set the bodies mass and moment of inertia to zero
			NewtonBodySetMassMatrix(m_pNewtonBody, 0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}

void BodyImpl::GetCenterOfMass(Vector3 &vPosition) const
{
	NewtonBodyGetCentreOfMass(m_pNewtonBody, vPosition);
}

void BodyImpl::SetCenterOfMass(const Vector3 &vPosition)
{
	NewtonBodySetCentreOfMass(m_pNewtonBody, vPosition);
}

void BodyImpl::GetPosition(Vector3 &vPosition) const
{
	vPosition = m_vPosition;
}

void BodyImpl::SetPosition(const Vector3 &vPosition)
{
	m_vPosition = vPosition;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Position;
}

void BodyImpl::GetRotation(Quaternion &qRotation) const
{
	qRotation = m_qRotation;
}

void BodyImpl::SetRotation(const Quaternion &qRotation)
{
	m_qRotation = qRotation;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Rotation;
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
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Position;
	m_nChangedByUserFlags |= Rotation;
}

void BodyImpl::GetAABoundingBox(AABoundingBox &cAABoundingBox) const
{
	dFloat fMin[3], fMax[3];
	NewtonBodyGetAABB(m_pNewtonBody, fMin, fMax);
	cAABoundingBox.vMin.SetXYZ(fMin[0], fMin[1], fMin[2]);
	cAABoundingBox.vMax.SetXYZ(fMax[0], fMax[1], fMax[2]);
}

void BodyImpl::GetLinearVelocity(Vector3 &vVelocity) const
{
	vVelocity = m_vLinearVelocity;
}

void BodyImpl::SetLinearVelocity(const Vector3 &vVelocity)
{
	m_vLinearVelocity = vVelocity;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= LinearVelocity;
}

void BodyImpl::GetAngularVelocity(Vector3 &vVelocity) const
{
	vVelocity = m_vAngularVelocity;
}

void BodyImpl::SetAngularVelocity(const Vector3 &vVelocity)
{
	m_vAngularVelocity = vVelocity;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= AngularVelocity;
}

void BodyImpl::GetForce(Vector3 &vForce) const
{
	vForce = m_vForce;
}

void BodyImpl::AddForce(const Vector3 &vForce)
{
	m_vForce += vForce;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Force;
}

void BodyImpl::SetForce(const Vector3 &vForce)
{
	m_vForce = vForce;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Force;
}

void BodyImpl::GetTorque(Vector3 &vTorque) const
{
	vTorque = m_vTorque;
}

void BodyImpl::AddTorque(const Vector3 &vTorque)
{
	m_vTorque += vTorque;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Torque;
}

void BodyImpl::SetTorque(const Vector3 &vTorque)
{
	m_vTorque = vTorque;
	if (!m_nChangedByUserFlags)
		static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
	m_nChangedByUserFlags |= Torque;
}

bool BodyImpl::IsAutoFreeze() const
{
	return m_bAutoFreeze;
}

void BodyImpl::SetAutoFreeze(bool bAutoFreeze)
{
	if (m_bAutoFreeze != bAutoFreeze) {
		m_bAutoFreeze = bAutoFreeze;
		NewtonBodySetAutoSleep(m_pNewtonBody, bAutoFreeze);
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
		if (!m_nChangedByUserFlags)
			static_cast<World&>(GetBody()->GetWorld()).m_lstChangedByUser.Add(GetBody());
		m_nChangedByUserFlags |= bFrozen ? Freeze : Unfreeze;
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
	m_pNewtonBody(nullptr),
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
	m_nChangedByPhysicsFlags(0)
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
	if (m_pNewtonBody) {
		// Get the physics world
		PLPhysics::World &cWorld = GetBody()->GetWorld();

		// Deactivate the physics simulation if required
		bool bSimulationActive = false;
		PLPhysics::Body *pBody = GetBody();
		if (pBody) {
			bSimulationActive = cWorld.IsSimulationActive();
			if (bSimulationActive)
				cWorld.SetSimulationActive(false);

			// Ensure the body is NOT within an update list of the world
			if (m_nChangedByUserFlags) {
				static_cast<World&>(cWorld).m_lstChangedByUser.Remove(pBody);
				m_nChangedByUserFlags = 0;
			}
			if (m_nChangedByPhysicsFlags) {
				static_cast<World&>(cWorld).m_lstChangedByPhysics.Remove(pBody);
				m_nChangedByPhysicsFlags = 0;
			}
		}

		// Reset the pointer to the PL physics body
		NewtonBodySetUserData(m_pNewtonBody, nullptr);

		// Destroy the Newton physics body
		NewtonDestroyBody(NewtonBodyGetWorld(m_pNewtonBody), m_pNewtonBody);

		// Reactivate the physics simulation if required
		if (bSimulationActive)
			cWorld.SetSimulationActive(bSimulationActive);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
