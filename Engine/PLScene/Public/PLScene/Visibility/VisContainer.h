/*********************************************************\
 *  File: VisContainer.h                                 *
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


#ifndef __PLSCENE_VISIBILITY_VISCONTAINER_H__
#define __PLSCENE_VISIBILITY_VISCONTAINER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Pool.h>
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Rectangle.h>
#include "PLScene/Visibility/VisNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SQCull;
class SceneNode;
class VisPortal;
class VisManager;
class SceneQueryHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Container of the visibility tree
*/
class VisContainer : public VisNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SQCull;
	friend class VisPortal;


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Projection information
		*/
		struct Projection {
			PLMath::Rectangle cRectangle;	/**< Screen space rectangle */
			float			  fZNear;		/**< Near depth value */
			float			  fZFar;		/**< Far depth value */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the cull scene query of this visibility container
		*
		*  @return
		*    The cull scene query of this visibility container, a null pointer if there's no such query
		*/
		PLS_API const SQCull *GetCullQuery() const;

		/**
		*  @brief
		*    Returns the list of visibility nodes
		*
		*  @return
		*    List of visibility nodes
		*/
		PLS_API const PLCore::Pool<VisNode*> &GetVisNodes() const;

		/**
		*  @brief
		*    Returns projection information
		*
		*  @return
		*    Projection information
		*/
		PLS_API const Projection &GetProjection() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    The parent visibility node, a null pointer if this is the root
		*/
		VisContainer(VisNode *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VisContainer();

		/**
		*  @brief
		*    Adds a scene node to this visibility container
		*
		*  @param[in] cSceneNode
		*    Scene node to add
		*  @param[in] fSquaredDistanceToCamera
		*    Squared distance of the given scene node to the camera
		*
		*  @return
		*    The added visibility node linking to the given scene node, a null pointer on error
		*/
		VisNode *AddSceneNode(SceneNode &cSceneNode, float fSquaredDistanceToCamera);

		/**
		*  @brief
		*    Frees all visibility nodes
		*/
		void FreeNodes();

		/**
		*  @brief
		*    Called when the scene node assigned with this visibility container was destroyed
		*/
		void OnDestroy();

		/**
		*  @brief
		*    Returns the visibility manager
		*
		*  @return
		*    The visibility manager, a null pointer on error
		*/
		VisManager *GetVisManager() const;


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerDestroy;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SceneQueryHandler							   *m_pQueryHandler;	/**< The cull scene query of this visibility container (always valid!) */
		PLCore::Pool<VisNode*>							m_lstNodes;			/**< List of currently used visibility nodes */
		PLCore::HashMap<PLCore::String, VisContainer*>  m_mapContainers;	/**< Map with all child visibility containers of this container */
		PLCore::HashMap<PLCore::String, VisPortal*>	    m_mapPortals;		/**< Map with all visibility portals of this container */
		Projection										m_sProjection;		/**< Projection information */


	//[-------------------------------------------------------]
	//[ Public virtual VisNode functions                      ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool IsContainer() const override;
		PLS_API virtual bool IsCell() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_VISIBILITY_VISCONTAINER_H__
