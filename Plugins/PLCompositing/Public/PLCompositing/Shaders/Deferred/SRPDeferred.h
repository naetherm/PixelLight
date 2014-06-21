/*********************************************************\
 *  File: SRPDeferred.h                                  *
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


#ifndef __PLCOMPOSITING_DEFERRED_H__
#define __PLCOMPOSITING_DEFERRED_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
}
namespace PLCompositing {
	class SRPDeferredGBuffer;
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
*    Abstract scene renderer pass for deferred rendering
*
*  @note
*    - There must be an SRPDeferredGBuffer instance within the render pipeline
*/
class SRPDeferred : public PLScene::SceneRendererPass {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferred, "PLCompositing", PLScene::SceneRendererPass, "Abstract scene renderer pass for deferred rendering")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the first found instance of a SRPDeferredGBuffer scene renderer pass instance within the scene renderer pipeline
		*
		*  @return
		*    The first found instance of a SRPDeferredGBuffer scene renderer pass instance within the scene renderer pipeline, can be a null pointer
		*/
		PLCOM_API SRPDeferredGBuffer *GetGBuffer() const;

		/**
		*  @brief
		*    Returns the vertex buffer of this deferred scene renderer pass
		*
		*  @return
		*    The vertex buffer of this deferred scene renderer pass, a null pointer on error
		*
		*  @remarks
		*    The default content of the vertex buffer is a fullscreen quad consisting of 4 vertices:
		*    Vertex 0 - lower/left corner
		*      - Position:             (-1.0f, -1.0f, 0.0f, 1.0f)
		*      - Texture coordinate 0: ( 0.0f,  0.0f)
		*    Vertex 1 - lower/right corner
		*      - Position:             ( 1.0f, -1.0f, 0.0f, 1.0f)
		*      - Texture coordinate 0: ( 1.0f,  0.0f)
		*    Vertex 2 - upper/left corner
		*      - Position:             (-1.0f,  1.0f, 0.0f, 1.0f)
		*      - Texture coordinate 0: ( 0.0f,  1.0f)
		*    Vertex 3 - upper/right corner
		*      - Position:             ( 1.0f,  1.0f, 0.0f, 1.0f)
		*      - Texture coordinate 0: ( 1.0f,  1.0f)
		*/
		PLCOM_API PLRenderer::VertexBuffer *GetVertexBuffer();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLCOM_API SRPDeferred();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferred();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::VertexBuffer *m_pVertexBuffer;	/**< Vertex buffer, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_H__
