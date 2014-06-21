/*********************************************************\
 *  File: ShadowMappingUSM.cpp                           *
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
#include <PLScene/Scene/SNSpotLight.h>
#include "PLCompositing/Shaders/ShadowMapping/ShadowMappingUSM.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShadowMappingUSM)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShadowMappingUSM::ShadowMappingUSM()
{
}

/**
*  @brief
*    Destructor
*/
ShadowMappingUSM::~ShadowMappingUSM()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShadowMapping functions                ]
//[-------------------------------------------------------]
void ShadowMappingUSM::CalculateLightMatrices(SNLight &cLight, Matrix4x4 &mLightView, Matrix4x4 &mLightProjection)
{
	// Spot or point light?
	if (cLight.IsSpotLight()) {
		// Cast to spot light
		SNSpotLight &cSpotLight = static_cast<SNSpotLight&>(cLight);

		// We "could" just reuse the view matrix of the light, but when for example constantly rotating the light around the z-axis,
		// the shadow mapping artifacts would also constantly move resulting in an annoying noisy shadow...
		// mLightView = cSpotLight.GetViewMatrix();
		// ... therefore we calculate a new view matrix for the shadow mapping with a fixed up-vector
		const Transform3 &cTransform = cLight.GetTransform();
		mLightView.LookAt(cTransform.GetPosition(), cTransform.GetPosition() + cTransform.GetRotation().GetZAxis(), Vector3::UnitY);

		// Just copy over the light projection matrix
		mLightProjection = cSpotLight.GetProjectionMatrix();
	} else {
		// Must be a directional light

		// [TODO] Implement me
		mLightView		 = Matrix4x4::Identity;
		mLightProjection = Matrix4x4::Identity;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
