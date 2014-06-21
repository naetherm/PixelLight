/*********************************************************\
 *  File: Panel.cpp                                      *
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
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Containers/Panel.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Panel)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Panel::Panel(Widget *pParent) : ContainerWidget(pParent),
	BorderStyle(this),
	m_nBorderStyle(RaisedBorder)
{
}

/**
*  @brief
*    Destructor
*/
Panel::~Panel()
{
}

/**
*  @brief
*    Get border style
*/
EBorderStyle Panel::GetBorderStyle() const
{
	// Return border style
	return m_nBorderStyle;
}

/**
*  @brief
*    Set border style
*/
void Panel::SetBorderStyle(EBorderStyle nBorderStyle)
{
	// Has the border style changed?
	if (m_nBorderStyle != nBorderStyle) {
		// Set border style
		m_nBorderStyle = nBorderStyle;

		// Size may have changed
		UpdateContent();

		// Redraw border
		Redraw();
	}
}


//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
void Panel::OnDraw(Graphics &cGraphics)
{
	// Draw border
	m_pTheme->DrawBorder(cGraphics, Vector2i::Zero, GetSize() - Vector2i(1, 1), m_nBorderStyle);
}

Vector2i Panel::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Do we have a content widget?
	if (m_pContentWidget) {
		// Start with reference size
		Vector2i vSize = vRefSize;

		// Get border size
		int nBorderSize = m_pTheme->GetPanelBorderSize();

		// Subtract border size from size for content widget
		if (vSize.x > -1) {
			vSize.x -= nBorderSize*2;
			if (vSize.x < 0) vSize.x = 0;
		}
		if (vSize.y > -1) {
			vSize.y -= nBorderSize*2;
			if (vSize.y < 0) vSize.y = 0;
		}

		// Get preferred size from content widget
		m_pContentWidget->CalculatePreferredSize(vSize);
		Vector2i vPreferredSize = m_pContentWidget->GetPreferredSize();

		// Add sizes for border again
		if (vPreferredSize.x >= 0) vPreferredSize.x += nBorderSize*2;
		if (vPreferredSize.y >= 0) vPreferredSize.y += nBorderSize*2;

		// Return preferred size
		return vPreferredSize;
	}

	// Give me everything
	return Vector2i(-1, -1);
}

void Panel::OnAdjustContent()
{
	// Get border size
	int nBorderSize = m_pTheme->GetPanelBorderSize();

	// Set position and size of content widget
	if (m_pContentWidget) {
		m_pContentWidget->SetPos (Vector2i(nBorderSize, nBorderSize));
		m_pContentWidget->SetSize(GetSize() - Vector2i(2*nBorderSize, 2*nBorderSize));
	}
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
