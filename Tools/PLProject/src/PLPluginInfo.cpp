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
#include "PLPluginClassInfo.h"
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
	// Cleanup all PLPLuginClassInfo instances
	for (uint32 i=0; i<m_lstClasses.GetNumOfElements(); i++)
		delete m_lstClasses[i];
}

/**
*  @brief
*    Sets the name of the library
*/
void PLPluginInfo::SetLibraryName(const String &sLibraryName)
{
	m_cPluginPlatformInfo.SetLibraryName(sLibraryName);
}

/**
*  @brief
*    Sets a library suffix, this string is appended to the library name
*/
void PLPluginInfo::SetLibrarySuffix(const String &sLibrarySuffix)
{
	m_cPluginPlatformInfo.SetSuffixName(sLibrarySuffix);
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
*    Parses the found header files in the given include path for pl_class..pl_class_end blocks
*/
void PLPluginInfo::ParseIncludeFiles(const String &sIncludePath)
{
	// Check if path exists and if it points to a directory
	FileObject cIncFile(sIncludePath);
	if (cIncFile.Exists() && cIncFile.IsDirectory()) {
		// Get all header files from the include directory
		Array<String> lstFiles;
		Find(lstFiles, sIncludePath, "*.h", true);

		// Parse each header file
		for (uint32 i=0; i<lstFiles.GetNumOfElements(); i++)
			ParseFile(lstFiles[i]);
	}
}

/**
*  @brief
*    Parses the found cpp files in the given source path for pl_class_metadata..pl_class_metadata_end blocks
*/
void PLPluginInfo::ParseSourceFiles(const PLCore::String &sSourcePath)
{
	// Check if path exists and if it points to a directory
	FileObject cIncFile(sSourcePath);
	if (cIncFile.Exists() && cIncFile.IsDirectory()) {
		// Get all header files from the include directory
		Array<String> lstFiles;
		Find(lstFiles, sSourcePath, "*.cpp", true);

		// Parse each header file
		for (uint32 i=0; i<lstFiles.GetNumOfElements(); i++)
			ParseFileMetaData(lstFiles[i]);
	}
}

/**
*  @brief
*    Parses the given main module source file for pl_module_plugin..pl_module_end blocks
*/
bool PLPluginInfo::ParseMainModuleFile(const String &sMainModuleFilename)
{
	// Get the complete content of the file as text
	String sContent = GetContentFromFile(sMainModuleFilename);
	if (sContent.GetLength()) {
		RegEx cPLPluginModuleBlock("^\\s*(pl_module_plugin.*pl_module_end)", RegEx::Multiline | RegEx::DotAll);
		if (cPLPluginModuleBlock.Match(sContent))
			ParsePluginModuleBlock(cPLPluginModuleBlock.GetResult(0));

		// Done
		return true;
	} else {
		// Error! No/empty file
		return false;
	}
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
*    Parses a single file for an pl_class..pl_class_end block
*/
void PLPluginInfo::ParseFile(const String &sFilename)
{
	// Get the complete content of the file as text
	String sContent = GetContentFromFile(sFilename);
	if (sContent.GetLength()) {
		RegEx cPLClassPLClassEndBlock("^\\s*(pl_class\\s*.*\\s*pl_class_end)", RegEx::Multiline | RegEx::DotAll);

		// Although not recommended, it's possible that there are multiple RTTI classes defined within a single file
		while (cPLClassPLClassEndBlock.Match(sContent)) {
			// Add RTTI class
			PLPluginClassInfo *pInfo = new PLPluginClassInfo();
			pInfo->ParsePLClassBlock(cPLClassPLClassEndBlock.GetResult(0));
			m_lstClasses.Add(pInfo);

			// Update the current position
			sContent = sContent.GetSubstring(cPLClassPLClassEndBlock.GetPosition());
		}
	}
}

/**
*  @brief
*    Parses a single file for an pl_class_metadata..pl_class_metadata_end block
*/
void PLPluginInfo::ParseFileMetaData(const String &sFilename)
{
	// Get the complete content of the file as text
	String sContent = GetContentFromFile(sFilename);
	if (sContent.GetLength()) {
		RegEx cPLClassPLClassEndBlock("^\\s*(pl_class_metadata\\s*.*\\s*pl_class_metadata_end)", RegEx::Multiline | RegEx::DotAll);

		// Although not recommended, it's possible that there are multiple RTTI classes defined within a single file
		while (cPLClassPLClassEndBlock.Match(sContent)) {
			// Add RTTI class
			PLPluginClassInfo *pInfo = new PLPluginClassInfo();
			pInfo->ParsePLClassMetadataBlock(cPLClassPLClassEndBlock.GetResult(0));
			m_lstClasses.Add(pInfo);

			// Update the current position
			sContent = sContent.GetSubstring(cPLClassPLClassEndBlock.GetPosition());
		}
	}
}

/**
*  @brief
*    Find files in a directory tree
*/
void PLPluginInfo::Find(Array<String> &lstNames, const String &sPath, const String &sPattern, bool bRecursive) const
{
	// Open directory
	Directory cDir(sPath);

	{ // Find files
		FileSearch cSearch(cDir, sPattern);
		while (cSearch.HasNextFile())
			lstNames.Add(sPath + '/' + cSearch.GetNextFile());
	}

	// Recurse into sub-directories?
	if (bRecursive) {
		FileSearch cSearch(cDir);
		while (cSearch.HasNextFile()) {
			const String sFilename = cSearch.GetNextFile();
			const String sFile     = sPath + '/' + sFilename;
			if (FileObject(sFile).IsDirectory() && bRecursive && sFilename != '.' && sFilename != "..")
				Find(lstNames, sFile, sPattern, bRecursive);
		}
	}
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
*    Parses an pl_module_plugin..pl_module_end block
*/
void PLPluginInfo::ParsePluginModuleBlock(const String &sPluginModuleBlock)
{
	// Setup regular expressions
	RegEx cRegExModulePlugin("^\\s*pl_module_plugin\\(\\s*\\\"(?<name>\\w*)\\\"\\s*\\)\\s*$");
	RegEx cRegExVendor("^\\s*pl_module_vendor\\s*\\(\\s*\\\"(?<text>.*)\\\"\\s*\\)\\s*$", RegEx::MatchCaseSensitive);
	RegEx cRegExLicense("^\\s*pl_module_license\\s*\\(\\s*\\\"(?<text>.*)\\\"\\s*\\)\\s*$", RegEx::MatchCaseSensitive);
	RegEx cRegExDescription("^\\s*pl_module_description\\s*\\(\\s*\\\"(?<text>.*)\\\"\\s*\\)\\s*$", RegEx::MatchCaseSensitive);
	RegEx cRegExVersion("^\\s*pl_module_version\\s*\\(\\s*\\\"(?<text>.*)\\\"\\s*\\)\\s*$", RegEx::MatchCaseSensitive);
	RegEx cRegExActive("^\\s*pl_module_active\\s*\\((?<num>\\d)\\)\\s*$");
	RegEx cRegExDelayed("^\\s*pl_module_delayed\\s*\\((?<num>\\d)\\)\\s*$");

	// Setup tokenizer
	Tokenizer cTokenizer;
	cTokenizer.SetDelimiters("\r\n");
	cTokenizer.SetSingleChars("");
	cTokenizer.SetQuotes("");
	cTokenizer.SetCommentStartTag("");
	cTokenizer.SetCommentEndTag("");
	cTokenizer.SetSingleLineComment("");
	cTokenizer.Start(sPluginModuleBlock);

	// Iterate through all lines
	String sLine = cTokenizer.GetNextToken();
	while (sLine.GetLength()) {
		// Check for pl_module_plugin
		if (cRegExModulePlugin.Match(sLine)) {
			m_sPluginName = cRegExModulePlugin.GetNameResult("name");

		// Check for pl_module_vendor
		} else if (cRegExVendor.Match(sLine)) {
			m_sVendor = cRegExVendor.GetNameResult("text");

		// Check for pl_module_license
		} else if (cRegExLicense.Match(sLine)) {
			m_sLicense = cRegExLicense.GetNameResult("text");

		// Check for pl_module_description
		} else if (cRegExDescription.Match(sLine)) {
			m_sDescription = cRegExDescription.GetNameResult("text");

		// Check for pl_module_version
		} else if (cRegExVersion.Match(sLine)) {
			m_sModuleVersion = cRegExVersion.GetNameResult("text");

		// Check for pl_module_active
		} else if (cRegExActive.Match(sLine)) {
			m_bActive = cRegExActive.GetNameResult("num").GetBool();

		// Check for pl_module_delayed
		} else if (cRegExDelayed.Match(sLine)) {
			m_bDelayed = cRegExDelayed.GetNameResult("num").GetBool();

		// Let PLPluginPlatformInfo parse the line
		} else {
			m_cPluginPlatformInfo.ParseLine(sLine);
		}

		// Next, please
		sLine = cTokenizer.GetNextToken();
	}
}

/**
* @brief
*   Returns the complete content of a file as text
*/
String PLPluginInfo::GetContentFromFile(const String &sFilename) const
{
	File cFile(sFilename);
	return cFile.Open(File::FileRead | File::FileText) ? cFile.GetContentAsString() : "";
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
	AddTextXmlElement(*pRootElement, "Name", m_sPluginName);
	if (m_sModuleVersion.GetLength())
		AddTextXmlElement(*pRootElement, "Version", m_sModuleVersion);
	if (m_sVendor.GetLength())
		AddTextXmlElement(*pRootElement, "Vendor", m_sVendor);
	if (m_sLicense.GetLength())
		AddTextXmlElement(*pRootElement, "License", m_sLicense);
	if (m_sDescription.GetLength())
		AddTextXmlElement(*pRootElement, "Description", m_sDescription);

	cDocument.LinkEndChild(*pRootElement);

	// Append the parsed information to the given XML element
	m_cPluginPlatformInfo.Save(*pRootElement);

	// Are there any classes?
	if (!m_lstClasses.IsEmpty()) {
		// Create the classes XML element instance
		XmlElement *pClassesElement = new XmlElement("Classes");

		// Iterate through all classes
		ConstIterator<PLPluginClassInfo*> cIterator = m_lstClasses.GetConstIterator();
		while (cIterator.HasNext())
			cIterator.Next()->Save(*pClassesElement);

		// Link the classes XML element instance to the XML root element
		pRootElement->LinkEndChild(*pClassesElement);
	}
}
