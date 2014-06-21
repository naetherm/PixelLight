/*********************************************************\
 *  File: SurfacePainter.cpp                             *
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
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/SurfacePainter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SurfacePainter)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor (not implemented!)
*/
SurfacePainter::SurfacePainter(const SurfacePainter &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Constructor
*/
SurfacePainter::SurfacePainter(Renderer &cRenderer) :
	m_pRenderer(&cRenderer)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy operator
*/
SurfacePainter &SurfacePainter::operator =(const SurfacePainter &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Protected virtual SurfacePainter functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SurfacePainter::~SurfacePainter()
{
}

/**
*  @brief
*    Is called when the owner surface is paint process begins
*/
bool SurfacePainter::OnPaintBegin(Surface &cSurface)
{
	// Renderer must match!
	if (&cSurface.GetRenderer() == m_pRenderer) {
		// Set new render target
		if (m_pRenderer->SetRenderTarget(&cSurface)) {
			// Begin scene
			m_pRenderer->BeginScene();

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Is called when the owner surface is paint process ends
*/
void SurfacePainter::OnPaintEnd(Surface &cSurface)
{
	// End scene
	m_pRenderer->EndScene();
	cSurface.Present();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
