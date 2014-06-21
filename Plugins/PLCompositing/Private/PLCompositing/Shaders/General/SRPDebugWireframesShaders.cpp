/*********************************************************\
 *  File: SRPDebugWireframesShaders.cpp                  *
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
#include <PLCore/Tools/Tools.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/Shaders/General/SRPDebugWireframesShaders.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDebugWireframesShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugWireframesShaders::SRPDebugWireframesShaders() :
	ShaderLanguage(this),
	EventHandlerDirty(&SRPDebugWireframesShaders::OnDirty, this),
	m_pVertexShader(nullptr),
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr),
	m_pPositionProgramAttribute(nullptr),
	m_pObjectSpaceToClipSpaceMatrixProgramUniform(nullptr),
	m_pColorProgramUniform(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugWireframesShaders::~SRPDebugWireframesShaders()
{
	// Destroy shaders
	if (m_pProgram)
		delete m_pProgram;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
	if (m_pVertexShader)
		delete m_pVertexShader;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPDebugWireframesShaders::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery) const
{
	// Get scene container
	SceneContainer &cSceneContainer = cCullQuery.GetSceneContainer();
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	// Draw the container scene node
	if (cSceneContainer.GetMeshHandler())
		DrawMesh(cRenderer, cVisContainer, *cSceneContainer.GetMeshHandler());

	// Render all visible scene nodes of this scene container
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
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Draw the target cell
					DrawRec(cRenderer, *pVisCell->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

				// Draw the portal itself
				if (pSceneNode->GetMeshHandler())
					DrawMesh(cRenderer, *pVisNode, *pSceneNode->GetMeshHandler());

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					DrawRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

				// Set the previous scissor rectangle
				cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

			// This must just be a quite boring scene node :)
			} else {
				if (pSceneNode->GetMeshHandler())
					DrawMesh(cRenderer, *pVisNode, *pSceneNode->GetMeshHandler());
			}
		}
	}
}

/**
*  @brief
*    Draws a mesh
*/
void SRPDebugWireframesShaders::DrawMesh(Renderer &cRenderer, const VisNode &cVisNode, const MeshHandler &cMeshHandler) const
{
	// Check for draw distance limitation
	if ((MaxDrawDistance <= 0.0f || cVisNode.GetSquaredDistanceToCamera() <= MaxDrawDistance*MaxDrawDistance) && cMeshHandler.GetVertexBuffer()) {
		// Get the mesh
		const Mesh *pMesh = cMeshHandler.GetResource();
		if (pMesh) {
			// Get buffers
			const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
			if (pLODLevel && pLODLevel->GetIndexBuffer()) {
					  IndexBuffer	  *pIndexBuffer  = pLODLevel->GetIndexBuffer();
				const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();

				// Bind index buffer
				cRenderer.SetIndexBuffer(pIndexBuffer);

				// Set the "ObjectSpaceToClipSpaceMatrixProgramUniform" fragment shader parameter
				if (m_pObjectSpaceToClipSpaceMatrixProgramUniform)
					m_pObjectSpaceToClipSpaceMatrixProgramUniform->Set(cVisNode.GetWorldViewProjectionMatrix());

				// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
				if (m_pPositionProgramAttribute)
					m_pPositionProgramAttribute->Set(cMeshHandler.GetVertexBuffer(), PLRenderer::VertexBuffer::Position);

				// Draw geometries
				for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
					// Is this geometry active?
					const Geometry &cGeometry = lstGeometries[nGeo];
					if (cGeometry.IsActive()) {
						// Draw geometry
						cRenderer.DrawIndexedPrimitives(
							cGeometry.GetPrimitiveType(),
							0,
							cMeshHandler.GetVertexBuffer()->GetNumOfElements()-1,
							cGeometry.GetStartIndex(),
							cGeometry.GetIndexSize()
						);
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Called when a program became dirty
*/
void SRPDebugWireframesShaders::OnDirty(Program *pProgram)
{
	// Get attributes and uniforms
	if (pProgram == m_pProgram) {
		m_pPositionProgramAttribute						= m_pProgram->GetAttribute("VertexPosition");
		m_pObjectSpaceToClipSpaceMatrixProgramUniform	= m_pProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
		m_pColorProgramUniform							= m_pProgram->GetUniform("Color");
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDebugWireframesShaders::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Check current fill mode
	if (cRenderer.GetRenderState(RenderState::FixedFillMode) == Fill::Solid) {
		// Get the shader language to use
		String sUsedShaderLanguage = ShaderLanguage;
		if (!sUsedShaderLanguage.GetLength())
			sUsedShaderLanguage = cRenderer.GetDefaultShaderLanguage();

		// Create the GPU program right now?
		if (!m_pProgram || m_pProgram->GetShaderLanguage() != sUsedShaderLanguage) {
			// If there's an previous instance of the program, destroy it first
			if (m_pProgram) {
				delete m_pProgram;
				m_pProgram = nullptr;
			}
			if (m_pFragmentShader) {
				delete m_pFragmentShader;
				m_pFragmentShader = nullptr;
			}
			if (m_pVertexShader) {
				delete m_pVertexShader;
				m_pVertexShader = nullptr;
			}
			m_pPositionProgramAttribute						= nullptr;
			m_pObjectSpaceToClipSpaceMatrixProgramUniform	= nullptr;
			m_pColorProgramUniform							= nullptr;

			// Get the shader language instance
			PLRenderer::ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sUsedShaderLanguage);
			if (pShaderLanguage) {
				// Shader source code
				String sVertexShaderSourceCode;
				String sFragmentShaderSourceCode;
				if (sUsedShaderLanguage == "GLSL") {
					#include "SRPDebugWireframesShaders_GLSL.h"
					if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
						// Get shader source codes
						sVertexShaderSourceCode   = "#version 100\n" + sDebugWireframes_GLSL_VS;
						sFragmentShaderSourceCode = "#version 100\n" + sDebugWireframes_GLSL_FS;
					} else {
						// Remove precision qualifiers so that we're able to use 110 (OpenGL 2.0 shaders) instead of 130 (OpenGL 3.0 shaders,
						// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible
						sVertexShaderSourceCode   = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sDebugWireframes_GLSL_VS);
						sFragmentShaderSourceCode = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sDebugWireframes_GLSL_FS);
					}
				} else if (sUsedShaderLanguage == "Cg") {
					#include "SRPDebugWireframesShaders_Cg.h"
					sVertexShaderSourceCode   = sDebugWireframes_Cg_VS;
					sFragmentShaderSourceCode = sDebugWireframes_Cg_FS;
				}

				// Create a vertex shader instance
				m_pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);

				// Create a fragment shader instance
				m_pFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

				// Create a program instance and assign the created vertex and fragment shaders to it
				m_pProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pFragmentShader);
				if (m_pProgram) {
					// Add our nark which will inform us as soon as the program gets dirty
					m_pProgram->EventDirty.Connect(EventHandlerDirty);

					// Get attributes and uniforms
					OnDirty(m_pProgram);
				}
			}
		}

		// Make our program to the current one
		cRenderer.GetRendererContext().GetEffectManager().Use();
		if (cRenderer.SetProgram(m_pProgram)) {
			// Setup renderer states
			const bool bUseDepth = (GetFlags() & UseDepth) != 0;
			cRenderer.SetRenderState(RenderState::CullMode,			 CullMode.Get());
			cRenderer.SetRenderState(RenderState::ZEnable,			 bUseDepth);
			cRenderer.SetRenderState(RenderState::ZWriteEnable,		 bUseDepth);
			cRenderer.SetRenderState(RenderState::BlendEnable,		 (LineColor.Get().a < 1.0f));
			cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);
			cRenderer.SetRenderState(RenderState::LineWidth,		 Tools::FloatToUInt32(LineWidth));

			// Set polygon offset to avoid nasty line artifacts
			cRenderer.SetRenderState(RenderState::SlopeScaleDepthBias,	Tools::FloatToUInt32(SlopeScaleDepthBias.Get()));
			cRenderer.SetRenderState(RenderState::DepthBias,			Tools::FloatToUInt32(DepthBias.Get()));

			// Set the "Color" fragment shader parameter
			if (m_pColorProgramUniform)
				m_pColorProgramUniform->Set(LineColor.Get());

			// Draw recursive from front to back
			const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
			cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Line);
			DrawRec(cRenderer, cCullQuery);
			cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
