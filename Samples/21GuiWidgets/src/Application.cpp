/*********************************************************\
 *  File: Application.cpp                                *
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
#include <PLCore/File/Url.h>
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/Tools/LoadableManager.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Screen.h>
#include "Application.h"
#include "TestWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGui;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application, "", PLGui::GuiApplication, "Application class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(Application)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application()
{
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::GuiApplication functions     ]
//[-------------------------------------------------------]
void Application::OnCreateMainWindow()
{
	// [sbusch]
//	String sDir = Url(sFilename).CutFilename();
//	System::GetInstance()->SetCurrentDir(sDir);
//	LoadableManager::GetInstance()->AddBaseDir(sDir);

	// Get GUI
	Gui *pGui = Gui::GetSystemGui();

	// List screens
	System::GetInstance()->GetConsole().Print("Screens:\n");
	for (uint32 i=0; i<pGui->GetScreens().GetNumOfElements(); i++) {
		Screen *pScreen = pGui->GetScreens().Get(i);
		String sName	= pScreen->GetName();
		Vector2i vSize	= pScreen->GetSize();
		Vector2i vPos	= pScreen->GetPos();
		bool bDefault	= pScreen->IsDefault();
		System::GetInstance()->GetConsole().Print(
			String("- ") + sName +
			" [" + vSize.x + 'x' + vSize.y +']' +
			" (" + vPos .x + ',' + vPos .y + ')' +
			(bDefault ? " (*)" : "") +
			'\n');
	}

	// Create window
	TestWindow *pWindow = new TestWindow();
//	pWindow->SetFullscreenEnabled(true);
	pWindow->SetPos(Vector2i(0, 0));
	pWindow->SetSize(Vector2i(640, 480));
	pWindow->Center(CenterScreen);
	pWindow->SetVisible(true);

	// Add modifiers
	pWindow->AddModifier("PLGui::ModClose", "ExitApplication=1");
//	pWindow->AddModifier("PLGui::ModBlend", "");

	// Set main window
	SetMainWindow(pWindow);
}
