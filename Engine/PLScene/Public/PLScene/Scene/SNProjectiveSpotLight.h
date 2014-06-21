/*********************************************************\
 *  File: SNProjectiveSpotLight.h                        *
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


#ifndef __PLSCENE_PROJECTIVESPOTLIGHT_H__
#define __PLSCENE_PROJECTIVESPOTLIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLScene/Scene/SNSpotLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Projective spot light scene node
*
*  @note
*    - By default, the 'NoCone' flag is set meaning that the given outer and inner angles are
*      not used for lighting
*/
class SNProjectiveSpotLight : public SNSpotLight {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoProjection = 1<<14	/**< Disable texture projection */
		};
		pl_enum(EFlags)
			pl_enum_base(SNSpotLight::EFlags)
			pl_enum_value(NoProjection, "Disable texture projection")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNProjectiveSpotLight, "PLScene", PLScene::SNSpotLight, "Projective spot light scene node")
		// Attributes
		pl_attribute(ProjectedMaterial,	PLCore::String,			"Data/Textures/PLLogo.dds",	ReadWrite,	GetSet,	"Projected material",	"Type='Material Effect Image TextureAni'")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	NoCone,						ReadWrite,	GetSet,	"Flags",				"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetProjectedMaterial() const;
		PLS_API void SetProjectedMaterial(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNProjectiveSpotLight();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNProjectiveSpotLight();

		/**
		*  @brief
		*    Returns the handler of the projected material
		*
		*  @return
		*    Handler of the projected material
		*/
		PLS_API const PLRenderer::MaterialHandler &GetProjectedMaterialHandler();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String				m_sProjectedMaterial;	/**< Projected material */
		bool						m_bProjectedMaterial;	/**< Projected material loaded? */
		PLRenderer::MaterialHandler m_cProjectedMaterial;	/**< Projected material */


	//[-------------------------------------------------------]
	//[ Public virtual SNLight functions                      ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool IsProjectiveSpotLight() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_PROJECTIVESPOTLIGHT_H__
