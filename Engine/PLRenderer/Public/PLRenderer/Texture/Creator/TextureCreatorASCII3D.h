/*********************************************************\
 *  File: TextureCreatorASCII3D.h                        *
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


#ifndef __PLRENDERER_TEXTURECREATOR_ASCII3D_H__
#define __PLRENDERER_TEXTURECREATOR_ASCII3D_H__
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
*    3D ASCII texture (TextureBuffer::L8) creator class
*
*  @note
*    - This texture can for instance be used for a ASCII post process
*      (all is rendered with ASCII characters :)
*    - The 2D ASCII texture (ASCIIFilename) MUST have a size of 256x256 and
*      MUST be luminance. (only one color component)
*    - Basing on the 'ASCII'-demo ASCII texture creation code from Humus.
*      (http://www.humus.name)
*/
class TextureCreatorASCII3D : public TextureCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, TextureCreatorASCII3D, "PLRenderer", PLRenderer::TextureCreator, "3D ASCII texture (TextureBuffer::L8) creator class")
		// Attributes
		pl_attribute(ASCIIFilename,	PLCore::String,	"",	ReadWrite,	DirectValue,	"2D ASCII texture filename",	"Type='Image'")
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
		PLRENDERER_API TextureCreatorASCII3D();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureCreatorASCII3D();


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


#endif // __PLRENDERER_TEXTURECREATOR_ASCII3D_H__
