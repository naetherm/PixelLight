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
		pl_flag(EFlags)
			pl_enum_base(SceneNodeModifier::EFlags)
			pl_enum_value(NoPosition,			"Do NOT manipulate the position")
			pl_enum_value(NoRotation,			"Do NOT manipulate the rotation")
			pl_enum_value(NoContainerUpdate,	"Do NOT change the container the attached node is in")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLS_API)
		// Attributes
		pl_attribute_getset		(SNMAnchor,	AttachedNode,			PLCore::String,		"",						ReadWrite)
		pl_attribute_directvalue(			PositionOffset,			PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_directvalue(			RotationOffset,			PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_directvalue(			SkeletonJoint,			PLCore::String,		"",						ReadWrite)
		pl_attribute_directvalue(			JointPositionOffset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_directvalue(			JointRotationOffset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
			// Overwritten SceneNodeModifier attributes
		pl_attribute_getset		(SNMAnchor,	Flags,					PLCore::uint32,		0,						ReadWrite)
		// Slots
		pl_slot_0_def(SNMAnchor,	OnContainer)
		pl_slot_0_def(SNMAnchor,	OnPositionRotationUpdate)
	pl_class_def_end


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
