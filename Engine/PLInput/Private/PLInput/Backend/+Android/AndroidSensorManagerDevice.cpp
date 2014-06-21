/*********************************************************\
 *  File: AndroidSensorManagerDevice.cpp                 *
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
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <PLCore/Log/Log.h>
#include "PLInput/Input/Devices/SensorManager.h"
#include "PLInput/Backend/Android/AndroidSensorManagerDevice.h"


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
AndroidSensorManagerDevice::AndroidSensorManagerDevice() :
	m_pSensorManager(nullptr),
	m_pSensorEventQueue(nullptr),
	m_pAccelerometerSensor(nullptr),
	m_pMagneticFieldSensor(nullptr),
	m_pGyroscopeSensor(nullptr),
	m_pLightSensor(nullptr),
	m_pProximitySensor(nullptr)
{
	// Destroy device implementation automatically
	m_bDelete = true;

	// [TODO] Sensors are working, but it's no good idea to enable all available sensors by default with a 60 events per second rate for all sensors
	// -> The device stops responding, so, rethink this very first experiment... maybe one device per sensor which is deactivated by default and has the
	//    option to be enabled/disabled + event rate configuration?

	// Get the Android sensor manager instance
	m_pSensorManager = ASensorManager_getInstance();
	if (m_pSensorManager) {
		// Create sensor event queue instance
		m_pSensorEventQueue = ASensorManager_createEventQueue(m_pSensorManager, ALooper_forThread(), LOOPER_ID_USER, nullptr, nullptr);
/*
		// Get accelerometer sensor instance
		m_pAccelerometerSensor = ASensorManager_getDefaultSensor(m_pSensorManager, ASENSOR_TYPE_ACCELEROMETER);
		if (m_pAccelerometerSensor) {
			// Write some debug information into the log
			PL_LOG(Debug, String("Android sensor manager accelerometer sensor instance found: Name=\"") + ASensor_getName(m_pAccelerometerSensor) +
																						  "\" Vendor=\"" + ASensor_getVendor(m_pAccelerometerSensor) +
																						  "\" Resolution=\"" + ASensor_getResolution(m_pAccelerometerSensor) +
																						  "\" Minimum delay=\"" + ASensor_getMinDelay(m_pAccelerometerSensor) + '\"')

			// Start monitoring the accelerometer
			if (ASensorEventQueue_enableSensor(m_pSensorEventQueue, m_pAccelerometerSensor) >= 0) {
				// We'd like to get 60 events per second (in us)
				ASensorEventQueue_setEventRate(m_pSensorEventQueue, m_pAccelerometerSensor, (1000L/60)*1000);
			}
		}

		// Get magnetic field sensor instance
		m_pMagneticFieldSensor = ASensorManager_getDefaultSensor(m_pSensorManager, ASENSOR_TYPE_MAGNETIC_FIELD);
		if (m_pMagneticFieldSensor) {
			// Write some debug information into the log
			PL_LOG(Debug, String("Android sensor manager magnetic field sensor instance found: Name=\"") + ASensor_getName(m_pMagneticFieldSensor) +
																						   "\" Vendor=\"" + ASensor_getVendor(m_pMagneticFieldSensor) +
																						   "\" Resolution=\"" + ASensor_getResolution(m_pMagneticFieldSensor) +
																						   "\" Minimum delay=\"" + ASensor_getMinDelay(m_pAccelerometerSensor) + '\"')

			// Start monitoring the accelerometer
			if (ASensorEventQueue_enableSensor(m_pSensorEventQueue, m_pMagneticFieldSensor) >= 0) {
				// We'd like to get 60 events per second (in us)
				ASensorEventQueue_setEventRate(m_pSensorEventQueue, m_pMagneticFieldSensor, (1000L/60)*1000);
			}
		}

		// Get gyroscope sensor instance
		m_pGyroscopeSensor = ASensorManager_getDefaultSensor(m_pSensorManager, ASENSOR_TYPE_GYROSCOPE);
		if (m_pGyroscopeSensor) {
			// Write some debug information into the log
			PL_LOG(Debug, String("Android sensor manager gyroscope sensor instance found: Name=\"") + ASensor_getName(m_pGyroscopeSensor) +
																					  "\" Vendor=\"" + ASensor_getVendor(m_pGyroscopeSensor) +
																					  "\" Resolution=\"" + ASensor_getResolution(m_pGyroscopeSensor) +
																					  "\" Minimum delay=\"" + ASensor_getMinDelay(m_pGyroscopeSensor) + '\"')

			// Start monitoring the accelerometer
			if (ASensorEventQueue_enableSensor(m_pSensorEventQueue, m_pGyroscopeSensor) >= 0) {
				// We'd like to get 60 events per second (in us)
				ASensorEventQueue_setEventRate(m_pSensorEventQueue, m_pGyroscopeSensor, (1000L/60)*1000);
			}
		}

		// Get light sensor instance
		m_pLightSensor = ASensorManager_getDefaultSensor(m_pSensorManager, ASENSOR_TYPE_LIGHT);
		if (m_pLightSensor) {
			// Write some debug information into the log
			PL_LOG(Debug, String("Android sensor manager light sensor instance found: Name=\"") + ASensor_getName(m_pLightSensor) +
																				  "\" Vendor=\"" + ASensor_getVendor(m_pLightSensor) +
																				  "\" Resolution=\"" + ASensor_getResolution(m_pLightSensor) +
																				  "\" Minimum delay=\"" + ASensor_getMinDelay(m_pLightSensor) + '\"')

			// Start monitoring the accelerometer
			if (ASensorEventQueue_enableSensor(m_pSensorEventQueue, m_pLightSensor) >= 0) {
				// We'd like to get 60 events per second (in us)
				ASensorEventQueue_setEventRate(m_pSensorEventQueue, m_pLightSensor, (1000L/60)*1000);
			}
		}

		// Get proximity sensor instance
		m_pProximitySensor = ASensorManager_getDefaultSensor(m_pSensorManager, ASENSOR_TYPE_PROXIMITY);
		if (m_pProximitySensor) {
			// Write some debug information into the log
			PL_LOG(Debug, String("Android sensor manager proximity sensor instance found: Name=\"") + ASensor_getName(m_pProximitySensor) +
																					  "\" Vendor=\"" + ASensor_getVendor(m_pProximitySensor) +
																					  "\" Resolution=\"" + ASensor_getResolution(m_pProximitySensor) +
																					  "\" Minimum delay=\"" + ASensor_getMinDelay(m_pProximitySensor) + '\"')

			// Start monitoring the accelerometer
			if (ASensorEventQueue_enableSensor(m_pSensorEventQueue, m_pProximitySensor) >= 0) {
				// We'd like to get 60 events per second (in us)
				ASensorEventQueue_setEventRate(m_pSensorEventQueue, m_pProximitySensor, (1000L/60)*1000);
			}
		}
		*/
	}
}

/**
*  @brief
*    Destructor
*/
AndroidSensorManagerDevice::~AndroidSensorManagerDevice()
{
	// Destroy sensor event queue instance
	ASensorManager_destroyEventQueue(m_pSensorManager, m_pSensorEventQueue);
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void AndroidSensorManagerDevice::Update()
{
	// Check if input device and event queue are valid
	if (m_pDevice && m_pSensorEventQueue) {
		// Get sensor manager device
		SensorManager *pSensorManager = static_cast<SensorManager*>(m_pDevice);

		// Process sensor event queue instance
		ASensorEvent sASensorEvent;
		while (ASensorEventQueue_getEvents(m_pSensorEventQueue, &sASensorEvent, 1) > 0) {
			// Check the sensor type
			switch (sASensorEvent.type) {
				// Accelerometer
				case ASENSOR_TYPE_ACCELEROMETER:
					if (pSensorManager->AccelerationX.GetValue() != sASensorEvent.acceleration.x)
						pSensorManager->AccelerationX.SetValue(sASensorEvent.acceleration.x, true);
					if (pSensorManager->AccelerationY.GetValue() != sASensorEvent.acceleration.y)
						pSensorManager->AccelerationY.SetValue(sASensorEvent.acceleration.y, true);
					if (pSensorManager->AccelerationZ.GetValue() != sASensorEvent.acceleration.z)
						pSensorManager->AccelerationZ.SetValue(sASensorEvent.acceleration.z, true);
					break;

				// Magnetic field
				case ASENSOR_TYPE_MAGNETIC_FIELD:
					if (pSensorManager->MagneticX.GetValue() != sASensorEvent.magnetic.x)
						pSensorManager->MagneticX.SetValue(sASensorEvent.magnetic.x, true);
					if (pSensorManager->MagneticY.GetValue() != sASensorEvent.magnetic.y)
						pSensorManager->MagneticY.SetValue(sASensorEvent.magnetic.y, true);
					if (pSensorManager->MagneticZ.GetValue() != sASensorEvent.magnetic.z)
						pSensorManager->MagneticZ.SetValue(sASensorEvent.magnetic.z, true);
					break;

				// Gyroscope
				case ASENSOR_TYPE_GYROSCOPE:
					if (pSensorManager->RotationX.GetValue() != sASensorEvent.vector.x)
						pSensorManager->RotationX.SetValue(sASensorEvent.vector.x, true);
					if (pSensorManager->RotationY.GetValue() != sASensorEvent.vector.y)
						pSensorManager->RotationY.SetValue(sASensorEvent.vector.y, true);
					if (pSensorManager->RotationZ.GetValue() != sASensorEvent.vector.z)
						pSensorManager->RotationZ.SetValue(sASensorEvent.vector.z, true);
					break;

				// Light
				case ASENSOR_TYPE_LIGHT:
					if (pSensorManager->Light.GetValue() != sASensorEvent.light)
						pSensorManager->Light.SetValue(sASensorEvent.light, true);
					break;

				// Proximity
				case ASENSOR_TYPE_PROXIMITY:
					if (pSensorManager->Proximity.GetValue() != sASensorEvent.distance)
						pSensorManager->Proximity.SetValue(sASensorEvent.distance, true);
					break;
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
