/*********************************************************\
 *  File: DynLibWindows.cpp                              *
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
#include "PLCore/File/Url.h"
#include "PLCore/System/+Windows/SystemWindows.h"
#include "PLCore/System/+Windows/DynLibWindows.h"


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
DynLibWindows::DynLibWindows() :
	m_hModule(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
DynLibWindows::~DynLibWindows()
{
	// Unload the library
	Unload();
}


//[-------------------------------------------------------]
//[ Private virtual DynLibImpl functions                  ]
//[-------------------------------------------------------]
bool DynLibWindows::IsLoaded() const
{
	// Return whether the library has been loaded
	return (m_hModule != nullptr);
}

bool DynLibWindows::Load(const Url &cUrl)
{
	// Is already a library loaded?
	if (m_hModule)
		return false; // Error!

	// Load library
	const String sWindowsPath = cUrl.GetWindowsPath();
	if (sWindowsPath.GetFormat() == String::ASCII)
		m_hModule = ::LoadLibraryExA(sWindowsPath.GetASCII(),   nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
	else
		m_hModule = ::LoadLibraryExW(sWindowsPath.GetUnicode(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

	// Return whether loading of the library was successful
	if (m_hModule) {
		// Done
		return true;
	} else {
		// Error: Failed to load dll!
		return false;
	}
}

String DynLibWindows::GetAbsPath() const
{
	// Allocate a buffer that is large enough to hold the information
	wchar_t *pszBuffer = new wchar_t[MAX_PATH];

	// Get module filename as Unicode
	if (GetModuleFileNameW(m_hModule, pszBuffer, MAX_PATH)) {
		return String(pszBuffer, false); // Do not copy, please
	} else {
		// We no longer need the data, so free it
		delete [] pszBuffer;

		// Error
		return "";
	}
}

bool DynLibWindows::Unload()
{
	if (m_hModule != nullptr) {
		// Unload library
		if (::FreeLibrary(m_hModule)) {
			m_hModule = nullptr;

			// Done
			return true;
		}
	}

	// Error, could not unload the library
	return false;
}

void *DynLibWindows::GetSymbol(const String &sSymbol) const
{
	// Is the library loaded? ... it looks like as if there's only a ASCII version of "GetProcAddress()"...
	return m_hModule ? ::GetProcAddress(m_hModule, sSymbol.GetASCII()) : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
