/*********************************************************\
 *  File: SRPPostProcessing.cpp                          *
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
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Profiling.h>
#include <PLCore/Tools/Stopwatch.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/SceneNodeModifiers/SNMPostProcess.h"
#include "PLCompositing/Shaders/PostProcessing/PostProcessor.h"
#include "PLCompositing/Shaders/PostProcessing/PostProcessManager.h"
#include "PLCompositing/SRPPostProcessing.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPPostProcessing)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPPostProcessing::SRPPostProcessing() :
	m_pPostProcessor(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPPostProcessing::~SRPPostProcessing()
{
	if (m_pPostProcessor)
		delete m_pPostProcessor;
}

/**
*  @brief
*    Returns the post processor
*/
PostProcessor *SRPPostProcessing::GetPostProcessor()
{
	if (!m_pPostProcessor)
		m_pPostProcessor = new PostProcessor();
	return m_pPostProcessor;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether or not post processing is required
*/
bool SRPPostProcessing::IsPostProcessingRequired(SNCamera &cCamera)
{
	// Loop through all modifiers of the camera scene node
	uint32		    nModifier = 0;
	SNMPostProcess *pModifier = static_cast<SNMPostProcess*>(cCamera.GetModifier("PLCompositing::SNMPostProcess", nModifier));
	while (pModifier) {
		// Is this modifier active and the effect weight greater than zero?
		if (pModifier->IsActive() && pModifier->EffectWeight.GetFloat() > 0.0f) {


			// [TODO] If there's an "PLCompositing::SRPDeferredDOF" instance, "PLCompositing::SNMPostProcessDepthOfField" has already been processed -> Find a way to deal with build in post process effects!
			if (GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPDeferredDOF") && pModifier->GetClass()->GetClassName() == "PLCompositing::SNMPostProcessDepthOfField") {
				pModifier = static_cast<SNMPostProcess*>(cCamera.GetModifier("PLCompositing::SNMPostProcess", ++nModifier));
				continue;
			}


			// Get the post process manager
			PostProcessManager &cPPM = static_cast<SNMPostProcess*>(pModifier)->GetPostProcessManager();

			// Loop through all post processes
			for (uint32 nPostProcess=0; nPostProcess<cPPM.GetNumOfElements(); nPostProcess++) {
				PostProcess *pPostProcess = cPPM.GetByIndex(nPostProcess);
				if (pPostProcess->IsActive()) {
					// Create post processor if there's currently no one
					if (!m_pPostProcessor)
						m_pPostProcessor = new PostProcessor();

					// Setup the texture format
					m_pPostProcessor->SetTextureFormat(static_cast<SNMPostProcess*>(pModifier)->GetPostProcessManager().GetTextureFormat());

					// Done, we need post processing
					return true;
				}
			}
		}

		// Next modifier, please
		pModifier = static_cast<SNMPostProcess*>(cCamera.GetModifier("PLCompositing::SNMPostProcess", ++nModifier));
	}

	// Done, we need not need post processing
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPPostProcessing::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Perform post processing?
	if (SNCamera::GetCamera() && IsPostProcessingRequired(*SNCamera::GetCamera()) && m_pPostProcessor) {
		// Get the "PLCompositing::SRPBegin" instance
		SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
		if (pSRPBegin) {
			// We need up-to-date front render target content, so swap the render targets
			pSRPBegin->SwapRenderTargets();

			// Get the front render target of SRPBegin, this holds the current content
			SurfaceTextureBuffer *pFrontSurfaceTextureBuffer = pSRPBegin->GetFrontRenderTarget();
			if (pFrontSurfaceTextureBuffer && pFrontSurfaceTextureBuffer->GetTextureBuffer()) {
				// Backup the current render target
				Surface *pSurfaceBackup = cRenderer.GetRenderTarget();

				// Ensure alpha is used
				cRenderer.SetColorMask(true, true, true, true);

				// Start stopwatch
				Stopwatch cStopwatch(true);

				// Begin the processing
				if (m_pPostProcessor->BeginProcessing(cRenderer, *pFrontSurfaceTextureBuffer)) {
					// Perform all active post processes in order
					SNCamera *pCamera = SNCamera::GetCamera();
					if (pCamera) {
						// Loop through all modifiers of the camera scene node
						uint32		    nModifier = 0;
						SNMPostProcess *pModifier = static_cast<SNMPostProcess*>(pCamera->GetModifier("PLCompositing::SNMPostProcess", nModifier));
						while (pModifier) {
							// Is this modifier active and the effect weight greater than zero?
							if (pModifier->IsActive() && pModifier->EffectWeight.GetFloat() > 0.0f) {


								// [TODO] If there's an "PLCompositing::SRPDeferredDOF" instance, "PLCompositing::SNMPostProcessDepthOfField" has already been processed -> Find a way to deal with build in post process effects!
								if (GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPDeferredDOF") && pModifier->GetClass()->GetClassName() == "PLCompositing::SNMPostProcessDepthOfField") {
									pModifier = static_cast<SNMPostProcess*>(pCamera->GetModifier("PLCompositing::SNMPostProcess", ++nModifier));
									continue;
								}


								SNMPostProcess *pPostProcessModifier = static_cast<SNMPostProcess*>(pModifier);

								// Get the post process manager
								PostProcessManager &cPPM = pPostProcessModifier->GetPostProcessManager();

								// Loop through all post processes
								bool bSetParameters = true;
								for (uint32 nPostProcess=0; nPostProcess<cPPM.GetNumOfElements(); nPostProcess++) {
									PostProcess *pPostProcess = cPPM.GetByIndex(nPostProcess);
									if (pPostProcess->IsActive()) {
										// Set post process settings now?
										if (bSetParameters) {
											pPostProcessModifier->SetParameters();
											bSetParameters = false;
										}

										// Process this post process
										m_pPostProcessor->Process(*pPostProcess);
									}
								}
							}

							// Next modifier, please
							pModifier = static_cast<SNMPostProcess*>(pCamera->GetModifier("PLCompositing::SNMPostProcess", ++nModifier));
						}
					}

					// End the processing
					m_pPostProcessor->EndProcessing();

					// Reset current render target
					if (cRenderer.SetRenderTarget(pSurfaceBackup)) {
						// Draw the result on screen
						m_pPostProcessor->DrawResult(cRenderer);
					}
				}

				// Update profiling information
				Profiling::GetInstance()->Set(GetClass()->GetClassName(), "PostProcessTime", String::Format("%.3f ms", cStopwatch.GetMilliseconds()));
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
