/*********************************************************\
 *  File: SNMAnchor.cpp                                  *
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
#include <PLMath/EulerAngles.h>
#include <PLMesh/Joint.h>
#include <PLMesh/Skeleton.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/SkeletonHandler.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMAnchor.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMAnchor)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMAnchor::GetAttachedNode() const
{
	return m_sAttachedNode;
}

void SNMAnchor::SetAttachedNode(const String &sValue)
{
	if (m_sAttachedNode != sValue) {
		m_sAttachedNode = sValue;
		m_cAttachedNodeHandler.SetElement(GetSceneNode().GetContainer()->GetByName(m_sAttachedNode));
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMAnchor::SNMAnchor(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode),
	AttachedNode(this),
	PositionOffset(this),
	RotationOffset(this),
	SkeletonJoint(this),
	JointPositionOffset(this),
	JointRotationOffset(this),
	Flags(this),
	SlotOnContainer(this),
	SlotOnPositionRotationUpdate(this)
{
}

/**
*  @brief
*    Destructor
*/
SNMAnchor::~SNMAnchor()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMAnchor::OnActivate(bool bActivate)
{
	// Connect/disconnect event handlers
	SceneNode &cSceneNode = GetSceneNode();
	if (bActivate) {
		// Connect event handlers
		cSceneNode.SignalContainer.Connect(SlotOnContainer);
		cSceneNode.GetTransform().EventPosition.Connect(SlotOnPositionRotationUpdate);
		cSceneNode.GetTransform().EventRotation.Connect(SlotOnPositionRotationUpdate);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Connect(SlotOnPositionRotationUpdate);

		// Make a first update to ensure position and rotation are up-to-date when we're going active (synchronization, no logic update)
		OnPositionRotationUpdate();
	} else {
		// Disconnect event handlers
		cSceneNode.SignalContainer.Disconnect(SlotOnContainer);
		cSceneNode.GetTransform().EventPosition.Disconnect(SlotOnPositionRotationUpdate);
		cSceneNode.GetTransform().EventRotation.Disconnect(SlotOnPositionRotationUpdate);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Disconnect(SlotOnPositionRotationUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node container changed
*/
void SNMAnchor::OnContainer()
{
	// Is it allowed to change the container the attached node is in?
	if (!(GetFlags() & NoContainerUpdate)) {
		// Get the scene node
		const SceneNode &cSceneNode = GetSceneNode();

		// Get the attached scene node
		SceneNode *pAttachedSceneNode = m_cAttachedNodeHandler.GetElement();
		if (!pAttachedSceneNode && cSceneNode.GetContainer()) {
			pAttachedSceneNode = cSceneNode.GetContainer()->GetByName(m_sAttachedNode);
			m_cAttachedNodeHandler.SetElement(pAttachedSceneNode);
		}
		if (pAttachedSceneNode && cSceneNode.GetContainer())
			pAttachedSceneNode->SetContainer(*cSceneNode.GetContainer());
	}
}

/**
*  @brief
*    Called when the scene node position or rotation changed or on update request
*/
void SNMAnchor::OnPositionRotationUpdate()
{
	// Is there anything to do in here?
	if (!(GetFlags() & NoPosition) || !(GetFlags() & NoRotation)) {
		// Get the owner scene node
		SceneNode &cOwnerSceneNode = GetSceneNode();

		// Get the attached scene node
		SceneNode *pAttachedSceneNode = m_cAttachedNodeHandler.GetElement();
		if (!pAttachedSceneNode && cOwnerSceneNode.GetContainer()) {
			pAttachedSceneNode = cOwnerSceneNode.GetContainer()->GetByName(m_sAttachedNode);
			m_cAttachedNodeHandler.SetElement(pAttachedSceneNode);
		}
		if (pAttachedSceneNode) {
			// Attached to a skeleton joint?
			Vector3 vPos;
			Quaternion qRot;
			if (SkeletonJoint.Get().GetLength()) {
				// Is there a mesh handler?
				const MeshHandler *pMeshHandler = cOwnerSceneNode.GetMeshHandler();
				if (pMeshHandler) {
					// Is there a skeleton handler?
					const SkeletonHandler *pSkeletonHandler = pMeshHandler->GetSkeletonHandler();
					if (pSkeletonHandler) {
						// Has this skeleton handler a skeleton resource?
						const Skeleton *pSkeleton = pSkeletonHandler->GetResource();
						if (pSkeleton) {
							// Is there a skeleton joint with the given name?
							const Joint *pJoint = pSkeleton->GetByName(SkeletonJoint.Get());
							if (pJoint) {
								// Get skeleton handler
								const Array<JointHandler> &lstJointHandlers = pSkeletonHandler->GetJointHandlers();
								const JointHandler &cJointHandler = lstJointHandlers[pJoint->GetID()];
								if (&cJointHandler != &Array<JointHandler>::Null) {
									// Set the position of the attached node
									if (!(GetFlags() & NoPosition))
										vPos = cJointHandler.GetTranslationAbsolute()+cJointHandler.GetRotationAbsolute().GetUnitInverted()*JointPositionOffset.Get();

									// Set the rotation of the attached node
									if (!(GetFlags() & NoRotation)) {
										Quaternion qRotOffset;
										EulerAngles::ToQuaternion(static_cast<float>(JointRotationOffset.Get().x*Math::DegToRad),
																  static_cast<float>(JointRotationOffset.Get().y*Math::DegToRad),
																  static_cast<float>(JointRotationOffset.Get().z*Math::DegToRad),
																  qRotOffset);
										qRot = cJointHandler.GetRotationAbsolute().GetUnitInverted()*qRotOffset;
									}
								}
							}
						}
					}
				}
			}

			// Transform into camera container space
			Matrix3x4 mTransform;
			cOwnerSceneNode.GetContainer()->GetTransformMatrixTo(*pAttachedSceneNode->GetContainer(), mTransform);

			// Set the position of the attached node
			if (!(GetFlags() & NoPosition))
				pAttachedSceneNode->GetTransform().SetPosition(mTransform*(cOwnerSceneNode.GetTransform().GetPosition()+cOwnerSceneNode.GetTransform().GetRotation()*(PositionOffset.Get()+vPos)*cOwnerSceneNode.GetTransform().GetScale()));

			// Set the rotation of the attached node
			if (!(GetFlags() & NoRotation)) {
				Quaternion qRotOffset;
				EulerAngles::ToQuaternion(static_cast<float>(RotationOffset.Get().x*Math::DegToRad),
										  static_cast<float>(RotationOffset.Get().y*Math::DegToRad),
										  static_cast<float>(RotationOffset.Get().z*Math::DegToRad),
										  qRotOffset);
				pAttachedSceneNode->GetTransform().SetRotation(Quaternion(mTransform)*(cOwnerSceneNode.GetTransform().GetRotation()*qRotOffset*qRot));
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
