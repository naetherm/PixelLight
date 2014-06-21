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


#ifndef __PLPHYSICSNEWTON_BODYIMPL_H__
#define __PLPHYSICSNEWTON_BODYIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLMath/Quaternion.h>
#include <PLPhysics/BodyImpl.h>
#include "PLPhysicsNewton/PLPhysicsNewton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Newton physics (rigid) body implementation class
*/
class BodyImpl : public PLPhysics::BodyImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class World;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the Newton physics body
		*
		*  @return
		*    The Newton physics body, a null pointer on error (if that's the case, something went totally wrong :)
		*
		*  @note
		*    - Do NOT change the user data of the Newton physics body! (using NewtonBodySetUserData())
		*      This data MUST hold a pointer to the PL physics body.
		*/
		PLPHYSICSNEWTON_API Newton::NewtonBody *GetNewtonBody() const;

		/**
		*  @brief
		*    Flushes the body
		*
		*  @remarks
		*    Sets for instance the current transform matrix of the Newton physics body.
		*/
		PLPHYSICSNEWTON_API void Flush();

		/**
		*  @brief
		*    Initializes the Newton physics body
		*
		*  @param[in] cBody
		*    PL physics body this is the implementation from
		*  @param[in] cNewtonBody
		*    Newton physics body
		*  @param[in] fCollisionVolume
		*    Collision volume
		*/
		void InitializeNewtonBody(PLPhysics::Body &cBody, Newton::NewtonBody &cNewtonBody, float fCollisionVolume);

		/**
		*  @brief
		*    Sets the Newton physics body freeze state
		*
		*  @param[in] bFreeze
		*    Freeze the physics body?
		*
		*  @note
		*    - The physics body is only unfrozen if it's currently active
		*/
		void SetNewtonBodyFreezeState(bool bFreeze);


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::BodyImpl functions          ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *GetBody() const override;
		PLPHYSICSNEWTON_API virtual bool IsActive() const override;
		PLPHYSICSNEWTON_API virtual void SetActive(bool bActive = true) override;
		PLPHYSICSNEWTON_API virtual float GetCollisionVolume() const override;
		PLPHYSICSNEWTON_API virtual float GetMass() const override;
		PLPHYSICSNEWTON_API virtual void SetMass(float fMass = 0.0f) override;
		PLPHYSICSNEWTON_API virtual void GetCenterOfMass(PLMath::Vector3 &vPosition) const override;
		PLPHYSICSNEWTON_API virtual void SetCenterOfMass(const PLMath::Vector3 &vPosition) override;
		PLPHYSICSNEWTON_API virtual void GetPosition(PLMath::Vector3 &vPosition) const override;
		PLPHYSICSNEWTON_API virtual void SetPosition(const PLMath::Vector3 &vPosition) override;
		PLPHYSICSNEWTON_API virtual void GetRotation(PLMath::Quaternion &qRotation) const override;
		PLPHYSICSNEWTON_API virtual void SetRotation(const PLMath::Quaternion &qRotation) override;
		PLPHYSICSNEWTON_API virtual void GetTransformMatrix(PLMath::Matrix3x4 &mTrans) const override;
		PLPHYSICSNEWTON_API virtual void SetTransformMatrix(const PLMath::Matrix3x4 &mTrans) override;
		PLPHYSICSNEWTON_API virtual void GetAABoundingBox(PLMath::AABoundingBox &cAABoundingBox) const override;
		PLPHYSICSNEWTON_API virtual void GetLinearVelocity(PLMath::Vector3 &vVelocity) const override;
		PLPHYSICSNEWTON_API virtual void SetLinearVelocity(const PLMath::Vector3 &vVelocity) override;
		PLPHYSICSNEWTON_API virtual void GetAngularVelocity(PLMath::Vector3 &vVelocity) const override;
		PLPHYSICSNEWTON_API virtual void SetAngularVelocity(const PLMath::Vector3 &vVelocity) override;
		PLPHYSICSNEWTON_API virtual void GetForce(PLMath::Vector3 &vForce) const override;
		PLPHYSICSNEWTON_API virtual void AddForce(const PLMath::Vector3 &vForce) override;
		PLPHYSICSNEWTON_API virtual void SetForce(const PLMath::Vector3 &vForce) override;
		PLPHYSICSNEWTON_API virtual void GetTorque(PLMath::Vector3 &vTorque) const override;
		PLPHYSICSNEWTON_API virtual void AddTorque(const PLMath::Vector3 &vTorque) override;
		PLPHYSICSNEWTON_API virtual void SetTorque(const PLMath::Vector3 &vTorque) override;
		PLPHYSICSNEWTON_API virtual bool IsAutoFreeze() const override;
		PLPHYSICSNEWTON_API virtual void SetAutoFreeze(bool bAutoFreeze = true) override;
		PLPHYSICSNEWTON_API virtual bool IsFrozen() const override;
		PLPHYSICSNEWTON_API virtual void SetFrozen(bool bFrozen = true) override;
		PLPHYSICSNEWTON_API virtual bool IsCollisionActive() const override;
		PLPHYSICSNEWTON_API virtual void SetCollisionActive(bool bActive = true) override;
		PLPHYSICSNEWTON_API virtual bool GetUseGravity() const override;
		PLPHYSICSNEWTON_API virtual void SetUseGravity(bool bUseGravity = true) override;
		PLPHYSICSNEWTON_API virtual void GetFreezeThreshold(float &fLinearVelocity, float &fAngularVelocity, PLCore::uint32 &nSteps) const override;
		PLPHYSICSNEWTON_API virtual void SetFreezeThreshold(float fLinearVelocity = 0.01f, float fAngularVelocity = 0.01f, PLCore::uint32 nSteps = 10) override;
		PLPHYSICSNEWTON_API virtual PLCore::uint8 GetCollisionGroup() const override;
		PLPHYSICSNEWTON_API virtual void SetCollisionGroup(PLCore::uint8 nCollisionGroup = 0) override;
		PLPHYSICSNEWTON_API virtual PLCore::uint32 GetNumOfPartners() const override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *GetPartner(PLCore::uint32 nIndex) const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		BodyImpl();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BodyImpl();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Changed flags
		*/
		enum EChangedFlags {
			Position		= 1<<0,
			Rotation		= 1<<1,
			LinearVelocity	= 1<<2,
			AngularVelocity	= 1<<3,
			Force			= 1<<4,
			Torque			= 1<<5,
			Freeze			= 1<<6,
			Unfreeze		= 1<<7
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Newton::NewtonBody *m_pNewtonBody;				/**< Newton physics body, can be a null pointer */
		bool			    m_bActive;					/**< Currently active? */
		float				m_fCollisionVolume;			/**< Collision volume */
		float				m_fMass;					/**< Mass of the physics body */
		PLMath::Vector3		m_vPosition;				/**< Body position */
		PLMath::Quaternion	m_qRotation;				/**< Body rotation */
		PLMath::Vector3		m_vLinearVelocity;			/**< Linear body velocity */
		PLMath::Vector3		m_vAngularVelocity;			/**< Angular body velocity */
		PLMath::Vector3		m_vForce;					/**< Force applied to the body at the next simulation update */
		PLMath::Vector3		m_vNewtonForce;				/**< Newton force applied to the body at the next simulation update */
		PLMath::Vector3		m_vTorque;					/**< Torque applied to the body at the next simulation update */
		PLMath::Vector3		m_vNewtonTorque;			/**< Newton torque applied to the body at the next simulation update */
		bool				m_bFrozen;					/**< Is the body currently frozen? */
		bool				m_bAutoFreeze;				/**< Automatically freeze the body? */
		float				m_fFreezeLinearVelocity;	/**< Linear velocity freeze threshold */
		float				m_fFreezeAngularVelocity;	/**< Angular velocity freeze threshold */
		PLCore::uint32		m_nFreezeSteps;				/**< Number of steps the linear and angular velocity must be below the thresholds before the body is frozen */
		PLCore::uint32		m_nCurrentFreezeSteps;		/**< Current freeze steps */
		bool				m_bCollisionActive;			/**< Can the body collide? */
		bool				m_bUseGravity;				/**< Use gravity? */
		PLCore::uint8		m_nCollisionGroup;			/**< Collision group the body is in */
		PLCore::uint16		m_nChangedByUserFlags;		/**< Changed by user flags */
		PLCore::uint8		m_nChangedByPhysicsFlags;	/**< Changed by physics flags */

		/** List of partner bodies */
		PLCore::Array<PLPhysics::Body*> m_lstPartnerBodies;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton


#endif // __PLPHYSICSNEWTON_BODYIMPL_H__
