/*********************************************************\
 *  File: SRPDeferredGBuffer.cpp                         *
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
#include <PLMath/Matrix3x3.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/Mesh.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDeferredGBuffer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredGBuffer::SRPDeferredGBuffer() :
	ShaderLanguage(this),
	TextureFiltering(this),
	Flags(this),
	EventHandlerMaterialRemoved(&SRPDeferredGBuffer::OnMaterialRemoved, this),
	m_pRenderTarget(nullptr),
	m_pColorTarget1(nullptr),
	m_pColorTarget2(nullptr),
	m_pColorTarget3(nullptr),
	m_bColorTarget3Used(false),
	m_bColorTarget3AlphaUsed(false),
	m_pSurfaceBackup(nullptr),
	m_pFullscreenQuad(nullptr),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredGBuffer::~SRPDeferredGBuffer()
{
	{ // SRPDeferredGBuffer-material cache cleanup
		Iterator<SRPDeferredGBufferMaterial*> lstIterator = m_lstMaterialCache.GetIterator();
		while (lstIterator.HasNext())
			delete lstIterator.Next();
		m_lstMaterialCache.Clear();
	}

	// Destroy the render target of the GBuffer
	if (m_pRenderTarget)
		delete m_pRenderTarget;
	if (m_pColorTarget1)
		delete m_pColorTarget1;
	if (m_pColorTarget2)
		delete m_pColorTarget2;
	if (m_pColorTarget3)
		delete m_pColorTarget3;

	// Destroy the fullscreen quad
	if (m_pFullscreenQuad)
		delete m_pFullscreenQuad;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;

	// Destroy all mesh batches
	{ // Currently free ones
		Iterator<MeshBatch*> cIterator = m_lstFreeMeshBatches.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
		m_lstFreeMeshBatches.Clear();
	}
	{ // Currently used ones
		Iterator<MeshBatch*> cIterator = m_lstMeshBatches.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
		m_lstMeshBatches.Clear();
	}
}

/**
*  @brief
*    Returns a render target texture buffer of the GBuffer
*/
TextureBufferRectangle *SRPDeferredGBuffer::GetRenderTargetTextureBuffer(uint32 nIndex) const
{
	switch (nIndex)  {
		case 0:
			return m_pRenderTarget ? static_cast<TextureBufferRectangle*>(m_pRenderTarget->GetTextureBuffer()) : nullptr;

		case 1:
			return m_pColorTarget1;

		case 2:
			return m_pColorTarget2;

		case 3:
			return m_pColorTarget3;

		default:
			return nullptr;
	}
}

// [TODO] Remove this!
SurfaceTextureBuffer *SRPDeferredGBuffer::GetRenderTarget() const
{
	return m_pRenderTarget;
}

/**
*  @brief
*    Returns whether or not color target 3 has real information
*/
bool SRPDeferredGBuffer::IsColorTarget3Used() const
{
	return m_bColorTarget3Used;
}

/**
*  @brief
*    Returns whether or not the alpha channel target 3 has real information
*/
bool SRPDeferredGBuffer::IsColorTarget3AlphaUsed() const
{
	return m_bColorTarget3AlphaUsed;
}

/**
*  @brief
*    Returns a shared fullscreen quad instance
*/
FullscreenQuad *SRPDeferredGBuffer::GetFullscreenQuad() const
{
	return m_pFullscreenQuad;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Collect mesh batches recursive
*/
void SRPDeferredGBuffer::CollectMeshBatchesRec(const SQCull &cCullQuery)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Loop through all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery())
					CollectMeshBatchesRec(*pVisCell->GetCullQuery());

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Collect this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					CollectMeshBatchesRec(*static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

			// This must just be a quite boring scene node :)
			} else {
				// Here we draw the stuff by hand in order to minimize state changes and other overhead
				const MeshHandler *pMeshHandler = pSceneNode->GetMeshHandler();
				if (pMeshHandler && pMeshHandler->GetVertexBuffer() && pMeshHandler->GetNumOfMaterials()) {
					// Get the used mesh
					const Mesh *pMesh = pMeshHandler->GetResource();
					if (pMesh) {
						// Get the mesh LOD level to use
						const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
						if (pLODLevel && pLODLevel->GetIndexBuffer() && pMeshHandler->GetVertexBuffer()) {
							// Loop through all mesh geometries
							const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
								// Is this geometry active and is it using the current used mesh material?
								const Geometry &cGeometry = lstGeometries[nGeo];
								if (cGeometry.IsActive()) {
									// Get the material the mesh geometry is using
									Material *pMaterial = pMeshHandler->GetMaterial(cGeometry.GetMaterial());
									if (pMaterial) {
										// Transparent material?
										static const String sOpacity = "Opacity";
										const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
										if (!pParameter || pParameter->GetValue1f() >= 1.0f) {
											// Get a free mesh batch
											MeshBatch &cMeshBatch = GetFreeMeshBatch();
											m_lstMeshBatches.Add(&cMeshBatch);

											// Fill the mesh batch
											cMeshBatch.pMaterial			= pMaterial;
											cMeshBatch.pVertexBuffer		= pMeshHandler->GetVertexBuffer();
											cMeshBatch.pIndexBuffer			= pLODLevel->GetIndexBuffer();
											cMeshBatch.pCullQuery			= &cCullQuery;
											cMeshBatch.pGeometry			= &cGeometry;
											cMeshBatch.pVisNode				= pVisNode;
											cMeshBatch.sScissorRectangle	= cVisContainer.GetProjection().cRectangle;

											// Add material
											if (!m_lstMaterials.IsElement(pMaterial))
												m_lstMaterials.Add(pMaterial);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Draws a mesh batch
*/
void SRPDeferredGBuffer::DrawMeshBatch(Renderer &cRenderer, SRPDeferredGBufferMaterial::GeneratedProgramUserData &cGeneratedProgramUserData, MeshBatch &cMeshBatch) const
{
	// Get the mesh batch vertex buffer
	VertexBuffer *pVertexBuffer = cMeshBatch.pVertexBuffer;

	{ // Set per mesh batch program parameters
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (cGeneratedProgramUserData.pVertexPosition)
			cGeneratedProgramUserData.pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);
		if (cGeneratedProgramUserData.pVertexTexCoord0)
			cGeneratedProgramUserData.pVertexTexCoord0->Set(pVertexBuffer, PLRenderer::VertexBuffer::TexCoord);
		if (cGeneratedProgramUserData.pVertexTexCoord1)
			cGeneratedProgramUserData.pVertexTexCoord1->Set(pVertexBuffer, PLRenderer::VertexBuffer::TexCoord, 1);
		if (cGeneratedProgramUserData.pVertexNormal)
			cGeneratedProgramUserData.pVertexNormal->Set(pVertexBuffer, PLRenderer::VertexBuffer::Normal);
		if (cGeneratedProgramUserData.pVertexTangent)
			cGeneratedProgramUserData.pVertexTangent->Set(pVertexBuffer, PLRenderer::VertexBuffer::Tangent);
		if (cGeneratedProgramUserData.pVertexBinormal)
			cGeneratedProgramUserData.pVertexBinormal->Set(pVertexBuffer, PLRenderer::VertexBuffer::Binormal);

		// Get the mesh batch visibility node
		const VisNode *pVisNode = cMeshBatch.pVisNode;

		// Two sided lighting?
		if (cGeneratedProgramUserData.pNormalScale)
			cGeneratedProgramUserData.pNormalScale->Set(1.0f);

		// Set world view projection matrix
		if (cGeneratedProgramUserData.pWorldVP)
			cGeneratedProgramUserData.pWorldVP->Set(pVisNode->GetWorldViewProjectionMatrix());

		// Set world view matrix
		if (cGeneratedProgramUserData.pWorldV)
			cGeneratedProgramUserData.pWorldV->Set(pVisNode->GetWorldViewMatrix());

		// Set object space eye position
		if (cGeneratedProgramUserData.pEyePos)
			cGeneratedProgramUserData.pEyePos->Set(pVisNode->GetInverseWorldMatrix()*(cMeshBatch.pCullQuery->GetVisContainer().GetWorldMatrix()*cMeshBatch.pCullQuery->GetCameraPosition()));

		// Set the "ViewSpaceToWorldSpace" fragment shader parameter
		if (cGeneratedProgramUserData.pViewSpaceToWorldSpace) {
			// [TODO] Add *SNCamera::GetInvViewMatrix()?
			Matrix3x3 mRot;
			if (SNCamera::GetCamera())
				mRot = SNCamera::GetCamera()->GetViewMatrix().GetInverted();
			else
				mRot = Matrix3x3::Identity;
			cGeneratedProgramUserData.pViewSpaceToWorldSpace->Set(mRot);
		}
	}

	// Bind index buffer
	cRenderer.SetIndexBuffer(cMeshBatch.pIndexBuffer);

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cMeshBatch.sScissorRectangle);

	// Get the mesh patch geometry
	const Geometry *pGeometry = cMeshBatch.pGeometry;

	// Draw the geometry
	cRenderer.DrawIndexedPrimitives(
		pGeometry->GetPrimitiveType(),
		0,
		pVertexBuffer->GetNumOfElements()-1,
		pGeometry->GetStartIndex(),
		pGeometry->GetIndexSize()
	);

	// If this is a two sided material, draw the primitives again - but with
	// flipped culling mode and vertex normals
	if (cGeneratedProgramUserData.pNormalScale) {
		// Flip normals
		cGeneratedProgramUserData.pNormalScale->Set(-1.0f);

		// Flip the backface culling
		const uint32 nCullModeBackup = cRenderer.GetRenderState(RenderState::CullMode);
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);

		// Draw geometry - again
		cRenderer.DrawIndexedPrimitives(
			pGeometry->GetPrimitiveType(),
			0,
			pVertexBuffer->GetNumOfElements()-1,
			pGeometry->GetStartIndex(),
			pGeometry->GetIndexSize()
		);

		// Restore the previous cull mode
		cRenderer.SetRenderState(RenderState::CullMode, nCullModeBackup);
	}
}

/**
*  @brief
*    Returns a free mesh batch
*/
SRPDeferredGBuffer::MeshBatch &SRPDeferredGBuffer::GetFreeMeshBatch()
{
	if (m_lstFreeMeshBatches.IsEmpty())
		return *(new MeshBatch);
	else {
		MeshBatch *pMeshBatch = m_lstFreeMeshBatches.Get(0);
		m_lstFreeMeshBatches.RemoveAtIndex(0);
		return *pMeshBatch;
	}
}

/**
*  @brief
*    Called when a material is removed
*/
void SRPDeferredGBuffer::OnMaterialRemoved(Material &cMaterial)
{
	// Is this material cached?
	const uint64 nMaterialID = reinterpret_cast<uint64>(&cMaterial);
	SRPDeferredGBufferMaterial *pSRPDeferredGBufferMaterial = m_lstMaterialCache.Get(nMaterialID);
	if (pSRPDeferredGBufferMaterial) {
		// Remove the material from the cache
		m_lstMaterialCache.Remove(nMaterialID);
		delete pSRPDeferredGBufferMaterial;
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredGBuffer::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the shader language to use
	String sShaderLanguage = ShaderLanguage;
	if (!sShaderLanguage.GetLength())
		sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

	// Create the program generator if there's currently no instance of it
	if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
		// If not already done: Connect event handler - if it's already connected to this event, nothing happens
		cRenderer.GetRendererContext().GetMaterialManager().EventResourceRemoved.Connect(EventHandlerMaterialRemoved);

		// If there's an previous instance of the program generator, destroy it first
		if (m_pProgramGenerator) {
			delete m_pProgramGenerator;
			m_pProgramGenerator = nullptr;
		}

		// Choose the shader source codes depending on the requested shader language
		if (sShaderLanguage == "GLSL") {
			#include "SRPDeferredGBuffer_GLSL.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredGBuffer_GLSL_VS, "120", sDeferredGBuffer_GLSL_FS, "120");	// OpenGL 2.1 ("#version 120")
		} else if (sShaderLanguage == "Cg") {
			#include "SRPDeferredGBuffer_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredGBuffer_Cg_VS, "arbvp1", sDeferredGBuffer_Cg_FS, "arbfp1");
		}
	}

	// If there's no program generator, we don't need to continue
	if (m_pProgramGenerator) {
		// Create the fullscreen quad instance if required
		if (!m_pFullscreenQuad)
			m_pFullscreenQuad = new FullscreenQuad(cRenderer);

		// Get the size of the current render target
		const Vector2i vRTSize = cRenderer.GetRenderTarget()->GetSize();
		if (vRTSize.x != 0 && vRTSize.y != 0) {
			// Get the internal texture format to use
			const TextureBuffer::EPixelFormat nInternalFormat = (GetFlags() & Float32) ? TextureBuffer::R32G32B32A32F : TextureBuffer::R16G16B16A16F;

			// Render target size change?
			if (m_pRenderTarget && (m_pRenderTarget->GetSize() != vRTSize || m_pRenderTarget->GetFormat() != nInternalFormat)) {
				// Destroy the render target of the GBuffer
				if (m_pRenderTarget) {
					delete m_pRenderTarget;
					m_pRenderTarget = nullptr;
				}
				if (m_pColorTarget1) {
					delete m_pColorTarget1;
					m_pColorTarget1 = nullptr;
				}
				if (m_pColorTarget2) {
					delete m_pColorTarget2;
					m_pColorTarget2 = nullptr;
				}
				if (m_pColorTarget3) {
					delete m_pColorTarget3;
					m_pColorTarget3 = nullptr;
				}
			}

			// Create the render target of the GBuffer right now?
			if (!m_pRenderTarget) {
				m_pRenderTarget = cRenderer.CreateSurfaceTextureBufferRectangle(vRTSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing, 4);
				{ // Create color target 1, 2 and 3
					Image cImage = Image::CreateImage(DataFloat, ColorRGBA, Vector3i(vRTSize.x, vRTSize.y, 1));
					// [TODO] Currently, I just assume a real rectangle instance... find a better way!
					m_pColorTarget1 = static_cast<TextureBufferRectangle*>(cRenderer.CreateTextureBufferRectangle(cImage, nInternalFormat, TextureBuffer::RenderTarget));
					m_pColorTarget2 = static_cast<TextureBufferRectangle*>(cRenderer.CreateTextureBufferRectangle(cImage, nInternalFormat, TextureBuffer::RenderTarget));
					m_pColorTarget3 = static_cast<TextureBufferRectangle*>(cRenderer.CreateTextureBufferRectangle(cImage, nInternalFormat, TextureBuffer::RenderTarget));
				}
			}
		}

		// Is there a render target?
		if (m_pRenderTarget) {
			// Backup the current render target, pass on the depth buffer of the surface texture buffer and set the new render target
			m_pSurfaceBackup = cRenderer.GetRenderTarget();
			cRenderer.SetRenderTarget(m_pRenderTarget);
			SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
			if (pSRPBegin)
				cRenderer.SetDepthRenderTarget(pSRPBegin->GetTextureBufferDepth());
			cRenderer.SetColorRenderTarget(static_cast<TextureBuffer*>(m_pColorTarget1), 1);
			cRenderer.SetColorRenderTarget(static_cast<TextureBuffer*>(m_pColorTarget2), 2);
			cRenderer.SetColorRenderTarget(static_cast<TextureBuffer*>(m_pColorTarget3), 3);

			// Reset all render states to default
			cRenderer.GetRendererContext().GetEffectManager().Use();

			// Clear the GBuffer content
			if (GetFlags() & NoStencil) {
				// Do only clear the color buffer
				cRenderer.Clear(Clear::Color, Color4(0.0f, 0.0f, 0.0f, 0.0f));
			} else {
				// Clear color buffer and stencil buffer
				cRenderer.Clear(Clear::Color | Clear::Stencil, Color4(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 1);

				// Enable stencil test - if something is written, the pixel is tagged with 0
				cRenderer.SetRenderState(RenderState::StencilEnable, true);
				cRenderer.SetRenderState(RenderState::StencilRef,    0);
				cRenderer.SetRenderState(RenderState::StencilPass,   StencilOp::Replace);
			}

			// Backup the color mask
			bool bRed, bGreen, bBlue, bAlpha;
			cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

			// Setup the color mask and enable scissor test
			cRenderer.SetColorMask(true, true, true, true);
			cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

			// Reset current states
			m_bColorTarget3Used			= false;
			m_bColorTarget3AlphaUsed	= false;

			// Collect recursive
			CollectMeshBatchesRec(cCullQuery);

			{ // Loop through all currently used materials
				Iterator<Material*> cMaterialIterator = m_lstMaterials.GetIterator();
				while (cMaterialIterator.HasNext()) {
					// Get the current material
					Material *pMaterial = cMaterialIterator.Next();

					// SRPDeferredGBuffer-material caching
					SRPDeferredGBufferMaterial *pSRPDeferredGBufferMaterial = m_lstMaterialCache.Get(reinterpret_cast<uint64>(pMaterial));
					if (!pSRPDeferredGBufferMaterial) {
						// The material is not yet cached
						pSRPDeferredGBufferMaterial = new SRPDeferredGBufferMaterial(*pMaterial, *m_pProgramGenerator);
						m_lstMaterialCache.Add(reinterpret_cast<uint64>(pMaterial), pSRPDeferredGBufferMaterial);
					}

					// Make the material to the currently used one
					SRPDeferredGBufferMaterial::GeneratedProgramUserData *pGeneratedProgramUserData = pSRPDeferredGBufferMaterial->MakeMaterialCurrent(GetFlags(), TextureFiltering, m_bColorTarget3Used, m_bColorTarget3AlphaUsed);
					if (pGeneratedProgramUserData) {
						// Draw all mesh batches using this material
						Iterator<MeshBatch*> cMeshBatchIterator = m_lstMeshBatches.GetIterator();
						while (cMeshBatchIterator.HasNext()) {
							// Get the current mesh batch
							MeshBatch *pMeshBatch = cMeshBatchIterator.Next();

							// Same material?
							if (pMeshBatch->pMaterial == pMaterial) {
								// Draw the mesh batch
								DrawMeshBatch(cRenderer, *pGeneratedProgramUserData, *pMeshBatch);
							}
						}
					}
				}
				{ // Free the mesh batches
					Iterator<MeshBatch*> cMeshBatchIterator = m_lstMeshBatches.GetIterator();
					while (cMeshBatchIterator.HasNext())
						m_lstFreeMeshBatches.Add(cMeshBatchIterator.Next());
					m_lstMeshBatches.Clear();
				}
				m_lstMaterials.Clear();
			}

			// Restore the color mask
			cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);

			// Restore previous render target
			cRenderer.SetRenderTarget(m_pSurfaceBackup);
			if (pSRPBegin)
				cRenderer.SetDepthRenderTarget(pSRPBegin->GetTextureBufferDepth());
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
