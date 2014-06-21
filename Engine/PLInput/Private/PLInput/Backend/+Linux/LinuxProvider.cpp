/*********************************************************\
 *  File: LinuxProvider.cpp                              *
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
#include "PLInput/Input/Devices/Joystick.h"
#include "PLInput/Backend/Linux/LinuxKeyboardDevice.h"
#include "PLInput/Backend/Linux/LinuxMouseDevice.h"
#include "PLInput/Backend/Linux/LinuxEventDevice.h"
#include "PLInput/Backend/Linux/LinuxProvider.h"
#include <dirent.h>
#include <fcntl.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(LinuxProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
LinuxProvider::LinuxProvider()
{
}

/**
*  @brief
*    Destructor
*/
LinuxProvider::~LinuxProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void LinuxProvider::QueryDevices()
{
	// Create a keyboard device
	if (!CheckDevice("Keyboard")) {
		// Add device
		LinuxKeyboardDevice *pImpl = new LinuxKeyboardDevice();
		AddDevice("Keyboard", new Keyboard("Keyboard", pImpl));
	}

	// Create a mouse device
	if (!CheckDevice("Mouse")) {
		LinuxMouseDevice *pImpl = new LinuxMouseDevice();
		AddDevice("Mouse", new Mouse("Mouse", pImpl));
	}

	// List devices in "/dev/input/event*"
	DIR *pDir = opendir("/dev/input");
	if (pDir) {
		int nDevice = 0;

		// Read first entry
		dirent *pEntry = readdir(pDir);
		while (pEntry) {
			// Check if filename is "eventX"
			String sFilename = pEntry->d_name;
			if (sFilename.GetSubstring(0, 5) == "event") {
				// Try to open the device
				int f = open(("/dev/input/" + sFilename).GetASCII(), O_RDWR | O_NONBLOCK);
				if (f > 0) {
					// Create device
					LinuxEventDevice *pImpl = new LinuxEventDevice(f);
					String sName = String("Joystick") + nDevice;
					AddDevice(sName, new Joystick(sName, pImpl));
					nDevice++;
				}
			}

			// Read next entry
			pEntry = readdir(pDir);
		}

		// Be polite and close the directory after we're done...
		closedir(pDir);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
