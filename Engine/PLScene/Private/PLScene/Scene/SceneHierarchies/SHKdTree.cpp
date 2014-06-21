/*********************************************************\
 *  File: SHKdTree.cpp                                   *
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
#include "PLScene/Scene/SceneNode.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneHierarchyNodeItem.h"
#include "PLScene/Scene/SceneHierarchies/SHKdTree.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SHKdTree)




//[-------------------------------------------------------]
//[ SHKdTree implementation                               ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SHKdTree::SHKdTree()
{
	m_pRootNode = new SHKdTreeNode(*this);
}


//[-------------------------------------------------------]
//[ Private virtual SceneHierarchy functions              ]
//[-------------------------------------------------------]
SceneHierarchyNode &SHKdTree::CreateNode()
{
	return *(new SHKdTreeNode(*this));
}



//[-------------------------------------------------------]
//[ SHKdTreeNode implementation                           ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SHKdTreeNode::SHKdTreeNode(SceneHierarchy &cHierarchy) : SceneHierarchyNode(cHierarchy),
	m_pLeftNode(nullptr),
	m_pRightNode(nullptr),
	m_nSplitAxis(Leaf),
	m_fSplitValue(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SHKdTreeNode::~SHKdTreeNode()
{
	// Destroy the child nodes
	if (m_pLeftNode)
		delete m_pLeftNode;
	if (m_pRightNode)
		delete m_pRightNode;
}

/**
*  @brief
*    Merges the items of the scene hierarchy child nodes to this nodes if required
*/
void SHKdTreeNode::Merge()
{
	// If this is a leaf or the child nodes are no leafs or this node can't hold old items of the child nodes,
	// we can't perform merge!
	if (m_nSplitAxis != Leaf && m_pLeftNode->m_nSplitAxis == Leaf && m_pRightNode->m_nSplitAxis == Leaf &&
		m_pLeftNode->m_nNumOfItems+m_pRightNode->m_nNumOfItems < m_pHierarchy->GetMaxNumOfNodeItems()) {
		// Get the scene context
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext) {
			// Start process which is used to check whether a node was already touched
			pSceneContext->StartProcess();

			// Process left child node
			while (m_pLeftNode->m_pFirstItem) {
				// Detach the first item of the child node...
				SceneHierarchyNodeItem *pItem = m_pLeftNode->m_pFirstItem;
				pItem->Detach();

				// ... update the double counter of the scene node...
				if (pItem->GetSceneNode() && pSceneContext->TouchNode(*pItem->GetSceneNode())) {
					// ... and finally attach the item to this node
					pItem->Attach(*this);
				} else {
					// Hm, this item is NOT required...
					delete pItem;
				}
			}

			// Process right child node
			while (m_pRightNode->m_pFirstItem) {
				// Detach the first item of the child node...
				SceneHierarchyNodeItem *pItem = m_pRightNode->m_pFirstItem;
				pItem->Detach();

				// ... update the double counter of the scene node...
				if (pItem->GetSceneNode() && pSceneContext->TouchNode(*pItem->GetSceneNode())) {
					// ... and finally attach the item to this node
					pItem->Attach(*this);
				} else {
					// This item is NOT required...
					delete pItem;
				}
			}

			// End process
			pSceneContext->EndProcess();

			// This node now becomes a leaf...
			FreeNode(*m_pLeftNode);
			m_pLeftNode = nullptr;
			FreeNode(*m_pRightNode);
			m_pRightNode = nullptr;
			m_nSplitAxis = Leaf;
		}
	}
}

/**
*  @brief
*    Splits this scene hierarchy node
*/
void SHKdTreeNode::Split()
{
	// Create the child nodes
	m_pLeftNode  = static_cast<SHKdTreeNode*>(&GetFreeNode());
	m_pRightNode = static_cast<SHKdTreeNode*>(&GetFreeNode());
	m_pLeftNode->m_pParentNode = m_pRightNode->m_pParentNode = this;
	m_pLeftNode->m_nLevel = m_pRightNode->m_nLevel = m_nLevel + 1;

	// Get the split axis
	const float fSize[3] = { m_cAABoundingBox.GetWidth(), m_cAABoundingBox.GetHeight(), m_cAABoundingBox.GetDepth() };
	m_nSplitAxis = static_cast<EAxis>(m_cAABoundingBox.GetLongestAxis());

	// Get the split value
	m_fSplitValue = m_cAABoundingBox.vMin[m_nSplitAxis] + fSize[m_nSplitAxis]*0.5f;

	// Set the axis aligned bounding boxes of the child nodes
	m_pLeftNode->m_cAABoundingBox = m_pRightNode->m_cAABoundingBox = m_cAABoundingBox;
	m_pLeftNode->m_cAABoundingBox.vMax[m_nSplitAxis] = m_pRightNode->m_cAABoundingBox.vMin[m_nSplitAxis] = m_fSplitValue;
}


//[-------------------------------------------------------]
//[ Public virtual SceneHierarchyNode functions           ]
//[-------------------------------------------------------]
void SHKdTreeNode::Touch(bool bRecursive)
{
	// Distribute the items of the scene hierarchy node to child nodes if required

	// Split if required
	if (m_nLevel < m_pHierarchy->GetMaxLevel() &&
		(m_nSplitAxis != Leaf || m_nNumOfItems > m_pHierarchy->GetMaxNumOfNodeItems())) {
		// If this is a leaf, the maximum allowed number of items for a node is reached and
		// we need to split this leaf right now
		if (m_nSplitAxis == Leaf)
			Split();

		// If there are any items, distribute them to the child nodes
		if (m_pFirstItem) {
			// Distribute the items
			while (m_pFirstItem) {
				// Detach the first item from this node
				SceneHierarchyNodeItem *pItem1 = m_pFirstItem;
				pItem1->Detach();

				// Get center and size if the linked scene node
				if (pItem1->GetSceneNode()) {
					const AABoundingBox &cAABox = pItem1->GetSceneNode()->GetContainerAABoundingBox();
					const float fSize   = cAABox.vMax[m_nSplitAxis]-cAABox.vMin[m_nSplitAxis];
					const float fCenter = cAABox.vMin[m_nSplitAxis]+fSize;

					// Put the item into one (or both) of the child nodes
					if (fCenter+fSize < m_fSplitValue) {
						// Attach to the left child node
						if (m_pLeftNode)
							pItem1->Attach(*m_pLeftNode);
					} else if (fCenter-fSize > m_fSplitValue) {
						// Attach to the right child node
						if (m_pRightNode)
							pItem1->Attach(*m_pRightNode);
					} else {
						// Clone this item and attach if to both child nodes
						SceneHierarchyNodeItem &cItem2 = pItem1->Clone();
						if (m_pLeftNode)
							pItem1->Attach(*m_pLeftNode);
						if (m_pRightNode)
							cItem2. Attach(*m_pRightNode);
					}
				} else {
					// Hm, this item is NOT required...
					delete pItem1;
				}
			}
		}

		// Touch recursive?
		if (bRecursive) {
			m_pLeftNode-> Touch(bRecursive);
			m_pRightNode->Touch(bRecursive);
		}
	}

	// Merge
	Merge();
}

uint32 SHKdTreeNode::GetNumOfNodes() const
{
	return (m_nSplitAxis == Leaf) ? 0 : 2;
}

SceneHierarchyNode *SHKdTreeNode::GetNode(uint32 nIndex) const
{
	// Check the given index
	if (nIndex > 1)
		return nullptr; // Error!

	// Return the requested child node
	return nIndex ? m_pRightNode : m_pLeftNode;
}


//[-------------------------------------------------------]
//[ Private virtual SceneHierarchyNode functions          ]
//[-------------------------------------------------------]
void SHKdTreeNode::Init()
{
	// Call base implementation
	SceneHierarchyNode::Init();

	// Initialize the data
	m_pLeftNode   = nullptr;
	m_pRightNode  = nullptr;
	m_nSplitAxis  = Leaf;
	m_fSplitValue = 0.0f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
