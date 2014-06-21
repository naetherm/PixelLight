/*********************************************************\
 *  File: VisNode.h                                      *
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


#ifndef __PLSCENE_VISIBILITY_VISNODE_H__
#define __PLSCENE_VISIBILITY_VISNODE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Matrix3x4.h>
#include <PLMath/Matrix4x4.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneNode;
class VisContainer;
class SceneNodeHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Node of the visibility tree
*/
class VisNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SQCull;
	friend class VisManager;
	friend class VisContainer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the visibility root container
		*
		*  @return
		*    The visibility root container, a null pointer on error
		*/
		PLS_API const VisContainer *GetVisRootContainer() const;

		/**
		*  @brief
		*    Returns the parent visibility node
		*
		*  @return
		*    The parent visibility node, a null pointer if this is the root
		*
		*  @remarks
		*    The parent visibility node can be a VisContainer or a VisPortal. If the parent
		*    is a portal, this visibility node is the cell it links to.
		*/
		PLS_API const VisNode *GetParent() const;

		/**
		*  @brief
		*    Returns the scene node this visibility node links to
		*
		*  @return
		*    The scene node this visibility node links to, a null pointer if there's no scene node
		*    linked to this visibility node (maybe the scene node this visibility node was
		*    linked to was killed?)
		*/
		PLS_API SceneNode *GetSceneNode() const;

		/**
		*  @brief
		*    Returns the used projection matrix
		*
		*  @return
		*    The used projection matrix
		*/
		PLS_API const PLMath::Matrix4x4 &GetProjectionMatrix() const;

		/**
		*  @brief
		*    Returns the used view matrix
		*
		*  @return
		*    The used view matrix
		*/
		PLS_API const PLMath::Matrix4x4 &GetViewMatrix() const;

		/**
		*  @brief
		*    Returns the used view projection matrix
		*
		*  @return
		*    The used view projection matrix
		*/
		PLS_API const PLMath::Matrix4x4 &GetViewProjectionMatrix() const;

		/**
		*  @brief
		*    Returns the absolute world matrix of the scene node
		*
		*  @return
		*    The absolute world matrix of the scene node
		*
		*  @remarks
		*    If the camera is within a container, this world matrix is relative to
		*    the container the camera is in.
		*/
		PLS_API const PLMath::Matrix3x4 &GetWorldMatrix() const;

		// [TODO] VisNode refactoring -> currently, this method should not be used if possible because it doesn't update for example GetInverseWorldMatrix()
		void SetWorldMatrix(const PLMath::Matrix3x4 &mWorld);

		/**
		*  @brief
		*    Returns the absolute inverse world matrix of the scene node
		*
		*  @return
		*    The absolute inverse world matrix of the scene node
		*
		*  @remarks
		*    If the camera is within a container, this world matrix is relative to
		*    the container the camera is in.
		*/
		PLS_API const PLMath::Matrix3x4 &GetInverseWorldMatrix() const;

		/**
		*  @brief
		*    Returns the absolute world view matrix of the scene node
		*
		*  @return
		*    The absolute world view matrix of the scene node
		*/
		PLS_API const PLMath::Matrix4x4 &GetWorldViewMatrix() const;

		/**
		*  @brief
		*    Returns the absolute world view projection matrix of the scene node
		*
		*  @return
		*    The absolute world view projection matrix of the scene node
		*/
		PLS_API const PLMath::Matrix4x4 &GetWorldViewProjectionMatrix() const;

		/**
		*  @brief
		*    Returns the squared distance to the camera
		*
		*  @return
		*    The squared distance to the camera
		*/
		PLS_API float GetSquaredDistanceToCamera() const;


	//[-------------------------------------------------------]
	//[ Public virtual VisNodes functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not this is a container visibility node (VisContainer, links to a SceneContainer scene node)
		*
		*  @return
		*    'true' if this is a container visibility node, else 'false'
		*/
		PLS_API virtual bool IsContainer() const;

		/**
		*  @brief
		*    Returns whether or not this is a cell visibility node (VisContainer, links to a SNCell scene node)
		*
		*  @return
		*    'true' if this is a cell visibility node, else 'false'
		*/
		PLS_API virtual bool IsCell() const;

		/**
		*  @brief
		*    Returns whether or not this is a portal visibility node (VisPortal, links to a SNCellPortal scene node)
		*
		*  @return
		*    'true' if this is a portal visibility node, else 'false'
		*/
		PLS_API virtual bool IsPortal() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    The parent visibility node, a null pointer if this is the root
		*/
		VisNode(VisNode *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VisNode();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Flags which hold ínternal information
		*/
		enum EInternalFlags {
			RecalculateInvWorld		 = 1<<0,	/**< Recalculation of the current absolute inverse world matrix of the scene node required */
			RecalculateWorldView	 = 1<<1,	/**< Recalculation of the current absolute world view matrix of the scene node required */
			RecalculateWorldViewProj = 1<<2		/**< Recalculation of the current absolute world view projection matrix of the scene node required */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		VisNode			  *m_pParent;					/**< Parent visibility node (VisContainer or VisPortal, can be a null pointer) */
		SceneNodeHandler  *m_pSceneNodeHandler;			/**< Scene node this visibility node is linked to (always valid!) */
		PLMath::Matrix3x4  m_mWorld;					/**< Absolute world matrix of the scene node */
		float			   m_fSquaredDistanceToCamera;	/**< Squared distance to the camera */
		// Lazy evaluation
		mutable PLCore::uint8	  m_nInternalFlags;		/**< Internal flags */
		mutable PLMath::Matrix3x4 m_mInvWorld;			/**< Absolute inverse world matrix of the scene node (derived on demand from m_mWorld and SQCull data) */
		mutable PLMath::Matrix4x4 m_mWorldView;			/**< Absolute world view matrix of the scene node (derived on demand from m_mWorld and SQCull data) */
		mutable PLMath::Matrix4x4 m_mWorldViewProj;		/**< Absolute world view projection matrix of the scene node (derived on demand from m_mWorld and SQCull data) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_VISIBILITY_VISNODE_H__
