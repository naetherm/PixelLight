/*********************************************************\
 *  File: SNProjectivePointLight.h                       *
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


#ifndef __PLSCENE_PROJECTIVEPOINTLIGHT_H__
#define __PLSCENE_PROJECTIVEPOINTLIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLScene/Scene/SNPointLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Omni directional projective point light scene node
*/
class SNProjectivePointLight : public SNPointLight {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoProjection = 1<<13	/**< Disable texture projection */
		};
		pl_flag(EFlags)
			pl_enum_base(SNPointLight::EFlags)
			pl_enum_value(NoProjection, "Disable texture projection")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLS_API)
		// Attributes
		pl_attribute_getset(SNProjectivePointLight,	ProjectedMaterial,	PLCore::String,	"Data/Textures/DefaultCubeMap.dds",	ReadWrite)
			// Overwritten SceneNode attributes
		pl_attribute_getset(SNProjectivePointLight,	Flags,				PLCore::uint32,	0,									ReadWrite)
	pl_class_def_end


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
		PLS_API SNProjectivePointLight();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNProjectivePointLight();

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
		PLS_API virtual bool IsProjectivePointLight() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_PROJECTIVEPOINTLIGHT_H__
