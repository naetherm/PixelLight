/*********************************************************\
 *  File: Application67.cpp                              *
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
#include <PLInput/Input/Controls/Button.h>
#include <PLInput/Input/Virtual/VirtualController.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include <PLScene/Compositing/SceneRendererManager.h>
#include "Application67.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application67, "", PLEngine::EngineApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
	// Slots
	pl_slot_1_metadata(OnControl,	PLInput::Control&,	"Called when a control event has occurred, occurred control as first parameter",	"")
pl_class_metadata_end(Application67)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application67::Application67(Frontend &cFrontend) : EngineApplication(cFrontend),
	SlotOnControl(this)
{
}

/**
*  @brief
*    Destructor
*/
Application67::~Application67()
{
}

/**
*  @brief
*    Restarts the game
*/
void Application67::Restart()
{
	// Get the scene container
	SceneContainer *pSceneContainer = GetScene();
	if (pSceneContainer) {
		// Clean up your scene container
		pSceneContainer->Clear();

		// Create the camera (we need the camera to control the post processing)
		SceneNode *pCamera = pSceneContainer->Create("PLScene::SNCamera", "FixedCamera");
		if (pCamera && pCamera->IsInstanceOf("PLScene::SNCamera")) {
			// Add a post process scene node modifier from the 'PLPostProcessEffects'-plugin
			pCamera->AddModifier("PLPostProcessEffects::SNMPostProcessOldFilm", "Flags='Inactive'");

			// Make this to our main scene camera
			SetCamera(reinterpret_cast<SNCamera*>(pCamera));
		}

		// Create an UFO from mars attacking the earth :D
		pSceneContainer->Create("SNUFO", "Ufo");

		// Create the gun (the player)
		pSceneContainer->Create("SNGun", "Gun", "Position=\"157 155\"");
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control event has occurred
*/
void Application67::OnControl(Control &cControl)
{
	// Is it a button and was it just hit?
	if (cControl.GetType() == ControlButton && reinterpret_cast<Button&>(cControl).IsHit()) {
		// Check whether the escape key was pressed
		if (cControl.GetName() == "KeyboardEscape") {
			// Shut down the application
			Exit(0);

		// Restart the game
		} else if (cControl.GetName() == "KeyboardR") {
			Restart();

		// Toggle post processing
		} else if (cControl.GetName() == "KeyboardP") {
			// Get the camera
			const SceneNode *pCamera = reinterpret_cast<SceneNode*>(GetCamera());
			if (pCamera) {
				// Loop through all available post process scene node modifiers
				bool			   bRenderToTexture = false;
				uint32			   nIndex			= 0;
				SceneNodeModifier *pModifier		= pCamera->GetModifier("PLCompositing::SNMPostProcess", nIndex);
				while (pModifier) {
					// Toggle the active state of the post process scene node modifier
					pModifier->SetActive(!pModifier->IsActive());
					if (pModifier->IsActive())
						bRenderToTexture = true;

					// Next modifier, please
					pModifier = pCamera->GetModifier("PLCompositing::SNMPostProcess", ++nIndex);
				}

				// Enable/disable render to texture for the post processing feature
				GetSceneRendererTool().SetPassAttribute("Begin", "Flags", bRenderToTexture ? "" : "Inactive");
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application67::OnCreateScene(SceneContainer &cContainer)
{
	// Create a scene container with our 'concrete sound scene' using the default sound API
	SceneNode *pSceneContainerNode = cContainer.Create("PLSound::SCSound", "SoundScene");
	if (pSceneContainerNode && pSceneContainerNode->IsInstanceOf("PLScene::SceneContainer")) {
		SceneContainer *pSceneContainer = static_cast<SceneContainer*>(pSceneContainerNode);

		// Protect this important container!
		pSceneContainer->SetProtected(true);

		// Populate the scene container
		// Setup scene surface painter
		SurfacePainter *pPainter = GetPainter();
		if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
			SPScene *pSPScene = static_cast<SPScene*>(pPainter);
			pSPScene->SetRootContainer(cContainer.GetContainer());
			pSPScene->SetSceneContainer(pSceneContainer);

			// Get the scene context
			SceneContext *pSceneContext = GetSceneContext();
			if (pSceneContext) {
				// Create us a scene renderer
				SceneRenderer *pSceneRenderer = pSceneContext->GetSceneRendererManager().Create("2DGame");
				if (pSceneRenderer) {
					// Add begin scene renderer pass
					pSceneRenderer->Create("PLCompositing::SRPBegin", "Begin", "TextureFormat=\"R8G8B8A8\" Flags=\"Inactive\"");

					// Add our own scene renderer pass
					pSceneRenderer->Create("SRP2DGame", "2DGame");

					// Add post processing scene renderer pass
					pSceneRenderer->Create("PLCompositing::SRPPostProcessing", "PostProcessing");

					// Add end scene renderer pass
					pSceneRenderer->Create("PLCompositing::SRPEnd", "End");

					// Make this scene renderer to the default scene renderer of our scene surface painter
					pSPScene->SetDefaultSceneRenderer(pSceneRenderer->GetName());
				}
			}
		}

		// Set scene container
		SetScene(pSceneContainer);

		// Start the game
		Restart();
	}
}

void Application67::OnInputControllerFound(Controller *pInputController, String sInputSemantic)
{
	// Call base implementation
	EngineApplication::OnInputControllerFound(pInputController, sInputSemantic);

	// Is there an application input controller? If so, connect gun (SNGun)...
	if (m_pInputController && sInputSemantic == "Gun") {
		pInputController->Connect("X",		m_pInputController->GetControl("RotY"), -0.05f);
		// "Left" is connected automatically within "EngineApplication::OnInputControllerFound()" with the virtual standard controller
		// "Right" is connected automatically within "EngineApplication::OnInputControllerFound()" with the virtual standard controller
		pInputController->Connect("Left",	m_pInputController->GetControl("Forward"));
		pInputController->Connect("Right",	m_pInputController->GetControl("Backward"));
		pInputController->Connect("Left",	m_pInputController->GetControl("StrafeLeft"));
		pInputController->Connect("Right",	m_pInputController->GetControl("StrafeRight"));
		pInputController->Connect("Fire",	m_pInputController->GetControl("Button1"));
		pInputController->Connect("Fire",	m_pInputController->GetControl("Button2"));
		pInputController->Connect("Fire",	m_pInputController->GetControl("Button3"));
		pInputController->Connect("Fire",	m_pInputController->GetControl("Button4"));
		pInputController->Connect("Fire",	m_pInputController->GetControl("Button5"));
	}
}

void Application67::OnCreateInputController()
{
	// Call base implementation
	EngineApplication::OnCreateInputController();

	// Get virtual input controller
	Controller *pController = reinterpret_cast<Controller*>(GetInputController());
	if (pController)
		pController->SignalOnControl.Connect(SlotOnControl);
}
