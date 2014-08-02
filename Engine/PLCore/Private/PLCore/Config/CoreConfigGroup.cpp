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
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_begin_class(CoreConfigGroup, PLCore)
	pl_base_class(PLCore::ConfigGroup)
	pl_desc("PLCore general configuration 'collection' class")
pl_end_class()

pl_begin_class(CoreConfig, PLCore)
	pl_base_class(PLCore::CoreConfigGroup)
	pl_desc("PLCore configuration classes")
	pl_ctor()

	pl_property(FirstRun)
		pl_desc("Is this the first application start?")
		pl_default(true)

	pl_property(Language)
		pl_desc("Current used language, if empty the current set OS locale language is used (for instance 'German')")

pl_end_class()

pl_begin_class(FrontendConfig, PLCore)
	pl_base_class(PLCore::ConfigGroup)
	pl_desc("PLCore frontend configuration classes")
	pl_ctor()

	pl_property(X)
		pl_desc("X position of the frontend (in screen coordinates)")
		pl_default(0)

	pl_property(Y)
		pl_desc("Y position of the frontend (in screen coordinates)")
		pl_default(0)

	pl_property(Width)
		pl_desc("Width of the frontend")
		pl_default(800)

	pl_property(Height)
		pl_desc("Height of the frontend")
		pl_default(600)

pl_end_class()


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


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
CoreConfig::CoreConfig()
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
FrontendConfig::FrontendConfig()
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