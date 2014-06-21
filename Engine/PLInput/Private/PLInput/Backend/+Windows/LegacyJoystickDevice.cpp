/*********************************************************\
 *  File: LegacyJoystickDevice.cpp                       *
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
#include <PLMath/Math.h>
#include "PLInput/Input/Devices/Joystick.h"
#include "PLInput/Backend/+Windows/LegacyJoystickDevice.h"
#include "PLInput/+Windows/PLInputWindowsIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LegacyJoystickDevice::LegacyJoystickDevice(uint32 nJoystickID) :
	m_nJoystickID (nJoystickID),
	m_nFallbackPOV(0)
{
	// Destroy device implementation automatically
	m_bDelete = true;

	// Check whether or not the joystick supports POV
	JOYCAPS sJoyCaps;
	MemoryManager::Set(&sJoyCaps, 0, sizeof(sJoyCaps));
	if (joyGetDevCaps (m_nJoystickID, &sJoyCaps, sizeof(sJoyCaps)) == JOYERR_NOERROR && !(sJoyCaps.wCaps & JOYCAPS_HASPOV)) {
		// Get joystick state
		JOYINFOEX sJoyInfo;
		sJoyInfo.dwSize  = sizeof(JOYINFOEX);
		sJoyInfo.dwFlags = JOY_RETURNALL;
		if (joyGetPosEx(m_nJoystickID, &sJoyInfo) == JOYERR_NOERROR) {
			// Use the initial position as POV
			m_nFallbackPOV = sJoyInfo.dwXpos;
		}
	}
}

/**
*  @brief
*    Destructor
*/
LegacyJoystickDevice::~LegacyJoystickDevice()
{
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void LegacyJoystickDevice::Update()
{
	// Check if input device is valid
	if (m_pDevice) {
		// Get joystick device
		Joystick *pJoystick = static_cast<Joystick*>(m_pDevice);

		// Get joystick state
		JOYINFOEX sJoyInfo;
		sJoyInfo.dwSize  = sizeof(JOYINFOEX);
		sJoyInfo.dwFlags = JOY_RETURNALL;
		if (joyGetPosEx(m_nJoystickID, &sJoyInfo) == JOYERR_NOERROR) {
			// Update axes
			for (uint32 i=0; i<6; i++) {
				// Get POV
				int nPOV = m_nFallbackPOV ? m_nFallbackPOV : sJoyInfo.dwPOV / 2;

				// Calculate delta
				int nDelta = 0;
				switch (i) {
					case 0: nDelta = static_cast<int>(sJoyInfo.dwXpos - nPOV); break;
					case 1: nDelta = static_cast<int>(sJoyInfo.dwYpos - nPOV); break;
					case 2: nDelta = static_cast<int>(sJoyInfo.dwZpos - nPOV); break;
					case 3: nDelta = static_cast<int>(sJoyInfo.dwRpos - nPOV); break;
					case 4: nDelta = static_cast<int>(sJoyInfo.dwUpos - nPOV); break;
					case 5: nDelta = static_cast<int>(sJoyInfo.dwVpos - nPOV); break;
				}

				// Calculate axis values (between -1.0f and 1.0f)
				float fPos		 = 0.0f;
				int   nThreshold = pJoystick->GetThreshold();
				if (PLMath::Math::Abs(nDelta) >= nThreshold) {
					if (nDelta < 0.0f)
						fPos = static_cast<float>(nDelta + nThreshold) / (nPOV - nThreshold);
					else
						fPos = static_cast<float>(nDelta - nThreshold) / (nPOV - nThreshold);
					if (fPos < -1.0f)
						fPos = -1.0f;
					if (fPos >  1.0f)
						fPos =  1.0f;
				}

				// Set new value
				Axis *pAxis = pJoystick->GetAxes()[i];
				if (pAxis && pAxis->GetValue() != fPos)
					pAxis->SetValue(fPos, false);
			}

			// Update buttons
			uint32 nFlag = 1;
			for (uint32 i=0; i<32; i++) {
				bool bPressed = ((sJoyInfo.dwButtons & nFlag) != 0);
				Button *pButton = pJoystick->GetButtons()[i];
				if (pButton && pButton->IsPressed() != bPressed)
					pButton->SetPressed(bPressed);
				nFlag = nFlag << 1;
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
