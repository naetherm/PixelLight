/*********************************************************\
 *  File: XmlTextElement.cpp                             *
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
#include <PLCore/Log/Log.h>
#include <PLCore/File/Url.h>
#include <PLCore/String/ParseTools.h>
#include <PLCore/Xml/XmlDocument.h>
#include <PLCore/Xml/XmlElement.h>
#include <PLGui/Gui/Gui.h>
#include "PLGuiXmlText/XmlTextView.h"
#include "PLGuiXmlText/XmlTextRenderer.h"
#include "PLGuiXmlText/XmlTextElement.h"
#include "PLGuiXmlText/XmlTextBreak.h"
#include "PLGuiXmlText/XmlTextImage.h"
#include "PLGuiXmlText/XmlTextList.h"
#include "PLGuiXmlText/XmlTextListElement.h"
#include "PLGuiXmlText/XmlTextSection.h"
#include "PLGuiXmlText/XmlTextText.h"
#include "PLGuiXmlText/XmlTextTextNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLMath;
using namespace PLGui;
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextElement::XmlTextElement(XmlTextElement *pParent) :
	m_pXmlText(pParent ? pParent->m_pXmlText : nullptr),
	m_pParent(pParent)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextElement::~XmlTextElement()
{
	// Unload document
	Clear();
}

/**
*  @brief
*    Load XML file
*/
void XmlTextElement::Load(const String &sFilename)
{
	// Clear document
	Clear();

	// Load file
	XmlDocument cXml;
	if (cXml.Load(sFilename)) {
		// Get root node
		XmlNode *pRootNode = cXml.GetFirstChildElement();

		// Parse this node
		Parse(*pRootNode);
	} else {
		// Error!
		PL_LOG(Error, "Failed to load in the XML file \"" + sFilename + '\"')
	}
}

/**
*  @brief
*    Draw element and all children to screen
*/
void XmlTextElement::Draw(XmlTextRenderer &cRenderer)
{
	// Draw this element
	OnDraw(cRenderer);
}

/**
*  @brief
*    Get size of XMLText content (looks for the first section)
*/
Vector2i XmlTextElement::GetSize() const
{
	// Get size of this element
	return OnGetSize();
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clear element and all children
*/
void XmlTextElement::Clear()
{
	// Delete children
	for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
		delete m_lstChildren[i];
	}

	// Clear list
	m_lstChildren.Clear();
}

/**
*  @brief
*    Parse Xml node
*/
void XmlTextElement::Parse(XmlNode &cXmlNode)
{
	// Parse this element
	OnParse(cXmlNode);

	// Parse child elements
	XmlNode *pChildNode = cXmlNode.GetFirstChild();
	while (pChildNode) {
		// Create child element
		XmlTextElement *pChild = nullptr;
		if (pChildNode->GetType() == XmlNode::Element) {
			// We found a XML element
			XmlElement *pElement = static_cast<XmlElement*>(pChildNode);
			String sElement = pElement->GetValue();

			// Create element
				 if (sElement.CompareNoCase("Br"))			pChild = new XmlTextBreak(this);
			else if (sElement.CompareNoCase("Image"))		pChild = new XmlTextImage(this);
			else if (sElement.CompareNoCase("List"))		pChild = new XmlTextList(this);
			else if (sElement.CompareNoCase("ListElement"))	pChild = new XmlTextListElement(this);
			else if (sElement.CompareNoCase("Section"))		pChild = new XmlTextSection(this);
			else if (sElement.CompareNoCase("Text"))		pChild = new XmlTextText(true, this);
			else if (sElement.CompareNoCase("Style"))		pChild = new XmlTextText(false, this);
		} else if (pChildNode->GetType() == XmlNode::Text) {
			// Create text
			pChild = new XmlTextTextNode(this);
		}

		// Did we create a new child element?
		if (pChild) {
			// Add child element to list
			m_lstChildren.Add(pChild);

			// Parse child element
			pChild->Parse(*pChildNode);
		}

		// Get next XML node
		pChildNode = pChildNode->GetNextSibling();
	}
}

/**
*  @brief
*    Draw all children
*/
void XmlTextElement::DrawChildren(XmlTextRenderer &cRenderer)
{
	// Draw child elements
	for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
		m_lstChildren[i]->Draw(cRenderer);
	}
}

/**
*  @brief
*    Parse HTML compatible color value
*/
Color4 XmlTextElement::GetColor(const String &sColor)
{
	// Get lower case color string
	String sColor_  = sColor;
	sColor_.ToLower();

	// Predefined color?
	if (sColor_ == "aqua")			return Color4(static_cast<uint8>(  0), 255, 255, 255);
	else if (sColor_ == "black")	return Color4(static_cast<uint8>(  0),   0,   0, 255);
	else if (sColor_ == "blue")		return Color4(static_cast<uint8>(  0),   0, 255, 255);
	else if (sColor_ == "fuchsia")	return Color4(static_cast<uint8>(255),   0, 255, 255);
	else if (sColor_ == "gray")		return Color4(static_cast<uint8>(128), 128, 128, 255);
	else if (sColor_ == "green")	return Color4(static_cast<uint8>(  0), 128,   0, 255);
	else if (sColor_ == "lime")		return Color4(static_cast<uint8>(  0), 255,   0, 255);
	else if (sColor_ == "maroon")	return Color4(static_cast<uint8>(128),   0,   0, 255);
	else if (sColor_ == "navy")		return Color4(static_cast<uint8>(  0),   0, 128, 255);
	else if (sColor_ == "olive")	return Color4(static_cast<uint8>(128), 128,   0, 255);
	else if (sColor_ == "purple")	return Color4(static_cast<uint8>(128),   0, 128, 255);
	else if (sColor_ == "red")		return Color4(static_cast<uint8>(255),   0,   0, 255);
	else if (sColor_ == "silver")	return Color4(static_cast<uint8>(192), 192, 192, 255);
	else if (sColor_ == "teal")		return Color4(static_cast<uint8>(  0), 128, 128, 255);
	else if (sColor_ == "white")	return Color4(static_cast<uint8>(255), 255, 255, 255);
	else if (sColor_ == "yellow")	return Color4(static_cast<uint8>(255), 255,   0, 255);

	// Color in form #RGB
	else if (sColor.GetSubstring(0, 1) == "#" && sColor.GetLength() == 4) {
		uint8 nR = static_cast<uint8>(ParseTools::ParseHexValue(sColor.GetSubstring(1, 1) + sColor.GetSubstring(1, 1)));
		uint8 nG = static_cast<uint8>(ParseTools::ParseHexValue(sColor.GetSubstring(2, 1) + sColor.GetSubstring(2, 1)));
		uint8 nB = static_cast<uint8>(ParseTools::ParseHexValue(sColor.GetSubstring(3, 1) + sColor.GetSubstring(3, 1)));
		return Color4(nR, nG, nB, 255);
	}

	// Color in form #RRGGBB
	else if (sColor.GetSubstring(0, 1) == "#" && sColor.GetLength() == 7) {
		uint8 nR = static_cast<uint8>(ParseTools::ParseHexValue(sColor.GetSubstring(1, 2)));
		uint8 nG = static_cast<uint8>(ParseTools::ParseHexValue(sColor.GetSubstring(3, 2)));
		uint8 nB = static_cast<uint8>(ParseTools::ParseHexValue(sColor.GetSubstring(5, 2)));
		return Color4(nR, nG, nB, 255);
	}

	// Invalid color, return Black
	return Color4::Black;
}

/**
*  @brief
*    Load image file
*/
Image XmlTextElement::LoadImage(Gui &cGui, const String &sFilename)
{
	// Compose file name
	String sName = sFilename;
	if (!Url(sName).IsAbsolute()) sName = m_pXmlText->GetDocumentRoot() + sName;

	// Load image
	return Image(cGui, sName);
}


//[-------------------------------------------------------]
//[ Protected virtual XmlTextElement functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draw element
*/
void XmlTextElement::OnParse(XmlNode &cXmlNode)
{
	// To be implemented in derived classes
}

/**
*  @brief
*    Draw element
*/
void XmlTextElement::OnDraw(XmlTextRenderer &cRenderer)
{
	// Draw children
	DrawChildren(cRenderer);
}

/**
*  @brief
*    Get element size
*/
Vector2i XmlTextElement::OnGetSize() const
{
	// Get size of children
	for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
		// Get size of child element
		Vector2i vChildSize = m_lstChildren[i]->GetSize();
		if (vChildSize.x > 0 || vChildSize.y > 0) return vChildSize;
	}

	// No child element with a size, return (0, 0)
	return Vector2i::Zero;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
