/*********************************************************\
 *  File: SQPoint.cpp                                    *
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
#include "PLScene/Scene/SceneQueries/SQPoint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SQPoint, "PLScene", PLScene::SceneQuery, "Point intersection scene query class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SQPoint)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SQPoint::SQPoint()
{
}

/**
*  @brief
*    Destructor
*/
SQPoint::~SQPoint()
{
}

/**
*  @brief
*    Returns the used point
*/
Vector3 &SQPoint::GetPoint()
{
	return m_vPoint;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Recursive part of PerformQuery()
*/
bool SQPoint::PerformQueryRec(SceneHierarchyNode &cHierarchyNode)
{
	// Is this scene hierarchy node intersecting the point?
	if (!Intersect::AABoxPoint(cHierarchyNode.GetAABoundingBox().vMin,
							   cHierarchyNode.GetAABoundingBox().vMax,
							   m_vPoint))
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
				const AABoundingBox &cAABB = pSceneNode->GetContainerAABoundingBox();
				if (Intersect::AABoxPoint(cAABB.vMin, cAABB.vMax, m_vPoint)) {
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
							// Backup current point
							const Vector3 vPoint = m_vPoint;

							// Transform the point into container space
							m_vPoint *= pSceneNode->GetTransform().GetInverseMatrix();

							// Container recursion
							const bool bContinue = PerformQueryRec(static_cast<SceneContainer*>(pSceneNode)->GetHierarchyInstance()->GetRootNode());

							// Restore point
							m_vPoint = vPoint;

							// Stop the query right now?
							if (!bContinue)
								return false;

						// Is this a cell-portal?
						} else if (pSceneNode->IsPortal() && pSceneNode->IsInstanceOf("PLScene::SNCellPortal") && !(pSceneNode->GetFlags() & SNCellPortal::NoPassThrough)) {
							// Get the target cell
							SNCellPortal   &cCellPortal	= static_cast<SNCellPortal&>(*pSceneNode);
							SceneContainer *pCell		= reinterpret_cast<SceneContainer*>(cCellPortal.GetTargetCellInstance());
							if (pCell && pCell != pSceneNode->GetContainer()) {
								// Backup current point
								const Vector3 vPoint = m_vPoint;

								// Transform the point into container space
								m_vPoint *= cCellPortal.GetWarpMatrix();

								// Container recursion
								const bool bContinue = PerformQueryRec(pCell->GetHierarchyInstance()->GetRootNode());

								// Restore point
								m_vPoint = vPoint;

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
bool SQPoint::PerformQuery()
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
