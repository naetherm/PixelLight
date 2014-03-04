/*********************************************************\
 *  File: LinuxEventDevice.cpp                           *
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
#include "PLInput/Backend/Linux/LinuxEventDevice.h"
#include "PLInput/Input/Devices/Device.h"
#include "PLInput/Input/Controls/Button.h"
#include "PLInput/Input/Controls/Axis.h"
#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>


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
LinuxEventDevice::LinuxEventDevice(int nFile) :
	m_nFile(nFile),
	m_nBusType(0),
	m_nVendor(0),
	m_nProduct(0),
	m_nVersion(0)
{
	// Do not destroy device implementation automatically, because this is managed by HIDLinux
	m_bDelete = false;

	// Get device info
	input_id sDeviceInfo;
	if (!ioctl(m_nFile, EVIOCGID, &sDeviceInfo)) {
		// Save device info
		m_nBusType = sDeviceInfo.bustype;
		m_nVendor  = sDeviceInfo.vendor;
		m_nProduct = sDeviceInfo.product;
		m_nVersion = sDeviceInfo.version;
	}
}

/**
*  @brief
*    Destructor
*/
LinuxEventDevice::~LinuxEventDevice()
{
	// Close file
	close(m_nFile);
}

/**
*  @brief
*    Get bus type
*/
uint16 LinuxEventDevice::GetBusType() const
{
	// Return bus type
	return m_nBusType;
}

/**
*  @brief
*    Get vendor ID
*/
uint16 LinuxEventDevice::GetVendorID() const
{
	// Return vendor ID
	return m_nVendor;
}

/**
*  @brief
*    Get product ID
*/
uint16 LinuxEventDevice::GetProductID() const
{
	// Return product ID
	return m_nProduct;
}

/**
*  @brief
*    Get version
*/
uint16 LinuxEventDevice::GetVersion() const
{
	// Return version
	return m_nVersion;
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void LinuxEventDevice::Update()
{
	// Read input events
	struct input_event pEvents[64];
	ssize_t nSize = read(m_nFile, pEvents, sizeof(struct input_event)*64);
	int nEvents = (nSize > 0 ? static_cast<int>(nSize / sizeof(struct input_event)) : 0);
	for (int i=0; i<nEvents; i++) {
		// Get corresponding control
		Control *pControl = nullptr;
		if (pEvents[i].code == ABS_X)
			pControl = m_pDevice->GetControl("X");
		else if (pEvents[i].code == ABS_Y)
			pControl = m_pDevice->GetControl("Y");
		else if (pEvents[i].code == ABS_Z)
			pControl = m_pDevice->GetControl("Z");
		else if (pEvents[i].code == ABS_RX)
			pControl = m_pDevice->GetControl("RX");
		else if (pEvents[i].code == ABS_RY)
			pControl = m_pDevice->GetControl("RY");
		else if (pEvents[i].code == ABS_RZ)
			pControl = m_pDevice->GetControl("RZ");
		else if (pEvents[i].code == ABS_HAT0X)
			pControl = m_pDevice->GetControl("Hat");
		else if (pEvents[i].code >= BTN_JOYSTICK && pEvents[i].code < BTN_GAMEPAD)
			pControl = m_pDevice->GetControl(String("Button") + (pEvents[i].code - BTN_JOYSTICK));
		else if (pEvents[i].code >= BTN_GAMEPAD && pEvents[i].code < BTN_DIGI)
			pControl = m_pDevice->GetControl(String("Button") + (pEvents[i].code - BTN_GAMEPAD));

		// Get control type
		Axis   *pAxis   = (pControl != nullptr && pControl->GetType() == ControlAxis)   ? static_cast<Axis*>  (pControl) : nullptr;
		Button *pButton = (pControl != nullptr && pControl->GetType() == ControlButton) ? static_cast<Button*>(pControl) : nullptr;

		// Set control value
		if (pEvents[i].type == EV_KEY && pButton) {
			// Button
			pButton->SetPressed(pEvents[i].value != 0);
		} else if ((pEvents[i].type == EV_ABS || pEvents[i].type == EV_REL) && pAxis) {
			// Absolute or relative axis
			// Get minimum and maximum values
			input_absinfo sAbsInfo;
			if (!ioctl(m_nFile, EVIOCGABS(0), &sAbsInfo)) {
				// Get value in a range of -1.0 - +1.0
				float fValue = (static_cast<float>(pEvents[i].value - sAbsInfo.minimum) / static_cast<float>(sAbsInfo.maximum - sAbsInfo.minimum)) * 2.0f - 1.0f;
				if (fValue >  1.0f)
					fValue =  1.0f;
				if (fValue < -1.0f)
					fValue = -1.0f;
				pAxis->SetValue(fValue, false);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
