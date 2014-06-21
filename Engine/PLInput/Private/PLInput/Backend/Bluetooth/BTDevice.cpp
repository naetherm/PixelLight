/*********************************************************\
 *  File: BTDevice.cpp                                   *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/MemoryManager.h>
#include "PLInput/Backend/Bluetooth/BTDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BTDevice::BTDevice()
{
	// Set device type
	m_nDeviceType = ConnectionDevice::DeviceTypeBluetooth;

	// Initialize device
	MemoryManager::Set(m_nAddress, 0, 6);
	MemoryManager::Set(m_nClass,   0, 3);
}

/**
*  @brief
*    Copy constructor
*/
BTDevice::BTDevice(const BTDevice &cSource) :
	m_sName(cSource.m_sName)
{
	// Set device type
	m_nDeviceType = ConnectionDevice::DeviceTypeBluetooth;

	// Copy device
	MemoryManager::Copy(m_nAddress, cSource.m_nAddress, 6);
	MemoryManager::Copy(m_nClass,	cSource.m_nClass,   3);
}

/**
*  @brief
*    Destructor
*/
BTDevice::~BTDevice()
{
}

/**
*  @brief
*    Assignment operator
*/
BTDevice &BTDevice::operator =(const BTDevice &cSource)
{
	// Copy device
	m_sName = cSource.m_sName;
	MemoryManager::Copy(m_nAddress, cSource.m_nAddress, 6);
	MemoryManager::Copy(m_nClass,	cSource.m_nClass,   3);
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool BTDevice::operator ==(const BTDevice &cSource)
{
	// Compare devices
	return (m_sName == cSource.m_sName &&
			MemoryManager::Compare(m_nAddress,	cSource.m_nAddress, 6) == 0 &&
			MemoryManager::Compare(m_nClass,	cSource.m_nClass,   3) == 0);
}

/**
*  @brief
*    Get device name
*/
String BTDevice::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Get device address
*/
uint8 BTDevice::GetAddress(int8 nIndex) const
{
	// Return address part
	return (nIndex >= 0 && nIndex < 6) ? m_nAddress[nIndex] : 0;
}

/**
*  @brief
*    Get device class
*/
uint8 BTDevice::GetClass(int8 nIndex) const
{
	// Return class part
	return (nIndex >= 0 && nIndex < 3) ? m_nClass[nIndex] : 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
