/*********************************************************\
 *  File: SNMPhysicsJointCorkscrew.h                     *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_JOINTCORKSCREW_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_JOINTCORKSCREW_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsJoint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Physics corkscrew joint scene node modifier
*/
class SNMPhysicsJointCorkscrew : public SNMPhysicsJoint {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLPHYSICS_API)
		// Attributes
		pl_attribute_getset(SNMPhysicsJointCorkscrew,	PivotPoint,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_getset(SNMPhysicsJointCorkscrew,	PinDir,		PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API const PLMath::Vector3 &GetPivotPoint() const;
		PLPHYSICS_API void SetPivotPoint(const PLMath::Vector3 &vValue);
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir() const;
		PLPHYSICS_API void SetPinDir(const PLMath::Vector3 &vValue);


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
		PLPHYSICS_API SNMPhysicsJointCorkscrew(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsJointCorkscrew();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vPivotPoint;	/**< Origin of the corkscrew in local space */
		PLMath::Vector3 m_vPinDir;		/**< The line of action of the corkscrew in local space */


	//[-------------------------------------------------------]
	//[ Private virtual SNMPhysicsJoint functions             ]
	//[-------------------------------------------------------]
	private:
		virtual void CreatePhysicsJoint() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODEMODIFIERS_JOINTCORKSCREW_H__
