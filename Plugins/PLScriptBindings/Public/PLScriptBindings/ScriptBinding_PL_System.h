/*********************************************************\
 *  File: ScriptBinding_PL_System.h                      *
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


#ifndef __PLSCRIPTBINDINGS_PL_SYSTEM_H__
#define __PLSCRIPTBINDINGS_PL_SYSTEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Script/ScriptBinding.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class System;
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
*    System script binding class
*/
class ScriptBinding_PL_System : public PLCore::ScriptBinding {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, ScriptBinding_PL_System, "PLScriptBindings", PLCore::ScriptBinding, "System script binding class")
		// Properties
		pl_properties
			pl_property("Namespace",	"PL.System")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_0(IsWindows,					pl_ret_type(bool),				"Returns 'true' if we're currently running on a Windows platform, else 'false'",						"")
		pl_method_0(IsLinux,					pl_ret_type(bool),				"Returns 'true' if we're currently running on a Linux platform, else 'false'",							"")
		pl_method_0(GetPlatformArchitecture,	pl_ret_type(PLCore::String),	"Returns the platform architecture (for instance 'x86', 'x64', 'armeabi', 'armeabi-v7a' and so on)",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		bool IsWindows();
		bool IsLinux();
		PLCore::String GetPlatformArchitecture();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ScriptBinding_PL_System();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ScriptBinding_PL_System();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const PLCore::System *m_pSystem;	/**< System instance, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings


#endif // __PLSCRIPTBINDINGS_PL_SYSTEM_H__
