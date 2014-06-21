/*********************************************************\
 *  File: MacOSXProvider.cpp                             *
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
#include "PLInput/Backend/MacOSX/MacOSXKeyboardDevice.h"
#include "PLInput/Backend/MacOSX/MacOSXMouseDevice.h"
#include "PLInput/Backend/MacOSX/MacOSXProvider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MacOSXProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MacOSXProvider::MacOSXProvider()
{
}

/**
*  @brief
*    Destructor
*/
MacOSXProvider::~MacOSXProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void MacOSXProvider::QueryDevices()
{
	// Create a keyboard device
	if (!CheckDevice("Keyboard")) {
		// Add device
		MacOSXKeyboardDevice *pImpl = new MacOSXKeyboardDevice();
		AddDevice("Keyboard", new Keyboard("Keyboard", pImpl));
	}

	// Create a mouse device
	if (!CheckDevice("Mouse")) {
		MacOSXMouseDevice *pImpl = new MacOSXMouseDevice();
		AddDevice("Mouse", new Mouse("Mouse", pImpl));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
