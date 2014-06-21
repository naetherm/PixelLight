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


#ifndef __PLPHYSICS_WORLD_H__
#define __PLPHYSICS_WORLD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLCore/Base/Event/Event.h>
#include <PLCore/Container/Pool.h>
#include <PLCore/Container/ElementManager.h>
#include <PLMath/Vector3.h>
#include "PLPhysics/Element.h"
#include "PLPhysics/ElementHandler.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMesh {
	class MeshManager;
}
namespace PLPhysics {
	class Body;
	class Joint;
	class Sensor;
	class ContactInformation;
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
*    Abstract PL physics world (also called 'simulator') base class
*
*  @remarks
*    Because it's possible that the physics runs in an own thread, it's recommended to deactivate the simulation
*    by using SetSimulationActive(false) during scene creation.
*    Note that the behavior of the functions may differ a bit between the different physics implementations. For
*    instance, some physics implementations will ignore completely the linear velocity if the body is static. In general
*    you can't expect the same values as the ones set by yourself. For instance Body::GetLinearVelocity()
*    must not be vMyVelocity you set using Body::SetLinearVelocity(vMyVelocity)!
*/
class World : public PLCore::Object, public PLCore::ElementManager<Element> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Sensor;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Body pair flags
		*/
		enum EBodyPairFlags {
			Ignore = 1<<0	/**< Do NOT create contacts for this pair */
		};


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, World, "PLPhysics", PLCore::Object, "Abstract PL physics world (also called 'simulator') base class")
		// Signals
		pl_signal_1(SignalContact,	ContactInformation&,	"A contact between two bodies was detected by the physics. Contact information as parameter.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~World();


	//[-------------------------------------------------------]
	//[ Public virtual World functions                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Physics body creation                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates a physics box body
		*
		*  @param[in] vDimension
		*    Box dimension
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyBox(const PLMath::Vector3 &vDimension, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics sphere body
		*
		*  @param[in] fRadius
		*    Sphere radius
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodySphere(float fRadius, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics ellipsoid body
		*
		*  @param[in] vRadius
		*    Ellipsoid radius along each axis
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyEllipsoid(const PLMath::Vector3 &vRadius, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics convex hull body
		*
		*  @param[in] cMeshManager
		*    Mesh manager to use
		*  @param[in] sMesh
		*    Collision mesh
		*  @param[in] vMeshScale
		*    Mesh scale
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported? invalid mesh?)
		*
		*  @remarks
		*    The physics implementation is free to don't use the mesh at all. For example if no support or when
		*    using 'cached data'. Therefore this function is using a mesh manager and mesh name instead of a direct
		*    mesh instance to just load/use the mesh 'on demand'.
		*/
		virtual Body *CreateBodyConvexHull(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics mesh body
		*
		*  @param[in] cMeshManager
		*    Mesh manager to use
		*  @param[in] sMesh
		*    Collision mesh
		*  @param[in] vMeshScale
		*    Mesh scale
		*  @param[in] bOptimize
		*    Allow the physics API to optimize the mesh? (if supported)
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported? invalid mesh?)
		*
		*  @remarks
		*    The physics implementation is free to don't use the mesh at all. For example if no support or when
		*    using 'cached data'. Therefore this function is using a mesh manager and mesh name instead of a direct
		*    mesh instance to just load/use the mesh 'on demand'.
		*/
		virtual Body *CreateBodyMesh(PLMesh::MeshManager &cMeshManager, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale = PLMath::Vector3::One, bool bOptimize = false) = 0;

		/**
		*  @brief
		*    Creates a physics terrain body
		*
		*  @param[in] nWidth
		*    Terrain data width
		*  @param[in] nHeight
		*    Terrain data height
		*  @param[in] fTerrain
		*    Terrain data
		*  @param[in] vBoxMin
		*    Bounding box minimum
		*  @param[in] vBoxMax
		*    Bounding box maximum
		*  @param[in] vScale
		*    Scale
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyTerrain(PLCore::uint32 nWidth, PLCore::uint32 nHeight, const float fTerrain[], const PLMath::Vector3 &vBoxMin, const PLMath::Vector3 &vBoxMax, const PLMath::Vector3 &vScale) = 0;

		/**
		*  @brief
		*    Creates a physics cylinder body
		*
		*  @param[in] fRadius
		*    Cylinder radius at the base
		*  @param[in] fHeight
		*    Cylinder height along the x local axis from base to top
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyCylinder(float fRadius, float fHeight, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics cone body
		*
		*  @param[in] fRadius
		*    Cone radius at the base
		*  @param[in] fHeight
		*    Cone height along the x local axis from base to top
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyCone(float fRadius, float fHeight, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics capsule body
		*
		*  @param[in] fRadius
		*    Capsule radius at the base
		*  @param[in] fHeight
		*    Capsule height along the x local axis from base to top
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyCapsule(float fRadius, float fHeight, bool bStatic = false) = 0;

		/**
		*  @brief
		*    Creates a physics chamfer cylinder body
		*
		*  @param[in] fRadius
		*    Chamfer cylinder radius at the base
		*  @param[in] fHeight
		*    Chamfer cylinder height along the x local axis from base to top
		*  @param[in] bStatic
		*    Is this physics body static?
		*
		*  @return
		*    The created physics body, a null pointer on error (body type not supported?)
		*/
		virtual Body *CreateBodyChamferCylinder(float fRadius, float fHeight, bool bStatic = false) = 0;

		//[-------------------------------------------------------]
		//[ Physics joint creation                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates a physics ball and socket joint
		*
		*  @param[in] pParentBody
		*    Pointer to the parent rigid body, can be a null pointer
		*  @param[in] pChildBody
		*    Pointer to the attached rigid body, can be a null pointer
		*  @param[in] vPivotPoint
		*    Origin of the ball and socket in world space
		*  @param[in] vPinDir
		*    Vector defining the cone axis in world space
		*
		*  @return
		*    The created physics joint, a null pointer on error (joint type not supported?)
		*
		*  @note
		*    - If 'pParentBody' or 'pChildBody' is a null pointer, the joint is attached to the world (static joint)
		*    - ONLY 'pParentBody' OR 'pChildBody' can be a null pointer, if both are a null pointer this function will return a null pointer
		*    - If 'pParentBody' is the same as 'pChildBody' this function will return a null pointer
		*/
		virtual Joint *CreateJointBall(Body *pParentBody, Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) = 0;

		/**
		*  @brief
		*    Creates a physics slider joint
		*
		*  @param[in] pParentBody
		*    Pointer to the parent rigid body, can be a null pointer
		*  @param[in] pChildBody
		*    Pointer to the attached rigid body, can be a null pointer
		*  @param[in] vPivotPoint
		*    Origin of the slider in world space
		*  @param[in] vPinDir
		*    The line of action of the slider in world space
		*
		*  @return
		*    The created physics joint, a null pointer on error (joint type not supported?)
		*
		*  @see
		*    - CreateJointBall()
		*/
		virtual Joint *CreateJointSlider(Body *pParentBody, Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) = 0;

		/**
		*  @brief
		*    Creates a physics hinge joint
		*
		*  @param[in] pParentBody
		*    Pointer to the parent rigid body, can be a null pointer
		*  @param[in] pChildBody
		*    Pointer to the attached rigid body, can be a null pointer
		*  @param[in] vPivotPoint
		*    Origin of the hinge in world space
		*  @param[in] vPinDir
		*    The line of action of the hinge in world space
		*
		*  @return
		*    The created physics joint, a null pointer on error (joint type not supported?)
		*
		*  @see
		*    - CreateJointBall()
		*/
		virtual Joint *CreateJointHinge(Body *pParentBody, Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) = 0;

		/**
		*  @brief
		*    Creates a physics universal joint
		*
		*  @param[in] pParentBody
		*    Pointer to the parent rigid body, can be a null pointer
		*  @param[in] pChildBody
		*    Pointer to the attached rigid body, can be a null pointer
		*  @param[in] vPivotPoint
		*    Origin of the universal in world space
		*  @param[in] vPinDir1
		*    First axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in world space
		*  @param[in] vPinDir2
		*    Second axis of rotation fixed on 'Parent' body and perpendicular to 'PinDir1' in world space
		*
		*  @return
		*    The created physics joint, a null pointer on error (joint type not supported?)
		*
		*  @see
		*    - CreateJointBall()
		*/
		virtual Joint *CreateJointUniversal(Body *pParentBody, Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir1, const PLMath::Vector3 &vPinDir2) = 0;

		/**
		*  @brief
		*    Creates a physics corkscrew joint
		*
		*  @param[in] pParentBody
		*    Pointer to the parent rigid body, can be a null pointer
		*  @param[in] pChildBody
		*    Pointer to the attached rigid body, can be a null pointer
		*  @param[in] vPivotPoint
		*    Origin of the corkscrew in world space
		*  @param[in] vPinDir
		*    The line of action of the corkscrew in world space
		*
		*  @return
		*    The created physics joint, a null pointer on error (joint type not supported?)
		*
		*  @see
		*    - CreateJointBall()
		*/
		virtual Joint *CreateJointCorkscrew(Body *pParentBody, Body *pChildBody, const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir) = 0;

		/**
		*  @brief
		*    Creates a physics up vector joint
		*
		*  @param[in] cParentBody
		*    Reference to the parent rigid body
		*  @param[in] vPinDir
		*    The aligning vector in world space
		*
		*  @return
		*    The created physics joint, a null pointer on error (joint type not supported?)
		*/
		virtual Joint *CreateJointUpVector(Body &cParentBody, const PLMath::Vector3 &vPinDir) = 0;

		//[-------------------------------------------------------]
		//[ Physics sensor creation                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates a physics ray cast sensor
		*
		*  @param[in] vStart
		*    Beginning of the ray in global space
		*  @param[in] vEnd
		*    End of the ray in global space
		*  @param[in] nFlags
		*    Flags (see Sensor::EFlags)
		*
		*  @return
		*    The created physics sensor, a null pointer on error (sensor type not supported?)
		*/
		virtual Sensor *CreateSensorRaycast(const PLMath::Vector3 &vStart = PLMath::Vector3::Zero, const PLMath::Vector3 &vEnd = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) = 0;

		/**
		*  @brief
		*    Creates a physics axis aligned bounding box sensor
		*
		*  @param[in] vMin
		*    Minimum of the axis aligned bounding box in global space
		*  @param[in] vMax
		*    Maximum of the axis aligned bounding box in global space
		*  @param[in] nFlags
		*    Flags (see Sensor::EFlags)
		*
		*  @return
		*    The created physics sensor, a null pointer on error (sensor type not supported?)
		*/
		virtual Sensor *CreateSensorAABoundingBox(const PLMath::Vector3 &vMin = PLMath::Vector3::Zero, const PLMath::Vector3 &vMax = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0) = 0;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the world size
		*
		*  @param[out] vMin
		*    Receives the minimum world position
		*  @param[out] vMax
		*    Receives the maximum world position
		*
		*  @remarks
		*    Some physics API's may need a given world size in order to work correct. So, set one
		*    for sure.
		*/
		virtual void GetWorldSize(PLMath::Vector3 &vMin, PLMath::Vector3 &vMax) const = 0;

		/**
		*  @brief
		*    Sets the world size
		*
		*  @param[in] vMin
		*    Minimum world position
		*  @param[in] vMax
		*    Maximum world position
		*
		*  @see
		*    - GetWorldSize()
		*/
		virtual void SetWorldSize(const PLMath::Vector3 &vMin = PLMath::Vector3(-10000.0f, -10000.0f, -10000.0f),
								  const PLMath::Vector3 &vMax = PLMath::Vector3( 10000.0f,  10000.0f,  10000.0f)) = 0;

		/**
		*  @brief
		*    Returns whether the simulation is currently active or not
		*
		*  @return
		*    'true' if the simulation is currently active, else 'false'
		*
		*  @remarks
		*    If the physics simulation should be 'frozen', it's no good idea to skip just the
		*    UpdateSimulation() function because it's possible that the main physics processing is
		*    performed in another thread and this update function only performs state synchronization.
		*    So, in the case of just skipping this update function, the states visible to the user are not
		*    updated, but the physics itself is still running...
		*/
		virtual bool IsSimulationActive() const = 0;

		/**
		*  @brief
		*    Sets whether the simulation is currently active or not
		*
		*  @param[in] bActive
		*    'true' if the simulation is currently active, else 'false'
		*
		*  @see
		*    - IsSimulationActive()
		*/
		virtual void SetSimulationActive(bool bActive = true) = 0;

		/**
		*  @brief
		*    Returns the simulation speed
		*
		*  @return
		*    The simulation speed
		*/
		virtual float GetSimulationSpeed() const = 0;

		/**
		*  @brief
		*    Sets the simulation speed
		*
		*  @param[in] fSpeed
		*    The simulation speed, a speed of <= 0 is NOT allowed!
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
		*
		*  @note
		*    - A speed of <= 0 is NOT allowed because this may cause problems in certain
		*      situations, deactivate the simulation instead by hand!
		*    - Do NOT make the speed 'too' (for example > 4) extreme, this may cause
		*      problems in certain situations!
		*/
		virtual bool SetSimulationSpeed(float fSpeed = 1.0f) = 0;

		/**
		*  @brief
		*    Returns the simulation quality
		*
		*  @return
		*    The simulation quality, 1 means best realistic behavior, 0 for the fastest possible configuration
		*/
		virtual float GetSimulationQuality() const = 0;

		/**
		*  @brief
		*    Sets the simulation quality
		*
		*  @param[in] fQuality
		*    The simulation quality, 1 means best realistic behavior, 0 for the fastest possible configuration
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the quality factor is not within 0-1?)
		*/
		virtual bool SetSimulationQuality(float fQuality = 1.0f) = 0;

		/**
		*  @brief
		*    Returns the frame rate the simulation runs on
		*
		*  @return
		*    The frame rate the simulation runs on (>=60.0 and <=1000.0)
		*/
		virtual float GetFrameRate() const = 0;

		/**
		*  @brief
		*    Sets the frame rate the simulation runs on
		*
		*  @param[in] fFrameRate
		*    The frame rate the simulation runs on (>=60.0 and <=1000.0)
		*    (smaller=more performance, larger=more accurate simulation)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe the given value is NOT between >=60.0 and <=1000.0 ?)
		*/
		virtual bool SetFrameRate(float fFrameRate = 60.0f) = 0;

		/**
		*  @brief
		*    Returns the gravity vector
		*
		*  @param[out] vGravity
		*    Will receive the current gravity vector
		*/
		virtual void GetGravity(PLMath::Vector3 &vGravity) const = 0;

		/**
		*  @brief
		*    Sets the gravity vector
		*
		*  @param[in] vGravity
		*    New gravity vector
		*/
		virtual void SetGravity(const PLMath::Vector3 &vGravity = PLMath::Vector3(0.0f, -9.81f, 0.0f)) = 0;

		/**
		*  @brief
		*    Returns whether collision is active between the two given collision groups or not
		*
		*  @param[in] nGroup1
		*    First collision group(0-31)
		*  @param[in] nGroup2
		*    Second collision group(0-31)
		*
		*  @return
		*    'true' if the collision between the two given collision groups is active, else 'false'
		*
		*  @note
		*    - nGroup1 and nGroup2 are commutative
		*    - By default, collision between objects within the same group is disabled - except for the first group
		*/
		virtual bool GetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2) const = 0;

		/**
		*  @brief
		*    Sets whether collision is active between the two given collision groups or not
		*
		*  @param[in] nGroup1
		*    First collision group(0-31)
		*  @param[in] nGroup2
		*    Second collision group(0-31)
		*  @param[in] bActive
		*    'true' if the collision between the two given collision groups is active, else 'false'
		*
		*  @see
		*    - GetGroupCollision()
		*/
		virtual void SetGroupCollision(PLCore::uint8 nGroup1, PLCore::uint8 nGroup2, bool bActive = true) = 0;

		/**
		*  @brief
		*    Returns body pair flags
		*
		*  @param[in] cBody1
		*    The first body
		*  @param[in] cBody2
		*    The second body
		*
		*  @return
		*    Body pair flags (see EBodyPairFlags)
		*
		*  @note
		*    - GetBodyPairFlags() must NOT return the same flags set by SetBodyPairFlags()!
		*    - cBody1 and cBody2 are commutative
		*/
		virtual PLCore::uint8 GetBodyPairFlags(const Body &cBody1, const Body &cBody2) const = 0;

		/**
		*  @brief
		*    Sets the body pair flags
		*
		*  @param[in] cBody1
		*    The first body
		*  @param[in] cBody2
		*    The second body
		*  @param[in] nFlags
		*    Body pair flags (see EBodyPairFlags)
		*
		*  @see
		*    - SetBodyPairFlags()
		*/
		virtual void SetBodyPairFlags(const Body &cBody1, const Body &cBody2, PLCore::uint8 nFlags = 0) = 0;

		/**
		*  @brief
		*    Returns whether buoyancy force is used
		*
		*  @return
		*    'true' if buoyancy force is used, else 'false'
		*/
		virtual bool IsBuoyancyActive() const = 0;

		/**
		*  @brief
		*    Sets whether buoyancy force is used
		*
		*  @param[in] bActive
		*    'true' if buoyancy force is used, else 'false'
		*/
		virtual void SetBuoyancyActive(bool bActive = false) = 0;

		/**
		*  @brief
		*    Returns the y position of the buoyancy plane
		*
		*  @return
		*    The y position of the buoyancy plane
		*/
		virtual float GetBuoyancyPlaneY() const = 0;

		/**
		*  @brief
		*    Sets the y position of the buoyancy plane
		*
		*  @param[in] fY
		*    New y position of the buoyancy plane
		*/
		virtual void SetBuoyancyPlaneY(float fY = 0.0f) = 0;

		/**
		*  @brief
		*    Updates the simulation
		*
		*  @remarks
		*    If the simulation should be 'frozen', do NOT just skip this function, use
		*    SetSimulationActive(false) instead!
		*
		*  @see
		*    - SetSimulationActive()
		*/
		virtual void UpdateSimulation() = 0;

		/**
		*  @brief
		*    Returns whether the bodies are always static or not
		*
		*  @return
		*    'true' if bodies are always static, else 'false'
		*
		*  @remarks
		*    During the creation of a body using for instance World::CreateBodyBox() you can give the used physics API a hint whether
		*    this body is always static or not. Some physics API's will use this information, while other not. The Newton and ODE physics
		*    plugins for instance will ignore this information, you can change the static state by setting a mass directly while 0 means
		*    static, while >0 indicates a dynamic body. The PhysX physics plugin for instance will use this information. If a body is
		*    created this static set to true, this body is always static within the API even if you set a mass!
		*/
		virtual bool IsAlwaysStatic() const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLPHYSICS_API World();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Pool<void*> m_lstFreeBodyInformation;	/**< List of currently free body information */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_WORLD_H__
