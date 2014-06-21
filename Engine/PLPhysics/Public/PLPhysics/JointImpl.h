/*********************************************************\
 *  File: JointImpl.h                                    *
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


#ifndef __PLPHYSICS_JOINTIMPL_H__
#define __PLPHYSICS_JOINTIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/PLPhysics.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector3;
}
namespace PLPhysics {
	class Joint;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics joint implementation base class
*/
class JointImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Joint;


	//[-------------------------------------------------------]
	//[ Public virtual JointImpl functions                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the PL physics joint this is the implementation from
		*
		*  @return
		*    The PL physics joint this is the implementation from, a null pointer on error
		*/
		virtual Joint *GetJoint() const = 0;

		// For comments, see Joint
		virtual bool IsBreakable() const = 0;
		virtual void SetBreakable(bool bBreakable) = 0;
		virtual void GetBreakForce(PLMath::Vector3 &vForce) const = 0;
		virtual void SetBreakForce(const PLMath::Vector3 &vForce) = 0;
		virtual void GetBreakTorque(PLMath::Vector3 &vTorque) const = 0;
		virtual void SetBreakTorque(const PLMath::Vector3 &vTorque) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~JointImpl() {}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_JOINTIMPL_H__
