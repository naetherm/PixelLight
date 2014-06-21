/*********************************************************\
 *  File: FullscreenQuad.cpp                             *
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
#include <PLMath/Vector4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLCompositing/FullscreenQuad.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FullscreenQuad::FullscreenQuad(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pVertexBuffer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FullscreenQuad::~FullscreenQuad()
{
	// Destroy the vertex buffer
	if (m_pVertexBuffer)
		delete m_pVertexBuffer;
}

/**
*  @brief
*    Returns the vertex buffer of this fullscreen quad
*/
VertexBuffer *FullscreenQuad::GetVertexBuffer()
{
	// Initialize vertex buffer
	if (!m_pVertexBuffer) {
		// Create the vertex buffer
		m_pVertexBuffer = m_pRenderer->CreateVertexBuffer();

		// Add vertex position attribute to the vertex buffer, zw stores the texture coordinate
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float4);

		// Allocate
		m_pVertexBuffer->Allocate(4);

		// Fill
		if (m_pVertexBuffer->Lock(Lock::WriteOnly)) {
		// Vertex 0 - lower/left corner
			// Position
			float *pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[Vector4::X] = -1.0f;
			pfVertex[Vector4::Y] = -1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] =  0.0f;
			pfVertex[Vector4::W] =  0.0f;

		// Vertex 1 - lower/right corner
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[Vector4::X] =  1.0f;
			pfVertex[Vector4::Y] = -1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] =  1.0f;
			pfVertex[Vector4::W] =  0.0f;

		// Vertex 2 - upper/left corner
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[Vector4::X] = -1.0f;
			pfVertex[Vector4::Y] =  1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] =  0.0f;
			pfVertex[Vector4::W] =  1.0f;

		// Vertex 3 - upper/right corner
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(3, VertexBuffer::Position));
			pfVertex[Vector4::X] = 1.0f;
			pfVertex[Vector4::Y] = 1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] = 1.0f;
			pfVertex[Vector4::W] = 1.0f;

			// Unlock the vertex buffer
			m_pVertexBuffer->Unlock();
		}
	}

	// Return the vertex buffer
	return m_pVertexBuffer;
}

/**
*  @brief
*    Draws the fullscreen quad
*/
void FullscreenQuad::Draw(bool bSetupRenderer)
{
	// Setup renderer?
	uint32 nFixedFillModeBackup = 0;
	if (bSetupRenderer) {
		nFixedFillModeBackup = m_pRenderer->GetRenderState(RenderState::FixedFillMode);
		m_pRenderer->SetRenderState(RenderState::FixedFillMode, Fill::Solid);
		m_pRenderer->SetRenderState(RenderState::CullMode,		Cull::None);
		m_pRenderer->SetRenderState(RenderState::ZEnable,		false);
		m_pRenderer->SetRenderState(RenderState::ZWriteEnable,	false);
	}

	// Draw the fullscreen quad
	m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);

	// Restore fixed fill mode render state?
	if (bSetupRenderer)
		m_pRenderer->SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
