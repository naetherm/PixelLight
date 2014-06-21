/*********************************************************\
 *  File: World.h                                        *
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


#ifndef __PLPHYSICSBULLET_WORLD_H__
#define __PLPHYSICSBULLET_WORLD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/World.h>
#include "PLPhysicsBullet/PLPhysicsBullet.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLPhysics {
	class BodyImpl;
	class JointImpl;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Bullet physics world base class
*
*/
class World : public PLPhysics::World {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BodyImpl;
	friend class JointImpl;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICSBULLET_RTTI_EXPORT, World, "PLPhysicsBullet", PLPhysics::World, "Bullet physics world base class")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPHYSICSBULLET_API World();

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICSBULLET_API virtual ~World();

		/**
		*  @brief
		*    Returns the Bullet physics world
		*
		*  @return
		*    The Bullet physics world, a null pointer on error (if that's the case, something went totally wrong :)
		*/
		PLPHYSICSBULLET_API btDynamicsWorld *GetBulletWorld() const;

		/**
		*  @brief
		*    Creates a physics body implementation
		*
		*  @return
		*    The created physics body implementation
		*/
		PLPhysics::BodyImpl &CreateBodyImpl() const;

		/**
		*  @brief
		*    Creates a physics joint implementation
		*
		*  @return
		*    The created physics joint implementation
		*/
		PLPhysics::JointImpl &CreateJointImpl() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:

		// Private data
		btCollisionConfiguration	*m_pCollisionConfiguration;
		btDispatcher				*m_pDispatcher;
		btConstraintSolver			*m_pConstraintSolver;
		btBroadphaseInterface		*m_pBroadphaseInterface;
		btDynamicsWorld				*m_pBulletWorld;		/**< The Bullet physics world, can be a null pointer */
		PLMath::Vector3				 m_vWorldSizeMin;
		PLMath::Vector3				 m_vWorldSizeMax;
		float						 m_fFrameRate;
		bool						 m_bSimulationActive;
		float						 m_fSimulationSpeed;
		float						 m_fSimulationQuality;
		PLMath::Vector3				 m_vGravity;
		PLCore::uint32				 m_nGroupCollision[32];	/**< Group collision active states */
		bool						 m_bBuoyancyActive;
		float						 m_fBuoyancyPlaneY;
		
		PLCore::Pool<PLPhysics::Body*> m_lstChangedByUser;


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::World functions             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Physics body creation                                 ]
		//[-------------------------------------------------------]
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyBox(const PLMath::Vector3 &vDimension, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodySphere(float fRadius, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyEllipsoid(const PLMath::Vector3 &vRadius, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyConvexHull(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bOptimize = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyTerrain(PLCore::uint32 nWidth, PLCore::uint32 nHeight, const float fTerrain[], const PLMath::Vector3 &vBoxMin, const PLMath::Vector3 &vBoxMax, const PLMath::Vector3 &vScale) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyCylinder(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyCone(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyCapsule(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Body *CreateBodyChamferCylinder(float fRadius, float fHeight, bool bStatic = false) override;

		//[-------------------------------------------------------]
		//[ Physics joint creation                                ]
		//[-------------------------------------------------------]
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *CreateJointBall(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *CreateJointSlider(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *CreateJointHinge(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *CreateJointUniversal(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir1, const PLMath::Vector3 &vPinDir2) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *CreateJointCorkscrew(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *CreateJointUpVector(PLPhysics::Body &cParentBody, const PLMath::Vector3 &vPinDir) override;

		//[-------------------------------------------------------]
		//[ Physics sensor creation                               ]
		//[-------------------------------------------------------]
		PLPHYSICSBULLET_API virtual PLPhysics::Sensor *CreateSensorRaycast(const PLMath::Vector3 &vStart = PLMath::Vector3::Zero, const PLMath::Vector3 &vEnd = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) override;
		PLPHYSICSBULLET_API virtual PLPhysics::Sensor *CreateSensorAABoundingBox(const PLMath::Vector3 &vMin = PLMath::Vector3::Zero, const PLMath::Vector3 &vMax = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) override;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		PLPHYSICSBULLET_API virtual void GetWorldSize(PLMath::Vector3 &vMin, PLMath::Vector3 &vMax) const override;
		PLPHYSICSBULLET_API virtual void SetWorldSize(const PLMath::Vector3 &vMin = PLMath::Vector3(-10000.0f, -10000.0f, -10000.0f), const PLMath::Vector3 &vMax = PLMath::Vector3( 10000.0f,  10000.0f,  10000.0f)) override;
		PLPHYSICSBULLET_API virtual bool IsSimulationActive() const override;
		PLPHYSICSBULLET_API virtual void SetSimulationActive(bool bActive = true) override;
		PLPHYSICSBULLET_API virtual float GetSimulationSpeed() const override;
		PLPHYSICSBULLET_API virtual bool SetSimulationSpeed(float fSpeed = 1.0f) override;
		PLPHYSICSBULLET_API virtual float GetSimulationQuality() const override;
		PLPHYSICSBULLET_API virtual bool SetSimulationQuality(float fQuality = 1.0f) override;
		PLPHYSICSBULLET_API virtual float GetFrameRate() const override;
		PLPHYSICSBULLET_API virtual bool SetFrameRate(float fFrameRate = 60.0f) override;
		PLPHYSICSBULLET_API virtual void GetGravity(PLMath::Vector3 &vGravity) const override;
		PLPHYSICSBULLET_API virtual void SetGravity(const PLMath::Vector3 &vGravity = PLMath::Vector3(0.0f, -9.81f, 0.0f)) override;
		PLPHYSICSBULLET_API virtual bool GetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2) const override;
		PLPHYSICSBULLET_API virtual void SetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2, bool bActive = true) override;
		PLPHYSICSBULLET_API virtual PLCore::uint8 GetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2) const override;
		PLPHYSICSBULLET_API virtual void SetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2, PLCore::uint8 nFlags = 0) override;
		PLPHYSICSBULLET_API virtual bool IsBuoyancyActive() const override;
		PLPHYSICSBULLET_API virtual void SetBuoyancyActive(bool bActive = false) override;
		PLPHYSICSBULLET_API virtual float GetBuoyancyPlaneY() const override;
		PLPHYSICSBULLET_API virtual void SetBuoyancyPlaneY(float fY = 0.0f) override;
		PLPHYSICSBULLET_API virtual void UpdateSimulation() override;
		PLPHYSICSBULLET_API virtual bool IsAlwaysStatic() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual PLPhysics::Element *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsBullet


#endif // __PLPHYSICSBULLET_WORLD_H__
