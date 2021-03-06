/*********************************************************\
 *  File: RawInputProvider.cpp                           *
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
#include "PLInput/Input/Devices/Keyboard.h"
#include "PLInput/Input/Devices/Mouse.h"
#include "PLInput/Backend/Windows/RawInput.h"
#include "PLInput/Backend/Windows/RawInputDevice.h"
#include "PLInput/Backend/Windows/RawInputProvider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(RawInputProvider, "PLInput", PLInput::Provider, "Input provider for Windows using Raw-Input methods for keyboard and mouse")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(RawInputProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
RawInputProvider::RawInputProvider()
{
}

/**
*  @brief
*    Destructor
*/
RawInputProvider::~RawInputProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void RawInputProvider::QueryDevices()
{
	// Get list of Raw Input devices
	uint32 nKeyboard = 0;
	uint32 nMouse = 0;
	RawInput::GetInstance()->DetectDevices();
	const List<RawInputDevice*> &lstDevices = RawInput::GetInstance()->GetDevices();
	for (uint32 i=0; i<lstDevices.GetNumOfElements(); i++) {
		// Get device
		RawInputDevice *pDevice = lstDevices[i];
		if (pDevice->GetType() == RIM_TYPEKEYBOARD || pDevice->GetType() == RIM_TYPEMOUSE) {
			// Set device name
			String sName;
			if (pDevice->GetType() == RIM_TYPEKEYBOARD)	{
				// Keyboard
				sName = "Keyboard";
				if (!pDevice->IsVirtual()) {
					sName += nKeyboard;
					nKeyboard++;
				}
			} else if (pDevice->GetType() == RIM_TYPEMOUSE)	{
				// Mouse
				sName = "Mouse";
				if (!pDevice->IsVirtual()) {
					sName += nMouse;
					nMouse++;
				}
			}

			// Check if device is already known
			if (!CheckDevice(sName)) {
				// Create device
				if (pDevice->GetType() == RIM_TYPEKEYBOARD) {
					// Create a keyboard device
					AddDevice(sName, new Keyboard(sName, pDevice));
				} else if (pDevice->GetType() == RIM_TYPEMOUSE) {
					// Create a mouse device
					AddDevice(sName, new Mouse(sName, pDevice));
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
