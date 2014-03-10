/*********************************************************\
 *  File: LinuxEventDevice.h                             *
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


#ifndef __PLINPUT_LINUX_EVENTDEVICE_H__
#define __PLINPUT_LINUX_EVENTDEVICE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Backend/UpdateDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux device implementation using the event subsystem of input.h
*/
class LinuxEventDevice : public UpdateDevice {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nFile
		*    File handle
		*/
		PLINPUT_API LinuxEventDevice(int nFile);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~LinuxEventDevice();

		/**
		*  @brief
		*    Get bus type
		*
		*  @return
		*    Bus type
		*/
		PLINPUT_API PLCore::uint16 GetBusType() const;

		/**
		*  @brief
		*    Get vendor ID
		*
		*  @return
		*    Vendor ID
		*/
		PLINPUT_API PLCore::uint16 GetVendorID() const;

		/**
		*  @brief
		*    Get product ID
		*
		*  @return
		*    Product ID
		*/
		PLINPUT_API PLCore::uint16 GetProductID() const;

		/**
		*  @brief
		*    Get version
		*
		*  @return
		*    Version
		*/
		PLINPUT_API PLCore::uint16 GetVersion() const;


	//[-------------------------------------------------------]
	//[ Public virtual UpdateDevice functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual void Update() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int				m_nFile;	/**< File handle */
		PLCore::uint16	m_nBusType;	/**< Bus type (e.g. USB) */
		PLCore::uint16	m_nVendor;	/**< Vendor ID */
		PLCore::uint16	m_nProduct;	/**< Product ID */
		PLCore::uint16	m_nVersion;	/**< Version */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_LINUX_EVENTDEVICE_H__
