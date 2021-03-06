/*********************************************************\
 *  File: ScriptBinding_PL.cpp                           *
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
#include <PLCore/Application/CoreApplication.h>
#include "PLScriptBindings/ScriptBinding_PL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ScriptBinding_PL, "PLScriptBindings", PLCore::ScriptBinding, "PixelLight script binding class")
	// Properties
	pl_properties
		pl_property("Namespace",	"PL")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_0_metadata(GetApplication,	pl_ret_type(PLCore::CoreApplication*),	"Returns the application instance, can be a null pointer (but \"usually\" it's no null pointer)",	"")
pl_class_metadata_end(ScriptBinding_PL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the application instance
*/
CoreApplication *ScriptBinding_PL::GetApplication()
{
	return CoreApplication::GetApplication();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_PL::ScriptBinding_PL()
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_PL::~ScriptBinding_PL()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings
