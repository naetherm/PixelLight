/*********************************************************\
 *  File: SNRagdoll.cpp                                  *
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


// [TODO] Test/fix joint controller
// [TODO] Test/fix no collision
// [TODO] Implement enable/disable bodies functions


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Loader.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Animation/Animation.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/Mesh.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/Skeleton.h>
#include <PLMesh/SkeletonHandler.h>
#include <PLMesh/MeshAnimationManager.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/RagdollBody.h"
#include "PLPhysics/SceneNodes/RagdollJoint.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodes/RagdollLoader.h"
#include "PLPhysics/SceneNodes/SNRagdoll.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNRagdoll)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNRagdoll::GetRagdollFilename() const
{
	return m_sRagdollFilename;
}

void SNRagdoll::SetRagdollFilename(const String &sValue)
{
	if (m_sRagdollFilename != sValue) {
		m_sRagdollFilename = sValue;
		if (IsInitialized())
			LoadByFilename(m_sRagdollFilename, "AllowAttachToWorld=\"" + m_bAttachToWorldEnabled + '\"');
	}
}

bool SNRagdoll::GetAttachToWorldEnabled() const
{
	return m_bAttachToWorldEnabled;
}

void SNRagdoll::SetAttachToWorldEnabled(bool bValue)
{
	if (m_bAttachToWorldEnabled != bValue) {
		m_bAttachToWorldEnabled = bValue;
		if (IsInitialized())
			LoadByFilename(m_sRagdollFilename, "AllowAttachToWorld=\"" + m_bAttachToWorldEnabled + '\"');
	}
}

bool SNRagdoll::GetAutoFreeze() const
{
	return m_bAutoFreeze;
}

void SNRagdoll::SetAutoFreeze(bool bValue)
{
	if (m_bAutoFreeze != bValue) {
		m_bAutoFreeze = bValue;

		// Update all the bodies
		for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++) {
			Body *pPhysicsBody = m_lstBodies[i]->GetBody();
			if (pPhysicsBody)
				pPhysicsBody->SetAutoFreeze(m_bAutoFreeze);
		}
	}
}

bool SNRagdoll::GetUseGravity() const
{
	return m_bUseGravity;
}

void SNRagdoll::SetUseGravity(bool bValue)
{
	if (m_bUseGravity != bValue) {
		m_bUseGravity = bValue;

		// Update all the bodies
		for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++) {
			Body *pPhysicsBody = m_lstBodies[i]->GetBody();
			if (pPhysicsBody)
				pPhysicsBody->SetUseGravity(m_bUseGravity);
		}
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNRagdoll::SNRagdoll() :
	RagdollFilename(this),
	ControllerEnabled(this),
	AttachToWorldEnabled(this),
	MaxTorque(this),
	AutoFreeze(this),
	UseGravity(this),
	InitFrozen(this),
	Flags(this),
	DebugFlags(this),
	EventHandlerUpdate(&SNRagdoll::OnUpdate, this),
	m_pWorldContainer(nullptr),
	m_bEnabled(false),
	m_pRagdollSkeletonHandler(nullptr),
	m_bAttachToWorldEnabled(false),
	m_bAutoFreeze(true),
	m_bUseGravity(true)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|DynamicMesh);
}

/**
*  @brief
*    Destructor
*/
SNRagdoll::~SNRagdoll()
{
}

/**
*  @brief
*    Returns the PL physics world scene node container the ragdoll is in
*/
SCPhysicsWorld *SNRagdoll::GetWorldContainer() const
{
	return m_pWorldContainer;
}

/**
*  @brief
*    Creates the ragdoll using a skeleton handler
*/
bool SNRagdoll::CreateRagdoll(SkeletonHandler *pSkeletonHandler)
{
	// First, unload the old ragdoll
	Unload();

	// Create an automatic ragdoll?
	if (!pSkeletonHandler)
		pSkeletonHandler = GetMeshHandler()->GetSkeletonHandler();

	// Check parameter
	if (pSkeletonHandler) {
		const Skeleton *pSkeleton = pSkeletonHandler->GetResource();
		if (pSkeleton) {
			// Create the physics joints and bodies. We do this by only adding skeleton joints which
			// have a parent, if such a joint is found we are also able so set the physics body assigned with
			// this parent to some nice default values. Note that each skeleton joint can product multiple
			// physics joints, and EACH physics joint has a physics body assigned with it - you can also see
			// this 'physics joint' as a 'bone'.
			const Array<JointHandler> &lstJointHandlers = pSkeletonHandler->GetJointHandlers();
			for (uint32 i=0; i<pSkeleton->GetNumOfElements(); i++) {
				const PLMesh::Joint *pJoint = pSkeleton->GetByIndex(i);
				if (pJoint) {
					const JointHandler &cJointHandler = lstJointHandlers[i];
					if (&cJointHandler != &Array<JointHandler>::Null) {
						const PLMesh::Joint *pParentJoint = pSkeleton->GetByIndex(pJoint->GetParent());
						if (pParentJoint) {
							const JointHandler &cParentJointHandler = lstJointHandlers[pParentJoint->GetID()];
							const PLMesh::Joint *pParentParentJoint = pSkeleton->GetByIndex(pParentJoint->GetParent());

							// Add the joint
							RagdollJoint *pRagdollJoint = new RagdollJoint(this);
							pRagdollJoint->nIndex = m_lstJoints.GetNumOfElements();
							m_lstJoints.Add(pRagdollJoint);
							pRagdollJoint->sAttached      = pParentJoint->GetName();
							pRagdollJoint->sThisJointName = pParentJoint->GetName();
							pRagdollJoint->sNextJointName = pJoint->GetName();
							if (pParentParentJoint) {
								// Here comes the tricky part: We can't ensure that the name of the parent
								// parent joint is equal to the body name assigned with that ragdoll joint...
								// pRagdollJoint->sParent = pParentParentJoint->GetName();
								// ... this is because it's possible and normal that for ONE 'orginal skeleton joint'
								// different physics joints exist!
								pRagdollJoint->sParent = "";
								for (uint32 nJoint=0; nJoint<m_lstJoints.GetNumOfElements(); nJoint++) {
									const RagdollJoint *pJointT = m_lstJoints[nJoint];
									if (pJointT->sThisJointName == pParentParentJoint->GetName() &&
										pJointT->sNextJointName == pParentJoint->GetName()) {
										pRagdollJoint->sParent = pJointT->sAttached;
										break;
									}
								}
							} else {
								pRagdollJoint->sParent = "";
							}
							pRagdollJoint->bAttachToWorld   = false;
							pRagdollJoint->nJointType       = RagdollJoint::Universal;
							pRagdollJoint->fLowJointRange1  = -0.3f;
							pRagdollJoint->fHighJointRange1 =  0.3f;
							pRagdollJoint->fLowJointRange2  = -0.3f;
							pRagdollJoint->fHighJointRange2 =  0.3f;
							pRagdollJoint->vJointAnchor     = cParentJointHandler.GetTranslationAbsolute();
							Vector3 vRight, vUp;
							pRagdollJoint->vRotAxis1 = cParentJointHandler.GetRotationAbsolute().GetZAxis();
							pRagdollJoint->vRotAxis1.GetRightUp(vRight, vUp);
							pRagdollJoint->vRotAxis1 = vRight;
							pRagdollJoint->vRotAxis2 = vUp;

							RagdollBody *pBody = nullptr;
							{ // Add new body
								pBody = new RagdollBody(this);
								pBody->nIndex = m_lstBodies.GetNumOfElements();
								m_lstBodies.Add(pBody);
								pBody->sJoint   = "";
								pBody->bEnabled = true;
								pBody->fMass    = 0.1f;

								// Get an unused body name
								pBody->sName = pRagdollJoint->sAttached;
								uint32 nIndex = 0;
								while (m_mapBodies.Get(pBody->sName)) {
									pBody->sName = pRagdollJoint->sAttached + '_' + static_cast<int>(nIndex);
									nIndex++;
								}
								pRagdollJoint->sAttached = pBody->sName;

								// Set correct parent body position, rotation and size
								pBody->vPos = (cJointHandler.GetTranslationAbsolute() + cParentJointHandler.GetTranslationAbsolute())*0.5f;
								Vector3 vDir = cJointHandler.GetTranslationAbsolute() - cParentJointHandler.GetTranslationAbsolute();
								const float fLength = vDir.GetLength();
								vDir.Normalize();
								pBody->qRot = Vector3::UnitY.GetRotationTo(vDir);
								pBody->vSize.SetXYZ(6.0f, fLength, 6.0f);

								m_mapBodies.Add(pBody->sName, pBody);
							}

							pRagdollJoint->sName = pRagdollJoint->sAttached + '_' + pRagdollJoint->sParent;
							pBody->sJoint = pRagdollJoint->sName;
							m_mapJoints.Add(pRagdollJoint->sName, pRagdollJoint);

					/* // [TODO] Detect bodies which are NOT allowed to collide, else the simulation may be ugly slow
							if (m_mapBodies.Get(pRagdollJoint->sParent) && m_mapBodies.Get(pRagdollJoint->sAttached)) {
								// Add new no collision
								NoCollision *pNoCollision = new NoCollision;
								pNoCollision->sFirst  = pRagdollJoint->sParent;
								pNoCollision->sSecond = pRagdollJoint->sAttached;
								m_lstNoCollisions.Add(pNoCollision);
							}
					*/
						}
					}
				}
			}


			// [TODO] Create automatic no collision list


			// [TODO] Disable collision between ALL ragdoll bodies
			for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++) {
				const RagdollBody *pBody = m_lstBodies[i];
				for (uint32 j=i+1; j<m_lstBodies.GetNumOfElements(); j++) {
					const RagdollBody *pBody2 = m_lstBodies[j];

					// Add new no collision
					NoCollision *pNoCollision = new NoCollision;
					pNoCollision->sFirst  = pBody->sName;
					pNoCollision->sSecond = pBody2->sName;
					m_lstNoCollisions.Add(pNoCollision);
				}
			}



			// Now create the physics ragdoll
			CreatePhysicsRagdoll();

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Creates the physics
*/
void SNRagdoll::CreatePhysics(bool bAllowAttachToWorld)
{
	// Get the physics world
	World *pWorld = m_pWorldContainer ? m_pWorldContainer->GetWorld() : nullptr;

	// Deactivate the physics simulation if required (just for sure :)
	const bool bSimulationActive = pWorld && pWorld->IsSimulationActive();
	if (bSimulationActive)
		pWorld->SetSimulationActive(false);

	// Create all the bodies
	CreatePhysicsBodies();

	// Create all the joints
	CreatePhysicsJoints(bAllowAttachToWorld);

	// Setup all no collisions
	if (pWorld) {
		for (uint32 i=0; i<m_lstNoCollisions.GetNumOfElements(); i++) {
			const NoCollision *pNoCollision = m_lstNoCollisions[i];

			// Get the bodies
			const RagdollBody *pFirstBody = m_mapBodies.Get(pNoCollision->sFirst);
			if (pFirstBody && pFirstBody->GetBody()) {
				const RagdollBody *pSecondBody = m_mapBodies.Get(pNoCollision->sSecond);
				if (pSecondBody && pSecondBody->GetBody()) {
					// Disable the collision between this two bodies
					pWorld->SetBodyPairFlags(*pFirstBody->GetBody(), *pSecondBody->GetBody(), World::Ignore);
				}
			}
		}
	}

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		pWorld->SetSimulationActive(bSimulationActive);
}

/**
*  @brief
*    Creates the physics bodies
*/
void SNRagdoll::CreatePhysicsBodies()
{
	// Get the physics world
	World *pWorld = m_pWorldContainer ? m_pWorldContainer->GetWorld() : nullptr;

	// Deactivate the physics simulation if required (just for sure :)
	const bool bSimulationActive = pWorld && pWorld->IsSimulationActive();
	if (bSimulationActive)
		pWorld->SetSimulationActive(false);

	// Create
	for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++) {
		RagdollBody *pBody = m_lstBodies[i];

		// Create the physics body
		pBody->CreatePhysicsBody();

		// Body freeze settings
		Body *pPhysicsBody = pBody->GetBody();
		if (pPhysicsBody) {
			pPhysicsBody->SetAutoFreeze(m_bAutoFreeze);
			pPhysicsBody->SetUseGravity(m_bUseGravity);
			if (!InitFrozen)
				pPhysicsBody->SetFrozen(false);
		}
	}

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		pWorld->SetSimulationActive(bSimulationActive);
}

/**
*  @brief
*    Creates the physics joints
*/
void SNRagdoll::CreatePhysicsJoints(bool bAllowAttachToWorld)
{
	// Get the physics world
	World *pWorld = m_pWorldContainer ? m_pWorldContainer->GetWorld() : nullptr;

	// Deactivate the physics simulation if required (just for sure :)
	const bool bSimulationActive = pWorld && pWorld->IsSimulationActive();
	if (bSimulationActive)
		pWorld->SetSimulationActive(false);

	// Create
	for (uint32 i=0; i<m_lstJoints.GetNumOfElements(); i++) {
		RagdollJoint &cJointProperty = *m_lstJoints[i];

		// Is the joint attached to the world?
		if (!(cJointProperty.bAttachToWorld && !bAllowAttachToWorld)) {
			// Create the physics joint
			cJointProperty.CreatePhysicsJoint();
		}
	}

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		pWorld->SetSimulationActive(bSimulationActive);
}

/**
*  @brief
*    Creates the physics ragdoll
*/
void SNRagdoll::CreatePhysicsRagdoll(bool bAllowAttachToWorld)
{
	// Get the physics world the ragdoll is in
	SceneContainer *pContainer = GetContainer();
	while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld"))
		pContainer = pContainer->GetContainer();
	m_pWorldContainer = static_cast<SCPhysicsWorld*>(pContainer);

	// Create the physics bodies and joints
	if (m_pWorldContainer)
		CreatePhysics(bAllowAttachToWorld);
}

/**
*  @brief
*    Returns the ragdoll skeleton
*/
Skeleton *SNRagdoll::GetSkeleton() const
{
	return m_pRagdollSkeletonHandler ? m_pRagdollSkeletonHandler->GetResource() : nullptr;
}

/**
*  @brief
*    Returns whether the ragdoll is enabled or not
*/
bool SNRagdoll::IsEnabled() const
{
	// [TODO] Implement me
//	return mEnabled;
	return false;
}

/**
*  @brief
*    Enables/disables all bodies and joints
*/
void SNRagdoll::SetEnabled(bool bEnabled)
{
	// [TODO] Implement me
/*	mEnabled = bEnabled;

	// Loop through all bodies and enable them
	for (uint32 i=0; i<m_lstJoints.GetNumOfElements(); i++) {
		const RagdollJoint &cJointProperty = **m_lstJoints[i];

		// Get the attached body
		const RagdollBody *pAttachedBody = m_mapBodies.Get(cJointProperty.sAttached);
		if (pAttachedBody) {
			// Enable appropriate bodies
			if (mEnabled && pAttachedBody->enabled) {
				rigidLinks[pAttachedBody->nIndex]->SetEnabled(true);

				if (!cJointProperty.attachToWorld) {
					// Get the parent body
					RagdollBody *pParentBody = m_mapBodies.Get(cJointProperty.sParent);
					if (pParentBody)
						rigidLinks[pParentBody->nIndex]->SetEnabled(true);
				}
			} else if (!mEnabled) {
				rigidLinks[pAttachedBody->nIndex]->SetEnabled(false);
			}
			}
	}*/
}

/**
*  @brief
*    Returns the number of bodies
*/
uint32 SNRagdoll::GetNumOfBodies() const
{
	return m_lstBodies.GetNumOfElements();
}

/**
*  @brief
*    Returns a body
*/
RagdollBody *SNRagdoll::GetBody(uint32 nIndex) const
{
	return m_lstBodies[nIndex];
}

/**
*  @brief
*    Returns a body
*/
RagdollBody *SNRagdoll::GetBody(const String &sName) const
{
	return m_mapBodies.Get(sName);
}

/**
*  @brief
*    Adds a new body
*/
RagdollBody *SNRagdoll::AddBody(const String &sName, bool bCreatePhysics)
{
	// Create the new body
	RagdollBody *pBody = new RagdollBody(this);
	pBody->nIndex = m_lstBodies.GetNumOfElements();
	m_lstBodies.Add(pBody);

	// Find an unused body name
	String sBodyName;
	for (uint32 i=0;; i++) {
		if (i)
			sBodyName = sName + '_' + static_cast<int>(i);
		else
			sBodyName = sName;
		if (!m_mapBodies.Get(sBodyName)) {
			// Ok, now we have an unused name :)
			pBody->sName = sBodyName;
			m_mapBodies.Add(sBodyName, pBody);
			break;
		}
	}

	// Create the physics body?
	if (bCreatePhysics)
		pBody->CreatePhysicsBody();

	// Return the new body
	return pBody;
}

/**
*  @brief
*    Clones a body
*/
RagdollBody *SNRagdoll::CloneBody(uint32 nIndex, bool bCreatePhysics)
{
	// First, get the body to clone
	const RagdollBody *pBodyToClone = GetBody(nIndex);
	if (pBodyToClone) {
		// Clone
		RagdollBody *pNewBody = AddBody(pBodyToClone->sName, false);
		if (pNewBody) {
			// Copy attributes
			pNewBody->sJoint   = pBodyToClone->sJoint;
			pNewBody->bEnabled = pBodyToClone->bEnabled;
			pNewBody->vSize    = pBodyToClone->vSize;
			pNewBody->vPos     = pBodyToClone->vPos;
			pNewBody->qRot     = pBodyToClone->qRot;
			pNewBody->fMass    = pBodyToClone->fMass;
			pNewBody->qRot     = pBodyToClone->qRot;

			// Create the physics body?
			if (bCreatePhysics)
				pNewBody->CreatePhysicsBody();

			// Return the clone
			return pNewBody;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Removes a body
*/
bool SNRagdoll::RemoveBody(uint32 nIndex)
{
	// First, get the body
	RagdollBody *pBody = m_lstBodies[nIndex];
	if (pBody) {
		// Unmark this joint handler as not user controlled
		const MeshHandler *pMeshHandler = GetMeshHandler();
		if (pMeshHandler) {
			const SkeletonHandler *pMeshSkeletonHandler = pMeshHandler->GetSkeletonHandler();
			if (pMeshSkeletonHandler) {
				JointHandler *pJointHandler = pMeshSkeletonHandler->GetJointHandler(pBody->sName);
				if (pJointHandler)
					pJointHandler->SetUserControlled(false);
			}
		}

		// Remove and destroy
		m_lstBodies.RemoveAtIndex(nIndex);
		m_mapBodies.Remove(pBody->sName);
		delete pBody;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Removes a body and the joint attached to it
*/
bool SNRagdoll::RemoveBodyAndJoint(uint32 nIndex)
{
	// First, get the body
	RagdollBody *pBody = m_lstBodies[nIndex];
	if (pBody) {
		// Remove and destroy this attached joint
		RagdollJoint *pJoint = m_mapJoints.Get(pBody->sJoint);
		if (pJoint) {
			m_lstJoints.Remove(pJoint);
			m_mapJoints.Remove(pJoint->sName);
			delete pJoint;
		}

		// Unmark this joint as user not controlled
		const MeshHandler *pMeshHandler = GetMeshHandler();
		if (pMeshHandler) {
			const SkeletonHandler *pMeshSkeletonHandler = pMeshHandler->GetSkeletonHandler();
			if (pMeshSkeletonHandler) {
				JointHandler *pJointHandler = pMeshSkeletonHandler->GetJointHandler(pBody->sName);
				if (pJointHandler)
					pJointHandler->SetUserControlled(false);
			}
		}

		// Remove and destroy
		m_lstBodies.RemoveAtIndex(nIndex);
		m_mapBodies.Remove(pBody->sName);
		delete pBody;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the number of joints
*/
uint32 SNRagdoll::GetNumOfJoints() const
{
	return m_lstJoints.GetNumOfElements();
}

/**
*  @brief
*    Returns a joint
*/
RagdollJoint *SNRagdoll::GetJoint(uint32 nIndex) const
{
	return m_lstJoints[nIndex];
}

/**
*  @brief
*    Returns a joint
*/
RagdollJoint *SNRagdoll::GetJoint(const String &sName) const
{
	return m_mapJoints.Get(sName);
}

/**
*  @brief
*    Adds a new joint
*/
RagdollJoint *SNRagdoll::AddJoint(const String &sName, bool bCreatePhysics)
{
	// Create the new joint
	RagdollJoint *pJoint = new RagdollJoint(this);
	pJoint->nIndex = m_lstBodies.GetNumOfElements();
	m_lstJoints.Add(pJoint);

	// Find an unused joint name
	String sJointName;
	for (uint32 i=0;; i++) {
		if (i)
			sJointName = sName + '_' + static_cast<int>(i);
		else
			sJointName = sName;
		if (!m_mapJoints.Get(sJointName)) {
			// Ok, now we have an unused name :)
			pJoint->sName = sJointName;
			m_mapJoints.Add(sJointName, pJoint);
			break;
		}
	}

	// Create the physics joint?
	if (bCreatePhysics)
		pJoint->CreatePhysicsJoint();

	// Return the new joint
	return pJoint;
}

/**
*  @brief
*    Clones a joint
*/
RagdollJoint *SNRagdoll::CloneJoint(uint32 nIndex, bool bCreatePhysics)
{
	// First, get the joint to clone
	const RagdollJoint *pJointToClone = GetJoint(nIndex);
	if (pJointToClone) {
		// Clone
		RagdollJoint *pNewJoint = AddJoint(pJointToClone->sName, false);
		if (pNewJoint) {
			// Copy attributes
			pNewJoint->sAttached        = pJointToClone->sAttached;
			pNewJoint->sParent          = pJointToClone->sParent;
			pNewJoint->sThisJointName   = pJointToClone->sThisJointName;
			pNewJoint->sNextJointName   = pJointToClone->sNextJointName;
			pNewJoint->bAttachToWorld   = pJointToClone->bAttachToWorld;
			pNewJoint->nJointType       = pJointToClone->nJointType;
			pNewJoint->fLowJointRange1  = pJointToClone->fLowJointRange1;
			pNewJoint->fHighJointRange1 = pJointToClone->fHighJointRange1;
			pNewJoint->fLowJointRange2  = pJointToClone->fLowJointRange2;
			pNewJoint->fHighJointRange2 = pJointToClone->fHighJointRange2;
			pNewJoint->vJointAnchor     = pJointToClone->vJointAnchor;
			pNewJoint->vRotAxis1        = pJointToClone->vRotAxis1;
			pNewJoint->vRotAxis2        = pJointToClone->vRotAxis2;

			// Create the physics joint?
			if (bCreatePhysics)
				pNewJoint->CreatePhysicsJoint();

			// Return the clone
			return pNewJoint;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Removes a joint
*/
bool SNRagdoll::RemoveJoint(uint32 nIndex)
{
	// First, get the joint
	RagdollJoint *pJoint = m_lstJoints[nIndex];
	if (pJoint) {
		// Unmark this joint as user controlled
		const MeshHandler *pMeshHandler = GetMeshHandler();
		if (pMeshHandler) {
			const SkeletonHandler *pMeshSkeletonHandler = pMeshHandler->GetSkeletonHandler();
			if (pMeshSkeletonHandler) {
				JointHandler *pJointHandler = pMeshSkeletonHandler->GetJointHandler(pJoint->sName);
				if (pJointHandler)
					pJointHandler->SetUserControlled(false);
			}
		}

		// Remove and destroy
		m_lstJoints.RemoveAtIndex(nIndex);
		m_mapJoints.Remove(pJoint->sName);
		delete pJoint;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the number of no collisions
*/
uint32 SNRagdoll::GetNumOfNoCollisions() const
{
	return m_lstNoCollisions.GetNumOfElements();
}

/**
*  @brief
*    Adds a new no collision
*/
NoCollision *SNRagdoll::AddNoCollision()
{
	// Create the new no collision
	NoCollision *pNoCollision = new NoCollision;
	MemoryManager::Set(pNoCollision, 0, sizeof(NoCollision));
	m_lstNoCollisions.Add(pNoCollision);

	// Done
	return pNoCollision;
}

/**
*  @brief
*    Returns a no collision
*/
NoCollision *SNRagdoll::GetNoCollision(uint32 nIndex) const
{
	return m_lstNoCollisions[nIndex];
}

/**
*  @brief
*    Removes a no collision
*/
bool SNRagdoll::RemoveNoCollision(uint32 nIndex)
{
	NoCollision *pNoCollision = m_lstNoCollisions[nIndex];
	if (pNoCollision) {
		delete pNoCollision;
		m_lstNoCollisions.RemoveAtIndex(nIndex);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Returns the number of controllers
*/
uint32 SNRagdoll::GetNumOfControllers() const
{
	return m_lstControllers.GetNumOfElements();
}

/**
*  @brief
*    Adds a new controller
*/
JointController *SNRagdoll::AddController(const String &sJointName)
{
	// Is there already a controller for this joint?
	JointController *pController = m_mapControllers.Get(sJointName);
	if (pController)
		return pController;
	else {
		// Add new controller
		pController = new JointController;
		MemoryManager::Set(pController, 0, sizeof(JointController));
		m_lstControllers.Add(pController);

		// Add to controller hash map
		m_mapControllers.Add(pController->sJoint, pController);

		// Return the new controller
		return pController;
	}
}

/**
*  @brief
*    Returns a controller
*/
JointController *SNRagdoll::GetController(uint32 nIndex) const
{
	return m_lstControllers[nIndex];
}

/**
*  @brief
*    Returns a controller
*/
JointController *SNRagdoll::GetController(const String &sName) const
{
	return m_mapControllers.Get(sName);
}

/**
*  @brief
*    Removes a controller
*/
bool SNRagdoll::RemoveController(uint32 nIndex)
{
	JointController *pController = m_lstControllers[nIndex];
	if (pController) {
		m_mapControllers.Remove(pController->sJoint);
		delete pController;
		m_lstControllers.RemoveAtIndex(nIndex);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Computes and applies PD-controller torques to each body part
*/
void SNRagdoll::ApplyControlTorques()
{
	// Controllers enabled?
	if (ControllerEnabled) {
		// Loop through all controllers
		for (uint32 i=0; i<m_lstControllers.GetNumOfElements(); i++) {
			JointController *pController = m_lstControllers[i];
			if (pController->bIsControllable) {
				// Get the controller joint
				RagdollJoint *pControlledJoint = m_mapJoints.Get(pController->sJoint);
				if (pControlledJoint && !pControlledJoint->bAttachToWorld) {
					// Get the attached body
					const RagdollBody *pAttachedBody = m_mapBodies.Get(pControlledJoint->sAttached);
					if (pAttachedBody && pAttachedBody->bEnabled) {
						// Apply the torques
						pControlledJoint->ApplyControlTorques(pController, MaxTorque);
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNRagdoll::OnUpdate()
{
	MeshHandler *pMeshHandler = GetMeshHandler();
	if (pMeshHandler) {
		SkeletonHandler *pMeshSkeletonHandler = pMeshHandler->GetSkeletonHandler();
		if (pMeshSkeletonHandler) {
			if (!GetMeshHandler()->GetMeshAnimationManager()) {
				GetMeshHandler()->CreateMeshAnimationManager();
				if (pMeshSkeletonHandler) {
					pMeshSkeletonHandler->ResetJointStates();
					pMeshSkeletonHandler->ApplyBaseJointStates();
					pMeshSkeletonHandler->CalculateStates();
				}
			}

			// Get skeleton
			const Skeleton *pSkeleton = m_pRagdollSkeletonHandler ? m_pRagdollSkeletonHandler->GetResource() : nullptr;
			if (pSkeleton) {
				const Skeleton *pMeshSkeleton = pMeshSkeletonHandler->GetResource();
				if (pMeshSkeleton) {
					// Get absolute joint states
					const Array<JointHandler> &lstJointHandlers = pMeshSkeletonHandler->GetJointHandlers();
					for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++) {
						// Get this body
						const RagdollBody *pBody = m_lstBodies[i];

						const PLMesh::Joint *pJoint = pSkeleton->GetByName(pBody->sName);
						if (pJoint) {
							const RagdollJoint *pJointProperty = GetJoint(pBody->sJoint);
							if (pJointProperty) {
								const uint32 nID = pJoint->GetID();
								JointHandler &cJointHandler = lstJointHandlers[nID];
								if (&cJointHandler != &Array<JointHandler>::Null) {
									// Mark this joint handler as user controlled
									cJointHandler.SetUserControlled(true);

									// Get the absolute position of the joint
									Vector3 vAnchor;
									pJointProperty->GetCurrentAnchor(vAnchor);
									cJointHandler.SetTranslationAbsolute(vAnchor);

									// Get the absolute rotation of the joint
									Quaternion qQ;
									pBody->GetRotation(qQ);
									cJointHandler.SetRotationAbsolute(pJoint->GetRotationAbsolute()*(GetTransform().GetRotation().GetUnitInverted()*qQ*pBody->qRot.GetUnitInverted()).GetUnitInverted());
								}
							}
						}
					}

					// Set scene node position by using the root joint of the skeleton
					if (pSkeleton->GetRootJoints().GetNumOfElements()) {
						const JointHandler &cRootBaseJointHandler = pMeshSkeletonHandler->GetBaseJointHandlers()[pSkeleton->GetRootJoints()[0]];
						const JointHandler &cRootJointHandler     = lstJointHandlers[pSkeleton->GetRootJoints()[0]];
						if ((&cRootBaseJointHandler != &Array<JointHandler>::Null) &&
							(&cRootJointHandler != &Array<JointHandler>::Null)) {
							const PLMesh::Joint *pRootJoint = cRootJointHandler.GetElement();
							if (pRootJoint) {
								const Vector3 vRootTranslation = cRootJointHandler.GetTranslationAbsolute() - cRootBaseJointHandler.GetTranslation()*GetTransform().GetScale();
								MoveTo(vRootTranslation);

								// Update all joints
								Vector3 vInvScale(1/GetTransform().GetScale().x, 1/GetTransform().GetScale().y, 1/GetTransform().GetScale().z);
								for (uint32 i=0; i<lstJointHandlers.GetNumOfElements(); i++) {
									JointHandler &cJointHandler = lstJointHandlers[i];
									cJointHandler.SetTranslationAbsolute(GetTransform().GetRotation().GetUnitInverted()*(cJointHandler.GetTranslationAbsolute()-vRootTranslation)*vInvScale);
								}
							}
						}
					}

					// The mesh requires an update
					pMeshHandler->MeshUpdateRequired();

					// Ensure that there's a "PLScene::SNMMeshUpdate" instance within the owner scene node which takes care of the frequent mesh update
					GetSNMMeshUpdate();
				}
			}
		}
	}
}

/**
*  @brief
*    Returns a "PLScene::SNMMeshUpdate" instance from the owner scene node
*/
SNMMeshUpdate *SNRagdoll::GetSNMMeshUpdate()
{
	// Is there already an instance of the "PLScene::SNMMeshUpdate" scene node modifier?
	static const String sSNMMeshUpdate = "PLScene::SNMMeshUpdate";
	SNMMeshUpdate *pSNMMeshUpdate = reinterpret_cast<SNMMeshUpdate*>(GetModifier(sSNMMeshUpdate));
	if (!pSNMMeshUpdate)
		pSNMMeshUpdate = reinterpret_cast<SNMMeshUpdate*>(AddModifier(sSNMMeshUpdate, "Flags=\"Automatic\""));

	// Return the SNMMeshUpdate instance
	return pSNMMeshUpdate;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNRagdoll::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNMesh::DrawDebug(cRenderer, pVisNode);

	// Draw physics bodies?
	if (!(GetDebugFlags() & DebugNoPhysicsBodies) && pVisNode) {
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::BlendEnable,  true);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		static const Color4 cColor(1.0f, 0.5f, 0.5f, 0.5f);
		for (uint32 i=0; i<GetNumOfBodies(); i++)
			GetBody(i)->Draw(cRenderer, cColor, *pVisNode);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SNRagdoll::InitFunction()
{
	// Call base implementation
	SNMesh::InitFunction();

	// Get the mesh handler
	MeshHandler *pMeshHandler = GetMeshHandler();

	// Initialization
	m_pWorldContainer = nullptr;
	SkeletonHandler *pMeshSkeletonHandler = pMeshHandler->GetSkeletonHandler();
	if (!pMeshHandler->GetMeshAnimationManager()) {
		pMeshHandler->CreateMeshAnimationManager();
		if (pMeshSkeletonHandler) {
			pMeshSkeletonHandler->ResetJointStates();
			pMeshSkeletonHandler->ApplyBaseJointStates();
			pMeshSkeletonHandler->CalculateStates();
		}
	}

	// We will reset the joint states by yourself...
	if (pMeshHandler->GetMeshAnimationManager())
		pMeshHandler->GetMeshAnimationManager()->SetResetJointStates(true);

	// Load/create the ragdoll
	if (m_sRagdollFilename.GetLength())
		LoadByFilename(m_sRagdollFilename);
	else
		CreateRagdoll(pMeshSkeletonHandler);

	// Create the ragdoll skeleton
	m_pRagdollSkeletonHandler = new SkeletonHandler();
	if (pMeshSkeletonHandler)
		m_pRagdollSkeletonHandler->SetResource(pMeshSkeletonHandler->GetResource());

	// The mesh requires an update
	pMeshHandler->MeshUpdateRequired();
}

void SNRagdoll::DeInitFunction()
{
	// Destroy the ragdoll
	Unload();
	delete m_pRagdollSkeletonHandler;
	m_pRagdollSkeletonHandler = nullptr;

	// Call base implementation
	SNMesh::DeInitFunction();
}

void SNRagdoll::OnActivate(bool bActivate)
{
	// Call the base implementation
	SNMesh::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
			OnUpdate();
		} else {
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool SNRagdoll::Unload()
{
	// Get the physics world
	World *pWorld = m_pWorldContainer ? m_pWorldContainer->GetWorld() : nullptr;

	// Deactivate the physics simulation if required (just for sure :)
	const bool bSimulationActive = pWorld && pWorld->IsSimulationActive();
	if (bSimulationActive)
		pWorld->SetSimulationActive(false);

	// Clear bodies and unmark the used joints as not user controlled
	const MeshHandler *pMeshHandler = GetMeshHandler();
	if (pMeshHandler) {
		const SkeletonHandler *pMeshSkeletonHandler = pMeshHandler->GetSkeletonHandler();
		if (pMeshSkeletonHandler) {
			for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++) {
				// Unmark the joint handler as not user controlled
				JointHandler *pJointHandler = pMeshSkeletonHandler->GetJointHandler(m_lstBodies[i]->sName);
				if (pJointHandler)
					pJointHandler->SetUserControlled(false);

				// Delete the body
				delete m_lstBodies[i];
			}
			m_lstBodies.Clear();
		}
	}

	// Clear bodies (if the stuff above failed)
	for (uint32 i=0; i<m_lstBodies.GetNumOfElements(); i++)
		delete m_lstBodies[i];
	m_lstBodies.Clear();
	m_mapBodies.Clear();

	// Clear joints
	for (uint32 i=0; i<m_lstJoints.GetNumOfElements(); i++)
		delete m_lstJoints[i];
	m_lstJoints.Clear();
	m_mapJoints.Clear();

	// Clear no collisions
	for (uint32 i=0; i<m_lstNoCollisions.GetNumOfElements(); i++)
		delete m_lstNoCollisions[i];
	m_lstNoCollisions.Clear();

	// Clear controllers
	for (uint32 i=0; i<m_lstControllers.GetNumOfElements(); i++)
		delete m_lstControllers[i];
	m_lstControllers.Clear();
	m_mapControllers.Clear();

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		pWorld->SetSimulationActive(bSimulationActive);

	// Call base implementation
	return Loadable::Unload();
}

String SNRagdoll::GetLoadableTypeName() const
{
	static const String sString = "Ragdoll";
	return sString;
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::Loadable functions          ]
//[-------------------------------------------------------]
bool SNRagdoll::CallLoadable(File &cFile, Loader &cLoader, const String &sMethod, const String &sParams)
{
	// Get the loader implementation
	LoaderImpl *pLoaderImpl = cLoader.GetImpl();
	if (pLoaderImpl) {
		// Load
		if (sParams.GetLength()) {
			pLoaderImpl->CallMethod(sMethod, "Param0=\"" + Type<SNRagdoll&>::ConvertToString(*this) + "\" Param1=\"" + Type<File&>::ConvertToString(cFile) + "\" " + sParams);
			return true;
		} else {
			Params<bool, SNRagdoll&, File&> cParams(*this, cFile);
			pLoaderImpl->CallMethod(sMethod, cParams);
			return cParams.Return;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
