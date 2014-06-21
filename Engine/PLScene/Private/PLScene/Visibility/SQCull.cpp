/*********************************************************\
 *  File: SQCull.cpp                                     *
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
#include <PLCore/Tools/Stopwatch.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Renderer/OcclusionQuery.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/MeshHandler.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SNSpotLight.h"
#include "PLScene/Scene/SNCellPortal.h"
#include "PLScene/Scene/SNAntiPortal.h"
#include "PLScene/Scene/SNPointLight.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneHierarchy.h"
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Scene/SceneQueryHandler.h"
#include "PLScene/Scene/SceneHierarchyNode.h"
#include "PLScene/Scene/SceneHierarchyNodeItem.h"
#include "PLScene/Visibility/VisContainer.h"
#include "PLScene/Visibility/VisPortal.h"
#include "PLScene/Visibility/SQCull.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SQCull)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SQCull::SQCull() :
	m_nMode(Coherent),
	m_nVisibilityThreshold(10),
	m_pCameraContainer(new SceneNodeHandler()),
	m_bCameraInCell(false),
	m_bSetIdentityWorldMatrix(true),
	m_pHierarchy(nullptr),
	m_nFrame(0),
	m_nCurrentQueries(0),
	m_nOcclusionQueries(0),
	m_ppOcclusionQueries(nullptr),
	m_pVisRootContainer(nullptr),
	m_pVisContainer(nullptr)
{
	// Initialize the statistics
	InitStatistics();
}

/**
*  @brief
*    Destructor
*/
SQCull::~SQCull()
{
	// Destroy the camera container handler
	if (m_pCameraContainer)
		delete m_pCameraContainer;

	// Destroy occlusion queries
	if (m_ppOcclusionQueries) {
		for (uint32 i=0; i<m_nOcclusionQueries; i++)
			delete m_ppOcclusionQueries[i];
		delete [] m_ppOcclusionQueries;
	}

	// If this is the root of the scene, destroy the visibility tree
	if (m_pVisRootContainer == m_pVisContainer && m_pVisRootContainer) {
		m_pVisRootContainer->m_pQueryHandler->SetElement(nullptr);
		delete m_pVisRootContainer;
	}
}

/**
*  @brief
*    Returns the used cull mode
*/
SQCull::EMode SQCull::GetMode() const
{
	return m_nMode;
}

/**
*  @brief
*    Sets the used cull mode
*/
void SQCull::SetMode(EMode nMode)
{
	m_nMode = nMode;
}

/**
*  @brief
*    Returns visibility threshold
*/
uint32 SQCull::GetVisibilityThreshold() const
{
	return m_nVisibilityThreshold;
}

/**
*  @brief
*    Sets visible pixels threshold for visibility classification
*/
void SQCull::SetVisibilityThreshold(uint32 nThreshold)
{
	m_nVisibilityThreshold = nThreshold;
}

/**
*  @brief
*    Returns the statistics
*/
const SQCull::Statistics &SQCull::GetStatistics() const
{
	return m_sStatistics;
}

/**
*  @brief
*    Returns the container the camera is in
*/
SceneContainer *SQCull::GetCameraContainer() const
{
	return static_cast<SceneContainer*>(m_pCameraContainer->GetElement());
}

/**
*  @brief
*    Sets the container the camera is in
*/
bool SQCull::SetCameraContainer(SceneContainer *pCameraContainer)
{
	// Check parameter
	if (pCameraContainer && !pCameraContainer->IsContainer())
		return false; // Error!

	// Set the camera container
	m_pCameraContainer->SetElement(pCameraContainer);

	// Done
	return true;
}

/**
*  @brief
*    Returns the used camera position
*/
const Vector3 &SQCull::GetCameraPosition() const
{
	return m_vCameraPosition;
}

/**
*  @brief
*    Sets the used camera position
*/
void SQCull::SetCameraPosition(const Vector3 &vPos)
{
	m_vCameraPosition = vPos;
}

/**
*  @brief
*    Returns the used view frustum
*/
const PlaneSet &SQCull::GetViewFrustum() const
{
	return m_cViewFrustum;
}

/**
*  @brief
*    Sets the used view frustum
*/
void SQCull::SetViewFrustum(const PlaneSet &cFrustum)
{
	m_cViewFrustum = cFrustum;
}

// [TODO] Cleanup
const Matrix4x4 &SQCull::GetProjectionMatrix() const
{
	return m_mProjection;
}

void SQCull::SetProjectionMatrix(const Matrix4x4 &mProjection)
{
	m_mProjection = mProjection;
}

const Matrix4x4 &SQCull::GetViewMatrix() const
{
	return m_mView;
}

void SQCull::SetViewMatrix(const Matrix4x4 &mView)
{
	m_mView = mView;
}

const Matrix4x4 &SQCull::GetViewProjectionMatrix() const
{
	return m_mViewProjection;
}

void SQCull::SetViewProjectionMatrix(const Matrix4x4 &mViewProjection)
{
	m_mViewProjection = mViewProjection;
}

/**
*  @brief
*    Returns the visibility root container
*/
const VisContainer *SQCull::GetVisRootContainer() const
{
	if (!m_pVisRootContainer) {
		// Because the user is NOT allowed to manipulate the visibility tree by itself
		// ALL functions and returned stuff has to be constant...
		SQCull *pSQCull = const_cast<SQCull*>(this);
		pSQCull->m_pVisRootContainer = pSQCull->m_pVisContainer = new VisContainer();
		pSQCull->m_pVisRootContainer->m_pQueryHandler->SetElement(pSQCull);
	}
	return m_pVisRootContainer;
}

/**
*  @brief
*    Returns the visibility container this scene query belongs to
*/
const VisContainer &SQCull::GetVisContainer() const
{
	if (!m_pVisContainer) {
		// Because the user is NOT allowed to manipulate the visibility tree by itself
		// ALL functions and returned stuff has to be constant...
		SQCull *pSQCull = const_cast<SQCull*>(this);
		pSQCull->m_pVisRootContainer = pSQCull->m_pVisContainer = new VisContainer();
		pSQCull->m_pVisRootContainer->m_pQueryHandler->SetElement(pSQCull);
	}
	return *m_pVisContainer;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialize the statistics
*/
void SQCull::InitStatistics()
{
	m_sStatistics.nCullTime						   = 0;
	m_sStatistics.nNumOfTraversedNodes			   = 0;
	m_sStatistics.nNumOfOccluders				   = 0;
	m_sStatistics.nNumOfQueryCulledNodes		   = 0;
	m_sStatistics.nNumOfSkippedFrustumTests		   = 0;
	m_sStatistics.nNumOfFrustumCulledNodes		   = 0;
	m_sStatistics.nNumOfNearPlaneIntersectingNodes = 0;
	m_sStatistics.nNumOfVisibleSceneNodes		   = 0;
	m_sStatistics.nNumOfQueries					   = 0;
	m_sStatistics.nMaxNumOfQueries				   = 0;
	m_sStatistics.bWaitForQueryResult			   = false;
}

/**
*  @brief
*    Resizes the arrays with the additional scene hierarchy node information if required
*/
void SQCull::ResizeArrays()
{
	// Do we need to resize your arrays?
	uint32 nTotalNumOfNodes = m_pHierarchy->GetTotalNumOfNodes();
	if (m_lstVisibility.GetNumOfElements() != nTotalNumOfNodes) {
		m_lstVisibility.     Resize(nTotalNumOfNodes, true, true);
		m_lstTotalVisibility.Resize(nTotalNumOfNodes, true, true);
		m_lstLastVisited.    Resize(nTotalNumOfNodes, true, true);
	}
}

/**
*  @brief
*    This is the standard culling traversal algorithm doing only frustum culling
*/
bool SQCull::CullFrustum()
{
	// Add root node
	m_lstDistanceQueue.Add(0.0f, &GetSceneContainer().GetHierarchyInstance()->GetRootNode());

	// Loop until the distance queue is empty
	while (m_lstDistanceQueue.GetNumOfElements()) {
		// Get the first node and it's nearest distance to the camera and remove the node from the queue
		SceneHierarchyNode *pHierarchyNode;
		float				fNearestSquaredDistance;
		m_lstDistanceQueue.ExtractTop(&pHierarchyNode, &fNearestSquaredDistance);
		m_sStatistics.nNumOfTraversedNodes++;

		// We don't need to know about near plane intersection
		// for frustum culling only, but we have to pass a parameter
		bool bIntersectsNearplane;
		if (InsideViewFrustum(*pHierarchyNode, bIntersectsNearplane, false)) {
			// Touch this node and resize our arrays if required
			pHierarchyNode->Touch();
			ResizeArrays();

			if (bIntersectsNearplane)
				m_sStatistics.nNumOfNearPlaneIntersectingNodes++;
			m_lstVisibility.Set(pHierarchyNode->GetID());
			if (!TraverseNode(*pHierarchyNode)) {
				m_lstDistanceQueue.Clear();

				// Cancelled by the user
				return false;
			}
		} else {
			m_lstVisibility.Clear(pHierarchyNode->GetID());
			m_sStatistics.nNumOfFrustumCulledNodes++;
		}
	}

	// Done, not cancelled by the user
	return true;
}

/**
*  @brief
*    This is the naive algorithm always waiting for the query to finish
*/
bool SQCull::CullStopAndWait()
{
	// Add root node
	m_lstDistanceQueue.Add(0.0f, &GetSceneContainer().GetHierarchyInstance()->GetRootNode());

	// Loop until the distance queue is empty
	while (m_lstDistanceQueue.GetNumOfElements()) {
		// Get the first node and it's nearest distance to the camera and remove the node from the queue
		SceneHierarchyNode *pHierarchyNode;
		float				fNearestSquaredDistance;
		m_lstDistanceQueue.ExtractTop(&pHierarchyNode, &fNearestSquaredDistance);
		m_sStatistics.nNumOfTraversedNodes++;

		bool bIntersectsNearplane;
		if (InsideViewFrustum(*pHierarchyNode, bIntersectsNearplane)) {
			// Touch this node and resize our arrays if required
			pHierarchyNode->Touch();
			ResizeArrays();

			// For near plane intersecting AABs possible
			// wrong results => skip occlusion query
			if (bIntersectsNearplane) {
				m_lstVisibility.Set(pHierarchyNode->GetID());
				m_sStatistics.nNumOfNearPlaneIntersectingNodes++;
				if (!TraverseNode(*pHierarchyNode)) {
					m_lstDistanceQueue.Clear();

					// Cancelled by the user
					return false;
				}
			} else {
				IssueOcclusionQuery(*pHierarchyNode, false);

				// Wait if result not available
				const uint32 nVisiblePixels = GetOcclusionQueryResult(*pHierarchyNode);

				// Node visible
				if (nVisiblePixels > m_nVisibilityThreshold) {
					m_lstVisibility.Set(pHierarchyNode->GetID());
					if (!TraverseNode(*pHierarchyNode)) {
						m_lstDistanceQueue.Clear();

						// Cancelled by the user
						return false;
					}
				} else {
					m_sStatistics.nNumOfQueryCulledNodes++;
				}
			}
		} else {
			m_lstVisibility.Clear(pHierarchyNode->GetID());
			m_sStatistics.nNumOfFrustumCulledNodes++;
		}
	}

	// Done, not cancelled by the user
	return true;
}

/**
*  @brief
*    Culls the scene using coherent occlusion culling
*/
bool SQCull::CullCoherentWithQueue()
{
	Pool<SceneHierarchyNode*> lstQuery;

	// Add root node
	m_lstDistanceQueue.Add(0.0f, &GetSceneContainer().GetHierarchyInstance()->GetRootNode());

	// PART 1: Process finished occlusion queries
	while (m_lstDistanceQueue.GetNumOfElements() || lstQuery.GetNumOfElements()) {
		while (lstQuery.GetNumOfElements() && 
			  (ResultAvailable(*lstQuery[0]) || !m_lstDistanceQueue.GetNumOfElements())) {
			SceneHierarchyNode &cHierarchyNode = *lstQuery[0];
			lstQuery.RemoveAtIndex(0);

			// Wait until result available
			const uint32 nVisiblePixels = GetOcclusionQueryResult(cHierarchyNode);
			if (nVisiblePixels > m_nVisibilityThreshold) {
				PullUpVisibility(&cHierarchyNode);
				if (!TraverseNode(cHierarchyNode)) {
					m_lstDistanceQueue.Clear();

					// Cancelled by the user
					return false;
				}
			} else {
				m_sStatistics.nNumOfQueryCulledNodes++;
			}
		}

		// PART 2: Hierarchical traversal
		if (m_lstDistanceQueue.GetNumOfElements()) {
			// Get the first node and it's nearest distance to the camera and remove the node from the queue
			SceneHierarchyNode *pHierarchyNode;
			float			    fNearestSquaredDistance;
			m_lstDistanceQueue.ExtractTop(&pHierarchyNode, &fNearestSquaredDistance);
			m_sStatistics.nNumOfTraversedNodes++;

			bool bIntersectsNearplane;
			if (InsideViewFrustum(*pHierarchyNode, bIntersectsNearplane)) {
				// Touch this node and resize our arrays if required
				pHierarchyNode->Touch();
				ResizeArrays();

				// For near plane intersecting AABs possible
				// wrong results => skip occlusion query
				if (bIntersectsNearplane) {
					// Update node's visited flag
					m_lstLastVisited[pHierarchyNode->GetID()] = m_nFrame;

					m_sStatistics.nNumOfNearPlaneIntersectingNodes++;
					PullUpVisibility(pHierarchyNode);
					if (!TraverseNode(*pHierarchyNode)) {
						m_lstDistanceQueue.Clear();

						// Cancelled by the user
						return false;
					}
				} else {
					// Identify previously visible nodes
					const bool bWasVisible = m_lstVisibility.IsSet(pHierarchyNode->GetID()) && (m_lstLastVisited[pHierarchyNode->GetID()] == m_nFrame-1);

					// Identify nodes that we cannot skip queries for
					const bool bLeafOrWasInvisible = !bWasVisible || !pHierarchyNode->GetNumOfNodes();

					// Reset node's visibility classification 
					m_lstVisibility.Clear(pHierarchyNode->GetID());

					// Update node's visited flag
					m_lstLastVisited[pHierarchyNode->GetID()] = m_nFrame;

					// Skip testing previously visible interior nodes
					if (bLeafOrWasInvisible) {
						IssueOcclusionQuery(*pHierarchyNode, bWasVisible);
						lstQuery.Add(pHierarchyNode);
					}

					// Always traverse a node if it was visible
					if (bWasVisible) {
						if (!TraverseNode(*pHierarchyNode)) {
							m_lstDistanceQueue.Clear();

							// Cancelled by the user
							return false;
						}
					}
				}
			} else {
				m_sStatistics.nNumOfFrustumCulledNodes++;
			}
		}
	}

	// Done, not cancelled by the user
	return true;
}

/**
*  @brief
*    Traverses a node
*/
bool SQCull::TraverseNode(const SceneHierarchyNode &cHierarchyNode)
{
	// Insert all assigned scene nodes to the scene node distance query
	if (cHierarchyNode.GetNumOfItems()) {
		// Is the scene hierarchy total visible? In this case, we do not need to check
		// the scene nodes against the frustum...
		const bool bTotalVisible = m_lstTotalVisibility.IsSet(cHierarchyNode.GetID());

		// Use scene node class names (in performance critical situations, such static strings should be only allocated once)
		static const String sSNAntiPortal = "PLScene::SNAntiPortal";
		static const String sSNCellPortal = "PLScene::SNCellPortal";

		// [TODO] This is currently just a first anti-portal test, anti-portals should be passed down container hierarchies as well...
		Array<PlaneSet*> lstAntiPortals;
		{
			const SceneHierarchyNodeItem *pItem = cHierarchyNode.GetFirstItem();
			while (pItem) {
				// Get the linked scene node
				SceneNode *pSceneNode = pItem->GetSceneNode();

				// Is this an anti-portal?
				if (pSceneNode->IsPortal() && pSceneNode->IsInstanceOf(sSNAntiPortal)) {
					// Is the camera in front of this anti-portal?
					Plane::ESide nSide = static_cast<SNAntiPortal*>(pSceneNode)->GetPolygon().GetPlane().GetSide(pSceneNode->GetTransform().GetInverseMatrix()*m_vCameraPosition);
					if (nSide == Plane::InFront) {
						// Next item, please
						pItem = pItem->GetNextItem();
						continue;
					}

					// Check scene node against the frustum
					const AABoundingBox &cAABB = pSceneNode->GetContainerAABoundingBox();
					bool bVisible = bTotalVisible;
					if (!bTotalVisible) {
						if (pSceneNode->GetFlags() & SceneNode::NoCulling)
							bVisible = true;
						else
							bVisible = Intersect::PlaneSetAABox(m_cViewFrustum, cAABB.vMin, cAABB.vMax);
					}
					if (bVisible) {
						// Get and check the container space anti-portal polygon
						SNAntiPortal &cAntiPortal = static_cast<SNAntiPortal&>(*pSceneNode);
						Polygon &cPolygon = const_cast<Polygon&>(reinterpret_cast<const Polygon&>(cAntiPortal.GetContainerPolygon()));
						if (cPolygon.GetVertexList().GetNumOfElements() >= 3) {
							// Calculate the new anti-portal view frustum using the clipped anti-portal polygon
							PlaneSet *pPlaneSet = new PlaneSet();
							if (pPlaneSet->CreateViewPlanes(cPolygon.GetVertexList(), m_vCameraPosition))
								lstAntiPortals.Add(pPlaneSet);
							else
								delete pPlaneSet;
						}
					}
				}

				// Next item, please
				pItem = pItem->GetNextItem();
			}
		}


		// Get the scene context
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext) {
			const SceneHierarchyNodeItem *pItem = cHierarchyNode.GetFirstItem();
			while (pItem) {
				// Get the linked scene node
				SceneNode *pSceneNode = pItem->GetSceneNode();

				// Is this scene node a cell? If yes, skip it if the camera is within a cell, too - we CAN'T see it from
				// outside, we can ONLY see it through a cell-portal! :)
				if (!pSceneNode || !pSceneNode->IsVisible() || (pSceneNode->IsCell() && m_bCameraInCell && pSceneNode != m_pCameraContainer->GetElement())) {
					// Next item, please
					pItem = pItem->GetNextItem();
					continue;
				}

				// [TODO] What to do if the scene node was already visible through another cell-portal, but it's visible through the current one, too???
				//        -> A scene node can be seen through multiple cell-portals... (for instance 2 cell-portals linking into the same cell)
				// Was this scene node already processed?
				if (!pSceneContext->IsNodeTouched(*pSceneNode)) {
					const AABoundingBox &cAABB = pSceneNode->GetContainerAABoundingBox();

					// Before we do more complex tests - let's first do a simple distance test!
					const float fMaxDrawDistance = pSceneNode->MaxDrawDistance;
					float fNearestSquaredDistance = 0.0f;
					if (fMaxDrawDistance >= 0.0f) {
						// Get vector from viewpoint to center of bounding volume
						const Vector3 vV = cAABB.GetCenter()-m_vCameraPosition;

						// Compute distance from nearest point to viewpoint
						fNearestSquaredDistance = vV.GetSquaredLength();

						// Check if the scene node is 'out of range', if not, add it to the query
						if (fMaxDrawDistance > 0.0f && fNearestSquaredDistance > fMaxDrawDistance*fMaxDrawDistance) {
							// Next item, please
							pItem = pItem->GetNextItem();
							continue;
						}
					}

					// If this is a cell-portal, do ONLY take the cell-portal into account if the camera is with a cell and in front of it!
					// (else we may end up in an ugly infinite recursion!)
					if (m_bCameraInCell && pSceneNode->IsPortal() && pSceneNode->IsInstanceOf(sSNCellPortal)) {
						// [HACK] Nasty precision errors on the horizon of a portal to pass through: While the "go through" test
						// still says we're on the one side of the portal, due precision errors the visibility check may detect that we're already
						// on the other side on the portal. This results in, even if we should be able to 'look through' a portal, we can't see
						// through it because a test tells that this portal is facing away from us and therefore can be ignored.
						// Try to rethink the portal stuff when making the refactoring of the visibility system, but for now: If camera and portal
						// are within the same space, don't perform a portal side check
						if (pSceneNode->GetContainer() != m_pCameraContainer->GetElement()) {
							// Is the camera in front of this cell-portal?
							Plane::ESide nSide = static_cast<SNCellPortal*>(pSceneNode)->GetPolygon().GetPlane().GetSide(pSceneNode->GetTransform().GetInverseMatrix()*m_vCameraPosition);
							if (nSide == Plane::InFront) {
								// Next item, please
								pItem = pItem->GetNextItem();
								continue;
							}
						}
					}

					// Check scene node against the plane set. Because lights normally have a heavy impact on the performance,
					// we use more aggressive culling tests.
					bool bVisible = bTotalVisible;
					if (!bTotalVisible) {
						if (pSceneNode->GetFlags() & SceneNode::NoCulling) {
							bVisible = true;
						} else {
							if (pSceneNode->IsLight() && !static_cast<SNLight*>(pSceneNode)->IsEffectLight()) {
								if (static_cast<SNLight*>(pSceneNode)->IsSpotLight()) {
									bVisible = Intersect::PlaneSetSphere(m_cViewFrustum, pSceneNode->GetTransform().GetPosition(), static_cast<SNSpotLight*>(pSceneNode)->GetRange()) &&
											Intersect::PlaneSetPoints(m_cViewFrustum, static_cast<SNSpotLight*>(pSceneNode)->GetFrustumVertices());
								} else if (static_cast<SNLight*>(pSceneNode)->IsPointLight()) {
									bVisible = Intersect::PlaneSetSphere(m_cViewFrustum, pSceneNode->GetTransform().GetPosition(), static_cast<SNPointLight*>(pSceneNode)->GetRange());
								} else {
									bVisible = Intersect::PlaneSetAABox(m_cViewFrustum, cAABB.vMin, cAABB.vMax);
								}
							} else {
								bVisible = Intersect::PlaneSetAABox(m_cViewFrustum, cAABB.vMin, cAABB.vMax);
							}


							// [TODO] This is currently just a first anti-portal test
							if (bVisible) {
								for (uint32 i=0; i<lstAntiPortals.GetNumOfElements() && bVisible; i++) {
									PlaneSet *pPlaneSet = lstAntiPortals[i];

									// Check whether this node is complete within the frustum
									uint32 nClipMask = 0;
									bool bVisibleT = Intersect::PlaneSetAABox(*pPlaneSet, cAABB.vMin, cAABB.vMax, &nClipMask);
									if (bVisibleT) {
										if (!nClipMask)
											bVisible = false;
									}
								}
							}


						}
					}
					if (bVisible) {
						// Touch this node
						pSceneContext->TouchNode(*pSceneNode);

						// Total visible?
						if (bTotalVisible)
							m_sStatistics.nNumOfSkippedFrustumTests++;

						// Is there a maximum draw distance given?
						if (fMaxDrawDistance >= 0.0f) {
							// Add the scene node to the query
							m_lstNodeDistanceQueue.Add(fNearestSquaredDistance, pSceneNode);
						} else {
							// Special case, if the maximum draw distance of the node is negative draw this node BEFORE
							// other ones
							m_lstNodeDistanceQueue.Add(-(fMaxDrawDistance*fMaxDrawDistance), pSceneNode);
						}
					}
				}

				// Next item, please
				pItem = pItem->GetNextItem();
			}
		}


		// [TODO] This is currently just a first anti-portal test
		for (uint32 i=0; i<lstAntiPortals.GetNumOfElements(); i++)
			delete lstAntiPortals[i];


		// Collect all found scene nodes and inform all listeners
		if (m_lstNodeDistanceQueue.GetNumOfElements()) {
			// Go through the queue
			while (m_lstNodeDistanceQueue.GetNumOfElements()) {
				// Get the first node and it's nearest distance to the camera and remove the node from the queue
				SceneNode *pSceneNode;
				float      fNearestSquaredDistance;
				m_lstNodeDistanceQueue.ExtractTop(&pSceneNode, &fNearestSquaredDistance);

				// Add the scene node to the list of visible scene nodes
				VisContainer &cVisContainer = const_cast<VisContainer&>(GetVisContainer());
				VisNode *pNode = cVisContainer.AddSceneNode(*pSceneNode, fNearestSquaredDistance);
				if (pNode) {
					SQCull *pCullQuery = nullptr;

					// Get the renderer
					Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

					// Set the world transform matrix
					pNode->SetWorldMatrix(cVisContainer.m_mWorld*pSceneNode->GetTransform().GetMatrix());

					// Inform the scene node
					pSceneNode->OnAddedToVisibilityTree(*pNode);

					// [TODO] No fixed functions in here
					FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
					if (pFixedFunctions) {
						// Draw the occlude?
						if (pSceneNode->GetFlags() & SceneNode::CanOcclude && (m_nMode == StopAndWait || m_nMode == Coherent)) {
							const MeshHandler *pMeshHandler = pSceneNode->GetMeshHandler();
							if (pMeshHandler) {
								pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, pNode->m_mWorld);
								cRenderer.SetRenderState(RenderState::ZWriteEnable, true);
								pMeshHandler->Draw(false, false);

								// Update the statistics
								m_sStatistics.nNumOfOccluders++;
							}
						}
					}

					// Set identity world matrix next time...
					m_bSetIdentityWorldMatrix = true;

					// Emit signal
					SignalSceneNode(*this, *pSceneNode);
					if (m_nFlags & StopQuery) {
						m_lstNodeDistanceQueue.Clear();

						// Cancelled by the user
						return false;
					}

					// Is this a container
					if (pSceneNode->IsContainer()) {
						// Is recursion allowed?
						if (!(pSceneNode->GetFlags() & SceneContainer::NoRecursion)) {
							// Get the cull query
							pCullQuery = const_cast<SQCull*>(static_cast<const VisContainer*>(pNode)->GetCullQuery());
							if (pCullQuery) {
								// Set the projection information
								VisContainer::Projection &sNewProjection = static_cast<VisContainer*>(pNode)->m_sProjection;
								VisContainer::Projection &sProjection    = cVisContainer.m_sProjection;
								sNewProjection.cRectangle = sProjection.cRectangle;
								sNewProjection.fZNear     = sProjection.fZNear;
								sNewProjection.fZFar      = sProjection.fZFar;

								// Setup the container query
								pCullQuery->SetMode(GetMode());
								pCullQuery->SetVisibilityThreshold(GetVisibilityThreshold());
								pCullQuery->SetCameraContainer(nullptr);
								pCullQuery->m_bCameraInCell = m_bCameraInCell;

								// Transform the camera position into container space
								pCullQuery->m_vCameraPosition  = m_vCameraPosition;
								pCullQuery->m_vCameraPosition *= pSceneNode->GetTransform().GetInverseMatrix();

								// Transform the view frustum into container space
								pCullQuery->m_cViewFrustum = m_cViewFrustum;
								pCullQuery->m_cViewFrustum *= pSceneNode->GetTransform().GetInverseMatrix();

								// [TODO] Cleanup
								pCullQuery->m_mProjection     = m_mProjection;
								pCullQuery->m_mView			  = m_mView;
								pCullQuery->m_mViewProjection = m_mViewProjection;

								// Perform the query (recursion)
								pCullQuery->PerformQuery();
							}
						}

					// Is this a cell-portal?
					} else if (pSceneNode->IsPortal() && pSceneNode->IsInstanceOf(sSNCellPortal)) {
						// Is it possible to 'see through' this cell-portal?
						if (static_cast<const VisPortal*>(pNode)->GetTargetVisContainer() && (pSceneNode->GetFlags() & SNCellPortal::NoSeeThrough))
							continue;

						// Get and check the container space cell-portal polygon
						SNCellPortal &cCellPortal = static_cast<SNCellPortal&>(*pSceneNode);
						Polygon		 &cPolygon    = const_cast<Polygon&>(static_cast<const Polygon&>(cCellPortal.GetContainerPolygon()));
						if (cPolygon.GetVertexList().GetNumOfElements() >= 3) {
							// Set the projection information
							VisContainer *pCell = static_cast<const VisPortal*>(pNode)->GetTargetVisContainer();
							if (pCell && pCell->GetSceneNode()) {
								VisContainer::Projection &sNewProjection = pCell->m_sProjection;
								VisContainer::Projection &sProjection    = cVisContainer.m_sProjection;

								// Calculate the cell-portal scissor rectangle
								// Get viewport parameters
								const uint32 nX      = static_cast<uint32>(cRenderer.GetViewport().GetX());
								const uint32 nY      = static_cast<uint32>(cRenderer.GetViewport().GetY());
								const uint32 nWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
								const uint32 nHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());

								// Calculate the model view projection matrix
								Matrix4x4 mMVP = m_mViewProjection;
								mMVP *= pNode->m_mWorld;

								// Calculate the scissor rectangle
								sNewProjection.cRectangle.ScreenRectangle(cCellPortal.GetPolygon().GetVertexList(), true, mMVP, nX, nY, nWidth, nHeight);

								// Clip the new cell-portal rectangle against the current rectangle
								sNewProjection.cRectangle.ClipByRectangle(sProjection.cRectangle);

								// Is the cell-portal scissor rectangle visible?
								if (sNewProjection.cRectangle.GetWidth() && sNewProjection.cRectangle.GetHeight()) {
									// Increase the cell portal scissor rectangle a bit to reduce visible artifacts
									sNewProjection.cRectangle.vMin -= 1.0f;
									sNewProjection.cRectangle.vMax += 1.0f;

									// Clip the cell portal container space polygon against the current view frustum
									Polygon cClippedPolygon = cPolygon;
									bool bUseClippedPolygon = true;
									bool bNearPlane = false;
									for (uint32 i=0; i<m_cViewFrustum.GetNumOfPlanes(); i++) {
										if (bNearPlane || i != 0)
											cClippedPolygon = cClippedPolygon.Clip(*m_cViewFrustum[i]);
										else {
											if (cClippedPolygon.IsBehind(*m_cViewFrustum[i]) ||
												!cClippedPolygon.GetVertexList().GetNumOfElements()) {
												bUseClippedPolygon = false;
												break;
											}
										}
									}

									// Is there a valid clipped cell portal polygon? (< 3 - something went totally wrong :)
									if (bUseClippedPolygon && cClippedPolygon.GetVertexList().GetNumOfElements() >= 3) {
										// Setup the container query
										pCullQuery = const_cast<SQCull*>(pCell->GetCullQuery());
										if (pCullQuery) {
											pCullQuery->SetMode(GetMode());
											pCullQuery->SetVisibilityThreshold(GetVisibilityThreshold());
											pCullQuery->SetCameraContainer(nullptr);
											pCullQuery->m_bCameraInCell = m_bCameraInCell;

											// Transform the camera position into cell space
											pCullQuery->m_vCameraPosition  = m_vCameraPosition;
											pCullQuery->m_vCameraPosition *= cCellPortal.GetWarpMatrix();



											// [TODO] Make this universal
											// Make the world matrix relative to the container the camera is in
											VisContainer &cPortalVisContainer = const_cast<VisContainer&>(pCullQuery->GetVisContainer());
											SceneContainer *pCameraContainer = static_cast<SQCull*>(m_pVisRootContainer->m_pQueryHandler->GetElement())->GetCameraContainer();
											if (pCameraContainer) {
												cPortalVisContainer.SetWorldMatrix(pCell->GetSceneNode()->GetTransform().GetMatrix()*pCameraContainer->GetTransform().GetInverseMatrix());

												// If the camera is inside the axis aligned bounding box of the portal, just reuse the camera view
												// frustum instead of calculating a new, tight, one - to ensure this test also works for 'flat'
												// container axis aligned bounding boxes, we're performing a sphere test instead just a point test
												if (Intersect::AABoxSphere(cCellPortal.GetContainerAABoundingBox().vMin, cCellPortal.GetContainerAABoundingBox().vMax, m_vCameraPosition, 0.5f)) {
													// Just reuse the camera view frustum to avoid invalid frustums
													pCullQuery->m_cViewFrustum = m_cViewFrustum;
												} else {
													// Calculate the new cell-portal view frustum using the clipped cell-portal polygon
													pCullQuery->m_cViewFrustum.CreateViewPlanes(cClippedPolygon.GetVertexList(), m_vCameraPosition);
												}

												// Transform this view frustum into the target cell space
												pCullQuery->m_cViewFrustum *= cCellPortal.GetWarpMatrix();

												// [TODO] Cleanup
												pCullQuery->m_mProjection     = m_mProjection;
												pCullQuery->m_mView			  = m_mView;
												pCullQuery->m_mViewProjection = m_mViewProjection;

												// And finally, perform the query :) (recursion)
												pCullQuery->PerformQuery();
											}
										}
									} else {
										// Nope, remove this cell-portal from the list of visible nodes
										cVisContainer.m_lstNodes.RemoveAtIndex(cVisContainer.m_lstNodes.GetNumOfElements()-1);
									}
								} else {
									// Nope, we can remove this cell-portal from the list of visible nodes
									cVisContainer.m_lstNodes.RemoveAtIndex(cVisContainer.m_lstNodes.GetNumOfElements()-1);
								}
							}
						}
					}
				}
			}
		}
	}

	// Add children to priority queue for further processing
	for (uint32 i=0; i<cHierarchyNode.GetNumOfNodes(); i++) {
		SceneHierarchyNode *pNode = cHierarchyNode.GetNode(i);
		m_lstDistanceQueue.Add(pNode->GetShortestDistance(m_vCameraPosition), pNode);
	}

	// Done, not cancelled by the user
	return true;
}

/**
*  @brief
*    Visibility is pulled up from visibility of children
*/
void SQCull::PullUpVisibility(const SceneHierarchyNode *pHierarchyNode)
{
	while (pHierarchyNode && !m_lstVisibility.IsSet(pHierarchyNode->GetID())) {
		m_lstVisibility.Set(pHierarchyNode->GetID());
		pHierarchyNode = pHierarchyNode->GetParentNode();
	}
}

/**
*  @brief
*    Issues occlusion query for specified node
*/
void SQCull::IssueOcclusionQuery(const SceneHierarchyNode &cHierarchyNode, bool bWasVisible)
{
	// [TODO] Use 'bWasVisible'?

	// Get occlusion query
	if (cHierarchyNode.GetID() < m_nOcclusionQueries) {
		OcclusionQuery *pOcclusionQuery = m_ppOcclusionQueries[cHierarchyNode.GetID()];
		if (pOcclusionQuery->BeginOcclusionQuery()) {
			Renderer &cRenderer = pOcclusionQuery->GetRenderer();

			// Draw occlusion volume (check only, no z-buffer change)
			if (m_bSetIdentityWorldMatrix) {
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
				// [TODO] No fixed functions in here
				FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
				if (pFixedFunctions)
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
				m_bSetIdentityWorldMatrix = false;
			}
			// [TODO] Update this code (currently unused)
			//const AABoundingBox &cAABB = cHierarchyNode.GetAABoundingBox();
			// const Matrix4x4 mObjectSpaceToClipSpace = ...;
			//cRenderer.GetDrawHelpers().DrawBox(Color4::White, cAABB.vMin, cAABB.vMax, mObjectSpaceToClipSpace);

			// End occlusion test
			pOcclusionQuery->EndOcclusionQuery();

			// Update statistics
			m_nCurrentQueries++;
			m_sStatistics.nNumOfQueries++;
			if (m_sStatistics.nMaxNumOfQueries < m_nCurrentQueries)
				m_sStatistics.nMaxNumOfQueries = m_nCurrentQueries;
		}
	}
}

/**
*  @brief
*    Is result available from query queue?
*/
bool SQCull::ResultAvailable(const SceneHierarchyNode &cHierarchyNode)
{
	// Get occlusion query
	if (cHierarchyNode.GetID() >= m_nOcclusionQueries)
		return true; // Error!
	OcclusionQuery *pOcclusionQuery = m_ppOcclusionQueries[cHierarchyNode.GetID()];

	// We have to wait for the result
	uint32 nNumOfVisibleFragments;
	return pOcclusionQuery->PullOcclusionQuery(&nNumOfVisibleFragments);
}

/**
*  @brief
*    Returns occlusion query result for specified node
*/
uint32 SQCull::GetOcclusionQueryResult(const SceneHierarchyNode &cHierarchyNode)
{
	// Get occlusion query
	if (cHierarchyNode.GetID() >= m_nOcclusionQueries)
		return 0; // Error!
	OcclusionQuery *pOcclusionQuery = m_ppOcclusionQueries[cHierarchyNode.GetID()];

	// We have to wait for the result
	// [HACK] Wy do ne need 'nWaitCount'?? Sometimes the result is never available?? (check this in detail...)
	uint32 nWaitCount = 0;
	uint32 nNumOfVisibleFragments;
	while (!pOcclusionQuery->PullOcclusionQuery(&nNumOfVisibleFragments) && nWaitCount<10) {
		m_sStatistics.bWaitForQueryResult = true;
		nWaitCount++;
	}
	m_nCurrentQueries--;
	return nNumOfVisibleFragments;
}

/**
*  @brief
*    Checks whether a hierarchy node is within the view frustum
*/
bool SQCull::InsideViewFrustum(const SceneHierarchyNode &cHierarchyNode, bool &bIntersects, bool bCheckNear)
{
	// Fast escape test: If the parent node was already complete within the frustum, this node is
	// complete within the frustum, too :)
	if (cHierarchyNode.GetParentNode()) {
		if (m_lstTotalVisibility.IsSet(cHierarchyNode.GetParentNode()->GetID())) {
			// Jipi, we are in luck and the parent node was already complete within the frustum!
			m_lstTotalVisibility.Set(cHierarchyNode.GetID());
			bIntersects = false;
			m_sStatistics.nNumOfSkippedFrustumTests++;

			// Done
			return true;
		}
	}

	// Check plane set
	uint32 nClipMask = 0;
	if (cHierarchyNode.CheckPlaneSet(m_cViewFrustum, &nClipMask)) {
		// Near plane intersection?
		if (bCheckNear) {
			// We use the provided clip mask to check for near plane intersection
			bIntersects = (nClipMask & (1 << PlaneSet::VPNear));
			// We can use a extra function for this check...
//			bIntersects = !cHierarchyNode.GetPlaneSide(m_cViewFrustum[0]);
		} else {
			bIntersects = false;
		}

		// Check whether this node is complete within the frustum
		if (!nClipMask)
			m_lstTotalVisibility.Set(cHierarchyNode.GetID());

		// Done
		return true;
	} else {
		return false;
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneQuery functions                   ]
//[-------------------------------------------------------]
bool SQCull::PerformQuery()
{
	// Initializes the visibility container if required...
	VisContainer &cVisContainer = const_cast<VisContainer&>(GetVisContainer());

	// If mode is previous, we just inform the scene query listeners again... :)
	if (m_nMode == Previous) {
		// [TODO] Update this?
//		if (!(GetFlags() & SceneQuery::OnSceneNode))
//			return true; // Not cancelled by the user
		// Inform the scene nodes
/*		Iterator<SceneNode*> cIterator = m_lstVisibleSceneNodes.GetIterator();
		while (cIterator.HasNext()) {
			// Emit signal
			SignalSceneNode(*this, **cIterator.Next());

			InformOnSceneNode(**cIterator.Next());
		}*/

		// Done, not cancelled by the user
		return true;
	}

	// Get the scene container
	SceneContainer &cSceneContainer = GetSceneContainer();

	// Free all nodes (reset visibility information)
	cVisContainer.FreeNodes();

	// World matrix is identity
	cVisContainer.m_mWorldView     = m_mView;
	cVisContainer.m_mWorldViewProj = m_mViewProjection;

	// Get the used renderer
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

	// Initialize the projection information if this is the root container
	if (m_pVisRootContainer == m_pVisContainer) {
		VisContainer::Projection &sProjection = m_pVisContainer->m_sProjection;
		sProjection.cRectangle = cRenderer.GetViewport(&sProjection.fZNear, &sProjection.fZFar);
	}

	// Get the hierarchy we are working on
	m_pHierarchy = cSceneContainer.GetHierarchyInstance();
	if (!m_pHierarchy)
		return true; // Error, but not cancelled by the user

	// Initialize the statistics
	InitStatistics();

	// Get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		// Start process
		const bool bStartProcessResult = pSceneContext->StartProcess();

		// Start stopwatch
		Stopwatch cStopwatch(true);

		// Init visibility & last visited frame states
		ResizeArrays();
		m_lstTotalVisibility.ClearAll();
		m_nCurrentQueries = 0;
		m_bSetIdentityWorldMatrix = true;
		m_nFlags &= ~StopQuery;
		bool bContinue = true;


		// [TODO] Allow viewport scale
	//	cRenderer.SetViewport(Rectangle(0, 0, 100, 100));


		bool bPerformed = false;

		// Is the camera within another container?
	//	m_bCameraInCell = false;
		SceneContainer *pCameraContainer = GetCameraContainer();
		if (pCameraContainer && pCameraContainer != &cSceneContainer) {
			m_bCameraInCell = true;
			// Get next scene container
			SceneContainer *pContainer = pCameraContainer;
			while (pContainer && pContainer->GetContainer() != &GetSceneContainer())
				pContainer = pContainer->GetContainer();
			if (pContainer) {
				// Add the found scene container to the list of visible scene nodes
				VisNode *pNode = m_pVisContainer->AddSceneNode(*pContainer, 0.0f);
				if (pNode) {
					// Set the world transform matrix
					pNode->SetWorldMatrix(Matrix3x4::Identity);

					// Inform the scene node
					pContainer->OnAddedToVisibilityTree(*pNode);

					// Set the projection information
					VisContainer::Projection &sNewProjection = static_cast<VisContainer*>(pNode)->m_sProjection;
					VisContainer::Projection &sProjection    = m_pVisContainer->m_sProjection;
					sNewProjection.cRectangle = sProjection.cRectangle;
					sNewProjection.fZNear     = sProjection.fZNear;
					sNewProjection.fZFar      = sProjection.fZFar;

					// Recursion
					SQCull *pCullQuery = const_cast<SQCull*>(static_cast<const VisContainer*>(pNode)->GetCullQuery());
					if (pCullQuery) {
						pCullQuery->SetMode(GetMode());
						pCullQuery->SetVisibilityThreshold(GetVisibilityThreshold());
						pCullQuery->SetCameraContainer(pCameraContainer);
						pCullQuery->m_bCameraInCell   = m_bCameraInCell;
						pCullQuery->m_vCameraPosition = m_vCameraPosition;
						pCullQuery->m_cViewFrustum    = m_cViewFrustum;

						// [TODO] Cleanup
						pCullQuery->m_mProjection     = m_mProjection;
						pCullQuery->m_mView			  = m_mView;
						pCullQuery->m_mViewProjection = m_mViewProjection;

						bContinue = pCullQuery->PerformQuery();
						bPerformed = true;
					}
				}
			}
		}


		if (!bPerformed) {
			// Use the desired render mode
			if (m_nMode == Frustum) {
				bContinue = CullFrustum();
			} else if (m_nMode == StopAndWait || m_nMode == Coherent) {
				// [TODO] Make those settings configurable?
				const float fSlopeScaleDepthBias	= -0.1f;
				const float fDepthBias				= -0.1f;

				// Set render states
				cRenderer.GetRendererContext().GetEffectManager().Use();
				cRenderer.SetColorMask(false, false, false, false);
				cRenderer.SetRenderState(RenderState::SlopeScaleDepthBias, Tools::FloatToUInt32(fSlopeScaleDepthBias));
				cRenderer.SetRenderState(RenderState::DepthBias,		   Tools::FloatToUInt32(fDepthBias));

				// Set solid fill mode, if we use occlusion culling, for instance point rendering
				// isn't THAT efficient! ;-)
				const uint32 nFixedFillModeT = cRenderer.GetRenderState(RenderState::FixedFillMode);
				cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Solid);

				// Create occlusion queries
				if (!m_ppOcclusionQueries) {
					m_nOcclusionQueries = m_pHierarchy->GetTotalNumOfNodes();
					m_ppOcclusionQueries = new OcclusionQuery*[m_nOcclusionQueries];
					for (uint32 i=0; i<m_nOcclusionQueries; i++)
						m_ppOcclusionQueries[i] = cRenderer.CreateOcclusionQuery();
				}

				// Perform culling
				if (m_nMode == StopAndWait)
					bContinue = CullStopAndWait();
				else
					bContinue = CullCoherentWithQueue();

				// Reset render states
				cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeT);
				cRenderer.SetColorMask();
			}
		}


		// [TODO]
	//	cRenderer.SetViewport();


		// Increase frame counter
		m_nFrame++;

		// Update the statistics
		m_sStatistics.nNumOfVisibleSceneNodes = cVisContainer.m_lstNodes.GetNumOfElements();

		// Get culling time
		m_sStatistics.nCullTime = static_cast<uint64>(cStopwatch.GetMilliseconds());

		// Reset hierarchy pointer - just for sure :)
		m_pHierarchy = nullptr;

		// End process (but only if we started it :)
		if (bStartProcessResult)
			pSceneContext->EndProcess();

		// Done
		return bContinue;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
