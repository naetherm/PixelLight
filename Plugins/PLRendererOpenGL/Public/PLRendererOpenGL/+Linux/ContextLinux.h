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


#ifndef __PLRENDEREROPENGL_CONTEXTLINUX_H__
#define __PLRENDEREROPENGL_CONTEXTLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include "../../../../../External/Recommended/OpenGL/glx.h"
#include "PLRendererOpenGL/Context.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Renderer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux OpenGL context
*/
class ContextLinux : public Context {


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    The owner renderer
		*/
		ContextLinux(Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ContextLinux();

		/**
		*  @brief
		*    Returns the X server display connection
		*
		*  @return
		*    The X server display connection, a null pointer on error
		*/
		Display *GetDisplay() const;

		/**
		*  @brief
		*    Returns the primary render context
		*
		*  @return
		*    The primary render context, a null pointer on error
		*/
		GLXContext GetRenderContext() const;


	//[-------------------------------------------------------]
	//[ Public virtual Context methods                        ]
	//[-------------------------------------------------------]
	public:
		virtual bool IsValid() const override;
		virtual void MakeDummyCurrent() const override;
		virtual bool QueryDisplayModes(PLCore::Array<const PLRenderer::DisplayMode*> &lstDisplayModeList) override;
		virtual PLRenderer::SurfaceWindow *CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen = false) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Renderer 	*m_pRenderer;					/**< The owner renderer, always valid! */
		Display     *m_pDisplay;					/**< X server display connection, a null pointer on error */
		Window       m_hDummyNativeWindow;			/**< Native dummy window, always valid */
		XVisualInfo *m_pDummyVisualInfo;
		GLXContext	 m_hDummyWindowRenderContext;	/**< The render context of the OpenGL dummy window, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_CONTEXTLINUX_H__
