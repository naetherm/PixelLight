/*********************************************************\
 *  File: ExtensionsWindows.cpp                          *
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
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialize the supported extensions
*/
bool Extensions::Init(bool bUseExtensions)
{
	// Should the extensions be used?
	if (bUseExtensions) {
		m_bInitialized = true;
	} else {
		ResetExtensions();
		m_bInitialized = true;
		PL_LOG(Info, "Use no hardware supported extensions")

		// Done
		return true;
	}


	//[-------------------------------------------------------]
	//[ WGL (Windows only) definitions                        ]
	//[-------------------------------------------------------]
	// WGL_ARB_extensions_string
	wglGetExtensionsStringARB = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(wglGetProcAddress("wglGetExtensionsStringARB"));
	m_bWGL_ARB_extensions_string = (wglGetExtensionsStringARB != nullptr);

	// Print a list of all available extensions into the log
	PL_LOG(Info, "Extensions info:")
	WriteExtensionStringIntoLog(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
	if (m_bWGL_ARB_extensions_string)
		WriteExtensionStringIntoLog(reinterpret_cast<const char*>(wglGetExtensionsStringARB(wglGetCurrentDC())));

	// WGL_EXT_swap_control
	if (IsSupported("WGL_EXT_swap_control")) {
		wglSwapIntervalEXT	  = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>	  (wglGetProcAddress("wglSwapIntervalEXT"));
		wglGetSwapIntervalEXT = reinterpret_cast<PFNWGLGETSWAPINTERVALEXTPROC>(wglGetProcAddress("wglGetSwapIntervalEXT"));
		if (!wglSwapIntervalEXT || !wglGetSwapIntervalEXT) {
			PL_LOG(Info, "Couldn't use extension 'WGL_EXT_swap_control'!")
			m_bWGL_EXT_swap_control = false;
		} else {
			m_bWGL_EXT_swap_control = true;
		}
	} else {
		m_bWGL_EXT_swap_control = false;
	}

	// WGL_ARB_pixel_format
	if (IsSupported("WGL_ARB_pixel_format")) {
		wglGetPixelFormatAttribivARB = reinterpret_cast<PFNWGLGETPIXELFORMATATTRIBIVARBPROC>(wglGetProcAddress("wglGetPixelFormatAttribivARB"));
		wglGetPixelFormatAttribfvARB = reinterpret_cast<PFNWGLGETPIXELFORMATATTRIBFVARBPROC>(wglGetProcAddress("wglGetPixelFormatAttribfvARB"));
		wglChoosePixelFormatARB		 = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>		(wglGetProcAddress("wglChoosePixelFormatARB"));
		if (!wglGetPixelFormatAttribivARB || !wglGetPixelFormatAttribfvARB || !wglChoosePixelFormatARB) {
			PL_LOG(Info, "Couldn't use extension 'WGL_ARB_pixel_format'!")
			m_bWGL_ARB_pixel_format = false;
		} else {
			m_bWGL_ARB_pixel_format = true;
		}
	} else {
		m_bWGL_ARB_pixel_format = false;
	}

	// WGL_ARB_render_texture
	if (IsSupported("WGL_ARB_render_texture")) {
		wglBindTexImageARB	   = reinterpret_cast<PFNWGLBINDTEXIMAGEARBPROC>	(wglGetProcAddress("wglBindTexImageARB"));
		wglReleaseTexImageARB  = reinterpret_cast<PFNWGLRELEASETEXIMAGEARBPROC> (wglGetProcAddress("wglReleaseTexImageARB"));
		if (!wglBindTexImageARB || !wglReleaseTexImageARB) {
			PL_LOG(Info, "Couldn't use extension 'WGL_ARB_render_texture'!")
			m_bWGL_ARB_render_texture = false;
		} else {
			m_bWGL_ARB_render_texture = true;
		}
	} else {
		m_bWGL_ARB_render_texture = false;
	}

	// WGL_ARB_make_current_read
	if (IsSupported("WGL_ARB_make_current_read")) {
		wglMakeContextCurrentARB = reinterpret_cast<PFNWGLMAKECONTEXTCURRENTARBPROC>(wglGetProcAddress("wglMakeContextCurrentARB"));
		wglGetCurrentReadDCARB   = reinterpret_cast<PFNWGLGETCURRENTREADDCARBPROC>  (wglGetProcAddress("wglGetCurrentReadDCARB"));
		if (!wglMakeContextCurrentARB || !wglGetCurrentReadDCARB) {
			PL_LOG(Info, "Couldn't use extension 'WGL_ARB_make_current_read'!")
			m_bWGL_ARB_make_current_read = false;
		} else {
			m_bWGL_ARB_make_current_read = true;
		}
	} else {
		m_bWGL_ARB_make_current_read = false;
	}

	// WGL_ARB_multisample
	m_bWGL_ARB_multisample = IsSupported("WGL_ARB_multisample");

	// WGL_ATI_pixel_format_float
	m_bWGL_ATI_pixel_format_float = IsSupported("WGL_ATI_pixel_format_float");

	// WGL_NV_float_buffer
	m_bWGL_NV_float_buffer = IsSupported("WGL_NV_float_buffer");


	// Initialize the supported universal extensions
	return InitUniversal();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
