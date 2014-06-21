/*********************************************************\
 *  File: SRPBackgroundColorGradient.h                   *
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


#ifndef __PLCOMPOSITING_BACKGROUNDCOLORGRADIENT_H__
#define __PLCOMPOSITING_BACKGROUNDCOLORGRADIENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color4.h>
#include "PLCompositing/SRPBackground.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Color gradient background scene renderer pass
*/
class SRPBackgroundColorGradient : public SRPBackground {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPBackgroundColorGradient, "PLCompositing", PLCompositing::SRPBackground, "Color gradient background scene renderer pass")
		// Attributes
		pl_attribute(Color1,	PLGraphics::Color4,	PLGraphics::Color4::White,	ReadWrite,	DirectValue,	"First color to use",																"")
		pl_attribute(Color2,	PLGraphics::Color4,	PLGraphics::Color4::Black,	ReadWrite,	DirectValue,	"Second color to use",																"")
		pl_attribute(Angle,		float,				135.0f,						ReadWrite,	DirectValue,	"Clockwise angle (in degree) of the gradient color. 0° means from left to right.",	"")
		pl_attribute(Position,	PLMath::Vector2,	PLMath::Vector2::Zero,		ReadWrite,	DirectValue,	"Gradient quad position (0..1), (0, 0)=left top",									"")
		pl_attribute(Size,		PLMath::Vector2,	PLMath::Vector2::One,		ReadWrite,	DirectValue,	"Gradient quad size (0..1)",														"")
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
		PLCOM_API SRPBackgroundColorGradient();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPBackgroundColorGradient();


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_BACKGROUNDCOLORGRADIENT_H__
