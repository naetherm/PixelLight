/*********************************************************\
 *  File: SurfaceWindow.cpp                              *
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
#include "PLRenderer/Renderer/SurfaceWindowHandler.h"
#include "PLRenderer/Renderer/SurfaceHandler.h"
#include "PLRenderer/Renderer/SurfaceWindow.h"


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
SurfaceWindow::~SurfaceWindow()
{
	// Clean up the surface
	if (m_pHandler) {
		// Inform handler
		m_pHandler->m_pSurfaceHandler->SetSurface(nullptr);
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SurfaceWindow::SurfaceWindow(SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, bool bFullscreen) :
	Surface(*cHandler.GetRenderer(), Surface::Window),
	m_bIsFullscreen(bFullscreen),
	m_pHandler(&cHandler),
	m_nNativeWindowHandle(nNativeWindowHandle)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
SurfaceWindow::SurfaceWindow(const SurfaceWindow &cSource) : Surface(cSource.GetRenderer(), cSource.GetType()),
	m_bIsFullscreen(cSource.IsFullscreen()),
	m_pHandler(cSource.GetHandler()),
	m_nNativeWindowHandle(cSource.GetNativeWindowHandle())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
SurfaceWindow &SurfaceWindow::operator =(const SurfaceWindow &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
