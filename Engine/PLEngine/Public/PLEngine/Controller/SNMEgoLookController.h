/*********************************************************\
 *  File: SNMEgoLookController.h                         *
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


#ifndef __PLENGINE_CONTROLLER_SNMEGOLOOKCONTROLLER_H__
#define __PLENGINE_CONTROLLER_SNMEGOLOOKCONTROLLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLEngine/Controller/SNMLookController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node rotation input controller modifier class with 3D restriction
*
*  @note
*    - Primary intended for rapid prototyping
*    - Restricted 3D rotation via Euler angles
*    - The up-vector is automatically calculated by using the set scene node rotation quaternion
*/
class SNMEgoLookController : public SNMLookController {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNMEgoLookController, "PLEngine", PLEngine::SNMLookController, "Scene node rotation input controller modifier class with 3D restriction")
		// Attributes
		pl_attribute(UpVector,		PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite,	DirectValue,	"Up vector",																											"")
		pl_attribute(RollFactor,	float,				0.25f,					ReadWrite,	DirectValue,	"Roll factor. If there's an absolute z-axis (e.g. when using a 3D mouse), apply roll -> looks & feels more natural.",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnRotation,	"Called when the scene node rotation changed",	"")
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
		PL_API SNMEgoLookController(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNMEgoLookController();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SNMLookController functions         ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void OnUpdate() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node rotation changed
		*/
		void OnRotation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3	m_vUpVector;	/**< Up vector */
		float			m_fPitch;		/**< X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa) - in degree [-89.9, 89.9] */
		float			m_fYaw;			/**< Y rotation axis: Yaw (also called 'heading') change is turning to the left or right - in degree [0, 360] */
		bool			m_bListen;		/**< Listen to rotation signals? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONTROLLER_SNMEGOLOOKCONTROLLER_H__
