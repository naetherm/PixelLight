/*********************************************************\
 *  File: ConfigGroup.cpp                                *
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
#include "PLRenderer/Application/Config.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(ConfigGroup)
pl_implement_class(Config)


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
	// General
	RendererAPI(this),
	RendererMode(this),
	Fullscreen(this),
	ZBufferBits(this),
	StencilBits(this),
	MultisampleAntialiasingSamples(this),
	DefaultShaderLanguage(this),
	// Display mode
	DisplayWidth(this),
	DisplayHeight(this),
	DisplayColorBits(this),
	DisplayFrequency(this),
	// Misc
	SwapInterval(this),
	UseExtensions(this),
	TextureQuality(this),
	TextureMipmaps(this),
	TextureCompression(this)
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
//[ Private Config functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Config::Config(const Config &cSource) :
	// General
	RendererAPI(this),
	RendererMode(this),
	Fullscreen(this),
	ZBufferBits(this),
	StencilBits(this),
	MultisampleAntialiasingSamples(this),
	DefaultShaderLanguage(this),
	// Display mode
	DisplayWidth(this),
	DisplayHeight(this),
	DisplayColorBits(this),
	DisplayFrequency(this),
	// Misc
	SwapInterval(this),
	UseExtensions(this),
	TextureQuality(this),
	TextureMipmaps(this),
	TextureCompression(this)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Config &Config::operator =(const Config &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
