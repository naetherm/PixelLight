/*********************************************************\
 *  File: SNLoadScreenPL.h                               *
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


#ifndef __PLENGINE_COMPOSITING_LOADSCREENPL_H__
#define __PLENGINE_COMPOSITING_LOADSCREENPL_H__
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
*    PixelLight load screen scene node
*/
class SNLoadScreenPL : public SNLoadScreenBase {


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


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNLoadScreenPL, "PLEngine", PLEngine::SNLoadScreenBase, "Load screen scene node")
		// Attributes
		pl_attribute(ClearFlags,	pl_flag_type(EClearFlags),	Color|Depth,								ReadWrite,	DirectValue,	"Clear flags",				"")
		pl_attribute(ClearColor,	PLGraphics::Color4,			PLGraphics::Color4(0.0f, 0.0f, 0.0f, 0.0f),	ReadWrite,	DirectValue,	"Clear color (r/g/b/a)",	"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),		NoCulling,									ReadWrite,	GetSet,			"Flags",					"")
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
		SNLoadScreenPL();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNLoadScreenPL();


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
		PLRenderer::MaterialHandler	m_cLogoMaterial;


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_LOADSCREENPL_H__
