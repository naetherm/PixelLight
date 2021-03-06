/*********************************************************\
 *  File: SNText.cpp                                     *
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
#include "PLScene/Scene/SceneNodes/SNText.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNText, "PLScene", PLScene::SceneNode, "Abstract text scene node")
	// Attributes
	pl_attribute_metadata(Text,		PLCore::String,						"",							ReadWrite,	"Text do draw",																						"")
	pl_attribute_metadata(Color,	PLGraphics::Color4,					PLGraphics::Color4::White,	ReadWrite,	"Text color",																						"")
	pl_attribute_metadata(Font,		PLCore::String,						"",							ReadWrite,	"The used font, if empty the default font is used",													"")
	pl_attribute_metadata(FontSize,	PLCore::uint32,						0,							ReadWrite,	"The font size, if null the default font size is used - use scene node scale for dynamic scale",	"Max='100'")
		// Overwritten SceneNode attributes
	pl_attribute_metadata(Flags,	pl_flag_type_def3(SNText, EFlags),	0,							ReadWrite,	"Flags",																							"")
pl_class_metadata_end(SNText)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNText::GetText() const
{
	return m_sText;
}

void SNText::SetText(const String &sValue)
{
	m_sText = sValue;
}

String SNText::GetFont() const
{
	return m_sFont;
}

void SNText::SetFont(const String &sValue)
{
	m_sFont = sValue;
}

uint32 SNText::GetFontSize() const
{
	return m_nFontSize;
}

void SNText::SetFontSize(uint32 nValue)
{
	m_nFontSize = nValue;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNText::SNText() :
	Text(this),
	Color(this),
	Font(this),
	FontSize(this),
	Flags(this),
	m_nFontSize(0)
{
}

/**
*  @brief
*    Destructor
*/
SNText::~SNText()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
