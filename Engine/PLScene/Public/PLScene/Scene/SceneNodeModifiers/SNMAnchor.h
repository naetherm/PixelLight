/*********************************************************\
 *  File: SNMAnchor.h                                    *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_ANCHOR_H__
#define __PLSCENE_SCENENODEMODIFIER_ANCHOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Scene/SceneNodeModifier.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node anchor modifier class
*
*  @remarks
*    By using this modifier, you can link together scene nodes in multiple ways.
*/
class SNMAnchor : public SceneNodeModifier {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Flags (SceneNodeModifier flags extension)
		*/
		enum EFlags {
			NoPosition        = 1<<2,	/**< Do NOT manipulate the position */
			NoRotation        = 1<<3,	/**< Do NOT manipulate the rotation */
			NoContainerUpdate = 1<<4	/**< Do NOT change the container the attached node is in */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNodeModifier::EFlags)
			pl_enum_value(NoPosition,			"Do NOT manipulate the position")
			pl_enum_value(NoRotation,			"Do NOT manipulate the rotation")
			pl_enum_value(NoContainerUpdate,	"Do NOT change the container the attached node is in")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMAnchor, "PLScene", PLScene::SceneNodeModifier, "Scene node anchor modifier class")
		// Attributes
		pl_attribute(AttachedNode,			PLCore::String,			"",						ReadWrite,	GetSet,			"Name of the attached scene node",																				"")
		pl_attribute(PositionOffset,		PLMath::Vector3,		PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Position offset (node space)",																					"")
		pl_attribute(RotationOffset,		PLMath::Vector3,		PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Rotation offset in degree (node space)",																		"")
		pl_attribute(SkeletonJoint,			PLCore::String,			"",						ReadWrite,	DirectValue,	"If not empty, the attached node is relative to this skeleton joint (there must be a mesh handler + skeleton)",	"")
		pl_attribute(JointPositionOffset,	PLMath::Vector3,		PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Joint position offset (joint space)",																			"")
		pl_attribute(JointRotationOffset,	PLMath::Vector3,		PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Joint rotation offset in degree (joint space)",																"")
			// Overwritten SceneNodeModifier attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),	0,						ReadWrite,	GetSet,			"Flags",																										"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnContainer,				"Called when the scene node container changed",									"")
		pl_slot_0(OnPositionRotationUpdate,	"Called when the scene node position or rotation changed or on update request",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetAttachedNode() const;
		PLS_API void SetAttachedNode(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLS_API SNMAnchor(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMAnchor();


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNodeModifier functions         ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node container changed
		*/
		void OnContainer();

		/**
		*  @brief
		*    Called when the scene node position or rotation changed or on update request
		*/
		void OnPositionRotationUpdate();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sAttachedNode;
		SceneNodeHandler  m_cAttachedNodeHandler;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_ANCHOR_H__
