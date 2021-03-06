/*********************************************************\
 *  File: SNMPhysicsJointBall.h                          *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_JOINTBALL_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_JOINTBALL_H__
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
*    Physics ball and socket joint scene node modifier
*
*  @note
*    - All non-zero value for 'MaxConeAngle' are clamped between 5 degree and 175 degrees 
*    - Don't use 'NewtonBallSetConeLimits()' directly within your code, instead use the RTTI to set the variables
*      (... else this RTTI variables may have wrong values...)
*/
class SNMPhysicsJointBall : public SNMPhysicsJoint {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLPHYSICS_API)
		// Attributes
		pl_attribute_getset(SNMPhysicsJointBall,	PivotPoint,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_getset(SNMPhysicsJointBall,	PinDir,		PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite)
		// [TODO] Implement me
		// DEFINE_VAR(PL_VAR_FLOAT,	m_fMaxConeAngle,	"MaxConeAngle",		"0.0",			"Max angle in degree the attached body is allow to swing relative to the pin axis, a value of zero will disable this limits",	"",	"")
		// DEFINE_VAR(PL_VAR_FLOAT,	m_fMaxTwistAngle,	"MaxTwistAngle",	"0.0",			"Max angle in degree the attached body is allow to twist relative to the pin axis, a value of zero will disable this limits",	"",	"")
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
		PLPHYSICS_API SNMPhysicsJointBall(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsJointBall();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vPivotPoint;	/**< Origin of the ball and socket in local space */
		PLMath::Vector3 m_vPinDir;		/**< Vector defining the cone axis in local space */
		// [TODO] Implement me
		// float			m_fMaxConeAngle;
		// float			m_fMaxTwistAngle;


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


#endif // __PLPHYSICS_SCENENODEMODIFIERS_JOINTBALL_H__
