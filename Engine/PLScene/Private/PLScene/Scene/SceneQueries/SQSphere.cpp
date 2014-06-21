/*********************************************************\
 *  File: SQSphere.cpp                                   *
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
#include "PLScene/Scene/SNCellPortal.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneHierarchy.h"
#include "PLScene/Scene/SceneHierarchyNode.h"
#include "PLScene/Scene/SceneHierarchyNodeItem.h"
#include "PLScene/Scene/SceneQueries/SQSphere.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SQSphere)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SQSphere::SQSphere()
{
}

/**
*  @brief
*    Destructor
*/
SQSphere::~SQSphere()
{
}

/**
*  @brief
*    Returns the used sphere
*/
Sphere &SQSphere::GetSphere()
{
	return m_cSphere;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Recursive part of PerformQuery()
*/
bool SQSphere::PerformQueryRec(SceneHierarchyNode &cHierarchyNode)
{
	// Is this scene hierarchy node intersecting the sphere?
	if (!Intersect::SphereAABox(m_cSphere, cHierarchyNode.GetAABoundingBox()))
		return true; // Continue the query

	// Touch this node
	cHierarchyNode.Touch();

	// Get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		// Inform all listeners
		const SceneHierarchyNodeItem *pItem = cHierarchyNode.GetFirstItem();
		while (pItem) {
			// Get the linked scene node
			SceneNode *pSceneNode = pItem->GetSceneNode();

			// Was this scene node already processed?
			if (pSceneNode && !pSceneContext->IsNodeTouched(*pSceneNode)) {
				// Check scene node
				if (Intersect::SphereAABox(m_cSphere, pSceneNode->GetContainerAABoundingBox())) {
					// Touch this node
					pSceneContext->TouchNode(*pSceneNode);

					// Emit signal
					SignalSceneNode(*this, *pSceneNode);
					if (m_nFlags & StopQuery)
						return false; // Stop the query right now

					// Continue recursive?
					if (m_nFlags & Recursive) {
						// Is this a container and is recursion allowed?
						if (pSceneNode->IsContainer() && !(pSceneNode->GetFlags() & SceneContainer::NoRecursion)) {
							// Backup current sphere
							const Sphere cSphere = m_cSphere;

							// Transform the sphere into container space
							m_cSphere.SetPos(pSceneNode->GetTransform().GetInverseMatrix()*m_cSphere.GetPos());
							// Now comes the tricky part - the radius. We MUST assume that this is
							// no uniform scale. :(
							m_cSphere.SetRadius(pSceneNode->GetTransform().GetInverseMatrix().RotateVector(Vector3(m_cSphere.GetRadius(), m_cSphere.GetRadius(), m_cSphere.GetRadius())).GetLength());

							// Container recursion
							const bool bContinue = PerformQueryRec(static_cast<SceneContainer*>(pSceneNode)->GetHierarchyInstance()->GetRootNode());

							// Restore sphere
							m_cSphere = cSphere;

							// Stop the query right now?
							if (!bContinue)
								return false;

						// Is this a cell-portal?
						} else if (pSceneNode->IsPortal() && pSceneNode->IsInstanceOf("PLScene::SNCellPortal") && !(pSceneNode->GetFlags() & SNCellPortal::NoPassThrough)) {
							// Get the target cell
							SNCellPortal   &cCellPortal	= static_cast<SNCellPortal&>(*pSceneNode);
							SceneContainer *pCell		= reinterpret_cast<SceneContainer*>(cCellPortal.GetTargetCellInstance());
							if (pCell && pCell != pSceneNode->GetContainer()) {
								// Backup current sphere
								const Sphere cSphere = m_cSphere;

								// Transform the sphere into container space
								m_cSphere.SetPos(cCellPortal.GetWarpMatrix()*m_cSphere.GetPos());
								// Now comes the tricky part - the radius. We MUST assume that this is
								// no uniform scale. :(
								m_cSphere.SetRadius(cCellPortal.GetWarpMatrix().RotateVector(Vector3(m_cSphere.GetRadius(), m_cSphere.GetRadius(), m_cSphere.GetRadius())).GetLength());

								// Container recursion
								const bool bContinue = PerformQueryRec(pCell->GetHierarchyInstance()->GetRootNode());

								// Restore sphere
								m_cSphere = cSphere;

								// Stop the query right now?
								if (!bContinue)
									return false;
							}
						}
					}
				}
			}

			// Next item, please
			pItem = pItem->GetNextItem();
		}
	}

	// Check all sub-hierarchies
	for (uint32 i=0; i<cHierarchyNode.GetNumOfNodes(); i++) {
		if (!PerformQueryRec(*cHierarchyNode.GetNode(i)))
			return false; // Stop the query right now
	}

	// Done, continue the query
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual SceneQuery functions                   ]
//[-------------------------------------------------------]
bool SQSphere::PerformQuery()
{
	// Get the hierarchy we are working on
	const SceneHierarchy *pHierarchy = GetSceneContainer().GetHierarchyInstance();
	if (!pHierarchy)
		return true; // Error, but not cancelled by the user

	// Perform the query recursive...
	m_nFlags &= ~StopQuery;

	// ... get the scene context
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		pSceneContext->StartProcess();
		const bool bResult = PerformQueryRec(pHierarchy->GetRootNode());
		pSceneContext->EndProcess();

		// Done
		return bResult;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
