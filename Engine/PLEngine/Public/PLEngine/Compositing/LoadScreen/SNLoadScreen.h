/*********************************************************\
 *  File: SNLoadScreen.h                                 *
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


#ifndef __PLENGINE_COMPOSITING_LOADSCREEN_H__
#define __PLENGINE_COMPOSITING_LOADSCREEN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Material/MaterialHandler.h>
#include "PLEngine/Compositing/LoadScreen/SNLoadScreenBase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Load screen scene node
*/
class SNLoadScreen : public SNLoadScreenBase {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Clear flags
		*/
		enum EClearFlags {
			Color   = 1<<0,	/**< Clear color buffer */
			Depth   = 1<<1,	/**< Clear depth buffer */
			Stencil = 1<<2	/**< Clear stencil buffer */
		};
		pl_enum(EClearFlags)
			pl_enum_value(Color,	"Clear color buffer")
			pl_enum_value(Depth,	"Clear depth buffer")
			pl_enum_value(Stencil,	"Clear stencil buffer")
		pl_enum_end

		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoImageBlend = 1<<10	/**< Do not blend the images */
		};
		pl_enum(EFlags)
			pl_enum_base(SNLoadScreenBase::EFlags)
			pl_enum_value(NoImageBlend,	"Do not blend the images")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNLoadScreen, "PLEngine", PLEngine::SNLoadScreenBase, "Load screen scene node")
		// Attributes
		pl_attribute(ClearFlags,	pl_flag_type(EClearFlags),	Color|Depth,									ReadWrite,	DirectValue,	"Clear flags",											"")
		pl_attribute(ClearColor,	PLGraphics::Color4,			PLGraphics::Color4(0.0f, 0.0f, 0.0f, 0.0f),		ReadWrite,	DirectValue,	"Clear color (r/g/b/a)",								"")
		pl_attribute(Images,		PLCore::uint8,				4,												ReadWrite,	DirectValue,	"Number of images (1-4)",								"Min='1' Max='4'")

		// [TODO] New RTTI usage
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sScreenMaterial[0],	"ScreenMaterial[0]",	"Data/Materials/LoadScreen/0.mat",			"Screen 0 material filename",							"Type='Material Effect Image TextureAni'",	"")
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sScreenMaterial[1],	"ScreenMaterial[1]",	"Data/Materials/LoadScreen/1.mat",			"Screen 1 material filename",							"Type='Material Effect Image TextureAni'",	"")
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sScreenMaterial[2],	"ScreenMaterial[2]",	"Data/Materials/LoadScreen/2.mat",			"Screen 2 material filename",							"Type='Material Effect Image TextureAni'",	"")
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sScreenMaterial[3],	"ScreenMaterial[3]",	"Data/Materials/LoadScreen/3.mat",			"Screen 3 material filename",							"Type='Material Effect Image TextureAni'",	"")
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sBarBackMaterial,		"BarBackMaterial",		"Data/Materials/LoadScreen/BarBack.mat",	"Bar back material filename",							"Type='Material Effect Image TextureAni'",	"")
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sBarMaterial[0],		"BarMaterial[0]",		"Data/Materials/LoadScreen/Bar0.mat",		"Bar 0 material filename",								"Type='Material Effect Image TextureAni'",	"")
//		DEFINE_VAR(PL_VAR_FILENAME,	m_sBarMaterial[1],		"BarMaterial[1]",		"Data/Materials/LoadScreen/Bar1.mat",		"Bar 1 material filename",								"Type='Material Effect Image TextureAni'",	"")

		pl_attribute(ImageX,		float,						0.175f,											ReadWrite,	DirectValue,	"Image x position 'screen space' between 0.0 and 1.0",	"")
		pl_attribute(ImageY,		float,						0.133f,											ReadWrite,	DirectValue,	"Image y position 'screen space' between 0.0 and 1.0",	"")
		pl_attribute(ImageWidth,	float,						0.64f,											ReadWrite,	DirectValue,	"Image width 'screen space' between 0.0 and 1.0",		"")
		pl_attribute(ImageHeight,	float,						0.853f,											ReadWrite,	DirectValue,	"Image height 'screen space' between 0.0 and 1.0",		"")
		pl_attribute(ImageColor,	PLGraphics::Color4,			PLGraphics::Color4::White,						ReadWrite,	DirectValue,	"Image color",											"")
		pl_attribute(BarX,			float,						0.025f,											ReadWrite,	DirectValue,	"Bar x position 'screen space' between 0.0 and 1.0",	"")
		pl_attribute(BarY,			float,						0.083f,											ReadWrite,	DirectValue,	"Bar y position 'screen space' between 0.0 and 1.0",	"")
		pl_attribute(BarWidth,		float,						0.95f,											ReadWrite,	DirectValue,	"Bar width 'screen space' between 0.0 and 1.0",			"")
		pl_attribute(BarHeight,		float,						0.066f,											ReadWrite,	DirectValue,	"Bar height 'screen space' between 0.0 and 1.0",		"")
		pl_attribute(BarColor,		PLGraphics::Color4,			PLGraphics::Color4::White,						ReadWrite,	DirectValue,	"Bar color",											"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),		NoCulling,										ReadWrite,	GetSet,			"Flags",												"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SNLoadScreen();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNLoadScreen();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Free's the used materials
		*/
		void FreeMaterials();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String				m_sScreenMaterial[4];
		PLCore::String				m_sBarBackMaterial;
		PLCore::String				m_sBarMaterial[2];
		PLRenderer::MaterialHandler	m_cScreenMaterial[4];
		PLRenderer::MaterialHandler	m_cBarBackMaterial;
		PLRenderer::MaterialHandler	m_cBarMaterial[2];
		float						m_fAlpha[4];
		float						m_fBarBackAlpha;
		float						m_fBarAlpha[2];


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_LOADSCREEN_H__
