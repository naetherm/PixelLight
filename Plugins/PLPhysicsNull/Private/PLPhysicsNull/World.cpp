/*********************************************************\
 *  File: World.cpp                                      *
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
#include <limits.h>
#include "PLPhysicsNull/BodyImpl.h"
#include "PLPhysicsNull/BodyBox.h"
#include "PLPhysicsNull/BodySphere.h"
#include "PLPhysicsNull/BodyEllipsoid.h"
#include "PLPhysicsNull/BodyConvexHull.h"
#include "PLPhysicsNull/BodyMesh.h"
#include "PLPhysicsNull/BodyTerrain.h"
#include "PLPhysicsNull/BodyCylinder.h"
#include "PLPhysicsNull/BodyCone.h"
#include "PLPhysicsNull/BodyCapsule.h"
#include "PLPhysicsNull/BodyChamferCylinder.h"
#include "PLPhysicsNull/JointImpl.h"
#include "PLPhysicsNull/JointBall.h"
#include "PLPhysicsNull/JointSlider.h"
#include "PLPhysicsNull/JointHinge.h"
#include "PLPhysicsNull/JointUniversal.h"
#include "PLPhysicsNull/JointCorkscrew.h"
#include "PLPhysicsNull/JointUpVector.h"
#include "PLPhysicsNull/SensorRaycast.h"
#include "PLPhysicsNull/SensorAABoundingBox.h"
#include "PLPhysicsNull/World.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNull {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(World)


//[-------------------------------------------------------]
//[ Private MyHashFunction class implementation           ]
//[-------------------------------------------------------]
uint32 World::MyHashFunction::Hash(const BodyPair &sKey)
{
	return static_cast<uint32>(((sKey.pBody2 - sKey.pBody1) % UINT_MAX + (sKey.pBody1-sKey.pBody2) % UINT_MAX) % UINT_MAX);
}


//[-------------------------------------------------------]
//[ Private MyCompareFunction class implementation        ]
//[-------------------------------------------------------]
bool World::MyCompareFunction::AreEqual(const BodyPair &sKey1, const BodyPair &sKey2)
{
	return ((sKey1.pBody1 == sKey2.pBody1 && sKey1.pBody2 == sKey2.pBody2) ||
		    (sKey1.pBody1 == sKey2.pBody2 && sKey1.pBody2 == sKey2.pBody1));
}

bool World::MyCompareFunction::IsGreater(const BodyPair &sKey1, const BodyPair &sKey2)
{
	// Not implemented
	return false;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
World::World() :
	m_vWorldSizeMin(-10000.0f, -10000.0f, -10000.0f),
	m_vWorldSizeMax( 10000.0f,  10000.0f,  10000.0f),
	m_fFrameRate(60.0f),
	m_bSimulationActive(true),
	m_fSimulationSpeed(1.0f),
	m_fSimulationQuality(1.0f),
	m_vGravity(0.0f, -9.81f, 0.0f),
	m_bBuoyancyActive(false),
	m_fBuoyancyPlaneY(0.0f)
{
	// Initialize group collision active states
	MemoryManager::Set(m_nGroupCollision, UINT_MAX, sizeof(uint32)*32);

	// By default, collision between objects within the same group is disabled - except for the first group
	for (uint8 i=1; i<32; i++)
		SetGroupCollision(i, i, false);
}

/**
*  @brief
*    Destructor
*/
World::~World()
{
}

/**
*  @brief
*    Creates a physics body implementation
*/
PLPhysics::BodyImpl &World::CreateBodyImpl() const
{
	return *(new BodyImpl());
}

/**
*  @brief
*    Creates a physics joint implementation
*/
PLPhysics::JointImpl &World::CreateJointImpl() const
{
	return *(new JointImpl());
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::World functions             ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Physics body creation                                 ]
//[-------------------------------------------------------]
PLPhysics::Body *World::CreateBodyBox(const Vector3 &vDimension, bool bStatic)
{
	return new BodyBox(*this, vDimension);
}

PLPhysics::Body *World::CreateBodySphere(float fRadius, bool bStatic)
{
	return new BodySphere(*this, fRadius);
}

PLPhysics::Body *World::CreateBodyEllipsoid(const Vector3 &vRadius, bool bStatic)
{
	return new BodyEllipsoid(*this, vRadius);
}

PLPhysics::Body *World::CreateBodyConvexHull(PLMesh::MeshManager &cMeshManager, const String &sMesh, const Vector3 &vMeshScale, bool bStatic)
{
	return new BodyConvexHull(*this, sMesh, vMeshScale);
}

PLPhysics::Body *World::CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const String &sMesh, const Vector3 &vMeshScale, bool bOptimize)
{
	return new BodyMesh(*this, sMesh, vMeshScale, bOptimize);
}

PLPhysics::Body *World::CreateBodyTerrain(uint32 nWidth, uint32 nHeight, const float fTerrain[],
										  const Vector3 &vBoxMin, const Vector3 &vBoxMax, const Vector3 &vScale)
{
	return new BodyTerrain(*this);
}

PLPhysics::Body *World::CreateBodyCylinder(float fRadius, float fHeight, bool bStatic)
{
	return new BodyCylinder(*this, fRadius, fHeight);
}

PLPhysics::Body *World::CreateBodyCone(float fRadius, float fHeight, bool bStatic)
{
	return new BodyCone(*this, fRadius, fHeight);
}

PLPhysics::Body *World::CreateBodyCapsule(float fRadius, float fHeight, bool bStatic)
{
	return new BodyCapsule(*this, fRadius, fHeight);
}

PLPhysics::Body *World::CreateBodyChamferCylinder(float fRadius, float fHeight, bool bStatic)
{
	return new BodyChamferCylinder(*this, fRadius, fHeight);
}


//[-------------------------------------------------------]
//[ Physics joint creation                                ]
//[-------------------------------------------------------]
PLPhysics::Joint *World::CreateJointBall(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || (!pParentBody && !pChildBody))
		return nullptr; // Error!

	// Create the physics joint
	return new JointBall(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointSlider(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || (!pParentBody && !pChildBody))
		return nullptr; // Error!

	// Create the physics joint
	return new JointSlider(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointHinge(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || (!pParentBody && !pChildBody))
		return nullptr; // Error!

	// Create the physics joint
	return new JointHinge(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointUniversal(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint,
											  const Vector3 &vPinDir1, const Vector3 &vPinDir2)
{
	// Check parameters
	if (pParentBody == pChildBody || (!pParentBody && !pChildBody))
		return nullptr; // Error!

	// Create the physics joint
	return new JointUniversal(*this, pParentBody, pChildBody, vPivotPoint, vPinDir1, vPinDir2);
}

PLPhysics::Joint *World::CreateJointCorkscrew(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || (!pParentBody && !pChildBody))
		return nullptr; // Error!

	// Create the physics joint
	return new JointCorkscrew(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointUpVector(PLPhysics::Body &cParentBody, const Vector3 &vPinDir)
{
	// Create the physics joint
	return new JointUpVector(*this, cParentBody, vPinDir);
}


//[-------------------------------------------------------]
//[ Physics sensor creation                               ]
//[-------------------------------------------------------]
PLPhysics::Sensor *World::CreateSensorRaycast(const Vector3 &vStart, const Vector3 &vEnd, uint32 nFlags)
{
	return new SensorRaycast(*this, vStart, vEnd, nFlags);
}

PLPhysics::Sensor *World::CreateSensorAABoundingBox(const Vector3 &vMin, const Vector3 &vMax, uint32 nFlags)
{
	return new SensorAABoundingBox(*this, vMin, vMax, nFlags);
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
void World::GetWorldSize(Vector3 &vMin, Vector3 &vMax) const
{
	vMin = m_vWorldSizeMin;
	vMax = m_vWorldSizeMax;
}

void World::SetWorldSize(const Vector3 &vMin, const Vector3 &vMax)
{
	m_vWorldSizeMin = vMin;
	m_vWorldSizeMax = vMax;
}

bool World::IsSimulationActive() const
{
	return m_bSimulationActive;
}

void World::SetSimulationActive(bool bActive)
{
	m_bSimulationActive = bActive;
}

float World::GetSimulationSpeed() const
{
	return m_fSimulationSpeed;
}

bool World::SetSimulationSpeed(float fSpeed)
{
	if (fSpeed > 0.0f) {
		// Set the new speed
		m_fSimulationSpeed = fSpeed;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

float World::GetSimulationQuality() const
{
	return m_fSimulationQuality;
}

bool World::SetSimulationQuality(float fQuality)
{
	// Quality range check
	if (fQuality >= 0.0f && fQuality <= 1.0f) {
		m_fSimulationQuality = fQuality;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

float World::GetFrameRate() const
{
	return m_fFrameRate;
}

bool World::SetFrameRate(float fFrameRate)
{
	// Check parameter
	if (fFrameRate < 60.0f || fFrameRate > 1000.0f)
		return false; // Error!

	// Set the new frame rate
	m_fFrameRate = fFrameRate;

	// Done
	return true;
}

void World::GetGravity(Vector3 &vGravity) const
{
	vGravity = m_vGravity;
}

void World::SetGravity(const Vector3 &vGravity)
{
	m_vGravity = vGravity;
}

bool World::GetGroupCollision(uint8 nGroup1, uint8 nGroup2) const
{
	return (nGroup1 <= 31 && nGroup2 <= 31 && (m_nGroupCollision[nGroup1] & (1<<nGroup2)) != 0);
}

void World::SetGroupCollision(uint8 nGroup1, uint8 nGroup2, bool bActive)
{
	if (nGroup1 <= 31 && nGroup2 <= 31) {
		if (bActive) {
			m_nGroupCollision[nGroup1] |= 1<<nGroup2;
			m_nGroupCollision[nGroup2] |= 1<<nGroup1;
		} else {
			m_nGroupCollision[nGroup1] &= ~(1<<nGroup2);
			m_nGroupCollision[nGroup2] &= ~(1<<nGroup1);
		}
	}
}

uint8 World::GetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2) const
{
	// Get body pair
	BodyPair sBodyPair;
	sBodyPair.pBody1 = &cBody1;
	sBodyPair.pBody2 = &cBody2;

	// Get and return the flags
	return m_mapBodyPairs.Get(sBodyPair);
}

void World::SetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2, uint8 nFlags)
{
	// Get body pair
	BodyPair sBodyPair;
	sBodyPair.pBody1 = &cBody1;
	sBodyPair.pBody2 = &cBody2;

	// If flags are set to 0, just remove the map entry
	if (nFlags) {
		// Is this body pair already within the map?
		uint8 &nFlagsRef = m_mapBodyPairs.Get(sBodyPair);
		if (&nFlagsRef != &HashMap<BodyPair, uint8, MyHashFunction, MyCompareFunction>::Null)
			nFlagsRef = nFlags; // Jep, just change the value :)
		else {
			// Nope, add it right now!
			m_mapBodyPairs.Add(sBodyPair, nFlags);

			// Add the bodies to each other
			static_cast<BodyImpl&>(cBody1.GetBodyImpl()).m_lstPartnerBodies.Add(&const_cast<PLPhysics::Body&>(static_cast<const PLPhysics::Body&>(cBody2)));
			static_cast<BodyImpl&>(cBody2.GetBodyImpl()).m_lstPartnerBodies.Add(&const_cast<PLPhysics::Body&>(static_cast<const PLPhysics::Body&>(cBody1)));
		}
	} else {
		// Remove from map
		m_mapBodyPairs.Remove(sBodyPair);

		// Remove the bodies from each other
		static_cast<BodyImpl&>(cBody1.GetBodyImpl()).m_lstPartnerBodies.Remove(&const_cast<PLPhysics::Body&>(static_cast<const PLPhysics::Body&>(cBody2)));
		static_cast<BodyImpl&>(cBody2.GetBodyImpl()).m_lstPartnerBodies.Remove(&const_cast<PLPhysics::Body&>(static_cast<const PLPhysics::Body&>(cBody1)));
	}
}

bool World::IsBuoyancyActive() const
{
	return m_bBuoyancyActive;
}

void World::SetBuoyancyActive(bool bActive)
{
	m_bBuoyancyActive = bActive;
}

float World::GetBuoyancyPlaneY() const
{
	return m_fBuoyancyPlaneY;
}

void World::SetBuoyancyPlaneY(float fY)
{
	m_fBuoyancyPlaneY = fY;
}

void World::UpdateSimulation()
{
	// Is the simulation active?
	if (m_bSimulationActive) {
		// Loop through the list of physics bodies changed by the user
		if (!m_lstChangedByUser.IsEmpty()) {
			{
				Iterator<PLPhysics::Body*> cIterator = m_lstChangedByUser.GetIterator();
				while (cIterator.HasNext()) {
					PLPhysics::Body *pBody = cIterator.Next();
					BodyImpl &cBodyImpl = static_cast<BodyImpl&>(pBody->GetBodyImpl());
					cBodyImpl.m_bChangedByUser = false;

					// Reset the force and torque of the PL physics body
					cBodyImpl.m_vForce  = Vector3::Zero;
					cBodyImpl.m_vTorque = Vector3::Zero;
				}
			}
			m_lstChangedByUser.FreeElements();
		}
	}
}

bool World::IsAlwaysStatic() const
{
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
PLPhysics::Element *World::CreateElement(const String &sName)
{
	// Nothing to do here
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNull
