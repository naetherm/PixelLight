/*********************************************************\
 *  File: SurfaceWindowWindow.h                          *
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


#ifndef __PLRENDEREROPENGL_SURFACE_WINDOWWINDOWS_H__
#define __PLRENDEREROPENGL_SURFACE_WINDOWWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>	// For "HDC"
#include "PLRendererOpenGL/SurfaceWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    MS Windows OpenGL window renderer surface where we can render in
*/
class SurfaceWindowWindows : public SurfaceWindow {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ContextWindows;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SurfaceWindowWindows();

		/**
		*  @brief
		*    Returns the private GDI device context
		*
		*  @return
		*    Private GDI device context
		*/
		HDC GetDevice() const;

		/**
		*  @brief
		*    Returns the visual
		*
		*  @return
		*    Visual (for Windows the same as GetDevice())
		*/
		HDC GetVisual() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHandler
		*    Renderer surface handler this surface is assigned with (MUST be valid!)
		*  @param[in] nNativeWindowHandle
		*    Handle to the native window the renderer surface is assigned with
		*  @param[in] sDisplayMode
		*    Display mode information
		*  @param[in] bFullscreen
		*    Fullscreen mode?
		*/
		SurfaceWindowWindows(PLRenderer::SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HDC m_hDC;	/**< Private GDI device context */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::SurfaceWindow functions    ]
	//[-------------------------------------------------------]
	public:
		virtual bool GetGamma(float &fRed, float &fGreen, float &fBlue) const override;
		virtual bool SetGamma(float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::Surface functions          ]
	//[-------------------------------------------------------]
	public:
		virtual PLMath::Vector2i GetSize() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::Surface functions         ]
	//[-------------------------------------------------------]
	private:
		virtual bool Init() override;
		virtual void DeInit() override;
		virtual bool MakeCurrent(PLCore::uint8 nFace = 0) override;
		virtual bool Present() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_SURFACE_WINDOWWINDOWS_H__
