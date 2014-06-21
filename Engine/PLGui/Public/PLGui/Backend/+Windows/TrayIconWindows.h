/*********************************************************\
 *  File: TrayIconWindows.h                              *
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


#ifndef __PLGUI_TRAYICONWINDOWS_H__
#define __PLGUI_TRAYICONWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Backend/TrayIconImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tray icon implementation for Windows
*/
class TrayIconWindows : public TrayIconImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GuiWindows;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cTrayIcon
		*    Reference to platform independent tray icon
		*/
		PLGUI_API TrayIconWindows(TrayIcon &cTrayIcon);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TrayIconWindows();


	//[-------------------------------------------------------]
	//[ Private virtual TrayIconImpl functions                ]
	//[-------------------------------------------------------]
	private:
		PLGUI_API virtual void CreateTrayIcon() override;
		PLGUI_API virtual void DestroyTrayIcon() override;
		PLGUI_API virtual void SetVisible(bool bVisible) override;
		PLGUI_API virtual void Update() override;
		PLGUI_API virtual void ShowNotification(const PLCore::String &sTitle, const PLCore::String &sText) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Tray icon data
		Widget			*m_pWindow;		/**< Message window */
		PLCore::uint32	 m_nID;			/**< Tray icon ID */
		bool			 m_bVisible;	/**< Tray icon visible? */
		NOTIFYICONDATA	 m_sNotifyIcon;	/**< Notify icon data */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TRAYICONWINDOWS_H__
