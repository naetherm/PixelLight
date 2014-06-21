/*********************************************************\
 *  File: PLPluginInfo.cpp                               *
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
#include <PLCore/Xml/Xml.h>
#include <PLCore/File/File.h>
#include <PLCore/File/Directory.h>
#include <PLCore/File/FileSearch.h>
#include <PLCore/String/RegEx.h>
#include <PLCore/String/Tokenizer.h>
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLCore/System/DynLib.h>
#include <PLCore/Base/ClassManager.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Base/Module.h>
//#include "PLPluginClassInfo.h"
#include "PLPluginInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PLPluginInfo::PLPluginInfo() :
	m_bActive(true),	// By default, projects are active
	m_bDelayed(true)	// By default, projects are delayed
{
}

/**
*  @brief
*    Destructor
*/
PLPluginInfo::~PLPluginInfo()
{
}

/**
*  @brief
*    Sets the name of the library
*/
void PLPluginInfo::SetLibraryName(const String &sLibraryName)
{
	//m_cPluginPlatformInfo.SetLibraryName(sLibraryName);
}

/**
*  @brief
*    Sets a library suffix, this string is appended to the library name
*/
void PLPluginInfo::SetLibrarySuffix(const String &sLibrarySuffix)
{
	//m_cPluginPlatformInfo.SetSuffixName(sLibrarySuffix);
}

/**
*  @brief
*    Sets the version of the plugin file
*/
void PLPluginInfo::SetPluginFileVersion(const String &sPluginVersion)
{
	m_sPluginFileVersion = sPluginVersion;
}

/**
*  @brief
*    Sets the PixelLight version string
*/
void PLPluginInfo::SetPLVersion(const String &sPLVersion)
{
	m_sPLVersion = sPLVersion;
}

/**
*  @brief
*    Set active flag, this flag indicates if the plugin is active or not
*/
void PLPluginInfo::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Set delayed flag, this flag indicates if delayed loading should be used for this plugin
*/
void PLPluginInfo::SetDelayed(bool bDelayed)
{
	m_bDelayed = bDelayed;
}

/**
*  @brief
*    Saves the parsed information to the given file
*/
void PLPluginInfo::Save(const String &sFilename) const
{
	XmlDocument cDocument;
	AppendInformation(cDocument);
	cDocument.Save(sFilename);
}

/**
*  @brief
*    Saves the parsed information to the given file
*/
void PLPluginInfo::Save(File &cFile) const
{
	XmlDocument cDocument;
	AppendInformation(cDocument);
	cDocument.Save(cFile);
}

/**
*  @brief
*    Load the given library and get the plugin info from it
*/
bool PLPluginInfo::CreateFromModule(const String &sModulePath)
{
	// Load the module library
	DynLib *dll = new DynLib(); // We leak this pointer intentionaly
	if (!dll->Load(sModulePath))
	{
		System::GetInstance()->GetConsole().Print("!!! Failed to load plugin module " + sModulePath);
		return false;
	}

	m_sLibraryName = Url(sModulePath).GetFilename();

	// Now we should have a new module registered
	// Since we are only linking PLCore, it should be "the other one", but that
	// might change in the future. We are just doing it this way to save an additional
	// input parameter
	const List<const Module*>& modules = ClassManager::GetInstance()->GetModules();
	auto it = modules.GetConstIterator();
	while (it.HasNext())
	{
		const Module* module = it.Next();
		if (Url(module->GetFilename()).GetUnixPath().CompareNoCase(Url(sModulePath).GetUnixPath()))
		{
			// Hopefully this should be our module
			m_sDescription = module->GetDescription();
			m_sLicense = module->GetLicense();
			m_sVendor = module->GetVendor();
			m_sModuleName = module->GetName();

			// Get class info
			const List<const Class*>& classes = module->GetClasses();
			auto it2 = classes.GetIterator();
			while (it2.HasNext())
			{
				const Class* clss = it2.Next();
				m_lstClasses.Add(clss);
			}

			return true;
		}
	}

	return false;
}

//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
PLPluginInfo::PLPluginInfo(const PLPluginInfo &cOther) :
	m_bActive(true),	// By default, projects are active
	m_bDelayed(true)	// By default, projects are delayed
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
PLPluginInfo &PLPluginInfo::operator =(const PLPluginInfo &cOther)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Adds an XmlTextElement child to an XmlElement
*/
void PLPluginInfo::AddTextXmlElement(XmlElement &cParent, const String &sName, const String &sValue) const
{
	XmlElement *pElement = new XmlElement(sName);
	XmlText *pTextValue = new XmlText(sValue);
	pElement->LinkEndChild(*pTextValue);
	cParent.LinkEndChild(*pElement);
}

/**
*  @brief
*    Appends the parsed information about a plugin to the given XML document
*/
void PLPluginInfo::AppendInformation(XmlDocument &cDocument) const
{
	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);
	XmlElement *pRootElement = new XmlElement("Plugin");
	pRootElement->SetAttribute("Version", m_sPluginFileVersion);
	pRootElement->SetAttribute("PixelLightVersion", m_sPLVersion);

	AddTextXmlElement(*pRootElement, "Active",  m_bActive  ? "1" : "0");
	AddTextXmlElement(*pRootElement, "Delayed", m_bDelayed ? "1" : "0");
	AddTextXmlElement(*pRootElement, "Name", m_sModuleName);
	if (m_sModuleVersion.GetLength())
		AddTextXmlElement(*pRootElement, "Version", m_sModuleVersion);
	if (m_sVendor.GetLength())
		AddTextXmlElement(*pRootElement, "Vendor", m_sVendor);
	if (m_sLicense.GetLength())
		AddTextXmlElement(*pRootElement, "License", m_sLicense);
	if (m_sDescription.GetLength())
		AddTextXmlElement(*pRootElement, "Description", m_sDescription);

	cDocument.LinkEndChild(*pRootElement);

	// Save platform info
	String sPlatformName = System::GetInstance()->GetPlatform();
	XmlElement *pPlatformElement = new XmlElement("Platform");
	pPlatformElement->SetAttribute("Name", sPlatformName);
	pPlatformElement->SetAttribute("BitArchitecture", System::GetInstance()->GetPlatformBitArchitecture());

	String sBuildType = "Debug";
	XmlElement *pLibrary = new XmlElement("Library");
	pLibrary->SetAttribute("Type", sBuildType);
	//const String &sDependecies = m_mapLibraryDependencies.Get(sPlatformName + sBuildType);
	//if (sDependecies != m_mapLibraryDependencies.NullKey)
	//	pLibrary->SetAttribute("Dependency", sDependecies);

	XmlText *pTextElememt = new XmlText(m_sLibraryName);
	pLibrary->LinkEndChild(*pTextElememt);
	pPlatformElement->LinkEndChild(*pLibrary);

	// Repeat for release with the same lib name
	sBuildType = "Release";
	pLibrary = new XmlElement("Library");
	pLibrary->SetAttribute("Type", sBuildType);

	pTextElememt = new XmlText(m_sLibraryName);
	pLibrary->LinkEndChild(*pTextElememt);
	pPlatformElement->LinkEndChild(*pLibrary);

	pRootElement->LinkEndChild(*pPlatformElement);

	// Are there any classes?
	if (!m_lstClasses.IsEmpty())
	{
		// Create the classes XML element instance
		XmlElement *pClassesElement = new XmlElement("Classes");

		// Iterate through all classes
		auto itClass = m_lstClasses.GetConstIterator();
		while (itClass.HasNext())
		{
			const Class* clss = itClass.Next();

			// Create the class XML element instance
			XmlElement *pClassElement = new XmlElement("Class");
			pClassElement->SetAttribute("Name", clss->GetName());
			pClassElement->SetAttribute("Namespace", clss->GetNamespace());
			pClassElement->SetAttribute("BaseClassName", clss->GetBaseClassName());
			pClassElement->SetAttribute("Description", clss->GetDescription());
			pClassElement->SetAttribute("HasConstructor", clss->HasConstructor() ? 1 : 0);
			pClassElement->SetAttribute("HasDefaultConstructor", clss->HasDefaultConstructor() ? 1 : 0);

			// Link the created XML class element instance to the provided parent
			pRootElement->LinkEndChild(*pClassElement);

			// Are there any properties?
			auto mapProps = clss->GetProperties();
			if (!mapProps.IsEmpty())
			{
				// Create the XML properties element instance
				XmlElement *pPropertiesElement = new XmlElement("Properties");

				// Iterate through the properties
				ConstIterator<String> cIterator = mapProps.GetConstKeyIterator();
				while (cIterator.HasNext())
				{
					// Get the key and the value of the current property
					const String &sKey = cIterator.Next();
					const String &sValue = mapProps.Get(sKey);

					// Create the XML property element instance
					XmlElement *pElement = new XmlElement("Property");
					pElement->SetAttribute("Name", sKey);
					XmlText *pTextElement = new XmlText(sValue);
					pElement->LinkEndChild(*pTextElement);

					// Link the XML property element to the XML properties element
					pPropertiesElement->LinkEndChild(*pElement);
				}

				// Link the created XML properties element instance to the class element
				pClassElement->LinkEndChild(*pPropertiesElement);
			}
		}

		// Link the classes XML element instance to the XML root element
		pRootElement->LinkEndChild(*pClassesElement);
	}
}
