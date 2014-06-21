/*********************************************************\
 *  File: Border.cpp                                     *
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
#include "PLGui/Widgets/Containers/Border.h"


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
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Border)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Border::Border(Widget *pParent) : ContainerWidget(pParent),
	LineStyle(this),
	BorderSize(this),
	BorderColor(this),
	m_nLineStyle(SolidLine),
	m_nBorderSize(1),
	m_cBorderColor(Color4::Black)
{
}

/**
*  @brief
*    Destructor
*/
Border::~Border()
{
}

/**
*  @brief
*    Get line style
*/
ELineStyle Border::GetLineStyle() const
{
	// Return line style
	return m_nLineStyle;
}

/**
*  @brief
*    Set line style
*/
void Border::SetLineStyle(ELineStyle nLineStyle)
{
	// Has the line style changed?
	if (m_nLineStyle != nLineStyle) {
		// Set line style
		m_nLineStyle = nLineStyle;

		// Size may have changed
		UpdateContent();

		// Redraw border
		Redraw();
	}
}

/**
*  @brief
*    Get border size
*/
int Border::GetBorderSize() const
{
	// Return border size
	return m_nBorderSize;
}

/**
*  @brief
*    Set border size
*/
void Border::SetBorderSize(int nSize)
{
	// Has the border size changed?
	if (m_nBorderSize != nSize) {
		// Set border size
		m_nBorderSize = nSize;

		// Size has changed
		UpdateContent();

		// Redraw border
		Redraw();
	}
}

/**
*  @brief
*    Get border color
*/
const Color4 &Border::GetBorderColor() const
{
	// Return border color
	return m_cBorderColor;
}

/**
*  @brief
*    Set border color
*/
void Border::SetBorderColor(const Color4 &cColor)
{
	// Has the border color changed?
	if (m_cBorderColor != cColor) {
		// Set border color
		m_cBorderColor = cColor;

		// Redraw border
		Redraw();
	}
}


//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
void Border::OnDraw(Graphics &cGraphics)
{
	// Draw border
	m_pTheme->DrawRect(cGraphics, Vector2i::Zero, GetSize() - Vector2i(1, 1), m_nLineStyle, m_cBorderColor, 1);
}

Vector2i Border::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Call base implementation
	ContainerWidget::OnPreferredSize(vRefSize);

	// Do we have a content widget?
	if (m_pContentWidget) {
		// Start with reference size
		Vector2i vSize = vRefSize;

		// Subtract border size from size for content widget
		if (vSize.x > -1) {
			vSize.x -= m_nBorderSize*2;
			if (vSize.x < 0) vSize.x = 0;
		}
		if (vSize.y > -1) {
			vSize.y -= m_nBorderSize*2;
			if (vSize.y < 0) vSize.y = 0;
		}

		// Get preferred size from content widget
		m_pContentWidget->CalculatePreferredSize(vSize);
		Vector2i vPreferredSize = m_pContentWidget->GetPreferredSize();

		// Add sizes for border again
		if (vPreferredSize.x >= 0) vPreferredSize.x += m_nBorderSize*2;
		if (vPreferredSize.y >= 0) vPreferredSize.y += m_nBorderSize*2;

		// Return preferred size
		return vPreferredSize;
	}

	// Give me everything
	return Vector2i(-1, -1);
}

void Border::OnAdjustContent()
{
	// Call base implementation
	ContainerWidget::OnAdjustContent();

	// Set position and size of content widget
	if (m_pContentWidget) {
		m_pContentWidget->SetPos (Vector2i(m_nBorderSize, m_nBorderSize));
		m_pContentWidget->SetSize(GetSize() - Vector2i(2*m_nBorderSize, 2*m_nBorderSize));
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
