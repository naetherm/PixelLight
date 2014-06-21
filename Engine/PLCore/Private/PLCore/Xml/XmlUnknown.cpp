/*********************************************************\
 *  File: XmlUnknown.cpp                                 *
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
#include "PLCore/File/File.h"
#include "PLCore/Xml/XmlParsingData.h"
#include "PLCore/Xml/XmlDocument.h"
#include "PLCore/Xml/XmlUnknown.h"


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
XmlUnknown::XmlUnknown() : XmlNode(Unknown)
{
}

/**
*  @brief
*    Copy constructor
*/
XmlUnknown::XmlUnknown(const XmlUnknown &cSource) : XmlNode(Unknown)
{
	// Copy the value
	SetValue(cSource.GetValue());
}

/**
*  @brief
*    Destructor
*/
XmlUnknown::~XmlUnknown()
{
}


//[-------------------------------------------------------]
//[ Public virtual XmlBase functions                      ]
//[-------------------------------------------------------]
bool XmlUnknown::Save(File &cFile, uint32 nDepth)
{
	// Get the number of empty spaces
	const XmlDocument *pDocument = GetDocument();
	const uint32 nNumOfSpaces = (pDocument ? pDocument->GetTabSize() : 4) * nDepth;

	// Print empty spaces
	for (uint32 i=0; i<nNumOfSpaces; i++)
		cFile.PutC(' ');

	// Write value
	cFile.Print('<' + m_sValue + '>');

	// Done
	return true;
}

String XmlUnknown::ToString(uint32 nDepth) const
{
	// Get the number of empty spaces
	const XmlDocument *pDocument = GetDocument();
	const uint32 nNumOfSpaces = (pDocument ? pDocument->GetTabSize() : 4) * nDepth;

	// Print empty spaces
	String sXml;
	for (uint32 i=0; i<nNumOfSpaces; i++)
		sXml += ' ';

	// Write value
	sXml += '<' + m_sValue + '>';

	// Return the resulting string
	return sXml;
}

const char *XmlUnknown::Parse(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	pszData = SkipWhiteSpace(pszData, nEncoding);

	if (pData) {
		pData->Stamp(pszData, nEncoding);
		m_cCursor = pData->Cursor();
	}
	if (!pszData || !*pszData || *pszData != '<') {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorParsingUnknown, pszData, pData, nEncoding);

		// Error!
		return nullptr;
	}
	++pszData;
	m_sValue = "";

	while (pszData && *pszData && *pszData != '>') {
		m_sValue += *pszData;
		++pszData;
	}

	if (!pszData) {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorParsingUnknown, 0, 0, nEncoding);
	}
	if (pszData && *pszData == '>')
		return pszData + 1;

	// Done
	return pszData;
}


//[-------------------------------------------------------]
//[ Public virtual XmlNode functions                      ]
//[-------------------------------------------------------]
XmlNode *XmlUnknown::Clone() const
{
	return new XmlUnknown(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
