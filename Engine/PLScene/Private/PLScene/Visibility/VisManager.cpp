/*********************************************************\
 *  File: VisManager.cpp                                 *
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
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Visibility/VisManager.h"


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
*    Resource cleanup
*/
void VisManager::Cleanup()
{
	// Destroy all nodes
	Iterator<VisNode*> cIterator = m_lstFreeNodes.GetIterator();
	while (cIterator.HasNext())
		delete cIterator.Next();
	m_lstFreeNodes.Clear();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VisManager::VisManager()
{
	PL_LOG(Debug, "Create visibility manager")
}

/**
*  @brief
*    Destructor
*/
VisManager::~VisManager()
{
	PL_LOG(Debug, "Destroy visibility manager")
	Cleanup();
}

/**
*  @brief
*    Returns a free visibility node
*/
VisNode &VisManager::GetFreeNode()
{
	if (m_lstFreeNodes.IsEmpty())
		return *(new VisNode());
	else {
		VisNode *pNode = m_lstFreeNodes.Get(0);
		m_lstFreeNodes.RemoveAtIndex(0);
		return *pNode;
	}
}

/**
*  @brief
*    Frees a visibility node
*/
void VisManager::FreeNode(VisNode &cNode)
{
	m_lstFreeNodes.Add(&cNode);
	cNode.m_pSceneNodeHandler->SetElement();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
