/*********************************************************\
 *  File: GuiApplication.cpp                            *
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
#include "PLGui/Gui/Gui.h"
#include "PLGui/Widgets/Windows/Window.h"
#include "PLGui/Application/GuiApplication.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(GuiApplication)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiApplication::GuiApplication(const String &sGuiFilename) : CoreApplication(),
	EventHandlerOnDestroy(&GuiApplication::OnDestroyMainWindow, this),
	m_pMainWindow(nullptr)
{
	// Set application title
	SetTitle("PixelLight GUI application");
}

/**
*  @brief
*    Destructor
*/
GuiApplication::~GuiApplication()
{
}

/**
*  @brief
*    Get main window
*/
Widget *GuiApplication::GetMainWindow() const
{
	// Return pointer to main window
	return m_pMainWindow;
}

/**
*  @brief
*    Set main window
*/
void GuiApplication::SetMainWindow(Widget *pMainWindow)
{
	// Disconnect event handler
	if (m_pMainWindow)
		m_pMainWindow->SignalDestroy.Disconnect(EventHandlerOnDestroy);

	// Set pointer to main window
	m_pMainWindow = pMainWindow;

	// Connect event handler
	if (m_pMainWindow)
		m_pMainWindow->SignalDestroy.Connect(EventHandlerOnDestroy);
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractLifecycle functions ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialization function that is called prior to OnInit()
*/
bool GuiApplication::OnStart()
{
	// Call base implementation
	if (CoreApplication::OnStart()) {
		// Create main window
		OnCreateMainWindow();
		if (!m_bRunning) return false;

		// Done
		return true;
	}

	// Error
	return false;
}

/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*/
void GuiApplication::OnStop()
{
	// Call base implementation
	CoreApplication::OnStop();

	// Shut down system GUI
	Gui::GetSystemGui()->Shutdown();
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::CoreApplication functions   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Main function
*/
void GuiApplication::Main()
{
	// Run main loop
	Gui *pGui = Gui::GetSystemGui();
	while (pGui->IsActive() && m_bRunning) {
		// Get GUI messages
		pGui->ProcessMessages();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual GuiApplication functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function that is called to open the application's main window
*/
void GuiApplication::OnCreateMainWindow()
{
	// [TODO] Load GUI from file if filename is provided

	// Create standard window
	Window *pWindow = new Window();
	pWindow->AddModifier("PLGui::ModClose", "ExitApplication=1");
	pWindow->SetTitle(GetTitle());
	pWindow->SetSize(Vector2i(640, 480));
	pWindow->SetVisible(true);

	// Set main window
	SetMainWindow(pWindow);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a window was destroyed
*/
void GuiApplication::OnDestroyMainWindow()
{
	// We lost our main window :/
	m_pMainWindow = nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
