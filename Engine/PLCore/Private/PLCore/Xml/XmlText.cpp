/*********************************************************\
 *  File: XmlText.cpp                                    *
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
#include "PLCore/Xml/XmlText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlText::XmlText() : XmlNode(Text),
	m_bCDATA(false)
{
}

/**
*  @brief
*    Default constructor
*/
XmlText::XmlText(const String &sValue) : XmlNode(Text),
	m_bCDATA(false)
{
	// Copy the value
	SetValue(sValue);
}

/**
*  @brief
*    Copy constructor
*/
XmlText::XmlText(const XmlText &cSource) : XmlNode(Text),
	m_bCDATA(cSource.m_bCDATA)
{
	// Copy the value
	SetValue(cSource.GetValue());
}

/**
*  @brief
*    Destructor
*/
XmlText::~XmlText()
{
}

/**
*  @brief
*    Copy operator
*/
XmlText &XmlText::operator =(const XmlText &cSource)
{
	// Copy data
	m_bCDATA = cSource.m_bCDATA;
	SetValue(cSource.GetValue());
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual XmlBase functions                      ]
//[-------------------------------------------------------]
bool XmlText::Save(File &cFile, uint32 nDepth)
{
	String sBuffer;
	EncodeString(m_sValue, sBuffer);
	cFile.Print(sBuffer);

	// Done
	return true;
}

String XmlText::ToString(uint32 nDepth) const
{
	String sBuffer;
	EncodeString(m_sValue, sBuffer);
	return String(sBuffer);
}

const char *XmlText::Parse(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	// Constants
	static const char   *pszStartTag     = "<![CDATA[";
	static const uint32  nStartTagLength = static_cast<uint32>(strlen(pszStartTag));
	static const char   *pszEndTag       = "]]>";
	static const char   *pszEnd          = "<";

	m_sValue = "";

	if (pData) {
		pData->Stamp(pszData, nEncoding);
		m_cCursor = pData->Cursor();
	}

	if (m_bCDATA || StringEqual(pszData, pszStartTag, false, nEncoding)) {
		m_bCDATA = true;

		if (!StringEqual(pszData, pszStartTag, false, nEncoding)){
			// Set error code
			XmlDocument *pDocument = GetDocument();
			if (pDocument)
				pDocument->SetError(ErrorParsingCData, pszData, pData, nEncoding);

			// Error!
			return nullptr;
		}
		pszData += nStartTagLength;

		// Keep all the white space, ignore the encoding, etc.
		while (pszData && *pszData && !StringEqual(pszData, pszEndTag, false, nEncoding)) {
			m_sValue += *pszData;
			++pszData;
		}

		String sDummy;
		pszData = ReadText(pszData, sDummy, false, pszEndTag, false, nEncoding);

		// Done
		return pszData;
	} else {
		bool bIgnoreWhite = true;
		pszData = ReadText(pszData, m_sValue, bIgnoreWhite, pszEnd, false, nEncoding);
		if (pszData && *pszData)
			return pszData - 1;	// don't truncate the '<'

		// Error!
		return nullptr;
	}
}


//[-------------------------------------------------------]
//[ Public virtual XmlNode functions                      ]
//[-------------------------------------------------------]
XmlNode *XmlText::Clone() const
{
	return new XmlText(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
