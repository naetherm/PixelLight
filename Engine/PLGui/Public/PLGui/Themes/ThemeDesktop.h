/*********************************************************\
 *  File: ThemeDesktop.h                                 *
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


#ifndef __PLGUI_THEME_DESKTOP_H__
#define __PLGUI_THEME_DESKTOP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Themes/Theme.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight desktop theme
*/
class ThemeDesktop : public Theme {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Widget;


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, ThemeDesktop, "PLGui", PLGui::Theme, "PixelLight desktop theme")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		PLGUI_API ThemeDesktop(Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ThemeDesktop();


	//[-------------------------------------------------------]
	//[ Public virtual Theme functions                        ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API virtual void DrawRect(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, ELineStyle nStyle, const PLGraphics::Color4 &cColor, int nWidth, int nRound = 0) override;
		PLGUI_API virtual void DrawBorder(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EBorderStyle nBorderStyle) override;
		PLGUI_API virtual void DrawFocusRect(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2) override;
		PLGUI_API virtual void DrawSeparator(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation) override;
		PLGUI_API virtual void DrawRule(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation) override;

		PLGUI_API virtual void DrawPanel(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EBorderStyle nBorderStyle) override;
		PLGUI_API virtual void DrawWindow(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, bool bBorder, PLCore::uint32 nWidgetState, const PLCore::String &sTitle, const PLGui::Image &cIcon) override;
		PLGUI_API virtual void DrawSystemButton(Graphics &cGraphics, const PLMath::Vector2i &vPos, ESystemCommand nSystemCommand, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawButton(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, const PLCore::String &sTitle, const Image &cImage, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawToggleButton(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, const PLCore::String &sTitle, const Image &cImage, PLCore::uint32 nWidgetState, ECheckState nChecked) override;
		PLGUI_API virtual void DrawRadioButton(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, const PLCore::String &sTitle, PLCore::uint32 nWidgetState, ECheckState nChecked) override;
		PLGUI_API virtual void DrawCheckBox(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, const PLCore::String &sTitle, PLCore::uint32 nWidgetState, ECheckState nChecked) override;
		PLGUI_API virtual void DrawTooltip(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawSlider(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawSliderHandle(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawScrollBar(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawScrollBarPlusButton(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawScrollBarMinusButton(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawScrollBarHandle(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawSplitter(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation) override;
		PLGUI_API virtual void DrawMenuBar(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawPopupMenu(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;
		PLGUI_API virtual void DrawMenuItem(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, const PLCore::String &sTitle, const Image &cImage, bool bDrawArrow, EMenuItemType nType, EOrientation nOrientation, PLCore::uint32 nWidgetState) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API void DrawControlBorder(Graphics &cGraphics, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, const PLGraphics::Color4 &cColorBorder, const PLGraphics::Color4 &cColorHighlight);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Options
		int					m_nControlRound;					/**< Rounded edges for controls */

		// Colors
		PLGraphics::Color4	m_cColorPanelRaised;				/**< Raised border color */
		PLGraphics::Color4	m_cColorPanelSunken;				/**< Sunken border color */
		PLGraphics::Color4	m_cColorBorder;						/**< Border color */
		PLGraphics::Color4	m_cColorTitleBar;					/**< Title bar color (active) */
		PLGraphics::Color4	m_cColorTitleBarInactive;			/**< Title bar color (inactive) */
		PLGraphics::Color4	m_cColorControlBack;				/**< Controls: Background color */
		PLGraphics::Color4	m_cColorControlBackDisabled;		/**< Controls: Background color (disabled) */
		PLGraphics::Color4	m_cColorControlBackSelect;			/**< Controls: Background color (mouse-over) */
		PLGraphics::Color4	m_cColorControlBackPressed;			/**< Controls: Background color (pressed) */
		PLGraphics::Color4	m_cColorControlBorder;				/**< Controls: Border color */
		PLGraphics::Color4	m_cColorControlBorderSelect;		/**< Controls: Border color (mouse-over) */
		PLGraphics::Color4	m_cColorControlHighlightSelect;		/**< Controls: Highlight color (mouse-over) */

		// Images
		Image				m_cImageArrowLeft;					/**< Arrow left */
		Image				m_cImageArrowRight;					/**< Arrow right */
		Image				m_cImageArrowUp;					/**< Arrow up */
		Image				m_cImageArrowDown;					/**< Arrow down */
		Image				m_cImageRadioButton;				/**< Radio button */
		Image				m_cImageRadioButtonChecked;			/**< Radio button (checked) */
		Image				m_cImageCheckBox;					/**< Check box */
		Image				m_cImageCheckBoxChecked;			/**< Check box (checked) */
		Image				m_cImageCheckBoxPartially;			/**< Check box (partially checked) */
		Image				m_cImageMinimize;					/**< System button 'minimize' */
		Image				m_cImageMaximize;					/**< System button 'maximize' */
		Image				m_cImageRestore;					/**< System button 'restore' */
		Image				m_cImageClose;						/**< System button 'close' */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_THEME_DESKTOP_H__
