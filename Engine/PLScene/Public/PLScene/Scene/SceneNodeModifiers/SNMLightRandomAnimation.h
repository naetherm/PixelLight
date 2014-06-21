/*********************************************************\
 *  File: SNMLightRandomAnimation.h                      *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_LIGHTRANDOMANIMATION_H__
#define __PLSCENE_SCENENODEMODIFIER_LIGHTRANDOMANIMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
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
*    Scene node modifier class for a random light color animation
*
*  @remarks
*    Animates the color of the light scene node over time.
*/
class SNMLightRandomAnimation : public SceneNodeModifier {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Flags (SceneNodeModifier flags extension)
		*/
		enum EFlags {
			Multiply = 1<<2,	/**< Multiply the color to the fix color, if not set, the color is added to the fix color */
			NR       = 1<<3,	/**< Do NOT manipulate the red color component */
			NG       = 1<<4,	/**< Do NOT manipulate the green color component */
			NB       = 1<<5		/**< Do NOT manipulate the blue color component */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNodeModifier::EFlags)
			pl_enum_value(Multiply,	"Multiply the color to the fix color, if not set, the color is added to the fix color")
			pl_enum_value(NR,		"Do NOT manipulate the red color component")
			pl_enum_value(NG,		"Do NOT manipulate the green color component")
			pl_enum_value(NB,		"Do NOT manipulate the blue color component")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMLightRandomAnimation, "PLScene", PLScene::SceneNodeModifier, "Scene node modifier class for a random light color animation")
		// Properties
		pl_properties
			pl_property("SceneNodeClass",	"PLScene::SNLight")
		pl_properties_end
		// Attributes
		pl_attribute(Speed,		float,					2.0f,									ReadWrite,	DirectValue,	"Animation speed",	"")
		pl_attribute(Radius,	float,					0.5f,									ReadWrite,	DirectValue,	"Animation radius",	"")
		pl_attribute(FixColor,	PLGraphics::Color3,		PLGraphics::Color3(0.5f, 0.5f, 0.5f),	ReadWrite,	DirectValue,	"Fix color",		"")
		pl_attribute(Color,		PLGraphics::Color3,		PLGraphics::Color3::White,				ReadWrite,	DirectValue,	"Color to apply",	"")
			// Overwritten SceneNodeModifier attributes
		pl_attribute(Flags,		pl_flag_type(EFlags),	0,										ReadWrite,	GetSet,			"Flags",			"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnUpdate,	"Called when the scene node modifier needs to be updated",	"")
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
		PLS_API SNMLightRandomAnimation(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMLightRandomAnimation();


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
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fTimer;	/**< Animation timer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_LIGHTRANDOMANIMATION_H__
