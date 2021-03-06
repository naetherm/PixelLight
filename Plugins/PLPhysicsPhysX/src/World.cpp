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
#include <PLCore/Tools/Timing.h>
#include <PLCore/Core/MemoryManager.h>
#include <PLCore/Log/Log.h>
#include <PLPhysics/ContactInformation.h>
#include "PLPhysicsPhysX/BodyImpl.h"
#include "PLPhysicsPhysX/BodyBox.h"
#include "PLPhysicsPhysX/BodySphere.h"
#include "PLPhysicsPhysX/BodyMesh.h"
#include "PLPhysicsPhysX/JointImpl.h"
#include "PLPhysicsPhysX/JointBall.h"
#include "PLPhysicsPhysX/JointSlider.h"
#include "PLPhysicsPhysX/JointHinge.h"
#include "PLPhysicsPhysX/JointUniversal.h"
#include "PLPhysicsPhysX/_ErrorStream.h"
#include "PLPhysicsPhysX/World.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsPhysX {


//[-------------------------------------------------------]
//[ Global static variables                               ]
//[-------------------------------------------------------]
static ErrorStream g_ErrorStream;


//[-------------------------------------------------------]
//[ Private static variables                              ]
//[-------------------------------------------------------]
NxPhysicsSDK *World::m_pPhysXPhysicsSDK      = nullptr;
uint32        World::m_nPhysXInstanceCounter = 0;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(World, "PLPhysicsPhysX", PLPhysics::World, "PhysX physics world base class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(World)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Memory allocation/deallocation class
*/
class PhysXAllocator : public NxUserAllocator {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		void *malloc(NxU32 nSize)
		{
			return MemoryManager::Allocator(MemoryManager::NewArray, nSize);
		}

		void *mallocDEBUG(NxU32 nSize, const char *pszFilename, int nLine)
		{
			return MemoryManager::Allocator(MemoryManager::NewArray, nSize, pszFilename, nLine);
		}

		void *realloc(void *pAddress, NxU32 nSize)
		{
			return MemoryManager::Reallocator(pAddress, nSize);
		}

		void free(void *pAddress)
		{
			if (pAddress)
				MemoryManager::Deallocator(MemoryManager::DeleteArray, pAddress);
		}


} PhysXAllocatorInstance;


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
	m_bBuoyancyActive(false),
	m_fBuoyancyPlaneY(0.0f),
	m_pPhysXScene(nullptr)
{
	// Initialize PhysicsSDK if this is the first instance
	if (!m_pPhysXPhysicsSDK) {
		m_pPhysXPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, &PhysXAllocatorInstance, &g_ErrorStream);
		if (!m_pPhysXPhysicsSDK)
			return; // Error!
		PL_LOG(Info, String::Format("Initialized PhysX physics SDK (version: %d)", NX_PHYSICS_SDK_VERSION))
	}

	// Increase the PhysX instance counter
	m_nPhysXInstanceCounter++;

	// Create the PhysX physics scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.flags |= NX_SF_ENABLE_ACTIVETRANSFORMS; // We need this in order to be able to use 'getActiveTransforms()'
	m_pPhysXScene = m_pPhysXPhysicsSDK->createScene(sceneDesc);
	PL_LOG(Info, "Created PhysX scene")

	// Save the pointer to this PL physics world
	m_pPhysXScene->userData = this;

	// Set user contacts callback object ('onContactConstraint()')
	m_pPhysXScene->setUserContactModify(this);

	// Setup standard material
	NxMaterial *defaultMaterial = m_pPhysXScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	// Set simulation quality
	SetSimulationQuality(m_fSimulationQuality);

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
	// Destroy the PhysX physics scene
	if (m_pPhysXPhysicsSDK && m_pPhysXScene) {
		m_pPhysXPhysicsSDK->releaseScene(*m_pPhysXScene);
		PL_LOG(Info, "Released PhysX scene")
	}

	// Decrease the PhysX instance counter
	m_nPhysXInstanceCounter--;

	// De-initialize PhysX if there are no further PhysX instances
	if (!m_nPhysXInstanceCounter && m_pPhysXPhysicsSDK) {
		m_pPhysXPhysicsSDK->release();
		m_pPhysXPhysicsSDK = nullptr;
		PL_LOG(Info, String::Format("Released PhysX physics SDK (version: %d)", NX_PHYSICS_SDK_VERSION))
	}
}

/**
*  @brief
*    Returns the PhysX physics SDK
*/
NxPhysicsSDK *World::GetPhysXPhysicsSDK() const
{
	return m_pPhysXPhysicsSDK;
}

/**
*  @brief
*    Returns the PhysX physics scene
*/
NxScene *World::GetPhysXScene() const
{
	return m_pPhysXScene;
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
	return new BodyBox(*this, vDimension, bStatic);
}

PLPhysics::Body *World::CreateBodySphere(float fRadius, bool bStatic)
{
	return new BodySphere(*this, fRadius, bStatic);
}

PLPhysics::Body *World::CreateBodyEllipsoid(const Vector3 &vRadius, bool bStatic)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Body *World::CreateBodyConvexHull(PLMesh::MeshManager &cMeshManager, const String &sMesh, const Vector3 &vMeshScale, bool bStatic)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Body *World::CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const String &sMesh, const Vector3 &vMeshScale, bool bOptimize)
{
	return new BodyMesh(*this, sMesh, vMeshScale, bOptimize);
}

PLPhysics::Body *World::CreateBodyTerrain(uint32 nWidth, uint32 nHeight, const float fTerrain[],
										  const Vector3 &vBoxMin, const Vector3 &vBoxMax, const Vector3 &vScale)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Body *World::CreateBodyCylinder(float fRadius, float fHeight, bool bStatic)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Body *World::CreateBodyCone(float fRadius, float fHeight, bool bStatic)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Body *World::CreateBodyCapsule(float fRadius, float fHeight, bool bStatic)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Body *World::CreateBodyChamferCylinder(float fRadius, float fHeight, bool bStatic)
{
	// Not implemented
	return nullptr;
}


//[-------------------------------------------------------]
//[ Physics joint creation                                ]
//[-------------------------------------------------------]
PLPhysics::Joint *World::CreateJointBall(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || !pParentBody && !pChildBody)
		return nullptr; // Error!

	// Create the physics joint
	return new JointBall(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointSlider(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || !pParentBody && !pChildBody)
		return nullptr; // Error!

	// Create the physics joint
	return new JointSlider(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointHinge(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Check parameters
	if (pParentBody == pChildBody || !pParentBody && !pChildBody)
		return nullptr; // Error!

	// Create the physics joint
	return new JointHinge(*this, pParentBody, pChildBody, vPivotPoint, vPinDir);
}

PLPhysics::Joint *World::CreateJointUniversal(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint,
											  const Vector3 &vPinDir1, const Vector3 &vPinDir2)
{
	// Check parameters
	if (pParentBody == pChildBody || !pParentBody && !pChildBody)
		return nullptr; // Error!

	// Create the physics joint
	return new JointUniversal(*this, pParentBody, pChildBody, vPivotPoint, vPinDir1, vPinDir2);
}

PLPhysics::Joint *World::CreateJointCorkscrew(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const Vector3 &vPivotPoint, const Vector3 &vPinDir)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Joint *World::CreateJointUpVector(PLPhysics::Body &cParentBody, const Vector3 &vPinDir)
{
	// Not implemented
	return nullptr;
}


//[-------------------------------------------------------]
//[ Physics sensor creation                               ]
//[-------------------------------------------------------]
PLPhysics::Sensor *World::CreateSensorRaycast(const Vector3 &vStart, const Vector3 &vEnd, uint32 nFlags)
{
	// Not implemented
	return nullptr;
}

PLPhysics::Sensor *World::CreateSensorAABoundingBox(const Vector3 &vMin, const Vector3 &vMax, uint32 nFlags)
{
	// [TODO] Not implemented
	return nullptr;
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
	if (m_pPhysXScene)
		m_pPhysXScene->setTiming(1.0f/fFrameRate, 8, NX_TIMESTEP_FIXED);

	// Done
	return true;
}

void World::GetGravity(Vector3 &vGravity) const
{
	if (m_pPhysXScene) {
		NxVec3 vV;
		m_pPhysXScene->getGravity(vV);
		vGravity.x = vV.x;
		vGravity.y = vV.y;
		vGravity.z = vV.z;
	}
}

void World::SetGravity(const Vector3 &vGravity)
{
	if (m_pPhysXScene)
		m_pPhysXScene->setGravity(NxVec3(vGravity.x, vGravity.y, vGravity.z));
}

bool World::GetGroupCollision(uint8 nGroup1, uint8 nGroup2) const
{
	return (m_pPhysXScene && nGroup1 < 32 && nGroup2 < 32 && m_pPhysXScene->getGroupCollisionFlag(nGroup1, nGroup2));
}

void World::SetGroupCollision(uint8 nGroup1, uint8 nGroup2, bool bActive)
{
	if (m_pPhysXScene && nGroup1 < 32 && nGroup2 < 32)
		m_pPhysXScene->setGroupCollisionFlag(nGroup1, nGroup2, bActive);
}

uint8 World::GetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2) const
{
	// Check parameters
	if (!m_pPhysXScene)
		return 0; // Error!

	// Get the PhysX actors
	NxActor *pPhysXActor1 = ((BodyImpl&)cBody1.GetBodyImpl()).GetPhysXActor();
	NxActor *pPhysXActor2 = ((BodyImpl&)cBody2.GetBodyImpl()).GetPhysXActor();

	// Get the flags
	uint8 nFlags = 0;
	const NxU32 nAPIFlags = m_pPhysXScene->getActorPairFlags(*pPhysXActor1, *pPhysXActor2);
	if (nAPIFlags & NX_IGNORE_PAIR)
		nFlags |= Ignore;

	// Return the flags
	return nFlags;
}

void World::SetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2, uint8 nFlags)
{
	// Check parameters
	if (m_pPhysXScene) {
		// Get the PhysX actors
		NxActor *pPhysXActor1 = ((BodyImpl&)cBody1.GetBodyImpl()).GetPhysXActor();
		NxActor *pPhysXActor2 = ((BodyImpl&)cBody2.GetBodyImpl()).GetPhysXActor();

		// If flags are set to 0, just clear the flag
		if (nFlags) {
			// Set the flags
			NxU32 nAPIFlags = 0;
			if (nFlags & Ignore)
				nAPIFlags |= NX_IGNORE_PAIR;
			if (nAPIFlags) {
				if (!m_pPhysXScene->getActorPairFlags(*pPhysXActor1, *pPhysXActor2)) {
					// Add the bodies to each other
					((BodyImpl&)cBody1.GetBodyImpl()).m_lstPartnerBodies.Add(&((PLPhysics::Body&)cBody2));
					((BodyImpl&)cBody2.GetBodyImpl()).m_lstPartnerBodies.Add(&((PLPhysics::Body&)cBody1));
				}
				m_pPhysXScene->setActorPairFlags(*pPhysXActor1, *pPhysXActor2, nAPIFlags);
			}
		} else {
			// Clear the flag
			m_pPhysXScene->setActorPairFlags(*pPhysXActor1, *pPhysXActor2, 0);

			// Remove the bodies from each other
			((BodyImpl&)cBody1.GetBodyImpl()).m_lstPartnerBodies.Remove(&((PLPhysics::Body&)cBody2));
			((BodyImpl&)cBody2.GetBodyImpl()).m_lstPartnerBodies.Remove(&((PLPhysics::Body&)cBody1));
		}
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
	// Is there a physics scene and is the simulation active?
	if (m_pPhysXScene && m_bSimulationActive) {
		// Loop through the list of physics bodies changed by the user
		if (!m_lstChangedByUser.IsEmpty()) {
			{
				Iterator<PLPhysics::Body*> cIterator = m_lstChangedByUser.GetIterator();
				while (cIterator.HasNext()) {
					PLPhysics::Body *pBody = cIterator.Next();
					BodyImpl &cBodyImpl = (BodyImpl&)pBody->GetBodyImpl();
					cBodyImpl.m_bChangedByUser = false;
					Vector3 &vForce  = cBodyImpl.m_vForce;
					Vector3 &vTorque = cBodyImpl.m_vTorque;

					// Add force and torque to the PhysX physics body
					NxVec3 vV(vForce.x, vForce.y, vForce.z);
					cBodyImpl.GetPhysXActor()->addForce(vV);
					vV.x = vTorque.x;
					vV.y = vTorque.y;
					vV.z = vTorque.z;
					cBodyImpl.GetPhysXActor()->addTorque(vV);

					// Reset the force and torque of the PL physics body
					vForce  = Vector3::Zero;
					vTorque = Vector3::Zero;
				}
			}
			m_lstChangedByUser.FreeElements();
		}

		// Perform simulation update
		m_pPhysXScene->simulate(Timing::GetInstance()->GetTimeDifference()*m_fSimulationSpeed);
		m_pPhysXScene->flushStream();
		m_pPhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

		// Get a list of the NxActors whose transforms have been updated during the previous simulation step
		NxU32 nNumOfActors = 0;
		NxActiveTransform *pActors = m_pPhysXScene->getActiveTransforms(nNumOfActors);
		if (nNumOfActors && pActors) {
			// Loop through all actors within the list
			for (uint32 i=0; i<nNumOfActors; i++) {
				// Get the physics body
				PLPhysics::Body *pBody = (PLPhysics::Body*)pActors[i].userData;
				if (pBody) {
					// Emit event
					pBody->EventTransform();
				}
			}

			// Please note that we don't need to delete the received list
		}
	}
}

bool World::IsAlwaysStatic() const
{
	return true;
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
//[ Public virtual NxUserContactModify functions          ]
//[-------------------------------------------------------]
bool World::onContactConstraint(NxU32 &nChangeFlags, const NxShape *pShape0, const NxShape *pShape1,
								const NxU32 nFeatureIndex0, const NxU32 nFeatureIndex1, NxContactCallbackData &sData)
{
	// Get the first body
	if (pShape0) {
		PLPhysics::Body *pBody0 = ((PLPhysics::Body*)pShape0->getActor().userData);
		if (pBody0) {
			// Get the second body
			if (pShape1) {
				PLPhysics::Body *pBody1 = ((PLPhysics::Body*)pShape1->getActor().userData);
				if (pBody1) {
					// Create the 'ContactInformation'-instance on the runtime-stack
					PLPhysics::ContactInformation cContactInformation(*pBody0, *pBody1);

					// Emit signal
					SignalContact(cContactInformation);

					// Use this contact?
					return !cContactInformation.IsContactIgnored();
				}
			}
		}
	}

	// Do not use this contact
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsPhysX
