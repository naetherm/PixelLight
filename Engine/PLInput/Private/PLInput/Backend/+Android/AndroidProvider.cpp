/*********************************************************\
 *  File: AndroidProvider.cpp                            *
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
#include <android/input.h>
#include <PLCore/System/SystemAndroid.h>
#include "PLInput/Input/Devices/Mouse.h"
#include "PLInput/Input/Devices/Keyboard.h"
#include "PLInput/Input/Devices/SplitTouchPad.h"
#include "PLInput/Input/Devices/SensorManager.h"
#include "PLInput/Backend/Android/AndroidMouseDevice.h"
#include "PLInput/Backend/Android/AndroidKeyboardDevice.h"
#include "PLInput/Backend/Android/AndroidSplitTouchPadDevice.h"
#include "PLInput/Backend/Android/AndroidSensorManagerDevice.h"
#include "PLInput/Backend/Android/AndroidProvider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(AndroidProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
AndroidProvider::AndroidProvider() :
	SlotInputEvent(&AndroidProvider::OnInputEvent, this),
	m_pAndroidKeyboardDevice(nullptr),
	m_pAndroidSplitTouchPadDevice(nullptr),
	m_pAndroidMouseDevice(nullptr)
{
	// Connect the Android input event handler
	SystemAndroid::EventInputEvent.Connect(SlotInputEvent);
}

/**
*  @brief
*    Destructor
*/
AndroidProvider::~AndroidProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void AndroidProvider::QueryDevices()
{
	// Create a keyboard device
	if (!CheckDevice("Keyboard")) {
		// Add device
		m_pAndroidKeyboardDevice = new AndroidKeyboardDevice();
		AddDevice("Keyboard", new Keyboard("Keyboard", m_pAndroidKeyboardDevice));
	}

	// Create a splitscreen touch pad device
	if (!CheckDevice("SplitTouchPad")) {
		// Add device
		m_pAndroidSplitTouchPadDevice = new AndroidSplitTouchPadDevice();
		AddDevice("SplitTouchPad", new SplitTouchPad("SplitTouchPad", m_pAndroidSplitTouchPadDevice));
	}

	// Create a mouse device
	if (!CheckDevice("Mouse")) {
		// Add device
		m_pAndroidMouseDevice = new AndroidMouseDevice(m_pAndroidSplitTouchPadDevice);
		AddDevice("Mouse", new Mouse("Mouse", m_pAndroidMouseDevice));
	}

	// Create a sensor manager device
	if (!CheckDevice("SensorManager")) {
		AndroidSensorManagerDevice *pImpl = new AndroidSensorManagerDevice();
		AddDevice("SensorManager", new SensorManager("SensorManager", pImpl));
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called to process the next Android input event
*/
void AndroidProvider::OnInputEvent(const struct AInputEvent &cAInputEvent)
{
	// Check the input event type
	switch (AInputEvent_getType(&cAInputEvent)) {
		// Key (e.g. from the soft keyboard)
		case AINPUT_EVENT_TYPE_KEY:
			if (m_pAndroidKeyboardDevice)
				m_pAndroidKeyboardDevice->OnKeyInputEvent(cAInputEvent);
			break;

		// Motion (e.g. from the touchscreen)
		case AINPUT_EVENT_TYPE_MOTION:
			if (m_pAndroidMouseDevice)
				m_pAndroidMouseDevice->OnMotionInputEvent(cAInputEvent);
			if (m_pAndroidSplitTouchPadDevice)
				m_pAndroidSplitTouchPadDevice->OnMotionInputEvent(cAInputEvent);
			break;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput