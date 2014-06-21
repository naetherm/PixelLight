/*********************************************************\
 *  File: ContextMacOSX.h                                *
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
#include "PLRendererOpenGL/Extensions.h"	// Include this before including the funny Apple headers, if we don't, the result will be "error: ‘PFNGLBINDBUFFERRANGEEXTPROC’ does not name a type" and x-more of those...
#include <OpenGL/OpenGL.h>
#include <PLCore/Log/Log.h>
#include <PLMath/Vector2i.h>
#include <PLRenderer/Renderer/Types.h>
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/MacOSX/ContextMacOSX.h"
#include <IOKit/graphics/IOGraphicsTypes.h>	// Include this after the rest, else we get OS definition issues, again (required for "IO8BitIndexedPixels", "IO16BitIndexedPixels" and "IO32BitIndexedPixels")


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Global functions methods                              ]
//[-------------------------------------------------------]
/**
 *  @brief
 *    Returns the number of color bits from a given display mode
 *
 *  @param[in] pCGDisplayMode
 *    Display mode to return the number of color bits from
 *
 *  @return
 *    The number of color bits from the given display mode, 0 on error
 */
uint32 GetColorBitsFromDisplayMode(CGDisplayModeRef pCGDisplayMode)
{
	uint32 nColorBits = 0;
	const CFStringRef psString = CGDisplayModeCopyPixelEncoding(pCGDisplayMode);
	if (CFStringCompare(psString, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		nColorBits = 8;
	else if (CFStringCompare(psString, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		nColorBits = 16;
	else if (CFStringCompare(psString, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		nColorBits = 32;
	return nColorBits;
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ContextMacOSX::ContextMacOSX(Renderer &cRenderer, uint32 nMultisampleAntialiasingSamples) : Context(),
	m_pRenderer(&cRenderer),
	m_pCGLContextObj(nullptr)
{
	CGLError nCGLError = kCGLNoError;

	// Search for a suitable pixel format
	const CGLPixelFormatAttribute nCGLPixelFormatAttribute[] = {
		kCGLPFAColorSize,		static_cast<CGLPixelFormatAttribute>(24),
		kCGLPFAAlphaSize,		static_cast<CGLPixelFormatAttribute>(8),
		kCGLPFADepthSize,		static_cast<CGLPixelFormatAttribute>(cRenderer.GetCapabilities().nZBufferBits),
		kCGLPFAStencilSize,		static_cast<CGLPixelFormatAttribute>(cRenderer.GetCapabilities().nStencilBits),
		kCGLPFAAccelerated,		// No software rendering
		kCGLPFADoubleBuffer,
		kCGLPFASampleBuffers,	static_cast<CGLPixelFormatAttribute>(nMultisampleAntialiasingSamples ? 1 : 0),
		kCGLPFASamples,			static_cast<CGLPixelFormatAttribute>(nMultisampleAntialiasingSamples),
		static_cast<CGLPixelFormatAttribute>(0)
	};
	GLint nNumOfVirtualScreens = 0;
	CGLPixelFormatObj pCGLPixelFormatObj = nullptr;
	nCGLError = CGLChoosePixelFormat(nCGLPixelFormatAttribute, &pCGLPixelFormatObj, &nNumOfVirtualScreens);
	if (nCGLError == kCGLNoError) {
		// Create the CGL context object
		nCGLError = CGLCreateContext(pCGLPixelFormatObj, nullptr, &m_pCGLContextObj);
		if (nCGLError == kCGLNoError) {
			// Make our CGL context object to the current one (don't use "MakeDummyCurrent()" in here, we want to see if it's working in general)
			nCGLError = CGLSetCurrentContext(m_pCGLContextObj);
			if (nCGLError == kCGLNoError) {
				// Initialize the OpenGL extensions
				GetExtensions().Init();
			} else {
				// Error!
				PL_LOG(Error, String("Failed to make the CGL context object to the current one (\"") + CGLErrorString(nCGLError) + "\")")
			}
		} else {
			// Error!
			PL_LOG(Error, String("Failed to create the CGL context object (\"") + CGLErrorString(nCGLError) + "\")")
		}

		// Destroy the pixel format (meaning, release our reference)
		CGLDestroyPixelFormat(pCGLPixelFormatObj);
	} else {
		// Error!
		PL_LOG(Error, String("Failed to choose CGL pixel format (\"") + CGLErrorString(nCGLError) + "\")")
	}
}

/**
*  @brief
*    Destructor
*/
ContextMacOSX::~ContextMacOSX()
{
	// Is there a CGL context object?
	if (m_pCGLContextObj) {
		// Set no CGL context object
		CGLSetCurrentContext(nullptr);

		// Release the CGL context object
		// -> Don't use CGLDestroyContext because this CGL context might be used in other NSOpenGLContext instances
		CGLReleaseContext(m_pCGLContextObj);
	}
}
	
/**
*  @brief
*    Returns the primary render context
*/
CGLContextObj ContextMacOSX::GetRenderContext() const
{
	return m_pCGLContextObj;
}


//[-------------------------------------------------------]
//[ Public virtual Context methods                        ]
//[-------------------------------------------------------]
bool ContextMacOSX::IsValid() const
{
	return (m_pCGLContextObj != nullptr);
}

void ContextMacOSX::MakeDummyCurrent() const
{
	// Make our CGL context object to the current one
	CGLSetCurrentContext(m_pCGLContextObj);
}

bool ContextMacOSX::QueryDisplayModes(Array<const PLRenderer::DisplayMode*> &lstDisplayModeList)
{
	// Clear old list of display modes
	for (uint32 i=0; i<lstDisplayModeList.GetNumOfElements(); i++)
		delete lstDisplayModeList[i];
	lstDisplayModeList.Clear();

	// Get list of display modes
	PL_LOG(Info, "Query available display modes")
	const CFArrayRef lstDisplayModes = CGDisplayCopyAllDisplayModes(kCGDirectMainDisplay, nullptr);
	for (CFIndex nMode=0; nMode<CFArrayGetCount(lstDisplayModes); nMode++) {
		// Get the current display mode
		const CGDisplayModeRef pCGDisplayMode = (CGDisplayModeRef)CFArrayGetValueAtIndex(lstDisplayModes, nMode);	// No C++ style cast because it's tricky in here
		const Vector2i vSize(CGDisplayModeGetWidth(pCGDisplayMode), CGDisplayModeGetHeight(pCGDisplayMode));		// Screen resolution
		const uint32   nColorBits = GetColorBitsFromDisplayMode(pCGDisplayMode);									// Number of bits for the color (for example 32)
		const uint32   nFrequency = static_cast<uint32>(CGDisplayModeGetRefreshRate(pCGDisplayMode));				// Refresh rate (for example 60)

		// First at all, we're only interested in some of the settings - as a result, we really should check if there's
		// already a display mode within our list with the interesting settings of the current found display mode
		bool bNewMode = true;
		for (uint32 i=0; i<lstDisplayModeList.GetNumOfElements(); i++) {
			const PLRenderer::DisplayMode *pDisplayMode = lstDisplayModeList[i];
			if (pDisplayMode->vSize == vSize && pDisplayMode->nColorBits == nColorBits && pDisplayMode->nFrequency == nFrequency) {
				// We already have such a display mode within our list!
				bNewMode = false;

				// Get us out of this loop right now!
				i = lstDisplayModeList.GetNumOfElements();
			}
		}
		if (bNewMode) {
			// Get required information
			PLRenderer::DisplayMode *pDisplayMode = new PLRenderer::DisplayMode;
			pDisplayMode->vSize		 = vSize;
			pDisplayMode->nColorBits = nColorBits;
			pDisplayMode->nFrequency = nFrequency;

			// Give out log message
			String sTemp;
			if (pDisplayMode->nFrequency) {
				sTemp = String::Format("Found: %dx%dx%d %d Hz", pDisplayMode->vSize.x, pDisplayMode->vSize.y,
																pDisplayMode->nColorBits, pDisplayMode->nFrequency);
			} else {
				sTemp = String::Format("Found: %dx%dx%d", pDisplayMode->vSize.x, pDisplayMode->vSize.y, pDisplayMode->nColorBits);
			}

			// Add found display mode to list
			PL_LOG(Info, sTemp)
			lstDisplayModeList.Add(pDisplayMode);
		}
	}

	// Was at least one display mode found?
	if (lstDisplayModeList.IsEmpty()) {
		PL_LOG(Error, "No available & supported display modes found!")

		// Error!
		return false;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
