/*********************************************************\
 *  File: SRPVolume.cpp                                  *
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
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLVolumeRenderer/ShaderComposition.h"
#include "PLVolumeRenderer/ShaderCompositorPC.h"
#include "PLVolumeRenderer/SRPVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPVolume)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPVolume::SRPVolume() :
	ShaderLanguage(this),
	RaySetup(this),
	GlobalSampleRateFactor(this),
	JitterPosition(this),
	DitherRay(this),
	GlobalVolumeTextureLOD(this),
	AmbientColor(this),
	IlluminationThreshold(this),
	Flags(this),
	m_pShaderCompositor(new ShaderCompositorPC()),	// [TODO] Just a fixed build in test
	m_pMeshHandler(new MeshHandler())
{
}

/**
*  @brief
*    Destructor
*/
SRPVolume::~SRPVolume()
{
	// Destroy our mesh handler
	delete m_pMeshHandler;

	// Destroy the used shader compositor instance
	if (m_pShaderCompositor)
		delete m_pShaderCompositor;
}

/**
*  @brief
*    Returns the global sampling rate factor
*/
float SRPVolume::GetGlobalSampleRateFactor() const
{
	return (GetFlags() & NoGlobalSampleRateFactor) ? 1.0f : GlobalSampleRateFactor.Get();
}

/**
*  @brief
*    Returns the global volume texture level of detail
*/
float SRPVolume::GetGlobalVolumeTextureLOD() const
{
	return (GetFlags() & NoGlobalVolumeTextureLOD) ? 0.0f : GlobalVolumeTextureLOD.Get();
}


//[-------------------------------------------------------]
//[ Protected virtual PLVolume::SRPVolume functions       ]
//[-------------------------------------------------------]
void SRPVolume::DrawVolumeSceneNode(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode)
{
	// Get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (!pSceneContext)
		return;	// Early escape, there's no sense in continuing

	// Get our proxy cube mesh, create it right now if it doesn't exist, yet
	Mesh *pMesh = m_pMeshHandler->GetMesh();
	if (!pMesh) {
		// Create the mesh
		pMesh = pSceneContext->GetMeshManager().CreateMesh("PLVolume::MeshCreatorCube", true, "");
		m_pMeshHandler->SetMesh(pMesh);
	}
	if (!pMesh)
		return;	// Early escape, there's no sense in continuing

	// Get the mesh LOD level to use
	const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
	if (!pLODLevel || !pLODLevel->GetIndexBuffer())
		return;	// Early escape, there's no sense in continuing

	// Get the vertex buffer of the mesh handler
	VertexBuffer *pVertexBuffer = m_pMeshHandler->GetVertexBuffer();
	if (!pVertexBuffer)
		return;	// Early escape, there's no sense in continuing

	// Shader composition to use for rendering
	ShaderComposition *pShaderComposition = m_pShaderCompositor->Generate(cRenderer, cCullQuery, cVisNode, *this);

	// [TODO] Test
	if (!pShaderComposition)
		return;

	// Setup renderer
	cRenderer.SetRenderState(RenderState::ZEnable,      false);	// Disable depth test, else your drawn back faces might be culled away
	cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
	cRenderer.SetRenderState(RenderState::BlendEnable,  !(GetFlags() & NoBlend));

	// Enable depth clamp in order to avoid possible issues with the near and far clipping plane (avoids premature clipping e.g. when leaving the volume)
	cRenderer.SetRenderState(RenderState::DepthClamp, !(GetFlags() & SRPVolume::NoDepthClamp));

	{ // Draw the back side of the color cube
		// Get the GPU program of this shader composition
		Program &cProgram = pShaderComposition->GetProgram();
		ProgramWrapper *pProgramWrapper = static_cast<ProgramWrapper*>(&cProgram);

	  // -> Don't draw the front faces because then we also would have to deal with camera near-plane clipping
		// Get and use the index buffer of the mesh LOD level
		cRenderer.SetIndexBuffer(pLODLevel->GetIndexBuffer());

		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		pProgramWrapper->Set("VertexPosition", pVertexBuffer, VertexBuffer::Position);
		pProgramWrapper->Set("VertexTexCoord", pVertexBuffer, VertexBuffer::TexCoord);

		// Loop through all geometries of the mesh
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);
		const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
		for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
			// Is this geometry active?
			const Geometry &cGeometry = lstGeometries[nGeo];
			if (cGeometry.IsActive()) {
				// Draw the geometry
				cRenderer.DrawIndexedPrimitives(
					cGeometry.GetPrimitiveType(),
					0,
					pVertexBuffer->GetNumOfElements()-1,
					cGeometry.GetStartIndex(),
					cGeometry.GetIndexSize()
				);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
