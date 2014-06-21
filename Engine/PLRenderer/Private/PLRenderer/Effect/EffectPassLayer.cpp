/*********************************************************\
 *  File: EffectPassLayer.cpp                            *
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
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/Program.h"
#include "PLRenderer/Renderer/ProgramUniform.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/TextureHandler.h"
#include "PLRenderer/Material/Parameter.h"
#include "PLRenderer/Material/ParameterManager.h"
#include "PLRenderer/Effect/Effect.h"
#include "PLRenderer/Effect/EffectPass.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Effect/EffectTechnique.h"
#include "PLRenderer/Effect/EffectPassLayer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Makes the texture buffer of the texture handlers texture to the current renderer texture buffer
*/
bool EffectPassLayer::Bind(uint32 nStage, ParameterManager *pParameterManager) const
{
	// Get effect
	Effect &cEffect = m_pFXPass->GetTechnique().GetEffect();

	// Get renderer
	Renderer &cRenderer = cEffect.GetEffectManager().GetRendererContext().GetRenderer();

	// Bind texture
	if (!pParameterManager || !BindTexture(pParameterManager->GetParameter(m_sTexture), nStage))
		BindTexture(cEffect.GetParameterManager().GetParameter(m_sTexture), nStage);

	// Set sampler states
	for (uint32 i=0; i<Sampler::Number; i++)
		cRenderer.SetSamplerState(nStage, static_cast<Sampler::Enum>(i), m_cSamplerStates.Get(static_cast<Sampler::Enum>(i)));

	// Fixed functions
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		// Set texture stage states
		for (uint32 i=0; i<FixedFunctions::TextureStage::Number; i++)
			pFixedFunctions->SetTextureStageState(nStage, static_cast<FixedFunctions::TextureStage::Enum>(i), m_cFixedFunctionsTextureStageStates.Get(static_cast<FixedFunctions::TextureStage::Enum>(i)));
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Binds a texture
*/
bool EffectPassLayer::BindTexture(const Parameter *pParameter, uint32 nStage) const
{
	// Check parameter
	if (pParameter && pParameter->GetType() == Parameters::TextureBuffer) {
		// Get texture
		const TextureHandler *pTextureHandler = pParameter->GetValueTextureHandler();
		if (pTextureHandler) {
			const Texture *pTexture = pTextureHandler->GetTexture();
			bool bResult = true; // No error by default
			if (!pTexture) {
				// Use the default texture to avoid ugly graphics...
				pTexture = m_pFXPass->GetTechnique().GetEffect().GetEffectManager().GetRendererContext().GetTextureManager().GetStandard();
				if (!pTexture)
					return false; // Error! :(

				// Error!
				bResult = false;
			}
			TextureBuffer *pTextureBuffer = pTexture->GetTextureBuffer();
			if (pTextureBuffer) {
				{ // Set the GPU program uniform
					Program *pProgram = m_pFXPass->GetProgram();
					if (pProgram) {
						ProgramUniform *pProgramUniform = pProgram->GetUniform(pParameter->GetName());
						if (pProgramUniform && pProgramUniform->Set(pTextureBuffer) > -1) {
							// Done
							return bResult;
						}
					}
				}
				if (bResult)
					pTextureHandler->Bind(nStage);
				else {
					// Bind the fallback texture
					pTexture->Bind(nStage);

					// Fixed functions
					FixedFunctions *pFixedFunctions = pTextureBuffer->GetRenderer().GetFixedFunctions();
					if (pFixedFunctions) {
						// Set identity texture matrix
						pFixedFunctions->SetTransformState(static_cast<FixedFunctions::Transform::Enum>(static_cast<uint32>(FixedFunctions::Transform::Texture0) + nStage), Matrix4x4::Identity);
					}
				}

				// Done
				return bResult;
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
