/*********************************************************\
 *  File: SNMPositionLinearAnimation.h                   *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_POSITIONLINEARANIMATION_H__
#define __PLSCENE_SCENENODEMODIFIER_POSITIONLINEARANIMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Vector3.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMTransform.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linear position animation scene node modifier class
*
*  @remarks
*    Animates the position of the scene nodes over time.
*/
class SNMPositionLinearAnimation : public SNMTransform {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Automatic vector type
		*/
		enum EAutoVector {
			None  = 0,	/**< No automatic movement vector */
			XAxis = 1,	/**< Use the current x (left) axis vector automatically as movement vector */
			YAxis = 2,	/**< Use the current y (up) axis vector automatically as movement vector */
			ZAxis = 3	/**< Use the current z (view) axis vector automatically as movement vector */
		};
		pl_enum(EAutoVector)
			pl_enum_value(None,		"No automatic movement vector")
			pl_enum_value(XAxis,	"Use the current x (left) axis vector automatically as movement vector")
			pl_enum_value(YAxis,	"Use the current y (up) axis vector automatically as movement vector")
			pl_enum_value(ZAxis,	"Use the current z (view) axis vector automatically as movement vector")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMPositionLinearAnimation, "PLScene", PLScene::SNMTransform, "Linear position animation scene node modifier class")
		// Attributes
		pl_attribute(AutoVector,	pl_enum_type(EAutoVector),	None,					ReadWrite,	DirectValue,	"Automatic vector type",	"")
		pl_attribute(Vector,		PLMath::Vector3,			PLMath::Vector3::One,	ReadWrite,	DirectValue,	"Movement vector",			"")
		pl_attribute(Speed,			float,						1.0f,					ReadWrite,	DirectValue,	"Movement speed",			"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	SceneNode&,	"Parameter constructor",	"")
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
		PLS_API SNMPositionLinearAnimation(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMPositionLinearAnimation();


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
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_POSITIONLINEARANIMATION_H__
