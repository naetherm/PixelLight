/*********************************************************\
 *  File: ScriptManager.cpp                              *
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
#include "PLCore/Base/Class.h"
#include "PLCore/Base/ClassManager.h"
#include "PLCore/Tools/LoadableManager.h"
#include "PLCore/Script/Script.h"
#include "PLCore/Script/ScriptBinding.h"
#include "PLCore/Script/ScriptManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<ScriptManager>;


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
ScriptManager *ScriptManager::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<ScriptManager>::GetInstance();
}

bool ScriptManager::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<ScriptManager>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the filename extension of a given script language
*/
String ScriptManager::GetScriptLanguageExtension(const String &sScriptLanguage)
{
	RegisterClasses();

	// Get the RTTI class of the given script language
	const Class *pClass = m_mapScriptLanguages.Get(sScriptLanguage);
	if (pClass) {
		// Parse formats
		const String sFormats = pClass->GetProperties().Get("Formats");
		if (sFormats.GetLength()) {
			// Setup the tokenizer
			Tokenizer cTokenizer;
			cTokenizer.Start(sFormats);
			cTokenizer.SetDelimiters(" ,\t\r\n");
			cTokenizer.SetSingleChars("");

			// Return the first found filename extension
			return cTokenizer.GetNextToken();
		}
	}

	// Error!
	return "";
}

/**
*  @brief
*    Creates a script instance
*/
Script *ScriptManager::Create(const String &sScriptLanguage, bool bAddBindings)
{
	// Get the proper script class
	RegisterClasses();
	const Class *pClass = m_mapScriptLanguages.Get(sScriptLanguage);
	if (pClass) {
		// Create an instance of the script class
		Script *pScript = reinterpret_cast<Script*>(pClass->Create());

		// Bind all available script bindings at once?
		if (bAddBindings)
			pScript->AddBindings();

		// Return the created script instance
		return pScript;
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Creates a script instance by using a given filename
*/
Script *ScriptManager::CreateFromFile(const String &sFilename, bool bAddBindings, String::EFormat nStringFormat)
{
	// Create the script instance by using the extension of the given filename to detect the script language
	Script *pScript = Create(GetScriptLanguageByExtension(Url(sFilename).GetExtension()), bAddBindings);
	if (pScript) {
		// Get the script source code
		const String sSourceCode = LoadableManager::GetInstance()->LoadStringFromFile(sFilename, nStringFormat);

		// Set the script source code
		if (!sSourceCode.GetLength() || !pScript->SetSourceCode(sSourceCode)) {
			// Error! Destroy the created script instance...
			delete pScript;
			pScript = nullptr;
		}
	}

	// Done
	return pScript;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptManager::ScriptManager() :
	SlotClassLoaded(&ScriptManager::OnClassLoaded, this),
	SlotClassUnloaded(&ScriptManager::OnClassUnloaded, this)
{
	// The script manager MUST be informed if new classes are registered in order to register new script languages!
	ClassManager::GetInstance()->EventClassLoaded.Connect(SlotClassLoaded);
	ClassManager::GetInstance()->EventClassUnloaded.Connect(SlotClassUnloaded);

	{ // Register all script languages
		List<const Class*> lstClasses;
		ClassManager::GetInstance()->GetClasses(lstClasses, "PLCore::Script", Recursive, NoBase, NoAbstract);
		Iterator<const Class*> cIterator = lstClasses.GetIterator();
		while (cIterator.HasNext())
			m_lstNewClasses.Add(cIterator.Next());
	}

	{ // Register all script bindings
		List<const Class*> lstClasses;
		ClassManager::GetInstance()->GetClasses(lstClasses, "PLCore::ScriptBinding", Recursive, NoBase, NoAbstract);
		Iterator<const Class*> cIterator = lstClasses.GetIterator();
		while (cIterator.HasNext())
			m_lstNewClasses.Add(cIterator.Next());
	}
}

/**
*  @brief
*    Destructor
*/
ScriptManager::~ScriptManager()
{
	// Destroy all script binding instances, usually when we're in here there should no such instances be left
	for (uint32 i=0; i<m_lstScriptBindings.GetNumOfElements(); i++)
		delete m_lstScriptBindings[i];
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Unregister a class
*/
void ScriptManager::OnClassUnloaded(const Class *pClass)
{
	// Destroy script binding instance if required
	for (uint32 i=0; i<m_lstScriptBindings.GetNumOfElements(); i++) {
		// Get the current script binding instance
		ScriptBinding *pScriptBinding = m_lstScriptBindings[i];

		// Is this an instance of the unloaded class?
		if (pScriptBinding->GetClass() == pClass) {
			// Destroy instance
			m_lstScriptBindings.RemoveAtIndex(i);
			delete pScriptBinding;

			// Get us out of the loop (there's just one instance per script binding class)
			i = m_lstScriptBindings.GetNumOfElements();
		}
	}
}

/**
*  @brief
*    Registers queued classes
*/
void ScriptManager::RegisterClasses()
{
	// Is there anything on the register queue?
	if (m_lstNewClasses.GetNumOfElements()) {
		Iterator<const Class*> cIterator = m_lstNewClasses.GetIterator();
		while (cIterator.HasNext()) {
			// Get the class
			const Class *pClass = cIterator.Next();

			// Check parameter and base class
			static const String sScriptClassString = "PLCore::Script";
			static const String sScriptBindingClassString = "PLCore::ScriptBinding";
			if (pClass->IsDerivedFrom(sScriptClassString)) {
				// Register script language
				const String sLanguage = pClass->GetProperties().Get("Language");
				if (sLanguage.GetLength() && !m_mapScriptLanguages.Get(sLanguage)) {
					m_lstScriptLanguages.Add(sLanguage);
					m_mapScriptLanguages.Add(sLanguage, pClass);

					// Parse formats
					const String sFormats = pClass->GetProperties().Get("Formats");
					if (sFormats.GetLength()) {
						// Setup the tokenizer
						Tokenizer cTokenizer;
						cTokenizer.Start(sFormats);
						cTokenizer.SetDelimiters(" ,\t\r\n");
						cTokenizer.SetSingleChars("");

						// Register the filename extensions of this script language
						String sToken = cTokenizer.GetNextToken();
						while (sToken.GetLength()) {
							m_mapScriptLanguagesByExtension.Add(sToken, sLanguage);
							sToken = cTokenizer.GetNextToken();
						}
					}
				}
			} else if (pClass->IsDerivedFrom(sScriptBindingClassString)) {
				// Create an script binding instance
				ScriptBinding *pScriptBinding = static_cast<ScriptBinding*>(pClass->Create());
				if (pScriptBinding)
					m_lstScriptBindings.Add(pScriptBinding);
			}
		}
		m_lstNewClasses.Clear();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
