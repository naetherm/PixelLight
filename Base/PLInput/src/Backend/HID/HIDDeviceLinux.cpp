/*********************************************************\
 *  File: HIDDeviceLinux.cpp                             *
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
#include "PLInput/Backend/HID/HIDDeviceLinux.h"


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
HIDDeviceLinux::HIDDeviceLinux()
{
	// Do not destroy device implementation automatically, because this is managed by HIDLinux
	m_bDelete = false;
}

/**
*  @brief
*    Destructor
*/
HIDDeviceLinux::~HIDDeviceLinux()
{
	// Close a possibly open connection
	Close();
}


//[-------------------------------------------------------]
//[ Public virtual ConnectionDevice functions             ]
//[-------------------------------------------------------]
bool HIDDeviceLinux::Open(uint16 nOutputPort, uint16 nInputPort)
{
	// Close first
	if (IsOpen())
		Close();

	// [TODO]
	/*
	// Connect

	// Start read thread
	InitThread();

	// Call event
	EventOnConnect();

	// Done
	return true;
	*/

	// Error
	return false;
}

bool HIDDeviceLinux::Close()
{
	if (IsOpen()) {
		// [TODO]
		/*
		// Stop read thread
		StopThread();

		// Call event
		EventOnDisconnect();

		// Disconnect

		// Done
		return true;
		*/
	}

	// Error
	return false;
}

bool HIDDeviceLinux::IsOpen() const
{
	// [TODO]
	return false;
}

bool HIDDeviceLinux::Read(uint8 *pBuffer, uint32 nSize)
{
	// [TODO]

	// Error!
	return false;
}

bool HIDDeviceLinux::Write(const uint8 *pBuffer, uint32 nSize)
{
	// [TODO]

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
