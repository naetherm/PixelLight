/*********************************************************\
 *  File: Joint.h                                        *
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


#ifndef __PLMESH_JOINT_H__
#define __PLMESH_JOINT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Element.h>
#include "PLMesh/JointState.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Skeleton joint element
*/
class Joint : public PLCore::Element<Joint>, public JointState {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Skeleton;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*/
		PLMESH_API Joint(const PLCore::String &sName = "", PLCore::ElementManager<Joint> *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~Joint();

		/**
		*  @brief
		*    Get the ID of this joint
		*
		*  @return
		*    ID of this joint, < 0 normally means this is an unused ID
		*/
		PLMESH_API int GetID() const;

		/**
		*  @brief
		*    Set the ID this joint
		*
		*  @param[in] nID
		*    ID of this joint
		*/
		PLMESH_API void SetID(int nID = -1);

		/**
		*  @brief
		*    Get the parent joint number of this joint
		*
		*  @return
		*    Number of the parent joint number, if < 0 no parent
		*/
		PLMESH_API int GetParent() const;

		/**
		*  @brief
		*    Set the parent joint number of this joint
		*
		*  @param[in] nParent
		*    Number of the parent joint number, if < 0 no parent
		*/
		PLMESH_API void SetParent(int nParent = -1);

		/**
		*  @brief
		*    Gets an array with the child joints
		*
		*  @return
		*    Child joints array
		*
		*  @note
		*    - Call Skeleton::UpdateJointInformation() after changing parent/child joints!
		*/
		PLMESH_API const PLCore::Array<PLCore::uint32> &GetChildren() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This joint
		*/
		PLMESH_API Joint &operator =(const Joint &cSource);

		/**
		*  @brief
		*    Compares two joints
		*
		*  @param[in] cJoint
		*    Joint to compare with
		*
		*  @return
		*    'true' if both joints are equal, else 'false'
		*/
		PLMESH_API bool operator ==(const Joint &cJoint) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int								m_nID;			/**< Joint ID */
		int								m_nParent;		/**< Number of the parent joint */
		PLCore::Array<PLCore::uint32>	m_lstChildren;	/**< Array of child joints */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_JOINT_H__
