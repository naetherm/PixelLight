/*********************************************************\
 *  File: Bluetooth.cpp                                  *
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
#include "PLInput/Backend/Bluetooth/Bluetooth.h"
#include "PLInput/Backend/Bluetooth/BTImpl.h"
#if defined(LINUX) && !defined(ANDROID) && !defined(APPLE)
	#include "PLInput/Backend/Bluetooth/BTLinux.h"
#endif


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
namespace PLCore {
	template class Singleton<PLInput::Bluetooth>;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
Bluetooth *Bluetooth::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Bluetooth>::GetInstance();
}

bool Bluetooth::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Bluetooth>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Detect available Bluetooth devices
*/
void Bluetooth::DetectDevices()
{
	// Clear list
	Clear();

	// Check Bluetooth implementation
	if (m_pBTImpl) {
		// Enumerate devices
		m_pBTImpl->EnumerateDevices(m_lstDevices);
	}
}

/**
*  @brief
*    Get list of available Bluetooth devices
*/
const List<BTDevice*> &Bluetooth::GetDevices() const
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
Bluetooth::Bluetooth() :
	// Create Bluetooth implementation
	#if defined(LINUX) && !defined(ANDROID) && !defined(APPLE)
		m_pBTImpl(new BTLinux())
	#else
		m_pBTImpl(nullptr)
	#endif
{
	// Detect devices
	DetectDevices();
}

/**
*  @brief
*    Destructor
*/
Bluetooth::~Bluetooth()
{
	// Clear devices
	Clear();

	// Delete Bluetooth implementation
	if (m_pBTImpl)
		delete m_pBTImpl;
}

/**
*  @brief
*    Remove all devices
*/
void Bluetooth::Clear()
{
	// Destroy devices
	m_lstDevices.Clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
