/*********************************************************\
 *  File: MeshOctree.cpp                                 *
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
#include <PLCore/Container/Bitset.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Mesh.h"
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/MeshOctree.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MeshOctree::MeshOctree() :
	m_pOwnerMeshLODLevel(nullptr),
	m_nNumOfGeometries(0),
	m_pnGeometries(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
MeshOctree::~MeshOctree()
{
	if (m_pnGeometries)
		delete [] m_pnGeometries;
}

/**
*  @brief
*    Builds the octree
*/
bool MeshOctree::Build(MeshLODLevel &cMeshLODLevel, uint32 nNumOfGeometries, const uint32 nGeometries[],
					   Array<Array<uint32>*> *plstOctreeIDList)
{
	// Check parameters
	if (!cMeshLODLevel.GetMesh() || !cMeshLODLevel.GetMesh()->GetMorphTarget() ||
		!cMeshLODLevel.GetMesh()->GetMorphTarget()->GetVertexBuffer() || !cMeshLODLevel.GetIndexBuffer())
		return false; // Error!

	// Store geometries
	m_pOwnerMeshLODLevel = &cMeshLODLevel;
	m_bBuild			 = true;
	m_nNumOfGeometries	 = nNumOfGeometries;
	m_pnGeometries		 = new uint32[m_nNumOfGeometries];
	MemoryManager::Copy(m_pnGeometries, nGeometries, sizeof(uint32)*m_nNumOfGeometries);

	// Get vertex and index buffer pointers
	// [TODO] Get right morph target and LOD level!
	VertexBuffer *pVertexBuffer = cMeshLODLevel.GetMesh()->GetMorphTarget()->GetVertexBuffer();
	IndexBuffer  *pIndexBuffer  = cMeshLODLevel.GetIndexBuffer();

	// Lock buffers
	if (!m_pParent) {
		if (!pIndexBuffer->Lock(Lock::ReadOnly))
			return false; // Error!
		if (!pVertexBuffer->Lock(Lock::ReadOnly)) {
			// Unlock the index buffer
			pIndexBuffer->Unlock();

			// Error!
			return false;
		}
	}

	// Init bounding box with the first vertex of the geometry
	Vector3 vBoundingBox[2];
	{
		const Geometry &cGeometry = cMeshLODLevel.GetGeometries()->Get(m_pnGeometries[0]);
		for (int i=0; i<3; i++) {
			vBoundingBox[0][i] = 
			vBoundingBox[1][i] = static_cast<float*>(pVertexBuffer->GetData(pIndexBuffer->GetData(cGeometry.GetStartIndex()), VertexBuffer::Position))[i];
		}
	}

	// Get octree bounding box
	for (uint32 nGeometry=0; nGeometry<m_nNumOfGeometries; nGeometry++) {
		// Get geometry
		const Geometry &cGeometry = cMeshLODLevel.GetGeometries()->Get(m_pnGeometries[nGeometry]);

		// Add octree ID to the geometry octree list
		if (plstOctreeIDList)
			plstOctreeIDList->Get(m_pnGeometries[nGeometry])->Add(m_nID);

		// Loop through geometry vertices
		uint32 nIndex = cGeometry.GetStartIndex();
		for (uint32 i=0; i<cGeometry.GetIndexSize(); i++) {
			const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(pIndexBuffer->GetData(nIndex++), VertexBuffer::Position));
			if (vBoundingBox[0].x > pfVertex[Vector3::X])
				vBoundingBox[0].x = pfVertex[Vector3::X];
			if (vBoundingBox[1].x < pfVertex[Vector3::X])
				vBoundingBox[1].x = pfVertex[Vector3::X];
			if (vBoundingBox[0].y > pfVertex[Vector3::Y])
				vBoundingBox[0].y = pfVertex[Vector3::Y];
			if (vBoundingBox[1].y < pfVertex[Vector3::Y])
				vBoundingBox[1].y = pfVertex[Vector3::Y];
			if (vBoundingBox[0].z > pfVertex[Vector3::Z])
				vBoundingBox[0].z = pfVertex[Vector3::Z];
			if (vBoundingBox[1].z < pfVertex[Vector3::Z])
				vBoundingBox[1].z = pfVertex[Vector3::Z];
		}
	}
	if (m_pParent) { // Adjust bounding box
		// Get parent center position
		const Vector3 &vPCenter = static_cast<MeshOctree*>(m_pParent)->m_vBBCenter;
		const Vector3  vPBBMin  = static_cast<MeshOctree*>(m_pParent)->m_cBoundingBox.GetCorner1();
		const Vector3  vPBBMax  = static_cast<MeshOctree*>(m_pParent)->m_cBoundingBox.GetCorner2();

		// Check octree sector
		switch (m_nIDOffset) {
			// 0.  left     top		 back
			case 0:
				if (vBoundingBox[0].x <  vPBBMin.x)
					vBoundingBox[0].x =  vPBBMin.x;
				if (vBoundingBox[1].x > vPCenter.x)
					vBoundingBox[1].x = vPCenter.x;
				if (vBoundingBox[0].y < vPCenter.y)
					vBoundingBox[0].y = vPCenter.y;
				if (vBoundingBox[1].y >  vPBBMax.y)
					vBoundingBox[1].y =  vPBBMax.y;
				if (vBoundingBox[0].z <  vPBBMin.z)
					vBoundingBox[0].z =  vPBBMin.z;
				if (vBoundingBox[1].z > vPCenter.z)
					vBoundingBox[1].z = vPCenter.z;
				break;

			// 1.  right    top		 back
			case 1:
				if (vBoundingBox[0].x < vPCenter.x)
					vBoundingBox[0].x = vPCenter.x;
				if (vBoundingBox[1].x >  vPBBMax.x)
					vBoundingBox[1].x =  vPBBMax.x;
				if (vBoundingBox[0].y < vPCenter.y)
					vBoundingBox[0].y = vPCenter.y;
				if (vBoundingBox[1].y >  vPBBMax.y)
					vBoundingBox[1].y =  vPBBMax.y;
				if (vBoundingBox[0].z <  vPBBMin.z)
					vBoundingBox[0].z =  vPBBMin.z;
				if (vBoundingBox[1].z > vPCenter.z)
					vBoundingBox[1].z = vPCenter.z;
				break;

			// 2.  right    top		 front
			case 2:
				if (vBoundingBox[0].x < vPCenter.x)
					vBoundingBox[0].x = vPCenter.x;
				if (vBoundingBox[1].x >  vPBBMax.x)
					vBoundingBox[1].x =  vPBBMax.x;
				if (vBoundingBox[0].y < vPCenter.y)
					vBoundingBox[0].y = vPCenter.y;
				if (vBoundingBox[1].y >  vPBBMax.y)
					vBoundingBox[1].y =  vPBBMax.y;
				if (vBoundingBox[0].z < vPCenter.z)
					vBoundingBox[0].z = vPCenter.z;
				if (vBoundingBox[1].z >  vPBBMax.z)
					vBoundingBox[1].z =  vPBBMax.z;
				break;

			// 3.  left     top		 front
			case 3:
				if (vBoundingBox[0].x <  vPBBMin.x)
					vBoundingBox[0].x =  vPBBMin.x;
				if (vBoundingBox[1].x > vPCenter.x)
					vBoundingBox[1].x = vPCenter.x;
				if (vBoundingBox[0].y < vPCenter.y)
					vBoundingBox[0].y = vPCenter.y;
				if (vBoundingBox[1].y >  vPBBMax.y)
					vBoundingBox[1].y =  vPBBMax.y;
				if (vBoundingBox[0].z < vPCenter.z)
					vBoundingBox[0].z = vPCenter.z;
				if (vBoundingBox[1].z >  vPBBMax.z)
					vBoundingBox[1].z =  vPBBMax.z;
				break;

			// 4.  left     bottom	 back
			case 4:
				if (vBoundingBox[0].x <  vPBBMin.x)
					vBoundingBox[0].x =  vPBBMin.x;
				if (vBoundingBox[1].x > vPCenter.x)
					vBoundingBox[1].x = vPCenter.x;
				if (vBoundingBox[0].y <  vPBBMin.y)
					vBoundingBox[0].y =  vPBBMin.y;
				if (vBoundingBox[1].y > vPCenter.y)
					vBoundingBox[1].y = vPCenter.y;
				if (vBoundingBox[0].z <  vPBBMin.z)
					vBoundingBox[0].z =  vPBBMin.z;
				if (vBoundingBox[1].z > vPCenter.z)
					vBoundingBox[1].z = vPCenter.z;
				break;

			// 5.  right    bottom	 back
			case 5:
				if (vBoundingBox[0].x < vPCenter.x)
					vBoundingBox[0].x = vPCenter.x;
				if (vBoundingBox[1].x >  vPBBMax.x)
					vBoundingBox[1].x =  vPBBMax.x;
				if (vBoundingBox[0].y <  vPBBMin.y)
					vBoundingBox[0].y =  vPBBMin.y;
				if (vBoundingBox[1].y > vPCenter.y)
					vBoundingBox[1].y = vPCenter.y;
				if (vBoundingBox[0].z <  vPBBMin.z)
					vBoundingBox[0].z =  vPBBMin.z;
				if (vBoundingBox[1].z > vPCenter.z)
					vBoundingBox[1].z = vPCenter.z;
				break;

			// 6.  right    bottom	 front
			case 6:
				if (vBoundingBox[0].x < vPCenter.x)
					vBoundingBox[0].x = vPCenter.x;
				if (vBoundingBox[1].x >  vPBBMax.x)
					vBoundingBox[1].x =  vPBBMax.x;
				if (vBoundingBox[0].y <  vPBBMin.y)
					vBoundingBox[0].y =  vPBBMin.y;
				if (vBoundingBox[1].y > vPCenter.y)
					vBoundingBox[1].y = vPCenter.y;
				if (vBoundingBox[0].z < vPCenter.z)
					vBoundingBox[0].z = vPCenter.z;
				if (vBoundingBox[1].z >  vPBBMax.z)
					vBoundingBox[1].z =  vPBBMax.z;
				break;

			// 7.  left     bottom	 front
			case 7:
				if (vBoundingBox[0].x <  vPBBMin.x)
					vBoundingBox[0].x =  vPBBMin.x;
				if (vBoundingBox[1].x > vPCenter.x)
					vBoundingBox[1].x = vPCenter.x;
				if (vBoundingBox[0].y <  vPBBMin.y)
					vBoundingBox[0].y =  vPBBMin.y;
				if (vBoundingBox[1].y > vPCenter.y)
					vBoundingBox[1].y = vPCenter.y;
				if (vBoundingBox[0].z < vPCenter.z)
					vBoundingBox[0].z = vPCenter.z;
				if (vBoundingBox[1].z >  vPBBMax.z)
					vBoundingBox[1].z =  vPBBMax.z;
				break;
		}
	}

	// Get octree center
	m_vBBCenter		 = (vBoundingBox[0]+vBoundingBox[1])/2;
	vBoundingBox[0] -= m_vBBCenter;
	vBoundingBox[1] -= m_vBBCenter;

	// Set octree bounding box
	m_cBoundingBox.SetPos(m_vBBCenter.x, m_vBBCenter.y, m_vBBCenter.z);
	m_cBoundingBox.SetSize(vBoundingBox[0].x, vBoundingBox[0].y, vBoundingBox[0].z,
						   vBoundingBox[1].x, vBoundingBox[1].y, vBoundingBox[1].z);

	// [TODO] Clean this up
	// Check if the octree should be divided
	if (static_cast<signed>(m_nLevel) >= m_nSubdivide) {
//	if (m_nLevel >= m_nSubdivide || m_nNumOfGeometries < m_nMinGeometries) {
		// Unlock the buffers
		if (!m_pParent) {
			pIndexBuffer->Unlock();
			pVertexBuffer->Unlock();
		}

		// Done
		return true;
	}

	// Children
	uint32 nGeometriesT[8];
	uint32 *pGeometriesT[8];

	// Get the geometries in which are in the children
	for (int i=0; i<8; i++) {
		pGeometriesT[i] = new uint32[m_nNumOfGeometries];
		MemoryManager::Set(pGeometriesT[i], -1, sizeof(uint32)*m_nNumOfGeometries);
		nGeometriesT[i] = 0;
	}
	for (uint32 nGeometry=0; nGeometry<m_nNumOfGeometries; nGeometry++) {
		// Get data
		uint32			nGeoID	  = m_pnGeometries[nGeometry];
		const Geometry &cGeometry = cMeshLODLevel.GetGeometries()->Get(nGeoID);
		if (&cGeometry != &Array<Geometry>::Null) {
			// Loop through geometry vertices
			uint32 nIndex = cGeometry.GetStartIndex();
			for (uint32 i=0; i<cGeometry.GetIndexSize(); i++) {
				// Get vertex
				float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(pIndexBuffer->GetData(nIndex++), VertexBuffer::Position));

				// Check in which octree sector the vertex is in
				// 0.  left     top		 back
				if ((!nGeometriesT[0] || pGeometriesT[0][nGeometriesT[0]-1] != nGeoID) &&
					pfVertex[Vector3::X] <= m_vBBCenter.x && pfVertex[Vector3::Y] >= m_vBBCenter.y && pfVertex[Vector3::Z] <= m_vBBCenter.z) {
					pGeometriesT[0][nGeometriesT[0]] = nGeoID;
					nGeometriesT[0]++;
				}

				// 1.  right    top		 back
				if ((!nGeometriesT[1] || pGeometriesT[1][nGeometriesT[1]-1] != nGeoID) &&
					pfVertex[Vector3::X] >= m_vBBCenter.x && pfVertex[Vector3::Y] >= m_vBBCenter.y && pfVertex[Vector3::Z] <= m_vBBCenter.z) {
					pGeometriesT[1][nGeometriesT[1]] = nGeoID;
					nGeometriesT[1]++;
				}

				// 2.  right    top		 front
				if ((!nGeometriesT[2] || pGeometriesT[2][nGeometriesT[2]-1] != nGeoID) &&
					pfVertex[Vector3::X] >= m_vBBCenter.x && pfVertex[Vector3::Y] >= m_vBBCenter.y && pfVertex[Vector3::Z] >= m_vBBCenter.z) {
					pGeometriesT[2][nGeometriesT[2]] = nGeoID;
					nGeometriesT[2]++;
				}

				// 3.  left     top		 front
				if ((!nGeometriesT[3] || pGeometriesT[3][nGeometriesT[3]-1] != nGeoID) &&
					pfVertex[Vector3::X] <= m_vBBCenter.x && pfVertex[Vector3::Y] >= m_vBBCenter.y && pfVertex[Vector3::Z] >= m_vBBCenter.z) {
					pGeometriesT[3][nGeometriesT[3]] = nGeoID;
					nGeometriesT[3]++;
				}

				// 4.  left     bottom	 back
				if ((!nGeometriesT[4] || pGeometriesT[4][nGeometriesT[4]-1] != nGeoID) &&
					pfVertex[Vector3::X] <= m_vBBCenter.x && pfVertex[Vector3::Y] <= m_vBBCenter.y && pfVertex[Vector3::Z] <= m_vBBCenter.z) {
					pGeometriesT[4][nGeometriesT[4]] = nGeoID;
					nGeometriesT[4]++;
				}

				// 5.  right    bottom	 back
				if ((!nGeometriesT[5] || pGeometriesT[5][nGeometriesT[5]-1] != nGeoID) &&
					pfVertex[Vector3::X] >= m_vBBCenter.x && pfVertex[Vector3::Y] <= m_vBBCenter.y && pfVertex[Vector3::Z] <= m_vBBCenter.z) {
					pGeometriesT[5][nGeometriesT[5]] = nGeoID;
					nGeometriesT[5]++;
				}

				// 6.  right    bottom	 front
				if ((!nGeometriesT[6] || pGeometriesT[6][nGeometriesT[6]-1] != nGeoID) &&
					pfVertex[Vector3::X] >= m_vBBCenter.x && pfVertex[Vector3::Y] <= m_vBBCenter.y && pfVertex[Vector3::Z] >= m_vBBCenter.z) {
					pGeometriesT[6][nGeometriesT[6]] = nGeoID;
					nGeometriesT[6]++;
				}

				// 7.  left     bottom	 front
				if ((!nGeometriesT[7] || pGeometriesT[7][nGeometriesT[7]-1] != nGeoID) &&
					pfVertex[Vector3::X] <= m_vBBCenter.x && pfVertex[Vector3::Y] <= m_vBBCenter.y && pfVertex[Vector3::Z] >= m_vBBCenter.z) {
					pGeometriesT[7][nGeometriesT[7]] = nGeoID;
					nGeometriesT[7]++;
				}
			}
		}
	}

	// Get number of children
	m_nNumOfChildren = 0;
	for (int i=0; i<8; i++) {
		if (nGeometriesT[i])
//		if (nGeometriesT[i] >= m_nMinGeometries)
			m_nNumOfChildren++;
	}
	if (m_nNumOfChildren) { // Create children
		uint32 nChild = 0;

		m_ppChild = new Octree*[m_nNumOfChildren];
		for (int i=0; i<8; i++) {
			if (nGeometriesT[i]) {
//			if (nGeometriesT[i] >= m_nMinGeometries) {
				// Build child
				m_ppChild[nChild] = new MeshOctree();
				m_ppChild[nChild]->Init(this, m_nSubdivide, m_nMinGeometries, i);
				static_cast<MeshOctree*>(m_ppChild[nChild])->Build(cMeshLODLevel, nGeometriesT[i], pGeometriesT[i], plstOctreeIDList);
				nChild++;
			}
		}
	}

	// Delete temp child data
	for (int i=0; i<8; i++)
		delete [] pGeometriesT[i];

	// Unlock the buffers
	if (!m_pParent) {
		pIndexBuffer->Unlock();
		pVertexBuffer->Unlock();
	}

	// Done
	return true;
}

/**
*  @brief
*    Draws the octree
*/
void MeshOctree::Draw(const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fLineWidth) const
{
	// Check if we can/should draw the octree
	if (m_bBuild && m_bVisible) {
		// Draw octree bounding box
		Renderer *pRenderer = m_pOwnerMeshLODLevel->GetMesh()->GetRenderer();
		pRenderer->GetDrawHelpers().DrawBox(cColor, m_cBoundingBox.GetCorner1(), m_cBoundingBox.GetCorner2(), mWorldViewProjection, fLineWidth);

		// Draw children
		for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
			static_cast<MeshOctree*>(m_ppChild[nChild])->Draw(cColor, mWorldViewProjection, fLineWidth);
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLMath::Octree functions              ]
//[-------------------------------------------------------]
void MeshOctree::CustomVisible(Bitset *pBitset)
{
	// Check if the octree was build and if theres a bit set which can be filled
	if (m_bBuild && pBitset) {
		// Mark all geometries assigned to this octree as visible
		for (uint32 i=0; i<m_nNumOfGeometries; i++)
			pBitset->Set(m_pnGeometries[i]);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
