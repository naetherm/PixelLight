/*********************************************************\
 *  File: SPRTTShaders.h                                 *
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


#ifndef __PLSAMPLE_54_SURFACEPAINTER_SHADERS_H__
#define __PLSAMPLE_54_SURFACEPAINTER_SHADERS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "SPRTT.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexShader;
	class FragmentShader;
	class ProgramWrapper;
	class TextureBuffer2D;
	class SurfaceTextureBuffer;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Shaders based render to texture surface painter
*/
class SPRTTShaders : public SPRTT {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		SPRTTShaders(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SPRTTShaders();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the scene
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		void DrawScene(PLRenderer::Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::SurfaceTextureBuffer *m_pRenderTarget;			/**< The render target the teapot is rendered in, can be a null pointer */
		PLRenderer::TextureBuffer2D		 *m_pColorTarget1;			/**< Color target 1, can be a null pointer */
		PLRenderer::TextureBuffer2D		 *m_pColorTarget2;			/**< Color target 2, can be a null pointer */
		PLRenderer::TextureBuffer2D		 *m_pColorTarget3;			/**< Color target 3, can be a null pointer */
		PLRenderer::VertexShader		 *m_pSceneVertexShader;		/**< Scene vertex shader, can be a null pointer */
		PLRenderer::FragmentShader		 *m_pSceneFragmentShader;	/**< Scene fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pSceneProgram;			/**< Scene GPU program, can be a null pointer */
		PLRenderer::VertexShader		 *m_pVertexShader;			/**< Vertex shader, can be a null pointer */
		PLRenderer::FragmentShader		 *m_pFragmentShader;		/**< Fragment shader, can be a null pointer */
		PLRenderer::ProgramWrapper		 *m_pProgram;				/**< GPU program, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::SurfacePainter functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void OnPaint(PLRenderer::Surface &cSurface) override;


};


#endif // __PLSAMPLE_54_SURFACEPAINTER_SHADERS_H__
