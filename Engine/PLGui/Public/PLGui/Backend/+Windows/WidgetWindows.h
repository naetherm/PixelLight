/*********************************************************\
 *  File: WidgetWindows.h                                *
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


#ifndef __PLGUI_WIDGETWINDOWS_H__
#define __PLGUI_WIDGETWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include "PLGui/Backend/WidgetImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiWindows;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows widget implementation
*/
class WidgetWindows : public WidgetImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GuiWindows;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWidget
		*    Reference to platform independent widget
		*/
		PLGUI_API WidgetWindows(Widget &cWidget);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~WidgetWindows();


	//[-------------------------------------------------------]
	//[ Public virtual WidgetImpl functions                   ]
	//[-------------------------------------------------------]
	public:
		virtual void CreateWidget() override;
		virtual void CreateWrapperWidget(PLCore::handle nNativeWindowHandle) override;
		virtual bool IsDestroyed() const override;
		virtual void Destroy() override;
		virtual PLCore::handle GetNativeWindowHandle() const override;
		virtual void SetParent(WidgetImpl *pParent) override;
		virtual bool IsShowInTaskbar() const override;
		virtual void SetShowInTaskbar(bool bShowInTaskbar) override;
		virtual void MoveToScreen(Screen &cScreen) override;
		virtual PLMath::Vector2i GetPos() const override;
		virtual void SetPos(const PLMath::Vector2i &vPos) override;
		virtual PLMath::Vector2i GetSize() const override;
		virtual void SetSize(const PLMath::Vector2i &vSize) override;
		virtual void SetZPos(EZPos nZPos, Widget *pWidget) override;
		virtual bool IsTopmost() const override;
		virtual void SetTopmost(bool bTopmost) override;
		virtual EWindowState GetWindowState() const override;
		virtual void SetWindowState(EWindowState nWindowState) override;
		virtual void SetTransparency(ETransparency nTransparency, const PLGraphics::Color4 &cColor) override;
		virtual bool IsVisible() const override;
		virtual void SetVisible(bool bVisible) override;
		virtual bool IsEnabled() const override;
		virtual void SetEnabled(bool bEnabled) override;
		virtual void Activate() override;
		virtual void SetFocus() override;
		virtual void Redraw() override;
		virtual void SetCaptureMouse(bool bCapture) override;
		virtual void SetTrapMouse(bool bTrap) override;
		virtual void SetTitle(const PLCore::String &sTitle) override;
		virtual void SetIcon(const Image &cIcon) override;
		virtual PLCore::uint32 RegisterHotkey(PLCore::uint32 nModKey, PLCore::uint32 nKey) override;
		virtual void UnregisterHotkey(PLCore::uint32 nID) override;
		virtual void SetCursor(const Cursor &cCursor) override;
		virtual bool GetMousePos(PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Get window style according to the current state and options of the window
		*
		*  @param[out] nWinStyle
		*    Window style
		*  @param[out] nExtStyle
		*    Extended window style
		*/
		void GetWindowStyle(PLCore::uint32 &nWinStyle, PLCore::uint32 &nExtStyle);

		/**
		*  @brief
		*    Update window style of the widget
		*
		*  @param[in] bExtOnly
		*    Update only extended window style?
		*/
		void UpdateWindowStyle(bool bExtOnly = false);

		/**
		*  @brief
		*    Update trap mouse if required
		*
		*  @note
		*    - Should be called after widget position or size was changed
		*/
		void UpdateTrapMouse();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HWND			m_hWnd;				/**< Widgets widget handle */
		bool			m_bWrapper;			/**< 'true' if this is a wrapper for a system window */
		bool			m_bDestroyed;		/**< 'true' if the widget has already been destroyed */
		EWindowState	m_nWindowState;		/**< Current window state */
		bool			m_bFullscreen;		/**< Fullscreen mode */
		bool			m_bShowInTaskbar;	/**< Show window in taskbar? */
		ETransparency	m_nTransparency;	/**< Widget transparency type */
		bool			m_bIgnoreSetFocus;	/**< Ignore next focus message? */
		bool			m_bTrapMouse;		/**< Trap mouse? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WIDGETWINDOWS_H__
