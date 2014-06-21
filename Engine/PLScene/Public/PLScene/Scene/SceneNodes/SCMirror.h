/*********************************************************\
 *  File: SCMirror.h                                     *
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


#ifndef __PLSCENE_SCENENODE_MIRROR_H__
#define __PLSCENE_SCENENODE_MIRROR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Polygon.h>
#include "PLScene/Scene/SceneNodes/SCRenderToTexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Planar mirror scene node
*
*  @note
*    - The vertices of the mirror plane are on the x/y axis
*    - If the mirror is drawn, it will set the mirror texture automatically at the first
*      texture stage
*    - If the variable 'CameraName' is not set or the given camera is invalid, the mirror
*      will create a camera with the name 'Virtual camera' within it's container
*    - The camera defined through the variable 'CameraName' will be updated automatically by
*      the mirror
*    - If the mirror is rendered more than once per frame (multiple views, mirrored in another mirror etc.)
*      do NOT use a FPS limit - because the mirror only has ONE texture it will render in, this texture can't
*      used over multiple frames if the mirror is render more than once per frame!
*/
class SCMirror : public SCRenderToTexture {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SCMirror, "PLScene", PLScene::SCRenderToTexture, "Planar mirror scene node")
		// Attributes
		pl_attribute(Material,		PLCore::String,	"",										ReadWrite,	DirectValue,	"Material to use",																				"Type='Material Effect Image TextureAni'")
		pl_attribute(DynamicMap,	PLCore::String,	"DynamicMap",							ReadWrite,	DirectValue,	"Name of the material/effect parameter to assign the dynamic texture with",						"")
		pl_attribute(Vertices,		PLCore::String,	"1.0 -1.0 -1.0 -1.0 -1.0 1.0 1.0 1.0",	ReadWrite,	DirectValue,	"The vertices (x/y position) defining the mirror polygon. Each vertex has 3 components. (xyz)",	"")
		pl_attribute(NearScale,		float,			1.0f,									ReadWrite,	DirectValue,	"Virtual camera near plane scale",																"")
		pl_attribute(Far,			float,			1000.0f,								ReadWrite,	DirectValue,	"Virtual camera far plane",																		"")
			// Overwritten SCRenderToTexture attributes
		pl_attribute(FPSLimit,		float,			0.0f,									ReadWrite,	ModifyAttr,		"Frames per second limitation, if 0, there's no limitation",									"Min='0.0'")
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
		PLS_API SCMirror();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SCMirror();

		/**
		*  @brief
		*    Returns the mirror polygon
		*
		*  @return
		*    The mirror polygon
		*
		*  @note
		*    - After manipulating the vertices of the polygon you should call the polygon
		*      function ComputePlane() to calculate the plane the mirror polygon is on.
		*      Further call UpdateBoundingBox() to calculate the new mirror bounding box.
		*/
		PLS_API PLMath::Polygon &GetPolygon();

		/**
		*  @brief
		*    Updates the surrounding bounding box of the mirror
		*/
		PLS_API void UpdateBoundingBox();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Updates the virtual camera
		*/
		void UpdateVirtualCamera();

		/**
		*  @brief
		*    Sets the dynamic mirror texture
		*/
		void SetDynamicTexture();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float				 m_fWidth;			/**< Mirror width */
		float				 m_fHeight;			/**< Mirror height */
		PLMath::Polygon		 m_cPolygon;		/**< Mirror polygon */
		PLMesh::MeshHandler *m_pMeshHandler;	/**< Mirror mesh handler, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual PLMesh::MeshHandler *GetMeshHandler() override;
		PLS_API virtual void DrawPre(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;
		PLS_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;
		PLS_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Private virtual SceneNode functions                   ]
	//[-------------------------------------------------------]
	private:
		virtual void InitFunction() override;
		virtual void UpdateAABoundingBox() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_MIRROR_H__
