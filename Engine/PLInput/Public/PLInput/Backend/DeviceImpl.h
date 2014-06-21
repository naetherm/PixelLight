/*********************************************************\
 *  File: DeviceImpl.h                                   *
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


#ifndef __PLINPUT_DEVICEIMPL_H__
#define __PLINPUT_DEVICEIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/PLInput.h"
#include "PLInput/PLInputDefinitions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Device;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Device implementation base class
*
*  @remarks
*    A device implementation class represents a concrete device interface implementation, such as a HID device
*    or a Windows RawInput device. A logical device will use an implementation class to interface with the underlying
*    system device, and concrete backend implementation will implement those interface for their specific systems.
*/
class DeviceImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Device;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLINPUT_API DeviceImpl();

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~DeviceImpl();

		/**
		*  @brief
		*    Get backend type
		*
		*  @return
		*    Type of backend
		*/
		PLINPUT_API EDeviceBackend GetBackendType() const;

		/**
		*  @brief
		*    Get device
		*
		*  @return
		*    Device that owns this device implementation, can be a null pointer
		*/
		PLINPUT_API Device *GetDevice() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Set device
		*
		*  @param[in] pDevice
		*    Device that owns this device implementation, can be a null pointer
		*/
		PLINPUT_API void SetDevice(Device *pDevice);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EDeviceBackend	 m_nBackendType;	/**< Device backend type */
		Device			*m_pDevice;			/**< Device that owns to this device implementation, can be a null pointer */
		bool			 m_bDelete;			/**< If 'true', the device implementation will be destroyed automatically with the Device, else it must be destroyed manually */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_DEVICEIMPL_H__
