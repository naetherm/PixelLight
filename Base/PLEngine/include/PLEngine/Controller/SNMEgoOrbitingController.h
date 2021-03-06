/*********************************************************\
 *  File: SNMEgoOrbitingController.h                     *
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


#ifndef __PLENGINE_CONTROLLER_SNMEGOORBITINGCONTROLLER_H__
#define __PLENGINE_CONTROLLER_SNMEGOORBITINGCONTROLLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLEngine/Controller/SNMOrbitingController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node modifier class rotating a scene node towards a target scene node using an input controller with 3D restriction
*
*  @remarks
*    When using the unchanged virtual standard controller:
*    Hold the left mouse button to rotate, the right mouse button to zoom and the middle mouse button to pan.
*
*  @note
*    - Primary intended for rapid prototyping
*    - Restricted 3D rotation via Euler angles
*    - The up-vector is automatically calculated by using the set scene node rotation quaternion
*/
class SNMEgoOrbitingController : public SNMOrbitingController {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PL_API)
		// Attributes
		pl_attribute_directvalue(UpVector,		PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite)
		pl_attribute_directvalue(RollFactor,	float,				0.25f,					ReadWrite)
		// Slots
		pl_slot_0_def(SNMEgoOrbitingController,	OnRotation)
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
		PL_API SNMEgoOrbitingController(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNMEgoOrbitingController();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SNMEgoOrbitingController functions  ]
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


#endif // __PLENGINE_CONTROLLER_SNMEGOORBITINGCONTROLLER_H__
