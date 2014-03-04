/*********************************************************\
 *  File: HIDDeviceLinux.h                               *
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


#ifndef __PLINPUT_HIDDEVICE_LINUX_H__
#define __PLINPUT_HIDDEVICE_LINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Backend/HID/HIDDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux implementation of 'HIDDevice'
*/
class HIDDeviceLinux : public HIDDevice {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLINPUT_API HIDDeviceLinux();

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~HIDDeviceLinux();


	//[-------------------------------------------------------]
	//[ Public virtual ConnectionDevice functions             ]
	//[-------------------------------------------------------]
	public:
		PLINPUT_API virtual bool Open(PLCore::uint16 nOutputPort = 0, PLCore::uint16 nInputPort = 0) override;
		PLINPUT_API virtual bool Close() override;
		PLINPUT_API virtual bool IsOpen() const override;
		PLINPUT_API virtual bool Read(PLCore::uint8 *pBuffer, PLCore::uint32 nSize) override;
		PLINPUT_API virtual bool Write(const PLCore::uint8 *pBuffer, PLCore::uint32 nSize) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_HIDDEVICE_LINUX_H__
