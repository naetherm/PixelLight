/*********************************************************\
 *  File: XmlTextSection.cpp                             *
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
#include <PLGui/Gui/Resources/Graphics.h>
#include "PLGuiXmlText/XmlTextRenderer.h"
#include "PLGuiXmlText/XmlTextSection.h"


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
XmlTextSection::XmlTextSection(XmlTextElement *pParent) : XmlTextElement(pParent),
	m_nPaddingLeft(0),
	m_nPaddingRight(0),
	m_nPaddingTop(0),
	m_nPaddingBottom(0),
	m_cBgColor(Color4::Transparent),
	m_fAlpha(1.0f),
	m_nBorder(0)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextSection::~XmlTextSection()
{
}


//[-------------------------------------------------------]
//[ Protected virtual XmlTextElement functions            ]
//[-------------------------------------------------------]
void XmlTextSection::OnParse(XmlNode &cXmlNode)
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
			if (sName.CompareNoCase("X")) {
				// X
				m_vPos.x = sValue.GetInt();
			} else if (sName.CompareNoCase("Y")) {
				// Y
				m_vPos.y = sValue.GetInt();
			} else if (sName.CompareNoCase("Width")) {
				// Width
				m_vSize.x = sValue.GetInt();
			} else if (sName.CompareNoCase("Height")) {
				// Height
				m_vSize.y = sValue.GetInt();
			} else if (sName.CompareNoCase("Padding")) {
				// Padding on all sides
				m_nPaddingLeft	 = sValue.GetInt();
				m_nPaddingRight	 = m_nPaddingLeft;
				m_nPaddingTop	 = m_nPaddingLeft;
				m_nPaddingBottom = m_nPaddingLeft;
			} else if (sName.CompareNoCase("PaddingLeft")) {
				// Padding on left side
				m_nPaddingLeft = sValue.GetInt();
			} else if (sName.CompareNoCase("PaddingRight")) {
				// Padding on right side
				m_nPaddingRight	 = sValue.GetInt();
			} else if (sName.CompareNoCase("PaddingTop")) {
				// Padding on top side
				m_nPaddingTop	 = sValue.GetInt();
			} else if (sName.CompareNoCase("PaddingBottom")) {
				// Padding on bottom side
				m_nPaddingBottom	 = sValue.GetInt();
			} else if (sName.CompareNoCase("BgColor")) {
				// Background color
				m_cBgColor = GetColor(sValue);
			} else if (sName.CompareNoCase("Background")) {
				// Background image
				m_sBackground = sValue;
			} else if (sName.CompareNoCase("Border")) {
				// Border size
				m_nBorder = sValue.GetInt();
			} else if (sName.CompareNoCase("BorderColor")) {
				m_cBorderColor = GetColor(sValue);
			} else if (sName.CompareNoCase("Alpha")) {
				// Alpha transparency
				m_fAlpha = sValue.GetFloat();
			}

			// Next attribute
			pAttribute = pAttribute->GetNext();
		}
	}
}

void XmlTextSection::OnDraw(XmlTextRenderer &cRenderer)
{
	// Set section
	Vector2i vOldSectionPos  = cRenderer.GetSectionPos();
	Vector2i vOldSectionSize = cRenderer.GetSectionSize();
	cRenderer.SetSectionPos (m_vPos  + Vector2i(m_nPaddingLeft, m_nPaddingTop));
	cRenderer.SetSectionSize(m_vSize - Vector2i(m_nPaddingLeft + m_nPaddingRight, m_nPaddingTop + m_nPaddingBottom));

	// Draw background
	if (!m_cBgColor.IsTransparent()) {
		Color4 cColor(m_cBgColor.r, m_cBgColor.g, m_cBgColor.b, m_fAlpha);
		cRenderer.GetGraphics()->DrawBox(cColor, m_vPos, Vector2i(m_vPos.x + m_vSize.x - 1, m_vPos.y + m_vSize.y - 1));
	}

	// Draw background image
	if (m_sBackground.GetLength())
		cRenderer.GetGraphics()->DrawImage(LoadImage(cRenderer.GetGui(), m_sBackground), m_vPos, m_vSize);

	// Draw children
	DrawChildren(cRenderer);

	// Draw border
	if (m_nBorder > 0) {
		cRenderer.GetGraphics()->DrawRect(m_cBorderColor, m_vPos, Vector2i(m_vPos.x + m_vSize.x - 1, m_vPos.y + m_vSize.y - 1), m_nBorder);
	}

	// Restore section
	cRenderer.SetSectionPos (vOldSectionPos);
	cRenderer.SetSectionSize(vOldSectionSize);
}

Vector2i XmlTextSection::OnGetSize() const
{
	// Return size of section
	return m_vSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
