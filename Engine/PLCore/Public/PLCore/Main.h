/*********************************************************\
 *  File: Main.h                                         *
 *      Platform independent main function. This header can ONLY be includes once per project!
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


#ifndef __PLCORE_MAIN_H__
#define __PLCORE_MAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/System/System.h"
#include "PLCore/Tools/CommandLine.h"


//[-------------------------------------------------------]
//[ OS definitions                                        ]
//[-------------------------------------------------------]
// Windows platform
#ifdef WIN32
	#include "PLCore/+Windows/PLCoreWindowsIncludes.h"
#endif

// Linux platform
#ifdef LINUX
	#include <locale.h>
	#include "PLCore/+Linux/PLCoreLinuxIncludes.h"
#endif


//[-------------------------------------------------------]
//[ PL main function                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Program entry point
*
*  @param[in] sExecutableFilename
*    Absolute executable filename
*  @param[in] lstArguments
*    List of program arguments
*
*  @return
*    Exit code of the application (usually 0 means no error)
*/
int PLMain(const PLCore::String &sExecutableFilename, const PLCore::Array<PLCore::String> &lstArguments);


//[-------------------------------------------------------]
//[ Program entry point                                   ]
//[-------------------------------------------------------]
// Do only include the program entry point if the current build target is no shared library
// (On e.g. Linux those definitions are set by our build system -> we just use the MS Windows
//  names so that we don't have to invent a new definition for this purpose)
#if !defined(_WINDLL) && !defined(_USRDLL)
	// Windows implementation
	#ifdef WIN32
		#ifdef _CONSOLE
			#ifdef UNICODE
				int wmain(int argc, wchar_t **argv)
			#else
				int main(int argc, char **argv)
			#endif
				{
					PLCore::Array<PLCore::String> lstArguments;
					for (int i=1; i<argc; i++)
						lstArguments.Add(argv[i]);
					return PLMain(PLCore::System::GetInstance()->GetExecutableFilename(), lstArguments);
				}
		#else
			#ifdef UNICODE
				int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR pszCmdLine, int nShow)
			#else
				int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pszCmdLine, int nShow)
			#endif
				{
					return PLMain(PLCore::System::GetInstance()->GetExecutableFilename(), PLCore::CommandLine::StringToArguments(pszCmdLine));
				}
		#endif

	// Linux implementation
	#elif LINUX
		int main(int argc, char **argv)
		{
			PLCore::Array<PLCore::String> lstArguments;
			for (int i=1; i<argc; i++)
				lstArguments.Add(PLCore::String::FromUTF8(argv[i]));
			return PLMain(PLCore::System::GetInstance()->GetExecutableFilename(), lstArguments);
		}
	#endif
#endif


#endif // __PLCORE_MAIN_H__
