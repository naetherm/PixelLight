/*********************************************************\
 *  File: SHList.h                                       *
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


#ifndef __PLSCENE_SCENEHIERARCHY_LIST_H__
#define __PLSCENE_SCENEHIERARCHY_LIST_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneHierarchy.h"
#include "PLScene/Scene/SceneHierarchyNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene hierarchy which is in fact a simple list (a special case of a tree :)
*
*  @note
*    - Try to avoid using this hierarchy type for larger scenes because it's quite inefficient!
*    - Internally this 'hierarchy' will use the scene container nodes directly
*      (therefore no extra reference to the scene nodes required :)
*/
class SHList : public SceneHierarchy {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SHList, "PLScene", PLScene::SceneHierarchy, "Scene hierarchy which is in fact a simple list (a special case of a tree :)")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SHList();


	//[-------------------------------------------------------]
	//[ Private virtual SceneHierarchy functions              ]
	//[-------------------------------------------------------]
	private:
		virtual SceneHierarchyNode &CreateNode() override;


};

/**
*  @brief
*    List scene hierarchy node class
*/
class SHListNode : public SceneHierarchyNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SHList;


	//[-------------------------------------------------------]
	//[ Public virtual SceneHierarchyNode functions           ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void Touch(bool bRecursive = false) override;
		PLS_API virtual PLCore::uint32 GetNumOfNodes() const override;
		PLS_API virtual SceneHierarchyNode *GetNode(PLCore::uint32 nIndex) const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHierarchy
		*    The owner hierarchy
		*/
		SHListNode(SceneHierarchy &cHierarchy);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SHListNode();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEHIERARCHY_LIST_H__
