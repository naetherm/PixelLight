/*********************************************************\
 *  File: SNMPhysicsBodyTerrain.h                        *
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


#ifndef __PLNATURE_SCENENODEMODIFIERS_BODYTERRAIN_H__
#define __PLNATURE_SCENENODEMODIFIERS_BODYTERRAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h>
#include "PLNature/PLNature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLNature {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Physics terrain body scene node modifier
*/
class SNMPhysicsBodyTerrain : public PLPhysics::SNMPhysicsBody {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLNATURE_RTTI_EXPORT, SNMPhysicsBodyTerrain, "PLNature", PLPhysics::SNMPhysicsBody, "Physics terrain body scene node modifier")
		// Properties
		pl_properties
			pl_property("SceneNodeClass",	"PLNature::SNTerrain")
		pl_properties_end
		// Attributes
		pl_attribute(Scale,	PLMath::Vector3,	PLMath::Vector3::One,	ReadWrite,	GetSet,	"Height field scale",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLNATURE_API const PLMath::Vector3 &GetScale() const;
		PLNATURE_API void SetScale(const PLMath::Vector3 &vValue);


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
		PLNATURE_API SNMPhysicsBodyTerrain(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLNATURE_API virtual ~SNMPhysicsBodyTerrain();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vScale;	/**< Height field scale */


	//[-------------------------------------------------------]
	//[ Private virtual PLPhysics::SNMPhysicsBody functions   ]
	//[-------------------------------------------------------]
	private:
		virtual void CreatePhysicsBody() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLNATURE_SCENENODEMODIFIERS_BODYTERRAIN_H__
