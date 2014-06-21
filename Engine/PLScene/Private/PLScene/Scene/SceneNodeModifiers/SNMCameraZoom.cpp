/*********************************************************\
 *  File: SNMCameraZoom.cpp                              *
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
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMCameraZoom.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMCameraZoom)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMCameraZoom::SNMCameraZoom(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode),
	ZoomDegree(this),
	ZoomFactor(this),
	ZoomSpeed(this),
	SlotOnUpdate(this),
	m_fOriginalFOV(cSceneNode.IsCamera() ? static_cast<SNCamera&>(cSceneNode).GetFOV() : 0.0f),
	m_fCurrentZoomFactor(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMCameraZoom::~SNMCameraZoom()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMCameraZoom::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMCameraZoom::OnUpdate()
{
	// Get the scene node
	SceneNode &cSceneNode = GetSceneNode();
	if (cSceneNode.IsCamera()) {
		// Smooth camera zoom?
		if (ZoomSpeed) {
			// Get the current time difference
			const float fTimeDiff = Timing::GetInstance()->GetTimeDifference()*ZoomSpeed;

			// Update the current zoom factor
			if (m_fCurrentZoomFactor < ZoomFactor) {
				// Increase the current zoom factor
				m_fCurrentZoomFactor += fTimeDiff;
				if (m_fCurrentZoomFactor > ZoomFactor)
					m_fCurrentZoomFactor = ZoomFactor;
			} else {
				// Decrease the current zoom factor
				m_fCurrentZoomFactor -= fTimeDiff;
				if (m_fCurrentZoomFactor < ZoomFactor)
					m_fCurrentZoomFactor = ZoomFactor;
			}
		} else {
			// Set to target zoom factor at once
			m_fCurrentZoomFactor = ZoomFactor;
		}

		// Set the current FOV of the owner camera
		static_cast<SNCamera&>(cSceneNode).SetFOV(m_fOriginalFOV - m_fCurrentZoomFactor*ZoomDegree);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
