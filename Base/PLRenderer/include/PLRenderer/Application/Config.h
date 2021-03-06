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
	pl_class_def(PLRENDERER_API)
	pl_class_def_end


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
	pl_class_def(PLRENDERER_API)
		// Attributes
			// General
	#ifdef ANDROID
		pl_attribute_directvalue(RendererAPI,						PLCore::String,		"PLRendererOpenGLES2::Renderer",	ReadWrite)
	#else
		pl_attribute_directvalue(RendererAPI,						PLCore::String,		"PLRendererOpenGL::Renderer",		ReadWrite)
	#endif	
		pl_attribute_directvalue(RendererMode,						Renderer::EMode,	Renderer::ModeBoth,					ReadWrite)
		pl_attribute_directvalue(Fullscreen,						bool,				false,								ReadWrite)
		pl_attribute_directvalue(ZBufferBits,						PLCore::uint32,		24,									ReadWrite)
		pl_attribute_directvalue(StencilBits,						PLCore::uint32,		8,									ReadWrite)
		pl_attribute_directvalue(MultisampleAntialiasingSamples,	PLCore::uint32,		4,									ReadWrite)
		pl_attribute_directvalue(DefaultShaderLanguage,				PLCore::String,		"",									ReadWrite)
			// Display mode
		pl_attribute_directvalue(DisplayWidth,						PLCore::uint32,		800,								ReadWrite)
		pl_attribute_directvalue(DisplayHeight,						PLCore::uint32,		600,								ReadWrite)
		pl_attribute_directvalue(DisplayColorBits,					PLCore::uint32,		32,									ReadWrite)
		pl_attribute_directvalue(DisplayFrequency,					PLCore::uint32,		0,									ReadWrite)
			// Misc
		pl_attribute_directvalue(SwapInterval,						PLCore::uint32,		1,									ReadWrite)
		pl_attribute_directvalue(UseExtensions,						bool,				true,								ReadWrite)
		pl_attribute_directvalue(TextureQuality,					float,				1.0f,								ReadWrite)
		pl_attribute_directvalue(TextureMipmaps,					bool,				true,								ReadWrite)
		pl_attribute_directvalue(TextureCompression,				bool,				true,								ReadWrite)
	pl_class_def_end


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
