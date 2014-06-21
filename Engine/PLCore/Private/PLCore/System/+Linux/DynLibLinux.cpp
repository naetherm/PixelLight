/*********************************************************\
 *  File: DynLibLinux.cpp                                *
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
#if !defined(ANDROID) && !defined(APPLE)
	#include <link.h>	// For "dlinfo", not available on Android and Mac OS X
#endif
#include <dlfcn.h>
#include <stdio.h>
#include "PLCore/File/Url.h"
#include "PLCore/System/DynLibLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DynLibLinux::DynLibLinux() :
	m_pLib(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
DynLibLinux::~DynLibLinux()
{
	// Unload the library
	Unload();
}


//[-------------------------------------------------------]
//[ Private virtual DynLibImpl functions                  ]
//[-------------------------------------------------------]
bool DynLibLinux::IsLoaded() const
{
	// Return whether the library has been loaded
	return (m_pLib != nullptr);
}

bool DynLibLinux::Load(const Url &cUrl)
{
	// Is already a library loaded?
	if (m_pLib)
		return false; // Error!

	// Load library. Under Linux, there's nothing like 'LOAD_WITH_ALTERED_SEARCH_PATH' from MS Windows so the other files
	// will be searched within the same directory as this one. This has to be configured by using the RPATH when linking
	// the resulting binary (see http://www.cmake.org/Wiki/CMake_RPATH_handling). Type e.g. "objdump -x libPLCore.so" and
	// have a look at "Dynamic Section"->"RPATH", should be "$ORIGIN" when this option is enabled.
	const String sPath = cUrl.GetUnixPath();
	m_pLib = dlopen((sPath.GetFormat() == String::ASCII) ? sPath.GetASCII() : sPath.GetUTF8(), RTLD_LAZY);

	// Return whether loading of the library was successful
	if (m_pLib) {
		// Done
		return true;
	} else {
		// Error: Failed to load dll!
		return false;
	}
}

String DynLibLinux::GetAbsPath() const
{
	#if !defined(ANDROID) && !defined(APPLE)
		// "dlinfo" is not available on Android and Mac OS X
		link_map *pLinkMap = nullptr;
		if (m_pLib && !dlinfo(m_pLib, RTLD_DI_LINKMAP, &pLinkMap)) {
			// Success
			return String::FromUTF8(pLinkMap->l_name);
		}
	#endif

	// Error!
	return "";
}

bool DynLibLinux::Unload()
{
	if (m_pLib) {
		// Unload library
		if (!dlclose(m_pLib)) {
			m_pLib = nullptr;

			// Done
			return true;
		}
	}

	// Error, could not unload the library
	return false;
}

void *DynLibLinux::GetSymbol(const String &sSymbol) const
{
	// Is the library loaded?
	return m_pLib ? dlsym(m_pLib, (sSymbol.GetFormat() == String::ASCII) ? sSymbol.GetASCII() : sSymbol.GetUTF8()) : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
