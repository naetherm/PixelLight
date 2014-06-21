/*********************************************************\
 *  File: SRPBackgroundBitmap.h                          *
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


#ifndef __PLCOMPOSITING_BACKGROUNDBITMAP_H__
#define __PLCOMPOSITING_BACKGROUNDBITMAP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color4.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include "PLCompositing/SRPBackground.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Bitmap background scene renderer pass
*/
class SRPBackgroundBitmap : public SRPBackground {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			IgnoreColor = 1<<1	/**< Ignore the color variable of the bitmap */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPBackground::EFlags)
			pl_enum_value(IgnoreColor, "Ignore the color variable of the bitmap")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPBackgroundBitmap, "PLCompositing", PLCompositing::SRPBackground, "Abstract bitmap background scene renderer pass")
		// Attributes
		pl_attribute(Material,		PLCore::String,			"",							ReadWrite,	GetSet,			"Material to use",											"Type='Material Effect Image TextureAni'")
		pl_attribute(Color,			PLGraphics::Color4,		PLGraphics::Color4::White,	ReadWrite,	DirectValue,	"Bitmap color",												"")
		pl_attribute(Position,		PLMath::Vector2,		PLMath::Vector2::Zero,		ReadWrite,	DirectValue,	"Bitmap position (0..1), (0, 0)=left top",					"")
		pl_attribute(Size,			PLMath::Vector2,		PLMath::Vector2::One,		ReadWrite,	DirectValue,	"Bitmap size (0..1)",										"")
		pl_attribute(TexelStart,	PLMath::Vector2,		PLMath::Vector2::Zero,		ReadWrite,	DirectValue,	"Bitmap texture source rectangle (0..1) start coordinate",	"")
		pl_attribute(TexelEnd,		PLMath::Vector2,		PLMath::Vector2::One,		ReadWrite,	DirectValue,	"Bitmap texture source rectangle (0..1) end coordinate",	"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),	0,							ReadWrite,	GetSet,			"Flags",													"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLCOM_API PLCore::String GetMaterial() const;
		PLCOM_API void SetMaterial(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPBackgroundBitmap();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPBackgroundBitmap();

		/**
		*  @brief
		*    Returns the handler of the material
		*
		*  @return
		*    Handler of the material
		*/
		PLCOM_API const PLRenderer::MaterialHandler &GetMaterialHandler();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Loads the material
		*/
		PLCOM_API void LoadMaterial();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String				m_sMaterial;	/**< Material name */
		PLRenderer::MaterialHandler m_cMaterial;	/**< Material */
		bool						m_bLoaded;		/**< Material already loaded? */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_BACKGROUNDBITMAP_H__
