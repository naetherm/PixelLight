/*********************************************************\
 *  File: LegacyJoystickProvider.cpp                     *
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
#include "PLInput/Backend/Windows/LegacyJoystickDevice.h"
#include "PLInput/Backend/Windows/LegacyJoystickProvider.h"
#include "PLInput/PLInputWindowsIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(LegacyJoystickProvider, "PLInput", PLInput::Provider, "Input provider for Windows legacy joystick API")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(LegacyJoystickProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
LegacyJoystickProvider::LegacyJoystickProvider()
{
}

/**
*  @brief
*    Destructor
*/
LegacyJoystickProvider::~LegacyJoystickProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void LegacyJoystickProvider::QueryDevices()
{
	// Get list of joysticks
	JOYINFO sJoyInfo;
	for (uint32 nJoystick = 0; nJoystick < 16; nJoystick++) {
		// Check if joystick is valid
		if (joyGetPos(nJoystick, &sJoyInfo) == JOYERR_NOERROR) {
			// Get name and check if a device with that ID is not used already
			String sName = String("Joystick") + nJoystick;
			if (!CheckDevice(sName)) {
				// Add device
				LegacyJoystickDevice *pImpl = new LegacyJoystickDevice(nJoystick);
				AddDevice(sName, new Joystick(sName, pImpl));
			}
		} else {
			break;
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
LegacyJoystickProvider::LegacyJoystickProvider(const LegacyJoystickProvider &cSource)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Copy operator
*/
LegacyJoystickProvider &LegacyJoystickProvider::operator =(const LegacyJoystickProvider &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
