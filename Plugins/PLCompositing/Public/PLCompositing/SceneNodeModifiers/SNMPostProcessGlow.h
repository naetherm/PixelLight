/*********************************************************\
 *  File: SNMPostProcessGlow.h                           *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_GLOW_H__
#define __PLSCENE_SCENENODEMODIFIER_GLOW_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include "PLCompositing/SceneNodeModifiers/SNMPostProcess.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Glow camera scene node post process modifier class
*
*  @remarks
*    Glow effect.
*/
class SNMPostProcessGlow : public SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SNMPostProcessGlow, "PLCompositing", PLCompositing::SNMPostProcess, "Glow camera scene node post process modifier class")
		// Attributes
		pl_attribute(Strength,		PLMath::Vector2,	PLMath::Vector2(8.0f, 8.0f),	ReadWrite,	DirectValue,	"Blur strength",						"")
		pl_attribute(Luminance,		float,				0.08f,							ReadWrite,	DirectValue,	"Luminance",							"")
		pl_attribute(MiddleGray,	float,				0.18f,							ReadWrite,	DirectValue,	"Middle gray",							"")
		pl_attribute(WhiteCutoff,	float,				0.99f,							ReadWrite,	DirectValue,	"White cutoff",							"")
		pl_attribute(BloomScale,	float,				1.5f,							ReadWrite,	DirectValue,	"Bloom scale",							"")
			// Overloaded SNMPostProcess attributes
		pl_attribute(Filename,		PLCore::String,		"Data/PostProcesses/Glow.pp",	ReadWrite,	ModifyAttr,		"Filename of the post process to use",	"Type='PostProcess'")
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
		PLCOM_API SNMPostProcessGlow(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SNMPostProcessGlow();


	//[-------------------------------------------------------]
	//[ Public virtual SNMPostProcess functions               ]
	//[-------------------------------------------------------]
	public:
		PLCOM_API virtual void SetParameters() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLSCENE_SCENENODEMODIFIER_GLOW_H__
