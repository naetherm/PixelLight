/*********************************************************\
 *  File: SystemMacOSX.cpp                               *
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
#include <mach-o/dyld.h>	// For _NSGetExecutablePath
#include "PLCore/System/SystemMacOSX.h"


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
SystemMacOSX::SystemMacOSX()
{
}

/**
*  @brief
*    Destructor
*/
SystemMacOSX::~SystemMacOSX()
{
}


//[-------------------------------------------------------]
//[ Private virtual SystemImpl functions                  ]
//[-------------------------------------------------------]
String SystemMacOSX::GetPlatform() const
{
	static const String sString = "MacOSX";
	return sString;
}

String SystemMacOSX::GetExecutableFilename() const
{
	// First of all, get the length of the executable filename (including the terminating zero)
	uint32_t nSize = 0;
	_NSGetExecutablePath(nullptr, &nSize);
	if (nSize) {
		// Executable filename
		char *pszTemp = new char[nSize];
		if (!_NSGetExecutablePath(pszTemp, &nSize)) {
			// Get the string
			const String sString = String::FromUTF8(pszTemp);

			// We no longer need the data, so free it
			delete [] pszTemp;

			// Done
			return sString;
		} else {
			// We no longer need the data, so free it
			delete [] pszTemp;
		}
	}

	// Error!
	return "";
}

String SystemMacOSX::GetSharedLibraryExtension() const
{
	static const String sString = "dylib";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
