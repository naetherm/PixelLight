/*********************************************************\
 *  File: ContextLinux.h                                 *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Container/Array.h>
#include <PLRenderer/Renderer/Types.h>
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/Linux/SurfaceWindowLinux.h"
#include "PLRendererOpenGL/Linux/ContextLinux.h"


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
ContextLinux::ContextLinux(Renderer &cRenderer) : Context(),
	m_pRenderer(&cRenderer),
	m_pDisplay(nullptr),
	m_hDummyNativeWindow(NULL_HANDLE),
	m_pDummyVisualInfo(nullptr),
	m_hDummyWindowRenderContext(nullptr)
{
	// Get X server display connection
	m_pDisplay = XOpenDisplay(nullptr);
	if (m_pDisplay) {
		// Get an appropriate visual
		int nAttributeList[] = {
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_RED_SIZE,	4,
			GLX_GREEN_SIZE,	4,
			GLX_BLUE_SIZE,	4,
			GLX_DEPTH_SIZE,	16,
			0 };	// = "None"

		m_pDummyVisualInfo = glXChooseVisual(m_pDisplay, DefaultScreen(m_pDisplay), nAttributeList);
		if (m_pDummyVisualInfo) {
			// Create a GLX context
			m_hDummyWindowRenderContext = glXCreateContext(m_pDisplay, m_pDummyVisualInfo, 0, GL_TRUE);
			if (m_hDummyWindowRenderContext) {
				// Create a color map
				XSetWindowAttributes sSetWindowAttributes;
				sSetWindowAttributes.colormap = XCreateColormap(m_pDisplay, RootWindow(m_pDisplay, m_pDummyVisualInfo->screen), m_pDummyVisualInfo->visual, AllocNone);

				// Create a window
				sSetWindowAttributes.border_pixel = 0;
				sSetWindowAttributes.event_mask = 0;
				m_hDummyNativeWindow = XCreateWindow(m_pDisplay, RootWindow(m_pDisplay, m_pDummyVisualInfo->screen), 0, 0, 300,
													 300, 0, m_pDummyVisualInfo->depth, InputOutput, m_pDummyVisualInfo->visual,
													 CWBorderPixel|CWColormap|CWEventMask, &sSetWindowAttributes);
				if (m_hDummyNativeWindow) {
					// Make the internal dummy to the current render target
					MakeDummyCurrent();

					// Initialize the OpenGL extensions
					GetExtensions().Init();
				} else {
					// Error, failed to create dummy window!
				}
			} else {
				// Error, failed to create a GLX context!
			}
		} else {
			// Error, failed to get an appropriate visual!
		}
	} else {
		// Error, failed to get display!
	}
}

/**
*  @brief
*    Destructor
*/
ContextLinux::~ContextLinux()
{
	// Is there a valid X server display connection?
	if (m_pDisplay) {
		// Is the render context of the OpenGL dummy window is the currently active OpenGL render context?
		if (glXGetCurrentContext() == m_hDummyWindowRenderContext)
			glXMakeCurrent(m_pDisplay, 0L, nullptr);

		// Destroy the GLX context of the OpenGL dummy window
		if (m_hDummyWindowRenderContext != nullptr)
			glXDestroyContext(m_pDisplay, m_hDummyWindowRenderContext);

		// Destroy the dummy native window
		if (m_hDummyNativeWindow)
			XDestroyWindow(m_pDisplay, m_hDummyNativeWindow);
		if (m_pDummyVisualInfo)
			XFree(m_pDummyVisualInfo);

		// Close the X server display connection
		XCloseDisplay(m_pDisplay);
	}
}

/**
*  @brief
*    Returns the X server display connection
*/
Display *ContextLinux::GetDisplay() const
{
	return m_pDisplay;
}

/**
*  @brief
*    Returns the primary render context
*/
GLXContext ContextLinux::GetRenderContext() const
{
	return m_hDummyWindowRenderContext;
}


//[-------------------------------------------------------]
//[ Public virtual Context methods                        ]
//[-------------------------------------------------------]
bool ContextLinux::IsValid() const
{
	return (m_pDisplay != nullptr && m_hDummyNativeWindow != NULL_HANDLE && m_pDummyVisualInfo != nullptr && m_hDummyWindowRenderContext != nullptr);
}

void ContextLinux::MakeDummyCurrent() const
{
	// [TODO] What the... - usually we need to make a OpenGL context to the current one in order to call OpenGL commands, but if we uncomment the following, it doesn't work?!
	glXMakeCurrent(m_pDisplay, m_hDummyNativeWindow, m_hDummyWindowRenderContext);
}

bool ContextLinux::QueryDisplayModes(Array<const PLRenderer::DisplayMode*> &lstDisplayModeList)
{
	bool bResult = false;	// Error by default

	// Clear old list of display modes
	for (uint32 i=0; i<lstDisplayModeList.GetNumOfElements(); i++)
		delete lstDisplayModeList[i];
	lstDisplayModeList.Clear();

	// Get list of display modes
	PL_LOG(Info, "Query available display modes")

	// Get XRR screen configuration (don't forget "XRRFreeScreenConfigInfo()" if you no longer need it)
	const int nScreen = XDefaultScreen(m_pDisplay);
	XRRScreenConfiguration *pXRRScreenConfiguration = XRRGetScreenInfo(m_pDisplay, RootWindow(m_pDisplay, nScreen));

	// Get specific configuration information out of our screen configuration
	int nNumOfModes = 0;
	XRRScreenSize *pXRRScreenSize = XRRConfigSizes(pXRRScreenConfiguration, &nNumOfModes);

	// Loop through all modes
	for (int nMode=0; nMode<nNumOfModes; nMode++) {
		// First at all, we're only interested in some of the settings - as a result, we really should check if there's
		// already a display mode within our list with the interesting settings of the current found display mode
		bool bNewMode = true;
		for (uint32 i=0; i<lstDisplayModeList.GetNumOfElements(); i++) {
			const PLRenderer::DisplayMode *pDisplayMode = lstDisplayModeList[i];
			if (pDisplayMode->vSize.x == pXRRScreenSize[nMode].width && pDisplayMode->vSize.y == pXRRScreenSize[nMode].height) {
				// We already have such a display mode within our list!
				bNewMode = false;

				// Get us out of this loop right now!
				i = lstDisplayModeList.GetNumOfElements();
			}
		}
		if (bNewMode) {
			// Get required information
			PLRenderer::DisplayMode *pDisplayMode = new PLRenderer::DisplayMode;
			pDisplayMode->vSize.x	  = pXRRScreenSize[nMode].width;
			pDisplayMode->vSize.y	  = pXRRScreenSize[nMode].height;
			pDisplayMode->nColorBits  = XDefaultDepth(m_pDisplay, nScreen);

			// [TODO] Under Linux there is currently no real 32 bit visual (RGBA)
			// only 24 bit (RGB) which is equal to 32 bit in RGB (without alpha value)
			if (pDisplayMode->nColorBits == 24)
				pDisplayMode->nColorBits = 32;
			pDisplayMode->nFrequency = 0;

			// Give out a log message
			String sTemp;
			if (pDisplayMode->nFrequency) {
				sTemp = String::Format("Found: %dx%dx%d %d Hz", pDisplayMode->vSize.x, pDisplayMode->vSize.y, pDisplayMode->nColorBits, pDisplayMode->nFrequency);
			} else {
				sTemp = String::Format("Found: %dx%dx%d", pDisplayMode->vSize.x, pDisplayMode->vSize.y, pDisplayMode->nColorBits);
			}

			// Add found display mode to list
			PL_LOG(Info, sTemp)
			lstDisplayModeList.Add(pDisplayMode);
		}
	}

	// Was at least one display mode found?
	if (lstDisplayModeList.GetNumOfElements())
		bResult = true; // Success! :D
	else
		PL_LOG(Error, "No available & supported display modes found!")

	// Free XRR screen configuration
	XRRFreeScreenConfigInfo(pXRRScreenConfiguration);

	// Done
	return bResult;
}

PLRenderer::SurfaceWindow *ContextLinux::CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen)
{
	return new SurfaceWindowLinux(cHandler, nNativeWindowHandle, sDisplayMode, bFullscreen);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
