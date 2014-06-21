/*********************************************************\
 *  File: OSWindowWindows.h                              *
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


#ifndef __PLFRONTENDOS_OSWINDOW_WINDOWS_H__
#define __PLFRONTENDOS_OSWINDOW_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include "PLFrontendOS/OSWindow.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendOS {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Frontend;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OS Window frontend implementation class using standard Win32-functions
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'System' abstraction
*/
class OSWindowWindows : public OSWindow {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Frontend;


	//[-------------------------------------------------------]
	//[ Private static functions                              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Static window callback function
		*/
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontendOS
		*    Owner frontend implementation instance
		*/
		OSWindowWindows(Frontend &cFrontendOS);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OSWindowWindows();

		/**
		*  @brief
		*    Register a hotkey
		*
		*  @param[in] nModKey
		*    Modification key (for example VK_MENU (alt-key))
		*  @param[in] nKey
		*    Key (for example VK_RETURN)
		*
		*  @return
		*    Hotkey ID, 0 on error
		*/
		int RegisterHotkey(UINT nModKey, UINT nKey);

		/**
		*  @brief
		*    Unregister a hotkey
		*
		*  @param[in] nID
		*    Hotkey ID
		*/
		void UnregisterHotkey(int nID);

		/**
		*  @brief
		*    Update trap mouse if required
		*
		*  @note
		*    - Should be called after window position or size was changed
		*/
		void UpdateTrapMouse();

		/**
		*  @brief
		*    If the window is not visible yet, make it visible right now
		*/
		void MakeVisible();


	//[-------------------------------------------------------]
	//[ Private virtual OSWindow functions                    ]
	//[-------------------------------------------------------]
	private:
		virtual PLCore::handle GetNativeWindowHandle() const override;
		virtual void Redraw() override;
		virtual bool Ping() override;
		virtual PLCore::String GetTitle() const override;
		virtual void SetTitle(const PLCore::String &sTitle) override;
		virtual int GetX() const override;
		virtual int GetY() const override;
		virtual PLCore::uint32 GetWidth() const override;
		virtual PLCore::uint32 GetHeight() const override;
		virtual void SetWindowPositionSize(int nX, int nY, PLCore::uint32 nWidth, PLCore::uint32 nHeight) override;
		virtual void SetFullscreenAltTab(bool bAllowed) override;
		virtual void SetFullscreen(bool bFullscreen) override;
		virtual void RefreshFullscreen() override;
		virtual bool IsMouseOver() const override;
		virtual int GetMousePositionX() const override;
		virtual int GetMousePositionY() const override;
		virtual bool IsMouseVisible() const override;
		virtual void SetMouseVisible(bool bVisible) override;
		virtual void SetTrapMouse(bool bTrap) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Frontend   *m_pFrontendOS;				/**< Owner frontend implementation instance, always valid! */
		HINSTANCE	m_hInstance;				/**< OS main process handle */
		DWORD		m_nThreadID;				/**< OS main thread ID */
		WNDCLASS	m_WndClass;					/**< OS window class */
		HICON		m_hIcon;					/**< Extracted default icon of the process (don't forget to call "DestroyIcon()" on it), can be a null pointer */
		HWND		m_hWnd;						/**< OS window handle, can be a null pointer */
		bool		m_bInitialized;				/**< Initialization done? */
		bool		m_bBackgroundInitialized;	/**< Background initialization done? (if false, WM_ERASEBKGND won't be caught) */
		bool		m_bVisible;					/**< Was the window made visible? (independent of the real OS window visibility state) */
		bool		m_bDestroyed;				/**< 'true' if the window has already been destroyed */
		int			m_nHotkeyIDAltTab;			/**< Alt-Tab hotkey */
		bool		m_bWindowRectBackup;		/**< Is there a window position & size backup? */
		RECT		m_sWindowRectBackup;		/**< Window position & size backup */
		bool		m_bMouseVisible;			/**< Is the mouse cursor visible? */
		bool		m_bTrapMouse;				/**< Trap mouse? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendOS


#endif // __PLFRONTENDOS_OSWINDOW_WINDOWS_H__
