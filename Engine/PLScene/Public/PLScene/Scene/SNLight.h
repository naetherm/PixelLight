/*********************************************************\
 *  File: SNLight.h                                      *
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


#ifndef __PLSCENE_LIGHT_H__
#define __PLSCENE_LIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract light scene node base class
*
*  @note
*    - Scene node scale is ignored
*/
class SNLight : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			Flares = 1<<10,	/**< Lens flares */
			Corona = 1<<11,	/**< Corona around the light */
			Blend  = 1<<12	/**< Brighten the screen (blend effect) */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(Flares,	"Lens flares")
			pl_enum_value(Corona,	"Corona around the light")
			pl_enum_value(Blend,	"Brighten the screen (blend effect)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNLight, "PLScene", PLScene::SceneNode, "Abstract light scene node base class")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconLight.dds")
		pl_properties_end
		// Attributes
		pl_attribute(Color,				PLGraphics::Color3,		PLGraphics::Color3::White,	ReadWrite,	DirectValue,	"Light color (r/g/b)",	"")
		pl_attribute(CoronaSize,		float,					0.2f,						ReadWrite,	DirectValue,	"Corona size",			"")
		pl_attribute(FlareSize,			float,					0.1f,						ReadWrite,	DirectValue,	"Lens flare size",		"")
		pl_attribute(ScreenBrighten,	float,					0.3f,						ReadWrite,	DirectValue,	"Screen brighten",		"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,							ReadWrite,	GetSet,			"Flags",				"")
		#ifdef PLSCENE_EXPORTS	// The following is only required when compiling PLScene
			// Methods
			pl_method_0(IsRenderLight,			pl_ret_type(bool),	"Returns whether this is a render light or not. Returns 'true' if this is a render light, else 'false'.",																		"")
			pl_method_0(IsEffectLight,			pl_ret_type(bool),	"Returns whether this scene node is a effect light (SNEffectLight) or not. Returns 'true' if this scene node is a effect light, else 'false'.",									"")
			pl_method_0(IsPointLight,			pl_ret_type(bool),	"Returns whether this scene node is a point light (SNPointLight) or not. Returns 'true' if this scene node is a point light, else 'false'.",									"")
			pl_method_0(IsProjectivePointLight,	pl_ret_type(bool),	"Returns whether this scene node is a projective point light (SNProjectivePointLight) or not. Returns 'true' if this scene node is a projective point light, else 'false'.",	"")
			pl_method_0(IsSpotLight,			pl_ret_type(bool),	"Returns whether this scene node is a spot light (SNSpotLight) or not. Returns 'true' if this scene node is a spot light, else 'false'.",										"")
			pl_method_0(IsProjectiveSpotLight,	pl_ret_type(bool),	"Returns whether this scene node is a projective spot light (SNProjectiveSpotLight) or not. Returns 'true' if this scene node is a projective spot light, else 'false'.",		"")
			pl_method_0(IsDirectionalLight,		pl_ret_type(bool),	"Returns whether this scene node is a directional light (SNDirectionalLight) or not. Returns 'true' if this scene node is a directional light, else 'false'.",					"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public virtual SNLight functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether this is a render light or not
		*
		*  @return
		*    'true' if this is a render light, else 'false'
		*/
		PLS_API virtual bool IsRenderLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a effect light (SNEffectLight) or not
		*
		*  @return
		*    'true' if this scene node is a effect light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNEffectLight")
		*/
		PLS_API virtual bool IsEffectLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a point light (SNPointLight) or not
		*
		*  @return
		*    'true' if this scene node is a point light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNPointLight")
		*/
		PLS_API virtual bool IsPointLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a projective point light (SNProjectivePointLight) or not
		*
		*  @return
		*    'true' if this scene node is a projective point light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNProjectivePointLight")
		*/
		PLS_API virtual bool IsProjectivePointLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a spot light (SNSpotLight) or not
		*
		*  @return
		*    'true' if this scene node is a spot light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNSpotLight")
		*/
		PLS_API virtual bool IsSpotLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a projective spot light (SNProjectiveSpotLight) or not
		*
		*  @return
		*    'true' if this scene node is a projective spot light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNProjectiveSpotLight")
		*/
		PLS_API virtual bool IsProjectiveSpotLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a directional light (SNDirectionalLight) or not
		*
		*  @return
		*    'true' if this scene node is a directional light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNDirectionalLight")
		*/
		PLS_API virtual bool IsDirectionalLight() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNLight();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNLight();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_LIGHT_H__
