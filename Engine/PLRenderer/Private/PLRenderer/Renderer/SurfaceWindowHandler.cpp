/*********************************************************\
 *  File: SurfaceWindowHandler.cpp                       *
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
#include "PLRenderer/Renderer/SurfaceWindow.h"
#include "PLRenderer/Renderer/SurfaceWindowHandler.h"


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
*    Constructor
*/
SurfaceWindowHandler::SurfaceWindowHandler() :
	m_pRenderer(nullptr),
	m_pSurfaceHandler(new SurfaceHandler())
{
}

/**
*  @brief
*    Destructor
*/
SurfaceWindowHandler::~SurfaceWindowHandler()
{
	// Destroy the renderer surface
	DeInit();
	delete m_pSurfaceHandler;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the surface window handler
*/
void SurfaceWindowHandler::Init(Renderer &cRenderer, handle nNativeWindowHandle, const DisplayMode &sDisplayMode, bool bFullscreen)
{
	// Create the surface window
	m_pRenderer = &cRenderer;
	m_pSurfaceHandler->SetSurface(m_pRenderer->CreateSurfaceWindow(*this, nNativeWindowHandle, sDisplayMode, bFullscreen));
}

/**
*  @brief
*    De-initializes the surface window handler
*/
void SurfaceWindowHandler::DeInit()
{
	Surface *pSurface = m_pSurfaceHandler->GetSurface();
	if (pSurface) {
		static_cast<SurfaceWindow*>(pSurface)->m_pHandler = nullptr;
		m_pSurfaceHandler->SetSurface(nullptr);
		delete pSurface;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
SurfaceWindowHandler::SurfaceWindowHandler(const SurfaceWindowHandler &cSource) :
	m_pRenderer(cSource.GetRenderer()),
	m_pSurfaceHandler(new SurfaceHandler())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
SurfaceWindowHandler &SurfaceWindowHandler::operator =(const SurfaceWindowHandler &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
