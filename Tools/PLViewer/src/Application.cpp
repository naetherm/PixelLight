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
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include <PLCore/System/System.h>
#include <PLCore/Tools/LoadableType.h>
#include <PLCore/Tools/LoadableManager.h>
#include <PLCore/Script/ScriptManager.h>
#include <PLCore/Frontend/FrontendContext.h>
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Controls/Button.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLEngine/SceneCreator/SceneCreatorLoadableType.h>
#include "Application.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
using namespace PLEngine;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application, "", PLEngine::ScriptApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
	// Slots
	pl_slot_1_metadata(OnControl,	PLInput::Control&,	"Called when a control event has occurred, occurred control as first parameter",	"")
pl_class_metadata_end(Application)


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
// A helper to be able to toggle between 'choose resource file at start' and 'load
// default resource at start' (quite comfortable if you make many experiments :)
const String Application::DefaultFilename = "";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application(Frontend &cFrontend) : ScriptApplication(cFrontend),
	SlotOnControl(this)
{
	{ // Set no multi-user if standalone application
		// -> In case the file "PLViewer.cfg" is in the same directory as the executable, or within one directory "above" (e.g. because there are "x86" and "x64" versions)
		//    run this application in "standalone-mode"
		// -> When shipping something by using PLViewer, experience tells that there's always such a file provided to set the desired settings, so, this is no drawback
		//    and this enables us to provide just one version of this executable
		const String sDirectory = cFrontend.GetContext().GetAppDirectory() + '/';
		if (File(sDirectory + GetConfigName()).IsFile() ||															// Windows example: "C:\MyApplication\PLViewer.cfg"
			File(sDirectory + System::GetInstance()->GetPlatformArchitecture() + '/' + GetConfigName()).IsFile()) {	// Windows example: "C:\MyApplication\x86\PLViewer.cfg"
			// The configuration file exists, so run this application in standalone mode (e.g. log and configuration will not be written into the user directory)
			SetMultiUser(false);
		}
	}

	// This application accepts all the standard parameters that are defined in the application
	// base class (such as --help etc.). The last parameter however is the filename to load, so add that.
	m_cCommandLine.AddFlag("Standalone", "-s", "--standalone", "Standalone application, write log and configuration into the directory the executable is in", false);
	m_cCommandLine.AddArgument("Filename", "Resource (e.g. scene or script) filename", "", false);
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}

/**
*  @brief
*    Returns the filename of the currently loaded resource
*/
String Application::GetResourceFilename() const
{
	return m_sResourceFilename;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::AbstractLifecycle functions    ]
//[-------------------------------------------------------]
bool Application::OnStart()
{
	// Set human readable frontend title
	GetFrontend().SetTitle("PixelLight Viewer");

	// Call base implementation
	return ScriptApplication::OnStart();
}


//[-------------------------------------------------------]
//[ Public virtual PLEngine::EngineApplication functions  ]
//[-------------------------------------------------------]
bool Application::LoadScene(const String &sFilename)
{
	// Do not backup the filename of the current resource in here
	// -> Background: When the current resource is a script which is using "LoadScene()" in order to load a scene we would backup an imposter in here
	// m_sResourceFilename = sFilename;

	// Call base implementation
	return ScriptApplication::LoadScene(sFilename);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control event has occurred
*/
void Application::OnControl(Control &cControl)
{
	// Is it a button and was it just hit?
	if (cControl.GetType() == ControlButton) {
		// Shut down the application?
		if (cControl.GetName() == "KeyboardEscape") {
			if (reinterpret_cast<Button&>(cControl).IsHit())
				Exit(0);

		// Make a screenshot from the current render target?
		} else if (cControl.GetName() == "KeyboardF12") {
			if (reinterpret_cast<Button&>(cControl).IsHit())
				GetScreenshotTool().SaveScreenshot();

		// Toggle mouse cursor visibility?
		} else if (cControl.GetName() == "KeyboardM") {
			// Toggle mouse cursor visibility
			if (reinterpret_cast<Button&>(cControl).IsHit())
				GetFrontend().SetMouseVisible(!GetFrontend().IsMouseVisible());
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Application functions               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads a resource which type has to be evaluated internally
*/
bool Application::LoadResource(const String &sFilename, const String &sType)
{
	bool bResult = false;	// Error by default

	// Clear the scene, after calling this method the scene is empty
	ClearScene();

	// Destroy the currently used script
	DestroyScript();

	// Backup the filename of the current resource
	m_sResourceFilename = sFilename;

	// Is there anything to load in?
	if (sFilename.GetLength()) {
		{ // Make the directory of the scene to load in to the current directory

			// Ok, the next thing is tricky, and every solution will end up in being a hack due to lack of information.
			// Within materials, meshes etc. there are usually relative paths provided, no absolute (not just true for PixelLight file formats).
			// Further, those paths are usually relative to a project root resulting e.g. within a default directory layout like
			// - Root
			//   - Data
			//     - Meshes
			//     - Materials
			// For "normal" projects this is no issue because everything is usually relative to the root project directory...
			// ... but this viewer must be able to pick out e.g. a mesh out of nowhere and must still be able to locate the required
			// other resources like materials. So, in here, we can only work with heuristics... this can and will of course go from
			// time to time horribly wrong...

			// First try: Find the first "Data" occurrence with the given filename and hope that it's the project root directory

			// Get filename as clean URL
			Url cUrl = Url(sFilename);
			cUrl.Collapse();

			// Get the first part of the path, and then look for "Data"
			uint32 nPathPos = 0;
			String sPart = cUrl.GetFirstPath(nPathPos);
			while (sPart != "Data" && sPart.GetLength())
				sPart = cUrl.GetNextPath(nPathPos);
			if (sPart == "Data") {
				// Set the base directory of the application
				SetBaseDirectory(cUrl.GetRoot() + cUrl.GetPath().GetSubstring(0, nPathPos - 5));	// -5 = Remove "Data/"
			} else {
				// Second try: Cut of "/Data/Scenes/" and hope that it's the project root directory.
				// If it's not there, take the directory the given resource is in.

				// Validate path
				const String sDirectory = cUrl.CutFilename();

				// Search for "/Data/Scenes/" and get the prefix of that, in case it's not there just use directly the scene directory
				const int nIndex = sDirectory.IndexOf("/Data/Scenes/");

				// Set the base directory of the application
				SetBaseDirectory("file://" + ((nIndex >= 0) ? sDirectory.GetSubstring(0, nIndex) : sDirectory) + '/');
			}
		}

		// Set default (and very basic) scene renderer
		GetSceneRendererTool().SetSceneRenderer(GetScene(), DefaultSceneRenderer, DefaultSceneRenderer);

		// The viewer supports loading in multiple resource types, but scenes and scripts are
		// the most important because most powerful ones, so give them the highest priority.
		// -> It would be possible to implement support for scripting so that e.g. a Lua script
		//    can be used to decide how to process a resource. But this viewer should be able to
		//    work without external data and should be kept as simple as possible, so we stick to C++.
		// -> Prefer the given loadable type over the filename extension

		// Get file extension
		// -> There are file formats without an extension, so no extension must also be valid
		const String sExtension = Url(sFilename).GetExtension();

		// Is the given filename a supported scene?
		if (sType == "Scene" || (!sType.GetLength() && LoadableManager::GetInstance()->IsFormatLoadSupported(sExtension, "Scene"))) {
			// Is the given resource a scene?
			if (LoadScene(sFilename)) {
				// Done
				bResult = true;

				// Emit the scene loading has been finished successfully event
				// -> Not required because already done within "LoadScene()" above
				// SignalSceneLoadingFinished();
			} else {
				// Write an error message into the log
				PL_LOG(Error, "Failed to load the scene \"" + sFilename + '\"')
			}

		// Is the given file a supported script?
		} else if (ScriptManager::GetInstance()->GetScriptLanguageByExtension(sExtension).GetLength()) {
			// Load the script
			if (LoadScript(sFilename)) {
				// Done
				bResult = true;

				// Emit the scene loading has been finished successfully event
				SignalSceneLoadingFinished();
			} else {
				// Write an error message into the log
				PL_LOG(Error, "Failed to load the script \"" + sFilename + '\"')
			}

		// Ask the loadable system for the resource type
		} else {
			// Get loadable type
			LoadableType *pLoadableType = sType.GetLength() ? LoadableManager::GetInstance()->GetTypeByName(sType) : LoadableManager::GetInstance()->GetTypeByExtension(sExtension);
			if (pLoadableType) {
				// Get the scene container (the 'concrete scene')
				SceneContainer *pSceneContainer = GetScene();
				if (pSceneContainer) {
					// Configure scene and set the preferred camera scene node
					SetCamera(SceneCreatorLoadableType::ConfigureSceneByLoadableType(*pSceneContainer, pLoadableType->GetName(), sFilename));

					// Done
					bResult = true;

					// Emit the scene loading has been finished successfully event
					SignalSceneLoadingFinished();
				}
			}

			// Unknown file-type?
			if (!bResult) {
				// Write an error message into the log
				PL_LOG(Error, "Failed to load the resource \"" + sFilename + "\" because the file-type is unknown")
			}
		}

		// Set the state text, show the user a native filename within the GUI
		SetStateText(bResult ? Url(sFilename).GetNativePath() : ("Failed to load \"" + Url(sFilename).GetNativePath() + "\" (see log for details)"));
	} else {
		// Nothing to do = all went fine
		bResult = true;

		// Set the state text
		SetStateText("Nothing loaded");
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Sets the state text
*/
void Application::SetStateText(const String &sText)
{
	// Just misuse the frontend title to communicate with the user
	GetFrontend().SetTitle("PixelLight viewer - Use drag'n'drop to load in a scene/script - " + sText);
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::CoreApplication functions   ]
//[-------------------------------------------------------]
void Application::OnInitLog()
{
	// Check 'Standalone' commando line flag
	if (m_cCommandLine.IsValueSet("Standalone"))
		SetMultiUser(false);

	// Call base implementation
	ScriptApplication::OnInitLog();
}

void Application::OnInit()
{
	// Filename given?
	String sFilename = m_cCommandLine.GetValue("Filename");
	if (!sFilename.GetLength()) {
		// Get the filename of the default by using the PLViewer configuration
		const String sConfigDefault = GetConfig().GetVar("PLViewerConfig", "DefaultFilename");
		if (sConfigDefault.GetLength()) {
			// Set the default filename
			sFilename = sConfigDefault;
		} else {
			// Load a default resource on start?
			if (DefaultFilename.GetLength()) {
				// Set the default filename
				sFilename = DefaultFilename;
			} else {
				// No filename provided
			}
		}
	}

	// Anything to load in?
	if (sFilename.GetLength()) {
		// Reset the initial script file, else "ScriptApplication::OnInit()" will load and start the script given to it's constructor
		m_sInitialScriptFilename = "";
	}

	// Call base implementation
	ScriptApplication::OnInit();

	// Load the resource
	LoadResource(sFilename);
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractFrontend functions  ]
//[-------------------------------------------------------]
void Application::OnDrop(const Container<String> &lstFiles)
{
	// Load resource (if it's one :)
	LoadResource(lstFiles[0]);
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application::OnCreateScene(SceneContainer &cContainer)
{
	// Set scene container flags
	cContainer.SetFlags(SceneNode::NoCulling);

	// Setup scene surface painter
	SurfacePainter *pPainter = GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
		SPScene *pSPScene = static_cast<SPScene*>(pPainter);
		pSPScene->SetRootContainer(cContainer.GetContainer());
		pSPScene->SetSceneContainer(&cContainer);
	}

	// The default implementation of this method creates a scene which we never need
	// within this viewer, so, just overwrite it and do not create a scene in here

	// Set scene container
	SetScene(&cContainer);
}

void Application::OnCreateInputController()
{
	// Call base implementation
	ScriptApplication::OnCreateInputController();

	// Get virtual input controller
	Controller *pController = reinterpret_cast<Controller*>(GetInputController());
	if (pController)
		pController->SignalOnControl.Connect(SlotOnControl);
}
