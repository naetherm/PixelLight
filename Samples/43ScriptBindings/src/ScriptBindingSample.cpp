/*********************************************************\
 *  File: ScriptBindingSample.cpp                        *
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
#include "ScriptBindingSample.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ScriptBindingSample, "", PLCore::ScriptBinding, "Sample script binding class, don't take it to serious")
	// Properties
	pl_properties
		pl_property("Namespace",	"Sample")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_0_metadata(Return42,				pl_ret_type(int),			"Returns 42",						"")
	pl_method_1_metadata(IgnoreTheParameter,	pl_ret_type(void), float,	"Ignores the provided parameter",	"")
	pl_method_0_metadata(SaySomethingWise,		pl_ret_type(void),			"Says something wise",				"")
pl_class_metadata_end(ScriptBindingSample)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
int ScriptBindingSample::Return42()
{
	return 42;
}

void ScriptBindingSample::IgnoreTheParameter(float fValue)
{
	System::GetInstance()->GetConsole().Print(String("Ignoring \"") + fValue + "\" ... D'OH!\n");
}

void ScriptBindingSample::SaySomethingWise()
{
	System::GetInstance()->GetConsole().Print(String(Return42()) + " - wise enough?\n");
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBindingSample::ScriptBindingSample()
{
}

/**
*  @brief
*    Destructor
*/
ScriptBindingSample::~ScriptBindingSample()
{
}
