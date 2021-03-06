/*********************************************************\
 *  File: Config.cpp                                     *
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
#include <PLCore/Tools/Timing.h>
#include "PLEngine/Application/Config.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_class_metadata(ConfigGroup, "PLEngine", PLCore::ConfigGroup, "Engine configuration 'collection' class")
pl_class_metadata_end(ConfigGroup)
pl_class_metadata(Config, "PLEngine", PLEngine::ConfigGroup, "Engine configuration classes")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	//Attributes
	pl_attribute_metadata(ConsoleActivated,		bool,	true,	ReadWrite,	"Is the console active?",										"")
	pl_attribute_metadata(MaxTimeDifference,	float,	0.15f,	ReadWrite,	"The maximum time difference since the last frame in seconds",	"Min='0.0001'")
	pl_attribute_metadata(FPSLimit,				float,	0.0f,	ReadWrite,	"FPS limit, 0 if there's no FPS limitation",					"Min='0.0'")
pl_class_metadata_end(Config)


pl_class_metadata(DebugConfig, "PLEngine", PLEngine::ConfigGroup, "Engine debug configuration classes")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	//Attributes
	pl_attribute_metadata(ShowFPS,				bool,	false,	ReadWrite,	"Show FPS?",				"")
	pl_attribute_metadata(ShowCoordinateAxis,	bool,	false,	ReadWrite,	"Show coordinate axis?",	"")
	pl_attribute_metadata(ShowXZPlane,			bool,	false,	ReadWrite,	"Show x/z plane?",			"")
	pl_attribute_metadata(ShowXYPlane,			bool,	false,	ReadWrite,	"Show x/y plane?",			"")
	pl_attribute_metadata(ShowYZPlane,			bool,	false,	ReadWrite,	"Show y/z plane?",			"")
pl_class_metadata_end(DebugConfig)


//[-------------------------------------------------------]
//[ Protected ConfigGroup functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ConfigGroup::ConfigGroup()
{
}

/**
*  @brief
*    Destructor
*/
ConfigGroup::~ConfigGroup()
{
}


//[-------------------------------------------------------]
//[ Private ConfigGroup functions                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ConfigGroup::ConfigGroup(const ConfigGroup &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
ConfigGroup &ConfigGroup::operator =(const ConfigGroup &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}




//[-------------------------------------------------------]
//[ Public Config functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Config::Config() :
	ConsoleActivated(this),
	MaxTimeDifference(this),
	FPSLimit(this)
{
}

/**
*  @brief
*    Destructor
*/
Config::~Config()
{
}


//[-------------------------------------------------------]
//[ Public Config RTTI get/set functions                  ]
//[-------------------------------------------------------]
float Config::GetMaxTimeDifference() const
{
	return PLCore::Timing::GetInstance()->GetMaxTimeDifference();
}

void Config::SetMaxTimeDifference(float fValue) const
{
	PLCore::Timing::GetInstance()->SetMaxTimeDifference(fValue);
}

float Config::GetFPSLimit() const
{
	return PLCore::Timing::GetInstance()->GetFPSLimit();
}

void Config::SetFPSLimit(float fValue) const
{
	PLCore::Timing::GetInstance()->SetFPSLimit(fValue);
}




//[-------------------------------------------------------]
//[ Public DebugConfig functions                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
DebugConfig::DebugConfig() :
	ShowFPS(this),
	ShowCoordinateAxis(this),
	ShowXZPlane(this),
	ShowXYPlane(this),
	ShowYZPlane(this)
{
}

/**
*  @brief
*    Destructor
*/
DebugConfig::~DebugConfig()
{
}




//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
