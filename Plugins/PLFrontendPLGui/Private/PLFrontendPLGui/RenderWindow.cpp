/*********************************************************\
 *  File: RenderWindow.cpp                               *
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
#include "PLFrontendPLGui/RenderWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;
using namespace PLRenderer;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(RenderWindow)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RenderWindow::RenderWindow(Renderer &cRenderer, Widget *pParent, const DisplayMode *pDisplayMode, bool bFullscreen) : PLGui::Window(pParent), WindowConnection(cRenderer, *this, this),
	SlotOnTimer(this),
	m_cTimer(*GetGui()),
	m_bBackupAvailable(false)
{
	// There's no need to have a widget background because we're render into it
	GetContentWidget()->SetBackgroundColor(Color4::Transparent);

	// By default, it's allowed to switch windows into fullscreen mode
	SetToggleFullscreenMode(true);

	// By default, it's allowed to use Alt-Tab
	SetFullscreenAltTab(true);

	// Initialize the window connection
	Initialize(pDisplayMode, bFullscreen);

	// Set widget into fullscreen state?
	if (bFullscreen)
		SetWindowState(StateFullscreen);

	// Connect to timer
	m_cTimer.EventFire.Connect(SlotOnTimer);

	// Start timer - An interval of 10 milliseconds should be enough
	// [TODO] PLGui-timers: Currently, this timer will pollute the PLGui message system - the message processing will never stop because while processing the messages, the threaded PLGui timer fires again...
	// m_cTimer.Start(10);
}

/**
*  @brief
*    Destructor
*/
RenderWindow::~RenderWindow()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::WidgetFunctions functions    ]
//[-------------------------------------------------------]
void RenderWindow::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	Window::OnDraw(cGraphics);

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
void RenderWindow::OnDisplayMode()
{
	// Set the size of the window depending on the current set display mode
	SetSize(GetDisplayMode().vSize);

	// Call base implementation
	WindowConnection::OnDisplayMode();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
RenderWindow::RenderWindow(const RenderWindow &cSource) : PLGui::Window(cSource.GetParent()), WindowConnection(*cSource.GetRenderer(), *GetContentWidget()),
	SlotOnTimer(this),
	m_cTimer(*GetGui()),
	m_bBackupAvailable(false)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
RenderWindow &RenderWindow::operator =(const RenderWindow &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Timer callback
*/
void RenderWindow::OnTimer()
{
	// Ask PLGui politely to update (and repaint) the widget
	Redraw();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
