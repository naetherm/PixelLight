/*********************************************************\
 *  File: SNText3D.h                                     *
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


#ifndef __PLSCENE_SCENENODE_TEXT3D_H__
#define __PLSCENE_SCENENODE_TEXT3D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Material/MaterialHandler.h>
#include "PLScene/Scene/SceneNodes/SNText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    3D ("flat") text scene node
*
*  @note
*    - The text is drawn on the x/y plane and 'looks' into the positive z axis
*    - The 3D placed text has always a height of 1 to be independent of the actual 'font size'
*    - The 'font size' can be seen as the 'font quality'
*/
class SNText3D : public SNText {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoBackfaceCulling = 1<<11	/**< No backface culling so 'both' sides of the text can be seen */
		};
		pl_enum(EFlags)
			pl_enum_base(SNText::EFlags)
			pl_enum_value(NoBackfaceCulling, "No backface culling so 'both' sides of the text can be seen")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNText3D, "PLScene", PLScene::SNText, "3D (\"flat\") text scene node")
		// Attributes
		pl_attribute(BackgroundMaterial,	PLCore::String,			"",	ReadWrite,	GetSet,	"Background material to use",	"Type='Material Effect Image TextureAni'")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",						"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetBackgroundMaterial() const;
		PLS_API void SetBackgroundMaterial(const PLCore::String &sValue);
		PLS_API virtual void SetText(const PLCore::String &sValue) override;
		PLS_API virtual void SetFont(const PLCore::String &sValue) override;
		PLS_API virtual void SetFontSize(PLCore::uint32 nValue) override;
		PLS_API virtual void SetFlags(PLCore::uint32 nValue) override;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNText3D();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNText3D();

		/**
		*  @brief
		*    Returns the handler of the background material
		*
		*  @return
		*    Handler of the background material
		*/
		PLS_API const PLRenderer::MaterialHandler &GetBackgroundMaterialHandler();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String				m_sBackgroundMaterial;	/**< Background material */
		bool						m_bBackgroundMaterial;	/**< Background material loaded? */
		PLRenderer::MaterialHandler m_cBackgroundMaterial;	/**< Background material */


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode) override;


	//[-------------------------------------------------------]
	//[ Private virtual SceneNode functions                   ]
	//[-------------------------------------------------------]
	private:
		virtual void UpdateAABoundingBox() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_TEXT3D_H__
