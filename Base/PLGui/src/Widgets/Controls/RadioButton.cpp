/*********************************************************\
 *  File: RadioButton.cpp                                *
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
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Controls/RadioButton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_class_metadata(RadioButton, "PLGui", PLGui::AbstractToggleButton, "A radio button")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(RadioButton)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RadioButton::RadioButton(Widget *pParent) : AbstractToggleButton(pParent)
{
	// Accept focus
	SetFocusStyle(AcceptFocus);
}

/**
*  @brief
*    Destructor
*/
RadioButton::~RadioButton()
{
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
void RadioButton::OnDraw(Graphics &cGraphics)
{
	// Draw radio button
	m_pTheme->DrawRadioButton(cGraphics, Vector2i(0, 0), GetSize()-Vector2i(1, 1), GetText(), GetWidgetState(), GetState());
}

Vector2i RadioButton::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Call base function
	AbstractButton::OnPreferredSize(vRefSize);

	// Get width and height of the button text
	Graphics cGraphics(*GetGui());
	String sText = GetText();
	Font   cFont = GetTheme()->GetDefaultFont();
	int nTextWidth  = cGraphics.GetTextWidth (cFont, sText);
	int nTextHeight = cGraphics.GetTextHeight(cFont, sText);
	if (nTextHeight <= 0) {
		nTextWidth  = cGraphics.GetTextWidth (cFont, "Empty");
		nTextHeight = cGraphics.GetTextHeight(cFont, "Empty");
	}

	// Make widget as wide as needed for the text in X direction, and as small as possible in Y direction
	Vector2i vSize;
	vSize.x = nTextWidth  + 32;
	vSize.y = nTextHeight +  8;
	return vSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
