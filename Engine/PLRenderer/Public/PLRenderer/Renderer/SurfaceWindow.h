/*********************************************************\
 *  File: SurfaceWindow.h                                *
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


#ifndef __PLRENDERER_SURFACE_WINDOW_H__
#define __PLRENDERER_SURFACE_WINDOW_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Surface.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class SurfaceWindowHandler;
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
*    A window renderer surface where we can render in
*/
class SurfaceWindow : public Surface {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SurfaceWindowHandler;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~SurfaceWindow();

		/**
		*  @brief
		*    Returns the renderer surface handler this surface is assigned with
		*
		*  @return
		*    The renderer surface handler this surface is assigned with, can be a null pointer
		*/
		inline SurfaceWindowHandler *GetHandler() const;

		/**
		*  @brief
		*    Returns the native window the surface is assigned with
		*
		*  @return
		*    The native window the surface is assigned with, can be a null pointer
		*/
		inline PLCore::handle GetNativeWindowHandle() const;

		/**
		*  @brief
		*    Returns whether the window is in fullscreen mode or not
		*
		*  @return
		*    'true' if the window is in fullscreen mode, else 'false'
		*/
		inline bool IsFullscreen() const;


	//[-------------------------------------------------------]
	//[ Public virtual SurfaceWindow functions                ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Gets the current gamma components
		*
		*  @param[out] fRed
		*    Receives the red gamma component value
		*  @param[out] fGreen
		*    Receives the green gamma component value
		*  @param[out] fBlue
		*    Receives the blue gamma component value
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (e.g. device does not support gamma correction)
		*/
		virtual bool GetGamma(float &fRed, float &fGreen, float &fBlue) const = 0;

		/**
		*  @brief
		*    Sets the current gamma components
		*
		*  @param[in] fRed
		*    Red gamma component value
		*  @param[in] fGreen
		*    Green gamma component value
		*  @param[in] fBlue
		*    Blue gamma component value
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (e.g. device does not support gamma correction)
		*/
		virtual bool SetGamma(float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHandler
		*    Renderer surface handler this surface is assigned with (MUST be valid!)
		*  @param[in] nNativeWindowHandle
		*    Native window the surface is assigned with
		*  @param[in] bFullscreen
		*    Fullscreen mode?
		*/
		PLRENDERER_API SurfaceWindow(SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, bool bFullscreen = false);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool m_bIsFullscreen;	/**< Is this window in fullscreen mode? */


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
		SurfaceWindow(const SurfaceWindow &cSource);

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
		SurfaceWindow &operator =(const SurfaceWindow &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SurfaceWindowHandler *m_pHandler;				/**< Renderer surface handler this surface is assigned with, can be a null pointer */
		PLCore::handle		  m_nNativeWindowHandle;	/**< System native window handle the surface is assigned with, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/SurfaceWindow.inl"


#endif // __PLRENDERER_SURFACE_WINDOW_H__
