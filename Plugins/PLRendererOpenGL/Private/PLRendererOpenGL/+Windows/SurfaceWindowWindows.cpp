/*********************************************************\
 *  File: SurfaceWindowWindows.cpp                       *
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
#include <PLMath/Math.h>
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/+Windows/ContextWindows.h"
#include "PLRendererOpenGL/+Windows/SurfaceWindowWindows.h"


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
SurfaceWindowWindows::~SurfaceWindowWindows()
{
	// De-initialize the OpenGL surface window
	DeInit();
}

/**
*  @brief
*    Returns the private GDI device context
*/
HDC SurfaceWindowWindows::GetDevice() const
{
	return m_hDC;
}

/**
*  @brief
*    Returns the visual
*/
HDC SurfaceWindowWindows::GetVisual() const
{
	return m_hDC;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SurfaceWindowWindows::SurfaceWindowWindows(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen) :
	SurfaceWindow(cHandler, nNativeWindowHandle, sDisplayMode, bFullscreen),
	m_hDC(nullptr)
{
	// Initialize the OpenGL surface window
	Init();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::SurfaceWindow functions    ]
//[-------------------------------------------------------]
bool SurfaceWindowWindows::GetGamma(float &fRed, float &fGreen, float &fBlue) const
{
	WORD nRamp[256*3];

	if (!GetDeviceGammaRamp(m_hDC, nRamp))
		return false; // Error!
	float fRGB[3] = {1.0f, 1.0f, 1.0f};
	for (int i=0; i<3; i++) {
		float fSum = 0.0;
		int   nCount = 0;
		int   nMin   = 256*i;
		int   nMax   = nMin+256;
		for (int j=nMin; j<nMax; j++) {
			if (j != 0 && nRamp[j] != 0 && nRamp[j] != 65536) {
				double B = (j % 256)/256.0;
				double A = nRamp[j]/65536.0;
				float  C = static_cast<float>(Math::Log(A)/Math::Log(B));
				fSum += C;
				nCount++;
			}
		}
		fRGB[i] = fSum/nCount;
	}
	fRed   = fRGB[0];
	fGreen = fRGB[1];
	fBlue  = fRGB[2];

	// Done
	return true;
}

bool SurfaceWindowWindows::SetGamma(float fRed, float fGreen, float fBlue)
{
	// Clamp color components
	if (fRed   < 0.01f)
		fRed   = 0.01f;
	if (fGreen < 0.01f)
		fGreen = 0.01f;
	if (fBlue  < 0.01f)
		fBlue  = 0.01f;
	if (fRed   > 4.0f)
		fRed   = 4.0f;
	if (fGreen > 4.0f)
		fGreen = 4.0f;
	if (fBlue  > 4.0f)
		fBlue  = 4.0f;

	WORD nRamp[256*3];
	const float fRGB[3] = {fRed, fGreen, fBlue};
	for (int i=0; i<3; i++) {
		const int nMin = 256*i;
		const int nMax = nMin+256;
		for (int j=nMin; j<nMax; j++)
			nRamp[j] = static_cast<WORD>(Math::Pow(static_cast<float>((j % 256)/256.0), static_cast<float>(fRGB[i]))*65536);
	}

	// Gamma was changed...
	m_bGammaChanged = true;

	// Call the OS gamma ramp function
	return (SetDeviceGammaRamp(m_hDC, nRamp) != 0);
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Surface functions          ]
//[-------------------------------------------------------]
Vector2i SurfaceWindowWindows::GetSize() const
{
	if (GetNativeWindowHandle()) {
		RECT sRect;
		GetClientRect(reinterpret_cast<HWND>(GetNativeWindowHandle()), &sRect);
		return Vector2i(sRect.right, sRect.bottom);
	} else {
		return Vector2i::Zero;
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Surface functions         ]
//[-------------------------------------------------------]
bool SurfaceWindowWindows::Init()
{
	// First check if there is a native window handle
	const handle nNativeWindowHandle = GetNativeWindowHandle();
	if (nNativeWindowHandle) {
		// Initialize OpenGL window
		int nBitsPerPixel = 16;

		// Get color depth
		if (m_bIsFullscreen) {
			// Find correct display mode
			uint32 nDisplayMode = 0;
			bool bFound = false;
			DEVMODE DevMode;
			while (EnumDisplaySettings(nullptr, nDisplayMode++, &DevMode)) {
				// Get required information
				if (m_sDisplayMode.vSize.x	  == static_cast<int>(DevMode.dmPelsWidth)  &&
					m_sDisplayMode.vSize.y	  == static_cast<int>(DevMode.dmPelsHeight) &&
					m_sDisplayMode.nColorBits == DevMode.dmBitsPerPel &&
					(!m_sDisplayMode.nFrequency || m_sDisplayMode.nFrequency  == DevMode.dmDisplayFrequency)) {
					bFound = true;
					break;
				}
			}
			if (bFound) {
				// Get color depth from settings
				nBitsPerPixel = m_sDisplayMode.nColorBits;

				// Change display settings
				PL_LOG(Info, "Go into fullscreen mode")
				GetRenderer().BackupDeviceObjects();
				if (ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
					PL_LOG(Error, "Couldn't set display mode!")

					// Error!
					return false;
				}
				GetRenderer().RestoreDeviceObjects();
			} else {
				PL_LOG(Error, "No correct display setting was found, can't change to fullscreen!")
				m_bIsFullscreen = false;
			}
		}

		// Get desktop color depth
		if (!m_bIsFullscreen) {
			HDC hDeskTopDC = GetDC(nullptr);
			nBitsPerPixel = GetDeviceCaps(hDeskTopDC, BITSPIXEL);
			ReleaseDC(nullptr, hDeskTopDC);
		}

		// Setup pixel format
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),										// Size of this pixel format descriptor
			1,																	// Version number
			PFD_DRAW_TO_WINDOW |												// Format must support window
			PFD_SUPPORT_OPENGL |												// Format must support OpenGL
			PFD_DOUBLEBUFFER,													// Must support double buffering
			PFD_TYPE_RGBA,														// Request an RGBA format
			static_cast<UCHAR>(nBitsPerPixel),									// Select our color depth
			0, 0, 0, 0, 0, 0,													// Color bits ignored
			0,																	// No alpha buffer
			0,																	// Shift bit ignored
			0,																	// No accumulation buffer
			0, 0, 0, 0,															// Accumulation bits ignored
			static_cast<BYTE>(GetRenderer().GetCapabilities().nZBufferBits),	// Z-buffer (depth buffer)  
			static_cast<BYTE>(GetRenderer().GetCapabilities().nStencilBits),	// Stencil buffer bits
			0,																	// No auxiliary buffer
			PFD_MAIN_PLANE,														// Main drawing layer
			0,																	// Reserved
			0, 0, 0																// Layer masks ignored
		};

		// Setup OpenGL device context
		PL_LOG(Info, "Setup OpenGL device context")

		// Get multisample antialiasing samples per pixel
		const uint32 nMultisampleAntialiasingSamples = static_cast<Renderer&>(GetRenderer()).GetMultisampleAntialiasingSamples();

		// Search for a suitable pixel format
		int nAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
			WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
			WGL_COLOR_BITS_ARB,     nBitsPerPixel,
			WGL_DEPTH_BITS_ARB,     GetRenderer().GetCapabilities().nZBufferBits,
			WGL_STENCIL_BITS_ARB,   GetRenderer().GetCapabilities().nStencilBits,
			WGL_ACCUM_BITS_ARB,     0,
			WGL_SAMPLE_BUFFERS_ARB, nMultisampleAntialiasingSamples ? GL_TRUE : GL_FALSE,
			WGL_SAMPLES_ARB,        nMultisampleAntialiasingSamples,
			0,						0
		};
		int nPixelFormats[256];
		uint32 nPFormats = 0;

		// Get window device context
		m_hDC = ::GetDC(reinterpret_cast<HWND>(nNativeWindowHandle));
		if (m_hDC) { // Did we get a device context?
			int nBestFormat  = 0;
			int nBestSamples = 0;

			// Choose pixel format
			PL_LOG(Info, "Search for a suitable pixel format")
			if (wglChoosePixelFormatARB) {
				// Test through multiple color bits in case the desired value is not available
				int nCurrentBitsPerPixel = nBitsPerPixel;
				while (nCurrentBitsPerPixel && (!wglChoosePixelFormatARB(m_hDC, nAttribs, nullptr, sizeof(nPixelFormats)/sizeof(int), nPixelFormats, &nPFormats) || !nPFormats)) {
					switch (nCurrentBitsPerPixel) {
						case 32:
							nCurrentBitsPerPixel = 24;
							break;

						case 24:
							nCurrentBitsPerPixel = 16;
							break;

						case 16:
						default:
							nCurrentBitsPerPixel = 0;
							break;
					}
					nAttribs[7] = nCurrentBitsPerPixel;	// Select our color depth
				}
				if (nPFormats) {
					int nMinDiff = 0x7FFFFFFF;
					int nAttrib  = WGL_SAMPLES_ARB;
					int nSamples;

					// Search for the pixel format with the closest number of multisample samples to the requested
					for (uint32 i=0; i<nPFormats; i++) {
						wglGetPixelFormatAttribivARB(m_hDC, nPixelFormats[i], 0, 1, &nAttrib, &nSamples);
						int nDiff = Math::Abs(nSamples - static_cast<int>(nMultisampleAntialiasingSamples));
						if (nDiff < nMinDiff) {
							nMinDiff	 = nDiff;
							nBestFormat  = i;
							nBestSamples = nSamples;
						}
					}
				}
			}

			// Use fallback
			if (!nPFormats) {
				nPixelFormats[0] = ChoosePixelFormat(m_hDC, &pfd);
				if (!nPixelFormats[0]) { // Did Windows find a matching pixel format?
					PL_LOG(Error, "Can't find a suitable pixel format")
					DeInit();

					// Error!
					return false;
				}
			}

			// Enable multisampling
			if (nBestSamples > 0)
				glEnable(GL_MULTISAMPLE_ARB);

			// Set the pixel format
			PL_LOG(Info, "Set the pixel format")
			if (SetPixelFormat(m_hDC, nPixelFormats[nBestFormat], &pfd)) { // Are we able to set the pixel format?
				// Backup gamma
				GetGamma(m_fGammaBackup[0], m_fGammaBackup[1], m_fGammaBackup[2]);
				m_bGammaChanged = false;

				// Done
				return true;
			} else {
				PL_LOG(Error, "Can't set the pixel format")
				DeInit();
			}
		} else {
			PL_LOG(Error, "Can't setup OpenGL device context")
			DeInit();
		}
	}

	// Error!
	return false;
}

void SurfaceWindowWindows::DeInit()
{
	// First check if there is a native window handle
	const handle nNativeWindowHandle = GetNativeWindowHandle();
	if (nNativeWindowHandle) {
		// We're going to perform a cruel act - so give OpenGL a chance to flush and finish it's data...
		// just so be on the safe side, you never now...
		glFinish();

		// If this is the current render target, make the main window to the new current one
		Renderer &cRenderer = static_cast<Renderer&>(GetRenderer());
		if (cRenderer.GetRenderTarget() == this)
			cRenderer.SetRenderTarget(nullptr);

		// Reset gamma - but only when the gamma was changed by using "SetGamma()"
		if (m_bGammaChanged) {
			SetGamma(m_fGammaBackup[0], m_fGammaBackup[1], m_fGammaBackup[2]);
			m_bGammaChanged = false;
		}

		// Release the Windows DC
		if (m_hDC && !ReleaseDC(reinterpret_cast<HWND>(nNativeWindowHandle), m_hDC))
			PL_LOG(Error, "Release OpenGL device context failed")
		m_hDC = nullptr;

		// Is fullscreen?
		if (m_bIsFullscreen) {
			// Reset display settings
			PL_LOG(Info, "Set display mode to default")
			GetRenderer().BackupDeviceObjects();
			ChangeDisplaySettings(nullptr, CDS_RESET);
			GetRenderer().RestoreDeviceObjects();
		}
	}
}

bool SurfaceWindowWindows::MakeCurrent(uint8 nFace)
{
	if (m_hDC) {
		// Get the OpenGL render context
		ContextWindows &cContextWindows = static_cast<ContextWindows&>(static_cast<Renderer&>(GetRenderer()).GetContext());

		// Get the OpenGL render context
		HGLRC pHGLRC = cContextWindows.GetRenderContext();
		if (pHGLRC) {
			// Enable/disable multisample
			GetRenderer().SetRenderState(PLRenderer::RenderState::MultisampleEnable, (GetRenderer().GetCapabilities().nMultisampleAntialiasingSamples > 1));

			// Make this surface current
			return (wglMakeCurrent(m_hDC, pHGLRC) == TRUE);
		}
	}

	// Error!
	return false;
}

bool SurfaceWindowWindows::Present()
{
	// Swap interval (vertical synchronization) setting changed? (this setting is connected with the window, therefore we must do this update for every window)
	Renderer &cRenderer = static_cast<Renderer&>(GetRenderer());
	if (m_nSwapInterval != static_cast<int>(cRenderer.GetSwapInterval())) {
		m_nSwapInterval = cRenderer.GetSwapInterval();
		if (cRenderer.GetContext().GetExtensions().IsWGL_EXT_swap_control()) 
			wglSwapIntervalEXT(m_nSwapInterval);
	}

	// Swap buffers
	return (m_hDC && SwapBuffers(m_hDC) == TRUE);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
