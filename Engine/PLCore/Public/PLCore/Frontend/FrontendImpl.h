/*********************************************************\
 *  File: FrontendImpl.h                                 *
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


#ifndef __PLCORE_FRONTEND_IMPL_H__
#define __PLCORE_FRONTEND_IMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Object.h"
#include "PLCore/Frontend/AbstractFrontend.h"
#include "PLCore/Core/AbstractLifecycle.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Frontend;
class FrontendContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract frontend implementation base class
*
*  @remarks
*    This base class provides the backend interface for concrete implementations.
*    Just think of the frontend implementation as a puppet master, while the puppet
*    is the "Frontend"-class. The frontend implementation is e.g. a simple native OS
*    window or a browser such as MS Internet Explorer or Mozilla Firefox.
*/
class FrontendImpl : public Object, protected AbstractLifecycle, protected AbstractFrontend {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Frontend;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, FrontendImpl, "PLCore", PLCore::Object, "Abstract frontend implementation base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API FrontendImpl();
		
		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FrontendImpl();

		/**
		*  @brief
		*    Returns the frontend instance
		*
		*  @return
		*    The frontend instance, can be a null pointer
		*/
		inline Frontend *GetFrontend() const;


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractLifecycle functions         ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual void OnCreate() override;	// Automatically called from within "Frontend::Run()"
		PLCORE_API virtual void OnRestart() override;	// Don't forget to call this within derived frontend implementations
		PLCORE_API virtual bool OnStart() override;		// Don't forget to call this within derived frontend implementations
		PLCORE_API virtual void OnResume() override;	// Don't forget to call this within derived frontend implementations
		PLCORE_API virtual void OnPause() override;		// Don't forget to call this within derived frontend implementations
		PLCORE_API virtual void OnStop() override;		// Don't forget to call this within derived frontend implementations
		PLCORE_API virtual void OnDestroy() override;	// Automatically called from within "Frontend::Run()"


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractFrontend functions          ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual void OnSize() override;
		PLCORE_API virtual void OnFullscreenMode() override;
		PLCORE_API virtual void OnDraw() override;
		PLCORE_API virtual void OnUpdate() override;
		PLCORE_API virtual void OnDrop(const Container<String> &lstFiles) override;


	//[-------------------------------------------------------]
	//[ Protected virtual FrontendImpl functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the frontend is run
		*
		*  @param[in] sExecutableFilename
		*    Absolute application executable filename
		*  @param[in] lstArguments
		*    List of arguments to the program
		*
		*  @remarks
		*    This frontend method is called just before the frontend calls it's run-method in order to
		*    enter it's main-loop. This means that this method is called between "AbstractLifecycle::OnCreate()"
		*    and "AbstractLifecycle::OnStart()". Use this method for instance to pre-process command line arguments.
		*
		*  @note
		*    - Automatically called from within "Frontend::Run()"
		*/
		PLCORE_API virtual void OnRun(const String &sExecutableFilename, const Array<String> &lstArguments);

		/**
		*  @brief
		*    Called when the frontend should run
		*
		*  @param[in] sExecutableFilename
		*    Absolute application executable filename
		*  @param[in] lstArguments
		*    List of arguments to the program
		*
		*  @return
		*    Exit code (usually 0 means no error), usually <0 when there was an error
		*    (e.g. an embedded frontend implementation is run and controlled by another application and can't be run by using this method)
		*
		*  @note
		*    - The default implementation does nothing at all
		*    - Automatically called from within "Frontend::Run()"
		*/
		PLCORE_API virtual int Run(const String &sExecutableFilename, const Array<String> &lstArguments);

		/**
		*  @brief
		*    Get native window handle
		*
		*  @return
		*    Native window handle for the frontend window, can be a null pointer
		*/
		virtual handle GetNativeWindowHandle() const = 0;

		/**
		*  @brief
		*    Redraw frontend
		*
		*  @remarks
		*    There are situations were an application may do some heavy work without letting
		*    the frontend a chance to redraw. In such situations, it may be wise
		*    to call this method from time to time to give the frontend a chance to do redraw
		*    itself.
		*
		*  @note
		*    - Whenever possible, don't use this method, do heavy work within e.g. threads
		*    - Depending on the frontend implementation, the redraw may not be immediate
		*    - Doesn't include "Ping()"
		*/
		virtual void Redraw() = 0;

		/**
		*  @brief
		*    Give the frontend a chance to process OS messages
		*
		*  @remarks
		*    There are situations were an application may do some heavy work without letting
		*    the frontend a chance to process OS messages. In such situations, it may be wise
		*    to call this method from time to time to give the frontend a chance to do some
		*    message processing.
		*
		*  @note
		*    - Whenever possible, don't use this method, do heavy work within e.g. threads
		*/
		virtual void Ping() = 0;

		/**
		*  @brief
		*    Get frontend title
		*
		*  @return
		*    Frontend title
		*
		*  @remarks
		*    When the frontend has a window, this title can be seen within the window
		*    title bar. Please note that there's no guarantee that there's a window
		*    title bar or even a window. By default, the title is set to the frontend
		*    context name ("GetContext().GetName()") which is usually sufficient. So,
		*    unless you have a good reason to explicitly set an individual frontend
		*    title, just use the default setting and don't touch the frontend.
		*/
		virtual String GetTitle() const = 0;

		/**
		*  @brief
		*    Set frontend title
		*
		*  @param[in] sTitle
		*    Frontend title
		*
		*  @see
		*    - GetTitle()
		*/
		virtual void SetTitle(const String &sTitle) = 0;

		//[-------------------------------------------------------]
		//[ Position and size                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the x position of the frontend (in screen coordinates)
		*
		*  @return
		*    X position of the frontend
		*/
		virtual int GetX() const = 0;

		/**
		*  @brief
		*    Get the y position of the frontend (in screen coordinates)
		*
		*  @return
		*    Y position of the frontend
		*/
		virtual int GetY() const = 0;

		/**
		*  @brief
		*    Get frontend width
		*
		*  @return
		*    Width of the frontend
		*/
		virtual uint32 GetWidth() const = 0;

		/**
		*  @brief
		*    Get frontend height
		*
		*  @return
		*    Height of the frontend
		*/
		virtual uint32 GetHeight() const = 0;

		/**
		*  @brief
		*    Returns frontend window position and size
		*
		*  @param[out] nX
		*    Receives the x position of the frontend window (in screen coordinates)
		*  @param[out] nY
		*    Receives the y position of the frontend window (in screen coordinates)
		*  @param[out] nWidth
		*    Receives the width of the frontend window
		*  @param[out] nHeight
		*    Receives the height of the frontend window
		*
		*  @remarks
		*    The primary argument to allow the user to request a frontend window position and size change is,
		*    that it should be possible to restore the frontend window position and size of a previous session
		*    (may be important for the usability). Do not misuse this method to frequently manipulate
		*    the frontend window appearance. Please note that, as for all other frontend methods, this is only
		*    considered to be a request. A frontend implementation may deny the request in general or
		*    just improper settings (e.g. a too small size, position outside the visible screen etc.).
		*/
		virtual void GetWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const = 0;

		/**
		*  @brief
		*    Set frontend window position and size
		*
		*  @param[in] nX
		*    X position of the frontend window (in screen coordinates)
		*  @param[in] nY
		*    Y position of the frontend window (in screen coordinates)
		*  @param[in] nWidth
		*    Width of the frontend window
		*  @param[in] nHeight
		*    Height of the frontend window
		*
		*  @see
		*    - "GetWindowPositionSize"()
		*/
		virtual void SetWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight) = 0;

		//[-------------------------------------------------------]
		//[ Fullscreen                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Gets whether it's allowed to toggle the fullscreen mode using hotkeys
		*
		*  @return
		*    'true' if it's possible to toggle the fullscreen mode using hotkeys, else 'false'
		*/
		virtual bool GetToggleFullscreenMode() const = 0;

		/**
		*  @brief
		*    Sets whether it's allowed to toggle the fullscreen mode using hotkeys
		*
		*  @param[in] bToggleFullscreenMode
		*    Is it allowed to toggle the fullscreen mode using hotkeys?
		*
		*  @note
		*    - By default, it's allowed to switch widgets into fullscreen mode using Alt-Return or AltGr-Return
		*/
		virtual void SetToggleFullscreenMode(bool bToggleFullscreenMode) = 0;

		/**
		*  @brief
		*    Gets whether it's allowed to use Alt-Tab if fullscreen mode is used
		*
		*  @return
		*    'true' if it's possible to use Alt-Tab if fullscreen mode is used, else 'false'
		*
		*  @note
		*    - Widgets only
		*/
		virtual bool GetFullscreenAltTab() const = 0;

		/**
		*  @brief
		*    Sets whether it's allowed to use Alt-Tab if fullscreen mode is used
		*
		*  @param[in] bAllowed
		*    Is it allowed to use Alt-Tab within fullscreen mode?
		*
		*  @note
		*    - By default, it's allowed to use Alt-Tab
		*
		*  @see
		*    - GetFullscreenAltTab()
		*/
		virtual void SetFullscreenAltTab(bool bAllowed) = 0;

		/**
		*  @brief
		*    Returns whether the frontend is in fullscreen mode or not
		*
		*  @return
		*    'true' if the frontend is in fullscreen mode, else 'false'
		*/
		virtual bool IsFullscreen() const = 0;

		/**
		*  @brief
		*    Sets the frontend's fullscreen mode
		*
		*  @param[in] bFullscreen
		*    'true' if the frontend should be in fullscreen mode, else 'false'
		*/
		virtual void SetFullscreen(bool bFullscreen) = 0;

		/**
		*  @brief
		*    Something related to fullscreen mode has been changed (e.g. the display resolution)
		*/
		virtual void RefreshFullscreen() = 0;

		//[-------------------------------------------------------]
		//[ Mouse                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if the mouse is currently over the frontend
		*
		*  @return
		*    'true' if mouse-over, else 'false'
		*/
		virtual bool IsMouseOver() const = 0;

		/**
		*  @brief
		*    Get current mouse cursor X position inside the frontend
		*
		*  @return
		*    Current mouse cursor X position inside the frontend, negative value if the mouse cursor isn't currently over the frontend
		*/
		virtual int GetMousePositionX() const = 0;

		/**
		*  @brief
		*    Get current mouse cursor Y position inside the frontend
		*
		*  @return
		*    Current mouse cursor Y position inside the frontend, negative value if the mouse cursor isn't currently over the frontend
		*/
		virtual int GetMousePositionY() const = 0;

		/**
		*  @brief
		*    Check if the mouse cursor is visible
		*
		*  @return
		*    'true' if the mouse cursor is visible, else 'false'
		*
		*  @note
		*    - If the mouse cursor is visible in general, it's still possible that it's
		*      invisible over some special widgets.
		*    - If the mouse cursor is invisible in general, it will NEVER be visible!
		*/
		virtual bool IsMouseVisible() const = 0;

		/**
		*  @brief
		*    Set mouse cursor visibility
		*
		*  @param[in] bVisible
		*    Shall the mouse cursor be visible?
		*
		*  @see
		*    - IsMouseVisible()
		*/
		virtual void SetMouseVisible(bool bVisible) = 0;

		/**
		*  @brief
		*    Trap mouse inside the frontend
		*
		*  @param[in] bTrap
		*    'true' if the mouse should be trapped inside the frontend, else 'false'
		*/
		virtual void SetTrapMouse(bool bTrap) = 0;


	//[-------------------------------------------------------]
	//[ Protected static functions                            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates a frontend instance
		*
		*  @param[in] cFrontendContext
		*    Frontend context to use
		*  @param[in] cFrontendImpl
		*    Frontend implementation instance
		*
		*  @return
		*    Frontend instance, null pointer on error
		*/
		static PLCORE_API Frontend *CreateFrontend(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl);

		/**
		*  @brief
		*    Correct frontend position and size settings
		*
		*  @param[in, out] nX
		*    X position of the frontend (in screen coordinates)
		*  @param[in, out] nY
		*    Y position of the frontend (in screen coordinates)
		*  @param[in, out] nWidth
		*    Width of the frontend
		*  @param[in, out] nHeight
		*    Height of the frontend
		*  @param[in]     nScreenLeft
		*    Screen left side
		*  @param[in]     nScreenTop
		*    Screen top side
		*  @param[in]     nScreenWidth
		*    Screen width
		*  @param[in]     nScreenHeight
		*    Screen height
		*  @param[in]     nMinWidth
		*    Minimum allowed frontend width
		*  @param[in]     nMinHeight
		*    Minimum allowed frontend height
		*
		*  @note
		*    - Corrects: The frontend position shouldn't be negative
		*    - Corrects: The frontend position shouldn't be outside the visible screen
		*    - Corrects: The size of the frontend shouldn't leave the visible screen
		*/
		static PLCORE_API void CorrectPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight, int nScreenLeft, int nScreenTop, int nScreenWidth, int nScreenHeight, uint32 nMinWidth = 200, uint32 nMinHeight = 200);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Frontend *m_pFrontend;	/**< Pointer to frontend, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Frontend/FrontendImpl.inl"


#endif // __PLCORE_FRONTEND_IMPL_H__
