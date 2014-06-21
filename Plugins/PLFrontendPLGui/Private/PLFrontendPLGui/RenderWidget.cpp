/*********************************************************\
 *  File: RenderWidget.cpp                               *
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
#include <PLGui/Gui/Base/Keys.h>
#include <PLRenderer/Renderer/Surface.h>
#include "PLFrontendPLGui/RenderWidget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;
using namespace PLRenderer;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(RenderWidget)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RenderWidget::RenderWidget(Renderer &cRenderer, PLGui::Widget *pParent, const DisplayMode *pDisplayMode) : PLGui::Widget(pParent), WindowConnection(cRenderer, *this),
	SlotOnTimer(this),
	m_cTimer(*GetGui()),
	m_bBackupAvailable(false)
{
	// There's no need to have a widget background because we're render into it
	SetBackgroundColor(Color4::Transparent);

	// Initialize the window connection
	Initialize(pDisplayMode, false);

	// Connect to timer
	m_cTimer.EventFire.Connect(SlotOnTimer);

	// Start timer - An interval of 10 milliseconds should be enough
	// [TODO] PLGui-timers: Currently, this timer will pollute the PLGui message system - the message processing will never stop because while processing the messages, the threaded PLGui timer fires again...
//	m_cTimer.Start(10);
}

/**
*  @brief
*    Destructor
*/
RenderWidget::~RenderWidget()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::WidgetFunctions functions    ]
//[-------------------------------------------------------]
void RenderWidget::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	Widget::OnDraw(cGraphics);

	// Get the renderer surface
	Surface *pSurface = GetSurface();
	if (pSurface) {
		// Draw the renderer surface
		pSurface->Draw();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual WindowConnection functions          ]
//[-------------------------------------------------------]
void RenderWidget::OnDisplayMode()
{
	// Set the size of the widget depending on the current set display mode
	SetSize(GetDisplayMode().vSize);

	// Call base implementation
	WindowConnection::OnDisplayMode();
}

void RenderWidget::OnFullscreenMode()
{
	// Widget mode backup stuff
	if (IsFullscreen() && !m_bBackupAvailable) {
		m_vBackupSize = GetSize();
		if (m_vBackupSize.x || m_vBackupSize.y) {
			m_bBackupAvailable = true;
			m_vBackupPos	   = GetPos();
		}
	} else {
		if (!IsFullscreen() && m_bBackupAvailable) {
			SetPos (m_vBackupPos);
			SetSize(m_vBackupSize);
			m_bBackupAvailable = false;
		}
	}

	// Is the frame currently in fullscreen mode or not?
	if (IsFullscreen()) {
		// Place the widget at the left top side of the screen
		SetPos(Vector2i::Zero);

		// Set the size of the widget depending on the current set display mode
		SetSize(GetDisplayMode().vSize);
	}

	// Call base implementation
	WindowConnection::OnFullscreenMode();
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
RenderWidget::RenderWidget(const RenderWidget &cSource) : PLGui::Widget(cSource.GetParent()), WindowConnection(*cSource.GetRenderer(), *this),
	SlotOnTimer(this),
	m_cTimer(*GetGui()),
	m_bBackupAvailable(false)
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy operator
*/
RenderWidget &RenderWidget::operator =(const RenderWidget &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Timer callback
*/
void RenderWidget::OnTimer()
{
	// Ask PLGui politely to update (and repaint) the widget
	Redraw();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
