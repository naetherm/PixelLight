/*********************************************************\
 *  File: SRPLightEffects.h                              *
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


#ifndef __PLCOMPOSITING_LIGHTEFFECTS_H__
#define __PLCOMPOSITING_LIGHTEFFECTS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color4;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene renderer pass drawing light effects like lens flares
*/
class SRPLightEffects : public PLScene::SceneRendererPass {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoCorona    = 1<<1,	/**< Disable corona rendering */
			NoFlares    = 1<<2,	/**< Disable flares rendering */
			NoBlend     = 1<<3,	/**< Disable blend rendering */
			PrepareStep = 1<<4	/**< Is this a prepare step? */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneRendererPass::EFlags)
			pl_enum_value(NoCorona,		"Disable corona rendering")
			pl_enum_value(NoFlares,		"Disable flares rendering")
			pl_enum_value(NoBlend,		"Disable blend rendering")
			pl_enum_value(PrepareStep,	"Is this a prepare step?")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPLightEffects, "PLCompositing", PLScene::SceneRendererPass, "Abstract scene renderer pass drawing light effects like lens flares")
		// Attributes
		pl_attribute(CoronaMaterial,	PLCore::String,			"Data/Effects/DefaultCorona.plfx",	ReadWrite,	GetSet,	"Corona material",	"Type='Material Effect Image TextureAni'")
		pl_attribute(FlareMaterial,		PLCore::String,			"Data/Effects/DefaultFlare.plfx",	ReadWrite,	GetSet,	"Flare material",	"Type='Material Effect Image TextureAni'")
		pl_attribute(BlendMaterial,		PLCore::String,			"Data/Effects/DefaultBlend.plfx",	ReadWrite,	GetSet,	"Blend material",	"Type='Material Effect Image TextureAni'")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,									ReadWrite,	GetSet,	"Flags",			"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLCOM_API PLCore::String GetCoronaMaterial() const;
		PLCOM_API void SetCoronaMaterial(const PLCore::String &sValue);
		PLCOM_API PLCore::String GetFlareMaterial() const;
		PLCOM_API void SetFlareMaterial(const PLCore::String &sValue);
		PLCOM_API PLCore::String GetBlendMaterial() const;
		PLCOM_API void SetBlendMaterial(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the handler of the corona material
		*
		*  @return
		*    Handler of the corona material
		*/
		PLCOM_API const PLRenderer::MaterialHandler &GetCoronaMaterialHandler();

		/**
		*  @brief
		*    Returns the handler of the flare material
		*
		*  @return
		*    Handler of the flare material
		*/
		PLCOM_API const PLRenderer::MaterialHandler &GetFlareMaterialHandler();

		/**
		*  @brief
		*    Returns the handler of the blend material
		*
		*  @return
		*    Handler of the blend material
		*/
		PLCOM_API const PLRenderer::MaterialHandler &GetBlendMaterialHandler();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPLightEffects();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPLightEffects();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						m_bCoronaMaterial;	/**< Corona material loaded? */
		PLCore::String				m_sCoronaMaterial;	/**< Corona material */
		PLRenderer::MaterialHandler m_cCoronaMaterial;	/**< Corona material */
		bool						m_bFlareMaterial;	/**< Flare material loaded? */
		PLCore::String				m_sFlareMaterial;	/**< Flare material */
		PLRenderer::MaterialHandler m_cFlareMaterial;	/**< Flare material */
		bool						m_bBlendMaterial;	/**< Blend material loaded? */
		PLCore::String				m_sBlendMaterial;	/**< Blend material */
		PLRenderer::MaterialHandler m_cBlendMaterial;	/**< Blend material */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_LIGHTEFFECTS_H__
