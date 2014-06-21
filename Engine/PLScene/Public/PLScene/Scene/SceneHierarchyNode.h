/*********************************************************\
 *  File: SceneHierarchyNode.h                           *
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


#ifndef __PLSCENE_SCENEHIERARCHYNODE_H__
#define __PLSCENE_SCENEHIERARCHYNODE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/AABoundingBox.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color4;
}
namespace PLMath {
	class Line;
	class Plane;
	class PlaneSet;
}
namespace PLScene {
	class SceneNode;
	class SceneContext;
	class SceneHierarchy;
	class SceneHierarchyNodeItem;
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
*    Abstract scene hierarchy node class
*
*  @remarks
*    A scene hierarchy node can contain multiple scene hierarchy node items.
*/
class SceneHierarchyNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneHierarchy;
	friend class SceneHierarchyNodeItem;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the scene context the scene hierarchy node is in
		*
		*  @return
		*    The scene context the scene hierarchy node is in, can but shouldn't be a null pointer
		*/
		PLS_API SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the owner scene hierarchy
		*
		*  @return
		*    The owner scene hierarchy (NEVER a null pointer!)
		*/
		PLS_API SceneHierarchy *GetHierarchy() const;

		/**
		*  @brief
		*    Returns the parent scene hierarchy node
		*
		*  @return
		*    The parent scene hierarchy node, if a null pointer, this is the root
		*/
		PLS_API SceneHierarchyNode *GetParentNode() const;

		/**
		*  @brief
		*    Returns the unique ID of the scene hierarchy node
		*
		*  @return
		*    The unique ID of the scene hierarchy node
		*
		*  @remarks
		*    Some scene queries may need to hold some extra information per scene hierarchy node.
		*    In such cases, an unique node ID is quite useful to manage this information within for
		*    instance an array. SQCull for example may cache scene hierarchy node visibility information
		*    from the previous frame for the coherent hierarchical occlusion culling algorithm. In order
		*    to keep this unique node ID system working, ONLY SceneHierarchy::GetFreeNode() and
		*    SceneHierarchy::FreeNode() are used during runtime instead of new/delete. Have a look at
		*    the SceneHierarchy::GetFreeNode() documentation for more information about this.
		*/
		PLS_API PLCore::uint32 GetID() const;

		/**
		*  @brief
		*    Returns the level/tree depth of the scene hierarchy node
		*
		*  @return
		*    The level/tree depth of the scene hierarchy node
		*/
		PLS_API PLCore::uint8 GetLevel() const;

		/**
		*  @brief
		*    Returns the axis aligned bounding box of the scene hierarchy node
		*
		*  @return
		*    The axis aligned bounding box of the scene hierarchy node
		*/
		PLS_API const PLMath::AABoundingBox &GetAABoundingBox() const;

		/**
		*  @brief
		*    Checks whether the scene hierarchy node volume is intersecting the given line
		*
		*  @param[in] cLine
		*    Line to check against
		*
		*  @return
		*    'false' if the scene hierarchy node volume is not intersecting the given line, else 'true'
		*/
		PLS_API bool CheckLine(const PLMath::Line &cLine) const;

		/**
		*  @brief
		*    Checks whether the scene hierarchy node volume is within the given plane set
		*
		*  @param[in]  cPlaneSet
		*    Plane set to check against
		*  @param[out] pnOutClipMask
		*    If not a null pointer, this clip mask will receive the intersection state of a
		*    maximum number of 32 planes if the box intersects the plane set.
		*
		*  @return
		*    'false' if the scene hierarchy node volume is not within the given plane set, else 'true'
		*/
		PLS_API bool CheckPlaneSet(const PLMath::PlaneSet &cPlaneSet, PLCore::uint32 *pnOutClipMask = nullptr) const;

		/**
		*  @brief
		*    Returns the side of the plane the scene hierarchy node volume is on
		*
		*  @param[in] cPlane
		*    Plane to check against
		*
		*  @return
		*    '-1' if of scene hierarchy node volume is behind the plane, '0' if it is
		*    intersecting the plane, '1' if the volume is in front of the plane
		*/
		PLS_API char GetPlaneSide(const PLMath::Plane &cPlane) const;

		/**
		*  @brief
		*    Returns the shortest distance from a given point to the scene hierarchy node volume
		*
		*  @param[in] vPoint
		*    Point to get the shortest distance from
		*
		*  @return
		*    Shortest distance (squared) from the given point to the scene hierarchy node volume
		*/
		PLS_API float GetShortestDistance(const PLMath::Vector3 &vPoint) const;

		/**
		*  @brief
		*    Returns the number of scene hierarchy node items
		*
		*  @return
		*    The number of scene hierarchy node items
		*/
		PLS_API PLCore::uint32 GetNumOfItems() const;

		/**
		*  @brief
		*    Returns the first scene hierarchy node item
		*
		*  @return
		*    The first scene hierarchy node item, a null pointer if there are no items
		*
		*  @note
		*    - Use SceneHierarchyNodeItem::GetPreviousItem()/SceneHierarchyNodeItem::GetNextItem()
		*      get the previous/next scene hierarchy node item
		*/
		PLS_API SceneHierarchyNodeItem *GetFirstItem() const;


	//[-------------------------------------------------------]
	//[ Public virtual SceneHierarchyNode functions           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Touches the scene hierarchy scene node
		*
		*  @param[in] bRecursive
		*    Touch recursive?
		*
		*  @remarks
		*    Scene node distribution (splitt/merge) within the scene hierarchy is only performed if this function
		*    is called. If during for instance a visibility determination a scene hierarchy node is used the first
		*    time during this process, one should call this 'touch' function to inform the scene hierarchy that this
		*    scene hierarchy node is going to be used. This function will split a scene hierarchy node if certain conditions
		*    like a maximum number of items per scene hierarchy node are given. If other conditions are given a merge operation
		*    is performed which 'combines' scene hierarchy nodes instead of splitting them. As result of this function,
		*    normally only the scene hierarchy parts which are currently used will be updated. ("lazy evaluation" :)
		*    If 'bRecursive' is set to true, all child nodes are touched to - but this may only be useful for instance when
		*    initializing the scene hierarchy.
		*/
		virtual void Touch(bool bRecursive = false) = 0;

		/**
		*  @brief
		*    Returns the number of child scene hierarchy nodes
		*
		*  @return
		*    The number of child scene hierarchy nodes
		*/
		virtual PLCore::uint32 GetNumOfNodes() const = 0;

		/**
		*  @brief
		*    Returns a child scene hierarchy node
		*
		*  @param[in] nIndex
		*    Index of the requested child scene hierarchy node
		*
		*  @return
		*    The requested child scene hierarchy node, a null pointer on error
		*/
		virtual SceneHierarchyNode *GetNode(PLCore::uint32 nIndex) const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHierarchy
		*    The owner hierarchy
		*/
		PLS_API SceneHierarchyNode(SceneHierarchy &cHierarchy);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneHierarchyNode();

		/**
		*  @brief
		*    Returns a free scene hierarchy node
		*
		*  @return
		*    Free scene hierarchy node
		*
		*  @remarks
		*    This function will return a free scene hierarchy node. If there are no free nodes available,
		*    a new one is created. After a node is no longer required it is put into the list of free nodes
		*    by using FreeNode() instead of destroying it. This way, frequently creation and destruction of
		*    nodes is avoided and the management of the unique node ID's becomes quite simple.
		*    See SceneHierarchyNode::GetID() for more information about the unique node ID's.
		*/
		PLS_API SceneHierarchyNode &GetFreeNode();

		/**
		*  @brief
		*    Marks a scene hierarchy node as free
		*
		*  @param[in] cNode
		*    Scene hierarchy node to mark as free
		*
		*  @see
		*    - GetFreeNode()
		*/
		PLS_API void FreeNode(SceneHierarchyNode &cNode);


	//[-------------------------------------------------------]
	//[ Protected virtual SceneHierarchyNode functions        ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Initializes the scene hierarchy node variables
		*/
		PLS_API virtual void Init();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Adds a scene node to this scene hierarchy node
		*
		*  @param[in] cSceneNode
		*    Scene node to add
		*/
		void AddSceneNode(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Deletes all items attached to this scene hierarchy node
		*/
		void DeleteItems();

		/**
		*  @brief
		*    Draws the hierarchy node (for debugging)
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
		void Draw(PLRenderer::Renderer &cRenderer, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fLineWidth = 1.0f) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		SceneHierarchy		   *m_pHierarchy;		/**< Owner hierarchy (always valid!) */
		SceneHierarchyNode	   *m_pParentNode;		/**< Parent scene hierarchy node, can be a null pointer */
		PLCore::uint32			m_nID;				/**< The unique ID of the scene hierarchy node */
		PLCore::uint8			m_nLevel;			/**< Scene hierarchy node level */
		PLMath::AABoundingBox   m_cAABoundingBox;	/**< Axis aligned bounding box of this scene hierarchy node */
		PLCore::uint32			m_nNumOfItems;		/**< The number of scene hierarchy node items attached to this scene hierarchy node */
		SceneHierarchyNodeItem *m_pFirstItem;		/**< The first scene hierarchy node item, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEHIERARCHYNODE_H__
