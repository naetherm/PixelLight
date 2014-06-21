/*********************************************************\
 *  File: AndroidSensorManagerDevice.h                   *
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


#ifndef __PLINPUT_ANDROIDSENSORMANAGERDEVICE_H__
#define __PLINPUT_ANDROIDSENSORMANAGERDEVICE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Backend/UpdateDevice.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct ASensorManager;
typedef struct ASensorManager ASensorManager;
struct ASensorEventQueue;
typedef struct ASensorEventQueue ASensorEventQueue;
struct ASensor;
typedef struct ASensor ASensor;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sensor manager implementation for Android
*/
class AndroidSensorManagerDevice : public UpdateDevice {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AndroidSensorManagerDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AndroidSensorManagerDevice();


	//[-------------------------------------------------------]
	//[ Public virtual UpdateDevice functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual void Update() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ASensorManager    *m_pSensorManager;		/**< Android sensor manager, can be a null pointer */
		ASensorEventQueue *m_pSensorEventQueue;		/**< Sensor event queue, can be a null pointer */
		const ASensor     *m_pAccelerometerSensor;	/**< Accelerometer sensor, can be a null pointer */
		const ASensor     *m_pMagneticFieldSensor;	/**< Magnetic field sensor, can be a null pointer */
		const ASensor     *m_pGyroscopeSensor;		/**< Gyroscope sensor, can be a null pointer */
		const ASensor     *m_pLightSensor;			/**< Light sensor, can be a null pointer */
		const ASensor     *m_pProximitySensor;		/**< Proximity sensor, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_ANDROIDSENSORMANAGERDEVICE_H__
