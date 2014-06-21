/*********************************************************\
 *  File: SurfaceWindowMacOSX.cpp                        *
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
#include <ApplicationServices/ApplicationServices.h>	// "#include <CoreGraphics/CGDirectDisplay.h>" would do the job, but on Apple we're forced to use the "framework"-approach instead of addressing headers the natural way, and this one here is a "sub-framework" which can't be addressed directly, so we have to use this overkill approach and this really long comment
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/MacOSX/ContextMacOSX.h"
#include "PLRendererOpenGL/MacOSX/SurfaceWindowMacOSX.h"
#include <OpenGL/OpenGL.h>	// Don't try to include this as first header, else you will get compiler errors due to definition differences


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SurfaceWindowMacOSX::~SurfaceWindowMacOSX()
{
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SurfaceWindowMacOSX::SurfaceWindowMacOSX(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen) :
	SurfaceWindow(cHandler, nNativeWindowHandle, sDisplayMode, bFullscreen)
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLRenderer::Surface functions       ]
//[-------------------------------------------------------]
bool SurfaceWindowMacOSX::Init()
{
	// Is it fullscreen?
	if (m_bIsFullscreen) {
		// Get the MacOS X context implementation
		ContextMacOSX &cContextMacOSX = static_cast<ContextMacOSX&>(static_cast<Renderer&>(GetRenderer()).GetContext());

		// [TODO] Switch to the requested resolution

		// Attach rendering context to a fullscreen drawable object
		return (CGLSetFullScreenOnDisplay(cContextMacOSX.GetRenderContext(), CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay)) == kCGLNoError);
	} else {
		// CGL only supports fullscreen rendering
	}

	// Error!
	return false;
}

void SurfaceWindowMacOSX::DeInit()
{
	// Is it fullscreen?
	if (m_bIsFullscreen) {
		// Get the MacOS X context implementation
		ContextMacOSX &cContextMacOSX = static_cast<ContextMacOSX&>(static_cast<Renderer&>(GetRenderer()).GetContext());

		// Disassociate rendering context from any drawable objects attached to it
		CGLClearDrawable(cContextMacOSX.GetRenderContext());

		// [TODO] Restore the previous resolution
	} else {
		// CGL only supports fullscreen rendering
	}
}

bool SurfaceWindowMacOSX::MakeCurrent(uint8 nFace)
{
	// Get the MacOS X context implementation
	ContextMacOSX &cContextMacOSX = static_cast<ContextMacOSX&>(static_cast<Renderer&>(GetRenderer()).GetContext());

	// Set the current rendering context
	return (CGLSetCurrentContext(cContextMacOSX.GetRenderContext())== kCGLNoError);;
}

bool SurfaceWindowMacOSX::Present()
{
	// Get the MacOS X context implementation
	ContextMacOSX &cContextMacOSX = static_cast<ContextMacOSX&>(static_cast<Renderer&>(GetRenderer()).GetContext());

	// Set the current rendering context
	return (CGLFlushDrawable(cContextMacOSX.GetRenderContext())== kCGLNoError);;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
