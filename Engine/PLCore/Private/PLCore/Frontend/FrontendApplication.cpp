/*********************************************************\
 *  File: FrontendApplication.cpp                        *
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
#include "PLCore/System/System.h"
#include "PLCore/Tools/Localization.h"
#include "PLCore/Tools/LoadableManager.h"
#include "PLCore/Tools/LocalizationGroup.h"
#include "PLCore/Frontend/FrontendContext.h"
#include "PLCore/Frontend/FrontendApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(FrontendApplication)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendApplication::FrontendApplication(Frontend &cFrontend) : CoreApplication(),
	m_pFrontend(&cFrontend)
{
	// Set application name and title
	SetName(m_pFrontend->GetContext().GetName());
	SetTitle(m_pFrontend->GetContext().GetName());

	// Set running state
	m_bRunning = true;
}

/**
*  @brief
*    Destructor
*/
FrontendApplication::~FrontendApplication()
{
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*/
void FrontendApplication::OnStop()
{
	{ // Write down display mode information
		int nX = 0;
		int nY = 0;
		uint32 nWidth = 0;
		uint32 nHeight = 0;
		GetFrontend().GetWindowPositionSize(nX, nY, nWidth, nHeight);
		GetConfig().SetVar("PLCore::FrontendConfig", "X",      String(nX));
		GetConfig().SetVar("PLCore::FrontendConfig", "Y",      String(nY));
		GetConfig().SetVar("PLCore::FrontendConfig", "Width",  String(nWidth));
		GetConfig().SetVar("PLCore::FrontendConfig", "Height", String(nHeight));
	}

	// Call base implementation
	CoreApplication::OnStop();
}

//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the window size has been changed
*/
void FrontendApplication::OnSize()
{
	// No default implementation
}

/**
*  @brief
*    Called when the fullscreen mode was changed
*/
void FrontendApplication::OnFullscreenMode()
{
	// No default implementation
}

/**
*  @brief
*    Called to let the frontend draw into it's window
*/
void FrontendApplication::OnDraw()
{
	// No default implementation
}

/**
*  @brief
*    Called to let the frontend update it's states
*/
void FrontendApplication::OnUpdate()
{
	// No default implementation
}

/**
*  @brief
*    Called when string data has been dropped onto the frontend window
*/
void FrontendApplication::OnDrop(const Container<String> &lstFiles)
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Protected virtual CoreApplication functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when application should initialize it's configuration
*/
void FrontendApplication::OnInitConfig()
{
	// Call base implementation
	CoreApplication::OnInitConfig();

	// Read frontend configuration and set frontend window position and size of the previous session
	GetFrontend().SetWindowPositionSize(GetConfig().GetVarInt("PLCore::FrontendConfig", "X"),
										GetConfig().GetVarInt("PLCore::FrontendConfig", "Y"),
										GetConfig().GetVarInt("PLCore::FrontendConfig", "Width"),
										GetConfig().GetVarInt("PLCore::FrontendConfig", "Height"));
}

/**
*  @brief
*    Called when application should load it's plugins
*/
void FrontendApplication::OnInitPlugins()
{
	// The "Scan PL-runtime directory for compatible plugins and load them in"-part is job of a higher being

	// Scan for plugins in the application executable directory, but not recursively, please. This is
	// quite useful for shipping applications and putting all plugins inside the application root directory
	// (which is necessary due to VC manifest policy)
	ClassManager::GetInstance()->ScanPlugins(m_cApplicationContext.GetExecutableDirectory(), NonRecursive);

	// There's no guarantee that the application executable directory is the same as the application startup directory
	// -> If the application executable directory is not the same as the application startup directory, do also scan the application startup directory
	// -> This behavior is quite useful because it allows development of plugins which can be tested within e.g. PLViewer without copying files around,
	//    just set the current directory to your plugin directory when launching the viewer application
	const String sStartupDirectory = m_cApplicationContext.GetStartupDirectory();
	if (m_cApplicationContext.GetExecutableDirectory() != sStartupDirectory) {
		// Scan for plugins in the application startup directory, but not recursively, please
		ClassManager::GetInstance()->ScanPlugins(sStartupDirectory, NonRecursive);
	}
}

/**
*  @brief
*    Called when application should set it's data paths
*/
void FrontendApplication::OnInitData()
{
	// The "Scan PL-runtime directory for compatible data and register it"-part is job of a higher being

	// Is '.' (= the current directory) already a base directory? If not, add it right now...
	LoadableManager *pLoadableManager = LoadableManager::GetInstance();
	if (!pLoadableManager->IsBaseDir('.'))
		pLoadableManager->AddBaseDir('.');

	// Is the application directory already a base directory? If not, add it right now...
	if (!pLoadableManager->IsBaseDir(m_cApplicationContext.GetAppDirectory()))
		pLoadableManager->AddBaseDir(m_cApplicationContext.GetAppDirectory());

	// The application and current directories should have the highest priority of all base directories because the application data is most likely in those directories
	pLoadableManager->SetBaseDirPriority(m_cApplicationContext.GetAppDirectory(), pLoadableManager->GetBaseDir(0));
	pLoadableManager->SetBaseDirPriority('.', pLoadableManager->GetBaseDir(1));

	// Scan for packages in current "Data" directory
	const String sCurrentDir = System::GetInstance()->GetCurrentDir();
	pLoadableManager->ScanPackages(sCurrentDir + "/Data");

	// Scan for packages in application's "Data" directory
	if (sCurrentDir != m_cApplicationContext.GetAppDirectory())
		pLoadableManager->ScanPackages(m_cApplicationContext.GetAppDirectory() + "/Data");

	// Get localization language (from config or from default)
	String sLanguage = m_cConfig.GetVar("PLCore::CoreConfig", "Language");
	if (!sLanguage.GetLength()) {
		// Use always English instead of the current program locale language so that we have a known default behavior
		sLanguage = "English";
	}

	// Setup localization system
	Localization::GetInstance()->SetLanguage(sLanguage);
	if (sLanguage.GetLength()) {
		LocalizationGroup *pLocalizationGroup = Localization::GetInstance()->AddGroup(Localization::PixelLight);
		if (pLocalizationGroup)
			pLocalizationGroup->LoadByFilename("Data/Misc/" + Localization::PixelLight + '_' + sLanguage + ".loc");
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
