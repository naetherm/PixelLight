/*********************************************************\
 *  File: PreviewWidget.cpp                              *
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
#include <PLGui/Gui/Gui.h>
#include <PLRenderer/Renderer/Surface.h>
#include <PLRenderer/SPPreview.h>
#include "PLFrontendPLGui/PreviewWidget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGui;
using namespace PLRenderer;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PreviewWidget)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PreviewWidget::PreviewWidget(Renderer &cRenderer, PLGui::Widget *pParent) : RenderWidget(cRenderer, pParent),
	m_bLeftMouseButtonPressed(false),
	m_bRightMouseButtonPressed(false)
{
	// Set widget mouse cursor
	Cursor *pCursor = GetGui()->GetCursorManager().GetCursor(CursorSize);
	if (pCursor)
		SetCursor(*pCursor);
}

/**
*  @brief
*    Destructor
*/
PreviewWidget::~PreviewWidget()
{
}

/**
*  @brief
*    Resets the view
*/
void PreviewWidget::ResetView()
{
	SPPreview *pPainter = static_cast<SPPreview*>(GetSurface()->GetPainter());
	if (pPainter)
		pPainter->SetCameraPosition(Vector3(0.5f, 0.5f, -0.5f));
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
PreviewWidget::PreviewWidget(const PreviewWidget &cSource) : RenderWidget(cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
PreviewWidget &PreviewWidget::operator =(const PreviewWidget &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private virtual PLGui::Widget functions               ]
//[-------------------------------------------------------]
void PreviewWidget::OnMouseMove(const Vector2i &vPos)
{
	// Is the left or right mouse button pressed?
	if (m_bLeftMouseButtonPressed || m_bRightMouseButtonPressed) {
		// Preview camera control
		SPPreview *pPainter = static_cast<SPPreview*>(GetSurface()->GetPainter());
		if (pPainter) {
			Vector3 vCameraPos = pPainter->GetCameraPosition();
			if (m_bLeftMouseButtonPressed) {
				vCameraPos.x += (m_vPreviousMousePosition.x-vPos.x)/50.0f;
				vCameraPos.y += (m_vPreviousMousePosition.y-vPos.y)/50.0f;
			} else if (m_bRightMouseButtonPressed) {
				vCameraPos.z += (m_vPreviousMousePosition.y-vPos.y)/40.0f;
				if (vCameraPos.z > -0.1f)
					vCameraPos.z = -0.1f;
			}
			pPainter->SetCameraPosition(vCameraPos);

			// Backup current mouse position
			m_vPreviousMousePosition = vPos;
		}
	}
}

void PreviewWidget::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Call base implementation
	RenderWidget::OnMouseButtonDown(nButton, vPos);

	// Check mouse button
	switch (nButton) {
		// Left mouse button
		case 0:
			m_bLeftMouseButtonPressed = true;

			// Backup current mouse position
			m_vPreviousMousePosition = vPos;
			break;

		// Right mouse button
		case 1:
			m_bRightMouseButtonPressed = true;

			// Backup current mouse position
			m_vPreviousMousePosition = vPos;
			break;
	}
}

void PreviewWidget::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Call base implementation
	RenderWidget::OnMouseButtonUp(nButton, vPos);

	// Check mouse button
	switch (nButton) {
		// Left mouse button
		case 0:
			m_bLeftMouseButtonPressed = false;
			break;

		// Right mouse button
		case 1:
			m_bRightMouseButtonPressed = false;
			break;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
