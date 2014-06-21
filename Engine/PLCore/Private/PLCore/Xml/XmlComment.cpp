/*********************************************************\
 *  File: XmlComment.cpp                                 *
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
#include <string.h>
#include "PLCore/File/File.h"
#include "PLCore/Xml/XmlParsingData.h"
#include "PLCore/Xml/XmlDocument.h"
#include "PLCore/Xml/XmlComment.h"


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
XmlComment::XmlComment() : XmlNode(Comment)
{
}

/**
*  @brief
*    Constructor
*/
XmlComment::XmlComment(const String &sValue) : XmlNode(Comment)
{
	// Copy the value
	SetValue(sValue);
}

/**
*  @brief
*    Copy constructor
*/
XmlComment::XmlComment(const XmlComment &cSource) : XmlNode(Comment)
{
	// Copy the value
	SetValue(cSource.GetValue());
}

/**
*  @brief
*    Destructor
*/
XmlComment::~XmlComment()
{
}


//[-------------------------------------------------------]
//[ Public virtual XmlBase functions                      ]
//[-------------------------------------------------------]
bool XmlComment::Save(File &cFile, uint32 nDepth)
{
	// Get the number of empty spaces
	const XmlDocument *pDocument = GetDocument();
	const uint32 nNumOfSpaces = (pDocument ? pDocument->GetTabSize() : 4) * nDepth;

	// Print empty spaces
	for (uint32 i=0; i<nNumOfSpaces; i++)
		cFile.PutC(' ');

	// Write value
	cFile.Print("<!--" + m_sValue + "-->");

	// Done
	return true;
}

String XmlComment::ToString(uint32 nDepth) const
{
	// Get the number of empty spaces
	const XmlDocument *pDocument = GetDocument();
	const uint32 nNumOfSpaces = (pDocument ? pDocument->GetTabSize() : 4) * nDepth;

	// Print empty spaces
	String sXml;
	for (uint32 i=0; i<nNumOfSpaces; i++)
		sXml += ' ';

	// Write value
	sXml += "<!--" + m_sValue + "-->";

	// Return the resulting string
	return sXml;
}

const char *XmlComment::Parse(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	// Constants
	static const char   *pszStartTag     = "<!--";
	static const uint32  nStartTagLength = static_cast<uint32>(strlen(pszStartTag));
	static const char   *pszEndTag       = "-->";
	static const uint32  nEndTagLength   = static_cast<uint32>(strlen(pszEndTag));

	m_sValue = "";

	pszData = SkipWhiteSpace(pszData, nEncoding);

	if (pData) {
		pData->Stamp(pszData, nEncoding);
		m_cCursor = pData->Cursor();
	}

	if (!StringEqual(pszData, pszStartTag, false, nEncoding)) {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorParsingComment, pszData, pData, nEncoding);

		// Error!
		return nullptr;
	}
	pszData += nStartTagLength;

	// Keep all the white space
	while (pszData && *pszData && !StringEqual(pszData, pszEndTag, false, nEncoding)) {
		m_sValue += *pszData;
		++pszData;
	}
	if (pszData && *pszData)
		pszData += nEndTagLength;

	// Done
	return pszData;
}


//[-------------------------------------------------------]
//[ Public virtual XmlNode functions                      ]
//[-------------------------------------------------------]
XmlNode *XmlComment::Clone() const
{
	return new XmlComment(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
