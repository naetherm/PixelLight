/*********************************************************\
 *  File: SPMeshPreview.cpp                              *
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
#include "PLMesh/MeshHandler.h"
#include "PLMesh/SPMeshPreview.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SPMeshPreview)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPMeshPreview::SPMeshPreview(Renderer &cRenderer) : SPPreview(cRenderer),
	m_pMeshHandler(new MeshHandler())
{
}

/**
*  @brief
*    Destructor
*/
SPMeshPreview::~SPMeshPreview()
{
	delete m_pMeshHandler;
}

/**
*  @brief
*    Returns the used mesh
*/
Mesh *SPMeshPreview::GetMesh() const
{
	return m_pMeshHandler->GetMesh();
}

/**
*  @brief
*    Sets the used mesh
*/
void SPMeshPreview::SetMesh(Mesh *pMesh)
{
	m_pMeshHandler->SetMesh(pMesh);
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPMeshPreview::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Set all render states to known default settings
	cRenderer.Reset();

	// Clear the frame buffer
	cRenderer.Clear();

	// Get the used mesh
	const Mesh *pMesh = m_pMeshHandler->GetMesh();
	if (pMesh) {
		// [TODO] Draw the mesh
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
