/*********************************************************\
 *  File: Application44.cpp                              *
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
#include <PLCore/Log/Log.h>
#include <PLCore/File/Url.h>
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/Tools/LoadableManager.h>
#include <PLCore/Script/Script.h>
#include <PLCore/Script/FuncScriptPtr.h>
#include <PLCore/Script/ScriptManager.h>
#include "MyRTTIClass.h"
#include "Application44.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application44, "", PLCore::FrontendApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
	// Slots
	pl_slot_1_metadata(OnMySignal,	PLCore::String,	"Called on MySignal signal, a string as first parameter",	"")
pl_class_metadata_end(Application44)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application44::Application44(Frontend &cFrontend) : FrontendApplication(cFrontend),
	SlotOnMySignal(this),
	m_pMyRTTIClass(new MyRTTIClass())
{
	// Connect event handler
	m_pMyRTTIClass->MySignal.Connect(SlotOnMySignal);

	// Bring the log into the verbose mode so that the log also writes log entries
	// directly into the console. This way, we can e.g. see script errors at once.
	Log::GetInstance()->SetVerbose(true);

	// Do only show us error messages within the log
	Log::GetInstance()->SetLogLevel(Log::Error);
}

/**
*  @brief
*    Destructor
*/
Application44::~Application44()
{
	// Destroy the RTTI object instance
	delete m_pMyRTTIClass;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Runs a script
*/
void Application44::RunScript(const String &sScriptFilename)
{
	// Get the script source code
	const String sSourceCode = LoadableManager::GetInstance()->LoadStringFromFile(sScriptFilename);
	if (sSourceCode.GetLength()) {
		// Create the script instance by using the extension of the given filename to detect the script language
		Script *pScript = ScriptManager::GetInstance()->Create(ScriptManager::GetInstance()->GetScriptLanguageByExtension(Url(sScriptFilename).GetExtension()));
		if (pScript) {
			// Tell the script about "Application44::GetMyRTTIClassInstance"
			Functor<MyRTTIClass*, MyRTTIClass*> getInstanceMethod(&Application44::GetMyRTTIClassInstance, this);
			pScript->AddGlobalFunction("GetMyRTTIClassInstance", FunctorWrapper< MyRTTIClass*, MyRTTIClass* >(&getInstanceMethod));

			// Set the script source code
			if (pScript->SetSourceCode(sSourceCode)) {
				// Print the name of the used script language
				System::GetInstance()->GetConsole().Print("-- " + pScript->GetScriptLanguage() + " script language --\n");

				// Call the script function "OOP"
				FuncScriptPtr<void> ooPMethod(pScript, "OOP");
				FuncWrapper<void>(&ooPMethod).Call(Params<void>());

				{ // Call the script function "UseCppRTTIObject"
					Params<MyRTTIClass*, MyRTTIClass*> cParams(m_pMyRTTIClass);
					FuncScriptPtr<MyRTTIClass*, MyRTTIClass*> useCppRTTIObjectMethod(pScript, "UseCppRTTIObject");
					FuncWrapper<MyRTTIClass*, MyRTTIClass*>(&useCppRTTIObjectMethod).Call(cParams);
					if (cParams.Return != m_pMyRTTIClass)
						System::GetInstance()->GetConsole().Print("Error, script returned invalid RTTI object instance!\n");
				}

				// Cleanup
				delete pScript;

				// Print new line
				System::GetInstance()->GetConsole().Print("--\n");
			} else {
				// Error!
				System::GetInstance()->GetConsole().Print("Failed to use the script source code \"" + sScriptFilename + "\"\n");
			}
		}
	} else {
		// Error!
		System::GetInstance()->GetConsole().Print("Failed to load the script \"" + sScriptFilename + "\"\n");
	}

	// Print new line
	System::GetInstance()->GetConsole().Print('\n');
}

/**
*  @brief
*    Called on MySignal signal
*/
void Application44::OnMySignal(String sParameter)
{
	System::GetInstance()->GetConsole().Print(sParameter + " emitted MySignal signal\n");
}

/**
*  @brief
*    Returns the MyRTTIClass instance
*/
MyRTTIClass *Application44::GetMyRTTIClassInstance(MyRTTIClass *pObject)
{
	return (pObject == m_pMyRTTIClass) ? m_pMyRTTIClass : nullptr;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::AbstractLifecycle functions   ]
//[-------------------------------------------------------]
bool Application44::OnStart()
{
	// Call base implementation
	if (FrontendApplication::OnStart()) {
		// Get a list of supported script languages
		const Array<String> &lstScriptLanguages = ScriptManager::GetInstance()->GetScriptLanguages();
		for (uint32 i=0; i<lstScriptLanguages.GetNumOfElements(); i++) {
			// Get the name of the found script language
			const String sScriptLanguage = lstScriptLanguages[i];

			// Write the name of the found script language into the console
			System::GetInstance()->GetConsole().Print("- " + sScriptLanguage + '\n');

			// Get the filename extension of the found script language
			const String sScriptLanguageExtension = ScriptManager::GetInstance()->GetScriptLanguageExtension(sScriptLanguage);
			if (sScriptLanguageExtension.GetLength()) {
				// Run a script
				RunScript("Data/Scripts/44ScriptOOP." + sScriptLanguageExtension);
			} else {
				// This script language has no filename extension?!
				System::GetInstance()->GetConsole().Print(sScriptLanguage + " has no filename extension\n");
			}

			// Write a new line into the console
			System::GetInstance()->GetConsole().Print('\n');
		}

		// Exit the application
		Exit(0);

		// Done
		return true;
	}

	// Error
	return false;
}
