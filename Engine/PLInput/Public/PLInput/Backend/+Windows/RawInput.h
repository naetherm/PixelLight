/*********************************************************\
 *  File: RawInput2.h                                    *
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


#ifndef __PLINPUT_RAWINPUT_H__
#define __PLINPUT_RAWINPUT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/System/Thread.h>
#include <PLCore/Core/Singleton.h>
#include <PLCore/Container/List.h>
#include <PLCore/Container/HashMap.h>
#include "PLInput/+Windows/PLInputWindowsIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RawInput;
class RawInputDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Wrapper for the Raw-Input API on Windows
*/
class RawInput : public PLCore::Singleton<RawInput> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class PLCore::Singleton<RawInput>;


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		PLINPUT_API static RawInput *GetInstance();
		PLINPUT_API static bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Remove all devices
		*/
		PLINPUT_API void Clear();

		/**
		*  @brief
		*    Detect available Raw-Input devices
		*/
		PLINPUT_API void DetectDevices();

		/**
		*  @brief
		*    Get list of available input devices
		*
		*  @return
		*    Device list
		*
		*  @remarks
		*    There are two virtual devices:
		*    The virtual keyboard device named "Keyboard" catches all events from all keyboard devices.
		*    The virtual mouse device named "Mouse" catches all events from all mouse devices.
		*    All other devices represent real devices and have names given from Windows.
		*/
		PLINPUT_API const PLCore::List<RawInputDevice*> &GetDevices() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		RawInput();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RawInput();

		/**
		*  @brief
		*    Raw-Input message processing
		*
		*  @param[in] nMsg
		*    Received message
		*  @param[in] wParam
		*    First parameter of the received message
		*  @param[in] lParam
		*    Second parameter of the received message
		*
		*  @return
		*    Error code, 0 = success
		*/
		LRESULT ProcessRawInput(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


	//[-------------------------------------------------------]
	//[ Private static functions                              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Window callback function
		*
		*  @param[in] hWnd
		*    Window the message came from
		*  @param[in] nMsg
		*    Received message
		*  @param[in] wParam
		*    First parameter of the received message
		*  @param[in] lParam
		*    Second parameter of the received message
		*
		*  @return
		*    Error code, 0 = success
		*/
		static LRESULT WINAPI WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

		/**
		*  @brief
		*    Thread for reading raw-input messages constantly
		*
		*  @param[in] pData
		*    User parameter, containing a pointer to the RawInput-Object
		*
		*  @return
		*    Error code, 0 = success
		*/
		static int RawInputThread(void *pData);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Raw-Input system
		WNDCLASS											m_cWndClass;		/**< Window class */
		HWND												m_hWnd;				/**< Window that will receive input messages */
		PLCore::Thread										m_cThread;			/**< Raw-Input thread */
		bool												m_bThreadFinished;	/**< Finished-flag for thread */

		// Raw-Input devices
		PLCore::List<RawInputDevice*>						m_lstDevices;		/**< List of input devices */
		PLCore::HashMap<PLCore::uint32, RawInputDevice*>	m_mapDevices;		/**< Hash map: HANDLE -> Raw-Input device */
		RawInputDevice									   *m_pDeviceKeyboard;	/**< Virtual keyboard device (all events from all keyboards) */
		RawInputDevice									   *m_pDeviceMouse;		/**< Virtual mouse device (all events from all mice) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_RAWINPUT_H__
