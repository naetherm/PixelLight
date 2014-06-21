/*********************************************************\
 *  File: SceneHierarchyNode.cpp                         *
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
#include <PLMath/Plane.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneHierarchy.h"
#include "PLScene/Scene/SceneHierarchyNodeItem.h"
#include "PLScene/Scene/SceneHierarchyNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the scene context the scene hierarchy node is in
*/
SceneContext *SceneHierarchyNode::GetSceneContext() const
{
	return m_pHierarchy ? m_pHierarchy->GetSceneContainer().GetSceneContext() : nullptr;
}

/**
*  @brief
*    Returns the owner scene hierarchy
*/
SceneHierarchy *SceneHierarchyNode::GetHierarchy() const
{
	return m_pHierarchy;
}

/**
*  @brief
*    Returns the parent scene hierarchy node
*/
SceneHierarchyNode *SceneHierarchyNode::GetParentNode() const
{
	return m_pParentNode;
}

/**
*  @brief
*    Returns the unique ID of the scene hierarchy node
*/
uint32 SceneHierarchyNode::GetID() const
{
	return m_nID;
}

/**
*  @brief
*    Returns the level/tree depth of the scene hierarchy node
*/
uint8 SceneHierarchyNode::GetLevel() const
{
	return m_nLevel;
}

/**
*  @brief
*    Returns the axis aligned bounding box of the scene hierarchy node
*/
const AABoundingBox &SceneHierarchyNode::GetAABoundingBox() const
{
	return m_cAABoundingBox;
}

/**
*  @brief
*    Checks whether the scene hierarchy node volume is intersecting the given line
*/
bool SceneHierarchyNode::CheckLine(const Line &cLine) const
{
	return Intersect::AABoxLine(m_cAABoundingBox.vMin, m_cAABoundingBox.vMax, cLine);
}

/**
*  @brief
*    Checks whether the scene hierarchy node volume is within the given plane set
*/
bool SceneHierarchyNode::CheckPlaneSet(const PlaneSet &cPlaneSet, uint32 *pnOutClipMask) const
{
	return Intersect::PlaneSetAABox(cPlaneSet, m_cAABoundingBox.vMin, m_cAABoundingBox.vMax, pnOutClipMask);
}

/**
*  @brief
*    Returns the side of the plane the scene hierarchy node volume is on
*/
char SceneHierarchyNode::GetPlaneSide(const Plane &cPlane) const
{
	Vector3 vVertex[8];
	uint8 nInFront = 0;
	uint8 nInBack  = 0;

	// Check plane side
	m_cAABoundingBox.GetVertices(vVertex);
	for (uint8 i=0; i<8; i++) {
		if (cPlane.GetDistance(vVertex[i]) > 0.0f)
			nInFront++;
		else
			nInBack++;
	}

	// Return plane side
	if (nInFront == 8)
		return 1;
	else if (nInBack  == 8)
		return -1;
	else
		return 0;
}

/**
*  @brief
*    Returns the shortest distance from a given point to the scene hierarchy node volume
*/
float SceneHierarchyNode::GetShortestDistance(const Vector3 &vPoint) const
{
	// Get vector from viewpoint to center of bounding volume
	Vector3 vV = m_cAABoundingBox.GetCenter()-vPoint;

	// Compute distance from nearest point to viewpoint
	vV = m_cAABoundingBox.GetVertex(m_cAABoundingBox.GetNearestVertexIndex(vV))-vPoint;
	return vV.GetSquaredLength();
}

/**
*  @brief
*    Returns the number of scene hierarchy node items
*/
uint32 SceneHierarchyNode::GetNumOfItems() const
{
	return m_nNumOfItems;
}

/**
*  @brief
*    Returns the first scene hierarchy node item
*/
SceneHierarchyNodeItem *SceneHierarchyNode::GetFirstItem() const
{
	return m_pFirstItem;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneHierarchyNode::SceneHierarchyNode(SceneHierarchy &cHierarchy) :
	m_pHierarchy(&cHierarchy),
	m_pParentNode(nullptr),
	m_nLevel(0),
	m_nNumOfItems(0),
	m_pFirstItem(nullptr)
{
	// Get an unique ID
	m_nID = m_pHierarchy->m_nTotalNumOfNodes;

	// Update the total number of scene hierarchy nodes
	m_pHierarchy->m_nTotalNumOfNodes++;
}

/**
*  @brief
*    Destructor
*/
SceneHierarchyNode::~SceneHierarchyNode()
{
	// Update the total number of scene hierarchy nodes and free the unique node ID
	m_pHierarchy->m_nTotalNumOfNodes--;

	// Delete the items
	DeleteItems();
}

/**
*  @brief
*    Returns a free scene hierarchy node
*/
SceneHierarchyNode &SceneHierarchyNode::GetFreeNode()
{
	if (m_pHierarchy->m_lstFreeNodes.IsEmpty()) {
		// Create a new node
		return m_pHierarchy->CreateNode();
	} else {
		// Get the node and remove it from the list of free nodes
		SceneHierarchyNode *pNode = m_pHierarchy->m_lstFreeNodes.Get(0);
		m_pHierarchy->m_lstFreeNodes.RemoveAtIndex(0);

		// Initialize the variables - just for sure
		pNode->Init();

		// Return the node
		return *pNode;
	}
}

/**
*  @brief
*    Marks a scene hierarchy node as free
*/
void SceneHierarchyNode::FreeNode(SceneHierarchyNode &cNode)
{
	m_pHierarchy->m_lstFreeNodes.Add(&cNode);
}


//[-------------------------------------------------------]
//[ Protected virtual SceneHierarchyNode functions        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the scene hierarchy node variables
*/
void SceneHierarchyNode::Init()
{
//	m_pHierarchy	 = nullptr;	// Do not touch!
//	m_nID			 = 0;		// Do not touch!
	m_nLevel		 = 0;
//	m_cAABoundingBox = ?;		// We do not need touch this
	m_pParentNode	 = nullptr;
	m_nNumOfItems	 = 0;
	m_pFirstItem	 = nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a scene node to this scene hierarchy node
*/
void SceneHierarchyNode::AddSceneNode(SceneNode &cSceneNode)
{
	// Create a new item
	SceneHierarchyNodeItem *pItem = new SceneHierarchyNodeItem();

	// Link the scene node to this new item
	pItem->Link(cSceneNode);

	// Attach the new item to this scene hierarchy node
	pItem->Attach(*this);
}

/**
*  @brief
*    Deletes all items attached to this scene hierarchy node
*/
void SceneHierarchyNode::DeleteItems()
{
	while (m_pFirstItem) {
		SceneHierarchyNodeItem *pItem = m_pFirstItem;
		m_pFirstItem = m_pFirstItem->m_pNextSceneHierarchyNodeItem;
		delete pItem;
	}
	m_nNumOfItems = 0;
}

/**
*  @brief
*    Draws the hierarchy node (for debugging)
*/
void SceneHierarchyNode::Draw(Renderer &cRenderer, const Color4 &cColor, const Matrix4x4 &mWorldViewProjection, float fLineWidth) const
{
	// Draw the bounding box of this node
	cRenderer.GetDrawHelpers().DrawBox(cColor, m_cAABoundingBox.vMin, m_cAABoundingBox.vMax, mWorldViewProjection, fLineWidth);

	// Draw the child nodes
	for (uint32 i=0; i<GetNumOfNodes(); i++)
		GetNode(i)->Draw(cRenderer, cColor, mWorldViewProjection, fLineWidth);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
