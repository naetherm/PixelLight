/*********************************************************\
 *  File: TabBarEntry.cpp                                *
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
#include "PLGui/Widgets/Controls/TabBar.h"
#include "PLGui/Widgets/Controls/TabBarEntry.h"
#include "PLGui/Widgets/Controls/TabBarButton.h"


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
pl_implement_class(TabBarEntry)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TabBarEntry::TabBarEntry(Widget *pParent) : Widget(pParent),
	m_cIcon(*GetGui()),
	m_pWidget(nullptr),
	m_pData(nullptr),
	m_pTabBar(nullptr),
	m_nIndex(0),
	m_bClosable(false),
	m_pButtonClose(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
TabBarEntry::~TabBarEntry()
{
}

/**
*  @brief
*    Get TabBar
*/
TabBar *TabBarEntry::GetTabBar() const
{
	// Return TabBar
	return m_pTabBar;
}

/**
*  @brief
*    Get text
*/
String TabBarEntry::GetText() const
{
	// Return text
	return m_sText;
}

/**
*  @brief
*    Set text
*/
void TabBarEntry::SetText(const String &sText)
{
	// Set text
	m_sText = sText;
}

/**
*  @brief
*    Get icon
*/
const Image &TabBarEntry::GetIcon() const
{
	// Return icon
	return m_cIcon;
}

/**
*  @brief
*    Set icon
*/
void TabBarEntry::SetIcon(const Image &cIcon)
{
	// Set icon
	m_cIcon = cIcon;
}

/**
*  @brief
*    Get associated widget
*/
Widget *TabBarEntry::GetWidget() const
{
	// Return widget
	return m_pWidget;
}

/**
*  @brief
*    Set associated widget
*/
void TabBarEntry::SetWidget(Widget *pWidget)
{
	// Set widget
	m_pWidget = pWidget;
}

/**
*  @brief
*    Get associated data
*/
void *TabBarEntry::GetUserData() const
{
	// Return data
	return m_pData;
}

/**
*  @brief
*    Set associated data
*/
void TabBarEntry::SetUserData(void *pData)
{
	// Set data
	m_pData = pData;
}

/**
*  @brief
*    Get tab index
*/
int TabBarEntry::GetIndex() const
{
	// Return index
	return m_nIndex;
}

/**
*  @brief
*    Check if tab can be closed by the user
*/
bool TabBarEntry::IsClosable() const
{
	// Return closable
	return m_bClosable;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set TabBar
*/
void TabBarEntry::SetTabBar(TabBar *pTabBar)
{
	// Set TabBar
	m_pTabBar = pTabBar;
}

/**
*  @brief
*    Set tab index
*/
void TabBarEntry::SetIndex(int nIndex)
{
	// Set index
	m_nIndex = nIndex;
}

/**
*  @brief
*    Set if tab can be closed by the user
*/
void TabBarEntry::SetClosable(bool bClosable)
{
	// Set closable
	m_bClosable = bClosable;

	// Create close-button
	if (m_pButtonClose == nullptr && bClosable) {
		m_pButtonClose = new TabBarButton(this);
		m_pButtonClose->SetTabBar(m_pTabBar);
		m_pButtonClose->SetTabBarEntry(this);
		m_pButtonClose->SetFunction(TabFunctionCloseTab);
		m_pButtonClose->SetSize(Vector2i(16, 16));
		AdjustContent();
	}

	// Set button visibility
	if (m_pButtonClose) {
		m_pButtonClose->SetVisible(bClosable);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
Vector2i TabBarEntry::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Set preferred size
	Vector2i vSize;
	if (m_pTabBar->GetSide() == SideLeft || m_pTabBar->GetSide() == SideRight || (m_pTabBar->GetOptions() & TabOptionStaticSize) != 0) {
		// Static tab width
		vSize.x = m_pTabBar->GetTabWidth();
		vSize.y = m_pTabBar->GetTabHeight();
	} else {
		// Dynamic tab width
		Graphics cGraphics(*GetGui());
		Font cFont = GetTheme()->GetDefaultFont();
		uint32 nTextWidth = cGraphics.GetTextWidth(cFont, m_sText);
		nTextWidth += 16;
		if (m_bClosable) nTextWidth += 20;
		if (!m_cIcon.IsEmpty()) nTextWidth += 20;
		if (nTextWidth <					   32) nTextWidth = 32;
		if (nTextWidth > m_pTabBar->GetTabWidth()) nTextWidth = m_pTabBar->GetTabWidth();
		vSize.x = nTextWidth;
		vSize.y = m_pTabBar->GetTabHeight();
	}

	// Return preferred size
	return vSize;
}

void TabBarEntry::OnAdjustContent()
{
	// Call base implementation
	Widget::OnAdjustContent();

	// Update close-button
	if (m_pButtonClose) {
		Vector2i vSize = GetSize();
		Vector2i vPos;
		vPos.x =  vSize.x - 20;
		vPos.y = (vSize.y - 16) / 2;
		m_pButtonClose->SetPos(vPos);
	}
}

void TabBarEntry::OnMouseEnter()
{
	// Call base implementation
	Widget::OnMouseEnter();

	// Update mouseover-index
	m_pTabBar->SetMouseSelection(m_nIndex);
}

void TabBarEntry::OnMouseLeave()
{
	// Call base implementation
	Widget::OnMouseLeave();

	// Update mouseover-index
	m_pTabBar->SetMouseSelection(-1);
}

void TabBarEntry::OnMouseButtonClick(uint32 nButton, const Vector2i &vPos)
{
	// Call base implementation
	Widget::OnMouseButtonClick(nButton, vPos);

	// Set selected tab
	if (nButton == LeftButton) {
		m_pTabBar->SetSelection(m_nIndex);
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
