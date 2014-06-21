/*********************************************************\
 *  File: TrayIconImpl.h                                 *
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


#ifndef __PLGUI_TRAYICONIMPL_H__
#define __PLGUI_TRAYICONIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Image;
class TrayIcon;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tray icon implementation base class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'TrayIcon' abstraction
*/
class TrayIconImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TrayIcon;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cTrayIcon
		*    Reference to platform independent tray icon
		*/
		PLGUI_API TrayIconImpl(TrayIcon &cTrayIcon);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TrayIconImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual TrayIconImpl functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Create tray icon
		*/
		virtual void CreateTrayIcon() = 0;

		/**
		*  @brief
		*    Destroy tray icon
		*/
		virtual void DestroyTrayIcon() = 0;

		/**
		*  @brief
		*    Set visibility
		*
		*  @param[in] bVisible
		*    'true' if tray icon is visible, else 'false'
		*/
		virtual void SetVisible(bool bVisible) = 0;

		/**
		*  @brief
		*    Update tray icon
		*
		*  @remarks
		*    Call this after changing e.g. icon or tooltip text
		*/
		virtual void Update() = 0;

		/**
		*  @brief
		*    Show a notification text
		*
		*  @param[in] sTitle
		*    Title
		*  @param[in] sText
		*    Text
		*/
		virtual void ShowNotification(const PLCore::String &sTitle, const PLCore::String &sText) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		TrayIcon *m_pTrayIcon;	/**< Pointer to the platform independent tray icon object */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TRAYICONIMPL_H__
