/*********************************************************\
 *  File: RawInputDevice.h                               *
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


#ifndef __PLINPUT_RAWINPUTDEVICE_H__
#define __PLINPUT_RAWINPUTDEVICE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLInput/Backend/UpdateDevice.h"
#include "PLInput/+Windows/PLInputWindowsIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Keyboard;
class Button;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Information about a Raw-Input Device
*/
class RawInputDevice : public UpdateDevice {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RawInput;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLINPUT_API RawInputDevice();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLINPUT_API RawInputDevice(const RawInputDevice &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~RawInputDevice();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This instance
		*/
		PLINPUT_API RawInputDevice &operator =(const RawInputDevice &cSource);

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cSource
		*    RawInputDevice to compare with
		*
		*  @return
		*    'true', if both are equal, else 'false'
		*/
		PLINPUT_API bool operator ==(const RawInputDevice &cSource);

		/**
		*  @brief
		*    Get device name
		*
		*  @return
		*    Device name
		*/
		PLINPUT_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Get device type
		*
		*  @return
		*    Device type
		*/
		PLINPUT_API DWORD GetType() const;

		/**
		*  @brief
		*    Get device handle
		*
		*  @return
		*    Device handle
		*/
		PLINPUT_API HANDLE GetHandle() const;

		/**
		*  @brief
		*    Get device info
		*
		*  @return
		*    Device info
		*/
		PLINPUT_API RID_DEVICE_INFO GetDeviceInfo() const;

		/**
		*  @brief
		*    Check if the device is a virtual device
		*
		*  @return
		*    'true', if it is a virtual device, else 'false'
		*/
		PLINPUT_API bool IsVirtual() const;


	//[-------------------------------------------------------]
	//[ Public virtual UpdateDevice functions                 ]
	//[-------------------------------------------------------]
	public:
		PLINPUT_API virtual void Update() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Process raw keyboard data
		*
		*  @param[in] nMakeCode
		*    Scan code
		*  @param[in] nFlags
		*    Miscellaneous scan code information
		*  @param[in] nReserved
		*    Reserved
		*  @param[in] nKey
		*    Key information
		*  @param[in] nMessage
		*    Message information
		*  @param[in] nExtra
		*    Additional device specific event information
		*/
		PLINPUT_API void ProcessKeyboardData(unsigned short nMakeCode, unsigned short nFlags, unsigned short nReserved,
											 unsigned short nKey, unsigned int nMessage, unsigned long nExtra);

		/**
		*  @brief
		*    Process raw mouse data
		*
		*  @param[in] nFlags
		*    Flags
		*  @param[in] nButtons
		*    Buttons
		*  @param[in] nButtonFlags
		*    Button flags
		*  @param[in] nButtonData
		*    Button data
		*  @param[in] nRawButtons
		*    Raw buttons
		*  @param[in] nLastX
		*    Movement in the X direction (relative or absolute)
		*  @param[in] nLastY
		*    Movement in the Y direction (relative or absolute)
		*  @param[in] nExtra
		*    Additional device specific event information
		*/
		PLINPUT_API void ProcessMouseData(unsigned short nFlags, unsigned long nButtons, unsigned short nButtonFlags,
										  unsigned short nButtonData, long nRawButtons, long nLastX, long nLastY,
										  unsigned long nExtra);

		/**
		*  @brief
		*    Get key for virtual key code
		*
		*  @param[in] pKeyboard
		*    Pointer to keyboard device (must be valid!)
		*  @param[in] nKey
		*    Virtual key code
		*
		*  @return
		*    Corresponding key, a null pointer if key code is invalid
		*/
		PLINPUT_API Button *GetKeyboardKey(Keyboard *pKeyboard, unsigned short nKey);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Device data
		PLCore::String	m_sName;		/**< Device name */
		DWORD			m_nType;		/**< Device type (keyboard, mouse or HID) */
		HANDLE			m_hDevice;		/**< Device handle */
		RID_DEVICE_INFO	m_sDeviceInfo;	/**< Device information struct */
		bool			m_bVirtual;		/**< Virtual device flag */

		// Mouse data
		long			m_nOldX;		/**< X position (if mouse only supports absolute positions) */
		long			m_nOldY;		/**< Y position (if mouse only supports absolute positions) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_RAWINPUTDEVICE_H__
