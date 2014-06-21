/*********************************************************\
 *  File: ClassManager.cpp                               *
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
#include "PLCore/Xml/Xml.h"
#include "PLCore/Log/Log.h"
#include "PLCore/File/File.h"
#include "PLCore/File/Directory.h"
#include "PLCore/File/FileSearch.h"
#include "PLCore/System/System.h"
#include "PLCore/System/DynLib.h"
#include "PLCore/Tools/Loader.h"
#include "PLCore/Tools/Stopwatch.h"
#include "PLCore/Tools/LoaderImpl.h"
#include "PLCore/Base/Module.h"
#include "PLCore/Base/Rtti.h"
#include "PLCore/Base/Class.h"
#include "PLCore/Base/ClassImpl.h"
#include "PLCore/Base/ClassDummy.h"
#include "PLCore/Base/ClassManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<ClassManager>;


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
// Plugin API functions
typedef bool (*PLIsPluginDebugBuildFunc) ();
typedef int  (*PLGetPluginInfoFunc) ();


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
ClassManager *ClassManager::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<ClassManager>::GetInstance();
}

bool ClassManager::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<ClassManager>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get module by name
*/
const Module *ClassManager::GetModuleByName(const String &sModuleName) const
{
	// Have a look into our module list
	Iterator<const Module*> cIterator = m_lstModules.GetIterator();
	while (cIterator.HasNext()) {
		const Module *pModule = cIterator.Next();
		if (pModule->GetName() == sModuleName)
			return pModule;
	}

	// Module not found
	return nullptr;
}

/**
*  @brief
*    Load a module
*/
const Module *ClassManager::LoadModule(const String &sAbsFilename, bool bForceBuildTypeMatch)
{
	// Check if that library is already loaded
	Iterator<const Module*> cIterator = m_lstModules.GetIterator();
	while (cIterator.HasNext()) {
		const Module *pModule = cIterator.Next();
		if (pModule->GetFilename() == sAbsFilename && (!pModule->IsPlugin() || pModule->GetDynLib()))
			return pModule;
	}

	// Start the stopwatch
	Stopwatch cStopwatch(true);

	// Is the library existent?
	Module *pModule = nullptr;
	File cFile(sAbsFilename);
	if (cFile.Exists() && cFile.IsFile()) {
		// Load library
		DynLib *pDynLib = new DynLib();
		if (pDynLib->Load(sAbsFilename)) {
			bool bUseLibrary = true;

			// Check build type match
			if (bForceBuildTypeMatch) {
				PLIsPluginDebugBuildFunc pIsPluginDebugBuild = reinterpret_cast<PLIsPluginDebugBuildFunc>(pDynLib->GetSymbol("PLIsPluginDebugBuild"));
				if (pIsPluginDebugBuild) {
					if (PLCORE_IS_DEBUGMODE != pIsPluginDebugBuild()) {
						// Error!
						bUseLibrary = false;
						PL_LOG(Error, "Module '" + sAbsFilename + "': Release/debug mismatch")
					}
				} else {
					// Error!
					bUseLibrary = false;
					PL_LOG(Error, "Module '" + sAbsFilename + "': Forced build type match is active, but there's no 'PLIsPluginDebugBuild' function!")
				}
			}

			// Check if it is a valid PL plugin library
			if (bUseLibrary) {
				bUseLibrary = false;
				const String sLibraryFilename = Url(sAbsFilename).GetFilename();
				PLGetPluginInfoFunc pGetPluginInfo = reinterpret_cast<PLGetPluginInfoFunc>(pDynLib->GetSymbol("PLGetPluginInfo"));
				if (pGetPluginInfo) {
					// Get plugin info
					const int nModuleID = pGetPluginInfo();
					if (nModuleID > 0) {
						// Library successfully loaded
						PL_LOG(Info, "Module '" + sLibraryFilename + "' successfully loaded (required time: " + cStopwatch.GetSeconds() + " sec)")
						pModule = CreateModule(nModuleID);
						if (pModule) {
							// Set plugin information
							pModule->m_bPlugin   = true;
							pModule->m_pDynLib   = pDynLib;	// The module now takes over the control of the dynamic library instance
							pModule->m_sFilename = sAbsFilename;
							bUseLibrary = true;
						} else {
							// Error!
							PL_LOG(Error, "Module '" + sLibraryFilename + "': failed to create the module")
						}
					} else {
						// Error!
						PL_LOG(Error, "Module '" + sLibraryFilename + "': release/debug mismatch")
					}
				} else {
					// Error!
					PL_LOG(Error, "Module '" + sLibraryFilename + "' is no valid PixelLight plugin library")
				}
			}

			// Unload library if it is not used
			if (!bUseLibrary)
				delete pDynLib;
		} else {
			// Error!
			PL_LOG(Error, "Can't load the module '" + sAbsFilename + '\'')
			delete pDynLib;
		}
	} else {
		// Error!
		PL_LOG(Error, "Can't find the module '" + sAbsFilename + '\'')
	}

	// Return the loaded module
	return pModule;
}

/**
*  @brief
*    Scan a directory for compatible plugins and load them in
*/
bool ClassManager::ScanPlugins(const String &sPath, ERecursive nRecursive, bool bDelayedPluginLoading)
{
	// Get URL from path
	Url cUrl(sPath);
	if (!cUrl.IsDirectory())
		cUrl = sPath + '/';
	const String sUrl = cUrl.GetUrl();

	// Open directory
	Directory cDir(sUrl);
	if (cDir.Exists() && cDir.IsDirectory()) {
		// List files
		FileSearch cSearch(sUrl);
		while (cSearch.HasNextFile()) {
			// Get filename
			const String sFilename = cSearch.GetNextFile();
			if (sFilename != "." && sFilename != "..") {
				// Get absolute filename
				const String sFile = sUrl + sFilename;

				// Check extension
				const String sExtension = Url(sFile).GetExtension();
				if (sExtension == "plugin") {
					// Try to load plugin
					LoadPlugin(sFile, bDelayedPluginLoading);

				// Scan recursively?
				} else if (nRecursive == Recursive) {
					// Check if this is a directory
					File cFile(sFile);
					if (cFile.Exists() && cFile.IsDirectory()) {
						// Scan sub-directory
						ScanPlugins(sFile, nRecursive, bDelayedPluginLoading);
					}
				}
			}
		}

		// Return success
		return true;
	}

	// Error! Couldn't open directory
	return false;
}

/**
*  @brief
*    Load plugin
*/
bool ClassManager::LoadPlugin(const String &sFilename, bool bDelayedPluginLoading)
{
	// Get URL
	const Url cUrl = Url(sFilename);

	// Check file extension
	const String sExtension = cUrl.GetExtension();
	if (sExtension == "plugin") {
		// Load XML document
		XmlDocument cDocument;
		if (cDocument.Load(sFilename)) {
			// Get plugin element
			const XmlElement *pPluginElement = cDocument.GetFirstChildElement("Plugin");
			if (pPluginElement) {
				// Get format version
				const int nVersion = pPluginElement->GetAttribute("Version").GetInt();

				// Unknown
				if (nVersion > 1) {
					PL_LOG(Error, cDocument.GetValue() + ": " + LoaderImpl::UnknownFormatVersion)

				// 1 (current) or ""/0 (same format as 1)
				} else if (nVersion == 1 || nVersion == 0) {
					// [DEPRECATED]
					if (nVersion == 0)
						PL_LOG(Warning, cDocument.GetValue() + ": " + LoaderImpl::DeprecatedFormatVersion)

					// Load in the plugin
					return LoadPluginV1(cUrl, *pPluginElement, bDelayedPluginLoading);

				// No longer supported format version
				} else if (nVersion >= 0) {
					PL_LOG(Warning, cDocument.GetValue() + ": " + LoaderImpl::NoLongerSupportedFormatVersion)

				// Invalid format version (negative!)
				} else {
					PL_LOG(Error, cDocument.GetValue() + ": " + LoaderImpl::InvalidFormatVersion)
				}
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": Can't find 'Plugin' element")
			}
		} else {
			PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unload plugin
*/
bool ClassManager::UnloadPlugin(Module *pModule)
{
	// Is the module valid and a plugin?
	if (pModule && m_lstModules.IsElement(pModule) && pModule->IsPlugin()) {
		// Remove module
		UnregisterModule(pModule);

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unload all plugins
*/
bool ClassManager::UnloadAllPlugins()
{
	// Iterate through modules
	bool bError = false;
	for (uint32 i=0; i<m_lstModules.GetNumOfElements(); i++) {
		// Is that module a plugin?
		Module *pModule = const_cast<Module*>(m_lstModules[i]);
		if (pModule->IsPlugin()) {
			// Unload plugin
			if (!UnloadPlugin(pModule))
				bError = true;

			// As the module has been removed from the list, adjust counter variable
			i--;
		}
	}

	// Done
	return !bError;
}

/**
*  @brief
*    Get classes based on their base class and/or module (search for classes)
*/
void ClassManager::GetClasses(List<const Class*> &lstClasses, const String &sClass, ERecursive nRecursive, EIncludeBase nIncludeBase, EIncludeAbstract nIncludeAbstract, uint32 nModuleID) const
{
	// Iterate through classes
	Iterator<const Class*> cIterator = m_lstClasses.GetIterator();
	while (cIterator.HasNext()) {
		// Get class
		const Class *pClass = cIterator.Next();

		// Check base class
		if ( (sClass.GetLength() == 0) ||											// All classes wanted
			 (pClass->GetClassName() == sClass && nIncludeBase == IncludeBase) ||	// Base class itself
			 (pClass->GetBaseClassName() == sClass) ||								// Directly derived class
			 (nRecursive == Recursive && pClass->IsDerivedFrom(sClass))				// Derived class (recursive)
		   ) {
			// Only add classes of a specific module?
			if (nModuleID == 0 || (pClass->GetModule()->GetModuleID() == nModuleID)) {
				// Only add classes with a constructor?
				if (nIncludeAbstract == IncludeAbstract || pClass->HasConstructor()) {
					// Add class to list
					lstClasses.Add(pClass);
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ClassManager::ClassManager() :
	m_nModuleID(10000)	// Any value to start with
{
}

/**
*  @brief
*    Copy constructor
*/
ClassManager::ClassManager(const ClassManager &cSource) :
	m_nModuleID(10000)	// Any value to start with
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
ClassManager::~ClassManager()
{
	// Iterate through all modules and destroy them
	Iterator<const Module*> cModuleIterator = m_lstModules.GetIterator();
	while (cModuleIterator.HasNext()) {
		// Get module
		const Module *pModule = cModuleIterator.Next();

		// At this point, all real class instances may already have been destroyed automatically by the RTTI system (see "pl_class" -> "__pl_guard")
		// ... so, one might think that only the dummy classes should be left in here... but due to the random static de-initialization order we can't be sure...

		{ // Remove all classes from that module
			// Get list of classes (make a copy!)
			List<const Class*> lstClasses = pModule->GetClasses();

			// Remove all classes from that module
			Iterator<const Class*> cClassIterator = lstClasses.GetIterator();
			while (cClassIterator.HasNext()) {
				// Get the class implementation
				ClassImpl *pClassImpl = const_cast<Class*>(cClassIterator.Next())->m_pClassImpl;

				// Unregister the class
				UnregisterClass(pModule->GetModuleID(), pClassImpl);
			}
		}

		// Module has been unloaded (emit event)
		EventModuleUnloaded(pModule);

		// Destroy the module instance
		delete pModule;
	}
}

/**
*  @brief
*    Copy operator
*/
ClassManager &ClassManager::operator =(const ClassManager &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Get a unique module ID
*/
uint32 ClassManager::GetUniqueModuleID()
{
	// Return next module ID
	return m_nModuleID++;
}

/**
*  @brief
*    Create module
*/
Module *ClassManager::CreateModule(uint32 nModuleID)
{
	// Check if module is already there
	Module *pModule = const_cast<Module*>(m_mapModules.Get(nModuleID));
	if (!pModule) {
		// No, create new module
		pModule = new Module(nModuleID);

		// Add module to lists
		m_lstModules.Add(pModule);
		m_mapModules.Add(nModuleID, pModule);
	}

	// Return module
	return pModule;
}

/**
*  @brief
*    Load a module delayed
*/
void ClassManager::LoadModuleDelayed(const XmlElement &cPluginElement, const String &sAbsFilename, bool bForceBuildTypeMatch)
{
	// Is the library existent?
	File cFile(sAbsFilename);
	if (cFile.Exists() && cFile.IsFile()) {
		// Get classes element
		const XmlElement *pClassesElement = cPluginElement.GetFirstChildElement("Classes");
		if (pClassesElement) {
			// Request module ID from ClassManager
			const uint32 nModuleID = GetUniqueModuleID();

			// Create the module
			Module *pModule = CreateModule(nModuleID);
			if (pModule) {
				{ // Set plugin information
					pModule->m_bPlugin   = true;
					pModule->m_sFilename = sAbsFilename;

					// Name of module
					const XmlElement *pElement = cPluginElement.GetFirstChildElement("Name");
					if (pElement)
						pModule->m_sName = pElement->GetText();

					// Vendor of module
					pElement = cPluginElement.GetFirstChildElement("Vendor");
					if (pElement)
						pModule->m_sVendor = pElement->GetText();

					// License of module
					pElement = cPluginElement.GetFirstChildElement("License");
					if (pElement)
						pModule->m_sLicense = pElement->GetText();

					// Description of module
					pElement = cPluginElement.GetFirstChildElement("Description");
					if (pElement)
						pModule->m_sDescription = pElement->GetText();
				}

				// Iterate through all children and collect RTTI class meta information
				const XmlElement *pClassElement = pClassesElement->GetFirstChildElement("Class");
				while (pClassElement) {
					// Get class name, there must be a name!
					const String sClassName = pClassElement->GetAttribute("Name");
					if (sClassName.GetLength()) {
						// Get namespace
						const String sNamespace = pClassElement->GetAttribute("Namespace");

						// Early escape test: Check if the class is already known
						if (m_mapClasses.Get(sNamespace + "::" + sClassName)) {
							// The class name is already used. This is not really an error because the
							// class may have been loaded automatically through a shared library. The
							// class dummy stuff should be transparent to the user, so just be silent
							// in here.
						} else {
							// Create the dummy class implementation
							ClassDummy *pClassDummy = new ClassDummy(nModuleID, sClassName, pClassElement->GetAttribute("Description"),
								pClassElement->GetAttribute("Namespace"), pClassElement->GetAttribute("BaseClassName"), pClassElement->GetAttribute("HasConstructor").GetBool(), pClassElement->GetAttribute("HasDefaultConstructor").GetBool());

							// Get properties element
							const XmlElement *pPropertiesElement = pClassElement->GetFirstChildElement("Properties");
							if (pPropertiesElement) {
								// Iterate through all children and collect RTTI class meta information
								const XmlElement *pPropertyElement = pPropertiesElement->GetFirstChildElement("Property");
								while (pPropertyElement) {
									// Get property name, there must be a name!
									const String sPropertyName = pPropertyElement->GetAttribute("Name");
									if (sPropertyName.GetLength()) {
										// Get node value
										String sValue;
										const XmlNode *pValue = pPropertyElement->GetFirstChild();
										if (pValue && pValue->GetType() == XmlNode::Text)
											sValue = pValue->GetValue();

										// Add property
										pClassDummy->AddProperty(sPropertyName, sValue);
									}

									// Next property element, please
									pPropertyElement = pPropertyElement->GetNextSiblingElement("Property");
								}
							}

							// Register at class manager
							ClassManager::GetInstance()->RegisterClass(nModuleID, pClassDummy);
						}
					}

					// Next class element, please
					pClassElement = pClassElement->GetNextSiblingElement("Class");
				}
			} else {
				// Error!
				PL_LOG(Error, "Module '" + Url(sAbsFilename).GetFilename() + "': failed to create the module")
			}
		} else {
			// A plugin without any RTTI classes? Hm...
		}
	} else {
		// Error!
		PL_LOG(Error, "Can't find the module '" + sAbsFilename + '\'')
	}
}

/**
*  @brief
*    Register module
*/
void ClassManager::RegisterModule(const uint32 *pnModuleID, const String &sName, const String &sVendor, const String &sLicense, const String &sDescription)
{
	// Get module
	Module *pModule = CreateModule(*pnModuleID);	// "pnModuleID" is guaranteed to be valid
	if (pModule) {
		// Set module info
		pModule->SetModuleInfo(sName, sVendor, sLicense, sDescription);

		// Set module filename, or at least try it
		pModule->m_sFilename = System::GetInstance()->GetModuleFilenameByMemoryAddress(pnModuleID);

		// Module has been registered (emit event)
		EventModuleLoaded(pModule);
	}
}

/**
*  @brief
*    Unregister module
*/
void ClassManager::UnregisterModule(Module *pModule)
{
	// Check if module is valid
	if (pModule) {
		if (m_lstModules.IsElement(pModule)) {
			// Get list of classes (make a copy!)
			List<const Class*> lstClasses = pModule->GetClasses();

			// Remove all classes from that module
			Iterator<const Class*> cIterator = lstClasses.GetIterator();
			while (cIterator.HasNext())
				UnregisterClass(pModule->GetModuleID(), const_cast<Class*>(cIterator.Next())->m_pClassImpl);
		}

		// Module has been unloaded (emit event)
		EventModuleUnloaded(pModule);

		// Remove module
		m_lstModules.Remove(pModule);
		m_mapModules.Remove(pModule->GetModuleID());
		delete pModule;
	}
}

/**
*  @brief
*    Register class
*/
void ClassManager::RegisterClass(uint32 nModuleID, ClassImpl *pClassImpl)
{
	// Check for real/dummy class or duplicate class name
	Module *pModule = nullptr;
	const Class *pOldClass = m_mapClasses.Get(pClassImpl->GetClassName());
	if (pOldClass) {
		// Is the already registered implementation of the class currently a dummy?
		if (pOldClass->m_pClassImpl->IsDummy()) {
			// Is the new given implementation also a dummy?
			if (pClassImpl->IsDummy()) {
				// We only need one dummy, destroy the given one because we're now responsible for the given object
				delete pClassImpl;
			} else {
				// The current class implementation is a worthless dummy, replace it through the real thing right now!

				// The dummy and the real class are in different modules (this is just a security check)
				if (pOldClass->m_pClassImpl->m_nModuleID != nModuleID) {
					// Add the real class to the real module...
					CreateModule(nModuleID)->AddClass(pOldClass);

					// ... and remove the dummy class from the dummy module because a class can only be within
					// one module at the same time! Please note that in the end, there may be still dummy
					// classes left in the dummy module - e.g. if there are classes for other target platforms
					// within the plugin file (those aren't linked into the binary but PLProject still lists them).
					CreateModule(pOldClass->m_pClassImpl->m_nModuleID)->RemoveClass(pOldClass);
				}

				// Destroy the dummy class implementation
				delete pOldClass->m_pClassImpl;

				// Set the real class implementation
				pOldClass->m_pClassImpl = pClassImpl;

				// Tell the class implementation about the class instance wrapping it
				pClassImpl->m_pClass = const_cast<Class*>(pOldClass);
			}

			// We're done, get us out of this method right now!
			return;
		} else {
			// It's no dummy, is the new class to register a dummy?
			if (pClassImpl->IsDummy()) {
				// Ok, the new class to register is a worthless dummy because we already have the real thing, so just ignore it!

				// Destroy the given dummy because we're now responsible for the given object
				delete pClassImpl;

				// We're done, get us out of this method right now!
				return;
			} else {
				// Ok, the old and new classes are not dummies, this means that there's a class name conflict!

				// Get module
				pModule = CreateModule(nModuleID);

				// The class has a name that is already used by another class
				const String sClass		= pClassImpl->GetClassName();
				const String sModule	= pModule->GetName();
				const String sOldModule	= pOldClass->GetModule()->GetName();

				// Add warning to log that the class will not be available through e.g. GetClass()
				PL_LOG(Warning, "Class '" + sClass + "' [module '" + sModule + "']: Name conflict with already registered class '" + sClass + "' [module '" + sOldModule + "']")
			}
		}
	} else {
		// Get module
		pModule = CreateModule(nModuleID);
	}

	// Create an class instance wrapping the class implementation?
	Class *pClass = new Class(*pClassImpl);

	// Add class to list
	m_lstClasses.Add(pClass);
	if (!pOldClass)
		m_mapClasses.Add(pClass->GetClassName(), pClass);

	// Add class to module
	pModule->AddClass(pClass);

	// Class has been registered (emit event)
	EventClassLoaded(pClass);
}

/**
*  @brief
*    Unregister class
*/
void ClassManager::UnregisterClass(uint32 nModuleID, ClassImpl *pClassImpl)
{
	// Get the class instance wrapping the class implementation
	Class *pClass = pClassImpl->GetClass();
	if (pClass) {
		// Get module
		Module *pModule = CreateModule(nModuleID);

		// Remove class from list
		m_lstClasses.Remove(pClass);
		m_mapClasses.Remove(pClass->GetClassName());

		// Remove class from module
		pModule->RemoveClass(pClass);

		// Class has been unloaded (emit event)
		EventClassUnloaded(pClass);

		// If it's a dummy, destroy the class implementation
		if (pClassImpl->IsDummy()) {
			delete pClassImpl;
		} else {
			// The class implementation is no longer wrapped by a class
			pClassImpl->m_pClass = nullptr;

			// -> The real class instance is destroyed automatically by the RTTI system (see "pl_class" -> "__pl_guard")
		}

		// Destroy the class instance wrapping the class implementation
		delete pClass;
	}
}

/**
*  @brief
*    Load file format version 1 plugin
*/
bool ClassManager::LoadPluginV1(const Url &cUrl, const XmlElement &cPluginElement, bool bDelayedPluginLoading)
{
	// Is plugin active?
	const XmlNode *pNode = cPluginElement.GetFirstChild("Active");
	if (pNode) {
		// Get node value
		const XmlNode *pValue = pNode->GetFirstChild();
		if (pValue && pValue->GetType() == XmlNode::Text) {
			const String sValue = pValue->GetValue();
			if (sValue.GetLength() && !sValue.GetBool()) {
				// Plugin is inactive
				PL_LOG(Info, cUrl.GetUrl() + ": Plugin is ignored because it's inactive")
				return false;
			}
		}
	}

	// By default, we do not force a build type match
	bool bForceBuildTypeMatch = false;
	{
		// Get the "ForceBuildTypeMatch" element
		pNode = cPluginElement.GetFirstChild("ForceBuildTypeMatch");
		if (pNode) {
			// Get the value of the node
			const XmlNode *pValue = pNode->GetFirstChild();
			if (pValue && pValue->GetType() == XmlNode::Text) {
				const String sValue = pValue->GetValue();
				if (sValue.GetLength())
					bForceBuildTypeMatch = sValue.GetBool();
			}
		}
	}

	// By default, delayed shared library loading depends on the provided setting
	bool bDelayed = bDelayedPluginLoading;
	if (bDelayed) {
		// Get the "DelayedLoading" element
		pNode = cPluginElement.GetFirstChild("Delayed");
		if (pNode) {
			// Get the value of the node
			const XmlNode *pValue = pNode->GetFirstChild();
			if (pValue && pValue->GetType() == XmlNode::Text) {
				const String sValue = pValue->GetValue();
				if (sValue.GetLength())
					bDelayed = sValue.GetBool();
			}
		}
	}

	// Iterate through all children and collect plugin meta information
	const XmlElement *pElement = cPluginElement.GetFirstChildElement();
	while (pElement) {
		// Get node name
		const String sNodeName = pElement->GetValue();

		// Platform
		if (sNodeName == "Platform") {
			// Get platform name and bit architecture
			const String sPlatformName    = pElement->GetAttribute("Name");
			const uint32 nBitArchitecture = pElement->GetAttribute("BitArchitecture").GetUInt32();

			// Platform match?
			if (sPlatformName.GetLength() && sPlatformName == System::GetInstance()->GetPlatform() && nBitArchitecture == System::GetInstance()->GetPlatformBitArchitecture()) {
				const XmlElement *pPlatformElement = pElement->GetFirstChildElement();
				while (pPlatformElement) {
					// Get platform node name
					const String sPlatformNodeName = pPlatformElement->GetValue();

					// Library
					if (sPlatformNodeName == "Library") {
						// Get node value
						const XmlNode *pValue = pPlatformElement->GetFirstChild();
						if (pValue && pValue->GetType() == XmlNode::Text) {
							const String sValue = pValue->GetValue();
							if (sValue.GetLength()) {
								// Get type
								const String sType = pPlatformElement->GetAttribute("Type");

								// If we don't use this variable, we may receive a "conditional expression is constant"-warning (4127) from VC
								bool bDebugMode = PLCORE_IS_DEBUGMODE;
								if ((bDebugMode && sType == "Debug") || (!bDebugMode && sType == "Release")) {
									// Get absolute filename (native path style)
									const String sAbsFilename = Url(Url(sValue).IsAbsolute() ? sValue : cUrl.CutFilename() + sValue).GetNativePath();

									// Check if that library is already loaded
									bool bLibAlreadyLoaded = false;
									Iterator<const Module*> cIterator = m_lstModules.GetIterator();
									while (!bLibAlreadyLoaded && cIterator.HasNext()) {
										const Module *pModule = cIterator.Next();
										if (pModule->GetFilename() == sAbsFilename)
											bLibAlreadyLoaded = true;
									}

									// Library already loaded?
									if (!bLibAlreadyLoaded) {
										// Delayed shared library loading enabled?
										if (bDelayed)
											LoadModuleDelayed(cPluginElement, sAbsFilename, bForceBuildTypeMatch);
										else
											LoadModule(sAbsFilename, bForceBuildTypeMatch);
									}
								}
							}
						}
					}

					// Next element, please
					pPlatformElement = pPlatformElement->GetNextSiblingElement();
				}
			}
		}

		// Next element, please
		pElement = pElement->GetNextSiblingElement();
	}

	// Done
	PL_LOG(Info, cUrl.GetUrl() + ": Plugin successfully loaded")
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
