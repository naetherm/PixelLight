/*********************************************************\
 *  File: GuiWindows.h                                   *
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


#ifndef __PLGUI_GUIWINDOWS_H__
#define __PLGUI_GUIWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include <PLCore/Container/List.h>
#include <PLCore/Container/HashMap.h>
#include "PLGui/Backend/GuiImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Screen;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows GUI implementation
*/
class GuiWindows : public GuiImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pGui
		*    Platform independent GUI
		*/
		PLGUI_API GuiWindows(Gui *pGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GuiWindows();


	//[-------------------------------------------------------]
	//[ Public virtual GuiImpl functions                      ]
	//[-------------------------------------------------------]
	public:
		virtual bool HasPendingMessages() override;
		virtual void ProcessMessage() override;
		virtual void PostMessage(const GuiMessage &cMessage) override;
		virtual void EnumerateScreens(PLCore::List<Screen*> &lstScreens) override;
		virtual PLMath::Vector2i GetScreenSize() const override;
		virtual bool HasTaskbar() override;
		virtual void SetMouseVisible(bool bVisible) override;
		virtual void ListFonts(PLCore::List<FontInfo> &lstFonts) const override;
		virtual WidgetImpl *CreateWidgetImpl(Widget &cWidget) const override;
		virtual GraphicsImpl *CreateGraphicsImpl(Graphics &cGraphics) const override;
		virtual ImageImpl *CreateImageImpl(Image &cImage) const override;
		virtual FontImpl *CreateFontImpl(Font &cFont) const override;
		virtual CursorImpl *CreateCursorImpl(Cursor &cCursor) const override;
		virtual TrayIconImpl *CreateTrayIconImpl(TrayIcon &cTrayIcon) const override;
		virtual ClipBoardImpl *CreateClipBoardImpl(ClipBoard &cClipBoard) const override;


	//[-------------------------------------------------------]
	//[ Private static functions                              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Static window callback function
		*/
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

		/**
		*  @brief
		*    Static monitor enumeration callback function
		*/
		static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

		/**
		*  @brief
		*    Static font enumeration callback function (enumerating font names only)
		*/
		static int CALLBACK FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam);

		/**
		*  @brief
		*    Static font enumeration callback function (enumerating all weights and styles of a font)
		*/
		static int CALLBACK FontEnumProc2(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam);

		/**
		*  @brief
		*    Windows timer callback
		*/
		static void CALLBACK TimerFunction(HWND hWnd, UINT nMsg, UINT_PTR nTimerID, DWORD nTime);

		/**
		*  @brief
		*    Process a tray icon message
		*
		*  @param[in] nID
		*    Tray icon ID
		*  @param[in] nMessage
		*    Notify message
		*/
		static void ProcessTrayIconMessage(TrayIcon *pTrayIcon, PLCore::uint32 nID, PLCore::uint32 nMessage);

		/**
		*  @brief
		*    Get PLGui widget that belongs to a certain window handle
		*
		*  @param[in] hWnd
		*    Window handle
		*
		*  @return
		*    Widget, can be a null pointer if hWnd is NULL_HANDLE or the window is not a PLGui widget
		*/
		static Widget *GetPLGuiWidget(HWND hWnd);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// System handles
		HINSTANCE				m_hInstance;	/**< Main process handle */
		DWORD					m_nThreadID;	/**< Main thread ID */
		WNDCLASS				m_WndClass;		/**< Window class */

		// Screens
		PLCore::List<Screen*>	m_lstScreens;	/**< List of available screens (monitors) */

		// Widgets
		Widget				   *m_pMouseOver;	/**< Widget that the mouse has currently entered */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUIWINDOWS_H__
