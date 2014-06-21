/*********************************************************\
 *  File: BluetoothProvider.cpp                          *
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
#include <PLInput/Input/Devices/WiiMote.h>
#include "PLInput/Backend/Bluetooth/Bluetooth.h"
#include "PLInput/Backend/Bluetooth/BTDevice.h"
#include "PLInput/Backend/Bluetooth/BluetoothProvider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(BluetoothProvider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
BluetoothProvider::BluetoothProvider()
{
}

/**
*  @brief
*    Destructor
*/
BluetoothProvider::~BluetoothProvider()
{
}


//[-------------------------------------------------------]
//[ Private virtual Provider functions                    ]
//[-------------------------------------------------------]
void BluetoothProvider::QueryDevices()
{
	// Device counter
	int nWiiMote = 0;

	// Get list of Bluetooth-devices
	const List<BTDevice*> &lstDevices = Bluetooth::GetInstance()->GetDevices();
	for (uint32 i=0; i<lstDevices.GetNumOfElements(); i++) {
		// Get device
		BTDevice *pDevice = lstDevices[i];

		// Check device type
		if ( pDevice->GetName() == "Nintendo RVL-CNT-01" ||
			(pDevice->GetClass(0) == WiiMote::DeviceClass0 &&
			 pDevice->GetClass(1) == WiiMote::DeviceClass1 &&
			 pDevice->GetClass(2) == WiiMote::DeviceClass2) ) {
			// WiiMote
			String sName = String("WiiMote") + nWiiMote;
			nWiiMote++;
			if (!CheckDevice(sName))
				AddDevice(sName, new WiiMote(sName, pDevice));
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
BluetoothProvider::BluetoothProvider(const BluetoothProvider &cSource)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Copy operator
*/
BluetoothProvider &BluetoothProvider::operator =(const BluetoothProvider &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
