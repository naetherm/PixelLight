/*********************************************************\
 *  File: XmlTextTextNode.cpp                            *
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
#include <PLCore/String/RegEx.h>
#include <PLCore/Xml/XmlText.h>
#include "PLGuiXmlText/XmlTextRenderer.h"
#include "PLGuiXmlText/XmlTextTextNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextTextNode::XmlTextTextNode(XmlTextElement *pParent) : XmlTextElement(pParent)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextTextNode::~XmlTextTextNode()
{
}


//[-------------------------------------------------------]
//[ Protected virtual XmlTextElement functions            ]
//[-------------------------------------------------------]
void XmlTextTextNode::OnParse(XmlNode &cXmlNode)
{
	// Get XML text
	XmlText &cXmlText = static_cast<XmlText&>(cXmlNode);
	m_sText = cXmlText.GetValue();
}

void XmlTextTextNode::OnDraw(XmlTextRenderer &cRenderer)
{
	// Get text word for word
	RegEx cRegEx("\\s*([^\\s]+)");
	uint32 nParsePos = 0;
	while (cRegEx.Match(m_sText, nParsePos)) {
		// Get next word
		nParsePos = cRegEx.GetPosition();
		String sWord = cRegEx.GetResult(0);

		// Draw word
		cRenderer.DrawText(sWord);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
