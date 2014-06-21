/*********************************************************\
 *  File: SurfaceTextureBuffer.cpp                       *
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
#include "PLRendererNull/TextureBuffer2D.h"
#include "PLRendererNull/TextureBufferRectangle.h"
#include "PLRendererNull/TextureBufferCube.h"
#include "PLRendererNull/SurfaceTextureBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SurfaceTextureBuffer::~SurfaceTextureBuffer()
{
	// De-initialize
	DeInit();

	// Delete texture buffer
	if (m_cTextureBufferHandler.GetResource())
		delete m_cTextureBufferHandler.GetResource();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SurfaceTextureBuffer::SurfaceTextureBuffer(PLRenderer::Renderer &cRenderer, PLRenderer::TextureBuffer &cTextureBuffer, uint32 nFlags, uint8 nMaxColorTargets) :
	PLRenderer::SurfaceTextureBuffer(cRenderer, nFlags, nMaxColorTargets),
	m_nFace(0)
{
	// Init data
	m_cTextureBufferHandler.SetResource(&cTextureBuffer);

	// Init
	Init();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::SurfaceTexture functions   ]
//[-------------------------------------------------------]
PLRenderer::TextureBuffer *SurfaceTextureBuffer::GetTextureBuffer() const
{
	return static_cast<PLRenderer::TextureBuffer*>(m_cTextureBufferHandler.GetResource());
}

uint8 SurfaceTextureBuffer::GetTextureBufferFace() const
{
	return m_nFace;
}

void SurfaceTextureBuffer::TakeDepthBufferFromSurfaceTextureBuffer(PLRenderer::SurfaceTextureBuffer &cSurfaceTextureBuffer)
{
	// Not implemented
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Surface functions          ]
//[-------------------------------------------------------]
Vector2i SurfaceTextureBuffer::GetSize() const
{
	// Return the size depending of the texture buffer type
	PLRenderer::TextureBuffer *pTextureBuffer = static_cast<PLRenderer::TextureBuffer*>(m_cTextureBufferHandler.GetResource());
	if (pTextureBuffer) {
		switch (pTextureBuffer->GetType()) {
			case PLRenderer::Resource::TypeTextureBuffer2D:
				return static_cast<PLRenderer::TextureBuffer2D*>(pTextureBuffer)->GetSize();

			case PLRenderer::Resource::TypeTextureBufferRectangle:
				return static_cast<PLRenderer::TextureBufferRectangle*>(pTextureBuffer)->GetSize();

			case PLRenderer::Resource::TypeTextureBufferCube:
			{
				 const uint32 nSize = static_cast<PLRenderer::TextureBufferCube*>(pTextureBuffer)->GetSize();
				 return Vector2i(nSize, nSize);
			}

			case PLRenderer::Resource::TypeIndexBuffer:
			case PLRenderer::Resource::TypeVertexBuffer:
			case PLRenderer::Resource::TypeUniformBuffer:
			case PLRenderer::Resource::TypeTextureBuffer1D:
			case PLRenderer::Resource::TypeTextureBuffer2DArray:
			case PLRenderer::Resource::TypeTextureBuffer3D:
			case PLRenderer::Resource::TypeOcclusionQuery:
			case PLRenderer::Resource::TypeVertexShader:
			case PLRenderer::Resource::TypeTessellationControlShader:
			case PLRenderer::Resource::TypeTessellationEvaluationShader:
			case PLRenderer::Resource::TypeGeometryShader:
			case PLRenderer::Resource::TypeFragmentShader:
			case PLRenderer::Resource::TypeProgram:
			case PLRenderer::Resource::TypeFont:
			default:
				break;	// Error, must be 2D, rectangle or cube!
		}
	}

	// Error!
	return Vector2i::Zero;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Surface functions         ]
//[-------------------------------------------------------]
bool SurfaceTextureBuffer::Init()
{
	// First de-initialize the old stuff
	DeInit();

	// Done
	return true;
}

void SurfaceTextureBuffer::DeInit()
{
}

bool SurfaceTextureBuffer::MakeCurrent(uint8 nFace)
{
	// Done
	return true;
}

bool SurfaceTextureBuffer::UnmakeCurrent()
{
	// Done
	return true;
}

bool SurfaceTextureBuffer::Present()
{
	// Nothing to do here...
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull
