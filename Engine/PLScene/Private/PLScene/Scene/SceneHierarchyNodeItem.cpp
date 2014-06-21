/*********************************************************\
 *  File: SceneHierarchyNodeItem.cpp                     *
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
#include "PLScene/Scene/SceneHierarchyNode.h"
#include "PLScene/Scene/SceneHierarchyNodeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the scene node this scene hierarchy node item is linked to
*/
SceneNode *SceneHierarchyNodeItem::GetSceneNode() const
{
	return m_pSceneNode;
}

/**
*  @brief
*    Returns the scene hierarchy node this scene hierarchy node item is attached to
*/
SceneHierarchyNode *SceneHierarchyNodeItem::GetHierarchyNode() const
{
	return m_pSceneHierarchyNode;
}

/**
*  @brief
*    Returns the previous scene hierarchy node item
*/
SceneHierarchyNodeItem *SceneHierarchyNodeItem::GetPreviousItem() const
{
	return m_pPreviousSceneHierarchyNodeItem;
}

/**
*  @brief
*    Returns the next scene hierarchy node item
*/
SceneHierarchyNodeItem *SceneHierarchyNodeItem::GetNextItem() const
{
	return m_pNextSceneHierarchyNodeItem;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneHierarchyNodeItem::SceneHierarchyNodeItem() :
	m_pSceneNode(nullptr),
	m_pPreviousSceneNodeItem(nullptr),
	m_pNextSceneNodeItem(nullptr),
	m_pSceneHierarchyNode(nullptr),
	m_pPreviousSceneHierarchyNodeItem(nullptr),
	m_pNextSceneHierarchyNodeItem(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SceneHierarchyNodeItem::~SceneHierarchyNodeItem()
{
	// Detach from scene hierarchy node
	Detach();

	// Unlink from scene node
	Unlink();
}

/**
*  @brief
*    Links this scene hierarchy node item with a scene node
*/
bool SceneHierarchyNodeItem::Link(SceneNode &cSceneNode)
{
	// Is this scene hierarchy node item already attached to a scene node and is the given parameter valid?
	if (m_pSceneNode) {
		// Error!
		return false;
	} else {
		// Set scene node scene hierarchy node item hierarchy pointer (uff :)
		m_pSceneNode = &cSceneNode;

		// Update scene node item connectivity (this is now the first item of the scene node)
		m_pPreviousSceneNodeItem = nullptr;
		m_pNextSceneNodeItem = m_pSceneNode->m_pFirstSceneHierarchyNodeItem;
		if (m_pNextSceneNodeItem)
			m_pNextSceneNodeItem->m_pPreviousSceneNodeItem = this;
		m_pSceneNode->m_pFirstSceneHierarchyNodeItem = this;

		// Done
		return true;
	}
}

/**
*  @brief
*    Unlinks this scene hierarchy node item from the scene node
*/
bool SceneHierarchyNodeItem::Unlink()
{
	// Is this item currently linked to a scene node?
	if (!m_pSceneNode)
		return false; // Error!

	// Check whether this was the first scene hierarchy node item
	if (m_pSceneNode->m_pFirstSceneHierarchyNodeItem == this)
		m_pSceneNode->m_pFirstSceneHierarchyNodeItem = m_pNextSceneNodeItem;

	// Reset scene node pointer
	m_pSceneNode = nullptr;

	// Update scene hierarchy node item connectivity
	if (m_pPreviousSceneNodeItem)
		m_pPreviousSceneNodeItem->m_pNextSceneNodeItem = m_pNextSceneNodeItem;
	if (m_pNextSceneNodeItem)
		m_pNextSceneNodeItem->m_pPreviousSceneNodeItem = m_pPreviousSceneNodeItem;
	m_pPreviousSceneNodeItem = nullptr;
	m_pNextSceneNodeItem     = nullptr;

	// Done
	return true;
}

/**
*  @brief
*    Attaches this scene hierarchy node item to a scene hierarchy node
*/
bool SceneHierarchyNodeItem::Attach(SceneHierarchyNode &cSceneHierarchyNode)
{
	// Is this scene hierarchy node item already attached to a scene hierarchy node and is the given parameter valid?
	if (m_pSceneHierarchyNode) {
		// Error!
		return false;
	} else {
		// Set scene hierarchy node pointer
		m_pSceneHierarchyNode = &cSceneHierarchyNode;

		// Update scene hierarchy node item connectivity (this is now the first item of the scene hierarchy node)
		m_pPreviousSceneHierarchyNodeItem = nullptr;
		m_pNextSceneHierarchyNodeItem = m_pSceneHierarchyNode->m_pFirstItem;
		if (m_pNextSceneHierarchyNodeItem)
			m_pNextSceneHierarchyNodeItem->m_pPreviousSceneHierarchyNodeItem = this;
		m_pSceneHierarchyNode->m_pFirstItem = this;

		// Update the number of scene hierarchy node items
		m_pSceneHierarchyNode->m_nNumOfItems++;

		// Done
		return true;
	}
}

/**
*  @brief
*    Detaches this scene hierarchy node item from it's scene hierarchy node
*/
bool SceneHierarchyNodeItem::Detach()
{
	// Is this item currently attached to a scene hierarchy node?
	if (!m_pSceneHierarchyNode)
		return false; // Error!

	// Update the number of scene hierarchy node items
	m_pSceneHierarchyNode->m_nNumOfItems--;

	// Check whether this was the first scene hierarchy node item
	if (m_pSceneHierarchyNode->m_pFirstItem == this)
		m_pSceneHierarchyNode->m_pFirstItem = m_pNextSceneHierarchyNodeItem;

	// Reset scene hierarchy node pointer
	m_pSceneHierarchyNode = nullptr;

	// Update scene hierarchy node item connectivity
	if (m_pPreviousSceneHierarchyNodeItem)
		m_pPreviousSceneHierarchyNodeItem->m_pNextSceneHierarchyNodeItem = m_pNextSceneHierarchyNodeItem;
	if (m_pNextSceneHierarchyNodeItem)
		m_pNextSceneHierarchyNodeItem->m_pPreviousSceneHierarchyNodeItem = m_pPreviousSceneHierarchyNodeItem;
	m_pPreviousSceneHierarchyNodeItem = nullptr;
	m_pNextSceneHierarchyNodeItem     = nullptr;

	// Done
	return true;
}

/**
*  @brief
*    Returns a clone of this scene hierarchy node item
*/
SceneHierarchyNodeItem &SceneHierarchyNodeItem::Clone() const
{
	// Create the clone
	SceneHierarchyNodeItem *pClone = new SceneHierarchyNodeItem();

	// Link the clone
	pClone->Link(*m_pSceneNode);

	// Return the clone
	return *pClone;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
