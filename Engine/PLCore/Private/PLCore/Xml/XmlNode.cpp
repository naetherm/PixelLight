/*********************************************************\
 *  File: XmlNode.cpp                                    *
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
#include "PLCore/Xml/XmlDeclaration.h"
#include "PLCore/Xml/XmlDocument.h"
#include "PLCore/Xml/XmlUnknown.h"
#include "PLCore/Xml/XmlComment.h"
#include "PLCore/Xml/XmlElement.h"
#include "PLCore/Xml/XmlText.h"
#include "PLCore/Xml/XmlNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
XmlNode::~XmlNode()
{
	// Delete all the children of this node
	Clear();
}

/**
*  @brief
*    Delete all the children of this node
*/
void XmlNode::Clear()
{
	XmlNode *pNode = m_pFirstChild;
	while (pNode) {
		XmlNode *pTempNode = pNode;
		pNode = pNode->m_pNextSibling;
		delete pTempNode;
	}
	m_pFirstChild = nullptr;
	m_pLastChild  = nullptr;
}

/**
*  @brief
*    Returns the first child of this node with the matching 'value'
*/
XmlNode *XmlNode::GetFirstChild(const String &sValue)
{
	for (XmlNode *pNode=m_pFirstChild; pNode; pNode=pNode->m_pNextSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}
	return nullptr;
}

const XmlNode *XmlNode::GetFirstChild(const String &sValue) const
{
	for (const XmlNode *pNode=m_pFirstChild; pNode; pNode=pNode->m_pNextSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}
	return nullptr;
}

/**
*  @brief
*    Returns the last child of this node with the matching 'value'
*/
XmlNode *XmlNode::GetLastChild(const String &sValue)
{
	for (XmlNode *pNode=m_pLastChild; pNode; pNode=pNode->m_pPreviousSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}
	return nullptr;
}

const XmlNode *XmlNode::GetLastChild(const String &sValue) const
{
	for (const XmlNode *pNode=m_pLastChild; pNode; pNode=pNode->m_pPreviousSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}
	return nullptr;
}

/**
*  @brief
*    Add a new node related to this (adds a child past the last child)
*/
XmlNode *XmlNode::InsertEndChild(const XmlNode &cAddThis)
{
	// Insertion of documents is NOT allowed!
	if (cAddThis.GetType() != Document) {
		// Clone the given node
		XmlNode *pNode = cAddThis.Clone();
		if (pNode)
			return LinkEndChild(*pNode);
	} else {
		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorDocumentTopOnly, 0, 0, EncodingUnknown);
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Add a new node related to this (adds a child past the last child)
*/
XmlNode *XmlNode::LinkEndChild(XmlNode &cAddThis)
{
	// Linking of documents is NOT allowed!
	if (cAddThis.GetType() != Document) {
		cAddThis.m_pParent			= this;
		cAddThis.m_pPreviousSibling	= m_pLastChild;
		cAddThis.m_pNextSibling		= nullptr;
		if (m_pLastChild)
			m_pLastChild->m_pNextSibling = &cAddThis;
		else
			m_pFirstChild = &cAddThis;	// It was an empty list
		m_pLastChild = &cAddThis;

		// Done
		return &cAddThis;
	} else {
		// Delete the given node
		delete &cAddThis;

		// Set error code
		XmlDocument *pDocument = GetDocument();
		if (pDocument)
			pDocument->SetError(ErrorDocumentTopOnly, 0, 0, EncodingUnknown );

		// Error!
		return nullptr;
	}
}

/**
*  @brief
*    Add a new node related to this (adds a child before the specified child)
*/
XmlNode *XmlNode::InsertBeforeChild(XmlNode &cBeforeThis, const XmlNode &cAddThis)
{
	// Both nodes must have one and the same parent!
	if (cBeforeThis.m_pParent == this) {
		// Insertion of documents is NOT allowed!
		if (cAddThis.GetType() != Document) {
			// Clone the given node
			XmlNode *pNode = cAddThis.Clone();
			if (pNode) {
				pNode->m_pParent		  = this;
				pNode->m_pNextSibling	  = &cBeforeThis;
				pNode->m_pPreviousSibling = cBeforeThis.m_pPreviousSibling;
				if (cBeforeThis.m_pPreviousSibling)
					cBeforeThis.m_pPreviousSibling->m_pNextSibling = pNode;
				else
					m_pFirstChild = pNode;
				cBeforeThis.m_pPreviousSibling = pNode;

				// Done
				return pNode;
			}
		} else {
			// Set error code
			XmlDocument *pDocument = GetDocument();
			if (pDocument)
				pDocument->SetError(ErrorDocumentTopOnly, 0, 0, EncodingUnknown);
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Add a new node related to this (adds a child after the specified child)
*/
XmlNode *XmlNode::InsertAfterChild(XmlNode &cAfterThis, const XmlNode &cAddThis)
{
	// Both nodes must have one and the same parent!
	if (cAfterThis.m_pParent == this) {
		// Insertion of documents is NOT allowed!
		if (cAddThis.GetType() != Document) {
			// Clone the given node
			XmlNode *pNode = cAddThis.Clone();
			if (pNode) {
				pNode->m_pParent		  = this;
				pNode->m_pPreviousSibling = &cAfterThis;
				pNode->m_pNextSibling	  = cAfterThis.m_pNextSibling;
				if (cAfterThis.m_pNextSibling)
					cAfterThis.m_pNextSibling->m_pPreviousSibling = pNode;
				else
					m_pLastChild = pNode;
				cAfterThis.m_pNextSibling = pNode;

				// Done
				return pNode;
			}
		} else {
			// Set error code
			XmlDocument *pDocument = GetDocument();
			if (pDocument)
				pDocument->SetError(ErrorDocumentTopOnly, 0, 0, EncodingUnknown);
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Replace a child of this node
*/
XmlNode *XmlNode::ReplaceChild(XmlNode &cReplaceThis, const XmlNode &cWithThis)
{
	// Both nodes must have one and the same parent!
	if (cReplaceThis.m_pParent == this) {
		// Insertion of documents is NOT allowed!
		if (cWithThis.GetType() != Document) {
			// Clone the given node
			XmlNode *pNode = cWithThis.Clone();
			if (pNode) {
				pNode->m_pNextSibling	  = cReplaceThis.m_pNextSibling;
				pNode->m_pPreviousSibling = cReplaceThis.m_pPreviousSibling;
				if (cReplaceThis.m_pNextSibling)
					cReplaceThis.m_pNextSibling->m_pPreviousSibling = pNode;
				else
					m_pLastChild = pNode;
				if (cReplaceThis.m_pPreviousSibling)
					cReplaceThis.m_pPreviousSibling->m_pNextSibling = pNode;
				else
					m_pFirstChild = pNode;
				pNode->m_pParent = this;

				// Destroy the replaced node
				delete &cReplaceThis;

				// Done
				return pNode;
			}
		} else {
			// Set error code
			XmlDocument *pDocument = GetDocument();
			if (pDocument)
				pDocument->SetError(ErrorDocumentTopOnly, 0, 0, EncodingUnknown);
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Delete a child of this node
*/
bool XmlNode::RemoveChild(XmlNode &cRemoveThis)
{
	// Both nodes must have one and the same parent!
	if (cRemoveThis.m_pParent == this) {
		if (cRemoveThis.m_pNextSibling)
			cRemoveThis.m_pNextSibling->m_pPreviousSibling = cRemoveThis.m_pPreviousSibling;
		else
			m_pLastChild = cRemoveThis.m_pPreviousSibling;
		if (cRemoveThis.m_pPreviousSibling)
			cRemoveThis.m_pPreviousSibling->m_pNextSibling = cRemoveThis.m_pNextSibling;
		else
			m_pFirstChild = cRemoveThis.m_pNextSibling;

		// Destroy the removed node
		delete &cRemoveThis;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Navigate to a sibling node with the given 'value'
*/
XmlNode *XmlNode::GetPreviousSibling(const String &sValue)
{
	for (XmlNode *pNode=m_pPreviousSibling; pNode; pNode=pNode->m_pPreviousSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;

	}
	// There's no previous sibling
	return nullptr;
}

const XmlNode *XmlNode::GetPreviousSibling(const String &sValue) const
{
	for (const XmlNode *pNode=m_pPreviousSibling; pNode; pNode=pNode->m_pPreviousSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}

	// There's no previous sibling
	return nullptr;
}

/**
*  @brief
*    Navigate to a sibling node with the given 'value'
*/
XmlNode *XmlNode::GetNextSibling(const String &sValue)
{
	for (XmlNode *pNode=m_pNextSibling; pNode; pNode=pNode->m_pNextSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}

	// There's no next sibling
	return nullptr;
}

const XmlNode *XmlNode::GetNextSibling(const String &sValue) const
{
	for (const XmlNode *pNode=m_pNextSibling; pNode; pNode=pNode->m_pNextSibling) {
		if (pNode->m_sValue == sValue)
			return pNode;
	}

	// There's no next sibling
	return nullptr;
}

/**
*  @brief
*    Convenience function to get through elements
*/
const XmlElement *XmlNode::GetNextSiblingElement() const
{
	for (const XmlNode *pNode=GetNextSibling(); pNode; pNode=pNode->GetNextSibling()) {
		if (pNode->ToElement())
			return pNode->ToElement();
	}

	// There's no next sibling element
	return nullptr;
}

/**
*  @brief
*    Convenience function to get through elements
*/
XmlElement *XmlNode::GetNextSiblingElement(const String &sValue)
{
	for (XmlNode *pNode=GetNextSibling(sValue); pNode; pNode=pNode->GetNextSibling(sValue)) {
		if (pNode->ToElement())
			return pNode->ToElement();
	}

	// There's no next sibling element
	return nullptr;
}

const XmlElement *XmlNode::GetNextSiblingElement(const String &sValue) const
{
	for (const XmlNode *pNode=GetNextSibling(sValue); pNode; pNode=pNode->GetNextSibling(sValue)) {
		if (pNode->ToElement())
			return pNode->ToElement();
	}

	// There's no next sibling element
	return nullptr;
}

/**
*  @brief
*    Convenience function to get through elements
*/
const XmlElement *XmlNode::GetFirstChildElement() const
{
	for (const XmlNode *pNode=GetFirstChild(); pNode; pNode=pNode->GetNextSibling()) {
		if (pNode->ToElement())
			return pNode->ToElement();
	}

	// There's no first child element
	return nullptr;
}

/**
*  @brief
*    Convenience function to get through elements
*/
const XmlElement *XmlNode::GetFirstChildElement(const String &sValue) const
{
	for (const XmlNode *pNode=GetFirstChild(sValue); pNode; pNode=pNode->GetNextSibling(sValue)) {
		if (pNode->ToElement())
			return pNode->ToElement();
	}

	// There's no first child element
	return nullptr;
}

/**
*  @brief
*    Return a pointer to the document this node lives in
*/
const XmlDocument *XmlNode::GetDocument() const
{
	for (const XmlNode *pNode=this; pNode; pNode=pNode->m_pParent) {
		if (pNode->ToDocument())
			return pNode->ToDocument();
	}
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlNode::XmlNode(ENodeType nType) :
	m_pParent(nullptr),
	m_nType(nType),
	m_pFirstChild(nullptr),
	m_pLastChild(nullptr),
	m_pPreviousSibling(nullptr),
	m_pNextSibling(nullptr)
{
}

/**
*  @brief
*    Figure out what is at *pszStart, and parse it
*/
XmlNode *XmlNode::Identify(const char *pszData, EEncoding nEncoding)
{
	pszData = SkipWhiteSpace(pszData, nEncoding);
	if (!pszData || !*pszData || *pszData != '<')
		return nullptr; // Error!

	pszData = SkipWhiteSpace(pszData, nEncoding);
	if (!pszData || !*pszData)
		return nullptr; // Error!

	// What is this thing?
	// - Elements start with a letter or underscore, but XML is reserved
	// - Comments: <!--
	// - Declaration: <?xml
	// - Everything else is unknown to the XML parser
	static const char *pszXmlHeader		= "<?xml";
	static const char *pszCommentHeader	= "<!--";
	static const char *pszDtdHeader		= "<!";
	static const char *pszCDataHeader	= "<![CDATA[";

	XmlNode *pReturnNode = nullptr;
	if (StringEqual(pszData, pszXmlHeader, true, nEncoding))
		pReturnNode = new XmlDeclaration();
	else if (StringEqual(pszData, pszCommentHeader, false, nEncoding))
		pReturnNode = new XmlComment();
	else if (StringEqual(pszData, pszCDataHeader, false, nEncoding)) {
		XmlText *pTextNode = new XmlText();
		pTextNode->SetCDATA(true);
		pReturnNode = pTextNode;
	} else if (StringEqual(pszData, pszDtdHeader, false, nEncoding))
		pReturnNode = new XmlUnknown();
	else if (IsAlpha(*(pszData+1)) || *(pszData+1) == '_')
		pReturnNode = new XmlElement("");
	else
		pReturnNode = new XmlUnknown();

	// Set the parent, so it can report errors
	if (pReturnNode)
		pReturnNode->m_pParent = this;

	// Done
	return pReturnNode;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
XmlNode::XmlNode(const XmlNode &cSource) :
	m_pParent(nullptr),
	m_nType(Unknown),
	m_pFirstChild(nullptr),
	m_pLastChild(nullptr),
	m_pPreviousSibling(nullptr),
	m_pNextSibling(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
XmlNode &XmlNode::operator =(const XmlNode &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
