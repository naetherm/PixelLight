/*********************************************************\
 *  File: SNMPhysicsUpdateVelocity.h                     *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_UPDATEVELOCITY_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_UPDATEVELOCITY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Vector3.h>
#include <PLMath/Quaternion.h>
#include "PLPhysics/SceneNodeModifiers/SNMPhysics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the linear and angular velocity of physics bodies according to the scene node movement
*
*  @remarks
*    If there's a static (immobile) physics body which should for example move along a well defined path, this
*    static physics body will not interact properly with dynamic physics bodies because it's assumed to stay on one
*    and the same place. But when you have a moving platform or an escalator, this physics body should still be
*    static and therefore not influenced by other physics bodies, but other dynamic physics object should interact
*    correctly with the static moving physics body. (for example move with the platform) The static physics body is
*    in such a case called a kinematic physics body and can be realized within the scene graph by adding this scene
*    node modifier to your scene node which should be a kinematic physics body.
*
*  @note
*    - Keeps track of position changes and updates the linear velocity of the static physics bodies
*    - Keeps track of rotation changes and updates the angular velocity of the static physics bodies
*    - Scene container ownership changes are not tracked
*/
class SNMPhysicsUpdateVelocity : public SNMPhysics {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, SNMPhysicsUpdateVelocity, "PLPhysics", PLPhysics::SNMPhysics, "Updates the linear and angular velocity of physics bodies according to the scene node movement")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


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
		PLPHYSICS_API SNMPhysicsUpdateVelocity(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsUpdateVelocity();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PLPHYSICS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerPosition;
		PLCore::EventHandler<> EventHandlerRotation;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			   m_bPreviousPosition;	/**< Is there a previous scene node position? */
		PLMath::Vector3	   m_vPreviousPosition;	/**< Previous scene node position */
		bool			   m_bPreviousRotation;	/**< Is there a previous scene node rotation? */
		PLMath::Quaternion m_qPreviousRotation;	/**< Previous scene node rotation */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODEMODIFIERS_UPDATEVELOCITY_H__
