/*********************************************************\
 *  File: SceneHierarchy.h                               *
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


#ifndef __PLSCENE_SCENEHIERARCHY_H__
#define __PLSCENE_SCENEHIERARCHY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLCore/Container/Pool.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector3;
	class Matrix4x4;
}
namespace PLGraphics {
	class Color4;
}
namespace PLRenderer {
	class Renderer;
}
namespace PLScene {
	class SceneNode;
	class SceneContext;
	class SceneContainer;
	class SceneHierarchyNode;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene hierarchy class (spatial database, Bounding Volume Hierarchies (BVHs))
*
*  @remarks
*    Manages scene nodes of one or more scene containers within a hierarchy.
*/
class SceneHierarchy : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneNode;
	friend class SceneContainer;
	friend class SceneHierarchyNode;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneHierarchy, "PLScene", PLCore::Object, "Abstract scene hierarchy class (spatial database, Bounding Volume Hierarchies (BVHs))")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the scene context the scene hierarchy is in
		*
		*  @return
		*    The scene context the scene hierarchy is in, can but shouldn't be a null pointer
		*/
		PLS_API SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the scene container this scene hierarchy operates on
		*
		*  @return
		*    The scene container this scene hierarchy operates on
		*/
		PLS_API SceneContainer &GetSceneContainer() const;

		/**
		*  @brief
		*    Returns the scene hierarchy root node
		*
		*  @return
		*    The scene hierarchy root node
		*/
		PLS_API SceneHierarchyNode &GetRootNode() const;

		/**
		*  @brief
		*    Initializes the hierarchy
		*
		*  @param[in] vMin
		*    Minimum axis aligned bounding box value
		*  @param[in] vMax
		*    Maximum axis aligned bounding box value
		*
		*  @note
		*    - Call this function before you add any scene nodes
		*/
		PLS_API void Init(const PLMath::Vector3 &vMin, const PLMath::Vector3 &vMax);

		/**
		*  @brief
		*    Returns the maximum allowed level (tree depth)
		*
		*  @return
		*    The maximum allowed level
		*/
		PLS_API PLCore::uint32 GetMaxLevel() const;

		/**
		*  @brief
		*    Sets the maximum allowed level (tree depth)
		*
		*  @param[in] nMaxLevel
		*    The maximum allowed level
		*/
		PLS_API void SetMaxLevel(PLCore::uint32 nMaxLevel = 16);

		/**
		*  @brief
		*    Returns the maximum allowed number of items per scene hierarchy node
		*
		*  @return
		*    The maximum allowed number of items per scene hierarchy node
		*/
		PLS_API PLCore::uint32 GetMaxNumOfNodeItems() const;

		/**
		*  @brief
		*    Sets the maximum allowed number of items per scene hierarchy node
		*
		*  @param[in] nMaxNumOfNodeItems
		*    The maximum allowed number of items per scene hierarchy node
		*/
		PLS_API void SetMaxNumOfNodeItems(PLCore::uint32 nMaxNumOfNodeItems = 16);

		/**
		*  @brief
		*    Returns the total number of scene hierarchy nodes (used or currently unused)
		*
		*  @return
		*    Total number of scene hierarchy nodes
		*/
		PLS_API PLCore::uint32 GetTotalNumOfNodes() const;

		/**
		*  @brief
		*    Returns the number of currently used scene hierarchy nodes
		*
		*  @return
		*    Total number of currently used scene hierarchy nodes
		*/
		PLS_API PLCore::uint32 GetNumOfNodes() const;

		/**
		*  @brief
		*    Returns the number of scene nodes within this hierarchy
		*
		*  @return
		*    The number of scene nodes within this hierarchy
		*/
		PLS_API PLCore::uint32 GetNumOfSceneNodes() const;

		/**
		*  @brief
		*    Draws the hierarchy
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fLineWidth
		*    Line width
		*/
		PLS_API void Draw(PLRenderer::Renderer &cRenderer, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fLineWidth = 1.0f) const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLS_API SceneHierarchy();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneHierarchy();

		/**
		*  @brief
		*    Adds a scene node
		*
		*  @param[in] cSceneNode
		*    Scene node to add
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe the given scene node is already within a hierarchy)
		*/
		PLS_API bool AddSceneNode(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Removes a scene node
		*
		*  @param[in] cSceneNode
		*    Scene node to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool RemoveSceneNode(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Refreshes a scene node
		*
		*  @param[in] cSceneNode
		*    Scene node to refresh
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe the given scene node is within no hierarchy)
		*/
		PLS_API bool RefreshSceneNode(SceneNode &cSceneNode);


	//[-------------------------------------------------------]
	//[ Protected virtual SceneHierarchy functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates a hierarchy node
		*
		*  @return
		*    The created scene hierarchy node
		*/
		virtual SceneHierarchyNode &CreateNode() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		SceneContainer	   *m_pSceneContainer;		/**< Scene container this scene hierarchy node operates on (NEVER a null pointer!) */
		SceneHierarchyNode *m_pRootNode;			/**< Scene hierarchy root node (NEVER a null pointer!) */
		PLCore::uint32		m_nTotalNumOfNodes;		/**< Total number of scene hierarchy nodes */
		PLCore::uint32		m_nNumOfSceneNodes;		/**< The number of scene nodes within this hierarchy */
		PLCore::uint32		m_nMaxLevel;			/**< Maximum allowed level (tree depth) */
		PLCore::uint32		m_nMaxNumOfNodeItems;	/**< Maximum allowed number of items per scene hierarchy node */

		/** List of currently unused scene hierarchy nodes */
		PLCore::Pool<SceneHierarchyNode*> m_lstFreeNodes;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEHIERARCHY_H__
