/*********************************************************\
 *  File: SNBitmap.h                                     *
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


#ifndef __PLSCENE_SCENENODE_BITMAP_H__
#define __PLSCENE_SCENENODE_BITMAP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color4.h>
#include <PLRenderer/Material/MaterialHandler.h>
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
*    Abstract bitmap scene node
*
*  @remarks
*    If 'Material' is not a material or an effect, the effect of the default value of
*    'Material' is used together with the given texture.
*/
class SNBitmap : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			IgnoreColor = 1<<10	/**< Ignore the color variable of the bitmap */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(IgnoreColor, "Ignore the color variable of the bitmap")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNBitmap, "PLScene", PLScene::SceneNode, "Abstract bitmap scene node")
		// Attributes
		pl_attribute(Material,		PLCore::String,			"Data/Effects/DefaultBitmap.plfx",	ReadWrite,	GetSet,	"Material to use",											"Type='Material Effect Image TextureAni'")
		pl_attribute(Color,			PLGraphics::Color4,		PLGraphics::Color4::White,			ReadWrite,	GetSet,	"Bitmap color",												"")
		pl_attribute(TexelStart,	PLMath::Vector2,		PLMath::Vector2::Zero,				ReadWrite,	GetSet,	"Bitmap texture source rectangle (0..1) start coordinate",	"")
		pl_attribute(TexelEnd,		PLMath::Vector2,		PLMath::Vector2::One,				ReadWrite,	GetSet,	"Bitmap texture source rectangle (0..1) end coordinate",	"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),	0,									ReadWrite,	GetSet,	"Flags",													"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetMaterial() const;
		PLS_API void SetMaterial(const PLCore::String &sValue);
		PLS_API virtual const PLGraphics::Color4 &GetColor() const;
		PLS_API virtual void SetColor(const PLGraphics::Color4 &cValue);
		PLS_API virtual const PLMath::Vector2 &GetTexelStart() const;
		PLS_API virtual void SetTexelStart(const PLMath::Vector2 &vValue);
		PLS_API virtual const PLMath::Vector2 &GetTexelEnd() const;
		PLS_API virtual void SetTexelEnd(const PLMath::Vector2 &vValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the handler of the material
		*
		*  @return
		*    Handler of the material
		*/
		PLS_API const PLRenderer::MaterialHandler &GetMaterialHandler();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNBitmap();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNBitmap();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String				m_sMaterial;	/**< Material */
		bool						m_bMaterial;	/**< Material loaded? */
		PLRenderer::MaterialHandler m_cMaterial;	/**< Material */
		PLGraphics::Color4			m_cColor;		/**< Bitmap color */
		PLMath::Vector2				m_vTexelStart;	/**< Bitmap texture source rectangle (0..1) start coordinate */
		PLMath::Vector2				m_vTexelEnd;	/**< Bitmap texture source rectangle (0..1) end coordinate */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_BITMAP_H__
