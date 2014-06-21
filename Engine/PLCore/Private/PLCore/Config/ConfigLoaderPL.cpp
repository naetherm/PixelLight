/*********************************************************\
 *  File: ConfigLoaderPL.cpp                             *
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
#include "PLCore/Base/Class.h"
#include "PLCore/Config/Config.h"
#include "PLCore/Config/ConfigLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ConfigLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ConfigLoaderPL::Load(Config &cConfig, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get config element
		const XmlElement *pConfigElement = cDocument.GetFirstChildElement("Config");
		if (pConfigElement) {
			// Get the format version
			int nVersion = pConfigElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cConfig, *pConfigElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cConfig, *pConfigElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Config' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool ConfigLoaderPL::Save(const Config &cConfig, File &cFile)
{
	return SaveParams(cConfig, cFile, "", false);
}

bool ConfigLoaderPL::SaveParams(const Config &cConfig, File &cFile, String sGroup, bool bNoDefault)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add config
	XmlElement *pConfigElement = new XmlElement("Config");
	pConfigElement->SetAttribute("Version", "1");

	// Save ONLY configuration class instances which are in a certain configuration class group?
	bool bResult = true; // No error by default
	if (sGroup.GetLength()) {
		// Loop through all config class instances and save them
		for (uint32 nConfig=0; nConfig<cConfig.GetNumOfClasses(); nConfig++) {
			// Get config class instance
			const ConfigGroup *pConfigGroup = cConfig.GetClassByIndex(nConfig);

			// Check whether the class is within the given configuration group
			if (pConfigGroup && pConfigGroup->IsInstanceOf(sGroup)) {
				// Save this configuration group
				if (!SaveGroup(*pConfigElement, *pConfigGroup, bNoDefault))
					bResult = false; // Error!
			}
		}
	} else { // Save ALL configuration class instances
		// Loop through all config class instances and save them
		for (uint32 nConfig=0; nConfig<cConfig.GetNumOfClasses(); nConfig++) {
			// Get config class instance
			const ConfigGroup *pConfigGroup = cConfig.GetClassByIndex(nConfig);

			// Save this configuration group
			if (pConfigGroup && !SaveGroup(*pConfigElement, *pConfigGroup, bNoDefault))
				bResult = false; // Error!
		}
	}

	// Link config element to parent
	cDocument.LinkEndChild(*pConfigElement);

	// Save config
	cDocument.Save(cFile);

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ConfigLoaderPL::ConfigLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
ConfigLoaderPL::~ConfigLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool ConfigLoaderPL::LoadV1(Config &cConfig, const XmlElement &cConfigElement) const
{
	// Iterate through all groups
	const XmlElement *pGroupElement = cConfigElement.GetFirstChildElement("Group");
	while (pGroupElement) {
		// Get group class name
		String sClass = pGroupElement->GetAttribute("Class");
		if (sClass.GetLength()) {
			// Get config class instance
			ConfigGroup *pClass = cConfig.GetClass(sClass);
			if (pClass) {
				// Set variables
				pClass->SetValuesXml(*pGroupElement);
			}
		}

		// Next element, please
		pGroupElement = pGroupElement->GetNextSiblingElement("Group");
	}

	// Done
	return true;
}

/**
*  @brief
*    Save configuration group settings
*/
bool ConfigLoaderPL::SaveGroup(XmlElement &cConfigElement, const ConfigGroup &cGroup, bool bNoDefault) const
{
	// Create config group
	XmlElement *pGroupElement = new XmlElement("Group");
	pGroupElement->SetAttribute("Class", cGroup.GetClass()->GetClassName());

	// Add variables as elements
	cGroup.GetValuesXml(*pGroupElement, bNoDefault ? NoDefault : WithDefault);

	// Link group to config if it isn't empty
	if (pGroupElement->GetFirstAttribute())
		cConfigElement.LinkEndChild(*pGroupElement);
	else
		delete pGroupElement; // Ok, the group is empty - say goodbye useless group :)

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
