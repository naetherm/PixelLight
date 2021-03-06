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
pl_class_metadata(ConfigGroup, "PLRenderer", PLCore::ConfigGroup, "Renderer configuration 'collection' class")
pl_class_metadata_end(ConfigGroup)
pl_class_metadata(Config, "PLRenderer", PLRenderer::ConfigGroup, "Renderer configuration class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	#ifdef ANDROID
	pl_attribute_metadata(RendererAPI,						PLCore::String,						"PLRendererOpenGLES2::Renderer",	ReadWrite,	"The used renderer API",																																													"")
	#else
	pl_attribute_metadata(RendererAPI,						PLCore::String,						"PLRendererOpenGL::Renderer",		ReadWrite,	"The used renderer API",																																													"")
	#endif	
	pl_attribute_metadata(RendererMode,						pl_enum_type_def3(Renderer, EMode),	Renderer::ModeBoth,					ReadWrite,	"The mode the renderer should run in (fixed functions, shaders or both)",																																	"")
	pl_attribute_metadata(Fullscreen,						bool,								false,								ReadWrite,	"Fullscreen mode?",																																															"")
	pl_attribute_metadata(ZBufferBits,						PLCore::uint32,						24,									ReadWrite,	"Z buffer bits (just a hint)",																																												"")
	pl_attribute_metadata(StencilBits,						PLCore::uint32,						8,									ReadWrite,	"Stencil buffer bits (just a hint)",																																										"")
	pl_attribute_metadata(MultisampleAntialiasingSamples,	PLCore::uint32,						4,									ReadWrite,	"Number of multisample antialiasing samples per pixel (just a hint)",																																		"")
	pl_attribute_metadata(DefaultShaderLanguage,			PLCore::String,						"",									ReadWrite,	"The name of the default shader language of the renderer (for example \"GLSL\" or \"Cg\"), if the string is empty, the default is chosen by the renderer implementation, this information is just a hint",	"")
		// Display mode
	pl_attribute_metadata(DisplayWidth,						PLCore::uint32,						800,								ReadWrite,	"Display width",																																															"")
	pl_attribute_metadata(DisplayHeight,					PLCore::uint32,						600,								ReadWrite,	"Display height",																																															"")
	pl_attribute_metadata(DisplayColorBits,					PLCore::uint32,						32,									ReadWrite,	"Color bits",																																																"")
	pl_attribute_metadata(DisplayFrequency,					PLCore::uint32,						0,									ReadWrite,	"Display frequency",																																														"")
		// Misc
	pl_attribute_metadata(SwapInterval,						PLCore::uint32,						1,									ReadWrite,	"The swap interval (vertical synchronization)",																																								"")
	pl_attribute_metadata(UseExtensions,					bool,								true,								ReadWrite,	"Use extensions?",																																															"")
	pl_attribute_metadata(TextureQuality,					float,								1.0f,								ReadWrite,	"Texture quality",																																															"Min='0.0' Max='1.0'")
	pl_attribute_metadata(TextureMipmaps,					bool,								true,								ReadWrite,	"Use texture mipmaps by default?",																																											"")
	pl_attribute_metadata(TextureCompression,				bool,								true,								ReadWrite,	"Use texture compression by default?",																																										"")
pl_class_metadata_end(Config)


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
