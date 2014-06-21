/*********************************************************\
 *  File: Picking.cpp                                    *
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
#include <PLMath/Intersect.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneQueries/SQLine.h>
#include "PLEngine/Picking/PickingResult.h"
#include "PLEngine/Picking/Picking.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Picking::Picking() :
	EventHandlerSceneNode(&Picking::OnSceneNode, this),
	m_pPickingResult(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Picking::~Picking()
{
}

/**
*  @brief
*    Performs picking by using the given line start and end positions
*/
bool Picking::PerformPicking(PickingResult &cPickingResult, SceneContainer &cContainer, const Vector3 &vLineStartPos, const Vector3 &vLineEndPos, Cull::Enum nCull)
{
	// Initialize the picking result
	m_pPickingResult = &cPickingResult;
	cPickingResult.m_pSceneNode				 = nullptr;
	cPickingResult.m_nGeometry				 = 0;
	cPickingResult.m_nTriangle				 = 0;
	cPickingResult.m_vPoint					 = Vector3::Zero;
	cPickingResult.m_pSceneContainer		 = &cContainer;
	cPickingResult.m_vLineStartPos			 = vLineStartPos;
	cPickingResult.m_fNearestSquaredDistance = -1.0f;

	// Trace line
	SQLine *pQuery = static_cast<SQLine*>(cContainer.CreateQuery("PLScene::SQLine"));
	if (pQuery) {
		pQuery->SignalSceneNode.Connect(EventHandlerSceneNode);
		pQuery->GetLine().Set(vLineStartPos, vLineEndPos);
		pQuery->SetCull(nCull);
		pQuery->PerformQuery();
		cContainer.DestroyQuery(*pQuery);
	}

	// Was something picked?
	if (cPickingResult.m_pSceneNode) {
		// Ensure that the picking distance is not greater than the possible maximum
		if ((vLineEndPos-vLineStartPos).GetSquaredLength() >= cPickingResult.m_fNearestSquaredDistance)
			return (cPickingResult.m_pSceneNode != nullptr);
	}

	// Nothing (in range) was picked
	return false;
}

/**
*  @brief
*    Performs picking using the mesh of the given scene node by using the given line start and end positions
*/
bool Picking::PerformPicking(PickingResult &cPickingResult, SceneNode &cSceneNode, const Vector3 &vLineStartPos, const Vector3 &vLineEndPos,
							 Array<uint32> *plstGeometries, Cull::Enum nCull)
{
	// Initialize the picking result
	m_pPickingResult = &cPickingResult;
	cPickingResult.m_pSceneNode				 = nullptr;
	cPickingResult.m_nGeometry				 = 0;
	cPickingResult.m_nTriangle				 = 0;
	cPickingResult.m_vPoint					 = Vector3::Zero;
	cPickingResult.m_pSceneContainer		 = nullptr;
	cPickingResult.m_vLineStartPos			 = vLineStartPos;
	cPickingResult.m_fNearestSquaredDistance = -1.0f;

	// Perform mesh intersection
	MeshIntersection(cSceneNode, vLineStartPos, vLineEndPos, plstGeometries, nCull);

	// Ensure that the picking distance is not greater than the possible maximum
	if ((vLineEndPos-vLineStartPos).GetSquaredLength() >= cPickingResult.m_fNearestSquaredDistance)
		return (cPickingResult.m_pSceneNode != nullptr);

	// Nothing (in range) was picked
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a scene node was found
*/
void Picking::OnSceneNode(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// First at all, call the picking filter function
	if (m_pPickingResult->m_pSceneContainer && OnPickingCandidate(cSceneNode)) {
		// Is there a mesh handler with a mesh?
		if (cSceneNode.GetMeshHandler() && cSceneNode.GetMeshHandler()->GetMesh()) {
			// We KNOW that it's a SQLine!
			SQLine &cLineQuery = static_cast<SQLine&>(cQuery);

			// Get line start/end position in node space
			const Vector3 vLineStartPos = cSceneNode.GetTransform().GetInverseMatrix()*cLineQuery.GetLine().vStart;
			const Vector3 vLineEndPos   = cSceneNode.GetTransform().GetInverseMatrix()*cLineQuery.GetLine().vEnd;

			// Perform mesh intersection
			MeshIntersection(cSceneNode, vLineStartPos, vLineEndPos, nullptr, cLineQuery.GetCull());
		} else {
			// There's no mesh we can use for an intersection test

			// [TODO] Currently it's impossible to pick something which does not have a polygonal mesh, think
			//        of a way to handle this case properly, especially when dealing with overlapping scene nodes
		}
	}
}

/**
*  @brief
*    Perform mesh intersection
*/
void Picking::MeshIntersection(SceneNode &cSceneNode, const Vector3 &vLineStartPos, const Vector3 &vLineEndPos,
							   Array<uint32> *plstGeometries, Cull::Enum nCull)
{
	// First of all, check the intersection distance against the axis aligned bounding box of the scene node
	float fIntersection;
	const AABoundingBox &cAABoundngBox = cSceneNode.GetAABoundingBox();
	if (Intersect::AABoxLine(cAABoundngBox.vMin, cAABoundngBox.vMax, vLineStartPos, vLineEndPos, &fIntersection)) {
		// Get the mesh handler of the scene node
		MeshHandler *pMeshHandler = cSceneNode.GetMeshHandler();
		if (pMeshHandler) {
			// Transform matrix to bring something into the same container space the original line start position is in
			Matrix3x4 matTransform;
			if (cSceneNode.GetContainer() && m_pPickingResult->m_pSceneContainer)
				cSceneNode.GetContainer()->GetTransformMatrixTo(*m_pPickingResult->m_pSceneContainer, matTransform);

			// Before we perform the expensive triangle intersection test we check the found axis aligned bounding box
			// intersection distance against the current nearest intersection distance... maybe we're in luck and can
			// skip further tests if we detect that no triangle inside the box can be nearer than the current nearest
			// distance...
			if (fIntersection >= 0.0f && m_pPickingResult->m_fNearestSquaredDistance >= 0.0f) {
				// Our start point is outside the axis aligned bounding box of the scene node and there's already a previous picking result available

				// Get intersection point in node space
				Vector3 vIntersectionPos = vLineStartPos + (vLineEndPos-vLineStartPos).Normalize()*fIntersection;

				// Transform intersection point into container space
				vIntersectionPos *= cSceneNode.GetTransform().GetMatrix();

				// Bring the intersection point into the same container space the original line start position is in
				vIntersectionPos *= matTransform;

				// Do the early escape test
				if ((m_pPickingResult->m_vLineStartPos-vIntersectionPos).GetSquaredLength() > m_pPickingResult->m_fNearestSquaredDistance)
					return; // Get us out of this method right now, there's no change that any triangle intersection point is closer as the previous picking result
			} else {
				// Our start point is inside the axis aligned bounding box of the scene node or we have nothing to early escape test against
			}

			// We ran out of possible early escape tests... now find intersection triangle - this may be quite slow...
			Vector3 vCollisionPoint;
			uint32 nTriangle, nGeometry;
			if (pMeshHandler->FindTriangle(vLineStartPos, vLineEndPos, nTriangle, &nGeometry, &vCollisionPoint, plstGeometries, nCull)) {
				// Lookout! If our start point is inside the axis aligned bounding box of the scene node, we need to take care of "backfiring"
				if (fIntersection < 0) {
					// Our start point is inside the axis aligned bounding box of the scene node

					// Get the normalized line direction in scene node space
					Vector3 vLineDirection = vLineEndPos;
					vLineDirection -= vLineStartPos;
					vLineDirection.Normalize();

					// Get the normalized collision point to line start point direction in scene node space
					Vector3 vCollisionPointDirection = vCollisionPoint;
					vCollisionPointDirection -= vLineStartPos;
					vCollisionPointDirection.Normalize();

					// Is the collision point within the correct direction?
					if (vLineDirection.DotProduct(vCollisionPointDirection) < 0)
						return;	// Get us out of this method right now, the found collision point is behind us and so we are not interested in it
				}

				// Backup the node space collision point
				const Vector3 vNodeSpaceCollisionPoint = vCollisionPoint;

				// Transform collision point into container space
				vCollisionPoint *= cSceneNode.GetTransform().GetMatrix();

				// Bring the collision point into the same container space the original line start position is in
				vCollisionPoint *= matTransform;

				// Calculate the squared distance between the two positions (that are now within the same vector space)
				const float fSquaredDistance = (m_pPickingResult->m_vLineStartPos-vCollisionPoint).GetSquaredLength();

				// Is this collision point nearer?
				if (m_pPickingResult->m_fNearestSquaredDistance < 0.0f || m_pPickingResult->m_fNearestSquaredDistance > fSquaredDistance) {
					m_pPickingResult->m_pSceneNode				= &cSceneNode;
					m_pPickingResult->m_nGeometry				= nGeometry;
					m_pPickingResult->m_nTriangle				= nTriangle;
					m_pPickingResult->m_vPoint					= vNodeSpaceCollisionPoint;
					m_pPickingResult->m_fNearestSquaredDistance = fSquaredDistance;
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Picking functions                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function that is called when a picking candidate has been found ("picking filter function")
*/
bool Picking::OnPickingCandidate(SceneNode &cSceneNode)
{
	// Nothing to do by default - just pass through the scene node candidate
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
