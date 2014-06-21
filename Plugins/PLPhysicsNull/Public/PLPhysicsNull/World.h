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


#ifndef __PLPHYSICSNULL_WORLD_H__
#define __PLPHYSICSNULL_WORLD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/World.h>
#include "PLPhysicsNull/PLPhysicsNull.h"


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
namespace PLPhysicsNull {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null physics world base class
*/
class World : public PLPhysics::World {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BodyImpl;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICSNULL_RTTI_EXPORT, World, "PLPhysicsNull", PLPhysics::World, "Null physics world base class")
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
		PLPHYSICSNULL_API World();

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICSNULL_API virtual ~World();

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
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal Body pair
		*/
		struct BodyPair {
			const PLPhysics::Body *pBody1;	/**< The first body (always valid!) */
			const PLPhysics::Body *pBody2;	/**< The second body (always valid!) */
		};


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal hash function
		*/
		class MyHashFunction {
			public:
				static PLCore::uint32 Hash(const BodyPair &sKey);
		};

		/**
		*  @brief
		*    Internal compare function
		*/
		class MyCompareFunction {
			public:
				static bool AreEqual(const BodyPair &sKey1, const BodyPair &sKey2);
				static bool IsGreater(const BodyPair &sKey1, const BodyPair &sKey2);
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3   m_vWorldSizeMin;
		PLMath::Vector3   m_vWorldSizeMax;
		float			  m_fFrameRate;
		bool			  m_bSimulationActive;
		float			  m_fSimulationSpeed;
		float			  m_fSimulationQuality;
		PLMath::Vector3   m_vGravity;
		PLCore::uint32 	  m_nGroupCollision[32];	/**< Group collision active states */
		bool			  m_bBuoyancyActive;
		float			  m_fBuoyancyPlaneY;

		PLCore::Pool<PLPhysics::Body*> m_lstChangedByUser;

		// Body pair flags
		PLCore::HashMap<BodyPair, PLCore::uint8, MyHashFunction, MyCompareFunction> m_mapBodyPairs;


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::World functions             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Physics body creation                                 ]
		//[-------------------------------------------------------]
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyBox(const PLMath::Vector3 &vDimension, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodySphere(float fRadius, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyEllipsoid(const PLMath::Vector3 &vRadius, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyConvexHull(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bOptimize = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyTerrain(PLCore::uint32 nWidth, PLCore::uint32 nHeight, const float fTerrain[], const PLMath::Vector3 &vBoxMin, const PLMath::Vector3 &vBoxMax, const PLMath::Vector3 &vScale) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyCylinder(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyCone(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyCapsule(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSNULL_API virtual PLPhysics::Body *CreateBodyChamferCylinder(float fRadius, float fHeight, bool bStatic = false) override;

		//[-------------------------------------------------------]
		//[ Physics joint creation                                ]
		//[-------------------------------------------------------]
		PLPHYSICSNULL_API virtual PLPhysics::Joint *CreateJointBall(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNULL_API virtual PLPhysics::Joint *CreateJointSlider(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNULL_API virtual PLPhysics::Joint *CreateJointHinge(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNULL_API virtual PLPhysics::Joint *CreateJointUniversal(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir1, const PLMath::Vector3 &vPinDir2) override;
		PLPHYSICSNULL_API virtual PLPhysics::Joint *CreateJointCorkscrew(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNULL_API virtual PLPhysics::Joint *CreateJointUpVector(PLPhysics::Body &cParentBody, const PLMath::Vector3 &vPinDir) override;

		//[-------------------------------------------------------]
		//[ Physics sensor creation                               ]
		//[-------------------------------------------------------]
		PLPHYSICSNULL_API virtual PLPhysics::Sensor *CreateSensorRaycast(const PLMath::Vector3 &vStart = PLMath::Vector3::Zero, const PLMath::Vector3 &vEnd = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) override;
		PLPHYSICSNULL_API virtual PLPhysics::Sensor *CreateSensorAABoundingBox(const PLMath::Vector3 &vMin = PLMath::Vector3::Zero, const PLMath::Vector3 &vMax = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) override;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		PLPHYSICSNULL_API virtual void GetWorldSize(PLMath::Vector3 &vMin, PLMath::Vector3 &vMax) const override;
		PLPHYSICSNULL_API virtual void SetWorldSize(const PLMath::Vector3 &vMin = PLMath::Vector3(-10000.0f, -10000.0f, -10000.0f), const PLMath::Vector3 &vMax = PLMath::Vector3( 10000.0f,  10000.0f,  10000.0f)) override;
		PLPHYSICSNULL_API virtual bool IsSimulationActive() const override;
		PLPHYSICSNULL_API virtual void SetSimulationActive(bool bActive = true) override;
		PLPHYSICSNULL_API virtual float GetSimulationSpeed() const override;
		PLPHYSICSNULL_API virtual bool SetSimulationSpeed(float fSpeed = 1.0f) override;
		PLPHYSICSNULL_API virtual float GetSimulationQuality() const override;
		PLPHYSICSNULL_API virtual bool SetSimulationQuality(float fQuality = 1.0f) override;
		PLPHYSICSNULL_API virtual float GetFrameRate() const override;
		PLPHYSICSNULL_API virtual bool SetFrameRate(float fFrameRate = 60.0f) override;
		PLPHYSICSNULL_API virtual void GetGravity(PLMath::Vector3 &vGravity) const override;
		PLPHYSICSNULL_API virtual void SetGravity(const PLMath::Vector3 &vGravity = PLMath::Vector3(0.0f, -9.81f, 0.0f)) override;
		PLPHYSICSNULL_API virtual bool GetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2) const override;
		PLPHYSICSNULL_API virtual void SetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2, bool bActive = true) override;
		PLPHYSICSNULL_API virtual PLCore::uint8 GetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2) const override;
		PLPHYSICSNULL_API virtual void SetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2, PLCore::uint8 nFlags = 0) override;
		PLPHYSICSNULL_API virtual bool IsBuoyancyActive() const override;
		PLPHYSICSNULL_API virtual void SetBuoyancyActive(bool bActive = false) override;
		PLPHYSICSNULL_API virtual float GetBuoyancyPlaneY() const override;
		PLPHYSICSNULL_API virtual void SetBuoyancyPlaneY(float fY = 0.0f) override;
		PLPHYSICSNULL_API virtual void UpdateSimulation() override;
		PLPHYSICSNULL_API virtual bool IsAlwaysStatic() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual PLPhysics::Element *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNull


#endif // __PLPHYSICSNULL_WORLD_H__
