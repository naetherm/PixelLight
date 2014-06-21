/*********************************************************\
 *  File: MeshLODLevel.cpp                               *
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
#include <PLCore/Container/Bitset.h>
#include <PLCore/Core/MemoryManager.h>
#include <PLMath/Math.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Mesh.h"
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshOctree.h"
#include "PLMesh/MeshLODLevel.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MeshLODLevel::MeshLODLevel(Mesh *pMesh) :
	m_pMesh(pMesh),
	m_fDistance(0.0f),
	m_pIndexBuffer(nullptr),
	m_plstGeometries(nullptr),
	m_pOctree(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
MeshLODLevel::~MeshLODLevel()
{
	ClearIndexBuffer();
	ClearGeometries();
	DestroyOctree();
}

/**
*  @brief
*    Returns the mesh the LOD level belongs to
*/
Mesh *MeshLODLevel::GetMesh() const
{
	return m_pMesh;
}

/**
*  @brief
*    Sets the mesh the LOD level belongs to
*/
void MeshLODLevel::SetMesh(Mesh *pMesh)
{
	m_pMesh = pMesh;
}

/**
*  @brief
*    Returns the distance for using this LOD level
*/
float MeshLODLevel::GetLODDistance() const
{
	return m_fDistance;
}

/**
*  @brief
*    Sets the distance for using this LOD level
*/
void MeshLODLevel::SetLODDistance(float fDistance)
{
	m_fDistance = fDistance;
}

/**
*  @brief
*    Copy operator
*/
MeshLODLevel &MeshLODLevel::operator =(const MeshLODLevel &cSource)
{
	// Copy data
	m_fDistance = cSource.m_fDistance;

	// Copy index buffer
	if (m_pIndexBuffer) {
		if (cSource.m_pIndexBuffer) {
			// Copy vertex buffer data
			*m_pIndexBuffer = *cSource.m_pIndexBuffer;
		} else {
			// Destroy vertex buffer
			delete m_pIndexBuffer;
			m_pIndexBuffer = nullptr;
		}
	} else {
		if (cSource.m_pIndexBuffer && m_pMesh) {
			// Create index buffer
			m_pIndexBuffer = m_pMesh->GetRenderer()->CreateIndexBuffer();

			// Copy index buffer data
			*m_pIndexBuffer = *cSource.m_pIndexBuffer;
		}
	}

	// Copy geometries
	CreateGeometries();
	Array<Geometry> *plstGeometries = cSource.GetGeometries();
	if (plstGeometries) {
		for (uint32 i=0; i<plstGeometries->GetNumOfElements(); i++) {
			Geometry &cGeometry = m_plstGeometries->Add();
			cGeometry = plstGeometries->Get(i);
		}
	}

	// Copy octree
	DestroyOctree();
	MeshOctree *pOctree = static_cast<const MeshLODLevel&>(cSource).GetOctree();
	if (pOctree)
		CreateOctree(pOctree->GetSubdivide(), pOctree->GetMinGeometries());

	// Copy precalculated data
	// Triangles
	m_lstTriangles.Resize(cSource.m_lstTriangles.GetNumOfElements());
	for (uint32 i=0; i<m_lstTriangles.GetNumOfElements(); i++)
		MemoryManager::Copy(&m_lstTriangles[i], &cSource.m_lstTriangles[i], m_lstTriangles.GetElementSize());
	// Edges
	m_lstEdges.Resize(cSource.m_lstEdges.GetNumOfElements());
	for (uint32 i=0; i<m_lstEdges.GetNumOfElements(); i++)
		MemoryManager::Copy(&m_lstEdges[i], &cSource.m_lstEdges[i], m_lstEdges.GetElementSize());

	// Return pointer
	return *this;
}


//[-------------------------------------------------------]
//[ Index buffer functions                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clears the index buffer for this LOD level
*/
void MeshLODLevel::ClearIndexBuffer()
{
	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
		m_pIndexBuffer = nullptr;
	}
}

/**
*  @brief
*    Creates the index buffer for this LOD level
*/
void MeshLODLevel::CreateIndexBuffer()
{
	ClearIndexBuffer();
	m_pIndexBuffer = (m_pMesh && m_pMesh->GetRenderer()) ? m_pMesh->GetRenderer()->CreateIndexBuffer() : nullptr;
}

/**
*  @brief
*    Returns the index buffer for this LOD level
*/
IndexBuffer *MeshLODLevel::GetIndexBuffer() const
{
	return m_pIndexBuffer;
}


//[-------------------------------------------------------]
//[ Geometry functions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the total number of LOD level triangles
*/
uint32 MeshLODLevel::GetNumOfTriangles() const
{
	// Check if there are geometries
	if (!m_plstGeometries)
		return 0;

	// Get number of LOD level triangles
	uint32 nNumOfTriangles = 0;
	for (uint32 i=0; i<m_plstGeometries->GetNumOfElements(); i++)
		nNumOfTriangles += m_plstGeometries->Get(i).GetNumOfTriangles();

	// Done
	return nNumOfTriangles;
}

/**
*  @brief
*    Clears the geometries for this LOD level
*/
void MeshLODLevel::ClearGeometries()
{
	if (m_plstGeometries) {
		delete m_plstGeometries;
		m_plstGeometries = nullptr;
	}

	// Destroy precalculated data
	m_lstTriangles.Clear();
	    m_lstEdges.Clear();
}

/**
*  @brief
*    Creates the geometries for this LOD level
*/
void MeshLODLevel::CreateGeometries()
{
	ClearGeometries();
	m_plstGeometries = new Array<Geometry>;
}

/**
*  @brief
*    Gets the geometries of this LOD level
*/
Array<Geometry> *MeshLODLevel::GetGeometries() const
{
	return m_plstGeometries;
}


//[-------------------------------------------------------]
//[ Visibility functions                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates the LOD level octree
*/
bool MeshLODLevel::CreateOctree(uint32 nSubdivide, uint32 nMinGeometries, Array<Array<uint32>*> *plstOctreeIDList)
{
	// Destroy old octree
	DestroyOctree();

	// Check if there are geometries
	if (m_plstGeometries) {
		// Check parameters
		if (nMinGeometries < 1)
			return false; // Error!

		// Create octree
		uint32  nGeometries	   = m_plstGeometries->GetNumOfElements();
		uint32 *plstGeometries = new uint32[nGeometries];

		m_pOctree = new MeshOctree();
		for (uint32 i=0; i<nGeometries; i++)
			plstGeometries[i] = i;
		m_pOctree->Init(nullptr, nSubdivide, nMinGeometries);
		m_pOctree->Build(*this, nGeometries, plstGeometries, plstOctreeIDList);
		delete [] plstGeometries;
	} else {
		// Create empty octree
		m_pOctree = new MeshOctree();
		m_pOctree->Init(nullptr, nSubdivide, nMinGeometries);
	}

	// Done
	return true;
}

/**
*  @brief
*    Destroys the LOD level octree
*/
void MeshLODLevel::DestroyOctree()
{
	if (m_pOctree) {
		delete m_pOctree;
		m_pOctree = nullptr;
	}
}

/**
*  @brief
*    Returns the LOD level octree
*/
MeshOctree *MeshLODLevel::GetOctree() const
{
	return m_pOctree;
}


//[-------------------------------------------------------]
//[ Tool functions                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Builds the connectivity information
*/
void MeshLODLevel::BuildConnectivity()
{
	BuildTriangleList();
	BuildEdgeList();
}

/**
*  @brief
*    Builds the current triangle list
*/
void MeshLODLevel::BuildTriangleList()
{
	// Lock index buffer
	if (m_pIndexBuffer && m_pIndexBuffer->Lock(Lock::ReadOnly)) {
		// Build triangle list
		uint32 nTriangle = 0;
		m_lstTriangles.Resize(GetNumOfTriangles());
		for (uint32 nGeo=0; nGeo<m_plstGeometries->GetNumOfElements(); nGeo++) {
			const Geometry &cGeometry = m_plstGeometries->Get(nGeo);
			for (uint32 nTri=0; nTri<cGeometry.GetNumOfTriangles(); nTri++) {
				// Get triangle data
				MeshTriangle &cTriangle = m_lstTriangles[nTriangle++];
				cTriangle.nGeometry = nGeo;
				GetTriangle(nGeo, nTri, cTriangle.nVertex[0],
										cTriangle.nVertex[1],
										cTriangle.nVertex[2]);
				cTriangle.nNeighbour[0] = cTriangle.nNeighbour[1] = cTriangle.nNeighbour[2] = -1;
			}
		}

		// Triangle connectivity determination
		for (uint32 i=0; i<m_lstTriangles.GetNumOfElements(); i++) {
			MeshTriangle &cTriangle = m_lstTriangles[i];
			for (uint32 j=i+1; j<m_lstTriangles.GetNumOfElements(); j++) {
				MeshTriangle &cTriangleT = m_lstTriangles[j];
				for (uint32 ki=0; ki<3; ki++) {
					if (cTriangle.nNeighbour[ki] < 0) {
						for (uint32 kj=0; kj<3; kj++) {
							int p1i = ki;
							int p1j = kj;
							int p2i = (ki+1)%3;
							int p2j = (kj+1)%3;

							p1i = cTriangle. nVertex[p1i];
							p2i = cTriangle. nVertex[p2i];
							p1j = cTriangleT.nVertex[p1j];
							p2j = cTriangleT.nVertex[p2j];

							int P1i = ((p1i+p2i)-Math::Abs(p1i-p2i))/2;
							int P2i = ((p1i+p2i)+Math::Abs(p1i-p2i))/2;
							int P1j = ((p1j+p2j)-Math::Abs(p1j-p2j))/2;
							int P2j = ((p1j+p2j)+Math::Abs(p1j-p2j))/2;

							if ((P1i==P1j) && (P2i==P2j)) {
								// They are neighbours
								cTriangle. nNeighbour[ki] = j+1;
								cTriangleT.nNeighbour[kj] = i+1;
							}
						}
					}
				}
			}
		}

		// Unlock the index buffer
		m_pIndexBuffer->Unlock();
	}
}

/**
*  @brief
*    Gets the triangle list
*/
Array<MeshTriangle> &MeshLODLevel::GetTriangleList()
{
	return m_lstTriangles;
}

/**
*  @brief
*    Builds the current edge list
*/
void MeshLODLevel::BuildEdgeList()
{
	// The following code builds a list of edges for an arbitrary triangle mesh and
	// has O(n) running time in the number of triangles n in the mesh.
	const MeshTriangle *pTriangle = &m_lstTriangles[0];
	uint32 nNumOfTriangles  = m_lstTriangles.GetNumOfElements();
	uint32 nVertexCount     = nNumOfTriangles*3; // ??
	uint32 nMaxEdgeCount    = nNumOfTriangles * 3;
	uint16 *pnFirstEdge     = new uint16[nVertexCount + nMaxEdgeCount];
	uint16 *pnNextEdge      = pnFirstEdge + nVertexCount;
	m_lstEdges.Resize(nMaxEdgeCount);

	for (uint32 a=0; a<nVertexCount; a++)
		pnFirstEdge[a] = 0xFFFF;

	// First pass over all triangles. This finds all the edges satisfying the
	// condition that the first vertex index is less than the second vertex index
	// when the direction from the first vertex to the second vertex represents
	// a counterclockwise winding around the triangle to which the edge belongs.
	// For each edge found, the edge index is stored in a linked list of edges
	// belonging to the lower-numbered vertex index i. This allows us to quickly
	// find an edge in the second pass whose higher-numbered vertex index is i.
	uint32 nEdgeCount = 0;
	for (uint32 a=0; a<nNumOfTriangles; a++) {
		uint32 i1 = pTriangle->nVertex[2];
		for (uint32 b=0; b<3; b++) {
			uint32 i2 = pTriangle->nVertex[b];
			if (i1 < i2) {
				MeshEdge &cEdge = m_lstEdges[nEdgeCount];

				cEdge.nVertex[0]   = static_cast<uint16>(i1);
				cEdge.nVertex[1]   = static_cast<uint16>(i2);
				cEdge.nTriangle[0] = static_cast<uint16>(a);
				cEdge.nTriangle[1] = static_cast<uint16>(a);

				uint32 nEdgeIndex = pnFirstEdge[i1];
				if (nEdgeIndex == 0xFFFF) {
					pnFirstEdge[i1] = static_cast<uint16>(nEdgeCount);
				} else {
					uint32 nIndex = pnNextEdge[nEdgeIndex];
					while (nIndex != 0xFFFF) {
						nEdgeIndex = nIndex;
						nIndex     = pnNextEdge[nEdgeIndex];
					}
					pnNextEdge[nEdgeIndex] = static_cast<uint16>(nEdgeCount);
				}

				pnNextEdge[nEdgeCount] = 0xFFFF;
				nEdgeCount++;
			}

			i1 = i2;
		}

		pTriangle++;
	}

	// Second pass over all triangles. This finds all the edges satisfying the
	// condition that the first vertex index is greater than the second vertex index
	// when the direction from the first vertex to the second vertex represents
	// a counterclockwise winding around the triangle to which the edge belongs.
	// For each of these edges, the same edge should have already been found in
	// the first pass for a different triangle. So we search the list of edges
	// for the higher-numbered vertex index for the matching edge and fill in the
	// second triangle index. The maximum number of comparisons in this search for
	// any vertex is the number of edges having that vertex as an endpoint.
	pTriangle = &m_lstTriangles[0];
	for (uint32 a=0; a<nNumOfTriangles; a++) {
		uint32 i1 = pTriangle->nVertex[2];
		for (uint32 b=0; b<3; b++) {
			uint32 i2 = pTriangle->nVertex[b];
			if (i1 > i2) {
				for (uint32 nEdgeIndex=pnFirstEdge[i2]; nEdgeIndex!=0xFFFF; nEdgeIndex=pnNextEdge[nEdgeIndex]) {
					MeshEdge &cEdge = m_lstEdges[nEdgeIndex];
					if ((cEdge.nVertex[1] == i1) && (cEdge.nTriangle[0] == cEdge.nTriangle[1])) {
						cEdge.nTriangle[1] = static_cast<uint16>(a);
						break;
					}
				}
			}

			i1 = i2;
		}

		pTriangle++;
	}

	// Cleanup
	delete [] pnFirstEdge;

	// Correct edge list size
	m_lstEdges.Resize(nEdgeCount);
}

/**
*  @brief
*    Gets the edge list
*/
Array<MeshEdge> &MeshLODLevel::GetEdgeList()
{
	return m_lstEdges;
}

/**
*  @brief
*    Gets a triangle of a geometry
*/
bool MeshLODLevel::GetTriangle(uint32 nGeometry, uint32 nIndex,
							   uint32 &nVertex1, uint32 &nVertex2, uint32 &nVertex3) const
{
	bool bResult = false; // Error by default

	// Check parameters
	const Geometry &cGeometry = m_plstGeometries->Get(nGeometry);
	if ((&cGeometry != &Array<Geometry>::Null) && m_pIndexBuffer && m_pIndexBuffer->Lock(Lock::ReadOnly)) {
		// Get triangle vertices
		uint32 nStartIndex = cGeometry.GetStartIndex();

		// Extract from a list of triangles
		if (cGeometry.GetPrimitiveType() == Primitive::TriangleList) {
			if (nIndex*3+2 < cGeometry.GetIndexSize()) {
				nVertex1 = m_pIndexBuffer->GetData(nStartIndex+nIndex*3+0);
				nVertex2 = m_pIndexBuffer->GetData(nStartIndex+nIndex*3+1);
				nVertex3 = m_pIndexBuffer->GetData(nStartIndex+nIndex*3+2);

				// Done
				bResult = true;
			}

		// Extract from a triangle strip
		} else if (cGeometry.GetPrimitiveType() == Primitive::TriangleStrip) {
			if (nIndex+2 < cGeometry.GetIndexSize()) {
				if (nIndex % 2) {
					nVertex1 = m_pIndexBuffer->GetData(nStartIndex+nIndex+1);
					nVertex2 = m_pIndexBuffer->GetData(nStartIndex+nIndex+0);
					nVertex3 = m_pIndexBuffer->GetData(nStartIndex+nIndex+2);
				} else {
					nVertex1 = m_pIndexBuffer->GetData(nStartIndex+nIndex+0);
					nVertex2 = m_pIndexBuffer->GetData(nStartIndex+nIndex+1);
					nVertex3 = m_pIndexBuffer->GetData(nStartIndex+nIndex+2);
				}

				// Done
				bResult = true;
			}

		// Extract from a triangle fan
		} else if (cGeometry.GetPrimitiveType() == Primitive::TriangleFan) {
			if (nIndex+2 <= cGeometry.GetIndexSize()) {
				nVertex1 = m_pIndexBuffer->GetData(nStartIndex+0);
				nVertex2 = m_pIndexBuffer->GetData(nStartIndex+nIndex+1);
				nVertex3 = m_pIndexBuffer->GetData(nStartIndex+nIndex+2);

				// Done
				bResult = true;
			}
		}

		// Unlock the index buffer
		m_pIndexBuffer->Unlock();
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Splits up geometries to geometries of single triangles
*/
bool MeshLODLevel::SplitGeometries(bool bSingleGeometries, uint32 *pSplit, uint32 nSplitNumber)
{
	// Lock current index buffer
	if (!m_pMesh || !m_pIndexBuffer || !m_pIndexBuffer->Lock(Lock::ReadWrite))
		return false; // Error!

	// Setup information about the geometries to split
	uint32 nNewIndexBufferSize = 0;
	Bitset cSplitGeometry(m_plstGeometries->GetNumOfElements(), true, true);
	if (pSplit) {
		// Mark the geometries which should be splitted
		for (uint32 i=0; i<nSplitNumber; i++) {
			cSplitGeometry.Set(*pSplit);
			pSplit++;
		}
		// Get the new number of indices
		for (uint32 i=0; i<m_plstGeometries->GetNumOfElements(); i++)
			nNewIndexBufferSize += cSplitGeometry.IsSet(i) ? m_plstGeometries->Get(i).GetNumOfTriangles()*3 : m_plstGeometries->Get(i).GetIndexSize();
	} else {
		cSplitGeometry.SetAll();
		nNewIndexBufferSize = GetNumOfTriangles()*3;
	}

	// Create the new geometries
	if (!nNewIndexBufferSize) {
		// Unlock the index buffer
		m_pIndexBuffer->Unlock();

		// Error!
		return false;
	}
	Array<Geometry> *plstGeometries = new Array<Geometry>;

	// Create new index buffer
	IndexBuffer *pIndexBuffer = m_pMesh->GetRenderer()->CreateIndexBuffer();
	pIndexBuffer->SetElementType(m_pIndexBuffer->GetElementType());
	pIndexBuffer->Allocate(nNewIndexBufferSize);

	// Setup the new geometries and index buffer
	if (pIndexBuffer->Lock(Lock::WriteOnly)) {
		uint32 nIndex      = 0;
		uint32 nStartIndex = 0;
		if (bSingleGeometries) {
			for (uint32 nGeometry=0; nGeometry<m_plstGeometries->GetNumOfElements(); nGeometry++) {
				// Get the geometry
				const Geometry cGeometry = m_plstGeometries->Get(nGeometry);

				// Check if we should split this geometry
				if (cSplitGeometry.IsSet(nGeometry)) {
					if (cGeometry.GetNumOfTriangles() > 0) {
						for (uint32 i=0; i<cGeometry.GetNumOfTriangles(); i++) {
							// Add new geometry
							Geometry &cNewGeometry = plstGeometries->Add();

							// Setup new geometry
							cNewGeometry.SetName(cGeometry.GetName());
							cNewGeometry.SetFlags(cGeometry.GetFlags());
							cNewGeometry.SetActive(cGeometry.IsActive());
							cNewGeometry.SetPrimitiveType(Primitive::TriangleList);
							cNewGeometry.SetMaterial(cGeometry.GetMaterial());
							cNewGeometry.SetStartIndex(nStartIndex);
							nStartIndex += 3;
							cNewGeometry.SetIndexSize(3);

							// Setup new index buffer
							uint32 nVertex1, nVertex2, nVertex3;
							GetTriangle(nGeometry, i, nVertex1, nVertex2, nVertex3);
							pIndexBuffer->SetData(nIndex++, nVertex1);
							pIndexBuffer->SetData(nIndex++, nVertex2);
							pIndexBuffer->SetData(nIndex++, nVertex3);
						}
					}
				} else {
					// No split, just copy the geometry
					Geometry &cNewGeometry = plstGeometries->Add();
					cNewGeometry = cGeometry;
					cNewGeometry.SetStartIndex(nStartIndex);
					for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
						pIndexBuffer->SetData(nIndex++, m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
					nStartIndex += cGeometry.GetIndexSize();
				}
			}
		} else {
			Bitset cUsed(m_plstGeometries->GetNumOfElements(), false, true);

			// Loop through all geometries
			for (uint32 nGT=0; nGT<m_plstGeometries->GetNumOfElements(); nGT++) {
				Geometry *pNewGeometry = nullptr;
				{ // Get the geometry
					const Geometry &cGeometry = m_plstGeometries->Get(nGT);

					// Check if we should split this geometry
					if (cGeometry.GetNumOfTriangles() < 1 || cUsed.IsSet(nGT))
						continue; // Invalid geometry!
					if (!cSplitGeometry.IsSet(nGT)) {
						// No split, just copy the geometry
						pNewGeometry = &plstGeometries->Add();
						*pNewGeometry = cGeometry;
						pNewGeometry->SetStartIndex(nStartIndex);
						for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
							pIndexBuffer->SetData(nIndex++, m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
						nStartIndex += cGeometry.GetIndexSize();
						cUsed.Set(nGT);
						continue;
					}

					// Setup new geometry
					pNewGeometry = &plstGeometries->Add();
					pNewGeometry->SetName(cGeometry.GetName());
					pNewGeometry->SetFlags(cGeometry.GetFlags());
					pNewGeometry->SetActive(cGeometry.IsActive());
					pNewGeometry->SetPrimitiveType(Primitive::TriangleList);
					pNewGeometry->SetMaterial(cGeometry.GetMaterial());
					pNewGeometry->SetStartIndex(nStartIndex);
					cUsed.Set(nGT);
					uint32 nNumOfTriangles = cGeometry.GetNumOfTriangles();
					pNewGeometry->SetIndexSize(nNumOfTriangles*3);
					for (uint32 i=0; i<nNumOfTriangles; i++) {
						uint32 nVertex1, nVertex2, nVertex3;
						GetTriangle(nGT, i, nVertex1, nVertex2, nVertex3);
						pIndexBuffer->SetData(nIndex++, nVertex1);
						pIndexBuffer->SetData(nIndex++, nVertex2);
						pIndexBuffer->SetData(nIndex++, nVertex3);
					}
					nStartIndex += nNumOfTriangles*3;
				}

				// Append other geometries with the same properties
				for (uint32 nGeometry=1; nGeometry<m_plstGeometries->GetNumOfElements(); nGeometry++) {
					// Now split the selected geometry
					Geometry &cGeometry = m_plstGeometries->Get(nGeometry);
					if (!cSplitGeometry.IsSet(nGeometry) || cGeometry.GetNumOfTriangles() < 1 ||
						cUsed.IsSet(nGeometry))
						continue; // Invalid geometry!

					// Check if we can append the triangles of this geometry to the other one
					if (pNewGeometry->GetName()     != cGeometry.GetName()     ||
						pNewGeometry->GetFlags()    != cGeometry.GetFlags()    ||
						pNewGeometry->IsActive()    != cGeometry.IsActive()    ||
						pNewGeometry->GetMaterial() != cGeometry.GetMaterial())
						continue; // We can't append it! :(

					// Append
					cUsed.Set(nGeometry);
					uint32 nNumOfTriangles = cGeometry.GetNumOfTriangles();
					pNewGeometry->SetIndexSize(pNewGeometry->GetIndexSize()+nNumOfTriangles*3);
					for (uint32 i=0; i<nNumOfTriangles; i++) {
						uint32 nVertex1, nVertex2, nVertex3;
						GetTriangle(nGeometry, i, nVertex1, nVertex2, nVertex3);
						pIndexBuffer->SetData(nIndex++, nVertex1);
						pIndexBuffer->SetData(nIndex++, nVertex2);
						pIndexBuffer->SetData(nIndex++, nVertex3);
					}
					nStartIndex += nNumOfTriangles*3;
				}
			}
		}

		// Unlock the index buffer
		pIndexBuffer->Unlock();
	}

	// Destroy old geometries and set new one
	delete m_plstGeometries;
	m_plstGeometries = plstGeometries;

	// Destroy old index buffer and set new one
	delete m_pIndexBuffer;
	m_pIndexBuffer = pIndexBuffer;

	// Destroy old octee
	DestroyOctree();

	// Done
	return true;
}

/**
*  @brief
*    Joins geometries with the same properties to one single geometry
*/
bool MeshLODLevel::JoinGeometries(uint32 *pJoin, uint32 nJoinNumber)
{
	// Lock current index buffer
	if (!m_pMesh || !m_pIndexBuffer || !m_pIndexBuffer->Lock(Lock::ReadWrite))
		return false; // Error!

	// Setup information about the geometries to join
	Bitset cJoinGeometry(m_plstGeometries->GetNumOfElements(), true, true);
	if (pJoin) {
		for (uint32 i=0; i<nJoinNumber; i++) {
			cJoinGeometry.Set(*pJoin);
			pJoin++;
		}
	} else {
		cJoinGeometry.SetAll();
	}

	// Create the new geometries
	Array<Geometry> *plstGeometries = new Array<Geometry>;

	// Create new index buffer - index buffer size is the same as before because only
	// triangles can be joined!
	IndexBuffer *pIndexBuffer = m_pMesh->GetRenderer()->CreateIndexBuffer();
	pIndexBuffer->SetElementType(m_pIndexBuffer->GetElementType());
	pIndexBuffer->Allocate(m_pIndexBuffer->GetNumOfElements());

	// Setup the new geometries and index buffer
	if (pIndexBuffer->Lock(Lock::WriteOnly)) {
		uint32 nIndex = 0;
		uint32 nStartIndex = 0;
		Bitset cUsed(m_plstGeometries->GetNumOfElements(), true, true);

		// Loop through all geometries
		for (uint32 nGT=0; nGT<m_plstGeometries->GetNumOfElements(); nGT++) {
			Geometry &cGeometry = m_plstGeometries->Get(nGT);

			// Check if we should join this geometry
			if (cGeometry.GetNumOfTriangles() < 1 || cUsed.IsSet(nGT))
				continue; // Invalid geometry!
			if (!cJoinGeometry.IsSet(nGT) ||
				cGeometry.GetPrimitiveType() == Primitive::LineStrip ||
				cGeometry.GetPrimitiveType() == Primitive::TriangleStrip ||
				cGeometry.GetPrimitiveType() == Primitive::TriangleFan) {
				// No join, just copy the geometry
				Geometry &cNewGeometry = plstGeometries->Add();
				cNewGeometry = cGeometry;
				cNewGeometry.SetStartIndex(nStartIndex);
				for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
					pIndexBuffer->SetData(nIndex++, m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
				nStartIndex += cGeometry.GetIndexSize();
				cUsed.Set(nGT);
				continue;
			}
			// Setup new geometry
			Geometry &cNewGeometry = plstGeometries->Add();
			cNewGeometry.SetName(cGeometry.GetName());
			cNewGeometry.SetFlags(cGeometry.GetFlags());
			cNewGeometry.SetActive(cGeometry.IsActive());
			cNewGeometry.SetPrimitiveType(cGeometry.GetPrimitiveType());
			cNewGeometry.SetMaterial(cGeometry.GetMaterial());
			cNewGeometry.SetStartIndex(nStartIndex);
			cNewGeometry.SetIndexSize(cGeometry.GetIndexSize());
			for (uint32 i=0; i<cNewGeometry.GetIndexSize(); i++)
				pIndexBuffer->SetData(nIndex++, m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
			nStartIndex += cNewGeometry.GetIndexSize();
			cUsed.Set(nGT);

			// Append other geometries with the same properties
			for (uint32 nGeometry=1; nGeometry<m_plstGeometries->GetNumOfElements(); nGeometry++) {
				// Now join the selected geometry
				cGeometry = m_plstGeometries->Get(nGeometry);
				if (!cJoinGeometry.IsSet(nGeometry) || cGeometry.GetNumOfTriangles() < 1 ||
					cUsed.IsSet(nGeometry))
					continue; // Invalid geometry!

				// Check if we can append the indices of this geometry to the other one
				if (cNewGeometry.GetName()			 != cGeometry.GetName() ||
					cNewGeometry.GetFlags()			 != cGeometry.GetFlags() ||
					cNewGeometry.IsActive()			 != cGeometry.IsActive() ||
					cNewGeometry.GetPrimitiveType()  != cGeometry.GetPrimitiveType() ||
					cNewGeometry.GetMaterial()		 != cGeometry.GetMaterial())
					continue; // We can't append it! :(

				// Append
				cUsed.Set(nGeometry);
				cNewGeometry.SetIndexSize(cNewGeometry.GetIndexSize()+cGeometry.GetIndexSize());
				for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
					pIndexBuffer->SetData(nIndex++, m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
				nStartIndex += cGeometry.GetIndexSize();
			}
		}

		// Unlock the index buffer
		pIndexBuffer->Unlock();
	}

	// Destroy old geometries and set new one
	delete m_plstGeometries;
	m_plstGeometries = plstGeometries;

	// Destroy old index buffer and set new one
	delete m_pIndexBuffer;
	m_pIndexBuffer = pIndexBuffer;

	// Destroy old octee
	DestroyOctree();

	// Done
	return true;
}

/**
*  @brief
*    Optimizes the geometries for an octree
*/
bool MeshLODLevel::GenerateOctreeGeometries(uint32 nSubdivide, uint32 nMinGeometries)
{
	// Is there an index buffer?
	if (!m_pIndexBuffer)
		return false; // Error!

	// Split up geometries
	SplitGeometries(true);

	// Create octree ID list
	Array<Array<uint32>*> lstOctreeIDList;
	for (uint32 i=0; i<m_plstGeometries->GetNumOfElements(); i++)
		lstOctreeIDList.Add(new Array<uint32>);

	// Create the temp octree
	CreateOctree(nSubdivide, nMinGeometries, &lstOctreeIDList);

	// Now we have to join all geometries with the same attributes
	if (m_pIndexBuffer->GetElementType() == IndexBuffer::UInt) {
		// Lock current index buffer
		if (!m_pIndexBuffer || !m_pIndexBuffer->Lock(Lock::ReadWrite))
			return false; // Error!

		// Create the new geometries
		Array<Array<uint32>*>  lstNewOctreeIDList;
		Array<uint32*>		   lstNewGeometryIndexBuffer;
		Array<Geometry>		  *plstGeometries = new Array<Geometry>;

		// Setup the new geometries and index buffer
		for (uint32 nGeometry=0; nGeometry<m_plstGeometries->GetNumOfElements(); nGeometry++) {
			const Geometry &cGeometry = m_plstGeometries->Get(nGeometry);

			// Now split the selected geometry
			if (cGeometry.GetNumOfTriangles() > 0) {
				// Check if this geometry can be joint with a new geometry
				Geometry *pNewGeometry = nullptr;
				for (uint32 nNewGeometry=0; nNewGeometry<plstGeometries->GetNumOfElements(); nNewGeometry++) {
					pNewGeometry = &plstGeometries->Get(nNewGeometry);

					// Get number of identical octrees
					uint32 nIdOctrees = 0;
					if (lstOctreeIDList[nGeometry] && lstNewOctreeIDList[nNewGeometry]) {
						for (uint32 i=0; ; i++) {
							if (i >= lstOctreeIDList[nGeometry]->GetNumOfElements() ||
								i >= lstNewOctreeIDList[nNewGeometry]->GetNumOfElements())
								break;
							if (lstOctreeIDList[nGeometry]->Get(i) == lstNewOctreeIDList[nNewGeometry]->Get(i))
								nIdOctrees++;
						}
					}

					// Check if we can join this two geometries
					if (cGeometry.GetName()          == pNewGeometry->GetName() &&
						cGeometry.GetFlags()         == pNewGeometry->GetFlags() &&
						cGeometry.IsActive()         == pNewGeometry->IsActive() &&
						cGeometry.GetPrimitiveType() == pNewGeometry->GetPrimitiveType() &&
						cGeometry.GetMaterial()      == pNewGeometry->GetMaterial() &&
						pNewGeometry->GetNumOfTriangles() < nMinGeometries && nIdOctrees) {
	//					*lstOctreeIDList[nGeometry]   == *lstNewOctreeIDList[nNewGeometry]) {
						// We can join the two geometries! Add the indices of this geometry to the
						// new one:
						uint32 *pIndexT       = lstNewGeometryIndexBuffer[nNewGeometry];
						uint32  nNewIndexSize = pNewGeometry->GetIndexSize()+cGeometry.GetIndexSize();
						pIndexT = static_cast<uint32*>(MemoryManager::Reallocator(pIndexT, sizeof(uint32)*nNewIndexSize));
						for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
							pIndexT[pNewGeometry->GetIndexSize()+i] = m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i);
						pNewGeometry->SetIndexSize(nNewIndexSize);
						lstNewGeometryIndexBuffer.ReplaceAtIndex(nNewGeometry, pIndexT);
						break;
					}
				}
				// It wasn't possible to join geometries, add a new one
				if (!pNewGeometry) {
					// Add geometry
					pNewGeometry = &plstGeometries->Add();
					pNewGeometry->SetName(cGeometry.GetName());
					pNewGeometry->SetFlags(cGeometry.GetFlags());
					pNewGeometry->SetActive(cGeometry.IsActive());
					pNewGeometry->SetPrimitiveType(Primitive::TriangleList);
					pNewGeometry->SetMaterial(cGeometry.GetMaterial());
					pNewGeometry->SetStartIndex(0); // Set later to correct value!
					pNewGeometry->SetIndexSize(cGeometry.GetIndexSize());

					// Add temp octree ID list
					Array<uint32> *pNewIDs = new Array<uint32>;
					*pNewIDs = *lstOctreeIDList[nGeometry];
					lstNewOctreeIDList.Add(pNewIDs);

					// Add temp geometry index buffer
					uint32 *pIndexT = new uint32[cGeometry.GetIndexSize()];
					for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
						pIndexT[i] = m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i);
					lstNewGeometryIndexBuffer.Add(pIndexT);
				}
			}
		}

		// The number of indices is the same as before because we only joint geometries
		// of triangles! Therefore we only have to fill the index buffer with the new values
		// and to set the start index of each new geometry:
		uint32 nStartIndex = 0;
		for (uint32 nGeometry=0; nGeometry<plstGeometries->GetNumOfElements(); nGeometry++) {
			Geometry &cGeometry = plstGeometries->Get(nGeometry);
			for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
				m_pIndexBuffer->SetData(nStartIndex+i, lstNewGeometryIndexBuffer[nGeometry][i]);
			cGeometry.SetStartIndex(nStartIndex);
			nStartIndex += cGeometry.GetIndexSize();
		}

		// Unlock the index buffer
		m_pIndexBuffer->Unlock();

		// Destroy new octree ID list
		for (uint32 i=0; i<lstNewOctreeIDList.GetNumOfElements(); i++)
			delete lstNewOctreeIDList[i];
		lstNewOctreeIDList.Clear();

		// Destroy temp index buffers
		for (uint32 i=0; i<lstNewGeometryIndexBuffer.GetNumOfElements(); i++)
			delete lstNewGeometryIndexBuffer[i];
		lstNewGeometryIndexBuffer.Clear();

		// Destroy old geometries and set new one
		delete m_plstGeometries;
		m_plstGeometries = plstGeometries;
	} else if (m_pIndexBuffer->GetElementType() == IndexBuffer::UShort) {
		// Lock current index buffer
		if (!m_pIndexBuffer || !m_pIndexBuffer->Lock(Lock::ReadWrite))
			return false; // Error!

		// Create the new geometries
		Array<Array<uint32>*>  lstNewOctreeIDList;
		Array<uint16*>		   lstNewGeometryIndexBuffer;
		Array<Geometry>		  *plstGeometries = new Array<Geometry>;

		// Setup the new geometries and index buffer
		for (uint32 nGeometry=0; nGeometry<m_plstGeometries->GetNumOfElements(); nGeometry++) {
			const Geometry &cGeometry = m_plstGeometries->Get(nGeometry);

			// Now split the selected geometry
			if (cGeometry.GetNumOfTriangles() > 0) {
				// Check if this geometry can be joint with a new geometry
				Geometry *pNewGeometry = nullptr;
				for (uint32 nNewGeometry=0; nNewGeometry<plstGeometries->GetNumOfElements(); nNewGeometry++) {
					pNewGeometry = &plstGeometries->Get(nNewGeometry);
					if (cGeometry.GetName()          == pNewGeometry->GetName() &&
						cGeometry.GetFlags()         == pNewGeometry->GetFlags() &&
						cGeometry.IsActive()         == pNewGeometry->IsActive() &&
						cGeometry.GetPrimitiveType() == pNewGeometry->GetPrimitiveType() &&
						cGeometry.GetMaterial()      == pNewGeometry->GetMaterial() &&
						*lstOctreeIDList[nGeometry]  == *lstNewOctreeIDList[nNewGeometry]) {
						// We can join the two geometries! Add the indices of this geometry to the
						// new one:
						uint16 *pIndexT       = lstNewGeometryIndexBuffer[nNewGeometry];
						uint32  nNewIndexSize = pNewGeometry->GetIndexSize()+cGeometry.GetIndexSize();
						pIndexT = static_cast<uint16*>(MemoryManager::Reallocator(pIndexT, sizeof(uint16)*nNewIndexSize));
						for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
							pIndexT[pNewGeometry->GetIndexSize()+i] = static_cast<uint16>(m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
						pNewGeometry->SetIndexSize(nNewIndexSize);
						lstNewGeometryIndexBuffer.ReplaceAtIndex(nNewGeometry, pIndexT);
						break;
					}
				}
				// It wasn't possible to join geometries, add a new one
				if (!pNewGeometry) {
					// Add geometry
					pNewGeometry = &plstGeometries->Add();
					pNewGeometry->SetName(cGeometry.GetName());
					pNewGeometry->SetFlags(cGeometry.GetFlags());
					pNewGeometry->SetActive(cGeometry.IsActive());
					pNewGeometry->SetPrimitiveType(Primitive::TriangleList);
					pNewGeometry->SetMaterial(cGeometry.GetMaterial());
					pNewGeometry->SetStartIndex(0); // Set later to correct value!
					pNewGeometry->SetIndexSize(cGeometry.GetIndexSize());

					// Add temp octree ID list
					Array<uint32> *pNewIDs = new Array<uint32>;
					*pNewIDs = *lstOctreeIDList[nGeometry];
					lstNewOctreeIDList.Add(pNewIDs);

					// Add temp geometry index buffer
					uint16 *pIndexT = new uint16[cGeometry.GetIndexSize()];
					for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
						pIndexT[i] = static_cast<uint16>(m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
					lstNewGeometryIndexBuffer.Add(pIndexT);
				}
			}
		}

		// The number of indices is the same as before because we only joint geometries
		// of triangles! Therefore we only have to fill the index buffer with the new values
		// and to set the start index of each new geometry:
		uint32 nStartIndex = 0;
		for (uint32 nGeometry=0; nGeometry<plstGeometries->GetNumOfElements(); nGeometry++) {
			Geometry &cGeometry = plstGeometries->Get(nGeometry);
			for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
				m_pIndexBuffer->SetData(nStartIndex+i, lstNewGeometryIndexBuffer[nGeometry][i]);
			cGeometry.SetStartIndex(nStartIndex);
			nStartIndex += cGeometry.GetIndexSize();
		}

		// Unlock the index buffer
		m_pIndexBuffer->Unlock();

		// Destroy new octree ID list
		for (uint32 i=0; i<lstNewOctreeIDList.GetNumOfElements(); i++)
			delete lstNewOctreeIDList[i];
		lstNewOctreeIDList.Clear();

		// Destroy temp index buffers
		for (uint32 i=0; i<lstNewGeometryIndexBuffer.GetNumOfElements(); i++)
			delete lstNewGeometryIndexBuffer[i];
		lstNewGeometryIndexBuffer.Clear();

		// Destroy old geometries and set new one
		delete m_plstGeometries;
		m_plstGeometries = plstGeometries;
	} else {
		// Lock current index buffer
		if (!m_pIndexBuffer || !m_pIndexBuffer->Lock(Lock::ReadWrite))
			return false; // Error!

		// Create the new geometries
		Array<Array<uint32>*>  lstNewOctreeIDList;
		Array<uint8*>		   lstNewGeometryIndexBuffer;
		Array<Geometry>		  *plstGeometries = new Array<Geometry>;

		// Setup the new geometries and index buffer
		for (uint32 nGeometry=0; nGeometry<m_plstGeometries->GetNumOfElements(); nGeometry++) {
			const Geometry &cGeometry = m_plstGeometries->Get(nGeometry);

			// Now split the selected geometry
			if (cGeometry.GetNumOfTriangles() > 0) {
				// Check if this geometry can be joint with a new geometry
				Geometry *pNewGeometry = nullptr;
				for (uint32 nNewGeometry=0; nNewGeometry<plstGeometries->GetNumOfElements(); nNewGeometry++) {
					pNewGeometry = &plstGeometries->Get(nNewGeometry);
					if (cGeometry.GetName()          == pNewGeometry->GetName() &&
						cGeometry.GetFlags()         == pNewGeometry->GetFlags() &&
						cGeometry.IsActive()         == pNewGeometry->IsActive() &&
						cGeometry.GetPrimitiveType() == pNewGeometry->GetPrimitiveType() &&
						cGeometry.GetMaterial()      == pNewGeometry->GetMaterial() &&
						*lstOctreeIDList[nGeometry]  == *lstNewOctreeIDList[nNewGeometry]) {
						// We can join the two geometries! Add the indices of this geometry to the
						// new one:
						uint8  *pIndexT       = lstNewGeometryIndexBuffer[nNewGeometry];
						uint32  nNewIndexSize = pNewGeometry->GetIndexSize()+cGeometry.GetIndexSize();
						pIndexT = static_cast<uint8*>(MemoryManager::Reallocator(pIndexT, sizeof(uint8)*nNewIndexSize));
						for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
							pIndexT[pNewGeometry->GetIndexSize()+i] = static_cast<uint8>(m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
						pNewGeometry->SetIndexSize(nNewIndexSize);
						lstNewGeometryIndexBuffer.ReplaceAtIndex(nNewGeometry, pIndexT);
						break;
					}
				}
				// It wasn't possible to join geometries, add a new one
				if (!pNewGeometry) {
					// Add geometry
					pNewGeometry = &plstGeometries->Add();
					pNewGeometry->SetName(cGeometry.GetName());
					pNewGeometry->SetFlags(cGeometry.GetFlags());
					pNewGeometry->SetActive(cGeometry.IsActive());
					pNewGeometry->SetPrimitiveType(Primitive::TriangleList);
					pNewGeometry->SetMaterial(cGeometry.GetMaterial());
					pNewGeometry->SetStartIndex(0); // Set later to correct value!
					pNewGeometry->SetIndexSize(cGeometry.GetIndexSize());

					// Add temp octree ID list
					Array<uint32> *pNewIDs = new Array<uint32>;
					*pNewIDs = *lstOctreeIDList[nGeometry];
					lstNewOctreeIDList.Add(pNewIDs);

					// Add temp geometry index buffer
					uint8 *pIndexT = new uint8[cGeometry.GetIndexSize()];
					for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
						pIndexT[i] = static_cast<uint8>(m_pIndexBuffer->GetData(cGeometry.GetStartIndex()+i));
					lstNewGeometryIndexBuffer.Add(pIndexT);
				}
			}
		}

		// The number of indices is the same as before because we only joint geometries
		// of triangles! Therefore we only have to fill the index buffer with the new values
		// and to set the start index of each new geometry:
		uint32 nStartIndex = 0;
		for (uint32 nGeometry=0; nGeometry<plstGeometries->GetNumOfElements(); nGeometry++) {
			Geometry &cGeometry = plstGeometries->Get(nGeometry);
			for (uint32 i=0; i<cGeometry.GetIndexSize(); i++)
				m_pIndexBuffer->SetData(nStartIndex+i, lstNewGeometryIndexBuffer[nGeometry][i]);
			cGeometry.SetStartIndex(nStartIndex);
			nStartIndex += cGeometry.GetIndexSize();
		}

		// Unlock the index buffer
		m_pIndexBuffer->Unlock();

		// Destroy new octree ID list
		for (uint32 i=0; i<lstNewOctreeIDList.GetNumOfElements(); i++)
			delete lstNewOctreeIDList[i];
		lstNewOctreeIDList.Clear();

		// Destroy temp index buffers
		for (uint32 i=0; i<lstNewGeometryIndexBuffer.GetNumOfElements(); i++)
			delete lstNewGeometryIndexBuffer[i];
		lstNewGeometryIndexBuffer.Clear();

		// Destroy old geometries and set new one
		delete m_plstGeometries;
		m_plstGeometries = plstGeometries;
	}

	// Destroy the temp octree
	DestroyOctree();

	// Destroy octree ID list
	for (uint32 i=0; i<lstOctreeIDList.GetNumOfElements(); i++)
		delete lstOctreeIDList[i];
	lstOctreeIDList.Clear();

	// Done
	return true;
}

/**
*  @brief
*    Optimizes the geometries using triangle strips
*/
bool MeshLODLevel::GenerateStrips(uint32 nVertexCacheSize, uint32 nMinStripLength)
{
	// [TODO] Implement me

	// Error!
	return false;
}

/**
*  @brief
*    Calculates the LOD level bounding box
*/
bool MeshLODLevel::CalculateBoundingBox(const Array<const Geometry*> &lstGeometries, VertexBuffer &cVertexBuffer, Vector3 &vMinPos, Vector3 &vMaxPos) const
{
	// Initialize the parameters
	vMinPos = Vector3::Zero;
	vMaxPos = Vector3::Zero;

	// Index buffer given?
	if (m_pIndexBuffer) {
		// Lock the vertex buffer
		if (cVertexBuffer.Lock(Lock::ReadOnly)) {
			// Lock the index buffer
			if (m_pIndexBuffer->Lock(Lock::ReadOnly)) {
				// We have to take the indices type into account...
				switch (m_pIndexBuffer->GetElementType()) {
					case IndexBuffer::UInt:
					{
						const uint32 *pIndices = static_cast<const uint32*>(m_pIndexBuffer->GetData());
						if (m_pIndexBuffer->GetNumOfElements()) {
							// Loop through all geometries
							bool bFirst = true;
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 i=pGeometry->GetStartIndex(); i<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); i++) {
										// Check index
										if (i < m_pIndexBuffer->GetNumOfElements()) {
											const float *pfPos = static_cast<const float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
											if (bFirst) {
												vMinPos = vMaxPos = pfPos;
												bFirst = false;
											} else {
												// Min
												if (vMinPos.x > pfPos[0])
													vMinPos.x = pfPos[0];
												if (vMinPos.y > pfPos[1])
													vMinPos.y = pfPos[1];
												if (vMinPos.z > pfPos[2])
													vMinPos.z = pfPos[2];
												// Max
												if (vMaxPos.x < pfPos[0])
													vMaxPos.x = pfPos[0];
												if (vMaxPos.y < pfPos[1])
													vMaxPos.y = pfPos[1];
												if (vMaxPos.z < pfPos[2])
													vMaxPos.z = pfPos[2];
											}
										}
									}
								}
							}
						}
						break;
					}

					case IndexBuffer::UShort:
					{
						const uint16 *pIndices = static_cast<const uint16*>(m_pIndexBuffer->GetData());
						if (m_pIndexBuffer->GetNumOfElements()) {
							// Loop through all geometries
							bool bFirst = true;
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 i=pGeometry->GetStartIndex(); i<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); i++) {
										// Check index
										if (i < m_pIndexBuffer->GetNumOfElements()) {
											const float *pfPos = static_cast<const float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
											if (bFirst) {
												vMinPos = vMaxPos = pfPos;
												bFirst = false;
											} else {
												// Min
												if (vMinPos.x > pfPos[Vector3::X])
													vMinPos.x = pfPos[Vector3::X];
												if (vMinPos.y > pfPos[Vector3::Y])
													vMinPos.y = pfPos[Vector3::Y];
												if (vMinPos.z > pfPos[Vector3::Z])
													vMinPos.z = pfPos[Vector3::Z];
												// Max
												if (vMaxPos.x < pfPos[Vector3::X])
													vMaxPos.x = pfPos[Vector3::X];
												if (vMaxPos.y < pfPos[Vector3::Y])
													vMaxPos.y = pfPos[Vector3::Y];
												if (vMaxPos.z < pfPos[Vector3::Z])
													vMaxPos.z = pfPos[Vector3::Z];
											}
										}
									}
								}
							}
						}
						break;
					}

					case IndexBuffer::UByte:
					{
						const uint8 *pIndices = static_cast<const uint8*>(m_pIndexBuffer->GetData());
						if (m_pIndexBuffer->GetNumOfElements()) {
							// Loop through all geometries
							bool bFirst = true;
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 i=pGeometry->GetStartIndex(); i<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); i++) {
										// Check index
										if (i < m_pIndexBuffer->GetNumOfElements()) {
											const float *pfPos = static_cast<const float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
											if (bFirst) {
												vMinPos = vMaxPos = pfPos;
												bFirst = false;
											} else {
												// Min
												if (vMinPos.x > pfPos[Vector3::X])
													vMinPos.x = pfPos[Vector3::X];
												if (vMinPos.y > pfPos[Vector3::Y])
													vMinPos.y = pfPos[Vector3::Y];
												if (vMinPos.z > pfPos[Vector3::Z])
													vMinPos.z = pfPos[Vector3::Z];
												// Max
												if (vMaxPos.x < pfPos[Vector3::X])
													vMaxPos.x = pfPos[Vector3::X];
												if (vMaxPos.y < pfPos[Vector3::Y])
													vMaxPos.y = pfPos[Vector3::Y];
												if (vMaxPos.z < pfPos[Vector3::Z])
													vMaxPos.z = pfPos[Vector3::Z];
											}
										}
									}
								}
							}
						}
						break;
					}
				}

				// Unlock the given index buffer
				m_pIndexBuffer->Unlock();
			}

			// Unlock the vertex buffer
			cVertexBuffer.Unlock();
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates the LOD level bounding sphere
*/
bool MeshLODLevel::CalculateBoundingSphere(const Array<const Geometry*> &lstGeometries, VertexBuffer &cVertexBuffer, Vector3 &vPos, float &fRadius) const
{
	// Initialize the parameters
	vPos    = Vector3::Zero;
	fRadius = 0.0f;

	// Index buffer given?
	if (m_pIndexBuffer) {
		// Lock the vertex buffer
		if (cVertexBuffer.Lock(Lock::ReadOnly)) {
			// Lock the given index buffer
			if (m_pIndexBuffer->Lock(Lock::ReadOnly)) {
				// We have to take the indices type into account...
				switch (m_pIndexBuffer->GetElementType()) {
					case IndexBuffer::UInt:
					{
						const uint32 *pIndices = static_cast<const uint32*>(m_pIndexBuffer->GetData());
						if (m_pIndexBuffer->GetNumOfElements()) {
							uint32 nNumOfPoints = 0;

							// First: Average points to get approximate center
							// Loop through all geometries
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 i=pGeometry->GetStartIndex(); i<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); i++) {
										// Check index
										if (i < m_pIndexBuffer->GetNumOfElements()) {
											const float *pfPos = static_cast<const float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
											vPos.x += pfPos[0];
											vPos.y += pfPos[1];
											vPos.z += pfPos[2];
										}
									}
									nNumOfPoints += pGeometry->GetIndexSize();
								}
							}
							vPos /= static_cast<float>(nNumOfPoints);

							// Second: Find maximum distance from center (sphere radius)
							// Loop through all geometries
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 nIndex=pGeometry->GetStartIndex(); nIndex<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); nIndex++) {
										// Check index
										if (nIndex < m_pIndexBuffer->GetNumOfElements()) {
											for (uint32 i=0; i<m_pIndexBuffer->GetNumOfElements(); i++) {
												Vector3 vV = static_cast<float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
												fRadius    = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
											}
										}
									}
								}
							}
							fRadius = fRadius ? Math::Sqrt(fRadius) : 0.0f;
						}
						break;
					}

					case IndexBuffer::UShort:
					{
						const uint16 *pIndices = static_cast<const uint16*>(m_pIndexBuffer->GetData());
						if (m_pIndexBuffer->GetNumOfElements()) {
							uint32 nNumOfPoints = 0;

							// First: Average points to get approximate center
							// Loop through all geometries
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 i=pGeometry->GetStartIndex(); i<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); i++) {
										// Check index
										if (i < m_pIndexBuffer->GetNumOfElements()) {
											const float *pfPos = static_cast<const float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
											vPos.x += pfPos[0];
											vPos.y += pfPos[1];
											vPos.z += pfPos[2];
										}
									}
									nNumOfPoints += pGeometry->GetIndexSize();
								}
							}
							vPos /= static_cast<float>(nNumOfPoints);

							// Second: Find maximum distance from center (sphere radius)
							// Loop through all geometries
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 nIndex=pGeometry->GetStartIndex(); nIndex<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); nIndex++) {
										// Check index
										if (nIndex < m_pIndexBuffer->GetNumOfElements()) {
											for (uint32 i=0; i<m_pIndexBuffer->GetNumOfElements(); i++) {
												Vector3 vV = static_cast<float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
												fRadius    = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
											}
										}
									}
								}
							}
							fRadius = fRadius ? Math::Sqrt(fRadius) : 0.0f;
						}
						break;
					}

					case IndexBuffer::UByte:
					{
						const uint8 *pIndices = static_cast<const uint8*>(m_pIndexBuffer->GetData());
						if (m_pIndexBuffer->GetNumOfElements()) {
							uint32 nNumOfPoints = 0;

							// First: Average points to get approximate center
							// Loop through all geometries
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 i=pGeometry->GetStartIndex(); i<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); i++) {
										// Check index
										if (i < m_pIndexBuffer->GetNumOfElements()) {
											const float *pfPos = static_cast<const float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
											vPos.x += pfPos[0];
											vPos.y += pfPos[1];
											vPos.z += pfPos[2];
										}
									}
									nNumOfPoints += pGeometry->GetIndexSize();
								}
							}
							vPos /= static_cast<float>(nNumOfPoints);

							// Second: Find maximum distance from center (sphere radius)
							// Loop through all geometries
							for (uint32 nGeometry=0; nGeometry<lstGeometries.GetNumOfElements(); nGeometry++) {
								// Get the geometry
								const Geometry *pGeometry = lstGeometries[nGeometry];
								if (pGeometry) {
									// Loop through all indices
									for (uint32 nIndex=pGeometry->GetStartIndex(); nIndex<pGeometry->GetStartIndex()+pGeometry->GetIndexSize(); nIndex++) {
										// Check index
										if (nIndex < m_pIndexBuffer->GetNumOfElements()) {
											for (uint32 i=0; i<m_pIndexBuffer->GetNumOfElements(); i++) {
												Vector3 vV = static_cast<float*>(cVertexBuffer.GetData(pIndices[i], VertexBuffer::Position));
												fRadius    = Math::Max(fRadius, (vV-vPos).GetSquaredLength());
											}
										}
									}
								}
							}
							fRadius = fRadius ? Math::Sqrt(fRadius) : 0.0f;
						}
						break;
					}
				}

				// Unlock the given index buffer
				m_pIndexBuffer->Unlock();
			}

			// Unlock this vertex buffer
			cVertexBuffer.Unlock();
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
