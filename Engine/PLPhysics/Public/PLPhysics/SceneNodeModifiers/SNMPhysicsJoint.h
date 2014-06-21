/*********************************************************\
 *  File: SNMPhysicsJoint.h                              *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_JOINT_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_JOINT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/SceneNodeModifiers/SNMPhysics.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Matrix3x4;
}
namespace PLPhysics {
	class Joint;
	class ElementHandler;
	class SCPhysicsWorld;
	class SNMPhysicsBody;
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
*    Abstract physics joint scene node modifier
*
*  @note
*    - There MUST already be a 'PLPhysics::SNMPhysicsBody'-modifier within the owner scene node, else the joint
*      has no effect. Further the owner body can NOT have an infinity (zero = static, the default value) mass.
*    - The scene node 'Target' MUST already exist, else the joint can not be attached to it!
*    - By default joint disables collision with the linked bodies
*/
class SNMPhysicsJoint : public SNMPhysics {


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags
		*/
		enum EFlags {
			LocalPinDirection  = 1<<2	/**< If this flag is set, the pin direction(s) of the joint is/are defined in local scene node space instead of scene container space */
		};
		pl_enum(EFlags)
			pl_enum_base(SNMPhysics::EFlags)
			pl_enum_value(LocalPinDirection,	"If this flag is set, the pin direction(s) of the joint is/are defined in local scene node space instead of scene container space")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, SNMPhysicsJoint, "PLPhysics", PLPhysics::SNMPhysics, "Abstract physics joint scene node modifier")
		// Attributes
		pl_attribute(Target,	PLCore::String,			"",	ReadWrite,	GetSet,	"Name of the target scene node (which must have a 'PLPhysics::SNMPhysicsBody' modifier!), can left undefined",	"")
			// Overwritten PLScene::SceneNodeModifier attributes
		pl_attribute(Flags,		pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",																										"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API PLCore::String GetTarget() const;
		PLPHYSICS_API void SetTarget(const PLCore::String &sValue);
		PLPHYSICS_API virtual void SetFlags(PLCore::uint32 nValue) override;	// From PLScene::SceneNodeModifier


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the PL physics world scene node container the physics joint is in
		*
		*  @return
		*    The PL physics world scene node container the physics joint is in, a null pointer on error
		*    (if that's the case, something went totally wrong :)
		*
		*  @note
		*    - This PL physics world scene node container can be another as the scene node container this scene node is in
		*/
		PLPHYSICS_API SCPhysicsWorld *GetWorldContainer() const;

		/**
		*  @brief
		*    Returns the PL physics joint
		*
		*  @return
		*    The PL physics joint, a null pointer on error (if that's the case, something went totally wrong :)
		*/
		PLPHYSICS_API Joint *GetJoint() const;

		/**
		*  @brief
		*    Returns the owner PL physics body scene node modifier the joint is attached to
		*
		*  @return
		*    The owner PL physics body scene node modifier, a null pointer if there's no such body
		*
		*  @note
		*    - The implementation is searching for the first "PLPhysics::SNMPhysicsBody" within the owner
		*      scene node of this scene node modifier, this is the "owner" physics body
		*/
		PLPHYSICS_API SNMPhysicsBody *GetOwnerBodyModifier() const;

		/**
		*  @brief
		*    Returns the target PL physics body scene node modifier the joint is attached to
		*
		*  @return
		*    The target PL physics body scene node modifier, a null pointer if there's no such body
		*/
		PLPHYSICS_API SNMPhysicsBody *GetTargetBodyModifier() const;


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
		PLPHYSICS_API SNMPhysicsJoint(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsJoint();

		/**
		*  @brief
		*    Recreates the PL physics joint
		*/
		PLPHYSICS_API void RecreatePhysicsJoint();

		/**
		*  @brief
		*    Calculates the transform matrix that transform from the local owner scene node space into the physics world scene container
		*
		*  @param[out] mTransform
		*    Receives the result
		*/
		PLPHYSICS_API void CalculateJointTransformMatrix(PLMath::Matrix3x4 &mTransform) const;


	//[-------------------------------------------------------]
	//[ Protected virtual SNMPhysicsJoint functions           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates the PL physics joint
		*/
		virtual void CreatePhysicsJoint() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String	   m_sTarget;			/**< Name of the target scene node (which must have a 'PLPhysics::SNMPhysicsBody' modifier!), can left undefined */
		SCPhysicsWorld    *m_pWorldContainer;	/**< The PL physics world scene node container the physics joint is in, can be a null pointer */
		ElementHandler    *m_pJointHandler;		/**< Holds the PL physics joint (ALWAYS valid!) */


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PLPHYSICS_API virtual void InformedOnInit() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODEMODIFIERS_JOINT_H__
