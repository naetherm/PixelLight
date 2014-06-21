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
#include <PLCore/Tools/Timing.h>
#include <PLCore/File/File.h>
#include <PLCore/Log/Log.h>
#include <PLMath/Plane.h>
#include <PLMath/Matrix3x3.h>
#include <PLPhysics/ContactInformation.h>
#include "PLPhysicsNewton/BodyImpl.h"
#include "PLPhysicsNewton/BodyBox.h"
#include "PLPhysicsNewton/BodySphere.h"
#include "PLPhysicsNewton/BodyEllipsoid.h"
#include "PLPhysicsNewton/BodyConvexHull.h"
#include "PLPhysicsNewton/BodyMesh.h"
#include "PLPhysicsNewton/BodyTerrain.h"
#include "PLPhysicsNewton/BodyCylinder.h"
#include "PLPhysicsNewton/BodyCone.h"
#include "PLPhysicsNewton/BodyCapsule.h"
#include "PLPhysicsNewton/BodyChamferCylinder.h"
#include "PLPhysicsNewton/JointImpl.h"
#include "PLPhysicsNewton/JointBall.h"
#include "PLPhysicsNewton/JointSlider.h"
#include "PLPhysicsNewton/JointHinge.h"
#include "PLPhysicsNewton/JointUniversal.h"
#include "PLPhysicsNewton/JointCorkscrew.h"
#include "PLPhysicsNewton/JointUpVector.h"
#include "PLPhysicsNewton/SensorRaycast.h"
#include "PLPhysicsNewton/SensorAABoundingBox.h"
#include "PLPhysicsNewton/WorldThread.h"
#include "PLPhysicsNewton/World.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace Newton;
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNewton {


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
//[ Public virtual PLCore::Base functions                 ]
//[-------------------------------------------------------]
	// [TODO] New RTTI usage
/*void World::VarChanged(const String &sVar)
{
	// Is there a physics thread?
	if (m_pWorldUpdate) {
		if (m_pWorldUpdate->IsThread()) {
			// ThreadPriorityClass
			if (sVar == "ThreadPriorityClass") {
				if (m_nThreadPriorityClass < 6)
					static_cast<WorldThread*>(m_pWorldUpdate)->SetPriorityClass(static_cast<Thread::EPriorityClass>(m_nThreadPriorityClass);
				else {
					// Change to none thread update object
					delete m_pWorldUpdate;
					m_pWorldUpdate = new WorldUpdate(*this);
				}

			// ThreadPriority
			} else if (sVar == "ThreadPriority")
				static_cast<WorldThread*>(m_pWorldUpdate)->SetPriority(static_cast<Thread::EPriority>(m_nThreadPriority));
		} else {
			// ThreadPriorityClass
			if (sVar == "ThreadPriorityClass") {
				if (m_nThreadPriorityClass < 6) {
					// Change to thread update object
					delete m_pWorldUpdate;
					m_pWorldUpdate = new WorldThread(*this);
				}
			}
		}
	} else {
		m_pWorldUpdate = (m_nThreadPriorityClass < 6) ? new WorldThread(*this) : new WorldUpdate(*this);
	}
}*/

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
World::World() :
	CacheDirectory(this),
	//ThreadPriorityClass(this),
	//ThreadPriority(this),

	// [TODO] New RTTI usage
	m_nThreadPriorityClass(6), // None
	m_nThreadPriority(3),

	m_pNewtonWorld(nullptr),
	m_pWorldUpdate(nullptr),
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
	// Set memory system functions, this is the same for all worlds
	NewtonSetMemorySystem(PhysicsAllocMemory, PhysicsFreeMemory);

	// Initialize group collision active states
	MemoryManager::Set(m_nGroupCollision, UINT_MAX, sizeof(uint32)*32);

	// By default, collision between objects within the same group is disabled - except for the first group
	for (uint8 i=1; i<32; i++)
		SetGroupCollision(i, i, false);

	// Create the Newton world
	m_pNewtonWorld = NewtonCreate();
	// [TODO] Remove this as soon as there's an up-to-date Linux version of Newton Game Dynamics available!
	#if (NEWTON_MAJOR_VERSION == 2) && (NEWTON_MINOR_VERSION >= 28)
		PL_LOG(Info, String("Created Newton Game Dynamics (version: ") + NewtonWorldGetVersion() + ") world")
	#else
		PL_LOG(Info, String("Created Newton Game Dynamics (version: ") + NewtonWorldGetVersion(m_pNewtonWorld) + ") world")
	#endif

	// Save the pointer to this PL physics world
	NewtonWorldSetUserData(m_pNewtonWorld, this);

	// Set a default world size
	SetWorldSize();

	// Get the default material ID
	const int nDefaultID = NewtonMaterialGetDefaultGroupID(m_pNewtonWorld);

	// Set default material properties (settings from Newton SDK samples)
	NewtonMaterialSetDefaultSoftness(m_pNewtonWorld, nDefaultID, nDefaultID, 0.05f);
	NewtonMaterialSetDefaultElasticity(m_pNewtonWorld, nDefaultID, nDefaultID, 0.4f);
	NewtonMaterialSetDefaultCollidable(m_pNewtonWorld, nDefaultID, nDefaultID, 1);
	NewtonMaterialSetDefaultFriction(m_pNewtonWorld, nDefaultID, nDefaultID, 1.0f, 0.5f);
	NewtonMaterialSetCollisionCallback(m_pNewtonWorld, nDefaultID, nDefaultID, nullptr, OnAABBOverlap, ContactsProcess);
//	NewtonMaterialSetSurfaceThickness(m_pNewtonWorld, nDefaultID, nDefaultID, 0.001f); // Looks not good :/

	// Set isle update event
	NewtonSetIslandUpdateEvent(m_pNewtonWorld, IslandUpdate);

	// Create the physics update object
	m_pWorldUpdate = (m_nThreadPriorityClass < 6) ? new WorldThread(*this) : new WorldUpdate(*this);

	// Set simulation quality
	SetSimulationQuality(m_fSimulationQuality);
}

/**
*  @brief
*    Destructor
*/
World::~World()
{
	// Destroy the physics update object
	if (m_pWorldUpdate)
		delete m_pWorldUpdate;

	// Clear the collision convex hull map
	{
		Iterator<NewtonCollision*> cIterator = m_mapCollisionConvexHull.GetIterator();
		while (cIterator.HasNext())
			NewtonReleaseCollision(m_pNewtonWorld, cIterator.Next());
	}
	m_mapCollisionConvexHull.Clear();

	// Clear the collision mesh map
	{
		Iterator<NewtonCollision*> cIterator = m_mapCollisionMesh.GetIterator();
		while (cIterator.HasNext())
			NewtonReleaseCollision(m_pNewtonWorld, cIterator.Next());
	}
	m_mapCollisionMesh.Clear();

	// Destroy the Newton world
	NewtonDestroy(m_pNewtonWorld);
	// [TODO] Remove this as soon as there's an up-to-date Linux version of Newton Game Dynamics available!
	#if (NEWTON_MAJOR_VERSION == 2) && (NEWTON_MINOR_VERSION >= 28)
		PL_LOG(Info, String("Destroyed Newton Game Dynamics (version: ") + NewtonWorldGetVersion() + ") world")
	#else
		PL_LOG(Info, String("Destroyed Newton Game Dynamics (version: ") + NewtonWorldGetVersion(m_pNewtonWorld) + ") world")
	#endif
}

/**
*  @brief
*    Returns the Newton physics world
*/
NewtonWorld *World::GetNewtonWorld() const
{
	return m_pNewtonWorld;
}

/**
*  @brief
*    Returns the collision convex hull map
*/
HashMap<String, NewtonCollision*> &World::GetCollisionConvexHullMap()
{
	return m_mapCollisionConvexHull;
}

/**
*  @brief
*    Returns the collision mesh map
*/
HashMap<String, NewtonCollision*> &World::GetCollisionMeshMap()
{
	return m_mapCollisionMesh;
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
//[ Public Newton callback functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Serialization callback function
*/
void World::PhysicsSerialize(void *pSerializeHandle, const void *pBuffer, int nSize)
{
	static_cast<File*>(pSerializeHandle)->Write(pBuffer, 1, static_cast<uint32>(nSize));
}

/**
*  @brief
*    De-serialization callback function
*/
void World::PhysicsDeserialize(void *pSerializeHandle, void *pBuffer, int nSize)
{
	static_cast<File*>(pSerializeHandle)->Read(pBuffer, 1, static_cast<uint32>(nSize));
}

/**
*  @brief
*    Gets the buoyancy plane callback function
*/
int World::PhysicsGetBuoyancyPlane(const int nCollisionID, void *pContext, const dFloat *pGlobalSpaceMatrix, dFloat *pGlobalSpacePlane)
{
	// Get the PL physics world
	if (!pContext)
		return 0;
	const World &cWorld = *static_cast<const World*>(pContext);

	// Get plane
	Plane cPlane;
	cPlane.ComputeND(Vector3(0.0f, cWorld.GetBuoyancyPlaneY(), 0.0f), Vector3::UnitY);
	pGlobalSpacePlane[0] = cPlane.a;
	pGlobalSpacePlane[1] = cPlane.b;
	pGlobalSpacePlane[2] = cPlane.c;
	pGlobalSpacePlane[3] = cPlane.d;

	// Done
	return 1;
}

/**
*  @brief
*    Applies gravity force to a rigid body callback function
*/
void World::PhysicsApplyGravityForce(const NewtonBody *pPhysicsBody, dFloat fTimeStep, int nThreadIndex)
{
	// Get the pointer to the PL physics body and implementation
	PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pPhysicsBody));
	if (pBody && pBody->IsActive()) {
			  BodyImpl   &cBodyImpl   = static_cast<BodyImpl&>(pBody->GetBodyImpl());
		const NewtonBody *pNewtonBody = cBodyImpl.m_pNewtonBody;

		// Get the initial force
		Vector3 vForce = cBodyImpl.m_vNewtonForce;

		// Apply gravity if requested
		if (cBodyImpl.m_bUseGravity) {
			// Get body mass
			const float fMass = pBody->GetMass();

			// Use buoyancy force?
			World &cWorld = static_cast<World&>(pBody->GetWorld());
			if (cWorld.IsBuoyancyActive()) {
				float fMatrix[16];
				NewtonBodyGetMatrix(pNewtonBody, fMatrix);

				if (fMatrix[13] < cWorld.GetBuoyancyPlaneY()) {
					// Body is in fluid, so we add buoyancy forces
					// Add force
					NewtonBodyAddBuoyancyForce(pNewtonBody, 70/cBodyImpl.m_fCollisionVolume*0.1f,
											   0.3f, 0.3f, cWorld.m_vGravity, PhysicsGetBuoyancyPlane, reinterpret_cast<void*>(&cWorld));

					// When in a fluid, we don't want our body to get autofreezed
					NewtonBodySetAutoSleep(pNewtonBody, 0);

					// Unfreeze body (sure is sure :)
					pBody->SetFrozen(false);
				} else {
					// Add gravity force
					vForce += cWorld.m_vGravity*fMass;

					// Turn auto freeze on/off
					NewtonBodySetAutoSleep(pNewtonBody, cBodyImpl.m_bAutoFreeze);
				}

			// Do never add buoyancy force, just a simple gravity one
			} else {
				// Add gravity force
				vForce += cWorld.m_vGravity*fMass;
			}
		}

		// Set force and torque of physics body
		NewtonBodySetForce (pNewtonBody, vForce);
		NewtonBodySetTorque(pNewtonBody, cBodyImpl.m_vNewtonTorque);

		// Reset the force and torque of the body
		cBodyImpl.m_vNewtonForce  = Vector3::Zero;
		cBodyImpl.m_vNewtonTorque = Vector3::Zero;
	}
}

/**
*  @brief
*    Set the transformation of a rigid body callback function
*/
void World::PhysicsSetTransform(const NewtonBody *pPhysicsBody, const dFloat *pfMatrix, int nThreadIndex)
{
	// Get the pointer to the PL physics body and implementation
	PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pPhysicsBody));
	if (pBody && pBody->IsActive()) {
		BodyImpl &cBodyImpl = static_cast<BodyImpl&>(pBody->GetBodyImpl());
		const NewtonBody *pNewtonBody = cBodyImpl.m_pNewtonBody;

		// Is the physics body asleep or frozen?
		if (NewtonBodyGetSleepState(pNewtonBody) == 0 && NewtonBodyGetFreezeState(pNewtonBody) == 0) {
			// Add the body to the list of changed bodies if required
			uint8 &nFlags = cBodyImpl.m_nChangedByPhysicsFlags;
			if (!nFlags)
				static_cast<World&>(pBody->GetWorld()).m_lstChangedByPhysics.Add(pBody);

			// Set flags
			nFlags |= BodyImpl::Position;
			nFlags |= BodyImpl::Rotation;
			nFlags |= BodyImpl::LinearVelocity;
			nFlags |= BodyImpl::AngularVelocity;
			nFlags |= BodyImpl::Force;
			nFlags |= BodyImpl::Torque;
		}
	}
}

/**
*  @brief
*    Rigid body destructor callback function
*/
void World::PhysicsBodyDestructor(const NewtonBody *pPhysicsBody)
{
	// Get the pointer to the PL physics body and implementation
	PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pPhysicsBody));
	if (pBody) {
		// Destroy the PL physics body, too
		static_cast<BodyImpl&>(pBody->GetBodyImpl()).m_pNewtonBody = nullptr;
		delete pBody;
	}
}

/**
*  @brief
*    Joint destructor callback function
*/
void World::PhysicsJointDestructor(const NewtonJoint *pPhysicsJoint)
{
	// Get the pointer to the PL physics joint
	PLPhysics::Joint *pJoint = static_cast<PLPhysics::Joint*>(NewtonJointGetUserData(pPhysicsJoint));
	if (pJoint) {
		// Destroy the PL physics joint, too
		static_cast<JointImpl&>(pJoint->GetJointImpl()).m_pNewtonJoint = nullptr;
		delete pJoint;
	}
}


//[-------------------------------------------------------]
//[ Private Newton callback functions                     ]
//[-------------------------------------------------------]
// Memory allocation for Newton
void *World::PhysicsAllocMemory(int nSizeInBytes)
{
	return new char[nSizeInBytes];
}

// Memory de-allocation for Newton
void World::PhysicsFreeMemory(void *pPtr, int nSizeInBytes)
{
	delete [] static_cast<char*>(pPtr);
}

// This callback is called when the two aabb boxes of the collision object overlap
int World::OnAABBOverlap(const NewtonMaterial *pMaterial, const NewtonBody *pNewtonBody1, const NewtonBody *pNewtonBody2, int nThreadIndex)
{
	// Get the PL physics bodies
	PLPhysics::Body *pBody1 = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pNewtonBody1));
	if (pBody1 && pBody1->IsActive() && pBody1->IsCollisionActive()) {
		PLPhysics::Body *pBody2 = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pNewtonBody2));
		if (pBody2 && pBody2->IsActive() && pBody2->IsCollisionActive()) {
			// Get the physics world
			World &cWorld = static_cast<World&>(pBody1->GetWorld());

			// Are the two bodies in different collision groups? If yes, is collision between this groups allowed?
			uint8 nCollisionGroup1 = pBody1->GetCollisionGroup();
			uint8 nCollisionGroup2 = pBody2->GetCollisionGroup();
			if ((cWorld.m_nGroupCollision[nCollisionGroup1] & (1<<nCollisionGroup2)) != 0) {
				// Get body pair flags
				nCollisionGroup1 = cWorld.GetBodyPairFlags(*pBody1, *pBody2);

				// Generate contacts?
				if (!(nCollisionGroup1 & Ignore)) {
					// Are both physics bodies currently asleep or frozen?
					const bool bInactive1 = NewtonBodyGetSleepState(pNewtonBody1) || NewtonBodyGetFreezeState(pNewtonBody1);
					const bool bInactive2 = NewtonBodyGetSleepState(pNewtonBody2) || NewtonBodyGetFreezeState(pNewtonBody2);

					// Two inactive bodies should NOT activate each other!
					if (!bInactive1 || !bInactive2) {
						// Create the 'ContactInformation'-instance on the runtime-stack
						PLPhysics::ContactInformation cContactInformation(*pBody1, *pBody2);

						// Emit signal
						cWorld.SignalContact(cContactInformation);

						// Return one the tell Newton the application wants to process this contact? (0=ignore, 1=use it)
						return cContactInformation.IsContactIgnored() ? 0 : 1;
					}
				}
			}
		}
	}

	// Return 0 the tell Newton the application does NOT want to process this contact
	return 0;
}

// This callback is called for every contact between the two bodies
void World::ContactsProcess(const NewtonJoint *pContact, dFloat fTimeStep, int nThreadIndex)
{
}

// Island update
int World::IslandUpdate(const NewtonWorld *pNewtonWorld, const void *pIslandHandle, int nBodyCount)
{
	// Loop through all physics bodies of the island
	for (int i=0; i<nBodyCount; i++) {
		// Get the physics body
		NewtonBody *pPhysicsBody = NewtonIslandGetBody(pIslandHandle, i);
		if (pPhysicsBody) {
			// Get the pointer to the PL physics body and implementation and check at first whether or not auto-freeze is enabled
			PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pPhysicsBody));
			if (pBody && pBody->IsActive() && pBody->IsAutoFreeze()) {
				// Check freeze threshold ('current length' < 'threshold length')
					  BodyImpl   &cBodyImpl   = static_cast<BodyImpl&>(pBody->GetBodyImpl());
				const NewtonBody *pNewtonBody = cBodyImpl.m_pNewtonBody;
				dFloat fVector[3];

				// Is the velocity below the threshold?
				NewtonBodyGetVelocity(pNewtonBody, fVector);
				if (fVector[0]*fVector[0] + fVector[1]*fVector[1] + fVector[2]*fVector[2] < cBodyImpl.m_fFreezeLinearVelocity*cBodyImpl.m_fFreezeLinearVelocity) {
					// Is the angular velocity below the threshold?
					NewtonBodyGetOmega(pNewtonBody, fVector);
					if (fVector[0]*fVector[0] + fVector[1]*fVector[1] + fVector[2]*fVector[2] < cBodyImpl.m_fFreezeAngularVelocity*cBodyImpl.m_fFreezeAngularVelocity) {
						cBodyImpl.m_nCurrentFreezeSteps++;
						if (cBodyImpl.m_nCurrentFreezeSteps >= cBodyImpl.m_nFreezeSteps) {
							// Freeze the physics body right now!
							cBodyImpl.m_nCurrentFreezeSteps = 0;
							cBodyImpl.SetNewtonBodyFreezeState(true);
						}
					} else {
						cBodyImpl.m_nCurrentFreezeSteps = 0;
					}
				} else {
					cBodyImpl.m_nCurrentFreezeSteps = 0;
				}
			}
		}
	}

	// Return 1 the tell Newton the application want to process this island
	return 1;
}

void World::WakeUp(const Newton::NewtonBody *pNewtonBody, void *pUserData)
{
	// Get the pointer to the PL physics body and implementation and check at first whether or not auto-freeze is enabled
	PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pNewtonBody));
	if (pBody)
		static_cast<BodyImpl&>(pBody->GetBodyImpl()).SetNewtonBodyFreezeState(false);
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
	PLPhysics::Body *pBody = new BodyConvexHull(*this, cMeshManager, sMesh, vMeshScale);

	// If the mesh loading failed, the Newton body is a null pointer...
	if (static_cast<BodyImpl&>(pBody->GetBodyImpl()).GetNewtonBody())
		return pBody;
	else {
		// Error!
		delete pBody;
		return nullptr;
	}
}

PLPhysics::Body *World::CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const String &sMesh, const Vector3 &vMeshScale, bool bOptimize)
{
	PLPhysics::Body *pBody = new BodyMesh(*this, cMeshManager, sMesh, vMeshScale, bOptimize);

	// If the mesh loading failed, the Newton body is a null pointer...
	if (static_cast<BodyImpl&>(pBody->GetBodyImpl()).GetNewtonBody())
		return pBody;
	else {
		// Error!
		delete pBody;
		return nullptr;
	}
}

PLPhysics::Body *World::CreateBodyTerrain(uint32 nWidth, uint32 nHeight, const float fTerrain[],
										  const Vector3 &vBoxMin, const Vector3 &vBoxMax, const Vector3 &vScale)
{
	return new BodyTerrain(*this, nWidth, nHeight, fTerrain, vBoxMin, vBoxMax, vScale);
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
	// [HACK] Is 'NewtonSetWorldSize()' not working correctly or why stop some objects if I don't multiplicate with 2??
	NewtonSetWorldSize(m_pNewtonWorld, m_vWorldSizeMin*2, m_vWorldSizeMax*2);
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

bool World::IsSimulationActive() const
{
	return m_bSimulationActive;
}

void World::SetSimulationActive(bool bActive)
{
	// State change?
	if (m_bSimulationActive != bActive) {
		// Set the new state
		m_bSimulationActive = bActive;

		// Update the update object if required
		if (m_pWorldUpdate) {
			if (m_bSimulationActive)
				m_pWorldUpdate->Unlock();
			else
				m_pWorldUpdate->Lock();
		}
	}
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

		// Set the solver precision mode (0=exact, 1=adaptive, n=linear, the default is exact)
		if (m_fSimulationQuality >= 0.5f)
			NewtonSetSolverModel(m_pNewtonWorld, (m_fSimulationQuality >= 0.75f) ? 0 : 1);
		else
			NewtonSetSolverModel(m_pNewtonWorld, 2+static_cast<int>((m_fSimulationQuality*2)*10.0f));

		// Set the friction model (0=exact coulomb, 1=adaptive coulomb, the default is exact)
		NewtonSetFrictionModel(m_pNewtonWorld, (m_fSimulationQuality >= 0.5f) ? 0 : 1);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
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
	// Is the simulation currently active?
	if (m_bSimulationActive) {
		// Lock the world update object
		if (m_pWorldUpdate)
			m_pWorldUpdate->Lock();

		// Physics update synchronization
		float fMatrix[16];
		Matrix3x3 mRot;

		// Loop through the list of physics bodies changed by the user
		if (m_lstChangedByUser.GetNumOfElements()) {
			{
				Iterator<PLPhysics::Body*> cIterator = m_lstChangedByUser.GetIterator();
				while (cIterator.HasNext()) {
					BodyImpl   &cBodyImpl     = static_cast<BodyImpl&>(cIterator.Next()->GetBodyImpl());
					NewtonBody *pNewtonBody   = cBodyImpl.m_pNewtonBody;
					uint16     &nUserFlags    = cBodyImpl.m_nChangedByUserFlags;
					uint8      &nPhysicsFlags = cBodyImpl.m_nChangedByPhysicsFlags;

					// Set the matrix of the rigid body, do NOT take scale into account!
					if ((nUserFlags & BodyImpl::Position) && (nUserFlags & BodyImpl::Rotation)) {
						// Construct the 4x4 matrix
						cBodyImpl.m_qRotation.ToRotationMatrix(mRot);
						fMatrix[0] = mRot.xx; fMatrix[4] = mRot.xy; fMatrix[ 8] = mRot.xz; fMatrix[12] = cBodyImpl.m_vPosition.x;
						fMatrix[1] = mRot.yx; fMatrix[5] = mRot.yy; fMatrix[ 9] = mRot.yz; fMatrix[13] = cBodyImpl.m_vPosition.y;
						fMatrix[2] = mRot.zx; fMatrix[6] = mRot.zy; fMatrix[10] = mRot.zz; fMatrix[14] = cBodyImpl.m_vPosition.z;
						fMatrix[3] = 0.0f;    fMatrix[7] = 0.0f;    fMatrix[11] = 0.0f;    fMatrix[15] = 1.0f;

						// Set the new physics body matrix
						NewtonBodySetMatrix(pNewtonBody, fMatrix);

						// Restore the previous freeze state, it looks like NewtonBodySetMatrix() not also wakes up bodies -
						// this wasn't the case in previous Newton Game Dynamics versions
						cBodyImpl.SetNewtonBodyFreezeState(cBodyImpl.m_bFrozen);

						// Reset by changed by physics state, user settings have a higher priority :)
						nPhysicsFlags &= ~BodyImpl::Position;
						nPhysicsFlags &= ~BodyImpl::Rotation;
					} else {
						// Only position
						if (nUserFlags & BodyImpl::Position) {
							// Construct the 4x4 matrix
							NewtonBodyGetMatrix(pNewtonBody, fMatrix);
							fMatrix[12] = cBodyImpl.m_vPosition.x;
							fMatrix[13] = cBodyImpl.m_vPosition.y;
							fMatrix[14] = cBodyImpl.m_vPosition.z;

							// Set the new physics body matrix
							NewtonBodySetMatrix(pNewtonBody, fMatrix);

							// Restore the previous freeze state, it looks like NewtonBodySetMatrix() not also wakes up bodies -
							// this wasn't the case in previous Newton Game Dynamics versions
							cBodyImpl.SetNewtonBodyFreezeState(cBodyImpl.m_bFrozen);

							// Reset by changed by physics state, user settings have a higher priority :)
							nPhysicsFlags &= ~BodyImpl::Position;
						} else {
							// Only rotation
							if (nUserFlags & BodyImpl::Rotation) {
								// Construct the 4x4 matrix
								NewtonBodyGetMatrix(pNewtonBody, fMatrix);
								cBodyImpl.m_qRotation.ToRotationMatrix(mRot);
								fMatrix[0] = mRot.xx; fMatrix[4] = mRot.xy; fMatrix[ 8] = mRot.xz;
								fMatrix[1] = mRot.yx; fMatrix[5] = mRot.yy; fMatrix[ 9] = mRot.yz;
								fMatrix[2] = mRot.zx; fMatrix[6] = mRot.zy; fMatrix[10] = mRot.zz;

								// Set the new physics body matrix
								NewtonBodySetMatrix(pNewtonBody, fMatrix);

								// Restore the previous freeze state, it looks like NewtonBodySetMatrix() not also wakes up bodies -
								// this wasn't the case in previous Newton Game Dynamics versions
								cBodyImpl.SetNewtonBodyFreezeState(cBodyImpl.m_bFrozen);

								// Reset by changed by physics state, user settings have a higher priority :)
								nPhysicsFlags &= ~BodyImpl::Rotation;
							}
						}
					}

					// Set the linear and angular velocity of the Newton physics body
					bool bWakeUpNeighbours = false;
					if (nUserFlags & BodyImpl::LinearVelocity) {
						NewtonBodySetVelocity(pNewtonBody, cBodyImpl.m_vLinearVelocity);
						bWakeUpNeighbours = true;

						// Reset by changed by physics state, user settings have a higher priority :)
						nPhysicsFlags &= ~BodyImpl::LinearVelocity;
					}
					if (nUserFlags & BodyImpl::AngularVelocity) {
						NewtonBodySetOmega(pNewtonBody, cBodyImpl.m_vAngularVelocity);
						bWakeUpNeighbours = true;

						// Reset by changed by physics state, user settings have a higher priority :)
						nPhysicsFlags &= ~BodyImpl::AngularVelocity;
					}
					// Wake up all physics bodies within this physics body's AABB? (mass of 0 = static body)
					if (bWakeUpNeighbours && cBodyImpl.GetMass() == 0.0f) {
						// Get the AABB of the physics body
						dFloat fP0[3];
						dFloat fP1[3];
						NewtonBodyGetMatrix(pNewtonBody, fMatrix);
						NewtonCollisionCalculateAABB(NewtonBodyGetCollision(pNewtonBody), fMatrix, fP0, fP1);

						// Wake up all physics bodies within this physics body's AABB
						NewtonWorldForEachBodyInAABBDo(m_pNewtonWorld, fP0, fP1, WakeUp, nullptr);

						// Now other physics bodies will wake up if they come into contact with a moving,
						// static physics body like for example a moving platform (also called kinematic/immobile object)
					}

					// Set force and torque of the Newton physics body
					if ((nUserFlags & BodyImpl::Force) || (nUserFlags & BodyImpl::Torque)) {
						if (nUserFlags & BodyImpl::Force) {
							cBodyImpl.m_vNewtonForce = cBodyImpl.m_vForce;

							// Reset by changed by physics state, user settings have a higher priority :)
							nPhysicsFlags &= ~BodyImpl::Force;
						}
						if (nUserFlags & BodyImpl::Torque) {
							cBodyImpl.m_vNewtonTorque = cBodyImpl.m_vTorque;

							// Reset by changed by physics state, user settings have a higher priority :)
							nPhysicsFlags &= ~BodyImpl::Torque;
						}

						// WAKE UP, it's time to do something! ;-)
						cBodyImpl.SetNewtonBodyFreezeState(false);
					}

					// Freeze/unfreeze
					if (nUserFlags & BodyImpl::Freeze)
						cBodyImpl.SetNewtonBodyFreezeState(true);
					if (nUserFlags & BodyImpl::Unfreeze)
						cBodyImpl.SetNewtonBodyFreezeState(false);

					// Done
					nUserFlags = 0;
				}
			}
			m_lstChangedByUser.FreeElements();
		}

		// Update the physics
		if (m_pWorldUpdate) {
			// Update the world update object
			m_pWorldUpdate->Update(Timing::GetInstance()->GetTimeScaleFactor(),
								   Timing::GetInstance()->GetMaxTimeDifference(),
								   m_fSimulationSpeed,
								   m_fFrameRate);
		}

		// Loop through the list of physics bodies changed by the physics
		if (m_lstChangedByPhysics.GetNumOfElements()) {
			{
				Iterator<PLPhysics::Body*> cIterator = m_lstChangedByPhysics.GetIterator();
				while (cIterator.HasNext()) {
					BodyImpl   &cBodyImpl   = static_cast<BodyImpl&>(cIterator.Next()->GetBodyImpl());
					NewtonBody *pNewtonBody = cBodyImpl.m_pNewtonBody;
					uint8      &nFlags      = cBodyImpl.m_nChangedByPhysicsFlags;

					// Position or rotation update?
					if ((nFlags & BodyImpl::Position) || (nFlags & BodyImpl::Rotation)) {
						// Get the matrix of the Newton physics body
						NewtonBodyGetMatrix(pNewtonBody, fMatrix);

						// Set PL physics body position
						if (nFlags & BodyImpl::Position)
							cBodyImpl.m_vPosition.SetXYZ(fMatrix[12], fMatrix[13], fMatrix[14]);

						// Set PL physics body rotation (we use the transposed Newton matrix)
						if (nFlags & BodyImpl::Rotation) {
							mRot.Set(fMatrix[0], fMatrix[4], fMatrix[ 8],
									 fMatrix[1], fMatrix[5], fMatrix[ 9],
									 fMatrix[2], fMatrix[6], fMatrix[10]);
							cBodyImpl.m_qRotation.FromRotationMatrix(mRot);
						}

						// Emit event
						const PLPhysics::Body *pBody = cBodyImpl.GetBody();
						if (pBody)
							pBody->EventTransform();
					}

					// Set the linear and angular velocity
					if (nFlags & BodyImpl::LinearVelocity)
						NewtonBodyGetVelocity(pNewtonBody, cBodyImpl.m_vLinearVelocity);
					if (nFlags & BodyImpl::AngularVelocity)
						NewtonBodyGetOmega(pNewtonBody, cBodyImpl.m_vAngularVelocity);

					// Set force and torque
					if (nFlags & BodyImpl::Force)
						cBodyImpl.m_vForce = cBodyImpl.m_vNewtonForce;
					if (nFlags & BodyImpl::Torque)
						cBodyImpl.m_vTorque = cBodyImpl.m_vNewtonTorque;

					// Frozen state
					cBodyImpl.m_bFrozen = (NewtonBodyGetSleepState(pNewtonBody) || NewtonBodyGetFreezeState(pNewtonBody));

					// Done, reset the flags
					nFlags = 0;
				}
			}
			m_lstChangedByPhysics.FreeElements();
		}

		// Unlock the world update object
		if (m_pWorldUpdate)
			m_pWorldUpdate->Unlock();
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
} // PLPhysicsNewton
