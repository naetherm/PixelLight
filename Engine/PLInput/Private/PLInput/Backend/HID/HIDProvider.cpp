/*********************************************************\
 *  File: HIDProvider.cpp                                *
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
#include "PLInput/Input/Devices/Joystick.h"
#include "PLInput/Input/Devices/WiiMote.h"
#include "PLInput/Input/Devices/SpaceMouse.h"
#include "PLInput/Backend/HID/HID.h"
#include "PLInput/Backend/HID/HIDDefinitions.h"
#include "PLInput/Backend/HID/HIDDevice.h"
#include "PLInput/Backend/HID/HIDProvider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(HIDProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
HIDProvider::HIDProvider()
{
}

/**
*  @brief
*    Destructor
*/
HIDProvider::~HIDProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void HIDProvider::QueryDevices()
{
	// Device counter
	int nSpaceMouse = 0;
	int nWiiMote	= 0;
// [TODO] HIDJoystick not finished yet ...
	int nJoystick	= 0;

	// Get list of HID-devices
	const Array<HIDDevice*> &lstDevices = HID::GetInstance()->GetDevices();
	for (uint32 i=0; i<lstDevices.GetNumOfElements(); i++) {
		// Get device
		HIDDevice *pDevice = lstDevices[i];

		// Get vendor and product ID
		uint32 nVendor    = pDevice->GetVendor();
		uint32 nProduct   = pDevice->GetProduct();
		uint32 nUsage	  = pDevice->GetUsage();
		uint32 nUsagePage = pDevice->GetUsagePage();

		// Check device type

		// SpaceMouse
		if (nVendor == SpaceMouse::VendorID && nUsagePage == UsagePageGeneric && nUsage == UsageMultiAxisController) {
			String sName = String("SpaceMouse") + nSpaceMouse;
			nSpaceMouse++;
			if (!CheckDevice(sName))
				AddDevice(sName, new SpaceMouse(sName, pDevice));

		// WiiMote
		} else if (nVendor == WiiMote::VendorID && nProduct == WiiMote::ProductID) {
			String sName = String("WiiMote") + nWiiMote;
			nWiiMote++;
			if (!CheckDevice(sName))
				AddDevice(sName, new WiiMote(sName, pDevice));

		// Joystick or Joypad
		} else if (nUsagePage == UsagePageGeneric && (nUsage == UsageJoystick || nUsage == UsageGamepad)) {
			String sName = String("Joystick") + nJoystick;
			nJoystick++;
			if (!CheckDevice(sName))
				AddDevice(sName, new Joystick(sName, pDevice));
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
HIDProvider::HIDProvider(const HIDProvider &cSource)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Copy operator
*/
HIDProvider &HIDProvider::operator =(const HIDProvider &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
