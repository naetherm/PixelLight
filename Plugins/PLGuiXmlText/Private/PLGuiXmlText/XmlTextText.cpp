/*********************************************************\
 *  File: XmlTextText.cpp                                *
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
#include <PLCore/Xml/XmlElement.h>
#include <PLCore/Xml/XmlAttribute.h>
#include <PLGui/Gui/Resources/Font.h>
#include "PLGuiXmlText/XmlTextRenderer.h"
#include "PLGuiXmlText/XmlTextText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLGui;
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextText::XmlTextText(bool bParagraph, XmlTextElement *pParent) : XmlTextElement(pParent),
	m_bParagraph(bParagraph),
	m_cTextColor(Color4::Transparent),
	m_cBgColor(Color4::Transparent),
	m_nFontSize(-1),
	m_nHAlign(XmlTextRenderer::HAlignUnknown),
	m_nVAlign(XmlTextRenderer::VAlignUnknown)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextText::~XmlTextText()
{
}


//[-------------------------------------------------------]
//[ Protected virtual XmlTextElement functions            ]
//[-------------------------------------------------------]
void XmlTextText::OnParse(XmlNode &cXmlNode)
{
	// Is this an XML element?
	if (cXmlNode.GetType() == XmlNode::Element) {
		// Get XML element
		XmlElement &cXmlElement = static_cast<XmlElement&>(cXmlNode);

		// Parse attributes
		XmlAttribute *pAttribute = cXmlElement.GetFirstAttribute();
		while (pAttribute) {
			// Get name and value
			String sName  = pAttribute->GetName();
			String sValue = pAttribute->GetValue();

			// Save attribute
			if (sName.CompareNoCase("Color")) {
				// Text color
				m_cTextColor = GetColor(sValue);
			} else if (sName.CompareNoCase("BgColor")) {
				// Background color
				m_cBgColor = GetColor(sValue);
			} else if (sName.CompareNoCase("Font")) {
				// Font name
				m_sFont = sValue;
			} else if (sName.CompareNoCase("FontSize")) {
				// Font size
				m_nFontSize = sValue.GetInt();
			} else if (sName.CompareNoCase("HAlign")) {
					 if (sValue.CompareNoCase("Left"))		m_nHAlign = XmlTextRenderer::HAlignLeft;
				else if (sValue.CompareNoCase("Right"))		m_nHAlign = XmlTextRenderer::HAlignRight;
				else if (sValue.CompareNoCase("Center"))	m_nHAlign = XmlTextRenderer::HAlignCenter;
				else if (sValue.CompareNoCase("Justify"))	m_nHAlign = XmlTextRenderer::HAlignJustify;
				else										m_nHAlign = XmlTextRenderer::HAlignUnknown;
			} else if (sName.CompareNoCase("VAlign")) {
					 if (sValue.CompareNoCase("Top"))		m_nVAlign = XmlTextRenderer::VAlignTop;
				else if (sValue.CompareNoCase("Bottom"))	m_nVAlign = XmlTextRenderer::VAlignBottom;
				else if (sValue.CompareNoCase("Middle"))	m_nVAlign = XmlTextRenderer::VAlignMiddle;
				else										m_nVAlign = XmlTextRenderer::VAlignUnknown;
			}

			// Next attribute
			pAttribute = pAttribute->GetNext();
		}
	}
}

void XmlTextText::OnDraw(XmlTextRenderer &cRenderer)
{
	// Start paragraph
	if (m_bParagraph) {
		cRenderer.AddLineBreak();
	}

	// Set renderer state
	Color4 cOldTextColor = cRenderer.GetTextColor();
	if (!m_cTextColor.IsTransparent()) cRenderer.SetTextColor(m_cTextColor);
	Color4 cOldBgColor = cRenderer.GetBgColor();
	if (!m_cBgColor.IsTransparent()) cRenderer.SetBgColor(m_cBgColor);
	Font *pOldFont = cRenderer.GetFont() ? new Font(*cRenderer.GetFont()) : nullptr;
	if (m_sFont.GetLength() || m_nFontSize > -1) cRenderer.SetFont(m_sFont, m_nFontSize);
	XmlTextRenderer::HAlign nOldHAlign = cRenderer.GetHAlign();
	if (m_nHAlign != XmlTextRenderer::HAlignUnknown) cRenderer.SetHAlign(m_nHAlign);
	XmlTextRenderer::VAlign nOldVAlign = cRenderer.GetVAlign();
	if (m_nVAlign != XmlTextRenderer::VAlignUnknown) cRenderer.SetVAlign(m_nVAlign);

	// Draw children
	DrawChildren(cRenderer);

	// End paragraph
	if (m_bParagraph) {
		cRenderer.AddLineBreak();
		cRenderer.AddVerticalSpace(10);
	}

	// Restore renderer state
	cRenderer.SetTextColor(cOldTextColor);
	cRenderer.SetBgColor(cOldBgColor);
	cRenderer.SetFont(pOldFont);
	cRenderer.SetHAlign(nOldHAlign);
	cRenderer.SetVAlign(nOldVAlign);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
