/*********************************************************\
 *  File: SNBitmap2D.h                                   *
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


#ifndef __PLSCENE_SCENENODE_BITMAP2D_H__
#define __PLSCENE_SCENENODE_BITMAP2D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneNodes/SNBitmap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    2D bitmap (overlay) scene node
*
*  @remarks
*    This scene node type can for instance be used to draw logos
*    or to composite simple 'Head-Up Displays' (HUD).
*
*  @note
*    - It's NOT recommended to use this scene node to construct complex GUIs!
*/
class SNBitmap2D : public SNBitmap {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			Center      = 1<<11,	/**< Center the bitmap */
			Pos3D       = 1<<12,	/**< The bitmap is placed within 3D. If this flag is not set, the node position is in 'screen space' between 0.0 and 1.0. */
			Background  = 1<<13		/**< Draw the bitmap at the background ('pre-draw') */
		};
		pl_enum(EFlags)
			pl_enum_base(SNBitmap::EFlags)
			pl_enum_value(Center,		"Center the bitmap")
			pl_enum_value(Pos3D,		"The bitmap is placed within 3D. If this flag is not set, the node position is in 'screen space' between 0.0 and 1.0.")
			pl_enum_value(Background,	"Draw the bitmap at the background ('pre-draw')")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNBitmap2D, "PLScene", PLScene::SNBitmap, "2D bitmap (overlay) scene node")
		// Attributes
		pl_attribute(Width,		float,					1.0f,		ReadWrite,	DirectValue,	"Bitmap width 'screen space' between 0.0 and 1.0",	"")
		pl_attribute(Height,	float,					1.0f,		ReadWrite,	DirectValue,	"Bitmap height 'screen space' between 0.0 and 1.0",	"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,		pl_flag_type(EFlags),	NoCulling,	ReadWrite,	GetSet,			"Flags",											"")
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
		PLS_API SNBitmap2D();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNBitmap2D();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the bitmap
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*/
		void DrawBitmap(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode);


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawPre(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode) override;
		PLS_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_BITMAP2D_H__
