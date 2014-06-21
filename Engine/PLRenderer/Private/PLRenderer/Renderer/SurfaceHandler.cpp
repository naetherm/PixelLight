/*********************************************************\
 *  File: SurfaceHandler.cpp                             *
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
#include "PLRenderer/Renderer/Surface.h"
#include "PLRenderer/Renderer/SurfaceHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SurfaceHandler::SurfaceHandler() :
	m_pSurface(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
SurfaceHandler::SurfaceHandler(const SurfaceHandler &cSource) :
	m_pSurface(nullptr)
{
	if (cSource.m_pSurface) {
		m_pSurface = cSource.m_pSurface;
		m_pSurface->m_lstHandler.Add(this);
	}
}

/**
*  @brief
*    Destructor
*/
SurfaceHandler::~SurfaceHandler()
{
	// Inform the surface
	if (m_pSurface)
		m_pSurface->m_lstHandler.Remove(this);
}

/**
*  @brief
*    Copy operator
*/
SurfaceHandler &SurfaceHandler::operator =(const SurfaceHandler &cSource)
{
	// Inform the old resource
	if (m_pSurface)
		m_pSurface->m_lstHandler.Remove(this);

	// Set new one
	m_pSurface = cSource.m_pSurface;
	if (m_pSurface)
		m_pSurface->m_lstHandler.Add(this);

	// Return this
	return *this;
}

/**
*  @brief
*    Set the renderer surface of this handler
*/
void SurfaceHandler::SetSurface(Surface *pSurface)
{
	// Inform the old surface
	if (m_pSurface)
		m_pSurface->m_lstHandler.Remove(this);

	// Set new one
	m_pSurface = pSurface;
	if (m_pSurface)
		m_pSurface->m_lstHandler.Add(this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
