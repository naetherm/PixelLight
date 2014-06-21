/*********************************************************\
 *  File: SurfaceWindowLinux.cpp                         *
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
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/Linux/ContextLinux.h"
#include "PLRendererOpenGL/Linux/SurfaceWindowLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SurfaceWindowLinux::~SurfaceWindowLinux()
{
	// De-initialize the OpenGL surface window
	DeInit();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SurfaceWindowLinux::SurfaceWindowLinux(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen) :
	SurfaceWindow(cHandler, nNativeWindowHandle, sDisplayMode, bFullscreen),
	m_nNativeWindowHandle(NULL_HANDLE),
	m_nOldSizeID(-1),
	m_nOldRotation(-1)
{
	// Initialize the OpenGL surface window
	Init();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::SurfaceWindow functions    ]
//[-------------------------------------------------------]
bool SurfaceWindowLinux::GetGamma(float &fRed, float &fGreen, float &fBlue) const
{
	// Get the Linux OpenGL render context
	ContextLinux &cContextLinux = static_cast<ContextLinux&>(static_cast<Renderer&>(GetRenderer()).GetContext());

	// Get the X server display connection
	Display *pDisplay = cContextLinux.GetDisplay();

	// Get gamma information
	XF86VidModeGamma sXF86VidModeGamma;
	if (XF86VidModeGetGamma(pDisplay, XDefaultScreen(pDisplay), &sXF86VidModeGamma)) {
		fRed   = sXF86VidModeGamma.red;
		fGreen = sXF86VidModeGamma.green;
		fBlue  = sXF86VidModeGamma.blue;

		// Done
		return true;
	}

	// Set fallback settings so that the reference parameters are never within an undefined state
	fRed   = 0.0f;
	fGreen = 0.0f;
	fBlue  = 0.0f;

	// Error!
	return false;
}

bool SurfaceWindowLinux::SetGamma(float fRed, float fGreen, float fBlue)
{
	if (static_cast<int>(fRed*10) <= 10 && static_cast<int>(fGreen*10) <= 10 && static_cast<int>(fBlue*10) <= 10) {
		// Get the Linux OpenGL render context
		ContextLinux &cContextLinux = static_cast<ContextLinux&>(static_cast<Renderer&>(GetRenderer()).GetContext());

		// Get the X server display connection
		Display *pDisplay = cContextLinux.GetDisplay();

		// Gamma was changed...
		m_bGammaChanged = true;

		// Call the OS gamma ramp function
		XF86VidModeGamma sXF86VidModeGamma;
		sXF86VidModeGamma.red   = fRed;
		sXF86VidModeGamma.green = fGreen;
		sXF86VidModeGamma.blue  = fBlue;
		if (XF86VidModeSetGamma(pDisplay, XDefaultScreen(pDisplay), &sXF86VidModeGamma))
			return true; // Done
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Surface functions          ]
//[-------------------------------------------------------]
Vector2i SurfaceWindowLinux::GetSize() const
{
	if (GetNativeWindowHandle()) {
		::Window nRootWindow = 0;
		int nPositionX = 0, nPositionY = 0;
		unsigned int nWidth = 0, nHeight = 0, nBorder = 0, nDepth = 0;

		// Get the Linux context implementation
		ContextLinux &cContextLinux = static_cast<ContextLinux&>(static_cast<Renderer&>(GetRenderer()).GetContext());

		// Get the X server display connection
		Display *pDisplay = cContextLinux.GetDisplay();
		if (pDisplay) {
			// Get X window geometry information
			XGetGeometry(pDisplay, GetNativeWindowHandle(), &nRootWindow, &nPositionX, &nPositionY, &nWidth, &nHeight, &nBorder, &nDepth);
		}

		// Return the window size
		return Vector2i(nWidth, nHeight);
	} else {
		return Vector2i::Zero;
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Surface functions         ]
//[-------------------------------------------------------]
bool SurfaceWindowLinux::Init()
{
	bool bResult = true;	// No error by default

	// First check if there's a native window
	const handle nNativeWindowHandle = GetNativeWindowHandle();
	if (nNativeWindowHandle) {
		// Backup the native window handle
		m_nNativeWindowHandle = nNativeWindowHandle;

		// Backup gamma
		GetGamma(m_fGammaBackup[0], m_fGammaBackup[1], m_fGammaBackup[2]);
		m_bGammaChanged = false;

		// Is fullscreen?
		if (m_bIsFullscreen) {
			// Get the renderer instance
			Renderer &cRenderer = static_cast<Renderer&>(GetRenderer());

			// Get the X server display connection
			Display *pDisplay = XOpenDisplay(nullptr);
			if (pDisplay) {
				const int nScreen = XDefaultScreen(pDisplay);

				// Get XRR screen configuration (don't forget "XRRFreeScreenConfigInfo()" if you no longer need it)
				XRRScreenConfiguration *pXRRScreenConfiguration = XRRGetScreenInfo(pDisplay, RootWindow(pDisplay, nScreen));

				// Get specific configuration information out of our screen configuration
				int nNumOfModes = 0;
				XRRScreenSize *pXRRScreenSize = XRRConfigSizes(pXRRScreenConfiguration, &nNumOfModes);
				if (nNumOfModes) {
					// Find correct display mode
					int nBestMode = 0;
					bool bFound = false;
					for (int i=0; i<nNumOfModes; i++) {
						if (m_sDisplayMode.vSize.x == pXRRScreenSize[i].width &&
							m_sDisplayMode.vSize.y == pXRRScreenSize[i].height) {
							nBestMode = i;
							bFound = true;
							break;
						}
					}
					if (bFound) {
						// Save desktop-resolution before switching modes
						m_nOldSizeID = XRRConfigCurrentConfiguration(pXRRScreenConfiguration, &m_nOldRotation);

						// Change display settings
						PL_LOG(Info, "PLRendererOpenGL fullscreen mode: Go into fullscreen mode")
						cRenderer.BackupDeviceObjects();
						if (XRRSetScreenConfig(pDisplay, pXRRScreenConfiguration, RootWindow(pDisplay, nScreen), nBestMode, RR_Rotate_0, CurrentTime) != RRSetConfigSuccess) {
							// Error!
							PL_LOG(Error, "PLRendererOpenGL fullscreen mode: Couldn't set display mode!")
							m_bIsFullscreen = false;
							bResult = false;
						}
						cRenderer.RestoreDeviceObjects();
					} else {
						// Error!
						PL_LOG(Error, "PLRendererOpenGL fullscreen mode: No correct display setting was found, can't change to fullscreen!")
						m_bIsFullscreen = false;
						bResult = false;
					}
				} else {
					// Error!
					PL_LOG(Error, "PLRendererOpenGL fullscreen mode: Couldn't get mode lines")
					bResult = false;
				}

				// Free XRR screen configuration
				XRRFreeScreenConfigInfo(pXRRScreenConfiguration);

				// Close X server display connection
				XCloseDisplay(pDisplay);
			}
		}
	}

	// Done
	return bResult;
}

void SurfaceWindowLinux::DeInit()
{
	// First check if there's a native window handle
	if (m_nNativeWindowHandle) {
		// We're going to perform a cruel act - so give OpenGL a chance to flush and finish it's data...
		// just so be on the safe side, you never now...
		glFinish();

		// Get the renderer instance
		Renderer &cRenderer = static_cast<Renderer&>(GetRenderer());

		// If this is the current render target, make the main window to the new current one
		if (cRenderer.GetRenderTarget() == this)
			cRenderer.SetRenderTarget(nullptr);

		// Reset gamma - but only when the gamma was changed by using "SetGamma()"
		if (m_bGammaChanged) {
			SetGamma(m_fGammaBackup[0], m_fGammaBackup[1], m_fGammaBackup[2]);
			m_bGammaChanged = false;
		}

		// Is fullscreen?
		if (m_bIsFullscreen) {
			// Get the X server display connection
			Display *pDisplay = XOpenDisplay(nullptr);
			if (pDisplay) {
				// Reset display settings
				PL_LOG(Info, "PLRendererOpenGL fullscreen mode: Set display mode to default")

				// Backup renderer device objects
				cRenderer.BackupDeviceObjects();

				// Get XRR screen configuration (don't forget "XRRFreeScreenConfigInfo()" if you no longer need it)
				const int nScreen = XDefaultScreen(pDisplay);
				XRRScreenConfiguration *pXRRScreenConfiguration = XRRGetScreenInfo(pDisplay, RootWindow(pDisplay, nScreen));

				// Set XRR screen configuration
				XRRSetScreenConfig(pDisplay, pXRRScreenConfiguration, RootWindow(pDisplay, nScreen), m_nOldSizeID, m_nOldRotation, CurrentTime);

				// Free XRR screen configuration
				XRRFreeScreenConfigInfo(pXRRScreenConfiguration);

				// Restore renderer device objects
				cRenderer.RestoreDeviceObjects();

				// Close X server display connection
				XCloseDisplay(pDisplay);
			}
		}
	}
}

bool SurfaceWindowLinux::MakeCurrent(uint8 nFace)
{
	// First check if there's a native window handle
	if (m_nNativeWindowHandle) {
		// Get the Linux OpenGL render context
		ContextLinux &cContextLinux = static_cast<ContextLinux&>(static_cast<Renderer&>(GetRenderer()).GetContext());

		// Make this surface current
		return glXMakeCurrent(cContextLinux.GetDisplay(), m_nNativeWindowHandle, cContextLinux.GetRenderContext());
	}

	// Error!
	return false;
}

bool SurfaceWindowLinux::Present()
{
	// First check if there's a native window handle
	if (m_nNativeWindowHandle) {
		// Get the Linux OpenGL render context
		Renderer &cRenderer = static_cast<Renderer&>(GetRenderer());
		ContextLinux &cContextLinux = static_cast<ContextLinux&>(cRenderer.GetContext());

		// Swap interval (vertical synchronization) setting changed? (this setting is connected with the window, therefore we must do this update for every window)
		if (m_nSwapInterval != static_cast<int>(cRenderer.GetSwapInterval())) {
			m_nSwapInterval = cRenderer.GetSwapInterval();
			if (cRenderer.GetContext().GetExtensions().IsGLX_SGI_swap_control())
				glXSwapIntervalSGI(m_nSwapInterval);
		}

		// Swap buffers
		glXSwapBuffers(cContextLinux.GetDisplay(), m_nNativeWindowHandle);

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
