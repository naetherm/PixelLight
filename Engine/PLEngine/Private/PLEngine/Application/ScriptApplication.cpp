/*********************************************************\
 *  File: ScriptApplication.cpp                          *
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
#include <PLCore/File/File.h>
#include <PLCore/Script/Script.h>
#include <PLCore/Script/FuncScriptPtr.h>
#include <PLCore/Script/ScriptManager.h>
#include <PLCore/Tools/LoadableManager.h>
#include "PLEngine/Application/ScriptApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ScriptApplication)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptApplication::ScriptApplication(Frontend &cFrontend) : EngineApplication(cFrontend),
	OnInitFunction(this),
	OnUpdateFunction(this),
	OnDeInitFunction(this),
	m_pScript(nullptr)
{
}

/**
*  @brief
*    Constructor for loading in and executing a scripted stand-alone application using just a single line of C++ code
*/
ScriptApplication::ScriptApplication(Frontend &cFrontend, String sScriptFilename) : EngineApplication(cFrontend),
	OnInitFunction(this),
	OnUpdateFunction(this),
	OnDeInitFunction(this),
	m_sInitialScriptFilename(sScriptFilename),
	m_pScript(nullptr)
{
	// Get the script filename without directory
	const String sScriptFilenameOnly = Url(sScriptFilename).GetFilename();

	// Set application name and title
	SetName(sScriptFilenameOnly);
	SetTitle("PixelLight script application \"" + sScriptFilenameOnly + '\"');
}

/**
*  @brief
*    Constructor for loading in and executing a scripted stand-alone application using just a single line of C++ code
*/
ScriptApplication::ScriptApplication(Frontend &cFrontend, String sScriptFilename, String sName, String sTitle, String sAppDataSubdir) : EngineApplication(cFrontend),
	OnInitFunction(this),
	OnUpdateFunction(this),
	OnDeInitFunction(this),
	m_sInitialScriptFilename(sScriptFilename),
	m_pScript(nullptr)
{
	// Get the script filename without directory
	const String sScriptFilenameOnly = Url(sScriptFilename).GetFilename();

	// Set application name and title
	SetName(sName.GetLength() ? sName : sScriptFilenameOnly);
	SetTitle(sTitle.GetLength() ? sTitle : ("PixelLight script application \"" + sScriptFilenameOnly + '\"'));
}

/**
*  @brief
*    Destructor
*/
ScriptApplication::~ScriptApplication()
{
	// The used script instance is destroyed within "OnDeInit()"
}

/**
*  @brief
*    Returns the base directory of the application
*/
String ScriptApplication::GetBaseDirectory() const
{
	return m_sCurrentSceneBaseDirectory;
}

/**
*  @brief
*    Sets the base directory of the application
*/
void ScriptApplication::SetBaseDirectory(const String &sBaseDirectory)
{
	// Ensure the given directory is native path style so we have something we can rely on
	String sNewBaseDirectory = Url(sBaseDirectory).GetNativePath();

	// Is the given base directory absolute?
	if (!Url(sNewBaseDirectory).IsAbsolute()) {
		// Nope - if there's currently a script running, use it's absolute filename as start point
		if (m_pScript) {
			// Get the directory the script is in (e.g. a script filename of "C:/MyApplication/Main.lua" will result in "C:/MyApplication/")
			const String sDirectory = Url(m_sScriptFilename).CutFilename();

			// Construct the application base directory and ensure it's native path style so we have something we can rely on
			sNewBaseDirectory = Url(sDirectory + sNewBaseDirectory).Collapse().GetNativePath();
		}
	}

	// Remove the current application base directory from the loadable manager
	if (m_sCurrentSceneBaseDirectory.GetLength())
		LoadableManager::GetInstance()->RemoveBaseDir(m_sCurrentSceneBaseDirectory);

	// Set the new application base directory
	m_sCurrentSceneBaseDirectory = sNewBaseDirectory;

	// Add the given base directory to the loadable manager
	if (m_sCurrentSceneBaseDirectory.GetLength())
		LoadableManager::GetInstance()->AddBaseDir(m_sCurrentSceneBaseDirectory);
}

/**
*  @brief
*    Returns the used script instance
*/
Script *ScriptApplication::GetScript() const
{
	return m_pScript;
}

/**
*  @brief
*    Returns the absolute filename of the used script
*/
String ScriptApplication::GetScriptFilename() const
{
	return m_sScriptFilename;
}

/**
*  @brief
*    Returns the absolute directory the used script is in
*/
String ScriptApplication::GetScriptDirectory() const
{
	return m_sScriptFilename.GetLength() ? Url(Url(m_sScriptFilename).CutFilename()).GetNativePath() : "";
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::CoreApplication functions   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when application should initialize itself
*/
void ScriptApplication::OnInit()
{
	// Call base implementation
	EngineApplication::OnInit();

	// Is there a initial script filename?
	if (m_sInitialScriptFilename.GetLength())
		LoadScript(m_sInitialScriptFilename);
}

/**
*  @brief
*    Called when application should de-initialize itself
*/
void ScriptApplication::OnDeInit()
{
	// Destroy the currently used script
	DestroyScript();

	// Call base implementation
	EngineApplication::OnDeInit();
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractFrontend functions  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called to let the frontend update it's states
*/
void ScriptApplication::OnUpdate()
{
	// Call base implementation
	EngineApplication::OnUpdate();

	// Is there a script? If so, do also check whether or not our optional global script function is there.
	if (m_pScript && m_pScript->IsGlobalFunction(OnUpdateFunction.Get())) {
		// Call the update script function
		FuncScriptPtr<void>(m_pScript, OnUpdateFunction.Get()).Call(Params<void>());
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads a script
*/
bool ScriptApplication::LoadScript(const String &sFilename)
{
	// Destroy the currently used script
	DestroyScript();

	// Open the file
	File cFile;
	if (LoadableManager::GetInstance()->OpenFile(cFile, sFilename)) {
		// Create the script instance by using the extension of the given filename to detect the script language
		m_pScript = ScriptManager::GetInstance()->Create(ScriptManager::GetInstance()->GetScriptLanguageByExtension(Url(sFilename).GetExtension()));
		if (m_pScript) {
			// Backup the absolute script filename
			m_sScriptFilename = Url(cFile.GetUrl()).Collapse().GetNativePath();

			// Get the script source code
			const String sSourceCode = cFile.GetContentAsString();

			// Set the script source code
			if (!sSourceCode.GetLength() || !m_pScript->SetSourceCode(sSourceCode)) {
				// Error! Destroy the created script instance...
				delete m_pScript;
				m_pScript = nullptr;
				m_sScriptFilename = "";
			} else {
				// Add the global variable "this" to the script so that it's able to access "this" RTTI class instance
				m_pScript->SetGlobalVariable("this", Var<Object*>(this));

				// Call the initialize script function, but only when it's really there because it's optional
				if (m_pScript->IsGlobalFunction(OnInitFunction.Get()))
					FuncScriptPtr<void>(m_pScript, OnInitFunction.Get()).Call(Params<void>());

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Destroys the currently used script
*/
void ScriptApplication::DestroyScript()
{
	// Is there a script?
	if (m_pScript) {
		// Call the de-initialize script function, but only when it's really there because it's optional
		if (m_pScript->IsGlobalFunction(OnDeInitFunction.Get()))
			FuncScriptPtr<void>(m_pScript, OnDeInitFunction.Get()).Call(Params<void>());

		// Destroy the used script instance
		delete m_pScript;
		m_pScript = nullptr;
		m_sScriptFilename = "";
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
