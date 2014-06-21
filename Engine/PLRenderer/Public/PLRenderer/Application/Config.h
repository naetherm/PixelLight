/*********************************************************\
 *  File: Config.h                                       *
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


#ifndef __PLRENDERER_CONFIG_H__
#define __PLRENDERER_CONFIG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Config/Config.h>
#include "PLRenderer/Renderer/Renderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Renderer configuration 'collection' class
*
*  @remarks
*    ALL renderer configuration classes are derived from this class.
*    Use cConfig.Save("Config/Renderer.cfg", "PLRenderer::ConfigGroup") to save
*    ONLY all renderer configuration classes.
*/
class ConfigGroup : public PLCore::ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, ConfigGroup, "PLRenderer", PLCore::ConfigGroup, "Renderer configuration 'collection' class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLRENDERER_API ConfigGroup();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~ConfigGroup();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ConfigGroup(const ConfigGroup &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		ConfigGroup &operator =(const ConfigGroup &cSource);


};

/**
*  @brief
*    Renderer configuration class
*/
class Config : public ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, Config, "PLRenderer", PLRenderer::ConfigGroup, "Renderer configuration class")
		// Attributes
			// General
	#ifdef ANDROID
		pl_attribute(RendererAPI,						PLCore::String,					"PLRendererOpenGLES2::Renderer",	ReadWrite,	DirectValue,	"The used renderer API",																																													"")
	#else
		pl_attribute(RendererAPI,						PLCore::String,					"PLRendererOpenGL::Renderer",		ReadWrite,	DirectValue,	"The used renderer API",																																													"")
	#endif	
		pl_attribute(RendererMode,						pl_enum_type(Renderer::EMode),	Renderer::ModeBoth,					ReadWrite,	DirectValue,	"The mode the renderer should run in (fixed functions, shaders or both)",																																	"")
		pl_attribute(Fullscreen,						bool,							false,								ReadWrite,	DirectValue,	"Fullscreen mode?",																																															"")
		pl_attribute(ZBufferBits,						PLCore::uint32,					24,									ReadWrite,	DirectValue,	"Z buffer bits (just a hint)",																																												"")
		pl_attribute(StencilBits,						PLCore::uint32,					8,									ReadWrite,	DirectValue,	"Stencil buffer bits (just a hint)",																																										"")
		pl_attribute(MultisampleAntialiasingSamples,	PLCore::uint32,					4,									ReadWrite,	DirectValue,	"Number of multisample antialiasing samples per pixel (just a hint)",																																		"")
		pl_attribute(DefaultShaderLanguage,				PLCore::String,					"",									ReadWrite,	DirectValue,	"The name of the default shader language of the renderer (for example \"GLSL\" or \"Cg\"), if the string is empty, the default is chosen by the renderer implementation, this information is just a hint",	"")
			// Display mode
		pl_attribute(DisplayWidth,						PLCore::uint32,					800,								ReadWrite,	DirectValue,	"Display width",																																															"")
		pl_attribute(DisplayHeight,						PLCore::uint32,					600,								ReadWrite,	DirectValue,	"Display height",																																															"")
		pl_attribute(DisplayColorBits,					PLCore::uint32,					32,									ReadWrite,	DirectValue,	"Color bits",																																																"")
		pl_attribute(DisplayFrequency,					PLCore::uint32,					0,									ReadWrite,	DirectValue,	"Display frequency",																																														"")
			// Misc
		pl_attribute(SwapInterval,						PLCore::uint32,					1,									ReadWrite,	DirectValue,	"The swap interval (vertical synchronization)",																																								"")
		pl_attribute(UseExtensions,						bool,							true,								ReadWrite,	DirectValue,	"Use extensions?",																																															"")
		pl_attribute(TextureQuality,					float,							1.0f,								ReadWrite,	DirectValue,	"Texture quality",																																															"Min='0.0' Max='1.0'")
		pl_attribute(TextureMipmaps,					bool,							true,								ReadWrite,	DirectValue,	"Use texture mipmaps by default?",																																											"")
		pl_attribute(TextureCompression,				bool,							true,								ReadWrite,	DirectValue,	"Use texture compression by default?",																																										"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLRENDERER_API Config();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Config();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Config(const Config &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Config &operator =(const Config &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_CONFIG_H__
