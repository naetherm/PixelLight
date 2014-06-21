/*********************************************************\
 *  File: JointUpVector.h                                *
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


#ifndef __PLPHYSICS_JOINTUPVECTOR_H__
#define __PLPHYSICS_JOINTUPVECTOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/Joint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics up vector joint
*
*  @remarks
*    This function creates an up vector joint. An up vector joint is a constraint that allows a body
*    to translate freely in 3d space, but it only allows the body to rotate around the pin direction
*    vector. This could be use by the application to control a character with physics and collision
*    Since the up vector joint is a unuary constraint, there is not need to have user callback or
*    user data assigned to it. The application can simple hold to the joint handle and update the pin
*    on the force callback function of the rigid body owning the joint.
*
*  @note
*    - This joint type has no parent
*/
class JointUpVector : public Joint {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~JointUpVector();

		/**
		*  @brief
		*    Returns the aligning vector in world space
		*
		*  @return
		*    The aligning vector in world space
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this joint is in
		*  @param[in] cJointImpl
		*    Reference to the physics API specific joint implementation
		*  @param[in] cParentBody
		*    Reference to the parent rigid body
		*  @param[in] vPinDir
		*    The aligning vector in world space
		*/
		PLPHYSICS_API JointUpVector(World &cWorld, JointImpl &cJointImpl, Body &cParentBody, const PLMath::Vector3 &vPinDir);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector3 m_vPinDir;	/**< The aligning vector in world space */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_JOINTUPVECTOR_H__
