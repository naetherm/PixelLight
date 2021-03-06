/*********************************************************\
 *  File: CoreConfigGroup.cpp                            *
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
#include "PLCore/Config/CoreConfigGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
// CoreConfigGroup
pl_class_metadata(CoreConfigGroup, "PLCore", PLCore::ConfigGroup, "PLCore general configuration 'collection' class")
pl_class_metadata_end(CoreConfigGroup)

// CoreConfig
pl_class_metadata(CoreConfig, "PLCore", PLCore::CoreConfigGroup, "PLCore configuration classes")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(FirstRun,	bool,	true,	ReadWrite,	"Is this the first application start?",																	"")
	pl_attribute_metadata(Language,	String,	"",		ReadWrite,	"Current used language, if empty the current set OS locale language is used (for instance 'German'",	"")
pl_class_metadata_end(CoreConfig)

// FrontendConfig
pl_class_metadata(FrontendConfig, "PLCore", PLCore::CoreConfigGroup, "PLCore frontend configuration classes")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(X,		int,	0,		ReadWrite,	"X position of the frontend (in screen coordinates)",	"")
	pl_attribute_metadata(Y,		int,	0,		ReadWrite,	"Y position of the frontend (in screen coordinates)",	"")
	pl_attribute_metadata(Width,	uint32,	800,	ReadWrite,	"Width of the frontend",								"")
	pl_attribute_metadata(Height,	uint32,	600,	ReadWrite,	"Height of the frontend",								"")
pl_class_metadata_end(FrontendConfig)


// CoreConfigGroup
//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
CoreConfigGroup::CoreConfigGroup()
{
}

/**
*  @brief
*    Destructor
*/
CoreConfigGroup::~CoreConfigGroup()
{
}




// CoreConfig
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
CoreConfig::CoreConfig() :
	FirstRun(this),
	Language(this)
{
}

/**
*  @brief
*    Destructor
*/
CoreConfig::~CoreConfig()
{
}




// FrontendConfig
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
FrontendConfig::FrontendConfig() :
	X(this),
	Y(this),
	Width(this),
	Height(this)
{
}

/**
*  @brief
*    Destructor
*/
FrontendConfig::~FrontendConfig()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
