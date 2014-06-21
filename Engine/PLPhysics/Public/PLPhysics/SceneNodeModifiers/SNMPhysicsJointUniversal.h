/*********************************************************\
 *  File: SNMPhysicsJointUniversal.h                     *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_JOINTUNIVERSAL_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_JOINTUNIVERSAL_H__
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
*    Physics universal joint scene node modifier
*
*  @remarks
*    A universal joint is a constraint that restricts two rigid bodies to be connected to a point 
*    fixed on both bodies, while and allowing one body to spin around a fix axis in is own frame, 
*    and the other body to spin around another axis fixes on it own frame. Both axis must be 
*    mutually perpendicular.
*/
class SNMPhysicsJointUniversal : public SNMPhysicsJoint {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, SNMPhysicsJointUniversal, "PLPhysics", PLPhysics::SNMPhysicsJoint, "Physics universal joint scene node modifier")
		// Attributes
		pl_attribute(PivotPoint,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite,	GetSet,	"Origin of the universal in local space",																																				"")
		pl_attribute(PinDir1,		PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite,	GetSet,	"First axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in scene container space if the 'LocalPinDirection'-flag is not set, else in local scene node space",			"")
		pl_attribute(PinDir2,		PLMath::Vector3,	PLMath::Vector3::UnitZ,	ReadWrite,	GetSet,	"Second axis of rotation fixed on 'Target' body and perpendicular to 'PinDir1' in scene container space if the 'LocalPinDirection'-flag is not set, else in local scene node space",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API const PLMath::Vector3 &GetPivotPoint() const;
		PLPHYSICS_API void SetPivotPoint(const PLMath::Vector3 &vValue);
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir1() const;
		PLPHYSICS_API void SetPinDir1(const PLMath::Vector3 &vValue);
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir2() const;
		PLPHYSICS_API void SetPinDir2(const PLMath::Vector3 &vValue);


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
		PLPHYSICS_API SNMPhysicsJointUniversal(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsJointUniversal();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vPivotPoint;	/**< Origin of the universal in local space */
		PLMath::Vector3 m_vPinDir1;		/**< First axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in local space */
		PLMath::Vector3 m_vPinDir2;		/**< Second axis of rotation fixed on 'Target' body and perpendicular to 'PinDir1' in local space */


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


#endif // __PLPHYSICS_SCENENODEMODIFIERS_JOINTUNIVERSAL_H__
