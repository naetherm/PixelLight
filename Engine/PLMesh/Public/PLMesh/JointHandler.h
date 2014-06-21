/*********************************************************\
 *  File: JointHandler.h                                 *
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


#ifndef __PLMESH_JOINTHANDLER_H__
#define __PLMESH_JOINTHANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/ElementHandler.h>
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
*    Joint handler class
*/
class JointHandler : public PLCore::ElementHandler<Joint>, public JointState {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API JointHandler();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~JointHandler();

		/**
		*  @brief
		*    Returns whether or not this joint is user controlled
		*
		*  @return
		*    'true' if this joint is user controlled, else 'false'
		*
		*  @remarks
		*    By default, joints are NOT user controlled which means their absolute translation
		*    and rotation is calculated automatically by the animation system by using their relative
		*    states. This way, multiple 'offline' (predefined :) animations can be mixed together to get
		*    a final animation where animations can only influence some parts of the skeleton. If a
		*    joint is marked as 'used controlled', the animations system does not calculate the
		*    absolute values by itself. In this case, this joints can be fully 'online' (dynamic) animated
		*    by using for instance physics objects in the case of ragdoll animations. Normally only a
		*    few joints are user controlled while others are still updated automatically relative to
		*    their parents.
		*/
		PLMESH_API bool IsUserControlled() const;

		/**
		*  @brief
		*    Sets whether or not this joint is user controlled
		*
		*  @param[in] bUserControlled
		*    'true' if this joint is user controlled, else 'false'
		*
		*  @see
		*    - IsUserControlled()
		*/
		PLMESH_API void SetUserControlled(bool bUserControlled = false);

		/**
		*  @brief
		*    Calculates the current absolute translation and rotation states and the
		*    transform joint matrix
		*
		*  @param[in] pJointHandler
		*    Parent joint of this joint, if a null pointer there's no parent joint
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the handler has no joint or
		*    the given parent joint is invalid)
		*/
		PLMESH_API bool CalculateStates(const JointHandler *pJointHandler = nullptr);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This joint state
		*/
		PLMESH_API JointHandler &operator =(const JointHandler &cSource);

		/**
		*  @brief
		*    Compares two joint handlers
		*
		*  @param[in] cJointHandler
		*    Joint handler to compare with
		*
		*  @return
		*    'true' if both joint handler are equal, else 'false'
		*/
		PLMESH_API bool operator ==(const JointHandler &cJointHandler) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bUserControlled;	/**< Is this joint user controlled? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_JOINTHANDLER_H__
