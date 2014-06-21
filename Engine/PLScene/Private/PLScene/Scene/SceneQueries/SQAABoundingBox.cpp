/*********************************************************\
 *  File: SQAABoundingBox.cpp                            *
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
#include "PLScene/Scene/SceneQueries/SQAABoundingBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SQAABoundingBox)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SQAABoundingBox::SQAABoundingBox()
{
}

/**
*  @brief
*    Destructor
*/
SQAABoundingBox::~SQAABoundingBox()
{
}

/**
*  @brief
*    Returns the used axis aligned bounding box
*/
AABoundingBox &SQAABoundingBox::GetAABoundingBox()
{
	return m_cAABoundingBox;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Recursive part of PerformQuery()
*/
bool SQAABoundingBox::PerformQueryRec(SceneHierarchyNode &cHierarchyNode)
{
	// Is this scene hierarchy node intersecting the axis aligned bounding box?
	if (!Intersect::AABoxAABox(cHierarchyNode.GetAABoundingBox(), m_cAABoundingBox))
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
				if (Intersect::AABoxAABox(pSceneNode->GetContainerAABoundingBox(), m_cAABoundingBox)) {
					// Touch the node
					pSceneContext->TouchNode(*pSceneNode);

					// Emit signal
					SignalSceneNode(*this, *pSceneNode);
					if (m_nFlags & StopQuery)
						return false; // Stop the query right now

					// Continue recursive?
					if (m_nFlags & Recursive) {
						// Is this a container and is recursion allowed?
						if (pSceneNode->IsContainer() && !(pSceneNode->GetFlags() & SceneContainer::NoRecursion)) {
							// Backup current axis aligned bounding box
							const AABoundingBox cAABoundingBox = m_cAABoundingBox;

							// Transform the axis aligned bounding box into container space
							const Vector3 &vMin = cAABoundingBox.vMin, &vMax = cAABoundingBox.vMax;
							const Matrix3x4 &mTrans = pSceneNode->GetTransform().GetInverseMatrix();
							// Transform ALL eight corners bounding box corners into 'scene container space'
							// and get min/max values of the 'scene container space' bounding box
							Vector3 vT = vMin;
							vT *= mTrans;
							m_cAABoundingBox.vMin = m_cAABoundingBox.vMax = vT;
							vT.SetXYZ(vMax.x, vMin.y, vMin.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);
							vT.SetXYZ(vMax.x, vMax.y, vMin.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);
							vT.SetXYZ(vMin.x, vMax.y, vMin.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);
							vT.SetXYZ(vMin.x, vMin.y, vMax.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);
							vT.SetXYZ(vMax.x, vMin.y, vMax.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);
							vT.SetXYZ(vMax.x, vMax.y, vMax.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);
							vT.SetXYZ(vMin.x, vMax.y, vMax.z);
							vT *= mTrans;
							m_cAABoundingBox.AppendToCubicHull(vT);

							// Container recursion
							const bool bContinue = PerformQueryRec(static_cast<SceneContainer*>(pSceneNode)->GetHierarchyInstance()->GetRootNode());

							// Restore axis aligned bounding box
							m_cAABoundingBox = cAABoundingBox;

							// Stop the query right now?
							if (!bContinue)
								return false;

						// Is this a cell-portal?
						} else if (pSceneNode->IsPortal() && pSceneNode->IsInstanceOf("PLScene::SNCellPortal") && !(pSceneNode->GetFlags() & SNCellPortal::NoPassThrough)) {
							// Get the target cell
							SNCellPortal   &cCellPortal	= static_cast<SNCellPortal&>(*pSceneNode);
							SceneContainer *pCell		= reinterpret_cast<SceneContainer*>(cCellPortal.GetTargetCellInstance());
							if (pCell && pCell != pSceneNode->GetContainer()) {
								// Backup current axis aligned bounding box
								const AABoundingBox cAABoundingBox = m_cAABoundingBox;

								// Transform the axis aligned bounding box into container space
								const Vector3 &vMin = cAABoundingBox.vMin, &vMax = cAABoundingBox.vMax;
								const Matrix3x4 &mTrans = cCellPortal.GetWarpMatrix();
								// Transform ALL eight corners bounding box corners into 'scene container space'
								// and get min/max values of the 'scene container space' bounding box
								Vector3 vT = vMin;
								vT *= mTrans;
								m_cAABoundingBox.vMin = m_cAABoundingBox.vMax = vT;
								vT.SetXYZ(vMax.x, vMin.y, vMin.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);
								vT.SetXYZ(vMax.x, vMax.y, vMin.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);
								vT.SetXYZ(vMin.x, vMax.y, vMin.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);
								vT.SetXYZ(vMin.x, vMin.y, vMax.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);
								vT.SetXYZ(vMax.x, vMin.y, vMax.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);
								vT.SetXYZ(vMax.x, vMax.y, vMax.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);
								vT.SetXYZ(vMin.x, vMax.y, vMax.z);
								vT *= mTrans;
								m_cAABoundingBox.AppendToCubicHull(vT);

								// Container recursion
								const bool bContinue = PerformQueryRec(pCell->GetHierarchyInstance()->GetRootNode());

								// Restore axis aligned bounding box
								m_cAABoundingBox = cAABoundingBox;

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
bool SQAABoundingBox::PerformQuery()
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
