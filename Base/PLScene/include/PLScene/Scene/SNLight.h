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
		pl_flag(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(Flares,	"Lens flares")
			pl_enum_value(Corona,	"Corona around the light")
			pl_enum_value(Blend,	"Brighten the screen (blend effect)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLS_API)
		// Attributes
		pl_attribute_directvalue(			Color,			PLGraphics::Color3,	PLGraphics::Color3::White,	ReadWrite)
		pl_attribute_directvalue(			CoronaSize,		float,				0.2f,						ReadWrite)
		pl_attribute_directvalue(			FlareSize,		float,				0.1f,						ReadWrite)
		pl_attribute_directvalue(			ScreenBrighten,	float,				0.3f,						ReadWrite)
			// Overwritten SceneNode attributes
		pl_attribute_getset		(SNLight,	Flags,			PLCore::uint32,		0,							ReadWrite)
	pl_class_def_end


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
