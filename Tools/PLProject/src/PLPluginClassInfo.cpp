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
#include <PLCore/String/RegEx.h>
#include <PLCore/String/Tokenizer.h>
#include <PLCore/Xml/XmlText.h>
#include <PLCore/Xml/XmlElement.h>
#include <PLCore/Xml/XmlDocument.h>
#include <PLCore/Container/ConstIterator.h>
#include "PLPluginClassInfo.h"


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
PLPluginClassInfo::PLPluginClassInfo() :
	m_bHasConstructor(false),
	m_bHasDefaultConstructor(false)
{
}

/**
*  @brief
*    Destructor
*/
PLPluginClassInfo::~PLPluginClassInfo()
{
}

/**
*  @brief
*    Appends the parsed information to the given XML element
*/
void PLPluginClassInfo::Save(XmlElement &cParent) const
{
	// Create the class XML element instance
	XmlElement *pClassElement = new XmlElement("Class");
	pClassElement->SetAttribute("Name",					 m_sClassName);
	pClassElement->SetAttribute("Namespace",			 m_sNamespace);
	pClassElement->SetAttribute("BaseClassName",		 m_sBaseClass);
	pClassElement->SetAttribute("Description",			 m_sDescription);
	pClassElement->SetAttribute("HasConstructor",		 m_bHasConstructor        ? 1 : 0);
	pClassElement->SetAttribute("HasDefaultConstructor", m_bHasDefaultConstructor ? 1 : 0);

	// Link the created XML class element instance to the provided parent
	cParent.LinkEndChild(*pClassElement);

	// Are there any properties?
	if (!m_mapProperties.IsEmpty()) {
		// Create the XML properties element instance
		XmlElement *pPropertiesElement = new XmlElement("Properties");

		// Iterate through the properties
		ConstIterator<String> cIterator = m_mapProperties.GetConstKeyIterator();
		while (cIterator.HasNext()) {
			// Get the key and the value of the current property
			const String &sKey   = cIterator.Next();
			const String &sValue = m_mapProperties.Get(sKey);

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

/**
*  @brief
*    Parse the given pl_class pl_class_end block
*/
void PLPluginClassInfo::ParsePLClassBlock(const String &sPLClassBlock)
{
	// Reset information
	m_sClassName			 = "";
	m_sNamespace			 = "";
	m_sBaseClass			 = "";
	m_sDescription			 = "";
	m_bHasConstructor		 = false;
	m_bHasDefaultConstructor = false;
	m_mapProperties.Clear();

	{ // Parse pl_class
		RegEx cPLClassBlock("^\\s*pl_class\\s*\\((?<RTTIEXP>\\w*)\\s*,\\s*(?<name>\\w*)\\s*,\\s*\\\"(?<namespace>\\w*)\\\"\\s*,\\s*(?<baseclass>\\w*\\:\\:\\w*)\\s*,\\s*\\\"(?<description>.*)\\\"\\)", RegEx::Multiline);
		if (cPLClassBlock.Match(sPLClassBlock)) {
			m_sClassName   = cPLClassBlock.GetNameResult("name");
			m_sNamespace   = cPLClassBlock.GetNameResult("namespace");
			m_sBaseClass   = cPLClassBlock.GetNameResult("baseclass");
			m_sDescription = cPLClassBlock.GetNameResult("description");
		}
	}

	{ // Parse constructor information
		// Is there any constructor?
		RegEx cPLConstructorBlock("^\\s*(pl_constructor_\\d\\d?)", RegEx::Multiline);
		m_bHasConstructor = cPLConstructorBlock.Match(sPLClassBlock);
		if (m_bHasConstructor) {
			// Is there a default constructor?
			RegEx cPLDefaultConstructorBlock("^\\s*(pl_constructor_0)", RegEx::Multiline);
			m_bHasDefaultConstructor = cPLDefaultConstructorBlock.Match(sPLClassBlock);
		}
	}

	{ // Parse property information
		RegEx cPLPropertiesBlock("^\\s*(pl_properties.*pl_properties_end)", RegEx::Multiline | RegEx::DotAll);
		if (cPLPropertiesBlock.Match(sPLClassBlock)) {
			// Setup the tokenizer
			Tokenizer cTokenizer;
			cTokenizer.SetDelimiters("\r\n");
			cTokenizer.SetSingleChars("");
			cTokenizer.SetQuotes("");
			cTokenizer.Start(cPLPropertiesBlock.GetResult(0));

			// Parse all individual properties
			RegEx cPLPropertyBlock("^\\s*(pl_property\\(\\\"(?<name>\\w*)\\\"\\s*,\\s*\\\"(?<value>.*)\\\"\\s*\\))", RegEx::Multiline);
			String sToken = cTokenizer.GetNextToken();
			while (sToken.GetLength()) {
				// Is this a property block?
				if (cPLPropertyBlock.Match(sToken)) {
					// Get the name and the value of the found property
					const String sPropertyName  = cPLPropertyBlock.GetNameResult("name");
					const String sPropertyValue = cPLPropertyBlock.GetNameResult("value");

					// Add the found property to the properties list
					m_mapProperties.Add(sPropertyName, sPropertyValue);
				}

				// Next, please!
				sToken = cTokenizer.GetNextToken();
			}
		}
	}
}

/**
*  @brief
*    Parse the given pl_class_metadata pl_class_metadata_end block
*/
void PLPluginClassInfo::ParsePLClassMetadataBlock(const PLCore::String &sPLClassMetadataBlock)
{
	// Reset information
	m_sClassName			 = "";
	m_sNamespace			 = "";
	m_sBaseClass			 = "";
	m_sDescription			 = "";
	m_bHasConstructor		 = false;
	m_bHasDefaultConstructor = false;
	m_mapProperties.Clear();

	{ // Parse pl_class
		RegEx cPLClassBlock("^\\s*pl_class_metadata\\s*\\(\\s*(?<name>\\w*)\\s*,\\s*\\\"(?<namespace>\\w*)\\\"\\s*,\\s*(?<baseclass>\\w*\\:\\:\\w*)\\s*,\\s*\\\"(?<description>.*)\\\"\\)");
		if (cPLClassBlock.Match(sPLClassMetadataBlock)) {
			m_sClassName   = cPLClassBlock.GetNameResult("name");
			m_sNamespace   = cPLClassBlock.GetNameResult("namespace");
			m_sBaseClass   = cPLClassBlock.GetNameResult("baseclass");
			m_sDescription = cPLClassBlock.GetNameResult("description");
		}
	}

	{ // Parse constructor information
		// Is there any constructor?
		RegEx cPLConstructorBlock("^\\s*(pl_constructor_\\d\\d?_metadata)", RegEx::Multiline);
		m_bHasConstructor = cPLConstructorBlock.Match(sPLClassMetadataBlock);
		if (m_bHasConstructor) {
			// Is there a default constructor?
			RegEx cPLDefaultConstructorBlock("^\\s*(pl_constructor_0_metadata)", RegEx::Multiline);
			m_bHasDefaultConstructor = cPLDefaultConstructorBlock.Match(sPLClassMetadataBlock);
		}
	}

	{ // Parse property information
		RegEx cPLPropertiesBlock("^\\s*(pl_properties.*pl_properties_end)", RegEx::Multiline | RegEx::DotAll);
		if (cPLPropertiesBlock.Match(sPLClassMetadataBlock)) {
			// Setup the tokenizer
			Tokenizer cTokenizer;
			cTokenizer.SetDelimiters("\r\n");
			cTokenizer.SetSingleChars("");
			cTokenizer.SetQuotes("");
			cTokenizer.Start(cPLPropertiesBlock.GetResult(0));

			// Parse all individual properties
			RegEx cPLPropertyBlock("^\\s*(pl_property\\(\\\"(?<name>\\w*)\\\"\\s*,\\s*\\\"(?<value>.*)\\\"\\s*\\))", RegEx::Multiline);
			String sToken = cTokenizer.GetNextToken();
			while (sToken.GetLength()) {
				// Is this a property block?
				if (cPLPropertyBlock.Match(sToken)) {
					// Get the name and the value of the found property
					const String sPropertyName  = cPLPropertyBlock.GetNameResult("name");
					const String sPropertyValue = cPLPropertyBlock.GetNameResult("value");

					// Add the found property to the properties list
					m_mapProperties.Add(sPropertyName, sPropertyValue);
				}

				// Next, please!
				sToken = cTokenizer.GetNextToken();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
PLPluginClassInfo::PLPluginClassInfo(const PLPluginClassInfo &cOther) :
	m_bHasConstructor(false),
	m_bHasDefaultConstructor(false)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
PLPluginClassInfo &PLPluginClassInfo::operator =(const PLPluginClassInfo &cOther)
{
	// No implementation because the copy operator is never used
	return *this;
}
