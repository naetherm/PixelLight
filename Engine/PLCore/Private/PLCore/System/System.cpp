/*********************************************************\
 *  File: System.cpp                                     *
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
#include "PLCore/System/Thread.h"
#if defined(WIN32)
	#include "PLCore/System/+Windows/SystemWindows.h"
#elif defined(ANDROID)
	#include "PLCore/System/+Linux/+Android/SystemAndroid.h"
#elif defined(APPLE)
	#include "PLCore/System/+Linux/+MacOSX/SystemMacOSX.h"
#elif defined(LINUX)
	#include "PLCore/System/+Linux/SystemLinux.h"
#endif
#include "PLCore/System/System.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<System>;


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
System *System::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<System>::GetInstance();
}

bool System::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<System>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns an information string about the system
*/
String System::GetInfo() const
{
	static const String sString = String("PLCore library") +
								  "\nEndian: "			+ (IsLittleEndian() ? "'Little Endian First'" : "'Big Endian First'") +
								  "\nPlatform: "		+ GetPlatform() + ' ' + GetPlatformBitArchitecture() + " bit (" + GetPlatformArchitecture() + ')' +
								  "\nOS: "				+ GetOS() +
								  '\n';
	return sString;
}

/**
*  @brief
*    Detects the current machine's endian ("byte order")
*/
bool System::IsLittleEndian() const
{
	union Probe {
		unsigned int  nNumber;
		unsigned char nBytes[sizeof(unsigned int)];
	};

	// Initialize the first member of p with unsigned 1
	Probe p = { 1u };

	// In a big endian architecture, p.nBytes[0] equals 0
	return (p.nBytes[0] == 1u);
}

/**
*  @brief
*    Returns the platform architecture
*/
String System::GetPlatformArchitecture() const
{
	#ifdef ARCHITECTURE_STRING
		// The exact architecture PLCore has been compiled for is provided as preprocessor definition
		static const String sString = ARCHITECTURE_STRING;
	#else
		// Use a fallback in case ARCHITECTURE_STRING is not given
		#if defined(WIN64) || defined(X64_ARCHITECTURE)
			static const String sString = "x64";
		#else
			static const String sString = "x86";
		#endif
	#endif
	return sString;
}

/**
*  @brief
*    Returns the platform bit architecture
*/
uint32 System::GetPlatformBitArchitecture() const
{
	#if defined(WIN64) || defined(X64_ARCHITECTURE)
		return 64;
	#else
		return 32;
	#endif
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
System::System() :
	m_pSystemImpl(nullptr),
	m_pMainThread(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pSystemImpl = new SystemWindows();
	#elif defined(ANDROID)
		// Create Android implementation
		m_pSystemImpl = new SystemAndroid();
	#elif defined(APPLE)
		// Create Mac OS X implementation
		m_pSystemImpl = new SystemMacOSX();
	#elif defined(LINUX)
		// Create Linux implementation
		m_pSystemImpl = new SystemLinux();
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif

	// Create main thread object (NULL_HANDLE for current thread)
	m_pMainThread = new Thread(NULL_HANDLE);
}

/**
*  @brief
*    Copy constructor
*/
System::System(const System &cSource) :
	m_pSystemImpl(nullptr),
	m_pMainThread(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
System::~System()
{
	// Destroy main thread object
	delete m_pMainThread;

	// Destroy system specific implementation
	if (m_pSystemImpl)
		delete m_pSystemImpl;
}

/**
*  @brief
*    Copy operator
*/
System &System::operator =(const System &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
