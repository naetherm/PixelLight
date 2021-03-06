/*********************************************************\
 *  File: SNMPhysicsBody.h                               *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_BODY_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_BODY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/SceneNodeModifiers/SNMPhysics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Body;
class ElementHandler;
class SCPhysicsWorld;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract physics body scene node modifier
*
*  @note
*    - During runtime, do NOT change the scale of the body scene node itself!
*    - This scene container MUST be within a SCPhysicsWorld scene container - else
*      this physics body will have no physics behavior at all
*    - If there's no mass, the body is static
*/
class SNMPhysicsBody : public SNMPhysics {


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags (PLScene::SceneNodeModifier flags extension)
		*/
		enum EFlags {
			InitUnfrozen = 1<<2,	/**< The body is not frozen on initialization */
			NoAutoFreeze = 1<<3,	/**< Do not freeze the body automatically */
			NoGravity    = 1<<4,	/**< The body is not influenced by gravity */
			NoRotation   = 1<<5		/**< Rotation is not used */
		};
		pl_flag(EFlags)
			pl_enum_base(SNMPhysics::EFlags)
			pl_enum_value(InitUnfrozen,	"The body is not frozen on initialization")
			pl_enum_value(NoAutoFreeze,	"Do not freeze the body automatically")
			pl_enum_value(NoGravity,	"The body is not influenced by gravity")
			pl_enum_value(NoRotation,	"Rotation is not used")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLPHYSICS_API)
		// Attributes
		pl_attribute_getset(SNMPhysicsBody,	Mass,			float,				0.0f,					ReadWrite)
		pl_attribute_getset(SNMPhysicsBody,	CenterOfMass,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_getset(SNMPhysicsBody,	PositionOffset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_getset(SNMPhysicsBody,	CollisionGroup,	PLCore::uint8,		0,						ReadWrite)
			// Overwritten PLScene::SceneNodeModifier attributes
		pl_attribute_getset(SNMPhysicsBody,	Flags,			PLCore::uint32,		0,						ReadWrite)
		// Slots
		pl_slot_0_def(SNMPhysicsBody,	OnActive)
		pl_slot_0_def(SNMPhysicsBody,	OnPosition)
		pl_slot_0_def(SNMPhysicsBody,	OnRotation)
		pl_slot_0_def(SNMPhysicsBody,	OnTransform)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API float GetMass() const;
		PLPHYSICS_API void SetMass(float fValue);
		PLPHYSICS_API const PLMath::Vector3 &GetCenterOfMass() const;
		PLPHYSICS_API void SetCenterOfMass(const PLMath::Vector3 &vValue);
		PLPHYSICS_API const PLMath::Vector3 &GetPositionOffset() const;
		PLPHYSICS_API void SetPositionOffset(const PLMath::Vector3 &vValue);
		PLPHYSICS_API PLCore::uint8 GetCollisionGroup() const;
		PLPHYSICS_API void SetCollisionGroup(PLCore::uint8 nValue);
		PLPHYSICS_API virtual void SetFlags(PLCore::uint32 nValue) override;	// From PLScene::SceneNodeModifier


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the PL physics world scene node container the physics body is in
		*
		*  @return
		*    The PL physics world scene node container the physics body is in, a null pointer on error
		*    (if that's the case, something went totally wrong :)
		*
		*  @note
		*    - This PL physics world scene node container can be another as the scene node container this scene node is in
		*/
		PLPHYSICS_API SCPhysicsWorld *GetWorldContainer() const;

		/**
		*  @brief
		*    Returns the PL physics body
		*
		*  @return
		*    The PL physics body, a null pointer on error (if that's the case, something went totally wrong :)
		*/
		PLPHYSICS_API Body *GetBody() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLPHYSICS_API SNMPhysicsBody(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsBody();

		/**
		*  @brief
		*    Recreates the PL physics body
		*/
		PLPHYSICS_API void RecreatePhysicsBody();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PLPHYSICS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SNMPhysicsBody functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates the PL physics body
		*/
		PLPHYSICS_API virtual void CreatePhysicsBody();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		float			  m_fMass;				/**< Mass of the physics body, 0=static body */
		PLMath::Vector3	  m_vCenterOfMass;		/**< Relative center of mass */
		PLMath::Vector3	  m_vPositionOffset;	/**< Position offset relative to the scene node (=origin) */
		PLCore::uint8	  m_nCollisionGroup;	/**< The collision group the body is in (0-31) */
		SCPhysicsWorld   *m_pWorldContainer;	/**< The PL physics world scene node container the physics body is in, can be a null pointer */
		ElementHandler   *m_pBodyHandler;		/**< Holds the PL physics body (ALWAYS valid!) */
		bool			  m_bListening;			/**< Listening scene node position changes etc.? */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node active state changed
		*/
		void OnActive();

		/**
		*  @brief
		*    Called when the scene node position changed
		*/
		void OnPosition();

		/**
		*  @brief
		*    Called when the scene node rotation changed
		*/
		void OnRotation();

		/**
		*  @brief
		*    Called when the transform was changed by the physics
		*/
		void OnTransform();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PLPHYSICS_API virtual void InformedOnInit();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODEMODIFIERS_BODY_H__
