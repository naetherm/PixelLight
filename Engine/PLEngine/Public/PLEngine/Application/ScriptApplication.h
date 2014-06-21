/*********************************************************\
 *  File: ScriptApplication.h                            *
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


#ifndef __PLENGINE_SCRIPTAPPLICATION_H__
#define __PLENGINE_SCRIPTAPPLICATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLEngine/Application/EngineApplication.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Script;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Script application class
*
*  @remarks
*    This class can load in and execute a given script out of the box.
*
*  @note
*    - If a script filename was given to the constructor, the script is started within the "OnInit()"-method
*    - Adds the global variable "this" to the script so that it's able to access "this" RTTI class instance
*/
class ScriptApplication : public EngineApplication {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, ScriptApplication, "PLEngine", PLEngine::EngineApplication, "Script application class")
		// Attributes
		pl_attribute(OnInitFunction,	PLCore::String,	"OnInit",	ReadWrite,	DirectValue,	"Name of the optional script function called by C++ when the application should initialize itself",		"")
		pl_attribute(OnUpdateFunction,	PLCore::String,	"OnUpdate",	ReadWrite,	DirectValue,	"Name of the optional script function called by C++ when the application should update itself",			"")
		pl_attribute(OnDeInitFunction,	PLCore::String,	"OnDeInit",	ReadWrite,	DirectValue,	"Name of the optional script function called by C++ when the application should de-initialize itself",	"")
		#ifdef PLENGINE_EXPORTS	// The following is only required when compiling PLEngine
			// Constructors
			pl_constructor_1(ParameterConstructor1,	PLCore::Frontend&,																	"Parameter constructor. Frontend this application instance is running in as first parameter.",																																																																				"")
			pl_constructor_2(ParameterConstructor2,	PLCore::Frontend&, PLCore::String,													"Parameter constructor. Frontend this application instance is running in as first parameter, parameter with the filename of the script to load as second parameter.",																																																		"")
			pl_constructor_5(ParameterConstructor5,	PLCore::Frontend&, PLCore::String,	PLCore::String,	PLCore::String,	PLCore::String,	"Parameter constructor. Frontend this application instance is running in as first parameter, parameter with the filename of the script to load as second parameter, the following parameters name, title and subdirectory for application data files are optional and will be constructed automatically by using the filename of the script if an empty string is given",	"")
			// Methods
			pl_method_0(GetBaseDirectory,	pl_ret_type(PLCore::String),							"Returns the base directory of the application (native path style, e.g. on Windows: 'C:\MyApplication\').",																		"")
			pl_method_1(SetBaseDirectory,	pl_ret_type(void),				const PLCore::String&,	"Sets the base directory of the application (e.g. on Windows: 'C:\MyApplication\'). Base directory as the first parameter.",													"")
			pl_method_0(GetScript,			pl_ret_type(PLCore::Script*),							"Returns the used script instance.",																																			"")
			pl_method_0(GetScriptFilename,	pl_ret_type(PLCore::String),							"Returns the absolute filename of the used script (native path style, e.g. on Windows: 'C:\MyApplication\Main.lua').",															"")
			pl_method_0(GetScriptDirectory,	pl_ret_type(PLCore::String),							"Returns the absolute directory the used script is in (native path style, e.g. on Windows: 'C:\MyApplication\' if currently the script 'C:\MyApplication\Main.lua' is used).",	"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*/
		PL_API ScriptApplication(PLCore::Frontend &cFrontend);

		/**
		*  @brief
		*    Constructor for loading in and executing a scripted stand-alone application using just a single line of C++ code
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*  @param[in] sScriptFilename
		*    Filename of the script to load
		*
		*  @remarks
		*  @verbatim
		*    Usage example:
		*    int PLMain(const String &sExecutableFilename, const Array<String> &lstArguments)
		*    {
		*        return ScriptApplication("Data/Scripts/45ScriptApplication.lua").Run(sExecutableFilename, lstArguments);
		*    }
		*  @endverbatim
		*/
		PL_API ScriptApplication(PLCore::Frontend &cFrontend, PLCore::String sScriptFilename);

		/**
		*  @brief
		*    Constructor for loading in and executing a scripted stand-alone application using just a single line of C++ code
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*  @param[in] sScriptFilename
		*    Filename of the script to load
		*  @param[in] sName
		*    Optional name of the application, if empty string a name is constructed automatically by using the filename of the script
		*  @param[in] sTitle
		*    Optional title of the application, if empty string a title is constructed automatically by using the filename of the script
		*  @param[in] sAppDataSubdir
		*    Optional subdirectory for application data files, if empty string a directory is constructed automatically by using the filename of the script
		*
		*  @remarks
		*  @verbatim
		*    Usage example:
		*    int PLMain(const String &sExecutableFilename, const Array<String> &lstArguments)
		*    {
		*        return ScriptApplication("Data/Scripts/45ScriptApplication.lua").Run(sExecutableFilename, lstArguments);
		*    }
		*  @endverbatim
		*/
		PL_API ScriptApplication(PLCore::Frontend &cFrontend, PLCore::String sScriptFilename, PLCore::String sName, PLCore::String sTitle, PLCore::String sAppDataSubdir);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~ScriptApplication();

		/**
		*  @brief
		*    Returns the base directory of the application
		*
		*  @return
		*    The base directory of the application (native path style, e.g. on Windows: 'C:\MyApplication\')
		*/
		PL_API PLCore::String GetBaseDirectory() const;

		/**
		*  @brief
		*    Sets the base directory of the application
		*
		*  @param[in] sBaseDirectory
		*    The base directory of the application (e.g. on Windows: 'C:\MyApplication\')
		*/
		PL_API void SetBaseDirectory(const PLCore::String &sBaseDirectory);

		/**
		*  @brief
		*    Returns the used script instance
		*
		*  @return
		*    Used script instance, can be a null pointer (do not destroy the returned instance)
		*/
		PL_API PLCore::Script *GetScript() const;

		/**
		*  @brief
		*    Returns the absolute filename of the used script
		*
		*  @return
		*    Absolute filename of the used script (native path style, e.g. on Windows: 'C:\MyApplication\Main.lua')
		*/
		PL_API PLCore::String GetScriptFilename() const;

		/**
		*  @brief
		*    Returns the absolute directory the used script is in
		*
		*  @return
		*    The absolute directory the used script is in (native path style, e.g. on Windows: 'C:\MyApplication\' if currently the script 'C:\MyApplication\Main.lua' is used)
		*/
		PL_API PLCore::String GetScriptDirectory() const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::CoreApplication functions   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when application should initialize itself
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that EngineApplication::OnInit() does
		*    - Load the script given to the constructor
		*    - Call optional <OnInitFunction> script function
		*    - Return and go on with Main()
		*/
		PL_API virtual void OnInit();

		/**
		*  @brief
		*    Called when application should de-initialize itself
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Call optional <OnDeInitFunction> script function
		*    - Destroy the script
		*    - Everything that EngineApplication::OnDeInit() does
		*/
		PL_API virtual void OnDeInit();


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::AbstractFrontend functions  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called to let the frontend update it's states
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that EngineApplication::OnUpdate() does
		*    - Call optional <OnUpdateFunction> script function
		*/
		PL_API virtual void OnUpdate() override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Loads a script
		*
		*  @param[in] sFilename
		*    Filename of the script to load
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Calls the optional script function <OnInitFunction>
		*/
		PL_API bool LoadScript(const PLCore::String &sFilename);

		/**
		*  @brief
		*    Destroys the currently used script
		*
		*  @note
		*    - Calls the optional script function <OnDeInitFunction>
		*/
		PL_API void DestroyScript();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String	 m_sInitialScriptFilename;		/**< Initial filename of the script to use */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String	 m_sCurrentSceneBaseDirectory;	/**< Base directory of the currently loaded scene */
		PLCore::String	 m_sScriptFilename;				/**< Absolute filename of the used script (native path style, e.g. on Windows: 'C:\MyApplication\Main.lua') */
		PLCore::Script	*m_pScript;						/**< Used script instance, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_SCRIPTAPPLICATION_H__
