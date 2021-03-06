/*********************************************************\
 *  File: SRPBackgroundColorGradient.cpp                 *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Math.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/SRPBackgroundColorGradient.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SRPBackgroundColorGradient, "PLCompositing", PLCompositing::SRPBackground, "Color gradient background scene renderer pass")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Color1,	PLGraphics::Color4,	PLGraphics::Color4::White,	ReadWrite,	"First color to use",																"")
	pl_attribute_metadata(Color2,	PLGraphics::Color4,	PLGraphics::Color4::Black,	ReadWrite,	"Second color to use",																"")
	pl_attribute_metadata(Angle,	float,				135.0f,						ReadWrite,	"Clockwise angle (in degree) of the gradient color. 0° means from left to right.",	"")
	pl_attribute_metadata(Position,	PLMath::Vector2,	PLMath::Vector2::Zero,		ReadWrite,	"Gradient quad position (0..1), (0, 0)=left top",									"")
	pl_attribute_metadata(Size,		PLMath::Vector2,	PLMath::Vector2::One,		ReadWrite,	"Gradient quad size (0..1)",														"")
pl_class_metadata_end(SRPBackgroundColorGradient)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPBackgroundColorGradient::SRPBackgroundColorGradient() :
	Color1(this),
	Color2(this),
	Angle(this),
	Position(this),
	Size(this)
{
}

/**
*  @brief
*    Destructor
*/
SRPBackgroundColorGradient::~SRPBackgroundColorGradient()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPBackgroundColorGradient::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Setup renderer states
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::ZEnable,			 false);
	cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

	// Begin 2D mode
	DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
	cDrawHelpers.Begin2DMode();

		// Draw the gradient quad, use degree instead of radian in here because humans tend to be able to imagine degree much better than radian
		cDrawHelpers.DrawGradientQuad(Color1.Get(), Color2.Get(), static_cast<float>(Angle.Get()*Math::DegToRad), Position.Get(), Size.Get());

	// End 2D mode
	cDrawHelpers.End2DMode();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
