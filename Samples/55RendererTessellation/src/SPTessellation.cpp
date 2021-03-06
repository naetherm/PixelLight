/*********************************************************\
 *  File: SPTessellation.cpp                             *
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
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "SPTessellation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPTessellation, "", PLRenderer::SurfacePainter, "Abstract tessellation surface painter")
pl_class_metadata_end(SPTessellation)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPTessellation::SPTessellation(Renderer &cRenderer) : SurfacePainter(cRenderer),
	m_pVertexBuffer(GetRenderer().CreateVertexBuffer()),
	m_pIndexBuffer(GetRenderer().CreateIndexBuffer()),
	m_fRotation(0.0f),
	m_fTessellationLevelInner(3.0f),
	m_fTessellationLevelOuter(2.0f)
{
	// Geometry is from: http://prideout.net/blog/?p=48 (Philip Rideout, "The Little Grasshopper - Graphics Programming Tips")

	// Setup the renderer vertex buffer
	if (m_pVertexBuffer) {
		static const float VertexData[] = {
			 0.000f,  0.000f,  1.000f,
			 0.894f,  0.000f,  0.447f,
			 0.276f,  0.851f,  0.447f,
			-0.724f,  0.526f,  0.447f,
			-0.724f, -0.526f,  0.447f,
			 0.276f, -0.851f,  0.447f,
			 0.724f,  0.526f, -0.447f,
			-0.276f,  0.851f, -0.447f,
			-0.894f,  0.000f, -0.447f,
			-0.276f, -0.851f, -0.447f,
			 0.724f, -0.526f, -0.447f,
			 0.000f,  0.000f, -1.000f
		};
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		m_pVertexBuffer->Allocate(sizeof(VertexData)/sizeof(float)/3, Usage::Static);
		m_pVertexBuffer->CopyDataFrom(VertexData);
	}

	// Setup the renderer index buffer
	if (m_pIndexBuffer) {
		static const int IndexData[] = {
			 2,  1,  0,
			 3,  2,  0,
			 4,  3,  0,
			 5,  4,  0,
			 1,  5,  0,
			11,  6,  7,
			11,  7,  8,
			11,  8,  9,
			11,  9, 10,
			11, 10,  6,
			 1,  2,  6,
			 2,  3,  7,
			 3,  4,  8,
			 4,  5,  9,
			 5,  1, 10,
			 2,  7,  6,
			 3,  8,  7,
			 4,  9,  8,
			 5, 10,  9,
			 1,  6, 10
		};
		m_pIndexBuffer->SetElementType(IndexBuffer::UInt);
		m_pIndexBuffer->Allocate(sizeof(IndexData)/sizeof(int), Usage::Static);
		m_pIndexBuffer->CopyDataFrom(IndexData);
	} else {
		// THIS should never, never happen! :)
	}
}

/**
*  @brief
*    Destructor
*/
SPTessellation::~SPTessellation()
{
	// Destroy our renderer vertex buffer (we will miss it ;-)
	if (m_pVertexBuffer)
		delete m_pVertexBuffer;
}
