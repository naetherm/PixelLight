/*********************************************************\
 *  File: MeshAnimationManager.h                         *
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


#ifndef __PLMESH_MESHANIMATIONMANAGER_H__
#define __PLMESH_MESHANIMATIONMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLRenderer/Animation/AnimationManager.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class MeshHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This is a manager for the mesh animation element
*/
class MeshAnimationManager : public PLRenderer::AnimationManager, public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshAnimationManager, "PLMesh", PLCore::Object, "This is a manager for the mesh animation element")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the mesh animation manager will reset the current joint states
		*    within the Apply() function before the current animation states are applied
		*
		*  @return
		*    'true' if the mesh animation manager will reset the current joint states, else 'false'
		*
		*  @remarks
		*    Normally, the manager will reset the current states to the base settings
		*    before the animation states are 'added'. If this isn't done automatically,
		*    you have to do this by yourself - but this way, you can do more complex
		*    animations.
		*/
		PLMESH_API bool GetResetJointStates() const;

		/**
		*  @brief
		*    Sets whether the mesh animation manager will reset the current joint states
		*    within the Apply() function before the current animation states are applied
		*
		*  @param[in] bReset
		*    Reset states?
		*/
		PLMESH_API void SetResetJointStates(bool bReset = true);

		/**
		*  @brief
		*    Returns whether the mesh animation manager will reset the current morph weights
		*    within the Apply() function before the current animation weights are applied
		*
		*  @return
		*    'true' if the mesh animation manager will reset the current morph weights, else 'false'
		*
		*  @see
		*    - GetResetJointStates()
		*/
		PLMESH_API bool GetResetMorphWeights() const;

		/**
		*  @brief
		*    Sets whether the mesh animation manager will reset the current morph weights
		*    within the Apply() function before the current animation weights are applied
		*
		*  @param[in] bReset
		*    Reset weights?
		*/
		PLMESH_API void SetResetMorphWeights(bool bReset = true);

		/**
		*  @brief
		*    Returns whether the mesh animation manager will apply base joint states before calculating
		*    the current states
		*
		*  @return
		*    'true' if apply base joint states before calculating the current states, else 'false'
		*/
		PLMESH_API bool GetApplyBaseJointStates() const;

		/**
		*  @brief
		*    Sets whether the mesh animation manager will apply base joint states before calculating
		*    the current states
		*
		*  @param[in] bApply
		*    Apply base joint states before calculating the current states?
		*/
		PLMESH_API void SetApplyBaseJointStates(bool bApply = true);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLMESH_API MeshAnimationManager();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshAnimationManager();


	//[-------------------------------------------------------]
	//[ Public virtual MeshAnimationManager functions         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Updates the mesh animation manager
		*
		*  @param[in] fTimeDifference
		*    Past time since last frame (use e.g. PLCore::Timing::GetInstance()->GetTimeDifference())
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API virtual bool Update(float fTimeDifference);

		/**
		*  @brief
		*    Applies all animations to the given mesh handler
		*
		*  @param[in] cMeshHandler
		*    Mesh handler to apply the animations on
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    This base implementation will only calculate the current skeleton handler
		*    joint states and morph target influences. Use a derived mesh animation manager
		*    to for instance manipulate the mesh, too.
		*/
		PLMESH_API virtual bool Apply(MeshHandler &cMeshHandler) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bResetJointStates;		/**< Reset joint states? */
		bool m_bResetMorphWeights;		/**< Reset morph weights? */
		bool m_bApplyBaseJointStates;	/**< Apply base joint states? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHANIMATIONMANAGER_H__
