/*********************************************************\
 *  File: Provider.cpp                                   *
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
#include "PLInput/Input/InputManager.h"
#include "PLInput/Input/Devices/Device.h"
#include "PLInput/Backend/Provider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Provider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Provider::Provider()
{
}

/**
*  @brief
*    Destructor
*/
Provider::~Provider()
{
	// Clean up
	Clear();
}

/**
*  @brief
*    Get list of devices
*/
const List<Device*> &Provider::GetDevices() const
{
	// Return device list
	return m_lstDevices;
}

/**
*  @brief
*    Detect devices
*/
void Provider::DetectDevices(bool bReset)
{
	// Delete all devices?
	if (bReset)
		Clear();

	// Flag all current devices as 'not confirmed'
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++)
		m_lstDevices[i]->m_bConfirmed = false;

	// Detect new devices (the ones that are already there will be ignored by AddDevice)
	QueryDevices();

	// Delete all devices that are no longer there (confirmed == false)
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++) {
		Device *pDevice = m_lstDevices[i];
		if (!pDevice->m_bConfirmed) {
			// Remove device
			InputManager::GetInstance()->RemoveDevice(pDevice);
			m_lstDevices.Remove(pDevice);
			delete pDevice;
			i--;
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destroy all devices
*/
void Provider::Clear()
{
	// Delete all input devices
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++) {
		Device *pDevice = m_lstDevices[i];
		InputManager::GetInstance()->RemoveDevice(pDevice);
		delete pDevice;
	}
	m_lstDevices.Clear();
}

/**
*  @brief
*    Check if a device is already known
*/
bool Provider::CheckDevice(const String &sName)
{
	// Check if the device is already present
	Device *pDevice = InputManager::GetInstance()->GetDevice(sName);
	if (pDevice) {
		// Update device
		pDevice->m_bConfirmed = true;
		return true;
	}

	// Not found
	return false;
}

/**
*  @brief
*    Add a new input device
*/
bool Provider::AddDevice(const String &sName, Device *pDevice)
{
	// Check if the device is already present
	Device *pDeviceFound = InputManager::GetInstance()->GetDevice(sName);
	if (!pDeviceFound) {
		// Add device to manager
		if (InputManager::GetInstance()->AddDevice(pDevice)) {
			// Add device to own list - if we're in here, we now that the pDevice pointer is valid
			m_lstDevices.Add(pDevice);
			pDevice->m_bConfirmed = true;
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
