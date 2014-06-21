/*********************************************************\
 *  File: TextureCreatorBlank2D.h                        *
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


#ifndef __PLRENDERER_TEXTURECREATOR_BLANK2D_H__
#define __PLRENDERER_TEXTURECREATOR_BLANK2D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
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
*    Blank 2D texture (TextureBuffer::R8G8B8) creator class
*/
class TextureCreatorBlank2D : public TextureCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, TextureCreatorBlank2D, "PLRenderer", PLRenderer::TextureCreator, "Blank 2D texture (TextureBuffer::R8G8B8) creator class")
		// Attributes
		pl_attribute(Width,			PLCore::uint32,		32,											ReadWrite,	DirectValue,	"Texture width (must be a power of 2)",		"Min='8'")
		pl_attribute(Height,		PLCore::uint32,		32,											ReadWrite,	DirectValue,	"Texture height (must be a power of 2)",	"Min='8'")
		pl_attribute(Components,	PLCore::uint8,		3,											ReadWrite,	DirectValue,	"Number of color components (1-4)",			"Min='1' Max='4'")
		pl_attribute(Color,			PLGraphics::Color4,	PLGraphics::Color4(0.0f, 0.0f, 0.0f, 1.0f),	ReadWrite,	DirectValue,	"Texture fill color",						"")
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
		PLRENDERER_API TextureCreatorBlank2D();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureCreatorBlank2D();


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


#endif // __PLRENDERER_TEXTURECREATOR_BLANK2D_H__
