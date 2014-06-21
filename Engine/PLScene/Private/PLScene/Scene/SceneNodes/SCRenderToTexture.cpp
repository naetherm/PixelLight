/*********************************************************\
 *  File: SCRenderToTexture.cpp                          *
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
#include <PLCore/System/System.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Renderer/SurfacePainter.h>
#include <PLRenderer/Renderer/SurfaceTextureBuffer.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include "PLScene/Scene/SPScene.h"
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/SCRenderToTexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SCRenderToTexture)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
bool SCRenderToTexture::GetCube() const
{
	return m_bCube;
}

void SCRenderToTexture::SetCube(bool bValue)
{
	if (m_bCube != bValue) {
		m_bCube = bValue;
		if (IsInitialized())
			CreateSurfaceTexture();
	}
}

uint16 SCRenderToTexture::GetWidth() const
{
	return m_nWidth;
}

void SCRenderToTexture::SetWidth(uint16 nValue)
{
	if (m_nWidth != nValue) {
		m_nWidth = nValue;
		if (IsInitialized())
			CreateSurfaceTexture();
	}
}

uint16 SCRenderToTexture::GetHeight() const
{
	return m_nHeight;
}

void SCRenderToTexture::SetHeight(uint16 nValue)
{
	if (m_nHeight != nValue) {
		m_nHeight = nValue;
		if (IsInitialized())
			CreateSurfaceTexture();
	}
}

SCRenderToTexture::EFormat SCRenderToTexture::GetFormat() const
{
	return m_nFormat;
}

void SCRenderToTexture::SetFormat(EFormat nValue)
{
	if (static_cast<int>(m_nFormat) != nValue) {
		m_nFormat = nValue;
		if (IsInitialized())
			CreateSurfaceTexture();
	}
}

uint32 SCRenderToTexture::GetSurfaceFlags() const
{
	return m_nSurfaceFlags;
}

void SCRenderToTexture::SetSurfaceFlags(uint32 nValue)
{
	if (m_nSurfaceFlags != nValue) {
		m_nSurfaceFlags = nValue;
		if (IsInitialized())
			CreateSurfaceTexture();
	}
}

String SCRenderToTexture::GetTextureName() const
{
	return m_sTextureName;
}

void SCRenderToTexture::SetTextureName(const String &sValue)
{
	if (m_sTextureName != sValue) {
		m_sTextureName = sValue;

		// Rename the resulting texture
		Texture *pTexture = m_pTextureHandler->GetResource();
		if (pTexture)
			pTexture->SetName(m_sTextureName);
	}
}

String SCRenderToTexture::GetPainter() const
{
	return m_sPainter;
}

void SCRenderToTexture::SetPainter(const String &sValue)
{
	if (m_sPainter != sValue) {
		m_sPainter = sValue;

		// Setup the surface painter
		if (m_pSurfaceTextureBuffer) {
			SurfacePainter *pSurfacePainter = GetSceneContext()->GetRendererContext().GetRenderer().CreateSurfacePainter(m_sPainter);
			m_pSurfaceTextureBuffer->SetPainter(pSurfacePainter);
			if (pSurfacePainter && pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
				// Do NOT let the renderer update this surface by default, WE update it inside DrawPre()
				m_pSurfaceTextureBuffer->SetActive(false);

				// THIS is the scene root :)
				SPScene *pPainter = static_cast<SPScene*>(pSurfacePainter);
				pPainter->SetRootContainer(this);

				// Tell the surface scene painter about the 'conrete scene'
				SceneNode *pSceneNode = GetByName(m_sSceneName);
				if (pSceneNode && pSceneNode->IsContainer())
					pPainter->SetSceneContainer(static_cast<SceneContainer*>(pSceneNode));
				else
					pPainter->SetSceneContainer(nullptr);

				// Set default scene renderer
				pPainter->SetDefaultSceneRenderer(m_sSceneRenderer);
			}
		}
	}
}

String SCRenderToTexture::GetSceneRenderer() const
{
	return m_sSceneRenderer;
}

void SCRenderToTexture::SetSceneRenderer(const String &sValue)
{
	if (m_sSceneRenderer != sValue) {
		m_sSceneRenderer = sValue;

		// Setup the surface painter
		if (m_pSurfaceTextureBuffer) {
			SurfacePainter *pSurfacePainter = m_pSurfaceTextureBuffer->GetPainter();
			if (pSurfacePainter && pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
				SPScene *pPainter = static_cast<SPScene*>(pSurfacePainter);
				pPainter->SetDefaultSceneRenderer(m_sSceneRenderer);
			}
		}
	}
}

String SCRenderToTexture::GetSceneName() const
{
	return m_sSceneName;
}

void SCRenderToTexture::SetSceneName(const String &sValue)
{
	if (m_sSceneName != sValue) {
		m_sSceneName = sValue;

		// Setup the surface painter
		if (m_pSurfaceTextureBuffer) {
			SurfacePainter *pSurfacePainter = m_pSurfaceTextureBuffer->GetPainter();
			if (pSurfacePainter && pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
				SPScene *pPainter = static_cast<SPScene*>(pSurfacePainter);
				SceneNode *pSceneNode = GetByName(m_sSceneName);
				if (pSceneNode && pSceneNode->IsContainer())
					pPainter->SetSceneContainer(static_cast<SceneContainer*>(pSceneNode));
				else
					pPainter->SetSceneContainer(nullptr);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SCRenderToTexture::SCRenderToTexture() :
	Cube(this),
	Width(this),
	Height(this),
	Format(this),
	SurfaceFlags(this),
	TextureName(this),
	FPSLimit(this),
	Painter(this),
	SceneRenderer(this),
	SceneName(this),
	CameraName(this),
	Flags(this),
	DebugFlags(this),
	m_bCube(false),
	m_nWidth(512),
	m_nHeight(512),
	m_nFormat(R8G8B8A8),
	m_nSurfaceFlags(Depth|Mipmaps),
	m_sPainter("PLScene::SPScene"),
	m_sSceneRenderer("Forward.sr"),
	m_sSceneName("Parent"),
	m_nFPSLimitLastTime(0),
	m_pSurfaceTextureBuffer(nullptr),
	m_pTextureHandler(new TextureHandler())
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCulling|NoRecursion);

	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPre));
}

/**
*  @brief
*    Destructor
*/
SCRenderToTexture::~SCRenderToTexture()
{
	// Delete render to texture buffer stuff
	if (m_pSurfaceTextureBuffer) {
		delete m_pSurfaceTextureBuffer;
		m_pSurfaceTextureBuffer = nullptr;
	}

	// Destroy the texture handler
	if (m_pTextureHandler->GetResource())
		delete m_pTextureHandler->GetResource();
	delete m_pTextureHandler;
}

/**
*  @brief
*    Returns the texture buffer surface we render in
*/
SurfaceTextureBuffer *SCRenderToTexture::GetSurfaceTextureBuffer() const
{
	return m_pSurfaceTextureBuffer;
}

/**
*  @brief
*    Returns the resulting texture
*/
Texture *SCRenderToTexture::GetTexture() const
{
	return m_pTextureHandler->GetResource();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates/recreates the texture surface
*/
void SCRenderToTexture::CreateSurfaceTexture()
{
	// Get the renderer
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

	// Delete the old render to texture buffer surface
	if (m_pSurfaceTextureBuffer) {
		delete m_pSurfaceTextureBuffer;
		m_pSurfaceTextureBuffer = nullptr;
	}

	// Delete the old resulting texture
	Texture *pTexture = m_pTextureHandler->GetResource();
	if (pTexture) {
		pTexture->Delete();
		m_pTextureHandler->SetResource(nullptr);
	}

	// Get the desired texture buffer format
	TextureBuffer::EPixelFormat nFormat;
	switch (m_nFormat) {
		case 0:
			nFormat = TextureBuffer::R8G8B8;
			break;

		case 1:
			nFormat = TextureBuffer::R8G8B8A8;
			break;

		default:
			nFormat = TextureBuffer::R8G8B8A8;
			break;
	}

	// Create 'render to texture buffer' surface
	if (m_bCube) {
		m_pSurfaceTextureBuffer = cRenderer.CreateSurfaceTextureBufferCube(m_nWidth, nFormat, m_nSurfaceFlags);
	} else {
		// If possible, we use a standard 2D texture buffer
		if (cRenderer.GetCapabilities().bTextureBufferNonPowerOfTwo || (Math::IsPowerOfTwo(m_nWidth) && Math::IsPowerOfTwo(m_nHeight)))
			m_pSurfaceTextureBuffer = cRenderer.CreateSurfaceTextureBuffer2D(Vector2i(m_nWidth, m_nHeight), nFormat, m_nSurfaceFlags);
		else
			m_pSurfaceTextureBuffer = cRenderer.CreateSurfaceTextureBufferRectangle(Vector2i(m_nWidth, m_nHeight), nFormat, m_nSurfaceFlags);
	}
	if (m_pSurfaceTextureBuffer) {
		// Setup the surface painter
		SurfacePainter *pSurfacePainter = cRenderer.CreateSurfacePainter(m_sPainter);
		m_pSurfaceTextureBuffer->SetPainter(pSurfacePainter);
		if (pSurfacePainter && pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
			// Do NOT let the renderer update this surface by default, WE update it inside DrawPre()
			m_pSurfaceTextureBuffer->SetActive(false);

			// THIS is the scene root :)
			SPScene *pPainter = static_cast<SPScene*>(pSurfacePainter);
			pPainter->SetRootContainer(this);

			// Tell the surface scene painter about the 'conrete scene'
			SceneNode *pSceneNode = GetByName(m_sSceneName);
			pPainter->SetSceneContainer((pSceneNode && pSceneNode->IsContainer()) ? static_cast<SceneContainer*>(pSceneNode) : nullptr);

			// Set default scene renderer
			pPainter->SetDefaultSceneRenderer(m_sSceneRenderer);
		}
	}

	// Add the texture
	TextureManager &cTextureManager = cRenderer.GetRendererContext().GetTextureManager();
	// If there's already a texture with this name we have to get another, still free resource name
	if (cTextureManager.GetByName(m_sTextureName)) {
		// Find an unused resource name
		String sName = m_sTextureName + "_0";
		for (uint32 i=1; cTextureManager.GetByName(sName); i++)
			sName = m_sTextureName + '_' + static_cast<int>(i);

		// We have found an unused name
		m_sTextureName = sName;
	}
	if (m_pSurfaceTextureBuffer && m_pSurfaceTextureBuffer->GetTextureBuffer()) {
		pTexture = cTextureManager.CreateTexture(m_sTextureName, *m_pSurfaceTextureBuffer->GetTextureBuffer());
		if (pTexture)
			m_sTextureName = pTexture->GetName();
		m_pTextureHandler->SetResource(pTexture);
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SCRenderToTexture::DrawPre(Renderer &cRenderer, const VisNode *pVisNode)
{
	if (IsActive() && m_pSurfaceTextureBuffer) {
		// Call base implementation
		SceneContainer::DrawPre(cRenderer, pVisNode);

		// Check FPS limitation
		if (FPSLimit) {
			const uint64 nCurrentTime = System::GetInstance()->GetMilliseconds();
			if (m_nFPSLimitLastTime && nCurrentTime-m_nFPSLimitLastTime < static_cast<uint64>(1000.0f/FPSLimit))
				return; // Do not update
			m_nFPSLimitLastTime = nCurrentTime;
		}

		// Get the surface scene painter
		SurfacePainter *pSurfacePainter = m_pSurfaceTextureBuffer->GetPainter();
		if (pSurfacePainter && pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
			SPScene *pPainter = static_cast<SPScene*>(pSurfacePainter);

			// Deactivate THIS node to avoid recursion
			SetActive(false);

			// Get current camera
			SNCamera *pCameraBackup = SNCamera::GetCamera();

			// Set camera
			SceneNode *pSceneNode = GetByName(CameraName.Get());
			if (pSceneNode && pSceneNode->IsCamera()) {
				// If the camera is not active, we do not need to do anything in here...
				if (!pSceneNode->IsActive())
					return;

				pPainter->SetCamera(static_cast<SNCamera*>(pSceneNode));
				static_cast<SNCamera*>(pSceneNode)->SetCamera(cRenderer);
			} else
				pPainter->SetCamera(nullptr);

			// Get current render target
			Surface *pSurfaceBackup = cRenderer.GetRenderTarget();


			// [HACK] If the painter has no scene container, check if the scene container exists now...
			if (!pPainter->GetSceneContainer()) {
				pSceneNode = GetByName(m_sSceneName);
				if (pSceneNode && pSceneNode->IsContainer())
					pPainter->SetSceneContainer(static_cast<SceneContainer*>(pSceneNode));
			}


			// Check the render target type
			if (m_bCube) { // Render to cube texture
				// Setup the camera
				SNCamera *pCamera = pPainter->GetCamera();
				if (pCamera) {
					// Set field of view and aspect
					pCamera->SetFOV(90.0f);
					pCamera->SetAspect(1.0f);

					// For each of the 6 cube faces...
					for (uint8 nFace=0; nFace<6; nFace++) {
						// Set rotation
						switch (nFace) {
							// x-positive
							case 0:
								pCamera->SetRotation(Vector3(  0.0f, -90.0f, 180.0f));
								break;

							// x-negative
							case 1:
								pCamera->SetRotation(Vector3(  0.0f,  90.0f, 180.0f));
								break;

							// y-positive
							case 2:
								pCamera->SetRotation(Vector3( 90.0f,   0.0f, 180.0f));
								break;

							// y-negative
							case 3:
								pCamera->SetRotation(Vector3(-90.0f,   0.0f, 180.0f));
								break;

							// z-positive
							case 4:
								pCamera->SetRotation(Vector3(  0.0f,   0.0f, 180.0f));
								break;

							// z-negative
							case 5:
								pCamera->SetRotation(Vector3(  0.0f, 180.0f, 180.0f));
								break;
						}

						// Set the new render target
						if (cRenderer.SetRenderTarget(m_pSurfaceTextureBuffer, nFace)) {
							// Draw a nice and colorful picture :)
							pPainter->OnPaint(*m_pSurfaceTextureBuffer);
						}
					}
				} else {
					// For each of the 6 cube faces...
					for (uint8 nFace=0; nFace<6; nFace++) {
						// Set the new render target
						if (cRenderer.SetRenderTarget(m_pSurfaceTextureBuffer, nFace)) {
							// Draw a nice and colorful picture :)
							pPainter->OnPaint(*m_pSurfaceTextureBuffer);
						}
					}
				}

			} else { // Render to 2D/rectangle texture
				// Set the new render target
				if (cRenderer.SetRenderTarget(m_pSurfaceTextureBuffer)) {
					// Draw a nice and colorful picture :)
					pPainter->OnPaint(*m_pSurfaceTextureBuffer);
				}
			}

			// Reset renderer target
			cRenderer.SetRenderTarget(pSurfaceBackup);
			if (pCameraBackup) {
				pPainter->SetCamera(pCameraBackup);
				pCameraBackup->SetCamera(cRenderer);
			}

			// Reactivate THIS node to avoid recursion
			SetActive(true);
		}
	}
}

void SCRenderToTexture::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneContainer::DrawDebug(cRenderer, pVisNode);

	// Draw rendered texture?
	if (!(GetDebugFlags() & DebugNoTexturePreview) && m_pSurfaceTextureBuffer->GetTextureBuffer()) {
		// [TODO] Add support for cube textures: We need to be able to use cube faces as normal 2D textures...
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

		// Begin 2D mode
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.Begin2DMode();

			// Draw image
			SamplerStates cSamplerStates;
			cDrawHelpers.DrawImage(*m_pSurfaceTextureBuffer->GetTextureBuffer(), cSamplerStates, Vector2::Zero, Vector2(0.5f, 0.5f));

		// End 2D mode
		cDrawHelpers.End2DMode();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SCRenderToTexture::InitFunction()
{
	// Call base implementation
	SceneContainer::InitFunction();

	// Create the texture surface
	CreateSurfaceTexture();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
