/*********************************************************\
 *  File: FullscreenQuad.h                               *
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


#ifndef __PLCOMPOSITING_FULLSCREENQUAD_H__
#define __PLCOMPOSITING_FULLSCREENQUAD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
	class VertexBuffer;
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
*    Fullscreen quad helper class
*
*  @remarks
*    The default content of the vertex buffer is a fullscreen quad consisting of 4 vertices:
*    Vertex 0 - lower/left corner
*      - Position:             (-1.0f, -1.0f)
*      - Texture coordinate 0: ( 0.0f,  0.0f)
*    Vertex 1 - lower/right corner
*      - Position:             ( 1.0f, -1.0f)
*      - Texture coordinate 0: ( 1.0f,  0.0f)
*    Vertex 2 - upper/left corner
*      - Position:             (-1.0f,  1.0f)
*      - Texture coordinate 0: ( 0.0f,  1.0f)
*    Vertex 3 - upper/right corner
*      - Position:             ( 1.0f,  1.0f)
*      - Texture coordinate 0: ( 1.0f,  1.0f)
*    To be as efficient as possible, only the xy-coordinate of the screen space position is stored.
*    The zw-component of the vertex position stores the texture coordinate. As a result, the vertex buffer
*    has only one "PLRenderer::VertexBuffer::Float4" vertex attribute with the semantic "PLRenderer::"VertexBuffer::Position".
*/
class FullscreenQuad {


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
		PLCOM_API FullscreenQuad(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API ~FullscreenQuad();

		/**
		*  @brief
		*    Returns the vertex buffer of this fullscreen quad
		*
		*  @return
		*    The vertex buffer of this fullscreen quad, a null pointer on error
		*/
		PLCOM_API PLRenderer::VertexBuffer *GetVertexBuffer();

		/**
		*  @brief
		*    Draws the fullscreen quad
		*
		*  @param[in] bSetupRenderer
		*    Setup renderer?
		*
		*  @remarks
		*    The 'bSetupRenderer'-parameter is just for convenience in order to set often used settings automatically.
		*    If 'bSetupRenderer' is false, this method just calls "m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);".
		*    If 'bSetupRenderer' is true, besides drawing the primitive, this method also makes the following renderer calls:
		*      m_pRenderer->SetRenderState(RenderState::FixedFillMode, Fill::Solid);
		*      m_pRenderer->SetRenderState(RenderState::CullMode,      Cull::None);
		*      m_pRenderer->SetRenderState(RenderState::ZEnable,       false);
		*      m_pRenderer->SetRenderState(RenderState::ZWriteEnable,  false);
		*      -> "RenderState::FixedFillMode" is reset to the previous state after drawing the primitive.
		*/
		PLCOM_API void Draw(bool bSetupRenderer = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer	  *m_pRenderer;		/**< Renderer to use, always valid! */
		PLRenderer::VertexBuffer  *m_pVertexBuffer;	/**< Vertex buffer, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_FULLSCREENQUAD_H__
