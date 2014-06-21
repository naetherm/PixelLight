/*********************************************************\
 *  File: Context.h                                      *
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


#ifndef __PLRENDEREROPENGL_CONTEXT_H__
#define __PLRENDEREROPENGL_CONTEXT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/AbstractContext.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	template <class ValueType> class Array;
}
namespace PLRenderer {
	struct DisplayMode;
	class SurfaceWindow;
	class SurfaceWindowHandler;
}
namespace PLRendererOpenGL {
	class Extensions;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract OpenGL context base class
*/
class Context : public PLCore::AbstractContext {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the available extensions
		*
		*  @return
		*    The available extensions
		*/
		const Extensions &GetExtensions() const;

		/**
		*  @brief
		*    Returns the available extensions
		*
		*  @return
		*    The available extensions
		*/
		Extensions &GetExtensions();


	//[-------------------------------------------------------]
	//[ Public virtual Context methods                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Context();

		/**
		*  @brief
		*    Returns whether or not the context is valid
		*
		*  @return
		*    'true' if the context is valid, else 'false'
		*/
		virtual bool IsValid() const = 0;

		/**
		*  @brief
		*    Returns makes the internal dummy to the current render target
		*/
		virtual void MakeDummyCurrent() const = 0;

		/**
		*  @brief
		*    Enumerates all available display modes
		*
		*  @param[out] lstDisplayModeList
		*    Receives all found display modes, list is not cleared before new items are added
		*
		*  @return
		*    'true' if all went fine and at least one display mode was found, else 'false'
		*/
		virtual bool QueryDisplayModes(PLCore::Array<const PLRenderer::DisplayMode*> &lstDisplayModeList) = 0;

		/**
		*  @brief
		*    Creates renderer surface window
		*
		*  @param[in] cHandler
		*    Renderer surface handler the new renderer surface is assigned with
		*  @param[in] nNativeWindowHandle
		*    Handle to the native window the renderer surface is assigned with
		*  @param[in] sDisplayMode
		*    Display mode information
		*  @param[in] bFullscreen
		*    Fullscreen mode?
		*
		*  @return
		*    The created renderer surface, a null pointer on error
		*
		*  @note
		*    - The new created renderer surface is added to this renderer automatically
		*    - The renderer surface handler is required because this kind of surface is assigned
		*      with a window and if the surface or window is lost they have to be informed
		*/
		virtual PLRenderer::SurfaceWindow *CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen = false) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*/
		Context();


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
		Context(const Context &cSource);

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
		Context &operator =(const Context &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		Extensions *m_pExtensions;	/**< Extensions instance, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_CONTEXT_H__
