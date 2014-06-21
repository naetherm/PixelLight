/*********************************************************\
 *  File: Skeleton.h                                     *
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


#ifndef __PLMESH_SKELETON_H__
#define __PLMESH_SKELETON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementManager.h>
#include <PLCore/Container/Resource.h>
#include <PLCore/Container/ResourceHandler.h>
#include <PLCore/Container/ResourceManager.h>
#include "PLMesh/JointAni.h"
#include "PLMesh/Joint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Hierarchical skeleton class
*/
class Skeleton : public PLCore::Resource<Skeleton>, public PLCore::ElementManager<Joint>, public JointAni {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SkeletonManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~Skeleton();

		//[-------------------------------------------------------]
		//[ Joints                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the skeleton root joint indices
		*
		*  @return
		*    Skeleton root joint indices
		*
		*  @note
		*    Must be called after joint parent/child manipulation
		*/
		PLMESH_API const PLCore::Array<PLCore::uint32> &GetRootJoints() const;

		/**
		*  @brief
		*    Updates the joint information
		*
		*  @see
		*    - GetRootJoints()
		*/
		PLMESH_API void UpdateJointInformation();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cManager
		*    Skeleton manager using this resource
		*  @param[in] sName
		*    Resource name to set
		*/
		Skeleton(PLCore::ResourceManager<Skeleton> &cManager, const PLCore::String &sName);

		/**
		*  @brief
		*    Recursive joint state calculations
		*
		*  @param[in] cJoint
		*    Current joint to process
		*  @param[in] pParentJoint
		*    Current parent joint to process, can be a null pointer
		*/
		void RecCalculateStates(Joint &cJoint, Joint *pParentJoint = nullptr) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<PLCore::uint32> m_lstRootJoints;	/**< Array of root joint indices */


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual Joint *CreateElement(const PLCore::String &sName) override;


	//[-------------------------------------------------------]
	//[ Public virtual AnimationInfo functions                ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API Skeleton &operator =(const Skeleton &cSource);
		PLMESH_API virtual PLCore::String GetSourceName() const override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API virtual bool Unload() override;
		PLMESH_API virtual PLCore::String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_SKELETON_H__
