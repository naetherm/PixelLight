/*********************************************************\
 *  File: ScriptBinding_PL_System.cpp                    *
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
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include "PLScriptBindings/ScriptBinding_PL_System.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ScriptBinding_PL_System, "PLScriptBindings", PLCore::ScriptBinding, "System script binding class")
	// Properties
	pl_properties
		pl_property("Namespace",	"PL.System")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_0_metadata(IsWindows,					pl_ret_type(bool),				"Returns 'true' if we're currently running on a Windows platform, else 'false'",						"")
	pl_method_0_metadata(IsLinux,					pl_ret_type(bool),				"Returns 'true' if we're currently running on a Linux platform, else 'false'",							"")
	pl_method_0_metadata(GetPlatformArchitecture,	pl_ret_type(PLCore::String),	"Returns the platform architecture (for instance 'x86', 'x64', 'armeabi', 'armeabi-v7a' and so on)",	"")
pl_class_metadata_end(ScriptBinding_PL_System)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ScriptBinding_PL_System::IsWindows()
{
	#ifdef WIN32
		return true;
	#else
		return false;
	#endif
}

bool ScriptBinding_PL_System::IsLinux()
{
	#ifdef LINUX
		return true;
	#else
		return false;
	#endif
}

String ScriptBinding_PL_System::GetPlatformArchitecture()
{
	return m_pSystem->GetPlatformArchitecture();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_PL_System::ScriptBinding_PL_System() :
	m_pSystem(System::GetInstance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_PL_System::~ScriptBinding_PL_System()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings
