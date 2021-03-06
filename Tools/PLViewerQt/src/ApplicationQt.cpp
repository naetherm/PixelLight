/*********************************************************\
 *  File: ApplicationQt.cpp                              *
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
#include <PLCore/Config/Config.h>
#include <PLCore/Tools/LoadableType.h>
#include <PLCore/Tools/LoadableManager.h>
#include <PLMath/Math.h>
#include <PLScene/Compositing/SceneRendererPass.h>
#include <PLFrontendQt/FrontendMainWindow.h>
#include <PLFrontendQt/DockWidget/DockWidgetManager.h>
#include "Gui.h"
#include "ApplicationQt.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ApplicationQt, "", Application, "PLViewer application class with an added lightweight Qt GUI-layer")
	// Methods
	pl_method_0_metadata(GetDockWidgetManager,	pl_ret_type(PLFrontendQt::DockWidgetManager*),	"Returns the dock widget manager instance the application is using, can be a null pointer.",	"")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
pl_class_metadata_end(ApplicationQt)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ApplicationQt::ApplicationQt(Frontend &cFrontend) : Application(cFrontend),
	m_pGui(nullptr),
	m_fLoadProgress(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
ApplicationQt::~ApplicationQt()
{
	// GUI instance has to be destroyed in "ApplicationQt::OnDeInit()" which is guaranteed to be called
}

/**
*  @brief
*    Returns the dock widget manager instance the application is using
*/
PLFrontendQt::DockWidgetManager *ApplicationQt::GetDockWidgetManager() const
{
	// Is there an GUI instance?
	if (m_pGui) {
		// Get the Qt main window
		PLFrontendQt::FrontendMainWindow *pFrontendMainWindow = m_pGui->GetFrontendMainWindow();
		if (pFrontendMainWindow)
			return &pFrontendMainWindow->GetDockWidgetManager();
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::AbstractLifecycle functions    ]
//[-------------------------------------------------------]
bool ApplicationQt::OnStart()
{
	// Set human readable frontend title
	GetFrontend().SetTitle("PixelLight Viewer (Qt)");

	// Call directly "ScriptApplication" base implementation (else "Application::OnStart()" would overwrite our title)
	return ScriptApplication::OnStart();
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractFrontend functions  ]
//[-------------------------------------------------------]
void ApplicationQt::OnUpdate()
{
	// Update the GUI
	if (m_pGui)
		m_pGui->Update();

	// Call base implementation
	Application::OnUpdate();
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void ApplicationQt::OnInit()
{
	// Initialize the GUI (no "m_pGui" security checks required, the application lifecycle is guaranteed)
	m_pGui = new Gui(*this);
	m_pGui->SetEnabled(true);

	// Call base implementation
	Application::OnInit();
}

void ApplicationQt::OnDeInit()
{
	// Get the Qt main window
	PLFrontendQt::FrontendMainWindow *pFrontendMainWindow = m_pGui->GetFrontendMainWindow();

	// De-initialize the GUI (no "m_pGui" security checks required, the application lifecycle is guaranteed)
	delete m_pGui;
	m_pGui = nullptr;

	// Close all dock widgets right now while most of the data is still valid
	if (pFrontendMainWindow)
		pFrontendMainWindow->GetDockWidgetManager().DestroyDockWidgets();

	// Call base implementation
	Application::OnDeInit();
}


//[-------------------------------------------------------]
//[ Public virtual EngineApplication functions            ]
//[-------------------------------------------------------]
bool ApplicationQt::LoadScene(const String &sFilename)
{
	// It's possible that someone is calling "LoadScene()" directly, so disabling the GUI just in "LoadResource()" is not sufficient
	// -> Within the viewer, usually it's "LoadResource()" -> "LoadScene()"
	// -> Disabling/enabling the GUI twice is no problem, "Gui::SetEnabled()" can handle it

	// Disable the GUI window while loading so the user can't prank around
	if (m_pGui)
		m_pGui->SetEnabled(false);

	// Reset the current load progress and ensure everything is up-to-date when we start to load
	m_fLoadProgress = -0.42f;	// So "OnLoadProgress()" is forced to update
	OnLoadProgress(0.0f);

	// Call base implementation
	const bool bResult = Application::LoadScene(sFilename);

	// Enable the Qt main window when loading is done
	if (m_pGui)
		m_pGui->SetEnabled(true);

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void ApplicationQt::OnLoadProgress(float fLoadProgress)
{
	// Time for an update? (we don't want to redraw & ping the frontend each time a single tiny scene node was loaded *performance*)
	if ((fLoadProgress - m_fLoadProgress) >= 0.01f) {
		// Update the current load progress
		m_fLoadProgress = fLoadProgress;

		// Is there an GUI instance?
		if (m_pGui) {
			// Ensure the loading process looks always reasonable to the user
			fLoadProgress = PLMath::Math::ClampToInterval(fLoadProgress, 0.0f, 1.0f);

			// Let the GUI handle the information presentation of the current load progress (in percentage)
			// -> When zero percentage, present another text because for instance loading a XML document may take a moment
			SetStateText(fLoadProgress ? String::Format("Loading... %.0f%%", fLoadProgress*100.0f) : "Prepare loading...");
		}

		// Redraw & ping the frontend so the GUI stays kind-of responsive while loading
		GetFrontend().RedrawAndPing();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Application functions               ]
//[-------------------------------------------------------]
bool ApplicationQt::LoadResource(const String &sFilename, const String &sType)
{
	// The used loadable type may be changed within this method
	String sTypeToUse = sType;

	// Disable the GUI window while loading so the user can't prank around
	if (m_pGui) {
		// In case no loadable type is provided, we need to guess the loadable type...
		// which can go terrible wrong. If required, ask the user for the desired loadable type.
		if (!sTypeToUse.GetLength()) {
			// Get file extension (it's valid that there's no extension, example: DICOM)
			const Url cUrl(sFilename);
			const String sExtension = cUrl.GetExtension();

			// Get loadable types by using the loadable extension
			Array<LoadableType*> lstTypes;
			LoadableManager::GetInstance()->GetTypesByExtension(sExtension, lstTypes);
			if (lstTypes.GetNumOfElements() > 1) {
				// Get array with type names
				Array<String> lstTypeNames;
				for (uint32 i=0; i<lstTypes.GetNumOfElements(); i++)
					lstTypeNames.Add(lstTypes[i]->GetName());

				// We have multiple candidates, ask the user for the desired loadable type
				sTypeToUse = m_pGui->InputDialog(cUrl.GetNativePath(), "Please specify the resource type", lstTypeNames);
				if (!sTypeToUse.GetLength()) {
					// Get us out of here right now!
					return false;
				}

				// It's also possible that one loadable type has multiple loaders with a same extension
				// -> For simplicity we ignore this case in here
			}
		}

		// Disable the GUI
		m_pGui->SetEnabled(false);

		// Give the user a hint what's currently going on
		SetStateText("Loading resource \"" + sFilename + '\"');

		// Ping the frontend so the GUI shows the important state text as soon as possible
		// -> Resource loading may take a while and showing an updated state text after loading is done is not that helpful
		GetFrontend().Ping();
	}

	// Call base implementation
	const bool bResult = Application::LoadResource(sFilename, sTypeToUse);

	// Update color gradient background according to the configuration
	UpdateColorGradientBackground();

	// Update backfaces and silhouettes visualization according to the configuration
	UpdateBackfacesAndSilhouettes();

	// Enable the Qt main window when loading is done
	if (m_pGui) {
		m_pGui->SetEnabled(true);

		// Usability: In case of an error open the log RTTI dock widget at once so we can see what when wrong
		if (!bResult)
			m_pGui->ShowDockWidget("PLFrontendQt::DockWidgetLog");
	}

	// Done
	return bResult;
}

void ApplicationQt::SetStateText(const String &sText)
{
	// Is there an GUI instance?
	if (m_pGui) {
		// Let the GUI handle the information presentation
		m_pGui->SetStateText(sText);
	} else {
		// Call base implementation
		Application::SetStateText(sText);
	}

	// We could ping the frontend via "GetFrontend().Ping()" in general so the GUI shows the
	// important state text as soon as possible, but this might have an notable influence on
	// the performance due to internal message processing
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Update color gradient background according to the configuration
*/
void ApplicationQt::UpdateColorGradientBackground()
{
	// Activated color gradient background within the scene renderer, or at least try it
	// -> The standard scene renderer compositions of PixelLight within "Standard.zip" always have an inactive "PLCompositing::SRPBackgroundColorGradient"-instance
	// -> By using a color gradient background, also completely black/gray/white etc. meshes can be seen which is a good thing as a default setting within this viewer
	if (GetConfig().GetVarInt("PLViewerQtConfig", "ShowColorGradientBackground"))
		GetSceneRendererTool().SetPassAttribute("BackgroundColorGradient", "Flags", "");
	else
		GetSceneRendererTool().SetPassAttribute("BackgroundColorGradient", "Flags", "Inactive");
}

/**
*  @brief
*    Update backfaces and silhouettes visualization according to the configuration
*/
void ApplicationQt::UpdateBackfacesAndSilhouettes()
{
	// Show backfaces as wireframe and silhouettes
	// -> Quite useful when there are no materials or we see within the initial camera configuration only
	//    culled backfaces which may at first look like nothing had been loaded in the first place
	// -> Even when a material is "two sided" we can see wireframes at the "back side", this is no problem and enables us to figure out which face is the front face
	// -> This results as a side effect in a slim silhouette, no problem because this viewer is meant for development and debugging
	if (GetConfig().GetVarInt("PLViewerQtConfig", "ShowBackfacesAndSilhouettes")) {
		SceneRendererPass *pSceneRendererPass = GetSceneRendererTool().GetPassByName("DebugWireframes");
		if (pSceneRendererPass) {
			pSceneRendererPass->SetAttribute("Flags",		"UseDepth");	// Activate and use depth buffer
			pSceneRendererPass->SetAttribute("CullMode",	"CW");			// Render only back faces
			pSceneRendererPass->SetAttribute("LineWidth",	"2");			// We have a slim silhouette anyway, so, make it more visible as a visual aid
		}
	} else {
		// Inactive
		GetSceneRendererTool().SetPassAttribute("DebugWireframes", "Flags", "Inactive");
	}
}
