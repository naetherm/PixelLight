/*********************************************************\
 *  File: SurfaceWindowHandler.h                         *
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


#ifndef __PLRENDERER_SURFACE_WINDOW_HANDLER_H__
#define __PLRENDERER_SURFACE_WINDOW_HANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	struct DisplayMode;
	class Surface;
	class Renderer;
	class SurfaceHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A renderer surface window handler will be informed automatically
*    if it's renderer surface is destroyed
*/
class SurfaceWindowHandler {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SurfaceWindow;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API SurfaceWindowHandler();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~SurfaceWindowHandler();

		/**
		*  @brief
		*    Returns the owner renderer
		*
		*  @return
		*    The owner renderer, can be a null pointer
		*/
		inline Renderer *GetRenderer() const;

		/**
		*  @brief
		*    Returns the renderer surface of this handler
		*
		*  @return
		*    The renderer surface of this handler, can be a null pointer
		*/
		inline Surface *GetSurface() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Initializes the surface window handler
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] nNativeWindowHandle
		*    Native window of renderer surface handler is assigned with
		*  @param[in] sDisplayMode
		*    Display mode information
		*  @param[in] bFullscreen
		*    Fullscreen mode?
		*/
		PLRENDERER_API void Init(Renderer &cRenderer, PLCore::handle nNativeWindowHandle, const DisplayMode &sDisplayMode, bool bFullscreen = false);

		/**
		*  @brief
		*    De-initializes the surface window handler
		*/
		PLRENDERER_API void DeInit();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Renderer	   *m_pRenderer;		/**< Owner renderer (can be a null pointer) */
		SurfaceHandler *m_pSurfaceHandler;	/**< The renderer surface handler (NEVER a null pointer!) */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		SurfaceWindowHandler(const SurfaceWindowHandler &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		SurfaceWindowHandler &operator =(const SurfaceWindowHandler &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/SurfaceWindowHandler.inl"


#endif // __PLRENDERER_SURFACE_WINDOW_HANDLER_H__
