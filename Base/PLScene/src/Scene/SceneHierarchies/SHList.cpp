/*********************************************************\
 *  File: SHList.cpp                                     *
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
#include "PLScene/Scene/SceneHierarchies/SHList.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SHList, "PLScene", PLScene::SceneHierarchy, "Scene hierarchy which is in fact a simple list (a special case of a tree :)")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SHList)


//[-------------------------------------------------------]
//[ SHList implementation                                 ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SHList::SHList()
{
	m_pRootNode = new SHListNode(*this);
}


//[-------------------------------------------------------]
//[ Private virtual SceneHierarchy functions              ]
//[-------------------------------------------------------]
SceneHierarchyNode &SHList::CreateNode()
{
	return *(new SHListNode(*this));
}




//[-------------------------------------------------------]
//[ SHListNode implementation                             ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public virtual SceneHierarchyNode functions           ]
//[-------------------------------------------------------]
void SHListNode::Touch(bool bRecursive)
{
	// Nothing to do in here :)
}

uint32 SHListNode::GetNumOfNodes() const
{
	// Because this is just a simple list, there are ONLY items in here!
	return 0;
}

SceneHierarchyNode *SHListNode::GetNode(uint32 nIndex) const
{
	// Because this is just a simple list, there are ONLY items in here!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SHListNode::SHListNode(SceneHierarchy &cHierarchy) : SceneHierarchyNode(cHierarchy)
{
	// Empty
}

/**
*  @brief
*    Destructor
*/
SHListNode::~SHListNode()
{
	// This implementation is that booorrring! ;-)
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
