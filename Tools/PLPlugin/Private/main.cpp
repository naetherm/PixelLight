/*********************************************************\
*  File: main.cpp                                       *
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
#include <PLCore/Main.h>
#include <PLCore/Runtime.h>
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/File/Url.h>

#include "PLPluginInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Program entry point                                   ]
//[-------------------------------------------------------]
int PLMain(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// Output startup info
	System::GetInstance()->GetConsole().Print("Starting PLPlugin utility (" + Runtime::GetVersion().ToString() + ")\n");

	// Parse command line
	String sOutputPath;
	String sModulePath;
	bool bError = false;

	for (uint32 i = 0; i < lstArguments.GetNumOfElements(); ++i)
	{
		String sArg = lstArguments[i];
		if (sArg == "--output-path")
		{
			if (i + 1 >= lstArguments.GetNumOfElements())
			{
				System::GetInstance()->GetConsole().Print("!!! The argument --output-path used without parameter, aborting");
				bError = true;
				break;
			}

			sOutputPath = lstArguments[++i];

			// Trim trailing slash, if any
			const uint32 nLastCharacter = sOutputPath.GetLength() - 1;
			if (sOutputPath[nLastCharacter] == '\\' || sOutputPath[nLastCharacter] == '/')
				sOutputPath.Delete(nLastCharacter);
		}
		else if (sArg == "--module-path")
		{
			if (i + 1 >= lstArguments.GetNumOfElements())
			{
				System::GetInstance()->GetConsole().Print("!!! The argument --module-path used without parameter, aborting");
				bError = true;
				break;
			}

			sModulePath = lstArguments[++i];

			// Trim trailing slash, if any
			const uint32 nLastCharacter = sModulePath.GetLength() - 1;
			if (sModulePath[nLastCharacter] == '\\' || sModulePath[nLastCharacter] == '/')
				sModulePath.Delete(nLastCharacter);
		}
	}

	if (!bError)
	{
		// Create the plugin info descriptor
		PLPluginInfo info;
		info.SetPluginFileVersion("1");
		info.SetPLVersion(Runtime::GetVersion().ToString());

		info.SetActive(true); //???
		info.SetDelayed(true); //???
		
		// [TEMP] We are not using any suffixes for now
		info.SetLibrarySuffix("");

		// Load plugin info from the module
		if (info.CreateFromModule(sModulePath))
		{
			// Save the plugin file
			String sModuleFile = Url(sModulePath).GetCompleteTitle();
			sModuleFile += ".plugin";
			info.Save(sOutputPath + "/" + sModuleFile);

			System::GetInstance()->GetConsole().Print("--> Written output plugin file into " + sModuleFile);

			// Done 
			return 0;
		}
	}

	// Error
	return 1;
}
