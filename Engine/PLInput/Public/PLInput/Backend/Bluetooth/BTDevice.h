/*********************************************************\
 *  File: BTDevice.h                                     *
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


#ifndef __PLINPUT_BLUETOOTH_DEVICE_H__
#define __PLINPUT_BLUETOOTH_DEVICE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLInput/Backend/ConnectionDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Bluetooth device
*/
class BTDevice : public ConnectionDevice {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BTLinux;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLINPUT_API BTDevice();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    BTDevice to copy from
		*/
		PLINPUT_API BTDevice(const BTDevice &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API ~BTDevice();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cSource
		*    BTDevice to copy from
		*
		*  @return
		*    Reference to this BTDevice
		*/
		PLINPUT_API BTDevice &operator =(const BTDevice &cSource);

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cSource
		*    BTDevice to compare with
		*
		*  @return
		*    'true', if both are equal, else 'false'
		*/
		PLINPUT_API bool operator ==(const BTDevice &cSource);

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
		*    Get device address
		*
		*  @param[in] nIndex
		*    Address component (0..7)
		*
		*  @return
		*    Address
		*/
		PLINPUT_API PLCore::uint8 GetAddress(PLCore::int8 nIndex) const;

		/**
		*  @brief
		*    Get device class
		*
		*  @param[in] nIndex
		*    Class component (0..2)
		*
		*  @return
		*    Class
		*/
		PLINPUT_API PLCore::uint8 GetClass(PLCore::int8 nIndex) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Device data
		PLCore::String m_sName;			/**< Device name */
		PLCore::uint8  m_nAddress[8];	/**< Bluetooth address */
		PLCore::uint8  m_nClass[3];		/**< Bluetooth device class */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_BLUETOOTH_DEVICE_H__
