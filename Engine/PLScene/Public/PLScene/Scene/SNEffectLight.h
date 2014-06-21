/*********************************************************\
 *  File: SNEffectLight.h                                *
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


#ifndef __PLSCENE_EFFECTLIGHT_H__
#define __PLSCENE_EFFECTLIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLScene/Scene/SNLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Effect light scene node
*/
class SNEffectLight : public SNLight {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNEffectLight, "PLScene", PLScene::SNLight, "Effect light scene node")
		// Attributes
		pl_attribute(CoronaMaterial,	PLCore::String,	"Data/Effects/DefaultCorona.plfx",	ReadWrite,	GetSet,	"Corona material",	"Type='Material Effect Image TextureAni'")
		pl_attribute(FlareMaterial,		PLCore::String,	"Data/Effects/DefaultFlare.plfx",	ReadWrite,	GetSet,	"Flare material",	"Type='Material Effect Image TextureAni'")
		pl_attribute(BlendMaterial,		PLCore::String,	"Data/Effects/DefaultBlend.plfx",	ReadWrite,	GetSet,	"Blend material",	"Type='Material Effect Image TextureAni'")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetCoronaMaterial() const;
		PLS_API void SetCoronaMaterial(const PLCore::String &sValue);
		PLS_API PLCore::String GetFlareMaterial() const;
		PLS_API void SetFlareMaterial(const PLCore::String &sValue);
		PLS_API PLCore::String GetBlendMaterial() const;
		PLS_API void SetBlendMaterial(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNEffectLight();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNEffectLight();

		/**
		*  @brief
		*    Returns the handler of the corona material
		*
		*  @return
		*    Handler of the corona material
		*/
		PLS_API const PLRenderer::MaterialHandler &GetCoronaMaterialHandler();

		/**
		*  @brief
		*    Returns the handler of the flare material
		*
		*  @return
		*    Handler of the flare material
		*/
		PLS_API const PLRenderer::MaterialHandler &GetFlareMaterialHandler();

		/**
		*  @brief
		*    Returns the handler of the blend material
		*
		*  @return
		*    Handler of the blend material
		*/
		PLS_API const PLRenderer::MaterialHandler &GetBlendMaterialHandler();


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


	//[-------------------------------------------------------]
	//[ Public virtual SNLight functions                      ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool IsEffectLight() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_EFFECTLIGHT_H__
