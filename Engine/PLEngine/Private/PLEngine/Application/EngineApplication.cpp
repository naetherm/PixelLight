/*********************************************************\
 *  File: EngineApplication.cpp                          *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Base/Class.h>
#include <PLCore/System/System.h>
#include <PLInput/Input/InputManager.h>
#include <PLInput/Input/Virtual/VirtualController.h>
#include <PLInput/Input/Virtual/VirtualStandardController.h>
#include <PLRenderer/RendererContext.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneQuery.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Compositing/SceneRenderer.h>
#include <PLScene/Scene/SceneNodes/SNKeyValue.h>
#include "PLEngine/SceneCreator/SceneCreator.h"
#include "PLEngine/Compositing/Console/ConsoleCommand.h"
#include "PLEngine/Compositing/Console/SNConsoleBase.h"
#include "PLEngine/Application/EngineApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(EngineApplication)


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String EngineApplication::DefaultSceneRenderer = "Forward.sr";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
EngineApplication::EngineApplication(Frontend &cFrontend, const String &sSceneFilename) : SceneApplication(cFrontend, sSceneFilename),
	m_sDefaultSceneRenderer(DefaultSceneRenderer),
	m_pFirstFoundCamera(nullptr),
	m_bHasLoadScreen(false),
	m_pInputController(nullptr),
	m_bEditModeEnabled(false),
	EventHandlerSceneNode			(&EngineApplication::OnSceneNode,            this),
	EventHandlerLoadProgress		(&EngineApplication::OnLoadProgress,         this),
	EventHandlerInputControllerFound(&EngineApplication::OnInputControllerFound, this)
{
	// By default, edit mode is enabled
	SetEditModeEnabled(true);
}

/**
*  @brief
*    Destructor
*/
EngineApplication::~EngineApplication()
{
}

/**
*  @brief
*    Returns the scene container
*/
SceneContainer *EngineApplication::GetScene() const
{
	// This cast is safe because we 'know' it can ONLY be a scene container!
	return static_cast<SceneContainer*>(m_cSceneContainerHandler.GetElement());
}

/**
*  @brief
*    Sets the scene container
*/
void EngineApplication::SetScene(SceneContainer *pContainer)
{
	m_cSceneContainerHandler.SetElement(pContainer);
}

/**
*  @brief
*    Clears the scene, after calling this method the scene is empty
*/
void EngineApplication::ClearScene()
{
	// Get the scene container holding our scene
	SceneContainer *pContainer = GetScene();
	if (pContainer) {
		// Clear the old scene
		pContainer->Clear();

		// Cleanup the scene context right now to ensure that all 'delete this'-marked scene nodes are really gone!
		// If this is not done, we may get problems with for example the names of dynamic textures because there may
		// occur name conflicts if multiple render-to-texture scene containers want to render into a same named texture...
		// Topics like 'the new scene is using resources that are already loaded in' must be handled on another level, e.g.
		// by delaying the unloading of currently unreferenced resources.
		if (m_pSceneContext)
			m_pSceneContext->Cleanup();
	}
}

/**
*  @brief
*    Get scene camera
*/
SNCamera *EngineApplication::GetCamera() const
{
	// This cast is safe because we 'know' it can ONLY be a camera!
	return reinterpret_cast<SNCamera*>(m_cCameraHandler.GetElement());
}

/**
*  @brief
*    Get virtual input controller
*/
VirtualController *EngineApplication::GetInputController() const
{
	// Return input controller
	return m_pInputController;
}

/**
*  @brief
*    Set virtual input controller
*/
void EngineApplication::SetInputController(VirtualController *pInputController)
{
	// Set input controller
	m_pInputController = pInputController;
}

/**
*  @brief
*    Get scene renderer tool
*/
SceneRendererTool &EngineApplication::GetSceneRendererTool()
{
	// Return scene renderer tool
	return m_cSceneRendererTool;
}

/**
*  @brief
*    Get scene renderer tool
*/
const SceneRendererTool &EngineApplication::GetSceneRendererTool() const
{
	// Return scene renderer tool
	return m_cSceneRendererTool;
}

/**
*  @brief
*    Get screenshot tool
*/
Screenshot &EngineApplication::GetScreenshotTool()
{
	// Return screenshot tool
	return m_cScreenshot;
}

/**
*  @brief
*    Returns whether or not edit mode is enabled
*/
bool EngineApplication::IsEditModeEnabled() const
{
	return m_bEditModeEnabled;
}

/**
*  @brief
*    Sets whether or not edit mode is enabled
*/
void EngineApplication::SetEditModeEnabled(bool bEnabled)
{
	// State change?
	if (m_bEditModeEnabled != bEnabled) {
		// Backup the new state
		m_bEditModeEnabled = bEnabled;

		// Get the root scene
		SceneContainer *pRootScene = GetRootScene();
		if (pRootScene) {
			// Enable/disable standard edit features from the PixelLight scene graph (if the user hasn't changed anything :)
			SceneNode *pSceneNode = pRootScene->GetByName("PLEngine::SNEngineInformation0");
			if (pSceneNode)
				pSceneNode->SetActive(bEnabled);
			pSceneNode = pRootScene->GetByName("PLEngine::SNConsole0");
			if (pSceneNode)
				pSceneNode->SetActive(bEnabled);
		}

		// Setup log level
		Log::GetInstance()->SetLogLevel(static_cast<uint8>(m_bEditModeEnabled ? Log::Debug : Log::Info));
	}
}

/**
*  @brief
*    Quit the engine
*/
void EngineApplication::ConsoleCommandQuit(ConsoleCommand &cCommand)
{
	Exit(0);
}


//[-------------------------------------------------------]
//[ Public virtual EngineApplication functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the scene camera
*/
void EngineApplication::SetCamera(SNCamera *pCamera)
{
	// Deactivate the current camera
	if (m_cCameraHandler.GetElement())
		m_cCameraHandler.GetElement()->SetActive(false);

	// Set new camera
	m_cCameraHandler.SetElement(reinterpret_cast<SceneNode*>(pCamera));

	// Get scene surface painter... and inform it about the new set camera
	SurfacePainter *pPainter = GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene"))
		static_cast<SPScene*>(pPainter)->SetCamera(pCamera);

	// Activate the new camera
	if (pCamera)
		reinterpret_cast<SceneNode*>(pCamera)->SetActive(true);

	// Emit signal
	SignalCameraSet();
}

bool EngineApplication::LoadScene(const String &sFilename)
{
	// Get the scene container holding our scene
	SceneContainer *pContainer = GetScene();
	if (!pContainer)
		return false; // Error! (should NEVER happen...)

	// Disable the ingame GUI
	SceneNode *pGui = pContainer->GetContainer()->GetByName("GUI");
	if (pGui)
		pGui->SetActive(false);

	// Clear the scene, after calling this method the scene is empty
	ClearScene();

	// Load the scene
	bool bResult = pContainer->LoadByFilename(sFilename);
	if (bResult) {
		// Set a null pointer camera and default scene renderer
		m_sDefaultSceneRenderer		= "";
		m_sSceneRendererVariables	= "";
		m_sClearColor				= "";
		m_sStartCamera				= "";
		m_pFirstFoundCamera			= nullptr;
		m_bHasLoadScreen			= false;

		// Get scene surface painter
		SurfacePainter *pPainter = GetPainter();
		if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
			// Assign the first found camera scene node to your surface listener and look for
			// known key/value data scene nodes
			SceneQuery *pSceneQuery = pContainer->CreateQuery("PLScene::SQEnumerate");
			if (pSceneQuery) {
				// Connect event handler
				pSceneQuery->SignalSceneNode.Connect(EventHandlerSceneNode);

				// Perform the query
				pSceneQuery->PerformQuery();

				// Destroy the query
				pContainer->DestroyQuery(*pSceneQuery);
			}

			// Set to default scene renderer?
			if (!m_sDefaultSceneRenderer.GetLength())
				m_sDefaultSceneRenderer = DefaultSceneRenderer;

			// Sets all scene renderer pass attribute values to their default value
			GetSceneRendererTool().SetDefaultValues();

			// Set the used scene renderer
			GetSceneRendererTool().SetSceneRenderer(pContainer, m_sDefaultSceneRenderer, DefaultSceneRenderer);

			// Set clear color
			GetSceneRendererTool().SetPassAttribute("Begin", "ColorClear", m_sClearColor);

			// Set scene renderer variables
			if (m_sSceneRendererVariables.GetLength())
				GetSceneRendererTool().SetValues(m_sSceneRendererVariables);

			// Is there a given start camera?
			SceneNode *pCamera = nullptr;
			if (m_sStartCamera.GetLength()) {
				SceneNode *pStartCamera = pContainer->GetByName(m_sStartCamera);
				if (pStartCamera && pStartCamera->IsCamera())
					pCamera = pStartCamera;
			}

			// Use the first found camera?
			if (!pCamera)
				pCamera = m_pFirstFoundCamera;

			// Activate the current used camera by default
			if (pCamera)
				pCamera->SetActive(true);

			// Assign this camera to the scene renderer and to the application
			SetCamera(reinterpret_cast<SNCamera*>(pCamera));
		}

		// Emit the scene loading has been finished successfully event
		SignalSceneLoadingFinished();
	}

	// Enable the ingame GUI
	if (pGui)
		pGui->SetActive(true);

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractLifecycle functions ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialization function that is called prior to OnInit()
*/
bool EngineApplication::OnStart()
{
	// Call base implementation - in here, we skip "SceneApplication::OnStart()" to overwrite the initialization order (input should come before scene)
	if (RendererApplication::OnStart()) {
		{ // Input initialization
			// Initialize input system
			InputManager::GetInstance()->DetectDevices();

			// Connect the input controller found event handler to the corresponding scene context event
			InputManager::GetInstance()->EventInputControllerFound.Connect(EventHandlerInputControllerFound);

			// Create virtual input controller
			OnCreateInputController();
			if (!m_bRunning)
				return false;
		}

		{ // Tools initialization
			// Initialize scene renderer tool
			m_cSceneRendererTool.SetPainter(GetPainter());

			// Initialize screenshot tool
			m_cScreenshot.SetRendererContext(GetRendererContext());
			m_cScreenshot.SetPainter(GetPainter());

			// Set screenshot directory
			if (m_bMultiUser)
				m_cScreenshot.SetScreenshotDirectory(System::GetInstance()->GetUserDataDir() + '/' + m_sAppDataSubdir);
			else
				m_cScreenshot.SetScreenshotDirectory(System::GetInstance()->GetCurrentDir());
		}

		{ // "SceneApplication::OnStart()"-part
			// Get renderer context
			RendererContext *pRendererContext = GetRendererContext();
			if (pRendererContext) {
				// Create scene context
				m_pSceneContext = new SceneContext(*pRendererContext);

				// Create root scene
				OnCreateRootScene();
				if (!m_bRunning)
					return false;
			}
		}

		// Done
		return true;
	}

	// Error
	return false;
}

/**
*  @brief
*    Called when the object has the focus (keep the implementation lightweight)
*/
void EngineApplication::OnResume()
{
	// Call base implementation
	SceneApplication::OnResume();

	// Activate input controller
	if (m_pInputController)
		m_pInputController->SetActive(true);
}

/**
*  @brief
*    Called when the object has no longer the focus (keep the implementation lightweight)
*/
void EngineApplication::OnPause()
{
	// Deactivate input controller
	if (m_pInputController)
		m_pInputController->SetActive(false);

	// Call base implementation
	SceneApplication::OnPause();
}

/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*/
void EngineApplication::OnStop()
{
	// Destroy virtual input controller
	if (m_pInputController) {
		delete m_pInputController;
		m_pInputController = nullptr;
	}

	// Call base implementation
	SceneApplication::OnStop();
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractFrontend functions  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called to let the frontend update it's states
*/
void EngineApplication::OnUpdate()
{
	// Call base implementation
	SceneApplication::OnUpdate();

	// Update input manager
	InputManager::GetInstance()->Update();
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneApplication functions ]
//[-------------------------------------------------------]
void EngineApplication::OnCreateRootScene()
{
	// Get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		// First, create the scene root container which holds the scene container with our 'concrete' scene within it
		SceneContainer *pRootContainer = pSceneContext->GetRoot() ? static_cast<SceneContainer*>(pSceneContext->GetRoot()->Create("PLScene::SceneContainer", "RootScene")) : nullptr;
		if (pRootContainer) {
			// Protect this important container!
			pRootContainer->SetProtected(true);

			// Create a scene container with our 'concrete scene'
			SceneNode *pSceneContainerNode = pRootContainer->Create("PLScene::SceneContainer", "Scene");
			if (pSceneContainerNode && pSceneContainerNode->IsInstanceOf("PLScene::SceneContainer")) {
				SceneContainer *pSceneContainer = static_cast<SceneContainer*>(pSceneContainerNode);

				// Protect this important container!
				pSceneContainer->SetProtected(true);

				// Connect event handler
				if (pSceneContainerNode->IsInstanceOf("PLScene::SceneContainer"))
					static_cast<SceneContainer*>(pSceneContainerNode)->SignalLoadProgress.Connect(EventHandlerLoadProgress);

				// Create the 'concrete scene'
				OnCreateScene(*pSceneContainer);
			}

			// Create scene node for engine information
			SceneNode *pSceneNode = pRootContainer->Create("PLEngine::SNEngineInformation");
			if (pSceneNode)
				pSceneNode->SetActive(m_bEditModeEnabled);

			// Create console scene node - using the console command 'timescale <value>' we
			// can change the scene time (slowdown or accelerate)
			pSceneNode = pRootContainer->Create("PLEngine::SNConsole");
			if (pSceneNode && pSceneNode->GetClass()->IsDerivedFrom("PLEngine::SNConsoleBase")) {
				SNConsoleBase *pConsole = static_cast<SNConsoleBase*>(pSceneNode);

				// Register default commands
				pConsole->RegisterCommand(0,	"quit",		"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));
				pConsole->RegisterCommand(0,	"exit",		"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));
				pConsole->RegisterCommand(0,	"bye",		"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));
				pConsole->RegisterCommand(0,	"logout",	"",	"",	Functor<void, ConsoleCommand &>(&EngineApplication::ConsoleCommandQuit, this));

				// Set active state
				pConsole->SetActive(m_bEditModeEnabled);
			}
		}

		// Set the root scene
		SetRootScene(pRootContainer);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual EngineApplication functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function that is called to create the application's scene container
*/
void EngineApplication::OnCreateScene(SceneContainer &cContainer)
{
	// [TODO] Load 'm_sSceneFilename' if provided

	// Set scene container flags
	cContainer.SetFlags(SceneNode::NoCulling);

	// Setup scene surface painter
	SurfacePainter *pPainter = GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
		SPScene *pSPScene = static_cast<SPScene*>(pPainter);
		pSPScene->SetRootContainer(cContainer.GetContainer());
		pSPScene->SetSceneContainer(&cContainer);
	}

	// Configure scene and set the currently used application camera
	SetCamera(SceneCreator::ConfigureScene(cContainer, "PLEngine::SceneCreatorDefault"));

	// Set scene container
	SetScene(&cContainer);
}

/**
*  @brief
*    Function that is called to initialize the application's virtual input controller
*/
void EngineApplication::OnCreateInputController()
{
	// Create virtual standard controller
	VirtualStandardController *pController = new VirtualStandardController();

	// Connect to physical devices
	pController->ConnectToDevices();

	// Set virtual input controller
	SetInputController(pController);
}

/**
*  @brief
*    Function that is called when an input controller has been found
*/
void EngineApplication::OnInputControllerFound(Controller *pInputController, String sInputSemantic)
{
	// Is there an application input controller?
	if (m_pInputController) {
		// Try to connect all controls automatically with the virtual standard controller
		pInputController->ConnectAll(m_pInputController, "", sInputSemantic);
	}
}

/**
*  @brief
*    Called on load progress
*/
void EngineApplication::OnLoadProgress(float fLoadProgress)
{
	// Call the 'update'-function so we can see the progress within the load screen
	if (m_bHasLoadScreen) {
		// Redraw & ping the frontend
		GetFrontend().RedrawAndPing();
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a scene node was found
*/
void EngineApplication::OnSceneNode(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// Is this a camera?
	if (cSceneNode.IsCamera()) {
		if (!m_pFirstFoundCamera)
			m_pFirstFoundCamera = &cSceneNode;

		// Deactivate all camera's by default
		cSceneNode.SetActive(false);

	// Key/value data scene node?
	} else if (cSceneNode.IsInstanceOf("PLScene::SNKeyValue")) {
		const SNKeyValue &cKeyValue = static_cast<const SNKeyValue&>(cSceneNode);

		// SceneRenderer
		if (cKeyValue.Key.GetString() == "SceneRenderer") {
			// Use the first found key
			if (!m_sDefaultSceneRenderer.GetLength())
				m_sDefaultSceneRenderer = cKeyValue.Value.GetString();

		// SceneRendererVariables
		} else if (cKeyValue.Key.GetString() == "SceneRendererVariables") {
			// Use the first found key
			if (!m_sSceneRendererVariables.GetLength())
				m_sSceneRendererVariables = cKeyValue.Value.GetString();

		// ClearColor
		} else if (cKeyValue.Key.GetString() == "ClearColor") {
			// Use the first found key
			if (!m_sClearColor.GetLength())
				m_sClearColor = cKeyValue.Value.GetString();

		// StartCamera
		} else if (cKeyValue.Key.GetString() == "StartCamera") {
			// Use the first found key
			if (!m_sStartCamera.GetLength())
				m_sStartCamera = cKeyValue.Value.GetString();
		}

	// Load screen scene node?
	} else if (cSceneNode.IsInstanceOf("PLEngine::SNLoadScreenBase")) {
		m_bHasLoadScreen = true;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
