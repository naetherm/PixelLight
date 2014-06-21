/*********************************************************\
 *  File: ContextMacOSX_X11.h                            *
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
#include "PLRendererOpenGL/MacOSX/SurfaceWindowMacOSX_Cocoa.h"
#include "PLRendererOpenGL/MacOSX/ContextMacOSX_X11.h"
#include <X11/Xutil.h>	// Include this after the rest, else we get OS definition issues, again 


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ContextMacOSX_X11::ContextMacOSX_X11(Renderer &cRenderer, uint32 nMultisampleAntialiasingSamples) : ContextMacOSX(cRenderer, nMultisampleAntialiasingSamples),
	m_pDisplay(XOpenDisplay(nullptr))
{
}

/**
*  @brief
*    Destructor
*/
ContextMacOSX_X11::~ContextMacOSX_X11()
{
	// Is there a valid X server display connection?
	if (m_pDisplay) {
		// Close the X server display connection
		XCloseDisplay(m_pDisplay);
	}
}

/**
*  @brief
*    Returns the X server display connection
*/
Display *ContextMacOSX_X11::GetDisplay() const
{
	return m_pDisplay;
}


//[-------------------------------------------------------]
//[ Public virtual Context methods                        ]
//[-------------------------------------------------------]
PLRenderer::SurfaceWindow *ContextMacOSX_X11::CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen)
{
	// [TODO] This method should create an SurfaceWindowMacOSX_X11 instance
	return new SurfaceWindowMacOSX_Cocoa(cHandler, nNativeWindowHandle, sDisplayMode, bFullscreen);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
