/*********************************************************\
 *  File: ShaderFunctionClipPositionVolumeTexture.h      *
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


#ifndef __PLVOLUMERENDERER_SHADERFUNCTION_CLIPPOSITION_VOLUMETEXTURE_H__
#define __PLVOLUMERENDERER_SHADERFUNCTION_CLIPPOSITION_VOLUMETEXTURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/ClipPosition/ShaderFunctionClipPosition.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Volume texture shader function implementation class for clip position
*/
class ShaderFunctionClipPositionVolumeTexture : public ShaderFunctionClipPosition {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderFunctionClipPositionVolumeTexture, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionClipPosition, "Volume texture shader function implementation class for clip position")
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
		PLVOLUMERENDERER_API ShaderFunctionClipPositionVolumeTexture();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderFunctionClipPositionVolumeTexture();

		/**
		*  @brief
		*    Sets the clip volume texture parameters
		*
		*  @param[in] cProgram
		*    Used GPU program
		*  @param[in] cVolumeVisNode
		*    Volume visibility node
		*  @param[in] cVolumeTextureVisNode
		*    Clip volume texture visibility node
		*  @param[in] nIndex
		*    Clip volume texture uniform index, <0 if no index
		*/
		PLVOLUMERENDERER_API void SetVolumeTexture(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLScene::VisNode &cVolumeTextureVisNode, int nIndex);


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunctionClipPosition functions   ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual void SetVolumeTextures(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipVolumeTextures) override;


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunction functions               ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual PLCore::String GetSourceCode(const PLCore::String &sShaderLanguage, ESourceCodeType nSourceCodeType) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERFUNCTION_CLIPPOSITION_VOLUMETEXTURE_H__
