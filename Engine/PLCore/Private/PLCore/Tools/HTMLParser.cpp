/*********************************************************\
 *  File: HTMLParser.cpp                                 *
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
#include "PLCore/String/RegEx.h"
#include "PLCore/Xml/XmlDocument.h"
#include "PLCore/Xml/XmlDeclaration.h"
#include "PLCore/Xml/XmlElement.h"
#include "PLCore/Xml/XmlText.h"
#include "PLCore/Tools/HTMLParser.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clear all data
*/
void HTMLParser::Clear()
{
	// Unload text
	m_sText		 = "";
	m_sTextLower = "";
	m_nPos		 = 0;
	m_sToken	 = "";

	// Reset parsed HTML
	if (m_pXML) {
		delete m_pXML;
		m_pXML = nullptr;
	}
	m_nErrors = 0;
}

/**
*  @brief
*    Load file
*/
void HTMLParser::Load(const String &sFilename)
{
	// Destroy old data
	Clear();

	// Open file
	File cFile(sFilename);
	if (cFile.Open(File::FileRead)) {
		// Read file to string (the string takes control over the allocated memory)
		uint32 nSize = cFile.GetSize();
		if (nSize == 0)
			nSize = 64 * 1024;
		char *pszBuffer = new char[nSize + 1];
		nSize = cFile.Read(pszBuffer, 1, nSize);
		pszBuffer[nSize] = '\0';
		m_sText = String(pszBuffer, false, nSize);
		m_sTextLower = m_sText; m_sTextLower.ToLower();

		// Close file
		cFile.Close();

		// Parse file
		Parse();
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse whole HTML file and generate a XML parsing tree
*/
bool HTMLParser::Parse()
{
	// Regular expressions
	static RegEx cRegExTag("<\\s*(?<name>[^\\s/>]+)(\\s+(?<options>((\\w+)(\\s*=\\s*(('[^']*')|(\"[^\"]*\")|([^\\s]*)))?\\s*)*))?\\s*(?<end>/?)>");
	static RegEx cRegExEndTag("<\\s*/(?<name>[^\\s/>]+)\\s*>");
	static RegEx cRegExOption("(?<option>\\w+)(\\s*=\\s*(?<value>('[^']*')|(\"[^\"]*\")|([^\\s]*)))?");
	static RegEx cRegExComment("<!--");
	static RegEx cRegExDoctype("<![^>]*>");

	// Create XML document
	if (m_pXML)
		delete m_pXML;
	m_pXML = new XmlDocument();
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	m_pXML->LinkEndChild(*pDeclaration);
	XmlNode *pNode = m_pXML;
	XmlText *pText = nullptr;

	// Parse whole file
	while (HasNextToken()) {
		// Get next token
		const String sToken = GetNextToken();

		// <!-- comment -->
		if (cRegExComment.Match(sToken)) {
			// Ignore comment
		}

		// <!DOCTYPE >
		else if (cRegExDoctype.Match(sToken)) {
			// Ignore doctype
		}

		// <Tag>
		else if (cRegExTag.Match(sToken)) {
			// End text node
			pText = nullptr;

			// Get name
			const String sName = cRegExTag.GetNameResult("name");
			const bool bSingleTag = (cRegExTag.GetNameResult("end") == '/');
			if (sName == "script") {
				// Ignore <script> tags
				const int nIndex = m_sTextLower.IndexOf("</script>", m_nPos);
				if (nIndex > -1)
					m_nPos = nIndex + 9;
			} else {
				// Add element
				XmlElement *pElement = new XmlElement(sName);
				pNode->LinkEndChild(*pElement);
				if (!bSingleTag && !IsSingleTag(sName)) {
					// Nest further elements into this one
					pNode = pElement;
				}

				// Get options
				const String sOptions = cRegExTag.GetNameResult("options");

				// Parse options
				int nPosOption = 0;
				while (cRegExOption.Match(sOptions, nPosOption)) {
					// Get option and value
					const String sOption = cRegExOption.GetNameResult("option");
					String sValue = cRegExOption.GetNameResult("value");
					if (sValue.GetSubstring(0, 1) == '\'' || sValue.GetSubstring(0, 1) == '\"')
						sValue = sValue.GetSubstring(1, sValue.GetLength()-2);
					nPosOption = cRegExOption.GetPosition();
					pElement->SetAttribute(sOption, sValue);
				}
			}
		}

		// </Tag>
		else if (cRegExEndTag.Match(sToken)) {
			// End text node
			pText = nullptr;

			// Get name
			const String sName = cRegExEndTag.GetNameResult("name");

			// Now it counts: name should be same as the current elements name. If it is not, we search the
			// tree upwards until we find a node with the right name. If we don't, we ignore this end-tag
			XmlNode *pCloseNode = pNode;
			if (sName != pCloseNode->GetValue()) {
				// Error, wrong closing tag. Search for a one that fits
				while (pCloseNode && pCloseNode->GetValue() != sName)
					pCloseNode = pCloseNode->GetParent();
				m_nErrors++;
			}

			// Close node
			if (pCloseNode) {
				// Make parent of closed node to our current node
				pNode = pCloseNode->GetParent();
				if (!pNode)
					pNode = m_pXML;
			}
		}

		// Text
		else {
			// Create a new text mode if necessary
			if (!pText) {
				pText = new XmlText("");
				pNode->LinkEndChild(*pText);
			}

			// Add text to node
			pText->SetValue(pText->GetValue() + ' ' + sToken);
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Check if there is another token waiting
*/
bool HTMLParser::HasNextToken()
{
	// If there is no token, try to parse next one
	if (!m_sToken.GetLength())
		m_sToken = GetNextToken();

	// Check if there is a token waiting
	return (m_sToken.GetLength() != 0);
}

/**
*  @brief
*    Get next token from HTML parser
*/
String HTMLParser::GetNextToken()
{
	// Regular expressions
	static RegEx cRegExToken("(([^\\s\\h\\v\"<>]+)|(\"[^\"]*\")|(<[^>]*>))");

	// Is there a token waiting?
	if (m_sToken.GetLength()) {
		// Return waiting token
		String sToken = m_sToken;
		m_sToken = "";
		return sToken;
	}

	// Parse token
	String sToken;
	if (cRegExToken.Match(m_sText, m_nPos)) {
		// Get next token
		sToken = cRegExToken.GetResult(0);
		m_nPos = cRegExToken.GetPosition();
	}

	// Return token
	return sToken;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
