/*********************************************************\
 *  File: Application71.cpp                              *
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
#include <Rocket/Core.h>
#include <PLlibRocket/libRocketAdapter.h>
#include <PLlibRocket/PLGuiMessageFilterRocket.h>
#include <PLGui/Gui/Gui.h>
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Controls/Control.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Compositing/SceneRenderer.h>
#include "Application71.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application71, "", PLEngine::EngineApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
	// Slots
	pl_slot_1_metadata(OnControl,	PLInput::Control&,	"Called when a control event has occurred, occurred control as first parameter",	"")
pl_class_metadata_end(Application71)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application71::Application71(Frontend &cFrontend) : EngineApplication(cFrontend),
	SlotOnControl(this),
	m_pRocketAdapter(nullptr),
	m_pPLGuiMessageFilterRocket(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Application71::~Application71()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control event has occurred
*/
void Application71::OnControl(Control &cControl)
{
	// Check whether the escape key was pressed
	if (cControl.GetType() == ControlButton && cControl.GetName() == "KeyboardEscape")
		Exit(0); // Shut down the application
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application71::OnInit()
{
	// This method is call by the application framework when application should initialize itself.
	// At this point, the core components of the application are ready to be used.

	// Call base implementation
	EngineApplication::OnInit();

	// Get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		// Create the libRocket adapter instance
		m_pRocketAdapter = new PLlibRocket::libRocketAdapter(pSceneContext->GetRendererContext());
		Rocket::Core::Context *pRocketContext = m_pRocketAdapter->GetRocketContext();
		if (pRocketContext) {
			// Set new libRocket context dimension
			OnSize();

			// Add message filter that feeds PLGui messages from the content widget of the main window into libRocket
			m_pPLGuiMessageFilterRocket = new PLlibRocket::PLGuiMessageFilterRocket(*pRocketContext);
			PLGui::Gui::GetSystemGui()->AddMessageFilter(m_pPLGuiMessageFilterRocket);

			// Get the application directory - look out! We can't just assume that the current work directory
			// is the same as were the executable is in. If we do so, for example starting the application by using
			// a MS Windows menu shortcut will not work... and just changing the current work directory by using
			// "PLCore::System::GetInstance()->SetCurrentDir(GetApplicationContext().GetAppDirectory())" isn't the polite way...
			String sAppDirectory = GetApplicationContext().GetAppDirectory();
			if (sAppDirectory.GetLength())
				sAppDirectory += '/';	// Do only add '/' when the application directory is not an empty string

			// Load the fonts
			Rocket::Core::FontDatabase::LoadFontFace((sAppDirectory + "Data/libRocket/Delicious-Roman.otf").GetUTF8());
			Rocket::Core::FontDatabase::LoadFontFace((sAppDirectory + "Data/libRocket/Delicious-Bold.otf").GetUTF8());
			Rocket::Core::FontDatabase::LoadFontFace((sAppDirectory + "Data/libRocket/Delicious-Italic.otf").GetUTF8());
			Rocket::Core::FontDatabase::LoadFontFace((sAppDirectory + "Data/libRocket/Delicious-BoldItalic.otf").GetUTF8());

			{ // Load the mouse cursor and release the caller's reference
				Rocket::Core::ElementDocument *pElementDocumentCursor = pRocketContext->LoadMouseCursor((sAppDirectory + "Data/libRocket/cursor.rml").GetUTF8());
				if (pElementDocumentCursor)
					pElementDocumentCursor->RemoveReference();
			}

			{ // Load the document and release the caller's reference
				Rocket::Core::ElementDocument *pElementDocumentDocument = pRocketContext->LoadDocument((sAppDirectory + "Data/libRocket/demo.rml").GetUTF8());
				if (pElementDocumentDocument) {
					pElementDocumentDocument->Show();
					pElementDocumentDocument->RemoveReference();
				}
			}

			// Add libRocket scene renderer pass instance
			SceneRenderer *pSceneRenderer = GetSceneRendererTool().GetSceneRenderer();
			if (pSceneRenderer) {
				PLlibRocket::SRPlibRocket *pSRPlibRocket = m_pRocketAdapter->CreateSRPlibRocketInstance();
				if (pSRPlibRocket)
					pSceneRenderer->Add(*reinterpret_cast<SceneRendererPass*>(pSRPlibRocket));
			}
		}
	}

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	GetSceneRendererTool().SetPassAttribute("Begin", "ColorClear", "0.5 0.5 0.5 0");
}

void Application71::OnDeInit()
{
	// Remove message filter that feeds PLGui messages into libRocket
	if (m_pPLGuiMessageFilterRocket) {
		// PLGui destroys automatically the "m_pPLGuiMessageFilterRocket"-instance
		PLGui::Gui::GetSystemGui()->RemoveMessageFilter(m_pPLGuiMessageFilterRocket);
		m_pPLGuiMessageFilterRocket = nullptr;
	}

	// Destroy the libRocket adapter instance
	if (m_pRocketAdapter) {
		delete m_pRocketAdapter;
		m_pRocketAdapter = nullptr;
	}

	// Call base implementation
	EngineApplication::OnDeInit();
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::AbstractFrontend functions    ]
//[-------------------------------------------------------]
void Application71::OnSize()
{
	// Get the libRocket context
	Rocket::Core::Context *pRocketContext = m_pRocketAdapter ? m_pRocketAdapter->GetRocketContext() : nullptr;
	if (m_pRocketAdapter) {
		// Set new libRocket context dimension
		pRocketContext->SetDimensions(Rocket::Core::Vector2i(GetFrontend().GetWidth(), GetFrontend().GetHeight()));
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application71::OnCreateScene(SceneContainer &cContainer)
{
	// Create a camera
	SceneNode *pCamera = cContainer.Create("PLScene::SNCamera", "FreeCamera", "Position=\"1 2 -3\" Rotation=\"25 210 0\"");
	if (pCamera && pCamera->IsInstanceOf("PLScene::SNCamera")) {
		// Make this to our main scene camera
		SetCamera(reinterpret_cast<SNCamera*>(pCamera));
	}

	// Create the floor
	cContainer.Create("PLScene::SNMesh", "Floor", "Position=\"0 0 -5\" Scale=\"4 0.1 4\" Mesh=\"Default\"");

	// Setup scene surface painter
	SurfacePainter *pPainter = GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
		SPScene *pSPScene = static_cast<SPScene*>(pPainter);
		pSPScene->SetRootContainer(cContainer.GetContainer());
		pSPScene->SetSceneContainer(&cContainer);
	}

	// Set scene container
	SetScene(&cContainer);
}

void Application71::OnCreateInputController()
{
	// Call base implementation
	EngineApplication::OnCreateInputController();

	// Get virtual input controller
	Controller *pController = reinterpret_cast<Controller*>(GetInputController());
	if (pController)
		pController->SignalOnControl.Connect(SlotOnControl);
}
