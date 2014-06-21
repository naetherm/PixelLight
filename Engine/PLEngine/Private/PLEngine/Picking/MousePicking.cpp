/*********************************************************\
 *  File: MousePicking.cpp                               *
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
#include <PLCore/Frontend/Frontend.h>
#include <PLMath/Vector2i.h>
#include <PLMath/Rectangle.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLEngine/Picking/MousePicking.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MousePicking::MousePicking(Frontend &cFrontend, SNCamera *pCamera) :
	EventHandlerCameraDestroy(&MousePicking::OnCameraDestroy, this),
	m_pFrontend(&cFrontend),
	m_pCamera(pCamera)
{
}

/**
*  @brief
*    Destructor
*/
MousePicking::~MousePicking()
{
}

/**
*  @brief
*    Returns the frontend instance used to determine the mouse position
*/
Frontend &MousePicking::GetFrontend() const
{
	return *m_pFrontend;
}

/**
*  @brief
*    Returns the camera to perform the picking with
*/
SNCamera *MousePicking::GetCamera() const
{
	return m_pCamera;
}

/**
*  @brief
*    Sets the camera to perform the picking with
*/
void MousePicking::SetCamera(SNCamera *pCamera)
{
	// New camera?
	if (m_pCamera != pCamera) {
		// Disconnect event handler
		if (m_pCamera)
			m_pCamera->SignalDestroy.Disconnect(EventHandlerCameraDestroy);

		// Set new camera
		m_pCamera = pCamera;

		// Connect event handler
		if (m_pCamera)
			m_pCamera->SignalDestroy.Connect(EventHandlerCameraDestroy);
	}
}

/**
*  @brief
*    Performs picking by using the current camera and current mouse position within the widget
*/
bool MousePicking::PerformMousePicking(PickingResult &cPickingResult, float fMaxDistance, Cull::Enum nCull)
{
	// Is there a camera?
	if (m_pCamera) {
		// Get current mouse cursor position inside the frontend
		const Vector2i vMousePos(m_pFrontend->GetMousePositionX(), m_pFrontend->GetMousePositionY());

		// Return the picked scene node
		return PerformMousePicking(cPickingResult, vMousePos, fMaxDistance, nCull);
	}

	// Error!
	return false;
}

/**
*  @brief
*    Performs picking by using the current camera and the given mouse position
*/
bool MousePicking::PerformMousePicking(PickingResult &cPickingResult, const Vector2i &vMousePos, float fMaxDistance, Cull::Enum nCull)
{
	// Don't do picking if mouse is outside the frontend - or there's no widget and/or camera at all
	if (m_pCamera && m_pCamera->GetContainer() &&
		vMousePos.x >= 0 && vMousePos.x < static_cast<int>(m_pFrontend->GetWidth()) &&
		vMousePos.y >= 0 && vMousePos.y < static_cast<int>(m_pFrontend->GetHeight())) {
		// Trace line from the camera position to the mouse world position

		// Get the viewport rectangle
		Rectangle cViewport(0.0f, 0.0f, static_cast<float>(m_pFrontend->GetWidth()), static_cast<float>(m_pFrontend->GetHeight()));

		// Calculate the clip space to object space matrix
		Matrix4x4 mClipSpaceToObjectSpace = m_pCamera->GetProjectionMatrix(cViewport);
		mClipSpaceToObjectSpace *= m_pCamera->GetViewMatrix();
		mClipSpaceToObjectSpace.Invert();

		// Get picking line start and end
		Vector3 v2DPos(static_cast<float>(vMousePos.x), static_cast<float>(vMousePos.y), 0.0f);
		Vector3 vLineStartPos = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cViewport);
		v2DPos.z = 1.0f;
		Vector3 vLineEndPos = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cViewport);

		// Is there a maximum picking distance?
		if (fMaxDistance >= 0.0f)
			vLineEndPos = vLineStartPos + (vLineEndPos-vLineStartPos).Normalize()*fMaxDistance;

		// Perform picking and return the result
		return PerformPicking(cPickingResult, *m_pCamera->GetContainer(), vLineStartPos, vLineEndPos, nCull);
	}

	// Error!
	return false;
}

/**
*  @brief
*    Called when a camera was destroyed
*/
void MousePicking::OnCameraDestroy()
{
	m_pCamera = nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
