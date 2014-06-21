/*********************************************************\
 *  File: WidgetLinux.h                                  *
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


#ifndef __PLGUI_WIDGETLINUX_H__
#define __PLGUI_WIDGETLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/Backend/WidgetImpl.h"
#include "PLGui/PLGuiLinuxIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GuiLinux;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux widget implementation
*/
class WidgetLinux : public WidgetImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GuiLinux;
	friend class TrayIconWidgetLinux;


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
		PLGUI_API WidgetLinux(Widget &cWidget);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~WidgetLinux();


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
		void AddWMState(::Atom sAtom1, ::Atom sAtom2 = 0);
		void RemoveWMState(::Atom sAtom1, ::Atom sAtom2 = 0);
		void UpdateWMStates();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Display		*m_pDisplay;			/**< X display */
		int			 m_nScreen;				/**< X screen */
		::Window	 m_nNativeWindowHandle;	/**< X window handle */
		bool		 m_bWrapper;			/**< 'true' if this is a wrapper for a system window */
		bool		 m_bDestroyed;			/**< 'true' if the window has already been destroyed */
		bool		 m_bVisible;			/**< 'true' if the window is visible */
		bool		 m_bEnabled;			/**< 'true' if the window is enabled */
		bool		 m_bTopmost;			/**< 'true' if the window is on top of all other windows */
		bool		 m_bShowInTaskbar;		/**< 'true' if a top level window is to be shown in the taskbar */
		int			 m_nX;					/**< X position */
		int			 m_nY;					/**< Y position */
		int			 m_nWidth;				/**< Width */
		int			 m_nHeight;				/**< Height */
		bool		 m_bFullscreen;			/**< Fullscreen mode? */
		bool		 m_bMaximized;			/**< Maximized? */
		bool		 m_bMinimized;			/**< Minimized? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WIDGETLINUX_H__
