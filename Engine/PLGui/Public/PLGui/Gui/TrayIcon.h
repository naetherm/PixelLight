/*********************************************************\
 *  File: TrayIcon.h                                     *
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


#ifndef __PLGUI_TRAYICON_H__
#define __PLGUI_TRAYICON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include "PLGui/Gui/Resources/Image.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
namespace PLGui {
	class Gui;
	class TrayIconImpl;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tray icon class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class TrayIcon {


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<const PLMath::Vector2i&>					EventMouseMove;					/**< Mouse moves inside the tray icon, mouse position within the widget as parameter */
		PLCore::Event<PLCore::uint32, const PLMath::Vector2i&>	EventMouseButtonDown;			/**< Mouse button is pressed, mouse button and mouse position within the widget as parameters */
		PLCore::Event<PLCore::uint32, const PLMath::Vector2i&>	EventMouseButtonUp;				/**< Mouse button is released, mouse button and mouse position within the widget as parameters */
		PLCore::Event<PLCore::uint32, const PLMath::Vector2i&>	EventMouseButtonClick;			/**< Mouse button has been clicked, mouse button and mouse position within the widget as parameters */
		PLCore::Event<PLCore::uint32, const PLMath::Vector2i&>	EventMouseButtonDoubleClick;	/**< Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters */


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
		PLGUI_API TrayIcon(Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TrayIcon();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform specific implementation
		*/
		PLGUI_API TrayIconImpl *GetImpl() const;

		/**
		*  @brief
		*    Get visibility
		*
		*  @return
		*    'true' if tray icon is visible, else 'false'
		*/
		PLGUI_API bool IsVisible() const;

		/**
		*  @brief
		*    Set visibility
		*
		*  @param[in] bVisible
		*    'true' if tray icon is visible, else 'false'
		*/
		PLGUI_API void SetVisible(bool bVisible);

		/**
		*  @brief
		*    Get icon
		*
		*  @return
		*    Icon that is displayed in the tray
		*/
		PLGUI_API const Image &GetIcon() const;

		/**
		*  @brief
		*    Set icon
		*
		*  @param[in] cIcon
		*    Icon that is displayed in the tray
		*/
		PLGUI_API void SetIcon(const Image &cIcon);

		/**
		*  @brief
		*    Get tooltip
		*
		*  @return
		*    Text that is displayed as a tooltip
		*/
		PLGUI_API PLCore::String GetTooltip() const;

		/**
		*  @brief
		*    Set tooltip
		*
		*  @param[in] sTooltip
		*    Text that is displayed as a tooltip
		*/
		PLGUI_API void SetTooltip(const PLCore::String &sTooltip);

		/**
		*  @brief
		*    Show a notification text
		*
		*  @param[in] sTitle
		*    Title
		*  @param[in] sText
		*    Text
		*/
		PLGUI_API void ShowNotification(const PLCore::String &sTitle, const PLCore::String &sText);


	//[-------------------------------------------------------]
	//[ Public virtual TrayIcon functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when the mouse is moved within the tray icon
		*
		*  @param[in] vPos
		*    Mouse position within the tray icon
		*/
		PLGUI_API virtual void OnMouseMove(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is pressed
		*
		*  @param[in] nButton
		*    Mouse button that is pressed
		*  @param[in] vPos
		*    Mouse position within the tray icon
		*/
		PLGUI_API virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is released
		*
		*  @param[in] nButton
		*    Mouse button that is released
		*  @param[in] vPos
		*    Mouse position within the tray icon
		*/
		PLGUI_API virtual void OnMouseButtonUp(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is clicked
		*
		*  @param[in] nButton
		*    Mouse button that is clicked
		*  @param[in] vPos
		*    Mouse position within the tray icon
		*/
		PLGUI_API virtual void OnMouseButtonClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is double-clicked
		*
		*  @param[in] nButton
		*    Mouse button that is double-clicked
		*  @param[in] vPos
		*    Mouse position within the tray icon
		*/
		PLGUI_API virtual void OnMouseButtonDoubleClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui				*m_pGui;		/**< Pointer to GUI instance */
		TrayIconImpl	*m_pImpl;		/**< Tray icon implementation */
		bool			 m_bVisible;	/**< Is the tray icon visible? */
		Image			 m_cIcon;		/**< Displayed icon */
		PLCore::String	 m_sTooltip;	/**< Displayed tooltip */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TRAYICON_H__
