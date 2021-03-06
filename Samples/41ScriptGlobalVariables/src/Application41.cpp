/*********************************************************\
 *  File: Application41.cpp                              *
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
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/Script/Script.h>
#include <PLCore/Script/FuncScriptPtr.h>
#include <PLCore/Script/ScriptManager.h>
#include "Application41.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application41, "", PLCore::FrontendApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
pl_class_metadata_end(Application41)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application41::Application41(Frontend &cFrontend) : FrontendApplication(cFrontend)
{
}

/**
*  @brief
*    Destructor
*/
Application41::~Application41()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::AbstractLifecycle functions   ]
//[-------------------------------------------------------]
bool Application41::OnStart()
{
	// Call base implementation
	if (FrontendApplication::OnStart()) {
		// Bring the log into the verbose mode so that the log also writes log entries
		// directly into the console. This way, we can e.g. see script errors at once.
		Log::GetInstance()->SetVerbose(true);

		// Do only show us error messages within the log
		Log::GetInstance()->SetLogLevel(Log::Error);

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
				RunScript("Data/Scripts/41ScriptGlobalVariables." + sScriptLanguageExtension);
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


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Runs a script
*/
void Application41::RunScript(const String &sScriptFilename)
{
	// Create the script instance
	Script *pScript = ScriptManager::GetInstance()->CreateFromFile(sScriptFilename);
	if (pScript) {
		// Print the name of the used script language
		System::GetInstance()->GetConsole().Print("-- " + pScript->GetScriptLanguage() + " script language --\n");

		{ // Enumerate all global variables
			{ // Get a list of all global variables in the global namespace
				Array<String> lstGlobalVariables;
				pScript->GetGlobalVariables(lstGlobalVariables);
				System::GetInstance()->GetConsole().Print(String("Number of global variables in the global namespace: ") + lstGlobalVariables.GetNumOfElements() + '\n');
				for (uint32 i=0; i<lstGlobalVariables.GetNumOfElements(); i++)
					System::GetInstance()->GetConsole().Print(String("- Global variable ") + i + ": \"" + lstGlobalVariables[i] + "\"\n");
			}

			{ // Get a list of all global variables in the "PublicVariables"-namespace
				Array<String> lstGlobalVariables;
				pScript->GetGlobalVariables(lstGlobalVariables, "PublicVariables");
				System::GetInstance()->GetConsole().Print(String("Number of global variables in the \"PublicVariables\"-namespace: ") + lstGlobalVariables.GetNumOfElements() + '\n');
				for (uint32 i=0; i<lstGlobalVariables.GetNumOfElements(); i++)
					System::GetInstance()->GetConsole().Print(String("- Global variable ") + i + ": \"" + lstGlobalVariables[i] + "\"\n");
			}
		}

		// Check whether or not "g_Factor" is a global variable
		if (pScript->IsGlobalVariable("g_Factor")) {
			// Get the type of the global variable
			const ETypeID nTypeID = pScript->GetGlobalVariableTypeID("g_Factor");
			System::GetInstance()->GetConsole().Print(String("The type ID of the global variable \"g_Factor\" is \"") + static_cast<int>(nTypeID) + "\"\n");

			// Get the current value of the global variable
			String sOriginalValue = pScript->GetGlobalVariable("g_Factor");
			System::GetInstance()->GetConsole().Print("The value of the global variable \"g_Factor\" is \"" + sOriginalValue + "\"\n");

			// Set the current value of the global variable
			pScript->SetGlobalVariable("g_Factor", Var<int>(42));

			// Get the current value of the global variable
			const String sValue = pScript->GetGlobalVariable("g_Factor");
			if (sValue == "42")
				System::GetInstance()->GetConsole().Print("The value of the global variable \"g_Factor\" was changed successfully\n");

			// Reset the original value
			pScript->SetGlobalVariable("g_Factor", Var<String>(sOriginalValue));

			// The global variable "this" shouldn't exist, yet
			if (pScript->IsGlobalVariable("this")) {
				// Error, "this" should not happen...
				System::GetInstance()->GetConsole().Print("Failed to add the global variable \"this\" because it's already there!?\n");
			} else {
				// Add global variable "this"
				pScript->SetGlobalVariable("this", Var<String>("Jap, that's me!"));
				System::GetInstance()->GetConsole().Print(pScript->IsGlobalVariable("this") ? "Global variable \"this\" was added successfully\n" : "Failed to add the global variable \"this\"\n");
			}
		} else {
			System::GetInstance()->GetConsole().Print("There's no global variable with the name \"g_Factor\"\n");
		}

		// Cleanup
		delete pScript;

		// Print new line
		System::GetInstance()->GetConsole().Print("--\n");
	} else {
		// Error!
		System::GetInstance()->GetConsole().Print("Failed to load the script \"" + sScriptFilename + "\"\n");
	}

	// Print new line
	System::GetInstance()->GetConsole().Print('\n');
}
