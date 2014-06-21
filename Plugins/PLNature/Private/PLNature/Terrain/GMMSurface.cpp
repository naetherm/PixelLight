/*********************************************************\
 *  File: GMMSurface.cpp                                 *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLNature/Terrain/GMMSurfacePatch.h"
#include "PLNature/Terrain/GMMSurfaceLevel.h"
#include "PLNature/Terrain/GMMSurface.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLNature {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GMMSurface::GMMSurface(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pMaterialHandler(new MaterialHandler())
{
	Init();
}

/**
*  @brief
*    Destructor
*/
GMMSurface::~GMMSurface()
{
	Destroy();
	delete m_pMaterialHandler;
}

/**
*  @brief
*    Creates the GMM surface
*/
bool GMMSurface::Create(uint32 nHeightMapSize, float fHeightMap[], uint32 nPatchSize, int nGeoMipMaps)
{
	// Check given data
	if (nHeightMapSize < 1 || nPatchSize < 1)
		return false; // Error!

	// Destroy the old GMM surface
	Destroy();

	// Setup height map
	m_nHeightMapSize = nHeightMapSize;
	m_pfHeightMap    = fHeightMap;

	// Setup patches
	m_nPatchSize = nPatchSize;
	m_nXYPatches = m_nHeightMapSize/nPatchSize;
	m_nPatches   = m_nXYPatches*m_nXYPatches;

	// Levels
	m_nGeoMipMaps = (nGeoMipMaps == -1) ? static_cast<uint32>(Math::Log(static_cast<double>(m_nPatchSize))*1.4427)+1 : nGeoMipMaps;
	m_ppLevels = new GMMSurfaceLevel*[m_nGeoMipMaps];
	for (uint32 i=0; i<m_nGeoMipMaps; i++) {
		m_ppLevels[i] = new GMMSurfaceLevel(*m_pRenderer);
		m_ppLevels[i]->BuildBodies(i, m_nPatchSize);
		m_ppLevels[i]->BuildLinkers(i, m_nPatchSize);
	}

	// Create vertices
	m_nVertices = (((m_nHeightMapSize-1)/m_nPatchSize)*((m_nHeightMapSize-1)/m_nPatchSize))*((m_nPatchSize+1)*(m_nPatchSize+1));
	m_pVertex = new Vertex[m_nVertices];

	// Create patches
	uint32 nOffset = 0;
	m_ppPatches = new GMMSurfacePatch*[m_nPatches];
	for (uint32 nY=0, nPatch=0; nY<m_nXYPatches; nY++)
		for (uint32 nX=0; nX<m_nXYPatches; nX++, nPatch++) {
			m_ppPatches[nPatch] = new GMMSurfacePatch(*this);
			m_ppPatches[nPatch]->Create(nX*m_nPatchSize, nY*m_nPatchSize, nOffset);
		}

	// Setup neighbours
	SetNeighbours();

	// Build quadtree
	m_cQuadtree.Init(0, 0, m_nXYPatches, m_nXYPatches);
	m_cQuadtree.Build();
	m_cQuadtree.UpdateBoundingBoxes(reinterpret_cast<QuadtreePatch**>(m_ppPatches));

	// Create vertex buffer
	m_pVertexBuffer = m_pRenderer->CreateVertexBuffer();
	m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
	m_pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
	m_pVertexBuffer->Allocate(m_nVertices, Usage::Static, false);
	if (m_pVertexBuffer->Lock(Lock::WriteOnly)) {
		Vertex *pVertex = m_pVertex;
		float *pfVertex;

		for (uint32 i=0; i<m_nVertices; i++) {
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(i, VertexBuffer::Position));
			pfVertex[Vector3::X] = pVertex->x;
			pfVertex[Vector3::Y] = pVertex->y;
			pfVertex[Vector3::Z] = pVertex->z;

			// Texture coordinate 0
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(i, VertexBuffer::TexCoord, 0));
			pfVertex[Vector2::X] = pVertex->u;
			pfVertex[Vector2::Y] = pVertex->v;

			// Increase vertex pointer
			pVertex++;
		}

		// Unlock the vertex buffer
		m_pVertexBuffer->Unlock();
	}

	// Done
	return true;
}

/**
*  @brief
*    Kills the GMM surface
*/
void GMMSurface::Destroy()
{
	if (m_ppPatches) {
		for (uint32 i=0; i<m_nPatches; i++)
			delete m_ppPatches[i];
		delete [] m_ppPatches;
	}
	if (m_pVertex) {
		if (m_pVertexBuffer) {
			delete m_pVertexBuffer;
			m_pVertexBuffer = nullptr;
		}
		delete [] m_pVertex;
	}
	if (m_ppLevels) {
		for (uint32 i=0; i<m_nGeoMipMaps; i++)
			delete m_ppLevels[i];
		delete [] m_ppLevels;
	}
	Init();
}

/**
*  @brief
*    Returns the viewer position
*/
const Vector3 &GMMSurface::GetViewerPos() const
{
	return m_vViewerPos;
}

/**
*  @brief
*    Sets the viewer position
*/
void GMMSurface::SetViewerPos(const Vector3 &vPos)
{
	m_vViewerPos = vPos;
}

/**
*  @brief
*    Returns the size of the height map
*/
uint32 GMMSurface::GetHeightMapSize() const
{
	return m_nHeightMapSize;
}

/**
*  @brief
*    Returns a pointer to the height map
*/
const float *GMMSurface::GetHeightMap() const
{
	return m_pfHeightMap;
}

/**
*  @brief
*    Returns the number of surface vertices
*/
uint32 GMMSurface::GetVertices() const
{
	return m_nVertices;
}

/**
*  @brief
*    Returns a pointer to a surface vertex
*/
GMMSurface::Vertex *GMMSurface::GetVertex(uint32 nID) const
{
	// Check ID
	return (nID < m_nVertices) ? &m_pVertex[nID] : nullptr;
}

/**
*  @brief
*    Returns the height of a height map coordinate
*/
float GMMSurface::GetHeight(uint32 nX, uint32 nY) const
{
	// Check if there's a height map, if so, check the given coordinate
	return (m_pfHeightMap && nX < m_nHeightMapSize && nY < m_nHeightMapSize) ? m_pfHeightMap[nY*m_nHeightMapSize + nX] : 0.0f;
}

/**
*  @brief
*    Sets the height of a height map coordinate
*/
bool GMMSurface::SetHeight(uint32 nX, uint32 nY, float fHeight)
{
	// Check if there's a height map, if so, check the given coordinate
	if (m_pfHeightMap && nX < m_nHeightMapSize && nY < m_nHeightMapSize) {
		// Set new height at the given coordinate
		m_pfHeightMap[nY*m_nHeightMapSize+nX] = fHeight;

		// [TODO] Force a rebuild of the patch surrounding the coordinate

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Returns the size of a patch
*/
uint32 GMMSurface::GetPatchSize() const
{
	return m_nPatchSize;
}

/**
*  @brief
*    Returns the number of patches per column/row
*/
uint32 GMMSurface::GetXYPatches() const
{
	return m_nXYPatches;
}

/**
*  @brief
*    Returns the total number of patches in the GMM surface
*/
uint32 GMMSurface::GetPatches() const
{
	return m_nPatches;
}

/**
*  @brief
*    Returns a pointer to the patch with the given ID
*/
GMMSurfacePatch *GMMSurface::GetPatch(uint32 nID) const
{
	// Check if there are patches if so, check the given ID
	return (m_ppPatches && nID < m_nPatches) ? m_ppPatches[nID] : nullptr;
}

/**
*  @brief
*    Returns the number of geometrically mipmaps maps
*/
uint32 GMMSurface::GetGeoMipMaps() const
{
	return m_nGeoMipMaps;
}

/**
*  @brief
*    Sets the neighbour GMM surfaces
*/
void GMMSurface::SetNeighbours(GMMSurface *pLeft, GMMSurface *pTop, GMMSurface *pRight, GMMSurface *pBottom)
{
	// Set surface neighbours
	m_pNeighbour[Left]   = pLeft;
	m_pNeighbour[Top]    = pTop;
	m_pNeighbour[Right]  = pRight;
	m_pNeighbour[Bottom] = pBottom;

	// Set surface patch neighbours
	for (uint32 nY=0; nY<m_nXYPatches; nY++) {
		for (uint32 nX=0; nX<m_nXYPatches; nX++) {
			// Get patch
			GMMSurfacePatch *pPatch = m_ppPatches[nY*m_nXYPatches+nX];

			// Left
			if (nX)
				pPatch->m_pNeighbour[Left] = m_ppPatches[nY*m_nXYPatches+nX-1];
			else if (m_pNeighbour[Left])
				pPatch->m_pNeighbour[Left] = m_pNeighbour[Left]->m_ppPatches[nY*m_nXYPatches+m_nXYPatches-1];
			else
				pPatch->m_pNeighbour[Left] = nullptr;

			// Top
			if (nY)
				pPatch->m_pNeighbour[Top] = m_ppPatches[(nY-1)*m_nXYPatches+nX];
			else if (m_pNeighbour[Top])
				pPatch->m_pNeighbour[Top] = m_pNeighbour[Top]->m_ppPatches[(m_nXYPatches-1)*m_nXYPatches+nX];
			else
				pPatch->m_pNeighbour[Top] = nullptr;

			// Right
			if (nX < m_nXYPatches-1)
				pPatch->m_pNeighbour[Right] = m_ppPatches[nY*m_nXYPatches+nX+1];
			else if (m_pNeighbour[Right])
				pPatch->m_pNeighbour[Right] = m_pNeighbour[Right]->m_ppPatches[nY*m_nXYPatches];
			else
				pPatch->m_pNeighbour[Right] = nullptr;

			// Bottom
			if (nY < m_nXYPatches-1)
				pPatch->m_pNeighbour[Bottom] = m_ppPatches[(nY+1)*m_nXYPatches+nX];
			else if (m_pNeighbour[Bottom])
				pPatch->m_pNeighbour[Bottom] = m_pNeighbour[Bottom]->m_ppPatches[nX];
			else
				pPatch->m_pNeighbour[Bottom] = nullptr;
		}
	}
}

/**
*  @brief
*    Returns the surface material handler
*/
MaterialHandler &GMMSurface::GetMaterialHandler()
{
	return *m_pMaterialHandler;
}

/**
*  @brief
*    Returns the GMM surface quadtree
*/
Quadtree &GMMSurface::GetQuadtree()
{
	return m_cQuadtree;
}

/**
*  @brief
*    Updates the GMM surface
*/
void GMMSurface::Update()
{
	PlaneSet &cSourcePlaneSet = SNCamera::GetCamera()->GetFrustum(m_pRenderer->GetViewport()), cPlaneSet;

	// Fixed functions support required
	FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
	if (pFixedFunctions) {
		// Transform frustum to object space
		Matrix4x4 mInvWorldMatrix = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);
		mInvWorldMatrix.Invert();
		for (uint32 i=0; i<cSourcePlaneSet.GetNumOfPlanes(); i++)
			*cPlaneSet.Create() = (*cSourcePlaneSet[i])*mInvWorldMatrix;
	}

	// Update visibility
	m_cQuadtree.UpdateVisibility(cPlaneSet, reinterpret_cast<QuadtreePatch**>(m_ppPatches));

	// Update patches
	for (uint32 i=0; i<m_nPatches; i++)
		m_ppPatches[i]->Update();
}

/**
*  @brief
*    Draws the GMM surface
*/
void GMMSurface::Draw() const
{
	// Render surface
	Material *pMaterial = m_pMaterialHandler->GetResource();
	if (pMaterial) {
		for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
			pMaterial->SetupPass(nPass);
			for (uint32 i=0; i<m_nPatches; i++)
				DrawPatch(*m_ppPatches[i]);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws a patch
*/
void GMMSurface::DrawPatch(GMMSurfacePatch &cPatch) const
{
	// Draw this patch?
	if (cPatch.IsActive() && cPatch.IsVisible()) {
		// Bind vertex buffer
		// [TODO] Remove FixedFunctions usage by using the new shader interface
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions)
			pFixedFunctions->SetVertexBuffer(m_pVertexBuffer, cPatch.m_nOffset);

		uint32 nVar   = 0;
		uint32 nLevel = cPatch.m_nLevel;
		for (int i=0; i<4; i++) {
			if (cPatch.m_pNeighbour[i] && cPatch.m_pNeighbour[i]->m_nLevel < nLevel)
				nVar += 1<<i;
		}

		// Draw body
		if (m_ppLevels[nLevel]->m_pBodies[nVar].pIndexBuffer) {
			uint32 nIndices = m_ppLevels[nLevel]->m_pBodies[nVar].nIndexBufferSize;
			m_pRenderer->SetIndexBuffer(m_ppLevels[nLevel]->m_pBodies[nVar].pIndexBuffer);
			m_pRenderer->DrawIndexedPrimitives(Primitive::TriangleStrip, 0, m_nVertices, 0, nIndices);
		}

		if (nVar) {
			if (nVar != 15) {
				if (m_ppLevels[nLevel]->m_pBodies[nVar].pCornerIndexBuffer) {
					uint32 nIndices = m_ppLevels[nLevel]->m_pBodies[nVar].nCornerIndexSize;
					m_pRenderer->SetIndexBuffer(m_ppLevels[nLevel]->m_pBodies[nVar].pCornerIndexBuffer);
					m_pRenderer->DrawIndexedPrimitives(Primitive::TriangleList, 0, m_nVertices, 0, nIndices);
				}
			}

			for (int i=0; i<4; i++) {
				if (nVar&(1<<i)) {
					uint32 nLowLevel = cPatch.m_pNeighbour[i]->m_nLevel;
					if (m_ppLevels[nLevel]->m_pLinkers[nLowLevel].pIndexBuffer[i]) {
						uint32 nIndices = m_ppLevels[nLevel]->m_pLinkers[nLowLevel].nIndexBufferSize[i];
						m_pRenderer->SetIndexBuffer(m_ppLevels[nLevel]->m_pLinkers[nLowLevel].pIndexBuffer[i]);
						m_pRenderer->DrawIndexedPrimitives(Primitive::TriangleList, 0, m_nVertices, 0, nIndices);
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Initializes the GMM surface
*/
void GMMSurface::Init()
{
	// General
	m_pNeighbour[0] = nullptr;
	m_pNeighbour[1] = nullptr;
	m_pNeighbour[2] = nullptr;
	m_pNeighbour[3] = nullptr;
	m_nVertices     = 0;
	m_pVertexBuffer = nullptr;
	m_pVertex       = nullptr;

	// Height map
	m_nHeightMapSize = 0;
	m_pfHeightMap    = nullptr;

	// Patches
	m_nPatchSize = 0;
	m_nXYPatches = 0;
	m_nPatches   = 0;
	m_ppPatches  = nullptr;

	// Levels
	m_nGeoMipMaps = 0;
	m_ppLevels    = nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature
