/*********************************************************\
 *  File: Application61.cpp                              *
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
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Localization.h>
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Controls/Button.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContainer.h>
#include "Application61.h"


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
pl_class_metadata(Application61, "", PLEngine::EngineApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
	// Slots
	pl_slot_1_metadata(OnControl,	PLInput::Control&,	"Called when a control event has occurred, occurred control as first parameter",	"")
pl_class_metadata_end(Application61)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application61::Application61(Frontend &cFrontend) : EngineApplication(cFrontend),
	SlotOnControl(this)
{
	// Get the "PLSound::SCSound"-class
	const Class *pClass = ClassManager::GetInstance()->GetClass("PLSound::SCSound");
	if (pClass) {
		// Get the "SoundAPI"-attribute
		const VarDesc *pVarDesc = pClass->GetAttribute("SoundAPI");
		if (pVarDesc) {
			// Use the default value of the "SoundAPI"-attribute as the default sound API within this sample
			m_sSoundAPI = pVarDesc->GetDefault();
		}
	}

	// Add command line argument for choosing the sound API
	m_cCommandLine.AddArgument("SoundAPI", "Sound API", m_sSoundAPI, false);
}

/**
*  @brief
*    Destructor
*/
Application61::~Application61()
{
}

/**
*  @brief
*    Returns the name of the used sound API
*/
String Application61::GetSoundAPI() const
{
	return m_sSoundAPI;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the text of the time scale text node
*/
void Application61::UpdateTimeScaleTextNode()
{
	// Get the scene
	SceneContainer *pSceneContainer = GetScene();
	if (pSceneContainer) {
		// Get the scene info text container
		SceneContainer *pInfoTextContainer = static_cast<SceneContainer*>(pSceneContainer->GetByName("Parent.InfoText"));
		if (pInfoTextContainer) {
			// Get the time scale text scene node
			SceneNode *pInfoTextNode = pInfoTextContainer->GetByName("TimeScale");
			if (pInfoTextNode)
				pInfoTextNode->SetAttribute("Text", PLT("1/2/0=Decrease/increase/reset timescale (current: ") + Timing::GetInstance()->GetTimeScaleFactor() + ')');
		}
	}
}

/**
*  @brief
*    Called when a control event has occurred
*/
void Application61::OnControl(Control &cControl)
{
	// Is it a button?
	if (cControl.GetType() == ControlButton && static_cast<Button&>(cControl).IsPressed()) {
		// Check whether the escape key was pressed
		if (cControl.GetName() == "KeyboardEscape") {
			// Shut down the application
			Exit(0);
		} else {
			// Get current time difference
			Timing *pTimer = Timing::GetInstance();
			const float fTimeScaleFactor = pTimer->GetTimeScaleFactor();

			// Check button
			if (cControl.GetName() == "Keyboard1") {
				// Decrease timescale
				pTimer->SetTimeScaleFactor(fTimeScaleFactor - 0.1f);
				if (pTimer->GetTimeScaleFactor() < 0.1f)
					pTimer->SetTimeScaleFactor(0.1f);
			} else if (cControl.GetName() == "Keyboard2") {
				// Increase timescale
				pTimer->SetTimeScaleFactor(fTimeScaleFactor +  0.1f);
				if (pTimer->GetTimeScaleFactor() > 4.0f)
					pTimer->SetTimeScaleFactor(4.0f);
			} else if (cControl.GetName() == "Keyboard3") {
				// Reset timescale
				pTimer->SetTimeScaleFactor();
			}

			// Time scale factor changed?
			if (fTimeScaleFactor != pTimer->GetTimeScaleFactor()) {
				// Update the time scale text node
				UpdateTimeScaleTextNode();

				// Update the pitch variable of the sound container using the time scale factor
				SceneContainer *pSceneContainer = GetScene();
				if (pSceneContainer)
					pSceneContainer->SetAttribute("Pitch", pTimer->GetTimeScaleFactor());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application61::OnInitCmdLine()
{
	// Call the base implementation
	EngineApplication::OnInitCmdLine();

	// Valid default sound API given?
	const String sClassName = m_cCommandLine.GetValue("SoundAPI");
	if (sClassName.GetLength()) {
		const Class *pClass = ClassManager::GetInstance()->GetClass(sClassName);
		if (pClass && pClass->IsDerivedFrom("PLSound::SoundManager"))
			m_sSoundAPI = sClassName;
	}
}

void Application61::OnInit()
{
	// Is a sound API selected?
	if (m_sSoundAPI.GetLength()) {
		// Call base implementation
		EngineApplication::OnInit();

		// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
		GetSceneRendererTool().SetPassAttribute("Begin", "ColorClear", "0.5 0.5 0.5 0");
	} else {
		// Error!
		Exit(1);
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application61::OnCreateScene(SceneContainer &cContainer)
{
	// Set no scene container as default
	SetScene(nullptr);

	// Sound API given?
	if (m_sSoundAPI.GetLength()) {
		// Create a scene container with our 'concrete sound scene' using the chosen sound API
		SceneNode *pSceneContainerNode = cContainer.Create("PLSound::SCSound", "SoundScene", "SoundAPI=\"" + m_sSoundAPI + "\" Pitch=\"" + Timing::GetInstance()->GetTimeScaleFactor() + '\"');
		if (pSceneContainerNode && pSceneContainerNode->IsInstanceOf("PLScene::SceneContainer")) {
			SceneContainer *pSceneContainer = static_cast<SceneContainer*>(pSceneContainerNode);

			// Protect this important container!
			pSceneContainer->SetProtected(true);

			// Set scene container
			SetScene(pSceneContainer);

			// Populate the scene container
			// Create a camera
			SceneNode *pCamera = pSceneContainer->Create("PLScene::SNCamera", "FreeCamera", "Position=\"0.0 -1.5 1.4\" Rotation=\"-14.0 180.0 0.0\"");
			if (pCamera && pCamera->IsInstanceOf("PLScene::SNCamera")) {
				// Make this to our main scene camera
				SetCamera(reinterpret_cast<SNCamera*>(pCamera));

				// Add a controller modifier so we can look around the camera by using a default control
				pCamera->AddModifier("PLEngine::SNMEgoLookController");

				// Add a controller modifier so we can move around the camera by using a default control
				pCamera->AddModifier("PLEngine::SNMMoveController", "Speed=\"2\"");

				// Make this camera to the 'sound listener'
				pSceneContainer->SetAttribute("Listener", pCamera->GetName());
			}

			// Create the floor
			pSceneContainer->Create("PLScene::SNMesh", "Floor", "Position=\"0.0 -2.1 -5.0\" Rotation=\"0.0 180.0 0.0\" Scale=\"4.0 0.1 4.0\" Mesh=\"Default\"");

			// Create a box on the floor
			pSceneContainer->Create("PLScene::SNMesh", "Box", "Position=\"0.0 -1.5 -5.0\" Scale=\"0.5 0.5 0.5\" Mesh=\"Default\"");

			// Create rain particles and rain sound
			pSceneContainer->Create("PLParticleGroups::PGRain", "Rain", "Position=\"0.0 15.0 -5.0\" Flags=\"ForceUpdate\" MediumSize=\"3\"");
			pSceneContainer->Create("PLSound::SNSound", "RainSound", "Sound=\"Data/Sounds/Rain.ogg\" Volume=\"0.2\"");

			// Create the soldier walking in the rain :)
			SceneNode *pSoldier = pSceneContainer->Create("PLScene::SNMesh", "Soldier", "Position=\"0.0 -1.5 -5.0\" Scale=\"0.007 0.007 0.007\" Mesh=\"Data/Meshes/Soldier.mesh\"");
			if (pSoldier) {
				// Add animation modifier
				pSoldier->AddModifier("PLScene::SNMMeshAnimation", "Name=\"walk_0\" Speed=\"2\"");

				// Let the soldier walk on a path...
				pSoldier->AddModifier("PLScene::SNMPositionPath", "Filename=\"Data/Misc/61Sound_Soldier.path\" Speed=\"0.5\" Flags=\"NodeIndexProgress\" Interpolation=\"CatmullRomCurve\"");

				// ... and we should look into the direction he's moving
				pSoldier->AddModifier("PLScene::SNMRotationMoveDirection");

				// Link a footsteps sound to the soldier
				pSoldier->AddModifier("PLSound::SNMSound","Sound=\"Data/Sounds/Walking.ogg\"");
			}

			// Setup scene surface painter
			SurfacePainter *pPainter = GetPainter();
			if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
				SPScene *pSPScene = static_cast<SPScene*>(pPainter);
				pSPScene->SetRootContainer(cContainer.GetContainer());
				pSPScene->SetSceneContainer(pSceneContainer);
			}

			// Within the parent container...
			SceneContainer *pContainer = pSceneContainer->GetContainer();
			if (pContainer) {
				// Get/add our information text scene nodes
				SceneNode *pInfoText = pContainer->Create("PLScene::SceneContainer", "InfoText", "Flags=\"NoCulling\"");
				if (pInfoText && pInfoText->IsInstanceOf("PLScene::SceneContainer")) {
					SceneContainer *pInfoTextContainer = static_cast<SceneContainer*>(pInfoText);

					// The name of the used sound API
					pInfoTextContainer->Create("PLScene::SNText2D", "SoundAPI", "Position=\"0.01 0.01\" Flags=\"No3DPosition|NoCenter\" Text=\"" + PLT("Sound API: ") + GetSoundAPI() + '\"');

					// Draw keys information
					pInfoTextContainer->Create("PLScene::SNText2D", "Keys",      "Position=\"0.01 0.04\" Flags=\"No3DPosition|NoCenter\" Text=\"" + PLT("Keys:")                      + '\"');
					pInfoTextContainer->Create("PLScene::SNText2D", "TimeScale", "Position=\"0.06 0.08\" Flags=\"No3DPosition|NoCenter\"");
					UpdateTimeScaleTextNode();
				}
			}
		}
	}
}

void Application61::OnCreateInputController()
{
	// Call base implementation
	EngineApplication::OnCreateInputController();

	// Get virtual input controller
	Controller *pController = reinterpret_cast<Controller*>(GetInputController());
	if (pController)
		pController->SignalOnControl.Connect(SlotOnControl);
}