/*********************************************************\
 *  File: SNMPostProcessDiagonalBlur.h                   *
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


#ifndef __PLPOSTPROCESSEFFECTS_DIAGONALBLUR_H__
#define __PLPOSTPROCESSEFFECTS_DIAGONALBLUR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLCompositing/SceneNodeModifiers/SNMPostProcess.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Diagonal blur camera scene node post process modifier class
*
*  @remarks
*    Performs a diagonal blur.
*/
class SNMPostProcessDiagonalBlur : public PLCompositing::SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SNMPostProcessDiagonalBlur, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
		// Attributes
		pl_attribute(ColorScale,	float,				4.0f,									ReadWrite,	DirectValue,	"Color scale",							"")
		pl_attribute(PixelKernel0,	PLMath::Vector2,	PLMath::Vector2( 2.0f, 2.0f),			ReadWrite,	DirectValue,	"Pixel kernel 0",						"")
		pl_attribute(PixelKernel1,	PLMath::Vector2,	PLMath::Vector2( 4.0f, 4.0f),			ReadWrite,	DirectValue,	"Pixel kernel 1",						"")
		pl_attribute(PixelKernel2,	PLMath::Vector2,	PLMath::Vector2(-6.0f, 6.0f),			ReadWrite,	DirectValue,	"Pixel kernel 2",						"")
		pl_attribute(PixelKernel3,	PLMath::Vector2,	PLMath::Vector2( 8.0f, 8.0f),			ReadWrite,	DirectValue,	"Pixel kernel 3",						"")
			// Overloaded SNMPostProcess attributes
		pl_attribute(Filename,		PLCore::String,		"Data/PostProcesses/DiagonalBlur.pp",	ReadWrite,	ModifyAttr,		"Filename of the post process to use",	"Type='PostProcess'")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		SNMPostProcessDiagonalBlur(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMPostProcessDiagonalBlur();


	//[-------------------------------------------------------]
	//[ Public virtual PLCompositing::SNMPostProcess functions ]
	//[-------------------------------------------------------]
	public:
		virtual void SetParameters() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects


#endif // __PLPOSTPROCESSEFFECTS_DIAGONALBLUR_H__
