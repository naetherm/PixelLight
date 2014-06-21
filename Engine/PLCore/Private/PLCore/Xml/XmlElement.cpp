/*********************************************************\
 *  File: XmlElement.cpp                                 *
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
#include "PLCore/Xml/XmlAttribute.h"
#include "PLCore/Xml/XmlDocument.h"
#include "PLCore/Xml/XmlText.h"
#include "PLCore/Xml/XmlElement.h"


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
XmlElement::XmlElement(const String &sValue) : XmlNode(Element)
{
	m_sValue = sValue;
}

/**
*  @brief
*    Copy constructor
*/
XmlElement::XmlElement(const XmlElement &cSource) : XmlNode(Element)
{
	*this = cSource;
}

/**
*  @brief
*    Destructor
*/
XmlElement::~XmlElement()
{
	ClearThis();
}

/**
*  @brief
*    Copy operator
*/
XmlElement &XmlElement::operator =(const XmlElement &cSource)
{
	ClearThis();
	m_sValue    = cSource.m_sValue;
	m_pUserData = cSource.m_pUserData;
	m_cCursor   = cSource.m_cCursor;

	// Clone the attributes
	for (const XmlAttribute *pAttribute=cSource.m_cAttributeSet.GetFirst(); pAttribute; pAttribute=pAttribute->GetNext())
		SetAttribute(pAttribute->GetName(), pAttribute->GetValue());

	// Clone the children
	for (const XmlNode *pNode=cSource.GetFirstChild(); pNode; pNode=pNode->GetNextSibling()) {
		XmlNode *pClone = pNode->Clone();
		if (pClone)
			LinkEndChild(*pClone);
	}

	// Return a reference to this instance
	return *this;
}

/**
*  @brief
*    Given an attribute name, 'GetAttribute()' returns the value
*    for the attribute of that name, or a null pointer if none exists
*/
String XmlElement::GetAttribute(const String &sName) const
{
	const XmlAttribute *pNode = m_cAttributeSet.Find(sName);
	return pNode ? pNode->GetValue() : "";
}

/**
*  @brief
*    Given an attribute name, 'GetAttribute()' returns the value
*    for the attribute of that name, or a null pointer if none exists
*/
String XmlElement::GetAttribute(const String &sName, int *pnValue) const
{
	const XmlAttribute *pAttribute = m_cAttributeSet.Find(sName);
	if (pAttribute) {
		const String sResult = pAttribute->GetValue();
		if (pnValue)
			pAttribute->QueryIntValue(*pnValue);
		return sResult;
	}
	return "";
}

/**
*  @brief
*    Given an attribute name, 'GetAttribute()' returns the value
*    for the attribute of that name, or a null pointer if none exists
*/
String XmlElement::GetAttribute(const String &sName, double *pdValue) const
{
	const XmlAttribute *pAttribute = m_cAttributeSet.Find(sName);
	if (pAttribute) {
		const String sResult = pAttribute->GetValue();
		if (pdValue)
			pAttribute->QueryDoubleValue(*pdValue);
		return sResult;
	}
	return "";
}

/**
*  @brief
*    Examines the attribute
*/
XmlBase::EQueryResult XmlElement::QueryIntAttribute(const String &sName, int *pnValue) const
{
	const XmlAttribute *pAttribute = m_cAttributeSet.Find(sName);
	if (pAttribute)
		return pnValue ? pAttribute->QueryIntValue(*pnValue) : Success;
	else
		return NoAttribute;	// There's no attribute with the given name
}

/**
*  @brief
*    Examines the attribute
*/
XmlBase::EQueryResult XmlElement::QueryFloatAttribute(const String &sName, float *pfValue) const
{
	const XmlAttribute *pAttribute = m_cAttributeSet.Find(sName);
	if (pAttribute) {
		if (pfValue) {
			double d = 0.0f;
			EQueryResult nResult = pAttribute->QueryDoubleValue(d);
			*pfValue = static_cast<float>(d);
			return nResult;
		} else {
			return Success;
		}
	} else {
		// There's no attribute with the given name
		return NoAttribute;
	}
}

/**
*  @brief
*    Examines the attribute
*/
XmlBase::EQueryResult XmlElement::QueryDoubleAttribute(const String &sName, double *pdValue) const
{
	const XmlAttribute *pAttribute = m_cAttributeSet.Find(sName);
	if (pAttribute)
		return pdValue ? pAttribute->QueryDoubleValue(*pdValue) : Success;
	else
		return NoAttribute; // There's no attribute with the given name
}

/**
*  @brief
*    Sets an attribute of name to a given value
*/
void XmlElement::SetAttribute(const String &sName, const String &sValue)
{
	XmlAttribute *pAttribute = m_cAttributeSet.FindOrCreate(sName);
	if (pAttribute)
		pAttribute->SetValue(sValue);
}

/**
*  @brief
*    Sets an attribute of name to a given value
*/
void XmlElement::SetAttribute(const String &sName, int nValue)
{
	XmlAttribute *pAttribute = m_cAttributeSet.FindOrCreate(sName);
	if (pAttribute)
		pAttribute->SetIntValue(nValue);
}

/**
*  @brief
*    Sets an attribute of name to a given value
*/
void XmlElement::SetDoubleAttribute(const String &sName, double dValue)
{
	XmlAttribute *pAttribute = m_cAttributeSet.FindOrCreate(sName);
	if (pAttribute)
		pAttribute->SetDoubleValue(dValue);
}

/**
*  @brief
*    Deletes an attribute with the given name
*/
void XmlElement::RemoveAttribute(const String &sName)
{
	XmlAttribute *pAttribute = m_cAttributeSet.Find(sName);
	if (pAttribute) {
		m_cAttributeSet.Remove(*pAttribute);
		delete pAttribute;
	}
}

/**
*  @brief
*    Convenience function for easy access to the text inside an element
*/
String XmlElement::GetText() const
{
	const XmlNode *pChild = GetFirstChild();
	if (pChild) {
		const XmlText *pChildText = pChild->ToText();
		if (pChildText)
			return pChildText->GetValue();
	}
	return "";
}


//[-------------------------------------------------------]
//[ Public virtual XmlBase functions                      ]
//[-------------------------------------------------------]
bool XmlElement::Save(File &cFile, uint32 nDepth)
{
	/// Get the number of empty spaces
	const XmlDocument *pDocument = GetDocument();
	const uint32 nNumOfSpaces = (pDocument ? pDocument->GetTabSize() : 4) * nDepth;

	// Print empty spaces
	for (uint32 i=0; i<nNumOfSpaces; i++)
		cFile.PutC(' ');

	// Print value
	cFile.Print('<' + m_sValue);

	// Print attributes
	for (XmlAttribute *pAttribute=GetFirstAttribute(); pAttribute; pAttribute=pAttribute->GetNext()) {
		cFile.PutC(' ');
		pAttribute->Save(cFile, nDepth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	if (!GetFirstChild())
		cFile.Print(" />");
	else if (GetFirstChild() == GetLastChild() && GetFirstChild()->GetType() == Text) {
		cFile.PutC('>');
		GetFirstChild()->Save(cFile, nDepth+1);
		cFile.Print("</" + m_sValue + '>');
	} else {
		cFile.PutC('>');

		for (XmlNode *pNode=GetFirstChild(); pNode; pNode=pNode->GetNextSibling()) {
			if (pNode->GetType() != Text)
				cFile.PutC('\n');
			pNode->Save(cFile, nDepth+1);
		}
		cFile.PutC('\n');

		// Print empty spaces
		for (uint32 i=0; i<nNumOfSpaces; i++)
			cFile.PutC(' ');

		// Print value
		cFile.Print("</" + m_sValue + '>');
	}

	// Done
	return true;
}

String XmlElement::ToString(uint32 nDepth) const
{
	// Get the number of empty spaces
	const XmlDocument *pDocument = GetDocument();
	const uint32 nNumOfSpaces = (pDocument ? pDocument->GetTabSize() : 4) * nDepth;

	// Print empty spaces
	String sXml;
	for (uint32 i=0; i<nNumOfSpaces; i++)
		sXml += ' ';

	// Print value
	sXml += '<' + m_sValue;

	// Print attributes
	for (const XmlAttribute *pAttribute=GetFirstAttribute(); pAttribute; pAttribute=pAttribute->GetNext())
		sXml += ' ' + pAttribute->ToString(nDepth);

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	if (!GetFirstChild())
		sXml += " />";
	else if (GetFirstChild() == GetLastChild() && GetFirstChild()->GetType() == Text) {
		sXml += '>' + GetFirstChild()->ToString(nDepth+1) + "</" + m_sValue + '>';
	} else {
		sXml += '>';

		for (const XmlNode *pNode=GetFirstChild(); pNode; pNode=pNode->GetNextSibling()) {
			if (pNode->GetType() != Text)
				sXml += '\n';
			sXml += pNode->ToString(nDepth+1);
		}
		sXml += '\n';

		// Print empty spaces
		for (uint32 i=0; i<nNumOfSpaces; i++)
			sXml += ' ';

		// Print value
		sXml += "</" + m_sValue + '>';
	}

	// Done
	return sXml;
}

const char *XmlElement::Parse(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	pszData = SkipWhiteSpace(pszData, nEncoding);
	if (!pszData || !*pszData) {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorParsingElement, 0, 0, nEncoding);

		// Error!
		return nullptr;
	}

	if (pData) {
		pData->Stamp(pszData, nEncoding);
		m_cCursor = pData->Cursor();
	}

	if (*pszData != '<') {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorParsingElement, pszData, pData, nEncoding);

		// Error!
		return nullptr;
	}
	pszData = SkipWhiteSpace(pszData + 1, nEncoding);

	// Read the name
	const char *pszError = pszData;
	pszData = ReadName(pszData, m_sValue, nEncoding);
	if (!pszData || !*pszData) {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorFailedToReadElementName, pszError, pData, nEncoding);

		// Error!
		return nullptr;
	}

	String sEndTag = "</";
	sEndTag += m_sValue;

	// Check for and read attributes. Also look for an empty tag or an end tag
	while (pszData && *pszData) {
		pszError = pszData;
		pszData = SkipWhiteSpace(pszData, nEncoding);
		if (!pszData || !*pszData) {
			// Set error code
			XmlDocument *pDocument = GetDocument();
			if (pDocument)
				pDocument->SetError(ErrorReadingAttributes, pszError, pData, nEncoding);

			// Error!
			return nullptr;
		}
		if (*pszData == '/') {
			++pszData;

			// Empty tag
			if (*pszData != '>') {
				// Set error code
				XmlDocument *pDocument = GetDocument();
				if (pDocument)
					pDocument->SetError(ErrorParsingEmpty, pszData, pData, nEncoding);

				// Error!
				return nullptr;
			}
			return (pszData + 1);
		} else if (*pszData == '>') {
			// Done with attributes (if there were any)
			// Read the value -- which can include other elements -- read the end tag, and return
			++pszData;
			pszData = ReadValue(pszData, pData, nEncoding);	// Note this is an Element method, and will set the error if one happens
			if (!pszData || !*pszData) {
				// We were looking for the end tag, but found nothing
				XmlDocument *pDocument = GetDocument();
				if (pDocument)
					pDocument->SetError(ErrorReadingEndTag, pszData, pData, nEncoding);

				// Error!
				return nullptr;
			}

			// We should find the end tag now
			// Note that:
			// </foo > and
			// </foo>
			// are both valid end tags
			if (StringEqual(pszData, sEndTag, false, nEncoding)) {
				pszData += sEndTag.GetLength();
				pszData = SkipWhiteSpace(pszData, nEncoding);
				if (pszData && *pszData && *pszData == '>') {
					++pszData;
					return pszData;
				}

				// Set error code
				XmlDocument *pDocument = GetDocument();
				if (pDocument)
					pDocument->SetError(ErrorReadingEndTag, pszData, pData, nEncoding);

				// Error!
				return nullptr;
			} else {
				// Set error code
				XmlDocument *pDocument = GetDocument();
				if (pDocument)
					pDocument->SetError(ErrorReadingEndTag, pszData, pData, nEncoding);

				// Error!
				return nullptr;
			}
		} else {
			// Try to read an attribute
			XmlAttribute *pAttribute = new XmlAttribute();
			pAttribute->m_pDocument = GetDocument();
			pszError = pszData;
			pszData = pAttribute->Parse(pszData, pData, nEncoding);
			if (!pszData || !*pszData) {
				// Set error code
				XmlDocument *pDocument = GetDocument();
				if (pDocument)
					pDocument->SetError(ErrorParsingElement, pszError, pData, nEncoding);

				// Destroy the created attribute
				delete pAttribute;

				// Error!
				return nullptr;
			}

			// Handle the strange case of double attributes
			XmlAttribute *pNode = m_cAttributeSet.Find(pAttribute->GetName());
			if (pNode) {
				// Set error code
				XmlDocument *pDocument = GetDocument();
				if (pDocument)
					pDocument->SetError(ErrorParsingElement, pszError, pData, nEncoding);

				// Destroy the created attribute
				delete pAttribute;

				// Error!
				return nullptr;
			}

			// Register the created attribute
			m_cAttributeSet.Add(*pAttribute);
		}
	}

	// Done
	return pszData;
}


//[-------------------------------------------------------]
//[ Public virtual XmlNode functions                      ]
//[-------------------------------------------------------]
XmlNode *XmlElement::Clone() const
{
	return new XmlElement(*this);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Like clear, but initializes 'this' object as well
*/
void XmlElement::ClearThis()
{
	Clear();
	while (m_cAttributeSet.GetFirst()) {
		XmlAttribute *pAttribute = m_cAttributeSet.GetFirst();
		m_cAttributeSet.Remove(*pAttribute);
		delete pAttribute;
	}
}

/**
*  @brief
*    Reads the "value" of the element -- another element, or text
*/
const char *XmlElement::ReadValue(const char *pszData, XmlParsingData *pData, EEncoding nEncoding)
{
	// Read in text and elements in any order
	const char *pWithWhiteSpace = pszData;
	pszData = SkipWhiteSpace(pszData, nEncoding);

	while (pszData && *pszData) {
		if (*pszData != '<') {
			// Take what we have, make a text element
			XmlText *pTextNode = new XmlText("");

			if (IsWhiteSpaceCondensed())
				pszData = pTextNode->Parse(pszData, pData, nEncoding);
			else {
				// Special case: we want to keep the white space so that leading spaces aren't removed
				pszData = pTextNode->Parse(pWithWhiteSpace, pData, nEncoding);
			}

			// Does the text value only contain white spaces?
			bool bIsBlank = true;
			{
				const String sValue = pTextNode->GetValue();
				for (uint32 i=0; i<sValue.GetLength(); i++) {
					if (!IsWhiteSpace(sValue[i])) {
						bIsBlank = false;
						break;
					}
				}
			}
			if (bIsBlank)
				delete pTextNode;
			else
				LinkEndChild(*pTextNode);
		} else {
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be a XmlText in the "CDATA" style
			if (StringEqual(pszData, "</", false, nEncoding))
				return pszData;
			else {
				XmlNode *pNode = Identify(pszData, nEncoding);
				if (pNode) {
					pszData = pNode->Parse(pszData, pData, nEncoding);
					LinkEndChild(*pNode);
				} else {
					return nullptr;
				}
			}
		}
		pWithWhiteSpace = pszData;
		pszData = SkipWhiteSpace(pszData, nEncoding);
	}

	if (!pszData) {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorReadingElementValue, 0, 0, nEncoding);
	}

	// Done
	return pszData;
}




//[-------------------------------------------------------]
//[ Private XmlAttributeSet class                         ]
//[-------------------------------------------------------]
XmlElement::XmlAttributeSet::XmlAttributeSet()
{
	cSentinel.m_pNextAttribute     = &cSentinel;
	cSentinel.m_pPreviousAttribute = &cSentinel;
}

XmlElement::XmlAttributeSet::~XmlAttributeSet()
{
}

void XmlElement::XmlAttributeSet::Add(XmlAttribute &cAttribute)
{
	cAttribute.m_pNextAttribute						 = &cSentinel;
	cAttribute.m_pPreviousAttribute					 = cSentinel.m_pPreviousAttribute;
	cSentinel.m_pPreviousAttribute->m_pNextAttribute = &cAttribute;
	cSentinel.m_pPreviousAttribute					 = &cAttribute;
}

void XmlElement::XmlAttributeSet::Remove(XmlAttribute &cAttribute)
{
	for (XmlAttribute *pAttribute=cSentinel.m_pNextAttribute; pAttribute!=&cSentinel; pAttribute=pAttribute->m_pNextAttribute) {
		if (pAttribute == &cAttribute) {
			pAttribute->m_pPreviousAttribute->m_pNextAttribute = pAttribute->m_pNextAttribute;
			pAttribute->m_pNextAttribute->m_pPreviousAttribute = pAttribute->m_pPreviousAttribute;
			pAttribute->m_pNextAttribute					   = nullptr;
			pAttribute->m_pPreviousAttribute				   = nullptr;

			// Get us out of here right now!
			return;
		}
	}
}

XmlAttribute *XmlElement::XmlAttributeSet::Find(const String &sName) const
{
	for (XmlAttribute *pAttribute=cSentinel.m_pNextAttribute; pAttribute!=&cSentinel; pAttribute=pAttribute->m_pNextAttribute) {
		if (pAttribute->m_sName == sName)
			return pAttribute;
	}
	return nullptr;
}

XmlAttribute *XmlElement::XmlAttributeSet::FindOrCreate(const String &sName)
{
	XmlAttribute *pAttribute = Find(sName);
	if (!pAttribute) {
		pAttribute = new XmlAttribute();
		Add(*pAttribute);
		pAttribute->SetName(sName);
	}
	return pAttribute;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
