/*********************************************************\
 *  File: SensorManager.cpp                              *
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
#include "PLInput/Backend/UpdateDevice.h"
#include "PLInput/Input/Devices/SensorManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(SensorManager)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SensorManager::SensorManager(const String &sName, DeviceImpl *pImpl) : Device(sName, "Sensor manager input controller", pImpl),
	// Accelerometer
	AccelerationX	(this,	"AccelerationX",	"X acceleration axis (Accelerometer)"),
	AccelerationY	(this,	"AccelerationY",	"Y acceleration axis (Accelerometer)"),
	AccelerationZ	(this,	"AccelerationZ",	"Z acceleration axis (Accelerometer)"),
	// Magnetic field
	MagneticX		(this,	"MagneticX",		"X magnetic axis (Magnetic field)"),
	MagneticY		(this,	"MagneticY",		"Y magnetic axis (Magnetic field)"),
	MagneticZ		(this,	"MagneticZ",		"Z magnetic axis (Magnetic field)"),
	// Gyroscope
	RotationX		(this,	"RotationX",		"X rotation axis (Gyroscope)"),
	RotationY		(this,	"RotationY",		"Y rotation axis (Gyroscope)"),
	RotationZ		(this,	"RotationZ",		"Z rotation axis (Gyroscope)"),
	// Light
	Light			(this,	"Light",			"Light"),
	// Proximity
	Proximity		(this,	"Proximity",		"Proximity")
{
}

/**
*  @brief
*    Destructor
*/
SensorManager::~SensorManager()
{
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void SensorManager::Update()
{
	// Update device backend
	if (m_pImpl && m_pImpl->GetBackendType() == BackendUpdateDevice) {
		static_cast<UpdateDevice*>(m_pImpl)->Update();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
