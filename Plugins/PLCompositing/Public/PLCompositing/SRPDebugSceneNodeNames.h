/*********************************************************\
 *  File: SRPDebugSceneNodeNames.h                       *
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


#ifndef __PLCOMPOSITING_DEBUG_SCENENODENAMES_H__
#define __PLCOMPOSITING_DEBUG_SCENENODENAMES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include "PLCompositing/SRPDebug.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Font;
}
namespace PLScene {
	class VisNode;
	class SceneNode;
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
*    Scene renderer pass drawing just simple scene node names
*/
class SRPDebugSceneNodeNames : public SRPDebug {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			AbsoluteNames = 1<<1	/**< Absolute (long) names */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDebug::EFlags)
			pl_enum_value(AbsoluteNames, "Absolute (long) names")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDebugSceneNodeNames, "PLCompositing", PLCompositing::SRPDebug, "Scene renderer pass drawing just simple scene node names")
		// Attributes
		pl_attribute(FontName,			PLCore::String,			"",							ReadWrite,	DirectValue,	"The used font, if empty the default font is used",											"")
		pl_attribute(FontSize,			PLCore::uint32,			0,							ReadWrite,	DirectValue,	"The font size, if null the default font size is used - use 'FontScale' for dynamic scale",	"Max='100'")
		pl_attribute(FontScale,			float,					1.0f,						ReadWrite,	DirectValue,	"The dynamic font scale",																	"Min='0' Max='100'")
		pl_attribute(FontColor,			PLGraphics::Color4,		PLGraphics::Color4::White,	ReadWrite,	DirectValue,	"The dynamic font color",																	"")
		pl_attribute(MaxDrawDistance,	float,					20.0f,						ReadWrite,	DirectValue,	"Maximum draw distance of names to the camera, if less or equal 0, there's no limitation",	"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,							ReadWrite,	GetSet,			"Flags",																					"")
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
		PLCOM_API SRPDebugSceneNodeNames();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDebugSceneNodeNames();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws recursive
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		void DrawRec(PLRenderer::Font &cFont, const PLScene::SQCull &cCullQuery) const;

		/**
		*  @brief
		*    Draws a scene node name
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cVisNode
		*    Visibility node to use
		*  @param[in] cSceneNode
		*    Scene node to use
		*/
		void DrawName(PLRenderer::Font &cFont, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, const PLScene::SceneNode &cSceneNode) const;


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


#endif // __PLCOMPOSITING_DEBUG_SCENENODENAMES_H__
