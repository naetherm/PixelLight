/*********************************************************\
 *  File: SNMPhysicsCharacterController.h                *
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


#ifndef __PLENGINE_CONTROLLER_SNMPHYSICSCHARACTERCONTROLLER_H__
#define __PLENGINE_CONTROLLER_SNMPHYSICSCHARACTERCONTROLLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLEngine/Controller/SNMPhysicsCharacter.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScene {
	class SNMMeshUpdate;
}
namespace PLEngine {
	class PhysicsCharacterController;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Basic PL physics character controller scene node modifier class
*
*  @remarks
*    When using the unchanged virtual standard controller:
*    Use a/w/d/s or cursor keys to walk, q/e to rotate and 'space' to jump. 'PageUp/PageDown'
*    to move upwards/downwards. Hold the 'shift'-key to speed up, hold the 'strg'-key to slow down.
*
*  @note
*    - Primary intended for rapid prototyping
*    - Adds automatically a "PLScene::SNMMeshUpdate" scene node modifier (if there's not yet one) in order to frequently update the mesh
*/
class SNMPhysicsCharacterController : public SNMPhysicsCharacter {


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags (PLScene::SceneNodeModifier flags extension)
		*/
		enum EFlags {
			NoJump  = 1<<3,	/**< Do not allow jumping */
			NoRun   = 1<<4,	/**< Do not allow running */
			NoCreep = 1<<5	/**< Do not allow creeping */
		};
		pl_flag(EFlags)
			pl_enum_base(SNMPhysicsCharacter::EFlags)
			pl_enum_value(NoJump,	"Do not allow jumping")
			pl_enum_value(NoRun,	"Do not allow running")
			pl_enum_value(NoCreep,	"Do not allow creeping")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PL_API)
		// Attributes
		pl_attribute_directvalue(								InputSemantic,		PLCore::String,	"",		ReadWrite)
		pl_attribute_directvalue(								YRotVelocity,		float,			0.0f,	ReadWrite)
		pl_attribute_directvalue(								IdleAnimation,		PLCore::String,	"",		ReadWrite)
		pl_attribute_directvalue(								IdleAnimationSpeed,	float,			24.0f,	ReadWrite)
		pl_attribute_directvalue(								WalkAnimation,		PLCore::String,	"",		ReadWrite)
		pl_attribute_directvalue(								WalkAnimationSpeed,	float,			24.0f,	ReadWrite)
		pl_attribute_directvalue(								RunAnimationSpeed,	float,			35.0f,	ReadWrite)
		pl_attribute_directvalue(								RotationNode,		PLCore::String,	"",		ReadWrite)
			// Overwritten PLScene::SceneNodeModifier attributes
		pl_attribute_getset		(SNMPhysicsCharacterController,	Flags,				PLCore::uint32,	0,		ReadWrite)
	pl_class_def_end


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
		PL_API SNMPhysicsCharacterController(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNMPhysicsCharacterController();


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNodeModifier functions   ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual PLInput::Controller *GetInputController() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void InformedOnInit() override;
		PL_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
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
		PLScene::SNMMeshUpdate *GetSNMMeshUpdate() const;


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						m_bJumping;		/**< Are we currently jumping? */
		PhysicsCharacterController *m_pController;	/**< Physics character input controller instance, always valid! */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONTROLLER_SNMPHYSICSCHARACTERCONTROLLER_H__
