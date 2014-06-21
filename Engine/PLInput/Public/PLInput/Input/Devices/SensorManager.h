/*********************************************************\
 *  File: SensorManager.h                                *
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


#ifndef __PLINPUT_SENSORMANAGER_H__
#define __PLINPUT_SENSORMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Input/Devices/Device.h"
#include "PLInput/Input/Controls/Axis.h"
#include "PLInput/Input/Controls/Button.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sensor manager input device
*
*  @remarks
*    The sensor manager is a collection of sensors usually available on a mobile device:
*    - Accelerometer
*    - Magnetic field
*    - Gyroscope
*    - Light
*    - Proximity
*
*    This class supports the following device backend types:
*    - UpdateDevice
*/
class SensorManager : public Device {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, SensorManager, "PLInput", PLInput::Device, "Sensor manager input controller")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Controller definition                                 ]
	//[-------------------------------------------------------]
	public:
		// Accelerometer
		PLInput::Axis	AccelerationX;	/**< X acceleration axis (Accelerometer) */
		PLInput::Axis	AccelerationY;	/**< Y acceleration axis (Accelerometer) */
		PLInput::Axis	AccelerationZ;	/**< Z acceleration axis (Accelerometer) */
		// Magnetic field
		PLInput::Axis	MagneticX;		/**< X magnetic axis (Magnetic field) */
		PLInput::Axis	MagneticY;		/**< Y magnetic axis (Magnetic field) */
		PLInput::Axis	MagneticZ;		/**< Z magnetic axis (Magnetic field) */
		// Gyroscope
		PLInput::Axis	RotationX;		/**< X rotation axis (Gyroscope) */
		PLInput::Axis	RotationY;		/**< Y rotation axis (Gyroscope) */
		PLInput::Axis	RotationZ;		/**< Z rotation axis (Gyroscope) */
		// Light
		PLInput::Axis	Light;			/**< Light */
		// Proximity
		PLInput::Axis	Proximity;		/**< Proximity */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Device name
		*  @param[in] pImpl
		*    System specific device implementation, can, but shouldn't be a null pointer
		*/
		PLINPUT_API SensorManager(const PLCore::String &sName, DeviceImpl *pImpl);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~SensorManager();


	//[-------------------------------------------------------]
	//[ Public virtual Controller functions                   ]
	//[-------------------------------------------------------]
	public:
		PLINPUT_API virtual void Update() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_SENSORMANAGER_H__
