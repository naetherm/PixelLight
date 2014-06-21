/*********************************************************\
 *  File: SpaceMouse.cpp                                 *
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
#include "PLInput/Backend/HID/HIDDevice.h"
#include "PLInput/Input/Devices/SpaceMouse.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(SpaceMouse)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SpaceMouse::SpaceMouse(const String &sName, DeviceImpl *pImpl) : Device(sName, "SpaceMouse type controller", pImpl),
	TransX	(this,	"TransX",	"X translation axis"),
	TransY	(this,	"TransY",	"Y translation axis"),
	TransZ	(this,	"TransZ",	"Z translation axis"),
	RotX	(this,	"RotX",		"X rotation axis"),
	RotY	(this,	"RotY",		"Y rotation axis"),
	RotZ	(this,	"RotZ",		"Z rotation axis"),
	Button0	(this,	"Button0",	"Button #0"),
	Button1	(this,	"Button1",	"Button #1"),
	Button2	(this,	"Button2",	"Button #2"),
	Button3	(this,	"Button3",	"Button #3"),
	Button4	(this,	"Button4",	"Button #4"),
	Button5	(this,	"Button5",	"Button #5"),
	Button6	(this,	"Button6",	"Button #6"),
	Button7	(this,	"Button7",	"Button #7"),
	EventHandlerOnDeviceRead(&SpaceMouse::OnDeviceRead, this),
	m_pHIDDevice(nullptr)
{
	// Check if we have a HID backend
	if (pImpl && pImpl->GetBackendType() == BackendHID) {
		// Save extra pointer to HIDDevice
		m_pHIDDevice = static_cast<HIDDevice*>(pImpl);

		// Connect to HIDDevice events
		m_pHIDDevice->EventOnRead.Connect(EventHandlerOnDeviceRead);

		// Connect to HID device
		m_pHIDDevice->Open();
	}
}

/**
*  @brief
*    Destructor
*/
SpaceMouse::~SpaceMouse()
{
	// Check if we have a HID backend
	if (m_pHIDDevice) {
		// We use m_pImpl here to check, because if the device backend has been deleted before, m_pImpl has
		// been reset to a null pointer, but not m_pHIDDevice as this is unknown in the base class
		if (m_pImpl) {
			// Disconnect
			m_pHIDDevice->Close();
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the HID device has read some data
*/
void SpaceMouse::OnDeviceRead()
{
	// Get input buffer
	uint8 *pInputBuffer = m_pHIDDevice->GetInputBuffer();
	if (pInputBuffer) {
		// Read data
		switch (pInputBuffer[0]) {
			// Translation
			case 0x01:
			{
				float fTransX = static_cast<float>(static_cast<int16>((pInputBuffer[1] & 0x000000ff) | (static_cast<int>(pInputBuffer[2])<<8 & 0xffffff00)));
				float fTransY = static_cast<float>(static_cast<int16>((pInputBuffer[3] & 0x000000ff) | (static_cast<int>(pInputBuffer[4])<<8 & 0xffffff00)));
				float fTransZ = static_cast<float>(static_cast<int16>((pInputBuffer[5] & 0x000000ff) | (static_cast<int>(pInputBuffer[6])<<8 & 0xffffff00)));
				if (TransX.GetValue() != fTransX)
					TransX.SetValue(fTransX, false);
				if (TransY.GetValue() != fTransY)
					TransY.SetValue(fTransY, false);
				if (TransZ.GetValue() != fTransZ)
					TransZ.SetValue(fTransZ, false);
				break;
			}

			// Rotation
			case 0x02:
			{
				float fRotX = static_cast<float>(static_cast<int16>((pInputBuffer[1] & 0x000000ff) | (static_cast<int>(pInputBuffer[2])<<8 & 0xffffff00)));
				float fRotY = static_cast<float>(static_cast<int16>((pInputBuffer[3] & 0x000000ff) | (static_cast<int>(pInputBuffer[4])<<8 & 0xffffff00)));
				float fRotZ = static_cast<float>(static_cast<int16>((pInputBuffer[5] & 0x000000ff) | (static_cast<int>(pInputBuffer[6])<<8 & 0xffffff00)));
				if (RotX.GetValue() != fRotX)
					RotX.SetValue(fRotX, false);
				if (RotY.GetValue() != fRotY)
					RotY.SetValue(fRotY, false);
				if (RotZ.GetValue() != fRotZ)
					RotZ.SetValue(fRotZ, false);
				break;
			}

			// Buttons
			case 0x03:
			{
				bool bPressed = ((pInputBuffer[1] & 0x0001) != 0);
				if (Button0.IsPressed() != bPressed)
					Button0.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0002) != 0);
				if (Button1.IsPressed() != bPressed)
					Button1.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0004) != 0);
				if (Button2.IsPressed() != bPressed)
					Button2.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0008) != 0);
				if (Button3.IsPressed() != bPressed)
					Button3.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0010) != 0);
				if (Button4.IsPressed() != bPressed)
					Button4.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0020) != 0);
				if (Button5.IsPressed() != bPressed)
					Button5.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0040) != 0);
				if (Button6.IsPressed() != bPressed)
					Button6.SetPressed(bPressed);
				bPressed = ((pInputBuffer[1] & 0x0080) != 0);
				if (Button7.IsPressed() != bPressed)
					Button7.SetPressed(bPressed);
				break;
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
