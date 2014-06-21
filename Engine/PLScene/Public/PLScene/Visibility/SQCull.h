/*********************************************************\
 *  File: SQCull.h                                       *
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


#ifndef __PLSCENE_SCENEQUERY_CULL_H__
#define __PLSCENE_SCENEQUERY_CULL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Pool.h>
#include <PLCore/Container/Array.h>
#include <PLCore/Container/Bitset.h>
#include <PLCore/Container/HashMap.h>
#include <PLCore/Container/BinaryHeap.h>
#include <PLMath/PlaneSet.h>
#include <PLMath/Matrix4x4.h>
#include "PLScene/Scene/SceneQuery.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class OcclusionQuery;
}
namespace PLScene {
	class VisContainer;
	class SceneHierarchy;
	class SceneNodeHandler;
	class SceneQueryHandler;
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
*    Visibility determination scene query class
*
*  @remarks
*    This scene query is used to request the current visible scene nodes. (Hidden Surface Removal (HSR))
*    You can choose between different culling algorithms whereby the advanced techniques not only ignore
*    scene nodes outside the view frustum but also scene nodes which are hidden through other scene nodes.
*    Additionally, some statistics are calculated. In optimal situations, cells&portals, frustum culling and
*    occlusion culling are used all together to get the desired results.
*    The result of the whole process is called PVS. (Potential Visibility Set)
*
*  @note
*    - The 'PerformQuery()'-call must be done within the correct render target and with a valid set
*      viewport and scissor rectangle
*/
class SQCull : public SceneQuery {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class VisContainer;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Cull mode
		*/
		enum EMode {
			Frustum     = 0,	/**< Culls the scene with view frustum culling only */
			StopAndWait = 1,	/**< Culls the scene with the hierarchical stop and wait algorithm */
			Coherent    = 2,	/**< Culls the scene with the coherent hierarchical algorithm (CHC) */
			Previous    = 3		/**< Culls the scene using the visibility information of the previous frame */
		};


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SQCull, "PLScene", PLScene::SceneQuery, "Visibility determination scene query class")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Several statistics of the culling process
		*/
		struct Statistics {
			PLCore::uint64 nCullTime;							/**< Culling time of the specified algorithm (in ms) */
			PLCore::uint32 nNumOfTraversedNodes;				/**< Number of traversed nodes */
			PLCore::uint32 nNumOfOccluders;						/**< Number of used occludes */
			PLCore::uint32 nNumOfQueryCulledNodes;				/**< Number of hierarchy nodes culled by the occlusion query */
			PLCore::uint32 nNumOfSkippedFrustumTests;			/**< Number of skipped frustum tests because for instance the parent node was
																	 already complete within the frustum. (hierarchy node & scene node tests) */
			PLCore::uint32 nNumOfFrustumCulledNodes;			/**< Number of hierarchy nodes culled by the frustum culling only */
			PLCore::uint32 nNumOfNearPlaneIntersectingNodes;	/**< Number of hierarchy nodes with near plane intersection */
			PLCore::uint32 nNumOfVisibleSceneNodes;				/**< Number of visible scene nodes */
			PLCore::uint32 nNumOfQueries;						/**< Total number of occlusion queries */
			PLCore::uint32 nMaxNumOfQueries;					/**< Maximum number of occlusion queries active at the same time */
			bool		   bWaitForQueryResult;					/**< Was waiting for a query result required? */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SQCull();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SQCull();

		/**
		*  @brief
		*    Returns the used cull mode
		*
		*  @return
		*    Used cull mode
		*/
		PLS_API EMode GetMode() const;

		/**
		*  @brief
		*    Sets the used cull mode
		*
		*  @param[in] nMode
		*    Used cull mode
		*/
		PLS_API void SetMode(EMode nMode = Coherent);

		/**
		*  @brief
		*    Returns visibility threshold
		*
		*  @return
		*    Visibility threshold
		*
		*  @remarks
		*    The visibility threshold is dependent of the render target dimension. If
		*    you set the visibility threshold to 100 and your render target dimension
		*    is 100x100 scene nodes may ugly disappear and at a render target dimension
		*    of 1000x1000 you can't see such disappearances.
		*
		*  @note
		*    - Used by the render modes StopAndWait and Coherent
		*/
		PLS_API PLCore::uint32 GetVisibilityThreshold() const;

		/**
		*  @brief
		*    Sets visible pixels threshold for visibility classification
		*
		*  @param[in] nThreshold
		*    Threshold
		*
		*  @see
		*    - GetVisibilityThreshold()
		*/
		PLS_API void SetVisibilityThreshold(PLCore::uint32 nThreshold = 1);

		/**
		*  @brief
		*    Returns the statistics
		*
		*  @return
		*    Statistics
		*/
		PLS_API const Statistics &GetStatistics() const;

		/**
		*  @brief
		*    Returns the container the camera is in
		*
		*  @return
		*    Container the camera is in, a null pointer on error
		*/
		PLS_API SceneContainer *GetCameraContainer() const;

		/**
		*  @brief
		*    Sets the container the camera is in
		*
		*  @param[in] pCameraContainer
		*    Container the camera is in, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool SetCameraContainer(SceneContainer *pCameraContainer);

		/**
		*  @brief
		*    Returns the used camera position
		*
		*  @return
		*    The used camera position (container space)
		*/
		PLS_API const PLMath::Vector3 &GetCameraPosition() const;

		/**
		*  @brief
		*    Sets the used camera position
		*
		*  @param[in] vPos
		*    The used camera position (container space)
		*/
		PLS_API void SetCameraPosition(const PLMath::Vector3 &vPos);

		/**
		*  @brief
		*    Returns the used view frustum
		*
		*  @return
		*    The used view frustum (container space)
		*/
		PLS_API const PLMath::PlaneSet &GetViewFrustum() const;

		/**
		*  @brief
		*    Sets the used view frustum
		*
		*  @param[in] cFrustum
		*    The used view frustum (container space)
		*/
		PLS_API void SetViewFrustum(const PLMath::PlaneSet &cFrustum);

		// [TODO] Cleanup
		PLS_API const PLMath::Matrix4x4 &GetProjectionMatrix() const;
		PLS_API void SetProjectionMatrix(const PLMath::Matrix4x4 &mProjection);
		PLS_API const PLMath::Matrix4x4 &GetViewMatrix() const;
		PLS_API void SetViewMatrix(const PLMath::Matrix4x4 &mView);
		PLS_API const PLMath::Matrix4x4 &GetViewProjectionMatrix() const;
		PLS_API void SetViewProjectionMatrix(const PLMath::Matrix4x4 &mViewProjection);

		/**
		*  @brief
		*    Returns the visibility root container
		*
		*  @return
		*    The visibility root container, a null pointer on error
		*
		*  @see
		*    - GetVisContainer()
		*/
		PLS_API const VisContainer *GetVisRootContainer() const;

		/**
		*  @brief
		*    Returns the visibility container this scene query belongs to
		*
		*  @return
		*    The visibility container this scene query belongs to, same as
		*    GetRootContainer() if this is the visibility root container
		*
		*  @remarks
		*    After you performed the cull query (= visibility determination) you can use
		*    this function to get the resulting visibility scene tree. Using this tree,
		*    you can render the visible parts of the scene without any effort.
		*/
		PLS_API const VisContainer &GetVisContainer() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initialize the statistics
		*/
		void InitStatistics();

		/**
		*  @brief
		*    Resizes the arrays with the additional scene hierarchy node information if required
		*/
		void ResizeArrays();

		/**
		*  @brief
		*    This is the standard culling traversal algorithm doing only frustum culling
		*
		*  @return
		*    'false' if the query was cancelled by the user, else 'true'
		*/
		bool CullFrustum();

		/**
		*  @brief
		*    This is the naive algorithm always waiting for the query to finish
		*
		*  @return
		*    'false' if the query was cancelled by the user, else 'true'
		*/
		bool CullStopAndWait();

		/**
		*  @brief
		*    Culls the scene using coherent occlusion culling
		*
		*  @remarks
		*    It uses a query queue and frame-to-frame coherence in order to prevent
		*    stalls and avoid unnecessary queries. This culling algorithm is basing on
		*    "Coherent Hierarchical Culling: Hardware Occlusion Queries Made Useful"
		*    (http://www.cg.tuwien.ac.at/research/vr/chcull/)
		*
		*  @return
		*    'false' if the query was cancelled by the user, else 'true'
		*/
		bool CullCoherentWithQueue();

		/**
		*  @brief
		*    Traverses a node
		*
		*  @param[in] cHierarchyNode
		*    Hierarchy node to traverse
		*
		*  @return
		*    'false' if the query was cancelled by the user, else 'true'
		*/
		bool TraverseNode(const SceneHierarchyNode &cHierarchyNode);

		/**
		*  @brief
		*    Visibility is pulled up from visibility of children
		*
		*  @param[in] pHierarchyNode
		*    Hierarchy node we currently work on, can be a null pointer
		*/
		void PullUpVisibility(const SceneHierarchyNode *pHierarchyNode);

		/**
		*  @brief
		*    Issues occlusion query for specified node
		*
		*  @param[in] cHierarchyNode
		*    Hierarchy node to work on
		*  @param[in] bWasVisible
		*    Was this hierarchy node visible at the last frame?
		*/
		void IssueOcclusionQuery(const SceneHierarchyNode &cHierarchyNode, bool bWasVisible);

		/**
		*  @brief
		*    Is result available from query queue?
		*
		*  @param[in] cHierarchyNode
		*    Hierarchy node we currently work on
		*
		*  @return
		*    'true' if the result is available, else 'false'
		*/
		bool ResultAvailable(const SceneHierarchyNode &cHierarchyNode);

		/**
		*  @brief
		*    Returns occlusion query result for specified node
		*
		*  @param[in] cHierarchyNode
		*    Hierarchy node we currently work on
		*/
		PLCore::uint32 GetOcclusionQueryResult(const SceneHierarchyNode &cHierarchyNode);

		/**
		*  @brief
		*    Checks whether a hierarchy node is within the view frustum
		*
		*  @param[in]  cHierarchyNode
		*    Hierarchy node we currently work on
		*  @param[out] bIntersects
		*    Will receive the information whether or not the hierarchy node is intersecting
		*    the near clip plane
		*  @param[in]  bCheckNear
		*    Perform near clip plane intersection test?
		*
		*  @remarks
		*    bIntersects returns true if bounding box intersects the near plane.
		*    additionally stores the distance from the near plane to the center of the
		*    current node with the node. this will for front-to-back ordering
		*
		*  @return
		*    'true' if bounding box is culled by view frustum culling, else 'false'
		*/
		bool InsideViewFrustum(const SceneHierarchyNode &cHierarchyNode, bool &bIntersects, bool bCheckNear = true);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General data
		EMode			   m_nMode;						/**< Used cull mode */
		PLCore::uint32	   m_nVisibilityThreshold;		/**< Visibility threshold */
		Statistics		   m_sStatistics;				/**< Some statistics */
		SceneNodeHandler  *m_pCameraContainer;			/**< Container the camera is in */
		bool			   m_bCameraInCell;				/**< Is the camera within a cell? */
		PLMath::Vector3    m_vCameraPosition;			/**< Camera position (container space) */
		PLMath::PlaneSet   m_cViewFrustum;				/**< View frustum used for the query (container space) */
		bool			   m_bSetIdentityWorldMatrix;	/**< Do we need to set the world matrix to identity? */
		SceneHierarchy    *m_pHierarchy;				/**< The hierarchy we are working on */
		PLMath::Matrix4x4  m_mProjection;				/**< Projection matrix */
		PLMath::Matrix4x4  m_mView;						/**< View matrix */
		PLMath::Matrix4x4  m_mViewProjection;			/**< View projection matrix */

		// Frame to frame data
		PLCore::uint32					m_nFrame;				/**< Current frame */
		PLCore::Bitset					m_lstVisibility;		/**< Visibility state of all hierarchy nodes */
		PLCore::Bitset					m_lstTotalVisibility;	/**< Total visibility state of all hierarchy nodes */
		PLCore::Array<PLCore::uint32>	m_lstLastVisited;		/**< Frame the node was visited last time */

		// Current frame data
		PLCore::uint32									 m_nCurrentQueries;			/**< Current number of queries */
		PLCore::uint32									 m_nOcclusionQueries;		/**< Number of occlusion queries */
		PLRenderer::OcclusionQuery					   **m_ppOcclusionQueries;		/**< Occlusion queries */
		PLCore::BinaryHeap<float, SceneHierarchyNode*> 	 m_lstDistanceQueue;		/**< Distance queue */
		PLCore::BinaryHeap<float, SceneNode*>			 m_lstNodeDistanceQueue;	/**< Distance queue for the scene nodes*/

		VisContainer *m_pVisRootContainer;	/**< Visibility root container */
		VisContainer *m_pVisContainer;		/**< Visibility container (do NOT use this variable directly, use GetVisContainer()!) */


	//[-------------------------------------------------------]
	//[ Public virtual SceneQuery functions                   ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool PerformQuery() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEQUERY_CULL_H__
