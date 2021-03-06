/*********************************************************\
 *  File: SPMultiView.cpp                                *
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
#include <PLCore/Tools/Timing.h>
#include <PLMath/Rectangle.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLMesh/MeshManager.h>
#include "SPMultiView.h"


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
pl_class_metadata(SPMultiView, "", PLRenderer::SurfacePainter, "Abstract multi view surface painter")
pl_class_metadata_end(SPMultiView)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPMultiView::SPMultiView(Renderer &cRenderer) : SurfacePainter(cRenderer),
	m_fRotation(0.0f),
	m_pMeshManager(new MeshManager(GetRenderer())),
	m_pMeshHandlerSphere(new MeshHandler()),
	m_pMeshHandlerTorus(new MeshHandler()),
	m_pMeshHandlerCube(new MeshHandler())
{
	// Create the meshes

	// Get/create the 'sphere' mesh
	m_pMeshHandlerSphere->SetMesh(m_pMeshManager->CreateMesh("PLMesh::MeshCreatorSphere", true, "Radius=\"1.0\""));

	// Get/create the 'torus' mesh
	m_pMeshHandlerTorus->SetMesh(m_pMeshManager->CreateMesh("PLMesh::MeshCreatorTorus", true, "Sides=\"15\" Rings=\"20\""));

	// Get/create the 'cube' mesh
	m_pMeshHandlerCube->SetMesh(m_pMeshManager->CreateMesh("PLMesh::MeshCreatorCube", true, "Dimension=\"1.0 1.0 1.0\""));
}

/**
*  @brief
*    Destructor
*/
SPMultiView::~SPMultiView()
{
	// Destroy our mesh handlers
	delete m_pMeshHandlerSphere;
	delete m_pMeshHandlerTorus;
	delete m_pMeshHandlerCube;

	// Destroy our mesh manager
	delete m_pMeshManager;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPMultiView::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Get the current viewport settings
	const uint32 nWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
	const uint32 nHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());
	float fMinZ, fMaxZ;
	cRenderer.GetViewport(&fMinZ, &fMaxZ);

	// Enable scissor rectangle test
	cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

	{ // Viewport 1 (red)
		// Setup viewport and reset scissor rectangle
		const Rectangle cRect(0.0f, 0.0f, static_cast<float>(nWidth/2), static_cast<float>(nHeight));
		cRenderer.SetViewport(&cRect, fMinZ, fMaxZ);
		cRenderer.SetScissorRect();

		// Clear the content of the current used render target
		cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Red);

		// Draw scene
		DrawScene(0);
	}

	{ // Viewport 2 (green)
		// Setup viewport and reset scissor rectangle
		const Rectangle cRect(static_cast<float>(nWidth/2), 0.0f, static_cast<float>(nWidth), static_cast<float>(nHeight));
		cRenderer.SetViewport(&cRect, fMinZ, fMaxZ);
		cRenderer.SetScissorRect();

		// Clear the content of the current used render target
		cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Green);

		// Draw scene
		DrawScene(1);
	}

	{ // Viewport 3 (blue)
		// Setup viewport and reset scissor rectangle
		const float fX = static_cast<float>(nWidth/2-100);
		const float fY = static_cast<float>(nHeight/2-100);
		const Rectangle cRect(fX, fY, fX+200.0f, fY+200.0f);
		cRenderer.SetViewport(&cRect, fMinZ, fMaxZ);
		cRenderer.SetScissorRect();

		// Clear the content of the current used render target
		cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Blue);

		// Draw scene
		DrawScene(2);
	}

	// Increase the rotation by the current time difference (time past since the last frame)
	// -> Normally, such work should NOT be performed within the rendering step, but we want
	//    to keep the implementation simple in here...
	m_fRotation -= Timing::GetInstance()->GetTimeDifference()*100;
}
