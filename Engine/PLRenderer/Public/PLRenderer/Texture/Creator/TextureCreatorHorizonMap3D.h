/*********************************************************\
 *  File: TextureCreatorHorizonMap3D.h                   *
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


#ifndef __PLRENDERER_TEXTURECREATOR_HORIZONMAP3D_H__
#define __PLRENDERER_TEXTURECREATOR_HORIZONMAP3D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Texture/Creator/TextureCreator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    3D horizon map texture (TextureBuffer::L8) creator class
*
*  @note
*    - This 3D horizon map texture can be used within shader programs for self shadowing.
*      For more information about this have a look at "Interactive Horizon Mapping".
*      (http://research.microsoft.com/~cohen/bs.pdf)
*    - The creation of horizons map shouldn't done at runtime because it's not
*      performant!
*/
class TextureCreatorHorizonMap3D : public TextureCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, TextureCreatorHorizonMap3D, "PLRenderer", PLRenderer::TextureCreator, "3D horizon map texture (TextureBuffer::L8) creator class")
		// Attributes
		pl_attribute(Filename2D,	PLCore::String,	"",		ReadWrite,	DirectValue,	"2D height map filename to create the horizon map for",	"")
		pl_attribute(XSize,			PLCore::uint32,	64,		ReadWrite,	DirectValue,	"Texture x size (must be a power of 2)",				"Min='8'")
		pl_attribute(YSize,			PLCore::uint32,	64,		ReadWrite,	DirectValue,	"Texture y size (must be a power of 2)",				"Min='8'")
		pl_attribute(ZSize,			PLCore::uint32,	64,		ReadWrite,	DirectValue,	"Texture z size (must be a power of 2)",				"Min='8'")
		pl_attribute(Height,		float,			0.16f,	ReadWrite,	DirectValue,	"Height",												"")
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
		PLRENDERER_API TextureCreatorHorizonMap3D();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureCreatorHorizonMap3D();


	//[-------------------------------------------------------]
	//[ Private virtual TextureCreator functions              ]
	//[-------------------------------------------------------]
	private:
		virtual Texture *Create(TextureManager &cTextureManager, Texture *pTexture = nullptr) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_TEXTURECREATOR_HORIZONMAP3D_H__
