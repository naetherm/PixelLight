/*********************************************************\
 *  File: SRPDeferred.cpp                                *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLMath/Vector4.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferred.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDeferred)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the first found instance of a SRPDeferredGBuffer scene renderer pass instance within the scene renderer pipeline
*/
SRPDeferredGBuffer *SRPDeferred::GetGBuffer() const
{
	static const String sClassName = "PLCompositing::SRPDeferredGBuffer";
	return reinterpret_cast<SRPDeferredGBuffer*>(GetFirstInstanceOfSceneRendererPassClass(sClassName));
}

/**
*  @brief
*    Returns the vertex buffer of this deferred scene renderer pass
*/
VertexBuffer *SRPDeferred::GetVertexBuffer()
{
	// Initialize vertex buffer
	if (!m_pVertexBuffer) {
		// Get the renderer instance
		Renderer *pRenderer = GetRenderer();
		if (pRenderer) {
			// Create the vertex buffer
			m_pVertexBuffer = pRenderer->CreateVertexBuffer();

			// Add vertex position attribute to the vertex buffer
			m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float4);

			// Add texture coordinate attribute to the vertex buffer
			m_pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);

			// Allocate
			m_pVertexBuffer->Allocate(4);

			// Fill
			if (m_pVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0 - lower/left corner
				// Position
				float *pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::Position));
				pfVertex[Vector4::X] = -1.0f;
				pfVertex[Vector4::Y] = -1.0f;
				pfVertex[Vector4::Z] =  0.0f;
				pfVertex[Vector4::W] =  1.0f;
				// Texture coordinate 0
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::TexCoord, 0));
				pfVertex[Vector2::X] = 0.0f;
				pfVertex[Vector2::Y] = 0.0f;

			// Vertex 1 - lower/right corner
				// Position
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(1, VertexBuffer::Position));
				pfVertex[Vector4::X] =  1.0f;
				pfVertex[Vector4::Y] = -1.0f;
				pfVertex[Vector4::Z] =  0.0f;
				pfVertex[Vector4::W] =  1.0f;
				// Texture coordinate 0
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(1, VertexBuffer::TexCoord, 0));
				pfVertex[Vector2::X] = 1.0f;
				pfVertex[Vector2::Y] = 0.0f;

			// Vertex 2 - upper/left corner
				// Position
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(2, VertexBuffer::Position));
				pfVertex[Vector4::X] = -1.0f;
				pfVertex[Vector4::Y] =  1.0f;
				pfVertex[Vector4::Z] =  0.0f;
				pfVertex[Vector4::W] =  1.0f;
				// Texture coordinate 0
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(2, VertexBuffer::TexCoord, 0));
				pfVertex[Vector2::X] = 0.0f;
				pfVertex[Vector2::Y] = 1.0f;

			// Vertex 3 - upper/right corner
				// Position
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(3, VertexBuffer::Position));
				pfVertex[Vector4::X] = 1.0f;
				pfVertex[Vector4::Y] = 1.0f;
				pfVertex[Vector4::Z] = 0.0f;
				pfVertex[Vector4::W] = 1.0f;
				// Texture coordinate 0
				pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(3, VertexBuffer::TexCoord, 0));
				pfVertex[Vector2::X] = 1.0f;
				pfVertex[Vector2::Y] = 1.0f;

				// Unlock the vertex buffer
				m_pVertexBuffer->Unlock();
			}
		}
	}

	// Return the vertex buffer
	return m_pVertexBuffer;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SRPDeferred::SRPDeferred() :
	m_pVertexBuffer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferred::~SRPDeferred()
{
	// Destroy the vertex buffer
	if (m_pVertexBuffer)
		delete m_pVertexBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
