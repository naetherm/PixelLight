/*********************************************************\
 *  File: XmlDeclaration.cpp                             *
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
#include "PLCore/Xml/XmlAttribute.h"
#include "PLCore/Xml/XmlDeclaration.h"


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
XmlDeclaration::XmlDeclaration() : XmlNode(Declaration)
{
}

/**
*  @brief
*    Constructor
*/
XmlDeclaration::XmlDeclaration(const String &sVersion, const String &sEncoding, const String &sStandalone) : XmlNode(Declaration),
	m_sVersion(sVersion),
	m_sEncoding(sEncoding),
	m_sStandalone(sStandalone)
{
}

/**
*  @brief
*    Copy constructor
*/
XmlDeclaration::XmlDeclaration(const XmlDeclaration &cSource) : XmlNode(Declaration),
	m_sVersion(cSource.m_sVersion),
	m_sEncoding(cSource.m_sEncoding),
	m_sStandalone(cSource.m_sStandalone)
{
}

/**
*  @brief
*    Destructor
*/
XmlDeclaration::~XmlDeclaration()
{
}

/**
*  @brief
*    Copy operator
*/
XmlDeclaration &XmlDeclaration::operator =(const XmlDeclaration &cSource)
{
	// Copy the data
	m_sVersion    = cSource.m_sVersion;
	m_sEncoding   = cSource.m_sEncoding;
	m_sStandalone = cSource.m_sStandalone;
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual XmlBase functions                      ]
//[-------------------------------------------------------]
bool XmlDeclaration::Save(File &cFile, uint32 nDepth)
{
	cFile.Print("<?xml ");

	if (m_sVersion.GetLength())
		cFile.Print("version=\"" + m_sVersion + "\" ");
	if (m_sEncoding.GetLength())
		cFile.Print("encoding=\"" + m_sEncoding + "\" ");
	if (m_sStandalone.GetLength())
		cFile.Print("standalone=\"" + m_sStandalone + "\" ");
	cFile.Print("?>");

	// Done
	return true;
}

String XmlDeclaration::ToString(uint32 nDepth) const
{
	String sXml = "<?xml ";

	if (m_sVersion.GetLength())
		sXml += "version=\"" + m_sVersion + "\" ";
	if (m_sEncoding.GetLength())
		sXml += "encoding=\"" + m_sEncoding + "\" ";
	if (m_sStandalone.GetLength())
		sXml += "standalone=\"" + m_sStandalone + "\" ";
	sXml += "?>";

	// Done
	return sXml;
}

const char *XmlDeclaration::Parse(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	pszData = SkipWhiteSpace(pszData, nEncoding);

	// Find the beginning, find the end, and look for the stuff in-between
	if (!pszData || !*pszData || !StringEqual(pszData, "<?xml", true, nEncoding)) {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorParsingDeclaration, 0, 0, nEncoding);

		// Error!
		return nullptr;
	}
	if (pData) {
		pData->Stamp(pszData, nEncoding);
		m_cCursor = pData->Cursor();
	}
	pszData += 5;

	m_sVersion    = "";
	m_sEncoding   = "";
	m_sStandalone = "";

	while (pszData && *pszData) {
		if (*pszData == '>') {
			++pszData;

			// Done
			return pszData;
		}

		pszData = SkipWhiteSpace(pszData, nEncoding);

		// "version"
		if (StringEqual(pszData, "version", true, nEncoding)) {
			XmlAttribute attrib;
			pszData = attrib.Parse(pszData, pData, nEncoding);
			m_sVersion = attrib.GetValue();

		// "nEncoding"
		} else if (StringEqual(pszData, "nEncoding", true, nEncoding)) {
			XmlAttribute attrib;
			pszData = attrib.Parse(pszData, pData, nEncoding);
			m_sEncoding = attrib.GetValue();

		// "standalone"
		} else if (StringEqual(pszData, "standalone", true, nEncoding)) {
			XmlAttribute attrib;
			pszData = attrib.Parse(pszData, pData, nEncoding);
			m_sStandalone = attrib.GetValue();

		} else {
			// Read over whatever it is
			while(pszData && *pszData && *pszData != '>' && !IsWhiteSpace(*pszData))
				++pszData;
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public virtual XmlNode functions                      ]
//[-------------------------------------------------------]
XmlNode *XmlDeclaration::Clone() const
{
	return new XmlDeclaration(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
