/*********************************************************\
 *  File: SHKdTree.h                                     *
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


#ifndef __PLSCENE_SCENEHIERARCHY_KDTREE_H__
#define __PLSCENE_SCENEHIERARCHY_KDTREE_H__
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
*    Kd tree scene hierarchy (axis aligned binary tree)
*
*  @remarks
*    A node has two children. The node can be either an interior node (i.e., left and
*    right child) or a leaf node, which holds the actual scene nodes.
*/
class SHKdTree : public SceneHierarchy {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SHKdTree, "PLScene", PLScene::SceneHierarchy, "Kd tree scene hierarchy (axis aligned binary tree)")
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
		PLS_API SHKdTree();


	//[-------------------------------------------------------]
	//[ Private virtual SceneHierarchy functions              ]
	//[-------------------------------------------------------]
	private:
		virtual SceneHierarchyNode &CreateNode() override;


};

/**
*  @brief
*    Kd tree scene hierarchy node class
*/
class SHKdTreeNode : public SceneHierarchyNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SHKdTree;


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
		SHKdTreeNode(SceneHierarchy &cHierarchy);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SHKdTreeNode();

		/**
		*  @brief
		*    Merges the items of the scene hierarchy child nodes to this nodes if required
		*/
		void Merge();

		/**
		*  @brief
		*    Splits this scene hierarchy node
		*/
		void Split();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Split axis
		*/
		enum EAxis {
			AxisX = 0,
			AxisY = 1,
			AxisZ = 2,
			Leaf  = 3
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SHKdTreeNode *m_pLeftNode;		/**< Left scene hierarchy node, can be a null pointer */
		SHKdTreeNode *m_pRightNode;		/**< Right scene hierarchy node, can be a null pointer */
		EAxis		  m_nSplitAxis;		/**< Split axis */
		float		  m_fSplitValue;	/**< Split value */


	//[-------------------------------------------------------]
	//[ Public virtual SceneHierarchyNode functions           ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void Touch(bool bRecursive = false) override;
		PLS_API virtual PLCore::uint32 GetNumOfNodes() const override;
		PLS_API virtual SceneHierarchyNode *GetNode(PLCore::uint32 nIndex) const override;


	//[-------------------------------------------------------]
	//[ Private virtual SceneHierarchyNode functions          ]
	//[-------------------------------------------------------]
	private:
		virtual void Init() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEHIERARCHY_KDTREE_H__
