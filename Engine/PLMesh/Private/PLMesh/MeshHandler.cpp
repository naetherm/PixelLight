/*********************************************************\
 *  File: MeshHandler.cpp                                *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Base/Class.h>
#include <PLMath/Ray.h>
#include <PLMath/Intersect.h>
#include <PLMath/BoundingBox.h>
#include <PLMath/AABoundingBox.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Animation/Animation.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshAnimationManager.h"
#include "PLMesh/Geometry.h"
#include "PLMesh/AnchorPoint.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/MorphTargetAni.h"
#include "PLMesh/MeshOctree.h"
#include "PLMesh/SkeletonManager.h"
#include "PLMesh/MeshHandler.h"


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
MeshHandler::MeshHandler() :
	EventHandlerAnimationFrameChange(&MeshHandler::OnAnimationFrameChange, this),
	m_pRenderer(nullptr),
	m_pMesh(nullptr),
	m_pVertexBuffer(nullptr),
	m_bMeshUpdateRequired(false),
	m_nLOD(0),
	m_pCurrentVertexBuffer(nullptr),
	m_pSkeletonHandler(nullptr),
	m_pMeshAnimationManager(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
MeshHandler::~MeshHandler()
{
	// Clear data
	SetMesh();

	// Destroy mesh animation manager
	CreateMeshAnimationManager("");
}

/**
*  @brief
*    Returns the used renderer
*/
Renderer *MeshHandler::GetRenderer() const
{
	// Get pointer to renderer
	return m_pRenderer;
}

/**
*  @brief
*    Whenever you manipulated mesh data "from outside" you need to call this function to set a "dirty"-flag
*/
void MeshHandler::MeshUpdateRequired()
{
	m_bMeshUpdateRequired = true;
}

/**
*  @brief
*    Returns the used mesh
*/
Mesh *MeshHandler::GetMesh() const
{
	return m_pMesh;
}

/**
*  @brief
*    Sets the used mesh
*/
bool MeshHandler::SetMesh(Mesh *pMesh)
{
	// Setup mesh
	SetResource(pMesh);
	m_pMesh				   = pMesh;
	m_pCurrentVertexBuffer = nullptr;
	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
		m_pVertexBuffer = nullptr;
	}
	if (pMesh) {
		m_cGeometryVisibility.Resize(m_pMesh->GetMaxNumOfGeometries(), true, true);
		m_lstBaseMorphTargetWeights.Resize(pMesh->GetNumOfMorphTargets(), true, true);
		m_lstMorphTargetWeights.Resize(pMesh->GetNumOfMorphTargets(), true, true);
	} else {
		m_cGeometryVisibility.Clear();
		m_lstBaseMorphTargetWeights.Clear();
		m_lstMorphTargetWeights.Clear();
	}
	m_cGeometryVisibility.SetAll();
	m_lstTrianglePlanes.Clear();
	m_bRecalculateTrianglePlanes = true;

	// Setup skeleton handler
	if (pMesh && pMesh->GetSkeletonHandlers().GetNumOfElements()) {
		if (!m_pSkeletonHandler)
			m_pSkeletonHandler = new SkeletonHandler();
		m_pSkeletonHandler->SetResource(pMesh->GetSkeletonHandlers()[0]->GetResource());
	} else {
		if (m_pSkeletonHandler) {
			delete m_pSkeletonHandler;
			m_pSkeletonHandler = nullptr;
		}
	}

	// Setup materials
	ClearMaterials();
	if (m_pMesh) {
		for (uint32 i=0; i<m_pMesh->GetNumOfMaterials(); i++)
			AddMaterial(m_pMesh->GetMaterial(i));

		// Set renderer
		m_pRenderer = m_pMesh->GetRenderer();
	} else {
		// Set renderer
		m_pRenderer = nullptr;
	}

	// Reset mesh animation manager
	if (m_pMeshAnimationManager)
		m_pMeshAnimationManager->Clear();

	// Update a first time
	Update(0.0f);

	// Done
	return true;
}

/**
*  @brief
*    Updates the mesh handler
*/
void MeshHandler::Update(float fTimeDifference, uint32 nLODLevel)
{
	// Is there a mesh?
	if (m_pMesh && m_pRenderer) {
		// Update mesh animation manager
		if (m_pMeshAnimationManager)
			m_pMeshAnimationManager->Update(fTimeDifference);

		// Backup current used LOD level
		m_nLOD = nLODLevel;

		// Indicates whether this mesh handler needs an own vertex buffer to hold the current mesh
		bool bOwnVertexBuffer = false;

		// Mesh update required?
		if (m_bMeshUpdateRequired) {
			// Use own vertex buffer?
			bOwnVertexBuffer = (m_pMeshAnimationManager != nullptr);
		}

		if (bOwnVertexBuffer) {
			// Create own vertex buffer
			if (!m_pVertexBuffer) {
				// Find the LOD level vertex buffer with the highest number of vertices
				const MeshMorphTarget *pMorphTarget = m_pMesh->GetMorphTarget();
				VertexBuffer *pVertexBuffer = nullptr;
				if (pMorphTarget)
					pVertexBuffer = pMorphTarget->GetVertexBuffer();

				// Create mesh handler own vertex buffer
				if (pVertexBuffer) {
					m_pVertexBuffer  = m_pRenderer->CreateVertexBuffer();
					*m_pVertexBuffer = *pVertexBuffer;
				}
			}
		}

		if (m_pVertexBuffer) {
			// Set mesh handler own vertex buffer as the currently used
			m_pCurrentVertexBuffer = m_pVertexBuffer;

			// Mesh update required?
			if (m_bMeshUpdateRequired) {
				// Apply mesh animation manager
				if (m_pMeshAnimationManager)
					static_cast<MeshAnimationManager*>(m_pMeshAnimationManager)->Apply(*this);

				// Rebuild of current triangle plane list required
				m_bRecalculateTrianglePlanes = true;
			}
		} else {
			const MeshMorphTarget *pMorphTarget = m_pMesh->GetMorphTarget(0);
			if (pMorphTarget)
				m_pCurrentVertexBuffer = pMorphTarget->GetVertexBuffer();
		}

		// Update done
		m_bMeshUpdateRequired = false;
	}
}


//[-------------------------------------------------------]
//[ Draw functions                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the mesh
*/
void MeshHandler::Draw(bool bBlend, bool bUseMaterials) const
{
	// Check pointers
	if (m_pMesh && m_pCurrentVertexBuffer) {
		// Get buffers
		const MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(m_nLOD);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			IndexBuffer			  *pIndexBuffer  = pLODLevel->GetIndexBuffer();
			const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();

			// Bind buffers
			GetRenderer()->SetIndexBuffer(pIndexBuffer);
			// [TODO] Remove FixedFunctions usage by using the new shader interface
			FixedFunctions *pFixedFunctions = GetRenderer()->GetFixedFunctions();
			if (pFixedFunctions)
				pFixedFunctions->SetVertexBuffer(m_pCurrentVertexBuffer);

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
								if (nMat == cGeometry.GetMaterial() && cGeometry.IsActive() && m_cGeometryVisibility.IsSet(nGeo)) {
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
										m_pCurrentVertexBuffer->GetNumOfElements()-1,
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
							m_pCurrentVertexBuffer->GetNumOfElements()-1,
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
*    Draws the vertices of the mesh (for debugging)
*/
void MeshHandler::DrawVertices(const Color4 &cColor) const
{
	if (m_pCurrentVertexBuffer) {
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// [TODO] Remove FixedFunctions usage by using the new shader interface
			pFixedFunctions->SetVertexBuffer(m_pCurrentVertexBuffer);
			pFixedFunctions->SetColor(cColor);
		}
		m_pRenderer->DrawPrimitives(Primitive::PointList, 0, m_pCurrentVertexBuffer->GetNumOfElements());
	}
}

/**
*  @brief
*    Draws the normals of the mesh (for debugging)
*/
void MeshHandler::DrawNormals(const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fScale, float fLineWidth) const
{
	// Lock the current vertex buffer
	if (m_pCurrentVertexBuffer && m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
		// Are there vertex normals?
		if (m_pCurrentVertexBuffer->GetData(0, VertexBuffer::Normal, 0)) {
			DrawHelpers &cDrawHelpers = m_pRenderer->GetDrawHelpers();
			for (uint32 i=0; i<m_pCurrentVertexBuffer->GetNumOfElements(); i++) {
				const float *pfP = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Position));
				const float *pfN = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Normal));
				cDrawHelpers.DrawLine(cColor,
									  Vector3(pfP[Vector3::X], pfP[Vector3::Y], pfP[Vector3::Z]),
									  Vector3(pfP[Vector3::X] + pfN[Vector3::X]*fScale, pfP[Vector3::Y] + pfN[Vector3::Y]*fScale, pfP[Vector3::Z] + pfN[Vector3::Z]*fScale),
									  mWorldViewProjection, fLineWidth);
			}
		}
		m_pCurrentVertexBuffer->Unlock();
	}
}

/**
*  @brief
*    Draws the tangents of the mesh (for debugging)
*/
void MeshHandler::DrawTangents(const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fScale, float fLineWidth) const
{
	// Lock the current vertex buffer
	if (m_pCurrentVertexBuffer && m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
		// Are there vertex tangents?
		if (m_pCurrentVertexBuffer->GetData(0, VertexBuffer::Tangent, 0)) {
			DrawHelpers &cDrawHelpers = m_pRenderer->GetDrawHelpers();
			for (uint32 i=0; i<m_pCurrentVertexBuffer->GetNumOfElements(); i++) {
				const float *pfP = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Position));
				const float *pfT = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Tangent));
				cDrawHelpers.DrawLine(cColor,
									  Vector3(pfP[Vector3::X], pfP[Vector3::Y], pfP[Vector3::Z]),
									  Vector3(pfP[Vector3::X] + pfT[Vector3::X]*fScale, pfP[Vector3::Y] + pfT[Vector3::Y]*fScale, pfP[Vector3::Z] + pfT[Vector3::Z]*fScale),
									  mWorldViewProjection, fLineWidth);
			}
		}
		m_pCurrentVertexBuffer->Unlock();
	}
}

/**
*  @brief
*    Draws the binormals of the mesh (for debugging)
*/
void MeshHandler::DrawBinormals(const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fScale, float fLineWidth) const
{
	// Lock the current vertex buffer
	if (m_pCurrentVertexBuffer && m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
		// Are there vertex binormals?
		if (m_pCurrentVertexBuffer->GetData(0, VertexBuffer::Binormal, 0)) {
			DrawHelpers &cDrawHelpers = m_pRenderer->GetDrawHelpers();
			for (uint32 i=0; i<m_pCurrentVertexBuffer->GetNumOfElements(); i++) {
				const float *pfP = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Position));
				const float *pfB = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Binormal));
				cDrawHelpers.DrawLine(cColor,
									  Vector3(pfP[Vector3::X], pfP[Vector3::Y], pfP[Vector3::Z]),
									  Vector3(pfP[Vector3::X] + pfB[Vector3::X]*fScale, pfP[Vector3::Y] + pfB[Vector3::Y]*fScale, pfP[Vector3::Z] + pfB[Vector3::Z]*fScale),
									  mWorldViewProjection, fLineWidth);
			}
		}
		m_pCurrentVertexBuffer->Unlock();
	}
}

/**
*  @brief
*    Draws the vertex numbers of the mesh (for debugging)
*/
void MeshHandler::DrawVertexNumbers(Font &cFont, const Color4 &cColor, const Matrix4x4 &mWorldViewProjection) const
{
	if (m_pCurrentVertexBuffer && m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
		Vector3 vV;

		// Loop through all vertices
		DrawHelpers &cDrawHelpers = m_pRenderer->GetDrawHelpers();
		for (uint32 i=0; i<m_pCurrentVertexBuffer->GetNumOfElements(); i++) {
			const float *pfVertex = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(i, VertexBuffer::Position));
			vV.x = pfVertex[Vector3::X];
			vV.y = pfVertex[Vector3::Y];
			vV.z = pfVertex[Vector3::Z];
			cDrawHelpers.DrawText(cFont, i, cColor, vV, mWorldViewProjection, Font::CenterText);

		}

		// Unlock the vertex buffer
		m_pCurrentVertexBuffer->Unlock();
	}
}

/**
*  @brief
*    Draws the anchor points of the mesh (for debugging)
*/
void MeshHandler::DrawAnchorPoints(Font &cFont, const Color4 &cColor, const Matrix4x4 &mWorldViewProjection) const
{
	// Draw anchor points
	if (m_pCurrentVertexBuffer && m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
		Vector3 vPos;
		DrawHelpers &cDrawHelpers = m_pRenderer->GetDrawHelpers();
		for (uint32 i=0; i<m_pMesh->GetAnchorPointManager().GetNumOfElements(); i++) {
			const AnchorPoint *pAnchorPoint = m_pMesh->GetAnchorPointManager().GetByIndex(i);
			if (pAnchorPoint) {
				if (!pAnchorPoint->GetType()) { // Vertex
					const float *pfVertex = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(pAnchorPoint->GetID(), VertexBuffer::Position));
					if (pfVertex) {
						vPos.x = pfVertex[Vector3::X];
						vPos.y = pfVertex[Vector3::Y];
						vPos.z = pfVertex[Vector3::Z];
					}
				} else { // Bone
					// [TODO]
				}
				m_pRenderer->SetTextureBuffer();
				cDrawHelpers.DrawPoint(cColor, vPos, mWorldViewProjection, 6.0f);
				cDrawHelpers.DrawText(cFont, pAnchorPoint->GetName(), cColor, vPos, mWorldViewProjection, Font::CenterText);
				// [TODO] Show anchor point direction if bind to bone...
			}
		}

		// Unlock the vertex buffer
		m_pCurrentVertexBuffer->Unlock();
	}
}

//[-------------------------------------------------------]
//[ Material functions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of materials
*/
uint32 MeshHandler::GetNumOfMaterials() const
{
	// Return the number of materials
	return m_lstMaterials.GetNumOfElements();
}

/**
*  @brief
*    Gets one of the mesh handler's materials
*/
Material *MeshHandler::GetMaterial(uint32 nMaterial) const
{
	MaterialHandler *pMH = m_lstMaterials[nMaterial];
	return pMH ? pMH->GetResource() : nullptr;
}

/**
*  @brief
*    Sets one of the mesh handler's materials
*/
bool MeshHandler::SetMaterial(uint32 nMaterial, Material *pMaterial)
{
	// Check material
	if (pMaterial) {
		// Set material
		MaterialHandler *pMH = m_lstMaterials[nMaterial];
		if (pMH)
			return pMH->SetResource(pMaterial);
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Morph target functions                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns an array holding the the base weight of each morph target
*/
Array<float> &MeshHandler::GetBaseMorphTargetWeights()
{
	// The data 'may' get changed and we 'may' need to update the mesh
	m_bMeshUpdateRequired = true;

	// Return a reference to the internal array
	return m_lstBaseMorphTargetWeights;
}

const Array<float> &MeshHandler::GetBaseMorphTargetWeights() const
{
	return m_lstBaseMorphTargetWeights;
}

/**
*  @brief
*    Returns an array holding the the current weight of each morph target
*/
Array<float> &MeshHandler::GetMorphTargetWeights()
{
	// The data 'may' get changed and we 'may' need to update the mesh
	m_bMeshUpdateRequired = true;

	// Return a reference to the internal array
	return m_lstMorphTargetWeights;
}

const Array<float> &MeshHandler::GetMorphTargetWeights() const
{
	return m_lstMorphTargetWeights;
}


//[-------------------------------------------------------]
//[ Animation functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the skeleton handler
*/
SkeletonHandler *MeshHandler::GetSkeletonHandler() const
{
	return m_pSkeletonHandler;
}

/**
*  @brief
*    Returns the mesh animation manager
*/
MeshAnimationManager *MeshHandler::GetMeshAnimationManager() const
{
	return m_pMeshAnimationManager;
}

/**
*  @brief
*    Creates the mesh animation manager
*/
MeshAnimationManager *MeshHandler::CreateMeshAnimationManager(const String &sName)
{
	// Destroy mesh animation manager
	if (m_pMeshAnimationManager) {
		delete m_pMeshAnimationManager;
		m_pMeshAnimationManager = nullptr;
	}

	// To keep things as fast as possible we store a pointer to the base class
	// (no hash map operation + quite efficient internal string comparison)
	static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLMesh::MeshAnimationManager");

	// Create new manager
	if (pBaseClass) {
		const Class *pClass = ClassManager::GetInstance()->GetClass(sName);
		if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
			Object *pObject = pClass->Create();
			if (pObject) {
				m_pMeshAnimationManager = static_cast<MeshAnimationManager*>(pObject);

				// Connect event handler
				m_pMeshAnimationManager->EventAnimationFrameChange.Connect(EventHandlerAnimationFrameChange);

				// Return the new mesh animation manager
				return m_pMeshAnimationManager;
			}
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns a list of all currently available animations
*/
void MeshHandler::GetAnimationsList(Array<String> &lstAnimations) const
{
	// Get the used mesh
	Mesh *pMesh = GetMesh();
	if (pMesh) {
		// First clear list
		lstAnimations.Clear();

		// Add all skeleton animations
		const SkeletonManager &cSkeletonManager = pMesh->GetSkeletonManager();
		for (uint32 i=0; i<cSkeletonManager.GetNumOfElements(); i++)
			lstAnimations.Add(cSkeletonManager.GetByIndex(i)->GetName());

		// Add all morph target animations
		if (m_pMesh) {
			MorphTargetAniManager &cMTManager = m_pMesh->GetMorphTargetAnimationManager();
			for (uint32 i=0; i<cMTManager.GetNumOfElements(); i++)
				lstAnimations.Add(cMTManager.GetByIndex(i)->GetName());
		}
	}
}

/**
*  @brief
*    Returns a pointer to the requested animation information
*/
AnimationInfo *MeshHandler::GetAnimationInfo(const String &sName, int nLogMessage) const
{
	// Get the used mesh
	Mesh *pMesh = GetMesh();
	if (pMesh) {
		// Check parameter
		if (sName.GetLength()) {
			// First, check the skeleton manager
			AnimationInfo *pInfo = pMesh->GetSkeletonManager().GetByName(sName);
			if (pInfo)
				return pInfo;

			// Check the morph target animations from the mesh
			if (m_pMesh) {
				pInfo = static_cast<MorphTargetAni*>(m_pMesh->GetMorphTargetAnimationManager().GetByName(sName));
				if (pInfo)
					return pInfo;
			}
		}

		// Animation information not found!
		if (nLogMessage >= 0)
			PL_LOG(Error, "Can't find animation: " + sName)
	}

	// Done
	return nullptr;
}

/**
*  @brief
*    Returns a current joint handler
*/
JointHandler *MeshHandler::GetJointHandler(const String &sJointName) const
{
	// Get the skeleton handler and skeleton
	const SkeletonHandler *pSkeletonHandler = GetSkeletonHandler();
	if (pSkeletonHandler) {
		const Skeleton *pSkeleton = pSkeletonHandler->GetResource();
		if (pSkeleton) {
			// Get the requested joint of the skeleton
			const Joint *pJoint = pSkeleton->GetByName(sJointName);
			if (pJoint) {
				// Get the base joint handler and return it
				JointHandler &cJointHandler = pSkeletonHandler->GetJointHandlers()[pJoint->GetID()];
				return (&cJointHandler != &Array<JointHandler>::Null) ? &cJointHandler : nullptr;
			}
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns a base joint handler
*/
JointHandler *MeshHandler::GetBaseJointHandler(const String &sJointName) const
{
	// Get the skeleton handler and skeleton
	SkeletonHandler *pSkeletonHandler = GetSkeletonHandler();
	if (pSkeletonHandler) {
		const Skeleton *pSkeleton = pSkeletonHandler->GetResource();
		if (pSkeleton) {
			// Get the requested joint of the skeleton
			const Joint *pJoint = pSkeleton->GetByName(sJointName);
			if (pJoint) {
				// Get the base joint handler and return it
				JointHandler &cJointHandler = pSkeletonHandler->GetBaseJointHandlers()[pJoint->GetID()];
				return (&cJointHandler != &Array<JointHandler>::Null) ? &cJointHandler : nullptr;
			}
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Visibility functions                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the mesh visibility
*/
void MeshHandler::UpdateVisibility(const PlaneSet &cPlaneSet, bool bFirst)
{
	// Check if theres a mesh
	if (m_pMesh) {
		// Check if there's an octree
		MeshOctree *pOctree = m_pMesh->GetLODLevel(m_nLOD)->GetOctree();
		if (pOctree && pOctree->IsBuild()) {
			// First, all geometries are invisible
			if (bFirst)
				m_cGeometryVisibility.ClearAll();

			// Update geometry visibility
			pOctree->UpdateVisibility(cPlaneSet, &m_cGeometryVisibility);
		} else {
			// Set all geometries to visible
			m_cGeometryVisibility.SetAll();
		}
	}
}

/**
*  @brief
*    Returns the bit set holding the geometry visibility state
*/
Bitset &MeshHandler::GetGeometryVisibility()
{
	return m_cGeometryVisibility;
}


//[-------------------------------------------------------]
//[ LOD functions                                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of mesh LOD levels
*/
uint32 MeshHandler::GetNumOfLODLevels() const
{
	return m_pMesh ? m_pMesh->GetNumOfLODLevels() : 0;
}

/**
*  @brief
*    Returns the current used mesh LOD level index
*/
uint32 MeshHandler::GetLODLevelIndex() const
{
	return m_nLOD;
}

/**
*  @brief
*    Returns the current used mesh LOD level
*/
MeshLODLevel *MeshHandler::GetLODLevel() const
{
	return m_pMesh ? m_pMesh->GetLODLevel(m_nLOD) : nullptr;
}


//[-------------------------------------------------------]
//[ Tool functions                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Calculates the current axis aligned joint bounding box
*/
bool MeshHandler::CalculateJointBoundingBox(AABoundingBox &cAAB) const
{
	// [TODO] Do also take joint bounding boxes (at least optionally) into account
	// Check if there's a skeleton handler
	if (m_pSkeletonHandler) {
		// Get joint handlers
		const Array<JointHandler> &lstJointHandlers = m_pSkeletonHandler->GetJointHandlers();
		if (lstJointHandlers.GetNumOfElements()) {
			// Data
			Vector3 &vMin = cAAB.vMin;
			Vector3 &vMax = cAAB.vMax;

			// Init bounding box
			const JointHandler *pJointHandler = &lstJointHandlers[0];
			vMin = vMax = pJointHandler->GetTranslationAbsolute();

			// Get min/max
			for (uint32 nJoint=1; nJoint<lstJointHandlers.GetNumOfElements(); nJoint++) {
				pJointHandler = &lstJointHandlers[nJoint];
				const Vector3 &vV = pJointHandler->GetTranslationAbsolute();
				for (int i=0; i<3; i++) {
					if (vMin[i] > vV[i])
						vMin[i] = vV[i];
					if (vMax[i] < vV[i])
						vMax[i] = vV[i];
				}
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the current used vertex buffer of the mesh
*/
VertexBuffer *MeshHandler::GetVertexBuffer() const
{
	return m_pCurrentVertexBuffer;
}

/**
*  @brief
*    Returns the total number of current triangles
*/
uint32 MeshHandler::GetNumOfTriangles() const
{
	const MeshLODLevel *pLODLevel = GetLODLevel();
	return pLODLevel ? pLODLevel->GetNumOfTriangles() : 0;
}

/**
*  @brief
*    Returns the first mesh triangle intersecting the line
*/
bool MeshHandler::FindTriangle(const Vector3 &vLineStartPos,
							   const Vector3 &vLineEndPos,
							   uint32 &nTriangle, uint32 *pnGeometry,
							   Vector3 *pvCollisionPoint,
							   Array<uint32> *plstGeometries,
							   Cull::Enum nCull) const
{
	// Check pointers
	if (m_pMesh && m_pCurrentVertexBuffer) {
		const MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(m_nLOD);
		if (pLODLevel) {
			// Lock buffers
			if (m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
				// Get a pointer to the first vertex position
				const char *pFirstVertex = static_cast<const char*>(m_pCurrentVertexBuffer->GetData(0, VertexBuffer::Position));
				if (pFirstVertex) {
					const uint32 nVertexSize = m_pCurrentVertexBuffer->GetVertexSize();

					// Get index buffer
					IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
					if (pIndexBuffer && pIndexBuffer->Lock(Lock::ReadOnly)) {
						// Calculate line direction
						Vector3 vDirection = vLineEndPos-vLineStartPos;

						// Get normalized line direction (required for culling)
						Vector3 vNormalizedDirection = vDirection;
						vNormalizedDirection.Normalize();

						// Variables
						Vector3 vIntersectionPointPos, vCollisionPoint, vV1, vV2, vV3, vN;
						float fSquaredDistance = -1.0f; // Not initialized yet

						// Loop through all geometries
						const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
						if (plstGeometries) {
							for (uint32 nGeo=0; nGeo<plstGeometries->GetNumOfElements(); nGeo++) {
								// Get and check the geometry index
								const uint32 nGeometryIndex = (*plstGeometries)[nGeo];
								if (nGeometryIndex < lstGeometries.GetNumOfElements()) {
									// Get the geometry
									const Geometry &cGeometry = lstGeometries[nGeometryIndex];

									// Is this geometry active?
									if (cGeometry.IsActive()) {
										const uint32 nNumOfTriangles = cGeometry.GetNumOfTriangles();

										// Optimized for 'triangle list' ('MeshHandler::FindTriangle()' can become a heavy bottleneck!)
										if (cGeometry.GetPrimitiveType() == Primitive::TriangleList) {
											if (pIndexBuffer->GetElementType() == IndexBuffer::UInt) {
												// Get a pointer to the first index of the geometry
												const uint32 *pnIndex = static_cast<const uint32*>(pIndexBuffer->GetData()) + cGeometry.GetStartIndex();

												// Loop through all triangles
												for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
													// Get triangle data
													const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV1.x = pfVertex[Vector3::X];
													vV1.y = pfVertex[Vector3::Y];
													vV1.z = pfVertex[Vector3::Z];
													pnIndex++;
													pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV2.x = pfVertex[Vector3::X];
													vV2.y = pfVertex[Vector3::Y];
													vV2.z = pfVertex[Vector3::Z];
													pnIndex++;
													pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV3.x = pfVertex[Vector3::X];
													vV3.y = pfVertex[Vector3::Y];
													vV3.z = pfVertex[Vector3::Z];
													pnIndex++;

													// Get face normal
													vN.GetFaceNormal(vV1, vV2, vV3);

													// Perform culling?
													if (nCull != Cull::None) {
														// Calculate the dot product between the two normalized vectors
														const float fDotProduct = vN.DotProduct(vNormalizedDirection);

														// Perform culling
														if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
															continue;	// Use a simple "continue", not additional conditions
													}

													// Check triangle intersection
													if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
														// Hit!
														// Get squared distance
														float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

														// Check squared distance
														if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
															fSquaredDistance = fCurrentSquaredDistance;
															nTriangle = nTri;
															if (pnGeometry)
																*pnGeometry = nGeometryIndex;
															if (pvCollisionPoint)
																*pvCollisionPoint = vIntersectionPointPos;
														}
													}
												}
											} else if (pIndexBuffer->GetElementType() == IndexBuffer::UShort) {
												// Get a pointer to the first index of the geometry
												const uint16 *pnIndex = static_cast<const uint16*>(pIndexBuffer->GetData()) + cGeometry.GetStartIndex();

												// Loop through all triangles
												for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
													// Get triangle data
													const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV1.x = pfVertex[Vector3::X];
													vV1.y = pfVertex[Vector3::Y];
													vV1.z = pfVertex[Vector3::Z];
													pnIndex++;
													pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV2.x = pfVertex[Vector3::X];
													vV2.y = pfVertex[Vector3::Y];
													vV2.z = pfVertex[Vector3::Z];
													pnIndex++;
													pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV3.x = pfVertex[Vector3::X];
													vV3.y = pfVertex[Vector3::Y];
													vV3.z = pfVertex[Vector3::Z];
													pnIndex++;

													// Get face normal
													vN.GetFaceNormal(vV1, vV2, vV3);

													// Perform culling?
													if (nCull != Cull::None) {
														// Calculate the dot product between the two normalized vectors
														const float fDotProduct = vN.DotProduct(vNormalizedDirection);

														// Perform culling
														if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
															continue;	// Use a simple "continue", not additional conditions
													}

													// Check triangle intersection
													if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
														// Hit!
														// Get squared distance
														float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

														// Check squared distance
														if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
															fSquaredDistance = fCurrentSquaredDistance;
															nTriangle = nTri;
															if (pnGeometry)
																*pnGeometry = nGeometryIndex;
															if (pvCollisionPoint)
																*pvCollisionPoint = vIntersectionPointPos;
														}
													}
												}
											} else {
												// Get a pointer to the first index of the geometry
												const uint8 *pnIndex = static_cast<const uint8*>(pIndexBuffer->GetData()) + cGeometry.GetStartIndex();

												// Loop through all triangles
												for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
													// Get triangle data
													const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV1.x = pfVertex[Vector3::X];
													vV1.y = pfVertex[Vector3::Y];
													vV1.z = pfVertex[Vector3::Z];
													pnIndex++;
													pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV2.x = pfVertex[Vector3::X];
													vV2.y = pfVertex[Vector3::Y];
													vV2.z = pfVertex[Vector3::Z];
													pnIndex++;
													pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
													vV3.x = pfVertex[Vector3::X];
													vV3.y = pfVertex[Vector3::Y];
													vV3.z = pfVertex[Vector3::Z];
													pnIndex++;

													// Get face normal
													vN.GetFaceNormal(vV1, vV2, vV3);

													// Perform culling?
													if (nCull != Cull::None) {
														// Calculate the dot product between the two normalized vectors
														const float fDotProduct = vN.DotProduct(vNormalizedDirection);

														// Perform culling
														if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
															continue;	// Use a simple "continue", not additional conditions
													}

													// Check triangle intersection
													if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
														// Hit!
														// Get squared distance
														float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

														// Check squared distance
														if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
															fSquaredDistance = fCurrentSquaredDistance;
															nTriangle = nTri;
															if (pnGeometry)
																*pnGeometry = nGeometryIndex;
															if (pvCollisionPoint)
																*pvCollisionPoint = vIntersectionPointPos;
														}
													}
												}
											}
										} else {
											// Loop through all triangles
											for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
												// Get triangle data
												uint32 nVertex1, nVertex2, nVertex3;
												pLODLevel->GetTriangle(nGeometryIndex, nTri, nVertex1, nVertex2, nVertex3);
												const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*nVertex1);
												vV1.x = pfVertex[Vector3::X];
												vV1.y = pfVertex[Vector3::Y];
												vV1.z = pfVertex[Vector3::Z];
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*nVertex2);
												vV2.x = pfVertex[Vector3::X];
												vV2.y = pfVertex[Vector3::Y];
												vV2.z = pfVertex[Vector3::Z];
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*nVertex3);
												vV3.x = pfVertex[Vector3::X];
												vV3.y = pfVertex[Vector3::Y];
												vV3.z = pfVertex[Vector3::Z];

												// Get face normal
												vN.GetFaceNormal(vV1, vV2, vV3);

												// Perform culling?
												if (nCull != Cull::None) {
													// Calculate the dot product between the two normalized vectors
													const float fDotProduct = vN.DotProduct(vNormalizedDirection);

													// Perform culling
													if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
														continue;	// Use a simple "continue", not additional conditions
												}

												// Check triangle intersection
												if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
													// Hit!
													// Get squared distance
													float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

													// Check squared distance
													if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
														fSquaredDistance = fCurrentSquaredDistance;
														nTriangle = nTri;
														if (pnGeometry)
															*pnGeometry = nGeometryIndex;
														if (pvCollisionPoint)
															*pvCollisionPoint = vIntersectionPointPos;
													}
												}
											}
										}
									}
								}
							}
						} else {
							for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
								// Get the geometry
								const Geometry &cGeometry = lstGeometries[nGeo];

								// Is this geometry active?
								if (cGeometry.IsActive()) {
									const uint32 nNumOfTriangles = cGeometry.GetNumOfTriangles();

									// Optimized for 'triangle list' ('MeshHandler::FindTriangle()' can become a heavy bottleneck!)
									if (cGeometry.GetPrimitiveType() == Primitive::TriangleList) {
										if (pIndexBuffer->GetElementType() == IndexBuffer::UInt) {
											// Get a pointer to the first index of the geometry
											const uint32 *pnIndex = static_cast<const uint32*>(pIndexBuffer->GetData()) + cGeometry.GetStartIndex();

											// Loop through all triangles
											for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
												// Get triangle data
												const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV1.x = pfVertex[Vector3::X];
												vV1.y = pfVertex[Vector3::Y];
												vV1.z = pfVertex[Vector3::Z];
												pnIndex++;
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV2.x = pfVertex[Vector3::X];
												vV2.y = pfVertex[Vector3::Y];
												vV2.z = pfVertex[Vector3::Z];
												pnIndex++;
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV3.x = pfVertex[Vector3::X];
												vV3.y = pfVertex[Vector3::Y];
												vV3.z = pfVertex[Vector3::Z];
												pnIndex++;

												// Get face normal
												vN.GetFaceNormal(vV1, vV2, vV3);

												// Perform culling?
												if (nCull != Cull::None) {
													// Calculate the dot product between the two normalized vectors
													const float fDotProduct = vN.DotProduct(vNormalizedDirection);

													// Perform culling
													if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
														continue;	// Use a simple "continue", not additional conditions
												}

												// Check triangle intersection
												if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
													// Hit!
													// Get squared distance
													float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

													// Check squared distance
													if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
														fSquaredDistance = fCurrentSquaredDistance;
														nTriangle = nTri;
														if (pnGeometry)
															*pnGeometry = nGeo;
														if (pvCollisionPoint)
															*pvCollisionPoint = vIntersectionPointPos;
													}
												}
											}
										} else if (pIndexBuffer->GetElementType() == IndexBuffer::UShort) {
											// Get a pointer to the first index of the geometry
											const uint16 *pnIndex = static_cast<const uint16*>(pIndexBuffer->GetData()) + cGeometry.GetStartIndex();

											// Loop through all triangles
											for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
												// Get triangle data
												const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV1.x = pfVertex[Vector3::X];
												vV1.y = pfVertex[Vector3::Y];
												vV1.z = pfVertex[Vector3::Z];
												pnIndex++;
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV2.x = pfVertex[Vector3::X];
												vV2.y = pfVertex[Vector3::Y];
												vV2.z = pfVertex[Vector3::Z];
												pnIndex++;
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV3.x = pfVertex[Vector3::X];
												vV3.y = pfVertex[Vector3::Y];
												vV3.z = pfVertex[Vector3::Z];
												pnIndex++;

												// Get face normal
												vN.GetFaceNormal(vV1, vV2, vV3);

												// Perform culling?
												if (nCull != Cull::None) {
													// Calculate the dot product between the two normalized vectors
													const float fDotProduct = vN.DotProduct(vNormalizedDirection);

													// Perform culling
													if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
														continue;	// Use a simple "continue", not additional conditions
												}

												// Check triangle intersection
												if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
													// Hit!
													// Get squared distance
													float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

													// Check squared distance
													if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
														fSquaredDistance = fCurrentSquaredDistance;
														nTriangle = nTri;
														if (pnGeometry)
															*pnGeometry = nGeo;
														if (pvCollisionPoint)
															*pvCollisionPoint = vIntersectionPointPos;
													}
												}
											}
										} else {
											// Get a pointer to the first index of the geometry
											const uint8 *pnIndex = static_cast<const uint8*>(pIndexBuffer->GetData()) + cGeometry.GetStartIndex();

											// Loop through all triangles
											for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
												// Get triangle data
												const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV1.x = pfVertex[Vector3::X];
												vV1.y = pfVertex[Vector3::Y];
												vV1.z = pfVertex[Vector3::Z];
												pnIndex++;
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV2.x = pfVertex[Vector3::X];
												vV2.y = pfVertex[Vector3::Y];
												vV2.z = pfVertex[Vector3::Z];
												pnIndex++;
												pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*(*pnIndex));
												vV3.x = pfVertex[Vector3::X];
												vV3.y = pfVertex[Vector3::Y];
												vV3.z = pfVertex[Vector3::Z];
												pnIndex++;

												// Get face normal
												vN.GetFaceNormal(vV1, vV2, vV3);

												// Perform culling?
												if (nCull != Cull::None) {
													// Calculate the dot product between the two normalized vectors
													const float fDotProduct = vN.DotProduct(vNormalizedDirection);

													// Perform culling
													if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
														continue;	// Use a simple "continue", not additional conditions
												}

												// Check triangle intersection
												if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
													// Hit!
													// Get squared distance
													float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

													// Check squared distance
													if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
														fSquaredDistance = fCurrentSquaredDistance;
														nTriangle = nTri;
														if (pnGeometry)
															*pnGeometry = nGeo;
														if (pvCollisionPoint)
															*pvCollisionPoint = vIntersectionPointPos;
													}
												}
											}
										}
									} else {
										// Loop through all triangles
										for (uint32 nTri=0; nTri<nNumOfTriangles; nTri++) {
											// Get triangle data
											uint32 nVertex1, nVertex2, nVertex3;
											pLODLevel->GetTriangle(nGeo, nTri, nVertex1, nVertex2, nVertex3);
											const float *pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*nVertex1);
											vV1.x = pfVertex[Vector3::X];
											vV1.y = pfVertex[Vector3::Y];
											vV1.z = pfVertex[Vector3::Z];
											pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*nVertex2);
											vV2.x = pfVertex[Vector3::X];
											vV2.y = pfVertex[Vector3::Y];
											vV2.z = pfVertex[Vector3::Z];
											pfVertex = reinterpret_cast<const float*>(pFirstVertex + nVertexSize*nVertex3);
											vV3.x = pfVertex[Vector3::X];
											vV3.y = pfVertex[Vector3::Y];
											vV3.z = pfVertex[Vector3::Z];

											// Get face normal
											vN.GetFaceNormal(vV1, vV2, vV3);

											// Perform culling?
											if (nCull != Cull::None) {
												// Calculate the dot product between the two normalized vectors
												const float fDotProduct = vN.DotProduct(vNormalizedDirection);

												// Perform culling
												if ((nCull == Cull::CW && fDotProduct < 0.0f) || (nCull == Cull::CCW && fDotProduct > 0.0f))
													continue;	// Use a simple "continue", not additional conditions
											}

											// Check triangle intersection
											if (Intersect::TriangleRay(vV1, vV2, vV3, vN, vLineStartPos, vDirection, vIntersectionPointPos)) {
												// Hit!
												// Get squared distance
												float fCurrentSquaredDistance = Math::Abs((vIntersectionPointPos-vLineStartPos).GetSquaredLength());

												// Check squared distance
												if (fSquaredDistance < 0.0f || fCurrentSquaredDistance < fSquaredDistance) {
													fSquaredDistance = fCurrentSquaredDistance;
													nTriangle = nTri;
													if (pnGeometry)
														*pnGeometry = nGeo;
													if (pvCollisionPoint)
														*pvCollisionPoint = vIntersectionPointPos;
												}
											}
										}
									}
								}
							}
						}

						// Unlock the buffers
						pIndexBuffer->Unlock();
						m_pCurrentVertexBuffer->Unlock();

						// Return result
						return (fSquaredDistance >= 0.0f);
					}
				}

				// Unlock the vertex buffer
				m_pCurrentVertexBuffer->Unlock();
			}
		}
	}

	// No triangle found...
	return false;
}

/**
*  @brief
*    Returns a list of mesh geometries intersecting the plane set
*/
uint32 MeshHandler::FindGeometries(const PlaneSet &cPlaneSet, uint32 **ppnGeometries, Vector3 *pvCamDir) const
{
	// Check pointers
	if (m_pMesh && m_pCurrentVertexBuffer) {
		const MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(m_nLOD);
		if (pLODLevel) {
			// Lock buffers
			if (m_pCurrentVertexBuffer->Lock(Lock::ReadOnly)) {
				IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
				if (pIndexBuffer && pIndexBuffer->Lock(Lock::ReadOnly)) {
					// Variables
					const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
					Vector3 vV1, vV2, vV3, vN, vCamDir;

					// Loop through all geometries
					uint32 nGeometries = 0;
					if (ppnGeometries)
						*ppnGeometries = new uint32[lstGeometries.GetNumOfElements()];
					for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
						// Get the geometry
						const Geometry &cGeometry = lstGeometries[nGeo];

						// Is this geometry active?
						if (cGeometry.IsActive()) {
							// Loop through all triangles
							for (uint32 nTri=0; nTri<cGeometry.GetNumOfTriangles(); nTri++) {
								// Get triangle data
								uint32 nVertex1, nVertex2, nVertex3;
								pLODLevel->GetTriangle(nGeo, nTri, nVertex1, nVertex2, nVertex3);
								const float *pfVertex = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(nVertex1, VertexBuffer::Position));
								vV1.x = pfVertex[Vector3::X];
								vV1.y = pfVertex[Vector3::Y];
								vV1.z = pfVertex[Vector3::Z];
								pfVertex = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(nVertex2, VertexBuffer::Position));
								vV2.x = pfVertex[Vector3::X];
								vV2.y = pfVertex[Vector3::Y];
								vV2.z = pfVertex[Vector3::Z];
								pfVertex = static_cast<const float*>(m_pCurrentVertexBuffer->GetData(nVertex3, VertexBuffer::Position));
								vV3.x = pfVertex[Vector3::X];
								vV3.y = pfVertex[Vector3::Y];
								vV3.z = pfVertex[Vector3::Z];

								// Check backface
								if (pvCamDir) {
									vN.GetFaceNormal(vV1, vV2, vV3);
									if (pvCamDir->DotProduct(vN) < 0.0f)
										continue;
								}

								// Check this triangle
								if (Intersect::PlaneSetTriangle(cPlaneSet, vV1, vV2, vV3)) {
									// If one of the geometry triangles is intersecting the plane set the whole
									// geometry is intersecting the plane set...
									if (ppnGeometries)
										(*ppnGeometries)[nGeometries] = nGeo;
									nGeometries++;
									break;
								}
							}
						}
					}

					// Unlock the buffers
					m_pCurrentVertexBuffer->Unlock();
					pIndexBuffer->Unlock();

					// Return the number of intersecting geometries
					return nGeometries;
				}
			}

			// Unlock the vertex buffer
			m_pCurrentVertexBuffer->Unlock();
		}
	}

	// No geometries found...
	return 0;
}

/**
*  @brief
*    Gets the triangle list
*/
Array<MeshTriangle> *MeshHandler::GetTriangleList()
{
	return m_pMesh ? &m_pMesh->GetLODLevel(m_nLOD)->GetTriangleList() : nullptr;
}

/**
*  @brief
*    Gets the edge list
*/
Array<MeshEdge> *MeshHandler::GetEdgeList()
{
	return m_pMesh ? &m_pMesh->GetLODLevel(m_nLOD)->GetEdgeList() : nullptr;
}

/**
*  @brief
*    Builds the current triangle planes
*/
void MeshHandler::BuildTrianglePlaneList()
{
	// Get data and lock vertex buffer if required
	MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(0);
	if (pLODLevel) {
		const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
		VertexBuffer		  *pVertexBuffer = GetVertexBuffer();
		if (lstGeometries.GetNumOfElements() && pVertexBuffer->GetNumOfElements() && pVertexBuffer->Lock(Lock::ReadOnly)) {
			// Calculate triangle planes
			const Array<MeshTriangle> &lstTriangles = pLODLevel->GetTriangleList();
			if (lstTriangles.GetNumOfElements()) { // Use the triangle list for faster calculation
				Vector3 vV0, vV1, vV2;

				// Loop through all triangles
				m_lstTrianglePlanes.Resize(lstTriangles.GetNumOfElements());
				for (uint32 i=0; i<lstTriangles.GetNumOfElements(); i++) {
					const MeshTriangle &cTriangle = lstTriangles[i];
					// V0
					const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(cTriangle.nVertex[0], VertexBuffer::Position));
					vV0.x = pfVertex[0];
					vV0.y = pfVertex[1];
					vV0.z = pfVertex[2];
					// V1
					pfVertex = static_cast<const float*>(pVertexBuffer->GetData(cTriangle.nVertex[1], VertexBuffer::Position));
					vV1.x = pfVertex[0];
					vV1.y = pfVertex[1];
					vV1.z = pfVertex[2];
					// V2
					pfVertex = static_cast<const float*>(pVertexBuffer->GetData(cTriangle.nVertex[2], VertexBuffer::Position));
					vV2.x = pfVertex[0];
					vV2.y = pfVertex[1];
					vV2.z = pfVertex[2];

					// Calculate triangle plane
					m_lstTrianglePlanes[i].ComputeND(vV0, vV1, vV2);
				}
			} else { // Use GetTriangle() which is slower
				// Lock index buffer
				IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
				if (pIndexBuffer && pIndexBuffer->Lock(Lock::ReadOnly)) {
					uint32 nTriangle = 0;
					Vector3 vV0, vV1, vV2;

					// Build triangle plane list
					m_lstTrianglePlanes.Resize(pLODLevel->GetNumOfTriangles());
					for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
						const Geometry &cGeometry = lstGeometries[nGeo];
						for (uint32 nTri=0; nTri<cGeometry.GetNumOfTriangles(); nTri++) {
							// Get triangle data
							uint32 nVertex0, nVertex1, nVertex2;
							pLODLevel->GetTriangle(nGeo, nTri, nVertex0, nVertex1, nVertex2);
							// V0
							const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex0, VertexBuffer::Position));
							vV0.x = pfVertex[0];
							vV0.y = pfVertex[1];
							vV0.z = pfVertex[2];
							// V1
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Position));
							vV1.x = pfVertex[0];
							vV1.y = pfVertex[1];
							vV1.z = pfVertex[2];
							// V2
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::Position));
							vV2.x = pfVertex[0];
							vV2.y = pfVertex[1];
							vV2.z = pfVertex[2];

							// Calculate triangle plane
							m_lstTrianglePlanes[nTriangle++].ComputeND(vV0, vV1, vV2);
						}
					}

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}
			}

			// Unlock vertex buffer
			pVertexBuffer->Unlock();
		}
	}
}

/**
*  @brief
*    Gets the triangle plane list
*/
Array<Plane> *MeshHandler::GetTrianglePlaneList()
{
	// First check morph target triangle planes
	MeshMorphTarget *pMorphTarget = m_pMesh->GetMorphTarget(0);
	if (m_pCurrentVertexBuffer == pMorphTarget->GetVertexBuffer() &&
		// [TODO] Remove this?
//	if (m_pCurrentVertexBuffer == pMorphTarget->GetVertexBuffer(m_nLOD) &&
		pMorphTarget->GetTrianglePlaneList().GetNumOfElements()) {
		return &pMorphTarget->GetTrianglePlaneList();
	}

	// Return individual mesh handler triangle planes
	if (m_bRecalculateTrianglePlanes) {
		m_bRecalculateTrianglePlanes = false;
		BuildTrianglePlaneList();
	}
	return &m_lstTrianglePlanes;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clears all materials
*/
void MeshHandler::ClearMaterials()
{
	// Delete all materials
	for (uint32 i=0; i<m_lstMaterials.GetNumOfElements(); i++)
		delete m_lstMaterials[i];
	m_lstMaterials.Clear();
}

/**
*  @brief
*    Adds a new material
*/
Material *MeshHandler::AddMaterial(Material *pMaterial)
{
	// Check parameter
	if (!pMaterial)
		return nullptr;

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
}

/**
*  @brief
*    Called when the frame of an animation manager animation has been changed
*/
void MeshHandler::OnAnimationFrameChange()
{
	// Relevant data 'may' have been changed and we 'may' need to update the mesh
	m_bMeshUpdateRequired = true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
