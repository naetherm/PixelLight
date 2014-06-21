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


#ifndef __PLPHYSICSNEWTON_WORLD_H__
#define __PLPHYSICSNEWTON_WORLD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/World.h>
#include "PLPhysicsNewton/PLPhysicsNewton.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLPhysics {
	class BodyImpl;
	class JointImpl;
}
namespace PLPhysicsNewton {
	class WorldUpdate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Newton physics world base class
*
*  @note
*    - By default, NewtonBodySetForceAndTorqueCallback() is set to PhysicsApplyGravityForce().
*      You can set another callback function is required.
*/
class World : public PLPhysics::World {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BodyImpl;
	friend class WorldThread;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICSNEWTON_RTTI_EXPORT, World, "PLPhysicsNewton", PLPhysics::World, "Newton physics world base class")
		// Attributes
		pl_attribute(CacheDirectory,		PLCore::String,	"",					ReadWrite,	DirectValue,	"Newton physics cache directory, if empty, no caching is used (best to avoid cache problems during development)",		"")

		// [TODO] New RTTI usage
		//pl_attribute(ThreadPriorityClass,	pl_enum_type(),		PriorityClassNone,	ReadWrite,	DirectValue,	"Physics thread priority class (use realtime priority class ONLY if you REALLY need it, 'None' = do not use a thread)",	"")
		//pl_attribute(ThreadPriority,		pl_enum_type(),		PriorityNormal,		ReadWrite,	DirectValue,	"Physics thread priority within the priority class it is in",															"")
		//DEFINE_VAR(PL_VAR_ENUM,		m_nThreadPriorityClass,	"",	"None",		"Physics thread priority class (use realtime priority class ONLY if you REALLY need it, 'None' = do not use a thread)",	"",	"Idle=0 BelowNormal=1 Normal=2 AboveNormal=3 High=4 Realtime=5 None=6")
		//DEFINE_VAR(PL_VAR_ENUM,		m_nThreadPriority,		"",		"Normal",	"Physics thread priority within the priority class it is in",															"",	"Idle=0 Lowest=1 BelowNormal=2 Normal=3 AboveNormal=4 Highest=5 TimeCritical=6")

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
		PLPHYSICSNEWTON_API World();

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICSNEWTON_API virtual ~World();

		/**
		*  @brief
		*    Returns the Newton physics world
		*
		*  @return
		*    The Newton physics world, a null pointer on error (if that's the case, something went totally wrong :)
		*/
		PLPHYSICSNEWTON_API Newton::NewtonWorld *GetNewtonWorld() const;

		/**
		*  @brief
		*    Returns the collision convex hull map
		*
		*  @return
		*    The collision convex hull map
		*/
		PLPHYSICSNEWTON_API PLCore::HashMap<PLCore::String, Newton::NewtonCollision*> &GetCollisionConvexHullMap();

		/**
		*  @brief
		*    Returns the collision mesh map
		*
		*  @return
		*    The collision mesh map
		*/
		PLPHYSICSNEWTON_API PLCore::HashMap<PLCore::String, Newton::NewtonCollision*> &GetCollisionMeshMap();

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
	//[ Public Newton callback functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Serialization callback function
		*
		*  @param[in] pSerializeHandle
		*    Pointer to a PixelLight File-object (always valid!)
		*  @param[in] pBuffer
		*    Pointer to the data to save (always valid!)
		*  @param[in] nSize
		*    Size of the data to save (in bytes)
		*/
		static PLPHYSICSNEWTON_API void PhysicsSerialize(void *pSerializeHandle, const void *pBuffer, int nSize);

		/**
		*  @brief
		*    De-serialization callback function
		*
		*  @param[in] pSerializeHandle
		*    Pointer to a PixelLight File-object (always valid!)
		*  @param[in] pBuffer
		*    Pointer to the data to load (always valid!)
		*  @param[in] nSize
		*    Size of the data to load (in bytes)
		*/
		static PLPHYSICSNEWTON_API void PhysicsDeserialize(void *pSerializeHandle, void *pBuffer, int nSize);

		/**
		*  @brief
		*    Gets the buoyancy plane callback function
		*
		*  @param[in]  nCollisionID
		*    Collision ID
		*  @param[in]  pContext
		*    Content, can be a null pointer
		*  @param[in]  pGlobalSpaceMatrix
		*    Global space matrix (always valid!)
		*  @param[out] pGlobalSpacePlane
		*    Will receive the global space planes (always valid!)
		*/
		static PLPHYSICSNEWTON_API int PhysicsGetBuoyancyPlane(const int nCollisionID, void *pContext, const dFloat *pGlobalSpaceMatrix, dFloat *pGlobalSpacePlane);

		/**
		*  @brief
		*    Applies gravity force to a rigid body callback function
		*
		*  @param[in] pPhysicsBody
		*    Newton body (always valid!)
		*  @param[in] fTimeStep
		*    Time step
		*  @param[in] nThreadIndex
		*    Thread index
		*/
		static PLPHYSICSNEWTON_API void PhysicsApplyGravityForce(const Newton::NewtonBody *pPhysicsBody, dFloat fTimeStep, int nThreadIndex);

		/**
		*  @brief
		*    Set the transformation of a rigid body callback function
		*
		*  @param[in] pPhysicsBody
		*    Newton body (always valid!)
		*  @param[in] pfMatrix
		*    Newton body transform matrix (always valid!)
		*  @param[in] nThreadIndex
		*    Thread index
		*/
		static PLPHYSICSNEWTON_API void PhysicsSetTransform(const Newton::NewtonBody *pPhysicsBody, const dFloat *pfMatrix, int nThreadIndex);

		/**
		*  @brief
		*    Rigid body destructor callback function
		*
		*  @param[in] pPhysicsBody
		*    Newton body (always valid!)
		*/
		static PLPHYSICSNEWTON_API void PhysicsBodyDestructor(const Newton::NewtonBody *pPhysicsBody);

		/**
		*  @brief
		*    Joint destructor callback function
		*
		*  @param[in] pPhysicsJoint
		*    Newton joint (always valid!)
		*/
		static PLPHYSICSNEWTON_API void PhysicsJointDestructor(const Newton::NewtonJoint *pPhysicsJoint);


	//[-------------------------------------------------------]
	//[ Private Newton callback functions                     ]
	//[-------------------------------------------------------]
	private:
		static void *PhysicsAllocMemory(int nSizeInBytes);
		static void PhysicsFreeMemory(void *pPtr, int nSizeInBytes);
		static int OnAABBOverlap(const Newton::NewtonMaterial *pMaterial, const Newton::NewtonBody *pNewtonBody1, const Newton::NewtonBody *pNewtonBody2, int nThreadIndex);
		static void ContactsProcess(const Newton::NewtonJoint *pContact, dFloat fTimeStep, int nThreadIndex);
		static int IslandUpdate(const Newton::NewtonWorld *pNewtonWorld, const void *pIslandHandle, int nBodyCount);
		static void WakeUp(const Newton::NewtonBody *pNewtonBody, void *pUserData);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal body pair
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
		// [TODO] New RTTI usage
		PLCore::uint32 m_nThreadPriorityClass;
		PLCore::uint32 m_nThreadPriority;

		// Private data
		Newton::NewtonWorld *m_pNewtonWorld;		/**< The Newton physics world, can be a null pointer */
		WorldUpdate			*m_pWorldUpdate;		/**< Physics update object, can be a null pointer */
		PLMath::Vector3		 m_vWorldSizeMin;
		PLMath::Vector3		 m_vWorldSizeMax;
		float				 m_fFrameRate;
		bool				 m_bSimulationActive;
		float				 m_fSimulationSpeed;
		float				 m_fSimulationQuality;
		PLMath::Vector3		 m_vGravity;
		PLCore::uint32		 m_nGroupCollision[32];	/**< Group collision active states */
		bool				 m_bBuoyancyActive;
		float				 m_fBuoyancyPlaneY;

		// Lists of changed bodies
		PLCore::Pool<PLPhysics::Body*> m_lstChangedByUser;
		PLCore::Pool<PLPhysics::Body*> m_lstChangedByPhysics;

		// Body pair flags
		PLCore::HashMap<BodyPair, PLCore::uint8, MyHashFunction, MyCompareFunction> m_mapBodyPairs;

		// Collision maps
		PLCore::HashMap<PLCore::String, Newton::NewtonCollision*> m_mapCollisionConvexHull;
		PLCore::HashMap<PLCore::String, Newton::NewtonCollision*> m_mapCollisionMesh;


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::World functions             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Physics body creation                                 ]
		//[-------------------------------------------------------]
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyBox(const PLMath::Vector3 &vDimension, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodySphere(float fRadius, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyEllipsoid(const PLMath::Vector3 &vRadius, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyConvexHull(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bOptimize = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyTerrain(PLCore::uint32 nWidth, PLCore::uint32 nHeight, const float fTerrain[], const PLMath::Vector3 &vBoxMin, const PLMath::Vector3 &vBoxMax, const PLMath::Vector3 &vScale) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyCylinder(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyCone(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyCapsule(float fRadius, float fHeight, bool bStatic = false) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Body *CreateBodyChamferCylinder(float fRadius, float fHeight, bool bStatic = false) override;

		//[-------------------------------------------------------]
		//[ Physics joint creation                                ]
		//[-------------------------------------------------------]
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *CreateJointBall(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *CreateJointSlider(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *CreateJointHinge(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *CreateJointUniversal(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir1, const PLMath::Vector3 &vPinDir2) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *CreateJointCorkscrew(PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *CreateJointUpVector(PLPhysics::Body &cParentBody, const PLMath::Vector3 &vPinDir) override;

		//[-------------------------------------------------------]
		//[ Physics sensor creation                               ]
		//[-------------------------------------------------------]
		PLPHYSICSNEWTON_API virtual PLPhysics::Sensor *CreateSensorRaycast(const PLMath::Vector3 &vStart = PLMath::Vector3::Zero, const PLMath::Vector3 &vEnd = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) override;
		PLPHYSICSNEWTON_API virtual PLPhysics::Sensor *CreateSensorAABoundingBox(const PLMath::Vector3 &vMin = PLMath::Vector3::Zero, const PLMath::Vector3 &vMax = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) override;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		PLPHYSICSNEWTON_API virtual void GetWorldSize(PLMath::Vector3 &vMin, PLMath::Vector3 &vMax) const override;
		PLPHYSICSNEWTON_API virtual void SetWorldSize(const PLMath::Vector3 &vMin = PLMath::Vector3(-10000.0f, -10000.0f, -10000.0f), const PLMath::Vector3 &vMax = PLMath::Vector3( 10000.0f,  10000.0f,  10000.0f)) override;
		PLPHYSICSNEWTON_API virtual bool IsSimulationActive() const override;
		PLPHYSICSNEWTON_API virtual void SetSimulationActive(bool bActive = true) override;
		PLPHYSICSNEWTON_API virtual float GetSimulationSpeed() const override;
		PLPHYSICSNEWTON_API virtual bool SetSimulationSpeed(float fSpeed = 1.0f) override;
		PLPHYSICSNEWTON_API virtual float GetSimulationQuality() const override;
		PLPHYSICSNEWTON_API virtual bool SetSimulationQuality(float fQuality = 1.0f) override;
		PLPHYSICSNEWTON_API virtual float GetFrameRate() const override;
		PLPHYSICSNEWTON_API virtual bool SetFrameRate(float fFrameRate = 60.0f) override;
		PLPHYSICSNEWTON_API virtual void GetGravity(PLMath::Vector3 &vGravity) const override;
		PLPHYSICSNEWTON_API virtual void SetGravity(const PLMath::Vector3 &vGravity = PLMath::Vector3(0.0f, -9.81f, 0.0f)) override;
		PLPHYSICSNEWTON_API virtual bool GetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2) const override;
		PLPHYSICSNEWTON_API virtual void SetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2, bool bActive = true) override;
		PLPHYSICSNEWTON_API virtual PLCore::uint8 GetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2) const override;
		PLPHYSICSNEWTON_API virtual void SetBodyPairFlags(const PLPhysics::Body &cBody1, const PLPhysics::Body &cBody2, PLCore::uint8 nFlags = 0) override;
		PLPHYSICSNEWTON_API virtual bool IsBuoyancyActive() const override;
		PLPHYSICSNEWTON_API virtual void SetBuoyancyActive(bool bActive = false) override;
		PLPHYSICSNEWTON_API virtual float GetBuoyancyPlaneY() const override;
		PLPHYSICSNEWTON_API virtual void SetBuoyancyPlaneY(float fY = 0.0f) override;
		PLPHYSICSNEWTON_API virtual void UpdateSimulation() override;
		PLPHYSICSNEWTON_API virtual bool IsAlwaysStatic() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual PLPhysics::Element *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton


#endif // __PLPHYSICSNEWTON_WORLD_H__
