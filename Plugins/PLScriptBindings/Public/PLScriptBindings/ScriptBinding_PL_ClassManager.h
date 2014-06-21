/*********************************************************\
 *  File: ScriptBinding_PL_ClassManager.h                *
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


#ifndef __PLSCRIPTBINDINGS_PL_CLASSMANAGER_H__
#define __PLSCRIPTBINDINGS_PL_CLASSMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Script/ScriptBinding.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class ClassManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class manager script binding class
*/
class ScriptBinding_PL_ClassManager : public PLCore::ScriptBinding {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, ScriptBinding_PL_ClassManager, "PLScriptBindings", PLCore::ScriptBinding, "Class manager script binding class")
		// Properties
		pl_properties
			pl_property("Namespace",	"PL.ClassManager")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_3(ScanPlugins,			pl_ret_type(bool),				PLCore::String,	bool,			bool,							"Scan a directory for compatible plugins and load them in. Directory to search in as first parameter, boolean value deciding whether or not to take sub-directories into account as second parameter, boolean value deciding whether or not its allowed to perform delayed shared library loading to speed up the program start as third parameter. Returns 'true' if all went fine, else 'false'.",																																							"")
		pl_method_2(Create,					pl_ret_type(PLCore::Object*),	PLCore::String,	PLCore::String,									"Creates a new RTTI class instance by using the default constructor. Name of the RTTI class to create an instance from as first parameter and optional parameter string for the created instance as second parameter. Returns a pointer to the new RTTI class instance or a null pointer if something went wrong (maybe unknown class). Created instance has initially no references, meaning that a script usually automatically destroys the instance when no longer used.",																					"")
		pl_method_4(CreateByConstructor,	pl_ret_type(PLCore::Object*),	PLCore::String,	PLCore::String,	PLCore::String,	PLCore::String,	"Creates a new RTTI class instance by using a specified constructor. Name of the RTTI class to create an instance from as first parameter, constructor name as second parameter, constructor parameters as third parameter and optional parameter string for the created instance as fourth parameter. Returns a pointer to the new RTTI class instance or a null pointer if something went wrong (maybe unknown class). Created instance has initially no references, meaning that a script usually automatically destroys the instance when no longer used.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		bool ScanPlugins(PLCore::String sPath, bool bRecursive, bool bDelayedPluginLoading);
		PLCore::Object *Create(PLCore::String sClass, PLCore::String sParameters);
		PLCore::Object *CreateByConstructor(PLCore::String sClass, PLCore::String sConstructor, PLCore::String sConstructorParameters, PLCore::String sParameters);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ScriptBinding_PL_ClassManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ScriptBinding_PL_ClassManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::ClassManager *m_pClassManager;	/**< Class manager instance, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings


#endif // __PLSCRIPTBINDINGS_PL_CLASSMANAGER_H__
