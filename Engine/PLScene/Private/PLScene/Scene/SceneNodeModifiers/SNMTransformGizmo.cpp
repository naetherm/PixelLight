/*********************************************************\
 *  File: SNMTransformGizmo.cpp                          *
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
#include <PLMath/Rectangle.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/Renderer/Renderer.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMTransformGizmo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMTransformGizmo)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the current selected axis (see ESelected flags)
*/
uint32 SNMTransformGizmo::GetSelected() const
{
	return m_nSelected;
}

/**
*  @brief
*    Sets the current selected axis (see ESelected flags)
*/
void SNMTransformGizmo::SetSelected(uint32 nSelected)
{
	m_nSelected = nSelected;
}

/**
*  @brief
*    Returns whether the transform gizmo is currently in transform mode or not
*/
bool SNMTransformGizmo::IsTransformMode() const
{
	return m_bTransform;
}

/**
*  @brief
*    Sets whether the transform gizmo is currently in transform mode or not
*/
void SNMTransformGizmo::SetTransformMode(bool bTransformMode)
{
	m_bTransform = bTransformMode;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMTransformGizmo::SNMTransformGizmo(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	DistanceFromCamera(this),
	SlotOnDrawTransparent(this),
	SlotOnUpdate(this),
	m_nSelected(0),
	m_bTransform(false)
{
	// Set draw function flags
	cSceneNode.SetDrawFunctionFlags(static_cast<uint8>(cSceneNode.GetDrawFunctionFlags() | SceneNode::UseDrawTransparent));
}

/**
*  @brief
*    Destructor
*/
SNMTransformGizmo::~SNMTransformGizmo()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMTransformGizmo::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			GetSceneNode().SignalDrawTransparent.Connect(SlotOnDrawTransparent);
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		} else {
			GetSceneNode().SignalDrawTransparent.Disconnect(SlotOnDrawTransparent);
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called on scene node transparent draw
*/
void SNMTransformGizmo::OnDrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Calculate the current transform gizmo matrix
	if (pVisNode) {
		// Get translation (rotation & scale has no influence on the transform gizmo)
		Vector3 vTranslation = pVisNode->GetWorldMatrix().GetTranslation();

		// Transform translation into view space (view space is our friend, we really like the view space... it's so, so simple...)
		vTranslation *= pVisNode->GetViewMatrix();

		// Get camera (at origin in view space) to scene node normalized direction
		const Vector3 vDirection = vTranslation.Normalize();

		// Set new translation in view space
		vTranslation = vDirection*DistanceFromCamera.Get();

		// Bring translation back into world space
		vTranslation *= pVisNode->GetViewMatrix().GetInverted();

		// Set translation matrix
		Matrix4x4 mTranslation;
		mTranslation.SetTranslationMatrix(vTranslation);

		{ // Calculate a known projection matrix
		  // -> Lookout! It's not safe to directly use the given projection matrix by writing
		  //   "m_mObjectSpaceToClipSpace = pVisNode->GetProjectionMatrix();"
		  // -> We can't really know the type of this given matrix
		  //    (should be a projection matrix, should... but we can't be absolutely ensure)
		  // -> We can't be sure whether or not e.g. "1.0" was used as z far parameter resulting that
		  //    our transform gizmo will be clipped away by the z far plane
		  // -> So, to be on the safe side, we use our own known projection matrix for the transform gizmo

			// Projection matrix parameters
				   const float FOV        = SNCamera::GetCamera() ? SNCamera::GetCamera()->GetFOV() : 45.0f;
			static const float Aspect     = 1.0f;
			static const float ZNear      = 0.01f;
			static const float ZFarOffset = 50.0f;

			// Calculate the current aspect radio
			const float fAspectRadio = cRenderer.GetViewport().GetWidth()/cRenderer.GetViewport().GetHeight()*Aspect;

			// Now that we have all parameters, calculate our known projection matrix
			m_mObjectSpaceToClipSpace.PerspectiveFov(static_cast<float>(FOV*Math::DegToRad), fAspectRadio, ZNear, DistanceFromCamera.Get() + ZFarOffset);
		}

		// Apply the given view matrix
		m_mObjectSpaceToClipSpace *= pVisNode->GetViewMatrix();

		// Apply our calculated translation matrix
		m_mObjectSpaceToClipSpace *= mTranslation;
	}

	// Update gizmo
	if (!m_bTransform)
		UpdateSelection(cRenderer, *pVisNode);

	// Draw gizmo
	DrawGizmo(cRenderer, pVisNode);
}

/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMTransformGizmo::OnUpdate()
{
	// Update gizmo
	if (m_bTransform)
		PerformTransform();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
