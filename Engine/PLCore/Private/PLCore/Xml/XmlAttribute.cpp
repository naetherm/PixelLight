/*********************************************************\
 *  File: XmlAttribute.cpp                               *
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
#include <stdio.h>
#include "PLCore/File/File.h"
#include "PLCore/Xml/XmlParsingData.h"
#include "PLCore/Xml/XmlDocument.h"
#include "PLCore/Xml/XmlAttribute.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
XmlAttribute::XmlAttribute() :
	m_pDocument(nullptr),
	m_pPreviousAttribute(nullptr),
	m_pNextAttribute(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
XmlAttribute::~XmlAttribute()
{
}

/**
*  @brief
*    Examines the value string
*/
XmlBase::EQueryResult XmlAttribute::QueryIntValue(int &nValue) const
{
	int nBackup = nValue;
	EQueryResult nResult = (sscanf(m_sValue.GetASCII(), "%d", &nValue) == 1) ? Success : WrongType;
	if (nResult != Success)
		nValue = nBackup;
	return nResult;
}

/**
*  @brief
*    Examines the value string
*/
XmlBase::EQueryResult XmlAttribute::QueryDoubleValue(double &dValue) const
{
	double dBackup = dValue;
	EQueryResult nResult = (sscanf(m_sValue.GetASCII(), "%lf", &dValue) == 1) ? Success : WrongType;
	if (nResult != Success)
		dValue = dBackup;
	return nResult;
}


//[-------------------------------------------------------]
//[ Public virtual XmlBase functions                      ]
//[-------------------------------------------------------]
bool XmlAttribute::Save(File &cFile, uint32 nDepth)
{
	String sName, sValue;

	EncodeString(m_sName,  sName);
	EncodeString(m_sValue, sValue);

	if (sValue.IsSubstring('\"'))
		cFile.Print(sName + "='" + sValue + '\'');
	else
		cFile.Print(sName + "=\"" + sValue + '\"');

	// Done
	return true;
}

String XmlAttribute::ToString(uint32 nDepth) const
{
	String sName, sValue;

	EncodeString(m_sName,  sName);
	EncodeString(m_sValue, sValue);

	if (sValue.IsSubstring('\"'))
		return sName + "='" + sValue + '\'';
	else
		return sName + "=\"" + sValue + '\"';
}

const char *XmlAttribute::Parse(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	pszData = SkipWhiteSpace(pszData, nEncoding);
	if (!pszData || !*pszData)
		return nullptr; // Error!

	if (pData) {
		pData->Stamp(pszData, nEncoding);
		m_cCursor = pData->Cursor();
	}

	// Read the name, the '=' and the value
	const char *pszError = pszData;
	pszData = ReadName(pszData, m_sName, nEncoding);
	if (!pszData || !*pszData) {
		// Set error code
		if (m_pDocument)
			m_pDocument->SetError(ErrorReadingAttributes, pszError, pData, nEncoding);

		// Error!
		return nullptr;
	}
	pszData = SkipWhiteSpace(pszData, nEncoding);
	if (!pszData || !*pszData || *pszData != '=') {
		// Set error code
		if (m_pDocument)
			m_pDocument->SetError(ErrorReadingAttributes, pszData, pData, nEncoding);

		// Error!
		return nullptr;
	}

	++pszData;	// skip '='
	pszData = SkipWhiteSpace(pszData, nEncoding);
	if (!pszData || !*pszData) {
		// Set error code
		if (m_pDocument)
			m_pDocument->SetError(ErrorReadingAttributes, pszData, pData, nEncoding);

		// Error!
		return nullptr;
	}

	// Constants
	const char szSingleQuote = '\'';
	const char szDoubleQuote = '\"';

	if (*pszData == szSingleQuote) {
		++pszData;
		const char *pszEnd = "\'";		// Single quote in string
		pszData = ReadText(pszData, m_sValue, false, pszEnd, false, nEncoding);
	} else if (*pszData == szDoubleQuote) {
		++pszData;
		const char *pszEnd = "\"";		// Double quote in string
		pszData = ReadText(pszData, m_sValue, false, pszEnd, false, nEncoding);
	} else {
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		m_sValue = "";
		while (    pszData && *pszData										// Existence
				&& !IsWhiteSpace( *pszData )								// Whitespace
				&& *pszData != '/' && *pszData != '>' ) {					// Tag end
			if (*pszData == szSingleQuote || *pszData == szDoubleQuote) {
				// We did not have an opening quote but seem to have a closing one. Give up and throw an error.

				// Set error code
				if (m_pDocument)
					m_pDocument->SetError(ErrorReadingAttributes, pszData, pData, nEncoding);

				// Error!
				return nullptr;
			}
			m_sValue += *pszData;
			++pszData;
		}
	}

	// Done
	return pszData;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
XmlAttribute::XmlAttribute(const XmlAttribute &cSource) :
	m_pDocument(nullptr),
	m_pPreviousAttribute(nullptr),
	m_pNextAttribute(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
XmlAttribute &XmlAttribute::operator =(const XmlAttribute &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
