/*********************************************************\
 *  File: HID.cpp                                        *
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
#include "PLInput/Backend/HID/HID.h"
#include "PLInput/Backend/HID/HIDImpl.h"
#if defined(WIN32)
	#include "PLInput/Backend/HID/+Windows/HIDWindows.h"
#elif defined(LINUX)
	#include "PLInput/Backend/HID/+Linux/HIDLinux.h"
#endif


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
namespace PLCore {
	template class Singleton<PLInput::HID>;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
HID *HID::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<HID>::GetInstance();
}

bool HID::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<HID>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Detect available HID devices
*/
void HID::DetectDevices()
{
	// Clear list
	Clear();

	// Check HID implementation
	if (m_pHIDImpl) {
		// Enumerate devices
		m_pHIDImpl->EnumerateDevices(m_lstDevices);
	}
}

/**
*  @brief
*    Get list of available HID devices
*/
const List<HIDDevice*> &HID::GetDevices() const
{
	// Return list
	return m_lstDevices;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
HID::HID() :
	// Create HID implementation
	#if defined(WIN32)
		m_pHIDImpl(new HIDWindows())
	#elif defined(LINUX)
		m_pHIDImpl(new HIDLinux())
	#endif
{
	// Detect devices
	DetectDevices();
}

/**
*  @brief
*    Destructor
*/
HID::~HID()
{
	// Clear devices
	Clear();

	// Delete HID implementation
	if (m_pHIDImpl)
		delete m_pHIDImpl;
}

/**
*  @brief
*    Remove all devices
*/
void HID::Clear()
{
	// Destroy devices
	m_lstDevices.Clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
