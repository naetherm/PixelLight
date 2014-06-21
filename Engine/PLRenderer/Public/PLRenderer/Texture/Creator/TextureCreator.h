/*********************************************************\
 *  File: TextureCreator.h                               *
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


#ifndef __PLRENDERER_TEXTURECREATOR_H__
#define __PLRENDERER_TEXTURECREATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Texture;
class TextureManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract texture creator base class
*/
class TextureCreator : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TextureManager;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, TextureCreator, "PLRenderer", PLCore::Object, "Abstract texture creator base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLRENDERER_API TextureCreator();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureCreator();

		/**
		*  @brief
		*    Creates the texture if a null pointer is given
		*
		*  @param[in] cTextureManager
		*    Texture manager to use
		*  @param[in] pTexture
		*    Pointer to the texture, if a null pointer a new texture is created
		*
		*  @return
		*   The created texture, a null pointer on error
		*/
		PLRENDERER_API Texture *CreateTexture(TextureManager &cTextureManager, Texture *pTexture) const;


	//[-------------------------------------------------------]
	//[ Protected virtual TextureCreator functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates a texture
		*
		*  @param[in] cTextureManager
		*    Texture manager to use
		*  @param[in] pTexture
		*    Texture to manipulate, if a null pointer a new texture is created
		*
		*  @return
		*   The created/manipulated texture, a null pointer on error
		*/
		virtual Texture *Create(TextureManager &cTextureManager, Texture *pTexture = nullptr) const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_TEXTURECREATOR_H__
