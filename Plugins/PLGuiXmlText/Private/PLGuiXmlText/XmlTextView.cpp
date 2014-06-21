/*********************************************************\
 *  File: XmlTextView.cpp                                *
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
#include <PLCore/File/Url.h>
#include "PLGuiXmlText/XmlTextView.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGui;
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(XmlTextView)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String XmlTextView::GetFilename() const
{
	return m_sFilename;
}

void XmlTextView::SetFilename(const String &sValue)
{
	if (m_sFilename != sValue) {
		m_sFilename = sValue;
		Load(m_sFilename);
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextView::XmlTextView(Widget *pParent) : Widget(pParent),
	Filename(this),
	m_cRenderer(*GetGui())
{
}

/**
*  @brief
*    Load XML file
*/
void XmlTextView::Load(const String &sFilename)
{
	// Save file name and document root
	m_sFilename = sFilename;
	m_sDocumentRoot = Url(m_sFilename).CutFilename();

	// Load XML file
	m_cXmlText.m_pXmlText = this;
	m_cXmlText.Load(sFilename);

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get document root
*/
String XmlTextView::GetDocumentRoot() const
{
	// Return path
	return m_sDocumentRoot;
}

/**
*  @brief
*    Get size of content
*/
Vector2i XmlTextView::GetContentSize() const
{
	// Return size of loaded XmlText
	return m_cXmlText.GetSize();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
XmlTextView::~XmlTextView()
{
}


//[-------------------------------------------------------]
//[ Public virtual Window functions                       ]
//[-------------------------------------------------------]
void XmlTextView::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	Widget::OnDraw(cGraphics);

	// Draw XmlText
	m_cRenderer.Init(cGraphics, GetSize());
	m_cXmlText.Draw(m_cRenderer);
	m_cRenderer.Finish();
}

void XmlTextView::OnSize(const Vector2i &vSize)
{
	// Call base implementation
	Widget::OnSize(vSize);

	// Redraw HTML
	Redraw();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
