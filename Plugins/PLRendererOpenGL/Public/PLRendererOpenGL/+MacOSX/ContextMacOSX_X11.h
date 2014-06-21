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


#ifndef __PLRENDEREROPENGL_CONTEXTMACOSX_X11_H__
#define __PLRENDEREROPENGL_CONTEXTMACOSX_X11_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRendererOpenGL/MacOSX/ContextMacOSX.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct _XDisplay;
typedef struct _XDisplay Display;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mac OS X OpenGL context using X11
*/
class ContextMacOSX_X11 : public ContextMacOSX {


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
		*  @param[in] nMultisampleAntialiasingSamples
		*    Multisample antialiasing samples per pixel, <=1 means no antialiasing
		*/
		ContextMacOSX_X11(Renderer &cRenderer, PLCore::uint32 nMultisampleAntialiasingSamples);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ContextMacOSX_X11();

		/**
		*  @brief
		*    Returns the X server display connection
		*
		*  @return
		*    The X server display connection, a null pointer on error
		*/
		Display *GetDisplay() const;


	//[-------------------------------------------------------]
	//[ Public virtual Context methods                        ]
	//[-------------------------------------------------------]
	public:
		virtual PLRenderer::SurfaceWindow *CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen = false) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Display *m_pDisplay;	/**< X server display connection, a null pointer on error */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_CONTEXTMACOSX_X11_H__
