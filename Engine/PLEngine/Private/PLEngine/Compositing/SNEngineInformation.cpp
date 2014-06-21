/*********************************************************\
 *  File: SNEngineInformation.cpp                        *
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
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Profiling.h>
#include <PLMath/Intersect.h>
#include <PLInput/Input/InputManager.h>
#include <PLInput/Input/Devices/Keyboard.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLEngine/Application/EngineApplication.h"
#include "PLEngine/Compositing/SNEngineInformation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNEngineInformation)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNEngineInformation::GetProfilingMaterial() const
{
	return m_sProfilingMaterial;
}

void SNEngineInformation::SetProfilingMaterial(const String &sValue)
{
	if (m_sProfilingMaterial != sValue) {
		m_sProfilingMaterial = sValue;
		m_bProfilingMaterial = false;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNEngineInformation::SNEngineInformation() :
	InfoFlags(this),
	ProfilingMaterial(this),
	Flags(this),
	EventHandlerUpdate(&SNEngineInformation::OnUpdate, this),
	m_sProfilingMaterial("Data/Effects/PLProfiling.plfx"),
	m_bProfilingMaterial(false),
	m_pProfilingMaterial(new MaterialHandler())
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCulling);

	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNEngineInformation::~SNEngineInformation()
{
	// Destroy the profiling material handler
	delete m_pProfilingMaterial;
}

/**
*  @brief
*    Returns the handler of the profiling material
*/
const MaterialHandler &SNEngineInformation::GetProfilingMaterialHandler()
{
	if (!m_bProfilingMaterial) {
		m_pProfilingMaterial->SetResource(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sProfilingMaterial));
		m_bProfilingMaterial = true;
	}
	return *m_pProfilingMaterial;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the information of the current selected profile group
*/
void SNEngineInformation::DrawProfiling(Renderer &cRenderer)
{
	// Check if we have to draw the profiling information
	PLCore::Profiling *pProfiling = Profiling::GetInstance();
	if (pProfiling->IsActive()) {
		// Backup renderer states
		const uint32 nBlendEnable  = cRenderer.GetRenderState(RenderState::BlendEnable);
		const uint32 nSrcBlendFunc = cRenderer.GetRenderState(RenderState::SrcBlendFunc);
		const uint32 nDstBlendFunc = cRenderer.GetRenderState(RenderState::DstBlendFunc);

		// Begin 2D mode
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.Begin2DMode(0.0f, 0.0f, 800.0f, 600.0f);

			// Get group
			ProfileGroup *pGroup = pProfiling->GetByName(pProfiling->GetSelectedGroup());
			if (pGroup) { // Draw selected group
				// Draw the background material
				PLRenderer::Material *pMaterial = GetProfilingMaterialHandler().GetResource();
				if (pMaterial) {
					const float fY = static_cast<float>(25+pGroup->GetNumOfElements()*10);
					for (uint32 i=0; i<pMaterial->GetNumOfPasses(); i++) {
						pMaterial->SetupPass(i);
						Color4 cColor;
						FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
						if (pFixedFunctions)
							cColor = pFixedFunctions->GetColor();
						cDrawHelpers.DrawQuad(cColor, Vector2::Zero, Vector2(800.0f, fY));
					}
				}

				// Get the font
				Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
				if (pFont) {
					// Setup render states
					cRenderer.GetRendererContext().GetEffectManager().Use();
					cRenderer.SetRenderState(RenderState::ZEnable,      false);
					cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

					// Draw group name
					cDrawHelpers.DrawText(*pFont, pGroup->GetName(), Color4::White, Vector2(5.0f, 5.0f));

					// Draw all elements
					for (uint32 i=0, nY=15; i<pGroup->GetNumOfElements(); i++, nY+=10)
						cDrawHelpers.DrawText(*pFont, pGroup->GetByIndex(i)->GetName() + ": " + pGroup->GetByIndex(i)->GetText(), Color4::White, Vector2(10.0f, static_cast<float>(nY)));
				}
			} else { // Draw general information
				// Draw the background material
				PLRenderer::Material *pMaterial = GetProfilingMaterialHandler().GetResource();
				if (pMaterial) {
					for (uint32 i=0; i<pMaterial->GetNumOfPasses(); i++) {
						pMaterial->SetupPass(i);
						Color4 cColor;
						FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
						if (pFixedFunctions)
							cColor = pFixedFunctions->GetColor();
						cDrawHelpers.DrawQuad(cColor, Vector2::Zero, Vector2(800.0f, 55.0f));
					}
				}

				// Get the font
				Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
				if (pFont) {
					// Setup render states
					cRenderer.GetRendererContext().GetEffectManager().Use();
					cRenderer.SetRenderState(RenderState::ZEnable,      false);
					cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

					// Draw all elements
					cDrawHelpers.DrawText(*pFont, String::Format("Number of profile groups %d", pProfiling->GetNumOfElements()),			  Color4::White, Vector2(5.0f,  5.0f));
					cDrawHelpers.DrawText(*pFont, String::Format("Frames %d",					Timing::GetInstance()->GetPastFrames()),	  Color4::White, Vector2(5.0f, 15.0f));
					cDrawHelpers.DrawText(*pFont, String::Format("FPS %g",						Timing::GetInstance()->GetFramesPerSecond()), Color4::White, Vector2(5.0f, 25.0f));
					cDrawHelpers.DrawText(*pFont, String::Format("Time difference %g sec",		Timing::GetInstance()->GetTimeDifference()),  Color4::White, Vector2(5.0f, 35.0f));
				}
			}

		// End 2D mode
		cDrawHelpers.End2DMode();

		// Reset renderer states
		cRenderer.SetRenderState(RenderState::BlendEnable,  nBlendEnable);
		cRenderer.SetRenderState(RenderState::SrcBlendFunc, nSrcBlendFunc);
		cRenderer.SetRenderState(RenderState::DstBlendFunc, nDstBlendFunc);
	}
}

/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNEngineInformation::OnUpdate()
{
	// Check profiling keys
	if ((InfoFlags & Profiling) && Profiling::GetInstance()->IsActive()) {
		// Check if input is active
		// [TODO] Don't use devices directly, use a virtual controller instead
		CoreApplication *pCoreApplication = CoreApplication::GetApplication();
		if (pCoreApplication && pCoreApplication->IsInstanceOf("PLEngine::EngineApplication")) {
			Controller *pController = reinterpret_cast<Controller*>(static_cast<EngineApplication*>(pCoreApplication)->GetInputController());
			if ((pController && pController->GetActive()) || !pController) {
				// Get keyboard input device
				Keyboard *pKeyboard = InputManager::GetInstance()->GetKeyboard();
				if (pKeyboard) {
					if (pKeyboard->PageUp.IsHit())
						Profiling::GetInstance()->SelectPreviousGroup();
					if (pKeyboard->PageDown.IsHit())
						Profiling::GetInstance()->SelectNextGroup();
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNEngineInformation::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawPost(cRenderer, pVisNode);

	// Get draw helper instances
	DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();

	// Get configuration
	// [TODO] Uh oh, this is an assumption that can be false in so many ways. What if someone does not
	//        use the application framework at all, be it a plugin or some own solution? One more reason
	//        for not having a centralistic config approach...
	//        For now, I just make sure that if we can't get the application config, we use an empty one
	CoreApplication *pApplication = CoreApplication::GetApplication();
	Config cEmptyConfig;
	Config &cConfig = (pApplication ? pApplication->GetConfig() : cEmptyConfig);

	// Draw coordinate axis
	if ((InfoFlags & CoordinateAxis) &&
		cConfig.GetVar("PLEngine::DebugConfig", "ShowCoordinateAxis").GetBool()) {

		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

		// Get the projection matrix and the current view matrix
		Matrix4x4 mProjection;
		Matrix4x4 mCurrentView;
		{
			SNCamera *pCamera = SNCamera::GetCamera();
			if (pCamera) {
				mProjection = pCamera->GetProjectionMatrix(cRenderer.GetViewport());
				mCurrentView = pCamera->GetViewMatrix();
			}
		}
		Matrix4x4 mViewProjection = mProjection*mCurrentView;

		// Draw coordinate axis at the origin of the system
		cDrawHelpers.DrawLine(Color4::Red,   Vector3::Zero, Vector3::UnitX, mViewProjection, 1.0f);
		cDrawHelpers.DrawLine(Color4::Green, Vector3::Zero, Vector3::UnitY, mViewProjection, 1.0f);
		cDrawHelpers.DrawLine(Color4::Blue,  Vector3::Zero, Vector3::UnitZ, mViewProjection, 1.0f);

		// Draw texts
		Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		if (pFont) {
			Vector3 vV(1.0f, 0.0f, 0.0f);
			SNCamera *pCamera = SNCamera::GetCamera();
			// X-axis
			if (!pCamera || Intersect::PlaneSetPoint(pCamera->GetFrustum(cRenderer.GetViewport()), vV))
				cDrawHelpers.DrawText(*pFont, "X (1 unit)", Color4::Red, vV, mViewProjection, Font::CenterText);
			// Y-axis
			vV.SetXYZ(0.0f, 1.0f, 0.0f);
			if (!pCamera || Intersect::PlaneSetPoint(pCamera->GetFrustum(cRenderer.GetViewport()), vV))
				cDrawHelpers.DrawText(*pFont, "Y (1 unit)", Color4::Green, vV, mViewProjection, Font::CenterText);
			// Z-axis
			vV.SetXYZ(0.0f, 0.0f, 1.0f);
			if (!pCamera || Intersect::PlaneSetPoint(pCamera->GetFrustum(cRenderer.GetViewport()), vV))
				cDrawHelpers.DrawText(*pFont, "Z (1 unit)", Color4::Blue, vV, mViewProjection, Font::CenterText);
		}

		// Get the view matrix
		Matrix4x4 mView;
		mView.SetTranslation(-4.0f, -2.5f);

		// Get the world matrix
		Matrix4x4 mWorld = mCurrentView;
		mWorld.SetTranslation(0.0f, 0.0f, -10.0f);

		// Calculate the world view projection matrix
		Matrix4x4 mWorldViewProjection = mProjection;
		mWorldViewProjection *= mView;
		mWorldViewProjection *= mWorld;

		{ // Draw coordinate axis on the lower left of the screen showing the current rotation
			// Setup the render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Draw
			cDrawHelpers.DrawLine(Color4::Red,   Vector3::Zero, Vector3::UnitX, mWorldViewProjection, 1.0f);
			cDrawHelpers.DrawLine(Color4::Green, Vector3::Zero, Vector3::UnitY, mWorldViewProjection, 1.0f);
			cDrawHelpers.DrawLine(Color4::Blue,  Vector3::Zero, Vector3::UnitZ, mWorldViewProjection, 1.0f);
		}

		// Draw texts
		if (pFont) {
			cDrawHelpers.DrawText(*pFont, "X", Color4::Red,   Vector3::UnitX, mWorldViewProjection, Font::CenterText);
			cDrawHelpers.DrawText(*pFont, "Y", Color4::Green, Vector3::UnitY, mWorldViewProjection, Font::CenterText);
			cDrawHelpers.DrawText(*pFont, "Z", Color4::Blue,  Vector3::UnitZ, mWorldViewProjection, Font::CenterText);
		}
	}

	// Show planes
	if (InfoFlags & Planes) {
		// Check whether or not any plane visualization is enabled
		const bool bShowXZPlane = cConfig.GetVar("PLEngine::DebugConfig", "ShowXZPlane").GetBool();
		const bool bShowXYPlane = cConfig.GetVar("PLEngine::DebugConfig", "ShowXYPlane").GetBool();
		const bool bShowYZPlane = cConfig.GetVar("PLEngine::DebugConfig", "ShowYZPlane").GetBool();
		if (bShowXZPlane || bShowXYPlane || bShowYZPlane) {
			// Get the projection matrix and the current view matrix
			Matrix4x4 mProjection;
			Matrix4x4 mCurrentView;
			{
				SNCamera *pCamera = SNCamera::GetCamera();
				if (pCamera) {
					mProjection = pCamera->GetProjectionMatrix(cRenderer.GetViewport());
					mCurrentView = pCamera->GetViewMatrix();
				}
			}
			Matrix4x4 mViewProjection = mProjection*mCurrentView;

			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::CullMode,     Cull::None);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			cRenderer.SetRenderState(RenderState::BlendEnable,  true);
			if (bShowXZPlane) {
				cDrawHelpers.DrawQuad(Color4(0.0f, 1.0f, 0.0f, 0.5f),
									  Vector3( 10000.0f, 0.0f, -10000.0f),
									  Vector3(-10000.0f, 0.0f, -10000.0f),
									  Vector3( 10000.0f, 0.0f,  10000.0f),
									  Vector3(-10000.0f, 0.0f,  10000.0f),
									  mViewProjection, 0.0f);
			}
			if (bShowXYPlane) {
				cDrawHelpers.DrawQuad(Color4(0.0f, 0.0f, 1.0f, 0.5f),
									  Vector3( 10000.0f,  10000.0f, 0.0f),
									  Vector3(-10000.0f,  10000.0f, 0.0f),
									  Vector3( 10000.0f, -10000.0f, 0.0f),
									  Vector3(-10000.0f, -10000.0f, 0.0f),
									  mViewProjection, 0.0f);
			}
			if (bShowYZPlane) {
				cDrawHelpers.DrawQuad(Color4(1.0f, 0.0f, 0.0f, 0.5f),
									  Vector3(0.0f,  10000.0f, -10000.0f),
									  Vector3(0.0f,  10000.0f,  10000.0f),
									  Vector3(0.0f, -10000.0f, -10000.0f),
									  Vector3(0.0f, -10000.0f,  10000.0f),
									  mViewProjection, 0.0f);
			}
			cRenderer.SetRenderState(RenderState::ZWriteEnable, true);
		}
	}

	// Draw FPS
	if ((InfoFlags & FPS) && cConfig.GetVar("PLEngine::DebugConfig", "ShowFPS").GetBool()) {
		// Get the font
		Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		if (pFont) {
			// Setup render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Begin 2D mode
			cDrawHelpers.Begin2DMode(0.0f, 0.0f, 800.0f, 600.0f);

				// Draw the text
				cDrawHelpers.DrawText(*pFont, String::Format("FPS: %.2f", Timing::GetInstance()->GetFramesPerSecond()), Color4::White, Vector2(5.0f, 570.0f));

			// End 2D mode
			cDrawHelpers.End2DMode();
		}
	}

	// Draw profiling information
	if (InfoFlags & Profiling)
		DrawProfiling(cRenderer);
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SNEngineInformation::OnActivate(bool bActivate)
{
	// Call the base implementation
	SceneNode::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
