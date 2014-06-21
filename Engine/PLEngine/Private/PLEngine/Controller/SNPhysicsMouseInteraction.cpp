/*********************************************************\
 *  File: SNPhysicsMouseInteraction.cpp                  *
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
#include <PLCore/Frontend/Frontend.h>
#include <PLCore/Frontend/FrontendApplication.h>
#include <PLInput/Input/InputManager.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/SurfaceWindow.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLPhysics/Body.h>
#include <PLPhysics/World.h>
#include <PLPhysics/Sensor.h>
#include <PLPhysics/SceneNodes/SCPhysicsWorld.h>
#include "PLEngine/Controller/PhysicsMouseInteraction.h"
#include "PLEngine/Controller/SNPhysicsMouseInteraction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
using namespace PLPhysics;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNPhysicsMouseInteraction)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNPhysicsMouseInteraction::GetForceLineName() const
{
	return m_sForceLineName;
}

void SNPhysicsMouseInteraction::SetForceLineName(const String &sValue)
{
	if (m_sForceLineName != sValue) {
		m_sForceLineName = sValue;
		m_cForceLineHandler.SetElement();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNPhysicsMouseInteraction::SNPhysicsMouseInteraction() :
	InputSemantic(this),
	MaxPickingRange(this),
	ThrowForce(this),
	ForceLineName(this),
	Flags(this),
	EventHandlerUpdate(&SNPhysicsMouseInteraction::OnUpdate, this),
	m_sForceLineName("PhysicsForceLine"),
	m_bPicking(false),
	m_pPickedPhysicsBody(nullptr),
	m_fPickedDistance(0.0f),
	m_pController(new PhysicsMouseInteraction())
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCulling);
}

/**
*  @brief
*    Destructor
*/
SNPhysicsMouseInteraction::~SNPhysicsMouseInteraction()
{
	// Destroy the input controller
	delete m_pController;
}

/**
*  @brief
*    Returns whether or not picking is currently performed
*/
bool SNPhysicsMouseInteraction::IsPicking(Vector2i *pvMousePos) const
{
	if (m_bPicking && pvMousePos)
		*pvMousePos = m_vPickingMousePos;
	return m_bPicking;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
Controller *SNPhysicsMouseInteraction::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SNPhysicsMouseInteraction::InitFunction()
{
	// Call base implementation
	SceneNode::InitFunction();

	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNPhysicsMouseInteraction::OnActivate(bool bActivate)
{
	// Call the base implementation
	SceneNode::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNPhysicsMouseInteraction::OnUpdate()
{
	// Get the used camera and check if input is active
	SNCamera *pCamera = SNCamera::GetCamera();
	if (pCamera && m_pController->GetActive()) {
		// Get the PL physics world this scene node is in
		SceneContainer *pContainer = GetContainer();
		while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld"))
			pContainer = pContainer->GetContainer();
		if (pContainer) {
			SCPhysicsWorld *pPhysicsWorldContainer = static_cast<SCPhysicsWorld*>(pContainer);
			World		   *pPhysicsWorld		   = pPhysicsWorldContainer->GetWorld();
			if (pPhysicsWorld) {
				// Get the physics force line
				SceneNode *pPhysicsForceLine = m_cForceLineHandler.GetElement();
				if (!pPhysicsForceLine) {
					// Get the physics force line once by name and store it within a scene node handler
					// (the scene node name changes, but we don't need to care about this when using handlers :)
					pPhysicsForceLine = GetContainer()->GetByName(m_sForceLineName);
					m_cForceLineHandler.SetElement(pPhysicsForceLine);
				}

				// On picking end
				if ( (!m_pController->Pickup.IsPressed() && m_bPicking) || (!m_bPicking && m_pPickedPhysicsBody)) {
					// Reset picking data
					if (!m_pController->Pickup.IsPressed()) {
						m_bPicking			 = false;
						m_pPickedPhysicsBody = nullptr;
						m_fPickedDistance	 = 0.0f;
						m_vAttachmentPoint	 = Vector3::Zero;
					}

				// On picking start
				} else if (m_pController->Pickup.IsPressed() && !m_bPicking) {
					// Check whether or not there's an application instance
					if (CoreApplication::GetApplication() && CoreApplication::GetApplication()->IsInstanceOf("PLCore::FrontendApplication")) {
						// Get the frontend instance
						Frontend &cFrontend = static_cast<FrontendApplication*>(CoreApplication::GetApplication())->GetFrontend();

						// Get current mouse cursor position inside the widget
						if (cFrontend.IsMouseOver()) {
							m_vPickingMousePos.x = cFrontend.GetMousePositionX();
							m_vPickingMousePos.y = cFrontend.GetMousePositionY();

							// Get the renderer
							const Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

							// Calculate the clip space to object space matrix
							Matrix4x4 mClipSpaceToObjectSpace = pCamera->GetProjectionMatrix(cRenderer.GetViewport());
							mClipSpaceToObjectSpace *= pCamera->GetViewMatrix();
							mClipSpaceToObjectSpace.Invert();

							// Get current mouse position in world coordinates relative to the picked point on the body
							Vector3 v2DPos(static_cast<float>(m_vPickingMousePos.x), static_cast<float>(m_vPickingMousePos.y), 0.0001f);
							Vector3 vV0 = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cRenderer.GetViewport());
							v2DPos.z = 0.9999f;
							Vector3 vV1 = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cRenderer.GetViewport());

							// Transform the two vectors into the physics world scene container
							pContainer = pCamera->GetContainer();
							while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld")) {
								// Transform the vectors
								vV0 = pContainer->GetTransform().GetMatrix()*vV0;
								vV1 = pContainer->GetTransform().GetMatrix()*vV1;

								// Next parent container
								pContainer = pContainer->GetContainer();
							}

							// Set correct start and end vector
							const float fDistance = MaxPickingRange ? MaxPickingRange : 10000.0f;
							const Vector3 vNormal = (vV1 - vV0).Normalize();
							vV1 = vV0 + vNormal*fDistance;

							// Perform physics ray cast
							Sensor *pSensor = pPhysicsWorld->CreateSensorRaycast(vV0, vV1, Sensor::ClosestBody);
							if (pSensor) {
								// Perform an immediate check
								if (pSensor->Check()) {
									// Get the closest hit physics body
									const Sensor::BodyInfo *pBodyInfo = pSensor->GetClosestBody();
									m_pPickedPhysicsBody = pBodyInfo ? pBodyInfo->pBody : nullptr;
									if (m_pPickedPhysicsBody && m_pPickedPhysicsBody->GetMass()) {
										// Calculate the attachment point in world space
										m_fPickedDistance = pBodyInfo->fDistance*fDistance;
										Vector3 vAttachmentPoint = vNormal*m_fPickedDistance;

										// Target in range?
										if (!MaxPickingRange || vAttachmentPoint.GetLength() < MaxPickingRange) {
											// Get the attachment point position
											vAttachmentPoint += vV0;

											// We are now in mouse picking mode
											m_bPicking = true;

											// Get the attachment point in local physics body space
											Matrix3x4 mTrans;
											m_pPickedPhysicsBody->GetTransformMatrix(mTrans);
											m_vAttachmentPoint = mTrans.GetInverted()*vAttachmentPoint;
										}
									}
								} else {
									// We do not allow picking of static physics bodies
									m_pPickedPhysicsBody = nullptr;
								}

								// Cleanup
								delete pSensor;
							}
						}
					}

				// On picking
				} else if (m_pController->Pickup.IsPressed() && m_bPicking && m_pPickedPhysicsBody) {
					// Check whether or not there's an application instance
					if (CoreApplication::GetApplication() && CoreApplication::GetApplication()->IsInstanceOf("PLCore::FrontendApplication")) {
						// Get the frontend instance
						Frontend &cFrontend = static_cast<FrontendApplication*>(CoreApplication::GetApplication())->GetFrontend();

						// Get current mouse cursor position inside the widget
						if (cFrontend.IsMouseOver()) {
							m_vPickingMousePos.x = cFrontend.GetMousePositionX();
							m_vPickingMousePos.y = cFrontend.GetMousePositionY();

							// Get the renderer
							const Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

							// Calculate the clip space to object space matrix
							Matrix4x4 mClipSpaceToObjectSpace = pCamera->GetProjectionMatrix(cRenderer.GetViewport());
							mClipSpaceToObjectSpace *= pCamera->GetViewMatrix();
							mClipSpaceToObjectSpace.Invert();

							// Get current mouse position in world coordinates relative to the picked point on the body
							Vector3 v2DPos(static_cast<float>(m_vPickingMousePos.x), static_cast<float>(m_vPickingMousePos.y), 0.0001f);
							Vector3 vV0 = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cRenderer.GetViewport());
							v2DPos.z = 0.9999f;
							Vector3 vV1 = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cRenderer.GetViewport());
							Vector3 vVLocal0 = vV0;

							// Transform the two vectors into the physics world scene container
							pContainer = pCamera->GetContainer();
							while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld")) {
								// Transform the vectors
								vV0 = pContainer->GetTransform().GetMatrix()*vV0;
								vV1 = pContainer->GetTransform().GetMatrix()*vV1;

								// Next parent container
								pContainer = pContainer->GetContainer();
							}

							// Set correct start and end vector
							const float fDistance = MaxPickingRange ? MaxPickingRange : 10000.0f;
							const Vector3 vNormal = (vV1 - vV0).Normalize();
							vV1 = vV0 + vNormal*fDistance;

							// Get the attachment point in world space
							Matrix3x4 mTrans;
							m_pPickedPhysicsBody->GetTransformMatrix(mTrans);
							Vector3 vAttachmentPoint = mTrans*m_vAttachmentPoint;

							// Get current point line
							const Vector3 vP = vV0 + vNormal*m_fPickedDistance;

							// Get the mass of the physics body
							const float fMass = m_pPickedPhysicsBody->GetMass();

							// Calculate the picking force
							Vector3 vPickedForce = vP - vAttachmentPoint;

							// Increase/decrease the picking force
							if (m_pController->IncreaseForce.IsPressed())
								vPickedForce *= 4;
							else if (m_pController->DecreaseForce.IsPressed())
								vPickedForce /= 4;

							// Kick the target away
							if (m_pController->Throw.IsPressed()) {
								m_bPicking = false;

								// Add 'throw' force
								float fThrowForceFactor = 1.0f;
								if (m_pController->IncreaseForce.IsPressed())
									fThrowForceFactor *= 2;
								else if (m_pController->DecreaseForce.IsPressed())
									vPickedForce *= 0.5f;
								const Vector3 vDir = (vV1 - pCamera->GetTransform().GetPosition()).Normalize();
								vPickedForce += vDir*ThrowForce*fThrowForceFactor;
							}

							// The mouse wheel can be used to push/pull the target
							const float fWheelDelta = m_pController->PushPull.GetValue();
							if (fWheelDelta) {
								// Do we need to take the current time difference into account?
								m_fPickedDistance += m_pController->PushPull.IsValueRelative() ? fWheelDelta : fWheelDelta*Timing::GetInstance()->GetTimeDifference();

								// Check distance
								const float fLength = (vNormal*m_fPickedDistance).GetLength();
								if (MaxPickingRange && fLength > MaxPickingRange) {
									const float fFactor = (fLength - MaxPickingRange)/MaxPickingRange;
									m_fPickedDistance -= m_fPickedDistance*fFactor;
								}
								if (m_fPickedDistance < 0.0f)
									m_fPickedDistance = 0.0f;
							}

							// Update the physics force line
							if (pPhysicsForceLine) {
								// Get the scene container the camera is in
								SceneContainer *pCameraContainer = pCamera->GetContainer();
								if (pCameraContainer) {
									// Setup the physics force line parent scene container
									pPhysicsForceLine->SetContainer(*pCameraContainer);

									// First, create the stack
									Stack<SceneContainer*> lstStack;
									while (pCameraContainer && pPhysicsWorldContainer != pCameraContainer) {
										// Push the container on the stack
										lstStack.Push(pCameraContainer);

										// Go into the next container, please
										pCameraContainer = pCameraContainer->GetContainer();
									}

									// Transform the attachment point into the physics force line parent container space
									Vector3 vLocalAttachmentPoint = vAttachmentPoint;
									while (lstStack.GetNumOfElements()) {
										// Get container
										pCameraContainer = lstStack.Top();
										lstStack.Pop();

										// Transform
										vLocalAttachmentPoint *= pCameraContainer->GetTransform().GetInverseMatrix();
									}

									// Update the start and end line points
									pPhysicsForceLine->GetTransform().SetPosition((vVLocal0+vLocalAttachmentPoint)*0.5f);
									vVLocal0 -= pPhysicsForceLine->GetTransform().GetPosition();
									vLocalAttachmentPoint -= pPhysicsForceLine->GetTransform().GetPosition();
									pPhysicsForceLine->SetAttribute("StartPosition", vVLocal0.ToString());
									pPhysicsForceLine->SetAttribute("EndPosition",   vLocalAttachmentPoint.ToString());
									pPhysicsForceLine->SetActive(true);
								}
							}

							// Get the linear velocity of the physics body
							Vector3 vLinearVelocity;
							m_pPickedPhysicsBody->GetLinearVelocity(vLinearVelocity);

							// Get the angular velocity of the physics body
							Vector3 vAngularVelocity;
							m_pPickedPhysicsBody->GetAngularVelocity(vAngularVelocity);

							// Calculate damp force
							const Vector3 vDampForce = vLinearVelocity*fMass*10.0f;

							// Calculate force
							const Vector3 vForce = vPickedForce*fMass*100.0f - vDampForce;

							// Calculate torque
							vAttachmentPoint = mTrans.RotateVector(m_vAttachmentPoint);
							const Vector3 vTorque = vAttachmentPoint.CrossProduct(vForce);

							// Add picking force and torque
							m_pPickedPhysicsBody->SetForce(vForce);
							m_pPickedPhysicsBody->SetTorque(vTorque/10);
						}
					}
				}

				// Deactivate the physics force line
				if (!m_bPicking && pPhysicsForceLine)
					pPhysicsForceLine->SetActive(false);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
