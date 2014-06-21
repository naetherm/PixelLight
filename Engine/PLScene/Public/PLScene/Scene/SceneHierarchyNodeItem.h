/*********************************************************\
 *  File: SceneHierarchyNodeItem.h                       *
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


#ifndef __PLSCENE_SCENEHIERARCHYITEM_H__
#define __PLSCENE_SCENEHIERARCHYITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneNode;
class SceneHierarchyNode;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene hierarchy node item class
*
*  @remarks
*    A scene hierarchy node item connects scene nodes and hierarchy nodes. One scene node can have
*    multiple hierarchy node items linking them to different hierarchy nodes and one hierarchy node can have
*    multiple hierarchy node items within it linking to different scene nodes.
*/
class SceneHierarchyNodeItem {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneHierarchy;
	friend class SceneHierarchyNode;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the scene node this scene hierarchy node item is linked to
		*
		*  @return
		*    The scene node this scene hierarchy node item is linked to, a null pointer on error
		*    (currently not linked to a scene node?)
		*/
		PLS_API SceneNode *GetSceneNode() const;

		/**
		*  @brief
		*    Returns the scene hierarchy node this scene hierarchy node item is attached to
		*
		*  @return
		*    The scene hierarchy node this scene hierarchy node item is attached to, a null pointer on error
		*    (currently not attached to a scene hierarchy node?)
		*/
		PLS_API SceneHierarchyNode *GetHierarchyNode() const;

		/**
		*  @brief
		*    Returns the previous scene hierarchy node item
		*
		*  @return
		*    The previous scene hierarchy node item, a null pointer if there's no previous item
		*/
		PLS_API SceneHierarchyNodeItem *GetPreviousItem() const;

		/**
		*  @brief
		*    Returns the next scene hierarchy node item
		*
		*  @return
		*    The next scene hierarchy node item, a null pointer if there's no next item
		*/
		PLS_API SceneHierarchyNodeItem *GetNextItem() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	// [TODO] How to make this protected but available for classes like SHKdTreeNode??
	public:
//	private:
		/**
		*  @brief
		*    Constructor
		*/
		PLS_API SceneHierarchyNodeItem();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API ~SceneHierarchyNodeItem();

		/**
		*  @brief
		*    Links this scene hierarchy node item with a scene node
		*
		*  @param[in] cSceneNode
		*    Scene node where link this scene hierarchy node item to
		*
		*  @return
		*    'true' if all went fine, else 'false' (currently already linked?)
		*/
		PLS_API bool Link(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Unlinks this scene hierarchy node item from the scene node
		*
		*  @return
		*    'true' if all went fine, else 'false' (currently not linked?)
		*/
		PLS_API bool Unlink();

		/**
		*  @brief
		*    Attaches this scene hierarchy node item to a scene hierarchy node
		*
		*  @param[in] cSceneHierarchyNode
		*    Scene hierarchy node where to attach this scene hierarchy node item
		*
		*  @return
		*    'true' if all went fine, else 'false' (currently already attached?)
		*/
		PLS_API bool Attach(SceneHierarchyNode &cSceneHierarchyNode);

		/**
		*  @brief
		*    Detaches this scene hierarchy node item from it's scene hierarchy node
		*
		*  @return
		*    'true' if all went fine, else 'false' (currently not attached?)
		*/
		PLS_API bool Detach();

		/**
		*  @brief
		*    Returns a clone of this scene hierarchy node item
		*
		*  @return
		*    A clone of this scene hierarchy node item
		*
		*  @note
		*    - The clone is already linked to the same scene node, but not attached to
		*      any scene hierarchy node
		*/
		PLS_API SceneHierarchyNodeItem &Clone() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Scene node
		SceneNode			   *m_pSceneNode;						/**< Scene node this scene hierarchy node item is linked with, can be a null pointer */
		SceneHierarchyNodeItem *m_pPreviousSceneNodeItem;			/**< Next scene node item, can be a null pointer */
		SceneHierarchyNodeItem *m_pNextSceneNodeItem;				/**< Previous scene node item, can be a null pointer */
		// Scene hierarchy node
		SceneHierarchyNode	   *m_pSceneHierarchyNode;				/**< Scene hierarchy node this scene hierarchy node item is attached to, can be a null pointer */
		SceneHierarchyNodeItem *m_pPreviousSceneHierarchyNodeItem;	/**< Next scene hierarchy node item, can be a null pointer */
		SceneHierarchyNodeItem *m_pNextSceneHierarchyNodeItem;		/**< Previous scene hierarchy node item, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEHIERARCHYITEM_H__
