/*********************************************************\
 *  File: XmlTextImage.cpp                               *
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
#include "PLGuiXmlText/XmlTextRenderer.h"
#include "PLGuiXmlText/XmlTextImage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGui;
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextImage::XmlTextImage(XmlTextElement *pParent) : XmlTextElement(pParent),
	m_pImage(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextImage::~XmlTextImage()
{
	// Destroy the image
	if (m_pImage)
		delete m_pImage;
}


//[-------------------------------------------------------]
//[ Protected virtual XmlTextElement functions            ]
//[-------------------------------------------------------]
void XmlTextImage::OnParse(XmlNode &cXmlNode)
{
	// Is this an XML element?
	if (cXmlNode.GetType() == XmlNode::Element) {
		// Destroy the previous image
		if (m_pImage) {
			delete m_pImage;
			m_pImage = nullptr;
		}

		// Get XML element
		XmlElement &cXmlElement = static_cast<XmlElement&>(cXmlNode);

		// Parse attributes
		XmlAttribute *pAttribute = cXmlElement.GetFirstAttribute();
		while (pAttribute) {
			// Get name and value
			String sName  = pAttribute->GetName();
			String sValue = pAttribute->GetValue();

			// Save attribute
			if (sName.CompareNoCase("Src")) {
				// Image filename
				m_sFilename = sValue;
			} else if (sName.CompareNoCase("Width")) {
				// Image width
				m_vSize.x = sValue.GetInt();
			} if (sName.CompareNoCase("Height")) {
				// Image height
				m_vSize.y = sValue.GetInt();
			}

			// Next attribute
			pAttribute = pAttribute->GetNext();
		}

	}
}

void XmlTextImage::OnDraw(XmlTextRenderer &cRenderer)
{
	// Draw image
	if (m_sFilename.GetLength()) {
		// Load the image right now?
		if (!m_pImage) {
			// Create the image object
			m_pImage = new Image(cRenderer.GetGui());
			
			// Load the image
			*m_pImage = LoadImage(cRenderer.GetGui(), m_sFilename);
		}

		// Draw the image
		if (m_pImage)
			cRenderer.DrawImage(m_pImage, m_vSize);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
