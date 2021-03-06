/*********************************************************\
 *  File: Application.cpp                                *
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
#include <PLCore/Runtime.h>
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include "Application.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application, "", PLCore::CoreApplication, "Application class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor, "Default constructor", "")
pl_class_metadata_end(Application)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application::Application() : CoreApplication()
{
}

/**
*  @brief
*    Destructor
*/
Application::~Application()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application::Main()
{
	// System directories
	System::GetInstance()->GetConsole().Print("System directories:\n");
	System::GetInstance()->GetConsole().Print("- Executable:    " + System::GetInstance()->GetExecutableFilename() + '\n');
	System::GetInstance()->GetConsole().Print("- Home:          " + System::GetInstance()->GetUserHomeDir() + '\n');
	System::GetInstance()->GetConsole().Print("- Data:          " + System::GetInstance()->GetUserDataDir() + '\n');
	System::GetInstance()->GetConsole().Print("- Current:       " + System::GetInstance()->GetCurrentDir() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// PixelLight runtime directories (usually you only need "Runtime::GetDirectory()" and "Runtime::GetDirectoryData()")
	System::GetInstance()->GetConsole().Print("PixelLight runtime directories:\n");
	System::GetInstance()->GetConsole().Print("- Local:         " + Runtime::GetLocalDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- Local data:    " + Runtime::GetLocalDataDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- System:        " + Runtime::GetSystemDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- System data:   " + Runtime::GetSystemDataDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- Registry:      " + Runtime::GetRegistryDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- Registry data: " + Runtime::GetRegistryDataDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- Runtime:       " + Runtime::GetDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- Runtime data:  " + Runtime::GetDataDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- PLCore:        " + Runtime::GetPLCoreSharedLibraryDirectory() + '\n');
	System::GetInstance()->GetConsole().Print('\n');

	// Application context
	System::GetInstance()->GetConsole().Print("Application context:\n");
	System::GetInstance()->GetConsole().Print("- Executable:    " + GetApplicationContext().GetExecutableFilename() + '\n');
	System::GetInstance()->GetConsole().Print("- Startup:       " + GetApplicationContext().GetStartupDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- AppDir:        " + GetApplicationContext().GetAppDirectory() + '\n');
	System::GetInstance()->GetConsole().Print("- Log:           " + GetApplicationContext().GetLogFilename() + '\n');
	System::GetInstance()->GetConsole().Print("- Config:        " + GetApplicationContext().GetConfigFilename() + '\n');
	System::GetInstance()->GetConsole().Print('\n');
}
