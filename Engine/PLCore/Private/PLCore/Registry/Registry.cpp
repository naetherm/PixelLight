/*********************************************************\
 *  File: Registry.cpp                                   *
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
#if defined(WIN32)
	#include "PLCore/Registry/+Windows/RegistryWindows.h"
#endif
#include "PLCore/Registry/Registry.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Registry::Registry() :
	m_pRegistryImpl(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pRegistryImpl = new RegistryWindows();
	#endif
}

/**
*  @brief
*    Constructor
*/
Registry::Registry(EKey nKey, const String &sSubKey, uint32 nAccess) :
	m_pRegistryImpl(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pRegistryImpl = new RegistryWindows();
	#endif

	// Open registry key
	if (m_pRegistryImpl)
		m_pRegistryImpl->Open(nKey, sSubKey, nAccess);
}

/**
*  @brief
*    Copy constructor
*/
Registry::Registry(const Registry &cRegistry) :
	m_pRegistryImpl(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pRegistryImpl = new RegistryWindows();
	#endif

	// Open the same key again
	if (m_pRegistryImpl)
		Open(cRegistry.GetOpenKey(), cRegistry.GetOpenSubKey(), cRegistry.GetOpenAccessMode());
}

/**
*  @brief
*    Destructor
*/
Registry::~Registry()
{
	// Delete system implementation
	if (m_pRegistryImpl)
		delete m_pRegistryImpl;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
