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
pl_implement_class(ConfigGroup)
pl_implement_class(Config)
pl_implement_class(DebugConfig)


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
