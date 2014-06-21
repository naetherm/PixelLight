/*********************************************************\
 *  File: TextureCreatorNormalizationCubeMap.h           *
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


#ifndef __PLRENDERER_TEXTURECREATOR_NORMALIZATIONCUBEMAP_H__
#define __PLRENDERER_TEXTURECREATOR_NORMALIZATIONCUBEMAP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
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
*    Normalization cube map (TextureBuffer::R8G8B8) texture creator class
*
*  @remarks
*    This cube map texture can be used within shader programs to normalize vectors
*    through a fast texture access instead of a slower normalization using
*    math functions.
*/
class TextureCreatorNormalizationCubeMap : public TextureCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, TextureCreatorNormalizationCubeMap, "PLRenderer", PLRenderer::TextureCreator, "Normalization cube map (TextureBuffer::R8G8B8) texture creator class")
		// Attributes
		pl_attribute(Size,	PLCore::uint32,	64,	ReadWrite,	DirectValue,	"Texture size (must be a power of 2)",	"Min='8'")
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
		PLRENDERER_API TextureCreatorNormalizationCubeMap();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureCreatorNormalizationCubeMap();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Returns a cube vector
		*
		*  @param[in] i
		*    Cube map side
		*  @param[in] x
		*    Cube map x position
		*  @param[in] y
		*    Cube map y position
		*
		*  @return
		*    The cube requested vector
		*/
		PLMath::Vector3 GetCubeVector(PLCore::uint8 i, PLCore::uint32 x, PLCore::uint32 y) const;


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


#endif // __PLRENDERER_TEXTURECREATOR_NORMALIZATIONCUBEMAP_H__
