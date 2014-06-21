/*********************************************************\
 *  File: Joystick.cpp                                   *
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
#include "PLInput/Backend/UpdateDevice.h"
#include "PLInput/Backend/HID/HIDDefinitions.h"
#include "PLInput/Backend/HID/HIDDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Joystick)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Joystick::Joystick(const String &sName, DeviceImpl *pImpl) : Device(sName, "Joystick input controller", pImpl),
	X		(this,	"X",		"X axis"),
	Y		(this,	"Y",		"Y axis"),
	Z		(this,	"Z",		"Z axis"),
	RX		(this,	"RX",		"Rotation X axis"),
	RY		(this,	"RY",		"Rotation Y axis"),
	RZ		(this,	"RZ",		"Rotation Z axis"),
	Hat		(this,	"Hat",		"Hat axis"),
	Button0	(this,	"Button0",	"Button #0"),
	Button1	(this,	"Button1",	"Button #1"),
	Button2	(this,	"Button2",	"Button #2"),
	Button3	(this,	"Button3",	"Button #3"),
	Button4	(this,	"Button4",	"Button #4"),
	Button5	(this,	"Button5",	"Button #5"),
	Button6	(this,	"Button6",	"Button #6"),
	Button7	(this,	"Button7",	"Button #7"),
	Button8	(this,	"Button8",	"Button #8"),
	Button9	(this,	"Button9",	"Button #9"),
	Button10(this,	"Button10",	"Button #10"),
	Button11(this,	"Button11",	"Button #11"),
	Button12(this,	"Button12",	"Button #12"),
	Button13(this,	"Button13",	"Button #13"),
	Button14(this,	"Button14",	"Button #14"),
	Button15(this,	"Button15",	"Button #15"),
	Button16(this,	"Button16",	"Button #16"),
	Button17(this,	"Button17",	"Button #17"),
	Button18(this,	"Button18",	"Button #18"),
	Button19(this,	"Button19",	"Button #19"),
	Button20(this,	"Button20",	"Button #20"),
	Button21(this,	"Button21",	"Button #21"),
	Button22(this,	"Button22",	"Button #22"),
	Button23(this,	"Button23",	"Button #23"),
	Button24(this,	"Button24",	"Button #24"),
	Button25(this,	"Button25",	"Button #25"),
	Button26(this,	"Button26",	"Button #26"),
	Button27(this,	"Button27",	"Button #27"),
	Button28(this,	"Button28",	"Button #28"),
	Button29(this,	"Button29",	"Button #29"),
	Button30(this,	"Button30",	"Button #30"),
	Button31(this,	"Button31",	"Button #31"),
	Rumble1	(this,	"Rumble1",	"Rumble motor #1"),
	Rumble2	(this,	"Rumble2",	"Rumble motor #2"),
	Rumble3	(this,	"Rumble3",	"Rumble motor #3"),
	Rumble4	(this,	"Rumble4",	"Rumble motor #4"),
	EventHandlerOnDeviceRead(&Joystick::OnDeviceRead, this),
	m_pHIDDevice(nullptr),
	m_nThreshold(12000)
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
Joystick::~Joystick()
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

/**
*  @brief
*    Get threshold
*/
int Joystick::GetThreshold() const
{
	return m_nThreshold;
}

/**
*  @brief
*    Set threshold
*/
void Joystick::SetThreshold(int nThreshold)
{
	m_nThreshold = nThreshold;
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void Joystick::Update()
{
	// Check if we have an UpdateDevice backend
	if (m_pImpl && m_pImpl->GetBackendType() == BackendUpdateDevice) {
		// Update device backend
		static_cast<UpdateDevice*>(m_pImpl)->Update();
	}
}

void Joystick::UpdateOutputControl(Control *pControl)
{
	// Check if we have a HID backend
	if (m_pHIDDevice) {
		// Get output values
		const List<HIDCapability> &lstOutputValues = m_pHIDDevice->GetOutputValues();
		for (uint32 i=0; i<lstOutputValues.GetNumOfElements(); i++) {
			// Get capability
			HIDCapability *pCapability = &lstOutputValues[i];
			Effect *pEffect = nullptr;

			// Rumble
			if (pCapability->m_nUsagePage == UsagePageLED) {
				// Get effect control
				if (pCapability->m_nUsagePage == UsagePageLED && pCapability->m_nUsage == UsageSlowBlinkOnTime)
					pEffect = &Rumble1;
				else if (pCapability->m_nUsagePage == UsagePageLED && pCapability->m_nUsage == UsageSlowBlinkOffTime)
					pEffect = &Rumble2;
				else if (pCapability->m_nUsagePage == UsagePageLED && pCapability->m_nUsage == UsageFastBlinkOnTime)
					pEffect = &Rumble3;
				else if (pCapability->m_nUsagePage == UsagePageLED && pCapability->m_nUsage == UsageFastBlinkOffTime)
					pEffect = &Rumble4;

				// Set effect value
				if (pEffect) {
					// Get value (must be between 0..1)
					float fValue = pEffect->GetValue();
					if (fValue < 0.0f)
						fValue = 0.0f;
					if (fValue > 1.0f)
						fValue = 1.0f;

					// Scale from 0..1 to logical range and set value
					pCapability->m_nValue = static_cast<uint32>(pCapability->m_nLogicalMin + fValue*(pCapability->m_nLogicalMax-pCapability->m_nLogicalMin));
				}
			}
		}

		// Send output report with that ID that belongs to the given control
		m_pHIDDevice->SendOutputReport();
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse HID input report
*/
void Joystick::ParseInputReport()
{
	// Get input buffer
	uint8 *pInputBuffer = m_pHIDDevice->GetInputBuffer();
	if (pInputBuffer) {
		// Parse input report
		m_pHIDDevice->ParseInputReport(pInputBuffer, m_pHIDDevice->GetInputReportSize());

		// Update axes
		const List<HIDCapability> &lstInputValues = m_pHIDDevice->GetInputValues();
		for (uint32 i=0; i<lstInputValues.GetNumOfElements(); i++) {
			// Get raw value and compute logical value
			uint32 nValue = lstInputValues[i].m_nValue;
			float  fValue = static_cast<float>(nValue);
			if (lstInputValues[i].m_nUsage != UsageHat) {
				uint32 nMin	= static_cast<uint32>(lstInputValues[i].m_nLogicalMin);
				uint32 nMax	= static_cast<uint32>(lstInputValues[i].m_nLogicalMax);
				uint32 nMid	=  nMin/2 + nMax/2;
				fValue		= (static_cast<float>(nValue) - nMid) / (static_cast<float>(nMax) - nMin) * 2.0f;
			}

			// Set axis value
			switch (lstInputValues[i].m_nUsage) {
				case UsageX:
					if (X.GetValue() != fValue)
						X.SetValue(fValue, false);
					break;

				case UsageY:
					if (Y.GetValue() != fValue)
						Y.SetValue(fValue, false);
					break;

				case UsageZ:
					if (Z.GetValue() != fValue)
						Z.SetValue(fValue, false);
					break;

				case UsageRX:
					if (RX.GetValue() != fValue)
						RX.SetValue(fValue, false);
					break;

				case UsageRY:
					if (RY.GetValue() != fValue)
						RY.SetValue(fValue, false);
					break;

				case UsageRZ:
					if (RZ.GetValue() != fValue)
						RZ.SetValue(fValue, false);
					break;

				case UsageHat:
					if (Hat.GetValue() != fValue)
						Hat.SetValue(fValue, false);
					break;
			}
		}

		// Update buttons
		const List<HIDCapability> &lstButtons = m_pHIDDevice->GetInputButtons();
		for (uint32 i=0; i<lstButtons.GetNumOfElements(); i++) {
			// Get state of buttons
			uint32 nValue = lstButtons[i].m_nValue;

			// Go through usage range
			for (uint32 nUsage = lstButtons[i].m_nUsageMin; nUsage <= lstButtons[i].m_nUsageMax; nUsage++) {
				// Check if button is pressed
				bool bPressed = ((nValue & 1) != 0);
				nValue = nValue >> 1;

				// Get button
				int nButton = (nUsage - UsageButton1);
				Button *pButton = GetButtons()[nButton];
				if (pButton) {
					// Update only if state has changed
					if (pButton->IsPressed() != bPressed)
						pButton->SetPressed(bPressed);
				}
			}
		}
	}
}

/**
*  @brief
*    Called when the HID device has read some data
*/
void Joystick::OnDeviceRead()
{
	// Parse input report
	ParseInputReport();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
