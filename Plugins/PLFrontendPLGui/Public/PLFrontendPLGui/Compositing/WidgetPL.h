/*********************************************************\
 *  File: WidgetPL.h                                     *
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


#ifndef __PLFRONTENDPLGUI_COMPOSITING_GUI_WIDGET_H__
#define __PLFRONTENDPLGUI_COMPOSITING_GUI_WIDGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/QuickSort.h>
#include <PLGraphics/Color/Color4.h>
#include <PLGui/Backend/WidgetImpl.h>
#include "PLFrontendPLGui/PLFrontendPLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class GuiPL;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight ingame GUI widget implementation
*/
class WidgetPL : public PLGui::WidgetImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GuiPL;


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
		PLFRONTENDPLGUI_API WidgetPL(PLGui::Widget &cWidget);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~WidgetPL();


	//[-------------------------------------------------------]
	//[ Public virtual PLGui::WidgetImpl functions            ]
	//[-------------------------------------------------------]
	public:
		virtual void CreateWidget() override;
		virtual void CreateWrapperWidget(PLCore::handle nNativeWindowHandle) override;
		virtual bool IsDestroyed() const override;
		virtual void Destroy() override;
		virtual PLCore::handle GetNativeWindowHandle() const override;
		virtual void SetParent(PLGui::WidgetImpl *pParent) override;
		virtual bool IsShowInTaskbar() const override;
		virtual void SetShowInTaskbar(bool bShowInTaskbar) override;
		virtual void MoveToScreen(PLGui::Screen &cScreen) override;
		virtual PLMath::Vector2i GetPos() const override;
		virtual void SetPos(const PLMath::Vector2i &vPos) override;
		virtual PLMath::Vector2i GetSize() const override;
		virtual void SetSize(const PLMath::Vector2i &vSize) override;
		virtual void SetZPos(PLGui::EZPos nZPos, PLGui::Widget *pWidget) override;
		virtual bool IsTopmost() const override;
		virtual void SetTopmost(bool bTopmost) override;
		virtual PLGui::EWindowState GetWindowState() const override;
		virtual void SetWindowState(PLGui::EWindowState nWindowState) override;
		virtual void SetTransparency(PLGui::ETransparency nTransparency, const PLGraphics::Color4 &cColor) override;
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
		virtual void SetIcon(const PLGui::Image &cIcon) override;
		virtual PLCore::uint32 RegisterHotkey(PLCore::uint32 nModKey, PLCore::uint32 nKey) override;
		virtual void UnregisterHotkey(PLCore::uint32 nID) override;
		virtual void SetCursor(const PLGui::Cursor &cCursor) override;
		virtual bool GetMousePos(PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set Z position
		*
		*  @param[in, out] lstChildren
		*    List of z position sorted child widgets, smaller number for widgets below other with higher numbers
		*  @param[in]      nZPos
		*    Z position (see EZPos)
		*  @param[in]      pWidget
		*    Other widget used to align relative to (only matters for 'ZAbove' or 'ZBehind')
		*/
		void SetZPos(PLCore::Array<PLGui::Widget*> &lstChildren, PLGui::EZPos nZPos, PLGui::Widget *pWidget = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Widget				   *m_pWidget;				/**< Reference to platform independent widget, always valid! */
		bool							m_bWrapper;				/**< 'true' if this is a wrapper for a system window */
		bool							m_bDestroyed;			/**< 'true' if the widget has already been destroyed */
		PLGui::EWindowState				m_nWindowState;			/**< Current window state */
		PLMath::Vector2i				m_vPos;					/**< Widget position */
		PLMath::Vector2i				m_vSize;				/**< Widget size */
		bool							m_bTopmost;				/**< Is this widget topmost */
		bool							m_bFullscreen;			/**< Fullscreen mode */
		PLGui::ETransparency			m_nTransparency;		/**< Widget transparency type */
		PLGraphics::Color4				m_cTransparencyColor;	/**< Widget color */
		bool							m_bVisible;				/**< Is the widget visible? */
		bool							m_bEnabled;				/**< Is the widget enabled? */
		bool							m_bCaptureMouse;		/**< Capture mouse? */
		PLCore::Array<PLGui::Widget*>	m_lstTopmostChildren;	/**< List of z position sorted topmost child widgets, smaller number for widgets below other with higher numbers */
		PLCore::Array<PLGui::Widget*>	m_lstChildren;			/**< List of z position sorted child widgets (including topmost), smaller number for widgets below other with higher numbers */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_COMPOSITING_GUI_WIDGET_H__
