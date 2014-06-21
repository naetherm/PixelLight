/*********************************************************\
 *  File: TextureCreatorPyroclasticCloud3D.h             *
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


#ifndef __PLVOLUME_TEXTURECREATOR_PYROCLASTICCLOUD3D_H__
#define __PLVOLUME_TEXTURECREATOR_PYROCLASTICCLOUD3D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include <PLRenderer/Texture/Creator/TextureCreator.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    3D pyroclastic cloud texture (TextureBuffer::L8) creator class
*
*  @note
*    - Basing on http://prideout.net/blog/?p=64 and http://magnuswrenninge.com/volumetricmethods (SIGGRAPH 2010 Course Notes "Volumetric Methods in Visual Effects")
*/
class TextureCreatorPyroclasticCloud3D : public PLRenderer::TextureCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, TextureCreatorPyroclasticCloud3D, "PLVolume", PLRenderer::TextureCreator, "3D pyroclastic cloud texture (TextureBuffer::L8) creator class")
		// PLVOLUME_RTTI_EXPORT
		pl_attribute(TextureSize,	PLCore::uint32,	128,	ReadWrite,	DirectValue,	"Texture size",	"Min='8'")
		pl_attribute(Radius,		float,			0.025f,	ReadWrite,	DirectValue,	"Radius",		"")
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
		TextureCreatorPyroclasticCloud3D();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureCreatorPyroclasticCloud3D();


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::TextureCreator functions  ]
	//[-------------------------------------------------------]
	private:
		virtual PLRenderer::Texture *Create(PLRenderer::TextureManager &cTextureManager, PLRenderer::Texture *pTexture = nullptr) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_TEXTURECREATOR_PYROCLASTICCLOUD3D_H__
