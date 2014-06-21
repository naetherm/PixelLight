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


#ifndef __PLRENDEREROPENGL_CONTEXTMACOSX_H__
#define __PLRENDEREROPENGL_CONTEXTMACOSX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OpenGL/CGLTypes.h>
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
*    Mac OS X OpenGL context
*
*  @remarks
*    According to http://www.opengl.org/wiki/Programming_OpenGL_on_Mac_OS_X there are several relevant API's on Mac OS X:
*    - AGL/Carbon: "AGL is the old Carbon-based API with C bindings" (see http://developer.apple.com/library/mac/#documentation/GraphicsImaging/Reference/AGL_OpenGL/Reference/reference.html)
*    - Cocoa/NSOpenGL (aka NSGL): "Cocoa is the modern API with Objective-C bindings"
*    -> Both building on top of the low level CGL (Core OpenGL) (see http://developer.apple.com/library/mac/#documentation/GraphicsImaging/Reference/CGL_OpenGL/Reference/reference.html)
*    -> To keep things maintainable, we're only supporting the modern CGL/Cocoa/NSOpenGL way.
*/
class ContextMacOSX : public Context {


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
		ContextMacOSX(Renderer &cRenderer, PLCore::uint32 nMultisampleAntialiasingSamples);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ContextMacOSX();

		/**
		*  @brief
		*    Returns the primary render context
		*
		*  @return
		*    The primary render context, a null pointer on error
		*/
		CGLContextObj GetRenderContext() const;


	//[-------------------------------------------------------]
	//[ Public virtual Context methods                        ]
	//[-------------------------------------------------------]
	public:
		virtual bool IsValid() const override;
		virtual void MakeDummyCurrent() const override;
		virtual bool QueryDisplayModes(PLCore::Array<const PLRenderer::DisplayMode*> &lstDisplayModeList) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Renderer	  *m_pRenderer;			/**< The owner renderer, always valid! */
		CGLContextObj  m_pCGLContextObj;	/**< Pointer to an opaque CGL context object, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_CONTEXTMACOSX_H__
