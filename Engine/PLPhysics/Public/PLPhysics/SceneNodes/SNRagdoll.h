/*********************************************************\
 *  File: SNRagdoll.h                                    *
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


#ifndef __PLPHYSICS_SCENENODES_RAGDOLL_H__
#define __PLPHYSICS_SCENENODES_RAGDOLL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Loadable.h>
#include <PLScene/Scene/SceneNodes/SNMesh.h>
#include "PLPhysics/PLPhysics.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMesh {
	class Skeleton;
	class SkeletonHandler;
}
namespace PLScene {
	class SNMMeshUpdate;
}
namespace PLPhysics {
	class RagdollBody;
	class RagdollJoint;
	class SCPhysicsWorld;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Structures                                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Disables collision between two bodies
*/
struct NoCollision {
	PLCore::String sFirst;
	PLCore::String sSecond;
};

/**
*  @brief
*    Set of controller gains for a particular joint
*/
struct JointController {
	PLCore::String sJoint;
	bool		   bIsControllable;
	
	// ks and kd parameters for each axis (ks2, kd2 not applicable to hinge joints)
	float ks1, kd1;
	float ks2, kd2;
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ragdoll scene node
*
*  @remarks
*    The ragdoll (other names are 'online animation', 'articulated') scene node is an extension of
*    SNMesh and allows the skeleton of the mesh to be influenced by a physics simulation.
*/
class SNRagdoll : public PLScene::SNMesh, public PLCore::Loadable {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RagdollBody;
	friend class RagdollJoint;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node debug flags (SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			DebugNoPhysicsBodies = 1<<18	/**< Do not draw the physics bodies of the ragdoll */
		};
		pl_enum(EDebugFlags)
			pl_enum_base(PLScene::SNMesh::EDebugFlags)
			pl_enum_value(DebugNoPhysicsBodies, "Do not draw the physics bodies of the ragdoll")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, SNRagdoll, "PLPhysics", PLScene::SNMesh, "Ragdoll scene node")
		// Attributes
		pl_attribute(RagdollFilename,		PLCore::String,				"",				ReadWrite,	GetSet,			"Ragdoll filename, if no provided a default ragdoll is created",	"Type='Ragdoll'")
		pl_attribute(ControllerEnabled,		bool,						false,			ReadWrite,	DirectValue,	"Is the PD-controller enabled?",									"")
		pl_attribute(AttachToWorldEnabled,	bool,						false,			ReadWrite,	GetSet,			"Are joints allowed to be attached to the world? (fixed joints)",	"")
		pl_attribute(MaxTorque,				float,						60.0f,			ReadWrite,	DirectValue,	"Cap the torque generated by the controller to a reasonable value",	"")
		pl_attribute(AutoFreeze,			bool,						true,			ReadWrite,	GetSet,			"Freeze the ragdoll automatically?",								"")
		pl_attribute(UseGravity,			bool,						true,			ReadWrite,	GetSet,			"Does the gravity influence this ragdoll?",							"")
		pl_attribute(InitFrozen,			bool,						true,			ReadWrite,	DirectValue,	"Is this ragdoll frozen on initialization?",						"")
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),		DynamicMesh,	ReadWrite,	GetSet,			"Flags",															"")
		pl_attribute(DebugFlags,			pl_flag_type(EDebugFlags),	0,				ReadWrite,	GetSet,			"Debug flags",														"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API PLCore::String GetRagdollFilename() const;
		PLPHYSICS_API void SetRagdollFilename(const PLCore::String &sValue);
		PLPHYSICS_API bool GetAttachToWorldEnabled() const;
		PLPHYSICS_API void SetAttachToWorldEnabled(bool bValue);
		PLPHYSICS_API bool GetAutoFreeze() const;
		PLPHYSICS_API void SetAutoFreeze(bool bValue);
		PLPHYSICS_API bool GetUseGravity() const;
		PLPHYSICS_API void SetUseGravity(bool bValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPHYSICS_API SNRagdoll();

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNRagdoll();

		/**
		*  @brief
		*    Returns the PL physics world scene node container the ragdoll is in
		*
		*  @return
		*    The PL physics world scene node container the ragdoll is in, a null pointer on error
		*    (if that's the case, something went totally wrong :)
		*
		*  @note
		*    - This PL physics world scene node container can be another as the scene node container this scene node is in
		*/
		PLPHYSICS_API SCPhysicsWorld *GetWorldContainer() const;

		/**
		*  @brief
		*    Clears the radgoll
		*/
		PLPHYSICS_API void ClearRagdoll();

		/**
		*  @brief
		*    Creates the ragdoll using a skeleton handler
		*
		*  @param[in] pSkeletonHandler
		*    Skeleton handler to get the ragdoll information from, if a null pointer, a ragdoll is created automatically using
		*    the skeleton handler of the used mesh
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    This function will create a ragdoll from a skeleton handler for you automatically.
		*    After the ragdoll was created, you just have to tweak it if required.
		*
		*  @note
		*    - Only the main layout can be created using the skeleton handler
		*      (bodies, joints, positions, rotations etc.)
		*    - Default joint type: Universal
		*    - Default body size: 0.2 'length' 0.2
		*    - Default body mass: 1.0
		*    - The joint limits are between -0.5 and 0.5
		*/
		PLPHYSICS_API bool CreateRagdoll(PLMesh::SkeletonHandler *pSkeletonHandler = nullptr);

		/**
		*  @brief
		*    Creates the physics
		*
		*  @param[in] bAllowAttachToWorld
		*    Is it allowed to attach joints to the world?
		*
		*  @note
		*    - This function calls CreatePhysicsBodies() and CreatePhysicsJoints()
		*    - You can use this function to reset the ragdoll to the initial pose
		*      (after a ragdoll is loaded it's in the initial pose by default)
		*/
		PLPHYSICS_API void CreatePhysics(bool bAllowAttachToWorld = true);

		/**
		*  @brief
		*    Creates the physics bodies
		*
		*  @note
		*    - After this function all physics bodies are in their initial pose
		*
		*  @see
		*    - CreatePhysicsBodies()
		*/
		PLPHYSICS_API void CreatePhysicsBodies();

		/**
		*  @brief
		*    Creates the physics joints
		*
		*  @param[in] bAllowAttachToWorld
		*    Is it allowed to attach joints to the world?
		*
		*  @note
		*    - After this function all physics joints are in their initial pose
		*
		*  @see
		*    - CreatePhysicsBodies()
		*/
		PLPHYSICS_API void CreatePhysicsJoints(bool bAllowAttachToWorld = true);

		/**
		*  @brief
		*    Creates the physics ragdoll
		*
		*  @param[in] bAllowAttachToWorld
		*    Is it allowed to attach joints to the world?
		*/
		PLPHYSICS_API void CreatePhysicsRagdoll(bool bAllowAttachToWorld = true);

		/**
		*  @brief
		*    Returns the ragdoll skeleton
		*
		*  @return
		*    The ragdoll skeleton, a null pointer on error
		*/
		PLPHYSICS_API PLMesh::Skeleton *GetSkeleton() const;

		/**
		*  @brief
		*    Returns whether the ragdoll is enabled or not
		*
		*  @return
		*    'true' if the ragdoll is enabled, else 'false'
		*/
		PLPHYSICS_API bool IsEnabled() const;

		/**
		*  @brief
		*    Enables/disables all bodies and joints
		*
		*  @param[in] bEnabled
		*    'true' if the ragdoll is enabled, else 'false'
		*/
		PLPHYSICS_API void SetEnabled(bool bEnabled = true);

		/**
		*  @brief
		*    Returns the number of bodies
		*
		*  @return
		*    The number of bodies
		*/
		PLPHYSICS_API PLCore::uint32 GetNumOfBodies() const;

		/**
		*  @brief
		*    Returns a body
		*
		*  @param[in] nIndex
		*    Index of the requested body
		*
		*  @return
		*    The requested body, a null pointer on error
		*/
		PLPHYSICS_API RagdollBody *GetBody(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a body
		*
		*  @param[in] sName
		*    Name of the requested body
		*
		*  @return
		*    The requested body, a null pointer on error
		*/
		PLPHYSICS_API RagdollBody *GetBody(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Adds a new body
		*
		*  @param[in] sName
		*    Desired name
		*  @param[in] bCreatePhysics
		*    Create the physics body after the body was added?
		*
		*  @return
		*    The new ragdoll body, a null pointer on error
		*/
		PLPHYSICS_API RagdollBody *AddBody(const PLCore::String &sName = "Joint", bool bCreatePhysics = true);

		/**
		*  @brief
		*    Clones a body
		*
		*  @param[in] nIndex
		*    Index of the body to clone
		*  @param[in] bCreatePhysics
		*    Create the physics body after the cloned body was added?
		*
		*  @return
		*    The new cloned ragdoll body, a null pointer on error
		*/
		PLPHYSICS_API RagdollBody *CloneBody(PLCore::uint32 nIndex, bool bCreatePhysics = true);

		/**
		*  @brief
		*    Removes a body
		*
		*  @param[in] nIndex
		*    Index of the body to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLPHYSICS_API bool RemoveBody(PLCore::uint32 nIndex);

		/**
		*  @brief
		*    Removes a body and the joint attached to it
		*
		*  @param[in] nIndex
		*    Index of the body to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLPHYSICS_API bool RemoveBodyAndJoint(PLCore::uint32 nIndex);

		/**
		*  @brief
		*    Returns the number of joints
		*
		*  @return
		*    The number of joints
		*/
		PLPHYSICS_API PLCore::uint32 GetNumOfJoints() const;

		/**
		*  @brief
		*    Returns a joint
		*
		*  @param[in] nIndex
		*    Index of the requested joint
		*
		*  @return
		*    The requested joint, a null pointer on error
		*/
		PLPHYSICS_API RagdollJoint *GetJoint(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a joint
		*
		*  @param[in] sName
		*    Name of the requested joint
		*
		*  @return
		*    The requested joint, a null pointer on error
		*/
		PLPHYSICS_API RagdollJoint *GetJoint(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Adds a new joint
		*
		*  @param[in] sName
		*    Desired name
		*  @param[in] bCreatePhysics
		*    Create the physics joint after the joint was added?
		*
		*  @return
		*    The new ragdoll joint, a null pointer on error
		*/
		PLPHYSICS_API RagdollJoint *AddJoint(const PLCore::String &sName = "Joint", bool bCreatePhysics = true);

		/**
		*  @brief
		*    Clones a joint
		*
		*  @param[in] nIndex
		*    Index of the joint to clone
		*  @param[in] bCreatePhysics
		*    Create the physics joint after the cloned joint was added?
		*
		*  @return
		*    The new cloned ragdoll joint, a null pointer on error
		*/
		PLPHYSICS_API RagdollJoint *CloneJoint(PLCore::uint32 nIndex, bool bCreatePhysics = true);

		/**
		*  @brief
		*    Removes a joint
		*
		*  @param[in] nIndex
		*    Index of the joint to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLPHYSICS_API bool RemoveJoint(PLCore::uint32 nIndex);

		/**
		*  @brief
		*    Returns the number of no collisions
		*
		*  @return
		*    The number of no collisions
		*/
		PLPHYSICS_API PLCore::uint32 GetNumOfNoCollisions() const;

		/**
		*  @brief
		*    Adds a new no collision
		*
		*  @return
		*    The new no collision, a null pointer on error
		*/
		PLPHYSICS_API NoCollision *AddNoCollision();

		/**
		*  @brief
		*    Returns a no collision
		*
		*  @param[in] nIndex
		*    Index of the requested no collision
		*
		*  @return
		*    The requested no collision, a null pointer on error
		*/
		PLPHYSICS_API NoCollision *GetNoCollision(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Removes a no collision
		*
		*  @param[in] nIndex
		*    Index of the no collision to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLPHYSICS_API bool RemoveNoCollision(PLCore::uint32 nIndex);

		/**
		*  @brief
		*    Returns the number of controllers
		*
		*  @return
		*    The number of controllers
		*/
		PLPHYSICS_API PLCore::uint32 GetNumOfControllers() const;

		/**
		*  @brief
		*    Adds a new controller
		*
		*  @param[in] sJointName
		*    Name of the joint to control
		*
		*  @return
		*    The new (if not already created) controller, a null pointer on error
		*/
		PLPHYSICS_API JointController *AddController(const PLCore::String &sJointName);

		/**
		*  @brief
		*    Returns a controller
		*
		*  @param[in] nIndex
		*    Index of the requested controller
		*
		*  @return
		*    The requested controller, a null pointer on error
		*/
		PLPHYSICS_API JointController *GetController(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a controller
		*
		*  @param[in] sName
		*    Name of the requested controller
		*
		*  @return
		*    The requested controller, a null pointer on error
		*/
		PLPHYSICS_API JointController *GetController(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Removes a controller
		*
		*  @param[in] nIndex
		*    Index of the controller to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLPHYSICS_API bool RemoveController(PLCore::uint32 nIndex);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Computes and applies PD-controller torques to each body part
		*/
		void ApplyControlTorques();

		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();

		/**
		*  @brief
		*    Returns a "PLScene::SNMMeshUpdate" instance from the owner scene node
		*
		*  @return
		*    A "PLScene::SNMMeshUpdate" instance
		*
		*  @note
		*    - If not already there, this method creates an instance of the "PLScene::SNMMeshUpdate" scene node modifier which frequently updates the mesh
		*/
		PLScene::SNMMeshUpdate *GetSNMMeshUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<>	EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SCPhysicsWorld			*m_pWorldContainer;			/**< The PL physics world scene node container the physics body is in, can be a null pointer */
		bool					 m_bEnabled;				/**< Is the ragdoll enabled? */
		PLMesh::SkeletonHandler *m_pRagdollSkeletonHandler;	/**< Skeleton handler with the current ragdoll pose, can be a null pointer */
		PLCore::String			 m_sRagdollFilename;		/**< Ragdoll filename, if no provided a default ragdoll is created */
		bool					 m_bAttachToWorldEnabled;	/**< Are joints allowed to be attached to the world? (fixed joints) */
		bool					 m_bAutoFreeze;				/**< Freeze the ragdoll automatically? */
		bool					 m_bUseGravity;				/**< Does the gravity influence this ragdoll? */

		// Specifies size, position, orientation, and mass of each body part
		PLCore::Array<RagdollBody*>							m_lstBodies;
		PLCore::HashMap<PLCore::String, RagdollBody*>		m_mapBodies;
		// Specifies all the joints between each of the body parts and their properties
		PLCore::Array<RagdollJoint*>						m_lstJoints;
		PLCore::HashMap<PLCore::String, RagdollJoint*>		m_mapJoints;
		// Disables collision between bodies
		PLCore::Array<NoCollision*>							m_lstNoCollisions;
		// Manually specified controller gains for near critically damped behavior
		PLCore::Array<JointController*>						m_lstControllers;
		PLCore::HashMap<PLCore::String, JointController*>	m_mapControllers;


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNode functions        ]
	//[-------------------------------------------------------]
	protected:
		PLPHYSICS_API virtual void InitFunction() override;
		PLPHYSICS_API virtual void DeInitFunction() override;
		PLPHYSICS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API virtual bool Unload() override;
		PLPHYSICS_API virtual PLCore::String GetLoadableTypeName() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::Loadable functions          ]
	//[-------------------------------------------------------]
	protected:
		PLPHYSICS_API virtual bool CallLoadable(PLCore::File &cFile, PLCore::Loader &cLoader, const PLCore::String &sMethod, const PLCore::String &sParams) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODES_RAGDOLL_H__
