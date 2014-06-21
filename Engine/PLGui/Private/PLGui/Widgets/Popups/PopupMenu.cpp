/*********************************************************\
 *  File: PopupMenu.cpp                                  *
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
#include "PLGui/Gui/Resources/Menu.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Popups/PopupMenu.h"


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
pl_implement_class(PopupMenu)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PopupMenu::PopupMenu(Widget *pParent) : AbstractMenu(pParent),
	m_bPopupSizeCalculated(false)
{
	// Menu bars are vertical
	m_nOrientation = Vertical;

	// Don't accept focus
	SetFocusStyle(NoFocus);
}

/**
*  @brief
*    Destructor
*/
PopupMenu::~PopupMenu()
{
}

/**
*  @brief
*    Get popup size
*/
PLMath::Vector2i PopupMenu::GetPopupSize()
{
	// Has popup size already been calculated?
	if (!m_bPopupSizeCalculated) {
		// Calculate preferred size
		CalculatePreferredSize();
		m_bPopupSizeCalculated = true;
	}

	// Return size
	return GetPreferredSize();
}

/**
*  @brief
*    Open popup menu at specific position
*/
void PopupMenu::ShowPopupMenu(const PLMath::Vector2i &vPos)
{
	// Calculate popup size
	Vector2i vSize = GetPopupSize();

	// Set position and size
	SetPos(vPos);
	SetSize(vSize);
	SetZPos(ZAbove);

	// Make first item selected
	m_pSelectedItem = m_pMenu ? m_pMenu->GetItems()[0] : nullptr;
	SetSelection(0);

	// Make visible
	SetVisible(true);
}

/**
*  @brief
*    Close popup menu
*/
void PopupMenu::ClosePopupMenu()
{
	// Close any open submenu
	CloseSubmenu();
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
void PopupMenu::OnDraw(Graphics &cGraphics)
{
	// Draw menu background and border
	m_pTheme->DrawPopupMenu(cGraphics, Vector2i::Zero, GetSize() - Vector2i(1, 1), m_nOrientation, GetWidgetState());

	// Draw menu items
	for (uint32 i=0; i<m_pMenu->GetItems().GetNumOfElements(); i++) {
		// Get item
		MenuItem *pItem = m_pMenu->GetItems().Get(i);
		if (static_cast<int>(i) < m_nItems) {
			// Draw item
			Vector2i vPos1	= m_pItems[i].vPos1;
			Vector2i vPos2	= m_pItems[i].vPos2;
			bool bSelected	= (m_nSelection == static_cast<int>(i));
			bool bDrawArrow	= (m_nOrientation == Vertical);
			uint32 nWidgetState = 0;
			if (IsEnabled())					nWidgetState |= WidgetEnabled;
			if (IsActive())						nWidgetState |= WidgetActive;
			if (HasFocus())						nWidgetState |= WidgetFocus;
			if (bSelected)						nWidgetState |= WidgetMouseOver;
			if (bSelected && m_pSelectedItem)	nWidgetState |= WidgetPressed;
			m_pTheme->DrawMenuItem(cGraphics, vPos1, vPos2, pItem->GetText(), pItem->GetIcon(), bDrawArrow, pItem->GetType(), m_nOrientation, nWidgetState);
		}
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
