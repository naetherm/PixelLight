/*********************************************************\
 *  File: BodyImpl.h                                     *
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


#ifndef __PLPHYSICS_BODYIMPL_H__
#define __PLPHYSICS_BODYIMPL_H__
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
	class Matrix3x4;
	class Quaternion;
	class AABoundingBox;
}
namespace PLPhysics {
	class Body;
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
*    Abstract PL physics (rigid) body implementation base class
*/
class BodyImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Body;


	//[-------------------------------------------------------]
	//[ Public virtual BodyImpl functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the PL physics body this is the implementation from
		*
		*  @return
		*    The PL physics body this is the implementation from, a null pointer on error
		*/
		virtual Body *GetBody() const = 0;

		// For comments, see Body
		virtual bool IsActive() const = 0;
		virtual void SetActive(bool bActive = true) = 0;
		virtual float GetCollisionVolume() const = 0;
		virtual float GetMass() const = 0;
		virtual void SetMass(float fMass = 0.0f) = 0;
		virtual void GetCenterOfMass(PLMath::Vector3 &vPosition) const = 0;
		virtual void SetCenterOfMass(const PLMath::Vector3 &vPosition) = 0;
		virtual void GetPosition(PLMath::Vector3 &vPosition) const = 0;
		virtual void SetPosition(const PLMath::Vector3 &vPosition) = 0;
		virtual void GetRotation(PLMath::Quaternion &qRotation) const = 0;
		virtual void SetRotation(const PLMath::Quaternion &qRotation) = 0;
		virtual void GetTransformMatrix(PLMath::Matrix3x4 &mTrans) const = 0;
		virtual void SetTransformMatrix(const PLMath::Matrix3x4 &mTrans) = 0;
		virtual void GetAABoundingBox(PLMath::AABoundingBox &cAABoundingBox) const = 0;
		virtual void GetLinearVelocity(PLMath::Vector3 &vVelocity) const = 0;
		virtual void SetLinearVelocity(const PLMath::Vector3 &vVelocity) = 0;
		virtual void GetAngularVelocity(PLMath::Vector3 &vVelocity) const = 0;
		virtual void SetAngularVelocity(const PLMath::Vector3 &vVelocity) = 0;
		virtual void GetForce(PLMath::Vector3 &vForce) const = 0;
		virtual void AddForce(const PLMath::Vector3 &vForce) = 0;
		virtual void SetForce(const PLMath::Vector3 &vForce) = 0;
		virtual void GetTorque(PLMath::Vector3 &vTorque) const = 0;
		virtual void AddTorque(const PLMath::Vector3 &vTorque) = 0;
		virtual void SetTorque(const PLMath::Vector3 &vTorque) = 0;
		virtual bool IsAutoFreeze() const = 0;
		virtual void SetAutoFreeze(bool bAutoFreeze = true) = 0;
		virtual void GetFreezeThreshold(float &fLinearVelocity, float &fAngularVelocity, PLCore::uint32 &nSteps) const = 0;
		virtual void SetFreezeThreshold(float fLinearVelocity = 0.01f, float fAngularVelocity = 0.01f, PLCore::uint32 nSteps = 10) = 0;
		virtual bool IsFrozen() const = 0;
		virtual void SetFrozen(bool bFrozen = true) = 0;
		virtual bool IsCollisionActive() const = 0;
		virtual void SetCollisionActive(bool bActive = true) = 0;
		virtual bool GetUseGravity() const = 0;
		virtual void SetUseGravity(bool bUseGravity = true) = 0;
		virtual PLCore::uint8 GetCollisionGroup() const = 0;
		virtual void SetCollisionGroup(PLCore::uint8 nCollisionGroup = 0) = 0;
		virtual PLCore::uint32 GetNumOfPartners() const = 0;
		virtual Body *GetPartner(PLCore::uint32 nIndex) const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~BodyImpl() {}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_BODYIMPL_H__
