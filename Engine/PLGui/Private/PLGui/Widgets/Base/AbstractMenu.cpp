/*********************************************************\
 *  File: AbstractMenu.cpp                               *
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
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Gui/Resources/Menu.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/Screen.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Base/AbstractMenu.h"
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
pl_implement_class(AbstractMenu)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AbstractMenu::AbstractMenu(Widget *pParent) : Widget(pParent),
	m_pMenu(nullptr),
	m_nOrientation(Vertical),
	m_nOpenSideHorizontal(SideRight),
	m_nOpenSideVertical(SideBottom),
	m_nSelection(-1),
	m_pItems(nullptr),
	m_nItems(0),
	m_nSize(-1),
	m_nMaxWidth(-1),
	m_nMouseOver(-1),
	m_pSelectedItem(nullptr),
	m_pPopupMenu(nullptr),
	m_nIgnoreMouse(0),
	m_pParentMenu(nullptr),
	m_pController(this),
	m_pControlMenu(this),
	m_bAllowButtonUp(false)
{
	// Don't accept focus
	SetFocusStyle(NoFocus);
}

/**
*  @brief
*    Destructor
*/
AbstractMenu::~AbstractMenu()
{
	// Delete menu data
	DeleteMenuData();
}

/**
*  @brief
*    Get menu
*/
Menu *AbstractMenu::GetMenu() const
{
	// Return menu
	return m_pMenu;
}

/**
*  @brief
*    Set menu
*/
void AbstractMenu::SetMenu(Menu *pMenu)
{
	// Set menu
	m_pMenu = pMenu;
}

/**
*  @brief
*    Get orientation of menu widget
*/
EOrientation AbstractMenu::GetOrientation() const
{
	// Return orientation
	return m_nOrientation;
}

/**
*  @brief
*    Set orientation of menu widget
*/
void AbstractMenu::SetOrientation(EOrientation nOrientation)
{
	// Set orientation
	m_nOrientation = nOrientation;

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get side on which is submenu is opened
*/
ESide AbstractMenu::GetOpenSide(EOrientation nOrientation) const
{
	// Return side
	return (nOrientation == Horizontal ? m_nOpenSideHorizontal : m_nOpenSideVertical);
}

/**
*  @brief
*    Set side on which is submenu is opened
*/
void AbstractMenu::SetOpenSide(EOrientation nOrientation, ESide nSide)
{
	// Check if side is possible
	if (nOrientation == Horizontal && (nSide == SideLeft || nSide == SideRight)) {
		m_nOpenSideHorizontal = nSide;
	} else if (nOrientation == Vertical && (nSide == SideTop || nSide == SideBottom)) {
		m_nOpenSideVertical = nSide;
	}
}

/**
*  @brief
*    Get currently selected menu item
*/
int AbstractMenu::GetSelection() const
{
	// Return selected item
	return m_nSelection;
}

/**
*  @brief
*    Set currently selected menu item
*/
void AbstractMenu::SetSelection(int nItem)
{
	// Has the selection changed?
	int nNumOfItems = (m_pMenu ? m_pMenu->GetItems().GetNumOfElements() : 0);
	if (m_nSelection != nItem && nItem >= -1 && nItem < nNumOfItems) {
		// Get currently selected item
		MenuItem *pItem = m_pMenu->GetItems().Get(nItem);
		bool bOpenSubmenu = (m_pSelectedItem != nullptr);

		// Close currently open item
		if (m_pSelectedItem) {
			// Close submenu
			if (m_pSelectedItem->GetType() == TypeMenu) {
				CloseSubmenu();
			}

			// Close submenu
			m_pSelectedItem = nullptr;
		}

		// Set selection
		m_nSelection = nItem;

		// Emit signal
		SignalItemSelected(m_nSelection);

		// Check selected item
		if (pItem && bOpenSubmenu) {
			// Set new open submenu (or item)
			m_pSelectedItem = pItem;

			// Open new submenu?
			if (pItem->GetType() == TypeMenu) {
				// Open submenu
				OpenSubmenu(m_pSelectedItem, nItem);
			}
		}

		// Redraw menu
		Redraw();
	}
}

/**
*  @brief
*    Click on a menu item
*/
void AbstractMenu::ClickItem(int nItem)
{
	// Get currently selected item
	MenuItem *pItem = m_pMenu->GetItems().Get(nItem);
	bool bCloseOnly = (m_pSelectedItem != nullptr && m_pSelectedItem == pItem);

	// Close currently open item
	if (m_pSelectedItem) {
		// Close submenu
		if (m_pSelectedItem->GetType() == TypeMenu) {
			CloseSubmenu();
		}

		// Close submenu
		m_pSelectedItem = nullptr;
	}

	// Check clicked item
	if (pItem) {
		// Open new submenu?
		if (pItem->GetType() == TypeMenu && !bCloseOnly) {
			// Set open item
			m_pSelectedItem = pItem;

			// Open submenu
			OpenSubmenu(m_pSelectedItem, nItem);
		} else if (pItem->GetType() == TypeMenuItem) {
			// Item clicked
			m_pController->OnItemClicked(pItem);
			m_pController->SetSelection(-1);
		}
	}

	// Reset selection?
	if (nItem == -1) {
		SetSelection(-1);
	}

	// Redraw menu
	Redraw();
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractMenu functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a menu item has been clicked
*/
void AbstractMenu::OnItemClicked(MenuItem *pItem)
{
	// Emit signal
	SignalItemClicked(pItem);
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
Vector2i AbstractMenu::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Update menu data
	const_cast<AbstractMenu*>(this)->UpdateMenuData();

	// Give back size of all items as preferred size
	Vector2i vSize;
	if (m_nItems > 0) {
		vSize = m_pItems[m_nItems-1].vPos2;
	}
	return vSize;
}

void AbstractMenu::OnMouseMove(const PLMath::Vector2i &vPos)
{
	// [HACK] Really, I hate this, but I don't see another way. When a submenu is opened, we keep getting
	//        MouseMove-events, although the mouse position has not really changed (on Windows). I'm not sure
	//		  if this is caused by some spaghetti code in PLGui or if it really is a Windows error that after
	//        a new window is made visible, a new mouse-move message is sent regardless of mouse movement.
	//        However this means that the currently selected item is constantly changed back to mouse selection,
	//        so we use this silly hack to ignore those mouse messages.
	if (m_nIgnoreMouse == 0) {
		// Get currently selected mouse item
		m_nMouseOver = GetItemFromMousePos(vPos);
		if (m_nSelection != m_nMouseOver) {
			// Change selection
			if (m_nMouseOver > -1 || !m_pSelectedItem)
				SetSelection(m_nMouseOver);
		}
	} else if (m_nIgnoreMouse > 0) m_nIgnoreMouse--;
}

void AbstractMenu::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Click on item
	if (nButton == LeftButton) {
		ClickItem(m_nMouseOver);
	}

	// Button has been released
	m_bAllowButtonUp = false;
}

void AbstractMenu::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Click on item
	if (nButton == LeftButton && m_bAllowButtonUp) {
		ClickItem(m_nMouseOver);
	}
}

void AbstractMenu::OnKeyDown(uint32 nKey, uint32 nModifiers)
{
	// Check pointer
	if (m_pControlMenu) {
		// Previous item
		if ( m_pControlMenu->GetSelection() > -1 &&
			 ((m_pControlMenu->GetOrientation() == Horizontal && nKey == PLGUIKEY_LEFT) ||
			  (m_pControlMenu->GetOrientation() == Vertical   && nKey == PLGUIKEY_UP)) )
		{
			// Select previous item
			int nSelection = m_pControlMenu->GetSelection() - 1;
			if (nSelection < 0) nSelection = m_pControlMenu->m_nItems-1;

			// Ignore separators
			MenuItem *pItem = (m_pMenu ? m_pMenu->GetItems().Get(nSelection) : nullptr);
			if (pItem && pItem->GetType() == TypeSeparator) {
				nSelection--;
				if (nSelection < 0) nSelection = m_pControlMenu->m_nItems-1;
			}

			// Update selection
			m_pControlMenu->SetSelection(nSelection);
			m_pControlMenu->m_nIgnoreMouse = 5;
		}

		// Next item
		if ( m_pControlMenu->GetSelection() > -1 &&
			 ((m_pControlMenu->GetOrientation() == Horizontal && nKey == PLGUIKEY_RIGHT) ||
			  (m_pControlMenu->GetOrientation() == Vertical   && nKey == PLGUIKEY_DOWN)) )
		{
			// Select next item
			int nSelection = m_pControlMenu->GetSelection() + 1;
			if (nSelection >= m_pControlMenu->m_nItems) nSelection = 0;

			// Ignore separators
			MenuItem *pItem = (m_pMenu ? m_pMenu->GetItems().Get(nSelection) : nullptr);
			if (pItem && pItem->GetType() == TypeSeparator) {
				nSelection++;
				if (nSelection >= m_pControlMenu->m_nItems) nSelection = 0;
			}

			// Update selection
			m_pControlMenu->SetSelection(nSelection);
			m_pControlMenu->m_nIgnoreMouse = 5;
		}

		// Select current item
		if ((nKey == PLGUIKEY_SPACE || nKey == PLGUIKEY_RETURN) && m_pControlMenu->GetSelection() > -1) {
			m_pControlMenu->ClickItem(m_pControlMenu->GetSelection());
		}

		// Open next popup-menu
		if ( !m_pControlMenu->m_pSelectedItem &&
			 ((m_pControlMenu->GetOrientation() == Horizontal && nKey == PLGUIKEY_DOWN) ||
			  (m_pControlMenu->GetOrientation() == Vertical   && nKey == PLGUIKEY_RIGHT)) )
		{
			// Must be a sub-menu
			MenuItem *pItem = (m_pControlMenu->m_pMenu ? m_pControlMenu->m_pMenu->GetItems().Get(m_pControlMenu->GetSelection()) : nullptr);
			if (pItem && pItem->GetType() == TypeMenu) {
				m_pControlMenu->ClickItem(m_pControlMenu->GetSelection());
			}
		}

		// Close current popup-menu
		if (m_pControlMenu->GetOrientation() == Vertical && nKey == PLGUIKEY_LEFT && m_pControlMenu->m_pParentMenu) {
			m_pControlMenu = m_pControlMenu->m_pParentMenu;
			m_pControlMenu->ClickItem(m_pControlMenu->GetSelection());
		}

		// Keyboard shortcut
		if (nKey >= PLGUIKEY_A && nKey <= PLGUIKEY_Z) {
			// Get character
			char nChar = static_cast<char>('a' + (nKey - PLGUIKEY_A));
			m_pControlMenu->PressShortcut(nChar);
			m_pControlMenu->m_nIgnoreMouse = 5;
		}
	}

	// Close entire menu
	if (nKey == PLGUIKEY_ESCAPE) {
		if (m_pSelectedItem) ClickItem(GetSelection());
		else				 ClickItem(-1);
	}
}

void AbstractMenu::OnGetFocus()
{
	// Set current menu
	m_pController->SetControlledMenu(m_pPopupMenu);
}

void AbstractMenu::OnLooseFocus()
{
	// Close items
	ClickItem(-1);

	// Set current menu
	m_pController->SetControlledMenu(m_pPopupMenu);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Open submenu
*/
void AbstractMenu::OpenSubmenu(MenuItem *pItem, int nItem)
{
	// Check if new item is valid
	if (pItem && pItem->GetType() == TypeMenu && nItem >= 0 && nItem < m_nItems) {
		// Set new item
		m_pSelectedItem = pItem;

		// Create popup menu and get size
		m_pPopupMenu = new PopupMenu(GetGui()->GetRootWidget());
		m_pPopupMenu->SetMenu(static_cast<Menu*>(m_pSelectedItem));
		m_pPopupMenu->SetFocusStyle(NoFocus);
		m_pPopupMenu->m_pParentMenu = this;
		m_pPopupMenu->m_pController = m_pController;
		m_pPopupMenu->m_bAllowButtonUp = true;
		Vector2i vSize = m_pPopupMenu->GetPopupSize();

		// Determine popup position
		Vector2i vPos;
		for (int i=0; i<2; i++) {
			// Set start position
			vPos = GetAbsPos();

			// Left
			if (m_nOrientation == Vertical && m_nOpenSideHorizontal == SideLeft) {
				vPos.x += m_pItems[nItem].vPos1.x - vSize.x;
				if (m_nOpenSideVertical == SideTop)	vPos.y += m_pItems[nItem].vPos2.y - vSize.y;
				else								vPos.y += m_pItems[nItem].vPos1.y;
			}

			// Right
			else if (m_nOrientation == Vertical && m_nOpenSideHorizontal == SideRight) {
				vPos.x += m_pItems[nItem].vPos2.x;
				if (m_nOpenSideVertical == SideTop)	vPos.y += m_pItems[nItem].vPos2.y - vSize.y;
				else								vPos.y += m_pItems[nItem].vPos1.y;
			}

			// Top
			else if (m_nOrientation == Horizontal && m_nOpenSideVertical == SideTop) {
				vPos.x += m_pItems[nItem].vPos1.x;
				vPos.y += m_pItems[nItem].vPos1.y - vSize.y;
			}

			// Bottom
			else if (m_nOrientation == Horizontal && m_nOpenSideVertical == SideBottom) {
				vPos.x += m_pItems[nItem].vPos1.x;
				vPos.y += m_pItems[nItem].vPos2.y;
			}

			// Check if position is valid
			bool bChanged = false;
			if (i == 0) {
				// Get screen size
				Widget *pParent = m_pPopupMenu->GetParent();
				Vector2i vScreenSize = pParent ? pParent->GetSize() : m_pPopupMenu->GetScreen()->GetSize();

				// Switch between left and right
				if ((m_nOpenSideHorizontal == SideLeft && vPos.x < 0) || (m_nOpenSideHorizontal == SideRight && vPos.x + vSize.x >= vScreenSize.x)) {
					m_nOpenSideHorizontal = (m_nOpenSideHorizontal == SideLeft ? SideRight : SideLeft);
					bChanged = true;
				}

				// Switch between top and bottom
				if ((m_nOpenSideVertical == SideTop && vPos.y < 0) || (m_nOpenSideVertical == SideBottom && vPos.y + vSize.y >= vScreenSize.y)) {
					m_nOpenSideVertical = (m_nOpenSideVertical == SideTop ? SideBottom : SideTop);
					bChanged = true;
				}
			}

			// Has the position changed?
			if (!bChanged) break;
		}

		// Set popup position and inherit open-side
		m_pPopupMenu->SetVisible(true);
		if (!m_pPopupMenu) return;	// If something is implemented poorly, the focus may got away causing m_pPopupMenu to be a null pointer
		m_pPopupMenu->ShowPopupMenu(vPos);
		if (!m_pPopupMenu) return;	// If something is implemented poorly, the focus may got away causing m_pPopupMenu to be a null pointer
		m_pPopupMenu->SetOpenSide(Horizontal, m_nOpenSideHorizontal);
		m_pPopupMenu->SetOpenSide(Vertical,   m_nOpenSideVertical);

		// Emit signal
		SignalSubmenuOpened(m_pSelectedItem);

		// Inform controller
		m_pController->SetControlledMenu(m_pPopupMenu);
	}
}

/**
*  @brief
*    Close submenu
*/
void AbstractMenu::CloseSubmenu()
{
	// Check if current item is valid
	if (m_pSelectedItem) {
		// Emit signal
		SignalSubmenuClosed(m_pSelectedItem);

		// Close popup menu
		if (m_pPopupMenu) {
			m_pPopupMenu->ClosePopupMenu();
			m_pPopupMenu->Destroy();
			m_pPopupMenu = nullptr;
		}

		// Inform controller
		m_pController->SetControlledMenu(this);
	}
}

/**
*  @brief
*    Activate menu shortcut
*/
void AbstractMenu::PressShortcut(char nShortcut)
{
	if (m_pMenu) {
		int32 nItem = m_pMenu->GetShortcutItem(nShortcut);
		if (nItem >= 0) {
			ClickItem(nItem);
			SetSelection(nItem);
		}
	}
}

/**
*  @brief
*    Update menu data
*/
void AbstractMenu::UpdateMenuData()
{
	// Delete old data
	DeleteMenuData();

	// Do we have a menu and at least one item?
	if (m_pMenu && m_pMenu->GetItems().GetNumOfElements() > 0) {
		// Create array to hold item widths
		m_nItems = m_pMenu->GetItems().GetNumOfElements();
		m_pItems = new SItem[m_nItems];

		// Calculate sizes
		Vector2i vPos(0, 0);
		m_nMaxWidth = 0;
		for (int i=0; i<m_nItems; i++) {
			// Get menu item
			MenuItem *pItem = m_pMenu->GetItems()[i];

			// Calculate size of menu item
			Vector2i vSize = CalculateItemSize(m_nOrientation, pItem);

			// Set positions
			m_pItems[i].vPos1 = vPos;
			m_pItems[i].vPos2 = vPos + vSize;

			// Keep track of maximum width
			if (vSize.x > m_nMaxWidth) m_nMaxWidth = vSize.x;

			// Go to next item
			if (m_nOrientation == Horizontal) vPos.x += vSize.x;
			else							  vPos.y += vSize.y;
		}

		// Make all items the same size for vertical menus
		if (m_nOrientation == Vertical) {
			for (int i=0; i<m_nItems; i++) {
				m_pItems[i].vPos2.x = m_pItems[i].vPos1.x + m_nMaxWidth;
			}
		}
	}
}

/**
*  @brief
*    Delete menu data
*/
void AbstractMenu::DeleteMenuData()
{
	// Destroy data
	if (m_pItems) {
		delete [] m_pItems;
		m_pItems	= nullptr;
		m_nItems	= 0;
		m_nSize		= -1;
		m_nMaxWidth = -1;
	}
}

/**
*  @brief
*    Check which item is selected based on mouse position
*/
int AbstractMenu::GetItemFromMousePos(const PLMath::Vector2i &vPos)
{
	// Quick check if the current item is still selected (avoid going through the loop)
	if (m_nMouseOver > 0 && m_nMouseOver < m_nItems) {
		// Check item
		if (m_pItems[m_nMouseOver].vPos1 <= vPos && vPos <= m_pItems[m_nMouseOver].vPos2) {
			return m_nMouseOver;
		}
	}

	// Loop through items
	for (int i=0; i<m_nItems; i++) {
		// Check item
		if (m_pItems[i].vPos1 <= vPos && vPos <= m_pItems[i].vPos2) {
			return i;
		}
	}

	// No item selected
	return -1;
}

/**
*  @brief
*    Set currently active menu in the chain of sub-menus
*/
void AbstractMenu::SetControlledMenu(AbstractMenu *pMenu)
{
	m_pControlMenu = pMenu;
}

/**
*  @brief
*    Calculate size of menu item
*/
Vector2i AbstractMenu::CalculateItemSize(EOrientation nOrientation, MenuItem *pItem)
{
	// Check parameter
	if (pItem) {
		// Separator
		if (pItem->GetType() == TypeSeparator) {
			if (nOrientation == Horizontal)	return Vector2i(10, 20);
			else							return Vector2i(10,  8);
		}

		// Item
		else {
			// Get font
			Font cFont = GetTheme()->GetDefaultFont();

			// Get offscreen graphics
			Graphics cGraphics(*GetGui());

			// Get icon size
			int nIconSize = 0;
			if (!pItem->GetIcon().IsEmpty()) {
				// Use an icon of size 16x16 and leave a little gap of 4
				nIconSize = 20;
			}

			// Get text size
			int nTextSize = 0;
			if (pItem->GetText().GetLength() > 0) {
				// Get text size
				String sText = pItem->GetText();
				nTextSize = cGraphics.GetTextWidth(cFont, pItem->GetText());
			}

			// Take menu-arrow into account
			int nArrowSize = 0;
			if (pItem->GetType() == TypeMenu) {
				nArrowSize = 14;
			}

			// Compute size, leave a gap of 4px left and right and always use a height of 24
			return Vector2i(nIconSize + nTextSize + nArrowSize + 8, 24);
		}
	}

	// Error!
	return Vector2i::Zero;
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
