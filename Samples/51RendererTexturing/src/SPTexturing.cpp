/*********************************************************\
 *  File: SPTexturing.cpp                                *
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
#include <PLMath/Math.h>
#include <PLMath/Vector3i.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "SPTexturing.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPTexturing, "", PLRenderer::SurfacePainter, "Abstract textured surface painter")
pl_class_metadata_end(SPTexturing)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPTexturing::SPTexturing(Renderer &cRenderer) : SurfacePainter(cRenderer),
	m_pVertexBuffer(GetRenderer().CreateVertexBuffer()),
	m_pTextureBuffer(nullptr),
	m_fRotation(0.0f)
{
	// Setup the renderer vertex buffer for our quad
	if (m_pVertexBuffer) {
		// Tell the vertex buffer that it has to provide vertex position data. Each
		// vertex position has three floating point components. (x/y/z)
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);

		// There should also be a two component texture coordinate per vertex
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);

		// Now that we have added all desired vertex attributes, allocate the buffer. We want to have
		// four vertices (for each corner of the quad) and the usage of the buffer is considered
		// to be static - this means that the content of the vertex buffer isn't changed during runtime.
		// In fact this data CAN be changed at any time, but this isn't that performant and you should
		// avoid this whenever possible!
		m_pVertexBuffer->Allocate(4, Usage::Static);

		// Setup the vertex buffer data. We have to lock our buffer before we are able to manipulate it.
		if (m_pVertexBuffer->Lock(Lock::WriteOnly)) {
			// Here we request a pointer to the given attribute of a certain vertex to fill it
			// with our data. If possible, you should always use the provided GetData(<VertexID>, <SEMANTIC>, <CHANNEL>)
			// and SetColor() functions to ensure that you put the data at the correct position into the buffer. For
			// instance the color value can have a different size on different renderer implementations! For
			// security we check the pointer given by GetData() for a null pointer, but normally that's not required.
			// -> "m_pVertexBuffer->SetFloat(0, VertexBuffer::Position, 0.0f, 1.0f, 0.0f);" is using a comfort function.
			//    It's still possible to access the data in a more direct way to e.g. copy over data within a single
			//    memory copy operation. Here's an example how to access data in a more performant direct way:
			//        float *pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::Position));
			//        if (pfVertex) {
			//            pfVertex[0] = 0.0f;
			//            pfVertex[1] = 1.0f;
			//            pfVertex[2] = 0.0f;
			//        }

			// Use the following vertex order when seen it as a fullscreen quad
			//   (2)..(3)
			//   .     .
			//   .     .
			//   (0)..(1)

			// Set clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1):
			//   (-1, 1)..(1, 1)
			//   .         .
			//   .         .
			//   (-1,-1)..(1,-1)

			// Use traditional OpenGL texture coordinate system:
			//   (0,1)..(1,1)
			//   .      .
			//   .      .
			//   (0,0)..(1,0)

			// Setup vertex 0
			m_pVertexBuffer->SetFloat(0, VertexBuffer::Position, 0, -1.0f, -1.0f, 0.0f);
			m_pVertexBuffer->SetFloat(0, VertexBuffer::TexCoord, 0,  0.0f, 0.0f);

			// Setup vertex 1
			m_pVertexBuffer->SetFloat(1, VertexBuffer::Position, 0, 1.0f, -1.0f, 0.0f);
			m_pVertexBuffer->SetFloat(1, VertexBuffer::TexCoord, 0, 1.0f, 0.0f);

			// Setup vertex 2
			m_pVertexBuffer->SetFloat(2, VertexBuffer::Position, 0, -1.0f, 1.0f, 0.0f);
			m_pVertexBuffer->SetFloat(2, VertexBuffer::TexCoord, 0, 0.0f, 1.0f);

			// Setup vertex 3
			m_pVertexBuffer->SetFloat(3, VertexBuffer::Position, 0, 1.0f, 1.0f, 0.0f);
			m_pVertexBuffer->SetFloat(3, VertexBuffer::TexCoord, 0, 1.0f, 1.0f);

			// Now that we have filled the buffer with our data, unlock it. Locked buffers
			// CANNOT be used for rendering!
			m_pVertexBuffer->Unlock();
		}
	} else {
		// THIS should never, never happen! :)
	}

	{ // Setup the renderer texture buffer for our quad
		// Create the image we want to use as texture (to keep this sample free of external resources, we don't just load in an image)
		// -> Intentionally low resolution so that one can see that it's a texture, and also can see the texture filtering in action
		const uint32 nWidth  = 16;
		const uint32 nHeight = 16;
		Image cImage = Image::CreateImage(DataByte, ColorGrayscale, Vector3i(nWidth, nHeight, 1));
		{ // Fill the image
			ImageBuffer *pImageBuffer = cImage.GetBuffer();

			// Create the texture data containing a spot and a white box at (0, 0)..(1, 1) in image space (0, 0)..(<image width>, <image height>)
			const float fWidthHalf  = static_cast<float>(nWidth/2)-0.5f;
			const float fHeightHalf = static_cast<float>(nHeight/2)-0.5f;
			const float fMin		= Color4::Gray.r;
			uint8 *pData = pImageBuffer->GetData();
			for (uint32 j=0; j<nWidth; j++) {
				for (uint32 i=0; i<nHeight; i++) {
					if (i < 2 && j < 2) {
						pData[j*nWidth+i] = 255;
					} else {
						const float x  = (i - fWidthHalf)/fWidthHalf;
						const float y  = (j - fHeightHalf)/fHeightHalf;
						const float ls = Math::Max(1 - (x*x + y*y), fMin);
						pData[j*nWidth+i] = static_cast<uint8>(255*ls);
					}
				}
			}
		}

		// Create the texture buffer instance by using our image data
		m_pTextureBuffer = reinterpret_cast<TextureBuffer*>(GetRenderer().CreateTextureBuffer2D(cImage));
	}
}

/**
*  @brief
*    Destructor
*/
SPTexturing::~SPTexturing()
{
	// Destroy our renderer vertex buffer (we will miss it ;-)
	if (m_pVertexBuffer)
		delete m_pVertexBuffer;

	// Destroy the texture buffer
	if (m_pTextureBuffer)
		delete m_pTextureBuffer;
}
