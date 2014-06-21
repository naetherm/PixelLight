/*********************************************************\
 *  File: SNBitmap3D.h                                   *
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


#ifndef __PLSCENE_SCENENODE_BITMAP3D_H__
#define __PLSCENE_SCENENODE_BITMAP3D_H__
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
*    3D bitmap scene node
*
*  @note
*    - It's NOT recommended to use this scene node for example to create dense vegetation on a terrain!
*    - The axis aligned bounding box is centered around zero and each axis has a length of 1
*    - The bitmap is drawn on the x/y plane and 'looks' into the positive z axis
*/
class SNBitmap3D : public SNBitmap {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNBitmap3D, "PLScene", PLScene::SNBitmap, "3D bitmap scene node")
		// Attributes
		pl_attribute(PositionOffset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite,	GetSet,	"Position offset (scene node space)",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API const PLMath::Vector3 &GetPositionOffset() const;
		PLS_API void SetPositionOffset(const PLMath::Vector3 &vValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNBitmap3D();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNBitmap3D();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the bitmap shape
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] cVisNode
		*    Used visibility node
		*/
		void DrawShape(PLRenderer::Renderer &cRenderer, const VisNode &cVisNode);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vPositionOffset;	/**< Position offset (scene node space) */


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;
		PLS_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


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


#endif // __PLSCENE_SCENENODE_BITMAP3D_H__
