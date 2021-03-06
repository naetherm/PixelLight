/*********************************************************\
 *  File: SPRTT.cpp                                      *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLMesh/MeshManager.h>
#include "SPRTT.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPRTT, "", PLRenderer::SurfacePainter, "Abstract render to texture surface painter")
pl_class_metadata_end(SPRTT)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPRTT::SPRTT(Renderer &cRenderer) : SurfacePainter(cRenderer),
	m_pMeshManager(new MeshManager(GetRenderer())),
	m_pMeshHandler(new MeshHandler()),
	m_pPositionVertexBuffer(nullptr),
	m_pColorVertexBuffer(nullptr),
	m_fRotation(0.0f)
{
	// Get/create the 'teapot' mesh
	m_pMeshHandler->SetMesh(m_pMeshManager->CreateMesh("PLMesh::MeshCreatorTeapot", true, ""));

	// Create the renderer vertex buffer for our triangle positions and texture coordinates
	m_pPositionVertexBuffer = cRenderer.CreateVertexBuffer();
	if (m_pPositionVertexBuffer) {
		// Add required vertex attributes
		m_pPositionVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		m_pPositionVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);

		// Allocate the vertex buffer
		m_pPositionVertexBuffer->Allocate(4, Usage::Static);
		
		// Setup the vertex buffer data
		if (m_pPositionVertexBuffer->Lock(Lock::WriteOnly)) {
			// Setup vertex 0
			m_pPositionVertexBuffer->SetFloat(0, VertexBuffer::Position, 0, -1.0f, -1.0f, 0.0f);
			m_pPositionVertexBuffer->SetFloat(0, VertexBuffer::TexCoord, 0,  0.0f,  0.0f);

			// Setup vertex 1
			m_pPositionVertexBuffer->SetFloat(1, VertexBuffer::Position, 0, 1.0f, -1.0f, 0.0f);
			m_pPositionVertexBuffer->SetFloat(1, VertexBuffer::TexCoord, 0, 1.0f,  0.0f);

			// Setup vertex 2
			m_pPositionVertexBuffer->SetFloat(2, VertexBuffer::Position, 0, -1.0f, 1.0f, 0.0f);
			m_pPositionVertexBuffer->SetFloat(2, VertexBuffer::TexCoord, 0,  0.0f, 1.0f);

			// Setup vertex 3
			m_pPositionVertexBuffer->SetFloat(3, VertexBuffer::Position, 0, 1.0f, 1.0f, 0.0f);
			m_pPositionVertexBuffer->SetFloat(3, VertexBuffer::TexCoord, 0, 1.0f, 1.0f);

			// Unlock the vertex buffer
			m_pPositionVertexBuffer->Unlock();
		}
	} else {
		// THIS should never, never happen! :)
	}
	m_pColorVertexBuffer = cRenderer.CreateVertexBuffer();
	if (m_pColorVertexBuffer) {
		// Add required vertex attributes
		m_pColorVertexBuffer->AddVertexAttribute(VertexBuffer::Color, 0, VertexBuffer::RGBA);

		// Allocate the vertex buffer
		m_pColorVertexBuffer->Allocate(4, Usage::Static);

		// Setup the vertex buffer data
		if (m_pColorVertexBuffer->Lock(Lock::WriteOnly)) {
			// Setup vertex 0
			m_pColorVertexBuffer->SetColor(0, Color4::Red);

			// Setup vertex 1
			m_pColorVertexBuffer->SetColor(1, Color4::Green);

			// Setup vertex 2
			m_pColorVertexBuffer->SetColor(2, Color4::White);

			// Setup vertex 3
			m_pColorVertexBuffer->SetColor(3, Color4::Blue);

			// Unlock the vertex buffer
			m_pColorVertexBuffer->Unlock();
		}
	} else {
		// THIS should never, never happen! :)
	}
}

/**
*  @brief
*    Destructor
*/
SPRTT::~SPRTT()
{
	// Destroy our mesh handler
	delete m_pMeshHandler;

	// Destroy our mesh manager
	delete m_pMeshManager;

	// Destroy our renderer vertex buffer (we will miss it ;-)
	if (m_pPositionVertexBuffer)
		delete m_pPositionVertexBuffer;
	if (m_pColorVertexBuffer)
		delete m_pColorVertexBuffer;
}
