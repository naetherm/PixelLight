/*********************************************************\
 *  File: Mesh.cpp                                       *
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
#include <PLMath/BoundingBox.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLMesh/MeshManager.h"
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/MeshOctree.h"
#include "PLMesh/SkeletonManager.h"
#include "PLMesh/Mesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ MeshTriangle                                          ]
//[-------------------------------------------------------]
MeshTriangle &MeshTriangle::operator =(const MeshTriangle &cSource)
{
	// Copy data
	nVertex[0]	  = cSource.nVertex[0];
	nVertex[1]	  = cSource.nVertex[1];
	nVertex[2]	  = cSource.nVertex[2];
	nNeighbour[0] = cSource.nNeighbour[0];
	nNeighbour[1] = cSource.nNeighbour[1];
	nNeighbour[2] = cSource.nNeighbour[2];

	// Return this
	return *this;
}

bool MeshTriangle::operator ==(const MeshTriangle &cMeshTriangle) const
{
	return (nVertex[0]	  == cMeshTriangle.nVertex[0]	 &&
		    nVertex[1]	  == cMeshTriangle.nVertex[1]	 &&
		    nVertex[2]	  == cMeshTriangle.nVertex[2]	 &&
		    nNeighbour[0] == cMeshTriangle.nNeighbour[0] &&
		    nNeighbour[1] == cMeshTriangle.nNeighbour[1] &&
		    nNeighbour[2] == cMeshTriangle.nNeighbour[2]);
}


//[-------------------------------------------------------]
//[ MeshEdge                                              ]
//[-------------------------------------------------------]
MeshEdge &MeshEdge::operator =(const MeshEdge &cSource)
{
	// Copy data
	nVertex[0]	 = cSource.nVertex[0];
	nVertex[1]	 = cSource.nVertex[1];
	nTriangle[0] = cSource.nTriangle[0];
	nTriangle[1] = cSource.nTriangle[1];

	// Return this
	return *this;
}

bool MeshEdge::operator ==(const MeshEdge &cMeshEdge) const
{
	return (nVertex[0]	 == cMeshEdge.nVertex[0]   &&
			nVertex[1]	 == cMeshEdge.nVertex[1]   &&
			nTriangle[0] == cMeshEdge.nTriangle[0] &&
			nTriangle[1] == cMeshEdge.nTriangle[1]);
}


//[-------------------------------------------------------]
//[ Mesh                                                  ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Mesh::~Mesh()
{
	if (m_pSkeletonManager)
		delete m_pSkeletonManager;
}

/**
*  @brief
*    Returns the owner mesh manager
*/
MeshManager *Mesh::GetMeshManager() const
{
	return static_cast<MeshManager*>(GetManager());
}

/**
*  @brief
*    Returns the skeleton manager
*/
SkeletonManager &Mesh::GetSkeletonManager()
{
	if (!m_pSkeletonManager)
		m_pSkeletonManager = new SkeletonManager();
	return *m_pSkeletonManager;
}

/**
*  @brief
*    Returns the used renderer
*/
Renderer *Mesh::GetRenderer() const
{
	// Get pointer to renderer
	return m_pRenderer;
}

/**
*  @brief
*    Checks all LODs and returns the maximum number of vertices
*/
uint32 Mesh::GetMaxNumOfVertices() const
{
	// Because all morph targets have in fact the same number of vertices per LOD
	// we only have to check one morph target :)
	const MeshMorphTarget *pMorphTarget = GetMorphTarget();
	if (pMorphTarget) {
		// Return number of vertices
		const VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		if (pVertexBuffer)
			return pVertexBuffer->GetNumOfElements();
	}

	// There are no vertices...
	return 0;
}

/**
*  @brief
*    Checks all LODs and returns the maximum number of geometries
*/
uint32 Mesh::GetMaxNumOfGeometries() const
{
	uint32 nMaxNumOfGeometries = 0;

	// Loop through all LOD levels
	for (uint32 i=0; i<GetNumOfLODLevels(); i++) {
		const Array<Geometry> *plstGeometries = GetLODLevel(i)->GetGeometries();
		if (plstGeometries && nMaxNumOfGeometries < plstGeometries->GetNumOfElements())
			nMaxNumOfGeometries = plstGeometries->GetNumOfElements();
	}

	// Done
	return nMaxNumOfGeometries;
}

/**
*  @brief
*    Checks all LODs and returns the maximum number of triangles
*/
uint32 Mesh::GetMaxNumOfTriangles() const
{
	uint32 nMaxNumOfTriangles = 0;

	// Loop through all LOD levels
	for (uint32 i=0; i<GetNumOfLODLevels(); i++) {
		if (nMaxNumOfTriangles < GetLODLevel(i)->GetNumOfTriangles())
			nMaxNumOfTriangles = GetLODLevel(i)->GetNumOfTriangles();
	}

	// Done
	return nMaxNumOfTriangles;
}

/**
*  @brief
*    Draws the mesh
*/
void Mesh::Draw(const Matrix4x4 &mWorldViewProjection, bool bBlend, uint32 nFlags, uint32 nLODLevel, uint32 nMorphTarget, bool bUseMaterials) const
{
	// Get buffers
	const MeshMorphTarget *pMorphTarget = GetMorphTarget(nMorphTarget);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		const MeshLODLevel *pLODLevel = GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			VertexBuffer		  *pVertexBuffer = pMorphTarget->GetVertexBuffer();
			IndexBuffer			  *pIndexBuffer  = pLODLevel->GetIndexBuffer();
			const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();

			// Bind buffers
			GetRenderer()->SetIndexBuffer(pIndexBuffer);
			// [TODO] Remove FixedFunctions usage by using the new shader interface
			FixedFunctions *pFixedFunctions = GetRenderer()->GetFixedFunctions();
			if (pFixedFunctions)
				pFixedFunctions->SetVertexBuffer(pVertexBuffer);

			// Loop through all materials
			if (bUseMaterials) {
				int nBoundMaterial = -1;
				for (uint32 nMat=0; nMat<m_lstMaterials.GetNumOfElements(); nMat++) {
					// Check material
					Material *pMaterial = m_lstMaterials[nMat]->GetResource();
					if (pMaterial && pMaterial->GetBlend() == bBlend) {
						// Draw all passes
						int nBoundPass = -1;
						for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
							// Draw geometries
							for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
								// Get the geometry
								const Geometry &cGeometry = lstGeometries[nGeo];

								// Is the geometry using the current bound material and is this geometry active?
								if (nMat == cGeometry.GetMaterial() && cGeometry.IsActive()) {
									// Check if we have to bind this material the first time
									if (nBoundMaterial != static_cast<int>(nMat))
										nBoundMaterial = nMat;

									// Check if we have to bind this material pass the first time
									if (nBoundPass != static_cast<int>(nPass)) {
										nBoundPass = nPass;
										pMaterial->SetupPass(nPass);
									}

									// Draw geometry
									m_pRenderer->DrawIndexedPrimitives(
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
				}
			} else { // Bind no materials
				// Draw geometries
				for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
					const Geometry &cGeometry = lstGeometries[nGeo];

					// Is this geometry active?
					if (cGeometry.IsActive()) {
						// Draw geometry
						m_pRenderer->DrawIndexedPrimitives(
							cGeometry.GetPrimitiveType(),
							0,
							pVertexBuffer->GetNumOfElements()-1,
							cGeometry.GetStartIndex(),
							cGeometry.GetIndexSize()
						);
					}
				}
			}

			// Draw points
			if (nFlags & DrawVertices) {
				m_pRenderer->SetRenderState(RenderState::PointSize, Tools::FloatToUInt32(4.0f));
				pFixedFunctions = m_pRenderer->GetFixedFunctions();
				if (pFixedFunctions)
					pFixedFunctions->SetColor(Color4::White);
				m_pRenderer->SetTextureBuffer();
				m_pRenderer->DrawPrimitives(Primitive::PointList,
											0,
											pVertexBuffer->GetNumOfElements());
			}

			// Draw normals
			if (nFlags & DrawNormals) {
				m_pRenderer->SetRenderState(RenderState::PointSize, Tools::FloatToUInt32(4.0f));
				m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(2.0f));
				m_pRenderer->SetTextureBuffer();
				if (pVertexBuffer->Lock(Lock::ReadOnly)) {
					if (pVertexBuffer->GetData(0, VertexBuffer::Normal, 0)) {
						for (uint32 i=0; i<pVertexBuffer->GetNumOfElements(); i++) {
							const float *pP = static_cast<const float*>(pVertexBuffer->GetData(i, VertexBuffer::Position));
							const float *pN = static_cast<const float*>(pVertexBuffer->GetData(i, VertexBuffer::Normal));
							m_pRenderer->GetDrawHelpers().DrawLine(Color4::White,
																   Vector3(pP[Vector3::X], pP[Vector3::Y], pP[Vector3::Z]),
																   Vector3(pP[Vector3::X]+pN[Vector3::X], pP[Vector3::Y]+pN[Vector3::Y], pP[Vector3::Z]+pN[Vector3::Z]),
																   mWorldViewProjection, 1.0f);
						}
					}

					// Unlock the vertex buffer
					pVertexBuffer->Unlock();
				}
			}

			// Draw (optional) octree
			if (nFlags & DrawOctree && pLODLevel->GetOctree())
				pLODLevel->GetOctree()->Draw(Color4::White, mWorldViewProjection, 1.0f);
		}
	}
}

/**
*  @brief
*    Copy operator
*/
Mesh &Mesh::operator =(const Mesh &cSource)
{
	// Copy data
	m_cAnchorPointManager = cSource.m_cAnchorPointManager;

	// Copy morph targets
	ClearMorphTargets();
	for (uint32 i=0; i<cSource.GetNumOfMorphTargets(); i++) {
		MeshMorphTarget *pMorphTarget = new MeshMorphTarget(this);
		*pMorphTarget = *cSource.GetMorphTarget(i);
		m_lstMorphTargets.Add(pMorphTarget);
	}

	// Copy LOD levels
	ClearLODLevels();
	for (uint32 i=0; i<cSource.GetNumOfLODLevels(); i++) {
		MeshLODLevel *pLODLevel = new MeshLODLevel(this);
		*pLODLevel = *cSource.GetLODLevel(i);
		m_lstLODLevels.Add(pLODLevel);
	}

	// Copy materials
	ClearMaterials();
	for (uint32 i=0; i<cSource.GetNumOfMaterials(); i++)
		AddMaterial(cSource.GetMaterial(i));

	// Copy skeleton handlers
	ClearSkeletonHandlers();
	for (uint32 i=0; i<const_cast<Mesh&>(cSource).GetSkeletonHandlers().GetNumOfElements(); i++) {
		SkeletonHandler *pSkeletonHandler = new SkeletonHandler();
		pSkeletonHandler->SetResource(const_cast<Mesh&>(cSource).GetSkeletonHandlers()[i]->GetResource());
		m_lstSkeletonHandler.Add(pSkeletonHandler);
	}

	// Copy weights and vertex weights
	m_lstWeights	   = cSource.m_lstWeights;
	m_lstVertexWeights = cSource.m_lstVertexWeights;

	// Return pointer
	return *this;
}


//[-------------------------------------------------------]
//[ Mesh data                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the anchor point manager
*/
AnchorPointManager &Mesh::GetAnchorPointManager()
{
	return m_cAnchorPointManager;
}


//[-------------------------------------------------------]
//[ Morph target functions                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clears all morph targets
*/
void Mesh::ClearMorphTargets()
{
	// Delete all morph target
	for (uint32 i=0; i<m_lstMorphTargets.GetNumOfElements(); i++)
		delete m_lstMorphTargets[i];
	m_lstMorphTargets.Clear();
}

/**
*  @brief
*    Returns the number of morph targets
*/
uint32 Mesh::GetNumOfMorphTargets() const
{
	// Get number of morph targets
	return m_lstMorphTargets.GetNumOfElements();
}

/**
*  @brief
*    Adds a new morph target
*/
MeshMorphTarget *Mesh::AddMorphTarget()
{
	// Create new morph target
	MeshMorphTarget *pMorphTarget = new MeshMorphTarget(this);
	m_lstMorphTargets.Add(pMorphTarget);
	return pMorphTarget;
}

/**
*  @brief
*    Gets one of the mesh's morph targets by index
*/
MeshMorphTarget *Mesh::GetMorphTarget(uint32 nMorphTarget) const
{
	return m_lstMorphTargets[nMorphTarget];
}

/**
*  @brief
*    Gets one of the mesh's morph targets by name
*/
MeshMorphTarget *Mesh::GetMorphTarget(const String &sName) const
{
	return m_mapMorphTargets.Get(sName);
}

/**
*  @brief
*    Returns the index of a given morph target
*/
int Mesh::GetMorphTargetIndex(const String &sName) const
{
	MeshMorphTarget *pMorphTarget = m_mapMorphTargets.Get(sName);
	return pMorphTarget ? m_lstMorphTargets.GetIndex(pMorphTarget) : -1;
}

/**
*  @brief
*    Returns the morph target animation manager
*/
MorphTargetAniManager &Mesh::GetMorphTargetAnimationManager()
{
	return m_cMorphTargetAnimation;
}

/**
*  @brief
*    Adds a morph target animation
*/
bool Mesh::AddMorphTargetAnimation(const String &sFilename)
{
	// [TODO] Implement me
	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ LOD functions                                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clears all LOD levels
*/
void Mesh::ClearLODLevels()
{
	// Clear all LOD levels
	for (uint32 i=0; i<m_lstLODLevels.GetNumOfElements(); i++)
		delete m_lstLODLevels[i];
	m_lstLODLevels.Clear();
}

/**
*  @brief
*    Returns the number of LOD levels
*/
uint32 Mesh::GetNumOfLODLevels() const
{
	// Get number of LOD levels
	return m_lstLODLevels.GetNumOfElements();
}

/**
*  @brief
*    Adds a new LOD level
*/
MeshLODLevel *Mesh::AddLODLevel()
{
	// Add a new LOD level
	MeshLODLevel *pMeshLODLevel = new MeshLODLevel(this);
	m_lstLODLevels.Add(pMeshLODLevel);

	// Return new LOD level
	return pMeshLODLevel;
}

/**
*  @brief
*    Gets one of the mesh's LOD levels
*/
MeshLODLevel *Mesh::GetLODLevel(uint32 nLODLevel) const
{
	return m_lstLODLevels[nLODLevel];
}


//[-------------------------------------------------------]
//[ Material functions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clears all materials
*/
void Mesh::ClearMaterials()
{
	// Delete all materials
	for (uint32 i=0; i<m_lstMaterials.GetNumOfElements(); i++)
		delete m_lstMaterials[i];
	m_lstMaterials.Clear();
}

/**
*  @brief
*    Returns the number of materials
*/
uint32 Mesh::GetNumOfMaterials() const
{
	// Return number of materials
	return m_lstMaterials.GetNumOfElements();
}

/**
*  @brief
*    Adds a new material
*/
Material *Mesh::AddMaterial(Material *pMaterial)
{
	if (pMaterial) {
		// Check whether the material is already added
		for (uint32 i=0; i<m_lstMaterials.GetNumOfElements(); i++) {
			if (m_lstMaterials[i]->GetResource() == pMaterial)
				return pMaterial;
		}

		// Add the material
		MaterialHandler *pMH = new MaterialHandler();
		pMH->SetResource(pMaterial);
		m_lstMaterials.Add(pMH);

		// Return the added material
		return pMaterial;
	} else {
		return nullptr;
	}
}

/**
*  @brief
*    Deletes a material
*/
bool Mesh::DeleteMaterial(uint32 nMaterial)
{
	// Get material
	MaterialHandler *pMH = m_lstMaterials[nMaterial];
	if (!pMH)
		return false; // Error!

	// Has the material handler a resource?
	Material *pMaterial = pMH->GetResource();
	if (pMaterial) {
		// Update the material index of all geometries
		// Loop through all LOD levels
		for (uint32 nLODLevel=0; nLODLevel<GetNumOfLODLevels(); nLODLevel++) {
			const MeshLODLevel    *pLODLevel	  = GetLODLevel(nLODLevel);
			const Array<Geometry> *plstGeometries = pLODLevel->GetGeometries();

			// Loop through all geometries
			for (uint32 nGeo=0; nGeo<plstGeometries->GetNumOfElements(); nGeo++) {
				Geometry &cGeometry = plstGeometries->Get(nGeo);
				if (cGeometry.GetMaterial() == nMaterial)
					cGeometry.SetMaterial(0);
				if (cGeometry.GetMaterial() >  nMaterial)
					cGeometry.SetMaterial(cGeometry.GetMaterial()-1);
			}
		}
	}

	// Delete material
	delete pMH;
	m_lstMaterials.RemoveAtIndex(nMaterial);

	// Done
	return true;
}

/**
*  @brief
*    Returns the number of geometries using this material
*/
uint32 Mesh::GetMaterialUsage(uint32 nMaterial) const
{
	// Get material
	const MaterialHandler *pMH = m_lstMaterials[nMaterial];
	if (pMH) {
		// Has the material handler a resource?
		const Material *pMaterial = pMH->GetResource();
		if (pMaterial) {
			uint32 nUsage = 0;

			// Loop through all LOD levels
			for (uint32 nLODLevel=0; nLODLevel<GetNumOfLODLevels(); nLODLevel++) {
				const MeshLODLevel    *pLODLevel	  = GetLODLevel(nLODLevel);
				const Array<Geometry> *plstGeometries = pLODLevel->GetGeometries();

				// Loop through all geometries
				for (uint32 nGeo=0; nGeo<plstGeometries->GetNumOfElements(); nGeo++) {
					const Geometry &cGeometry = plstGeometries->Get(nGeo);
					if (cGeometry.GetMaterial() == nMaterial)
						nUsage++;
				}
			}

			// Return the number of geometries using this material
			return nUsage;
		}
	}

	// Done
	return 0;
}

/**
*  @brief
*    Gets one of the mesh's materials
*/
Material *Mesh::GetMaterial(uint32 nMaterial) const
{
	MaterialHandler *pMH = m_lstMaterials[nMaterial];
	return pMH ? pMH->GetResource() : nullptr;
}

/**
*  @brief
*    Sets one of the mesh's materials
*/
bool Mesh::SetMaterial(uint32 nMaterial, Material *pMaterial)
{
	// Set material
	MaterialHandler *pMH = m_lstMaterials[nMaterial];
	return pMH && pMH->SetResource(pMaterial);
}


//[-------------------------------------------------------]
//[ Skeleton functions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clears the list of skeleton handlers
*/
void Mesh::ClearSkeletonHandlers()
{
	for (uint32 i=0; i<m_lstSkeletonHandler.GetNumOfElements(); i++)
		delete m_lstSkeletonHandler[i];
	m_lstSkeletonHandler.Clear();
}

/**
*  @brief
*    Returns the list of skeleton handlers
*/
Array<SkeletonHandler*> &Mesh::GetSkeletonHandlers()
{
	return m_lstSkeletonHandler;
}

/**
*  @brief
*    Returns the weights array
*/
Array<Weight> &Mesh::GetWeights()
{
	return m_lstWeights;
}

/**
*  @brief
*    Returns the vertex weights array
*/
Array<VertexWeights> &Mesh::GetVertexWeights()
{
	return m_lstVertexWeights;
}


//[-------------------------------------------------------]
//[ Tool functions                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Builds the connectivity information of all LOD levels
*/
void Mesh::BuildConnectivity()
{
	for (uint32 i=0; i<GetNumOfLODLevels(); i++)
		GetLODLevel(i)->BuildConnectivity();
}

/**
*  @brief
*    Builds the current triangle planes of all morph targets
*/
void Mesh::BuildTrianglePlaneList()
{
	for (uint32 i=0; i<GetNumOfMorphTargets(); i++)
		GetMorphTarget(i)->BuildTrianglePlaneList();
}

/**
*  @brief
*    Builds the LOD levels automatically
*/
bool Mesh::BuildLOD(uint32 nNumLODLevels)
{
	// [TODO] Here's a nice algorithm to build the LOD levels automatically :-))
	return false; // Error!
}

/**
*  @brief
*    Calculate the vertex normals of all morph targets of the mesh
*/
bool Mesh::CalculateNormals()
{
	// [TEST] Only calculate normals for the first morph target
	// Loop through all morph targets
	for (uint32 nMorph=0; nMorph<GetNumOfMorphTargets();) {
//	for (uint32 nMorph=0; nMorph<GetNumOfMorphTargets(); nMorph++) {
		GetMorphTarget(nMorph)->CalculateNormals();
		break;
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates all tangent space vectors of all morph targets of the mesh
*/
bool Mesh::CalculateTangentSpaceVectors(bool bTangentChannel, bool bBinormalChannel)
{
	// Loop through all morph targets
	for (uint32 nMorph=0; nMorph<GetNumOfMorphTargets(); nMorph++)
		GetMorphTarget(nMorph)->CalculateTangentSpaceVectors(bTangentChannel, bBinormalChannel);

	// Done
	return true;
}

/**
*  @brief
*    Returns the mesh bounding box
*/
void Mesh::GetBoundingBox(Vector3 &vMin, Vector3 &vMax) const
{
	vMin = m_vBBMin;
	vMax = m_vBBMax;
}

/**
*  @brief
*    Set's the mesh bounding box
*/
void Mesh::SetBoundingBox(const Vector3 &vMin, const Vector3 &vMax)
{
	m_vBBMin = vMin;
	m_vBBMax = vMax;
}

/**
*  @brief
*    Calculates the mesh bounding box
*/
bool Mesh::CalculateBoundingBox(Vector3 &vMin, Vector3 &vMax)
{
	// Loop through all morph targets
	for (uint32 nMorph=0; nMorph<GetNumOfMorphTargets(); nMorph++) {
		// Get morph target bounding box
		MeshMorphTarget *pTarget = GetMorphTarget(nMorph);
		if (!pTarget->IsRelative()) {
			Vector3 vMinT, vMaxT;
			pTarget->CalculateBoundingBox(vMinT, vMaxT);

			// Check components
			if (nMorph) {
				for (int i=0; i<3; i++) {
					if (vMin[i] > vMinT[i])
						vMin[i] = vMinT[i];
					if (vMax[i] > vMaxT[i])
						vMax[i] = vMaxT[i];
				}
			} else {
				vMin = vMinT;
				vMax = vMaxT;
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates the mesh bounding sphere
*/
bool Mesh::CalculateBoundingSphere(Vector3 &vPos, float &fRadius)
{
	// Loop through all morph targets
	for (uint32 nMorph=0; nMorph<GetNumOfMorphTargets(); nMorph++) {
		// Get morph target bounding sphere
		if (nMorph) {
			float fRadiusT;
			Vector3 vPosT;

			GetMorphTarget(nMorph)->CalculateBoundingSphere(vPosT, fRadiusT);
			if (fRadiusT > fRadius)
				fRadius = fRadiusT;
			fRadius += (vPos-vPosT).GetLength()/2;
			vPos = (vPos+vPosT)/2;
		} else {
			GetMorphTarget(nMorph)->CalculateBoundingSphere(vPos, fRadius);
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Mesh::Mesh(Renderer *pRenderer, ResourceManager<Mesh> &cManager, const String &sName, bool bStatic) :
	PLCore::Resource<Mesh>(sName, &cManager),
	m_pRenderer(pRenderer),
	m_bStatic(bStatic),
	m_pSkeletonManager(nullptr)
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Mesh::Unload()
{
	// Clear the mesh
	m_cAnchorPointManager.Clear();
	ClearMaterials();
	ClearMorphTargets();
	ClearLODLevels();
	ClearSkeletonHandlers();
	if (m_pSkeletonManager) {
		delete m_pSkeletonManager;
		m_pSkeletonManager = nullptr;
	}

	// Call base implementation
	return PLCore::Resource<Mesh>::Unload();
}

String Mesh::GetLoadableTypeName() const
{
	static const String sString = "Mesh";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
