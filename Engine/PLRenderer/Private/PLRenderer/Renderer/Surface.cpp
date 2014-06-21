/*********************************************************\
 *  File: Surface.cpp                                    *
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
#include <PLCore/Tools/Stopwatch.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/SurfaceHandler.h"
#include "PLRenderer/Renderer/SurfacePainter.h"
#include "PLRenderer/Renderer/Backend/RendererBackend.h"
#include "PLRenderer/Renderer/Surface.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Surface::~Surface()
{
	{ // Inform all surface handler
		Iterator<SurfaceHandler*> cIterator = m_lstHandler.GetIterator();
		while (cIterator.HasNext())
			cIterator.Next()->m_pSurface = nullptr;
	}

	// Remove surface from the renderer
	m_pRenderer->RemoveSurface(*this);

	// If there'a a painter, destroy it!
	if (m_pPainter)
		delete m_pPainter;
}

/**
*  @brief
*    Set a surface painter
*/
bool Surface::SetPainter(SurfacePainter *pPainter, bool bDestroy)
{
	// Destroy current surface painter
	if (bDestroy && m_pPainter) {
		delete m_pPainter;
		m_pPainter = nullptr;
	}

	// Set new painter
	m_pPainter = pPainter;

	// Done
	return true;
}

/**
*  @brief
*    Draws the surface
*/
void Surface::Draw()
{
	// Is this surface active and is there a surface painter?
	if (m_bActive && m_pPainter) {
		// Start the stopwatch
		Stopwatch cStopwatch(true);

		// Emit paint begin event
		EventPaintBegin();

		// Begin paint
		if (m_pPainter->OnPaintBegin(*this)) {
			// Emit paint event
			EventPaint();

			// Paint
			m_pPainter->OnPaint(*this);

			// End paint
			m_pPainter->OnPaintEnd(*this);
		}

		// Emit paint end event
		EventPaintEnd();

		// Update statistics (all renderer backends must be derived from the renderer backend class in order to have a common ground, so, this cast is assumed to be safe)
		reinterpret_cast<RendererBackend*>(m_pRenderer)->GetWritableStatistics().fRenderingTime += cStopwatch.GetMilliseconds();
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Surface::Surface(Renderer &cRenderer, EType nType) :
	m_pRenderer(&cRenderer),
	m_nType(nType),
	m_bActive(true),
	m_bSwapY(false),
	m_pPainter(nullptr)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Surface::Surface(const Surface &cSource) :
	m_pRenderer(&cSource.GetRenderer()),
	m_nType(cSource.GetType()),
	m_bActive(true),
	m_bSwapY(false),
	m_pPainter(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Surface &Surface::operator =(const Surface &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Protected virtual Surface functions                   ]
//[-------------------------------------------------------]
bool Surface::IsAPISwapY() const
{
	return m_bSwapY;
}

bool Surface::UnmakeCurrent()
{
	// Done
	return true;
}

void Surface::BackupDeviceData()
{
	// Nothing to do by default
}

void Surface::RestoreDeviceData()
{
	// Nothing to do by default
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
