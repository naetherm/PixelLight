/*********************************************************\
 *  File: TabBar.cpp                                     *
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
#include "PLGui/Widgets/Controls/TabBar.h"
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
pl_implement_class(TabBar)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TabBar::TabBar(Widget *pParent) : Widget(pParent),
	m_nSide(SideTop),
	m_nOptions(0),
	m_nTabWidth(160),
	m_nTabHeight(24),
	m_nSelection(-1),
	m_nMouseSelection(-1),
	m_nScrollingPos(0),
	m_pButtonCreate(nullptr),
	m_pButtonNext(nullptr),
	m_pButtonPrev(nullptr)
{
	// Give focus to child windows
	SetFocusStyle(ChildFocus);

	// Create button 'add'
	m_pButtonCreate = new TabBarButton(GetContentWidget());
	m_pButtonCreate->SetTabBar(this);
	m_pButtonCreate->SetFunction(TabFunctionCreateTab);
	m_pButtonCreate->SetSize(Vector2i(16, 16));
	m_pButtonCreate->SetVisible(true);

	// Create button 'scroll (next)'
	m_pButtonNext = new TabBarButton(GetContentWidget());
	m_pButtonNext->SetTabBar(this);
	m_pButtonNext->SetFunction(TabFunctionScrollNext);
	m_pButtonNext->SetSize(Vector2i(16, 16));
	m_pButtonNext->SetVisible(true);

	// Create button 'scroll (prev)'
	m_pButtonPrev = new TabBarButton(GetContentWidget());
	m_pButtonPrev->SetTabBar(this);
	m_pButtonPrev->SetFunction(TabFunctionScrollPrev);
	m_pButtonPrev->SetSize(Vector2i(16, 16));
	m_pButtonPrev->SetVisible(true);
}

/**
*  @brief
*    Destructor
*/
TabBar::~TabBar()
{
}

/**
*  @brief
*    Get side the tabbar is positioned on
*/
ESide TabBar::GetSide() const
{
	// Return side
	return m_nSide;
}

/**
*  @brief
*    Set side the tabbar is positioned on
*/
void TabBar::SetSide(ESide nSide)
{
	// Set side
	m_nSide = nSide;

	// After something changed, the widget has to be updated
	AdjustContent();
}

/**
*  @brief
*    Get TabBar options
*/
uint32 TabBar::GetOptions() const
{
	// Return options
	return m_nOptions;
}

/**
*  @brief
*    Set TabBar options
*/
void TabBar::SetOptions(uint32 nOptions)
{
	// Set options
	m_nOptions = nOptions;

	// Adjust options in tabs
	for (uint32 i=0; i<m_lstTabs.GetNumOfElements(); i++) {
		TabBarEntry *pTab = m_lstTabs[i];
		pTab->SetClosable((m_nOptions & TabOptionCloseTab) != 0 ? true : false);
	}

	// After something changed, the widget has to be updated
	AdjustContent();
}

/**
*  @brief
*    Get tab width
*/
uint32 TabBar::GetTabWidth() const
{
	// Return tab width
	return m_nTabWidth;
}

/**
*  @brief
*    Set tab width
*/
void TabBar::SetTabWidth(uint32 nWidth)
{
	// Set tab width
	m_nTabWidth = nWidth;

	// After something changed, the widget has to be updated
	AdjustContent();
}

/**
*  @brief
*    Get tab height
*/
uint32 TabBar::GetTabHeight() const
{
	// Return tab height
	return m_nTabHeight;
}

/**
*  @brief
*    Set tab height
*/
void TabBar::SetTabHeight(uint32 nHeight)
{
	// Set tab height
	m_nTabHeight = nHeight;

	// After something changed, the widget has to be updated
	AdjustContent();
}

/**
*  @brief
*    Create a new tab
*/
void TabBar::CreateTab()
{
	// Call virtual function
	OnCreateTab();
}

/**
*  @brief
*    Close tab
*/
void TabBar::CloseTab(TabBarEntry *pTab)
{
	// Check tab
	if (pTab && m_lstTabs.IsElement(pTab)) {
		// Call virtual function
		OnCloseTab(pTab);
	}
}

/**
*  @brief
*    Close tab
*/
void TabBar::CloseTab(uint32 nTab)
{
	// Check tab
	TabBarEntry *pTab = m_lstTabs.Get(nTab);
	if (pTab) {
		// Close tab
		CloseTab(pTab);
	}
}

/**
*  @brief
*    Get list of tabs
*/
const Container<TabBarEntry*> &TabBar::GetTabs() const
{
	// Return list of tabs
	return m_lstTabs;
}

/**
*  @brief
*    Add tab
*/
TabBarEntry *TabBar::AddTab(const String &sText)
{
	// Create new tab
	TabBarEntry *pTab = new TabBarEntry(GetContentWidget());
	pTab->SetTabBar(this);
	pTab->SetText(sText);
	pTab->SetVisible(true);
	pTab->SetClosable((m_nOptions & TabOptionCloseTab) != 0 ? true : false);

	// Add tab to list
	uint32 nIndex = m_lstTabs.GetNumOfElements();
	pTab->SetIndex(nIndex);
	m_lstTabs.Add(pTab);

	// Update selection
	if (m_nSelection < 0) {
		SetSelection(nIndex);
	}

	// After something changed, the widget has to be updated
	AdjustContent();

	// Call virtual function
	OnAddTab(pTab);

	// Return tab
	return pTab;
}

/**
*  @brief
*    Add tab
*/
TabBarEntry *TabBar::AddTab(const String &sText, const Image &cIcon)
{
	// Add tab
	TabBarEntry *pTab = AddTab(sText);
	if (pTab) {
		// Set icon
		pTab->SetIcon(cIcon);
	}

	// Return tab
	return pTab;
}

/**
*  @brief
*    Remove tab
*/
bool TabBar::RemoveTab(TabBarEntry *pTab)
{
	// Check tab
	if (pTab && m_lstTabs.IsElement(pTab)) {
		// Get currently selected tab
		int			 nSelectionIndex = pTab->GetIndex();
		TabBarEntry *pSelection		 = (m_nSelection != nSelectionIndex) ? m_lstTabs.Get(m_nSelection) : nullptr;

		// Call virtual function
		OnRemoveTab(pTab);

		// Remove tab from list
		m_lstTabs.Remove(pTab);

		// Destroy tab
		pTab->Destroy();

		// Renumber tabs
		for (uint32 i=0; i<m_lstTabs.GetNumOfElements(); i++) {
			m_lstTabs[i]->SetIndex(i);
		}

		// Update selection
		if (pSelection) {
			// Select previously selected tab
			SetSelection(pSelection->GetIndex());
		} else {
			// Try to select same index as before (clipped to new size)
			if (nSelectionIndex >= (int)m_lstTabs.GetNumOfElements())
				nSelectionIndex = (int)m_lstTabs.GetNumOfElements() - 1;
			SetSelection(nSelectionIndex);
		}

		// Update scrolling position
		if (m_nScrollingPos >= (int)m_lstTabs.GetNumOfElements()) {
			m_nScrollingPos = m_lstTabs.GetNumOfElements() - 1;
			if (m_nScrollingPos < 0) m_nScrollingPos = 0;
		}

		// Reset mouse selection
		m_nMouseSelection = -1;

		// After something changed, the widget has to be updated
		AdjustContent();

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Remove tab
*/
bool TabBar::RemoveTab(uint32 nTab)
{
	// Check tab
	TabBarEntry *pTab = m_lstTabs.Get(nTab);
	if (pTab) {
		// Remove tab
		return RemoveTab(pTab);
	}

	// Error!
	return false;
}

/**
*  @brief
*    Get currently selected tab
*/
int TabBar::GetSelection() const
{
	// Return current selection
	return m_nSelection;
}

/**
*  @brief
*    Set currently selected tab
*/
void TabBar::SetSelection(int nTab)
{
	// Check parameters
	if (nTab >= 0 && nTab < (int)m_lstTabs.GetNumOfElements() && m_nSelection != nTab) {
		// Get old selection
		int nOldSelection = m_nSelection;

		// Set new selection
		m_nSelection = nTab;

		// Call virtual function
		OnSelectTab(m_nSelection);

		// Redraw tabs
		RedrawTab(nOldSelection);
		RedrawTab(m_nSelection);
	}
}

/**
*  @brief
*    Get scrolling position
*/
int TabBar::GetScrollingPos() const
{
	// Return scrolling position
	return m_nScrollingPos;
}

/**
*  @brief
*    Set scrolling position
*/
void TabBar::SetScrollingPos(int nTab)
{
	// Check parameters
	if (nTab >= 0 && nTab < (int)m_lstTabs.GetNumOfElements()) {
		// Set scrolling position
		m_nScrollingPos = nTab;

		// Update selection if necessary
		if (m_nSelection < m_nScrollingPos)
			SetSelection(m_nScrollingPos);

		// After something changed, the widget has to be updated
		AdjustContent();
	}
}

/**
*  @brief
*    Get tab that is currently selected by the mouse
*/
int TabBar::GetMouseSelection() const
{
	// Return mouse-selection
	return m_nMouseSelection;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Redraw tab
*/
void TabBar::RedrawTab(int nTab)
{
	// Check if tab is valid
	if (nTab > -1) {
		// Get tab
		TabBarEntry *pTab = m_lstTabs.Get(nTab);
		if (pTab) {
			// Redraw tab
			pTab->Redraw();
		}
	}
}

/**
*  @brief
*    Set tab that is currently selected by the mouse
*/
void TabBar::SetMouseSelection(int nTab)
{
	// Get old selection
	int nOldSelection = m_nMouseSelection;

	// Set mouse-selection
	m_nMouseSelection = nTab;

	// Redraw tabs
	RedrawTab(nOldSelection);
	RedrawTab(m_nMouseSelection);
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
Vector2i TabBar::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Set preferred size
	Vector2i vSize;

	// Horizontal
	if (m_nSide == SideTop || m_nSide == SideBottom) {
		vSize.x = vRefSize.x;		// Take as much space as possible in X-direction
		vSize.y = m_nTabHeight;		// And set a static size in Y-direction
	}

	// Vertical
	else {
		vSize.x = m_nTabWidth;		// Set a static size in X-direction
		vSize.y = vRefSize.y;		// And take as much space as possible in Y-direction
	}

	// Calculate preferred size of tabs
	for (uint32 i=0; i<m_lstTabs.GetNumOfElements(); i++) {
		m_lstTabs[i]->CalculatePreferredSize();
	}

	// Return preferred size
	return vSize;
}

void TabBar::OnAdjustContent()
{
	// Call base implementation
	Widget::OnAdjustContent();

	// Get options
	bool bHorizontal = (m_nSide == SideTop || m_nSide == SideBottom);
	bool bCreateTab	 = ((m_nOptions & TabOptionCreateTab) != 0);
	bool bScrollPrev = (m_nScrollingPos > 0);
	bool bScrollNext = false;

	// Loop through tabs
	int nLastTab = 0;
	int nPos = 0;
	int nSize = (bHorizontal ? GetSize().x : GetSize().y);
	if (bHorizontal) nSize -= (bCreateTab ? 60 : 40);	// Buttons 'prev', 'next' and 'add' need additional place
	else			 nSize -= 20;						// All buttons fit on one line here
	bool bFirst = true;
	for (uint32 i=0; i<m_lstTabs.GetNumOfElements(); i++) {
		// Get tab
		TabBarEntry *pTab = m_lstTabs[i];

		// Is tab visible?
		if ((int)i >= m_nScrollingPos) {
			int nNextSize = 0;

			// Horizontal
			if (bHorizontal) {
				// Check for dynamic width
				bool bDynamic = !(m_nSide == SideLeft || m_nSide == SideRight || (m_nOptions & TabOptionStaticSize) != 0);

				// Set position
				pTab->SetPos (Vector2i(nPos, 0));

				// Set size
				if (bDynamic) pTab->CalculatePreferredSize();

				int nWidth = (bDynamic ? pTab->GetPreferredSize().x : m_nTabWidth);
				pTab->SetSize(Vector2i(nWidth, m_nTabHeight));

				// Next
				nNextSize = nWidth;
			}

			// Vertical
			else {
				// Set position
				pTab->SetPos (Vector2i(0, nPos));

				// Set size
				pTab->SetSize(Vector2i(m_nTabWidth, m_nTabHeight));

				// Next
				nNextSize = m_nTabHeight;
			}

			// Break if the tab is too long
			if (nPos + nNextSize > nSize && !bFirst) {
				// Item does not fit anymore
				pTab->SetVisible(false);

				// Show scroll button (next)
				bScrollNext = true;
				break;
			}

			// Set tab visible
			pTab->SetVisible(true);
			nLastTab = (int)i;

			// Next
			nPos += nNextSize;
			if (bFirst) bFirst = false;
		} else {
			// Tab is outside the visible range
			pTab->SetVisible(false);
		}
	}

	// Leave a gap
	nPos += 4;

	// Add button 'create'
	if (bCreateTab && m_pButtonCreate) {
		// Set position and size
		if (bHorizontal) m_pButtonCreate->SetPos(Vector2i(nPos, (m_nTabHeight - 16) / 2));
		else			 m_pButtonCreate->SetPos(Vector2i(m_nTabWidth - 60, GetSize().y - 20));
		m_pButtonCreate->SetSize(Vector2i(16, 16));
		m_pButtonCreate->SetVisible(true);

		// Leave a gap
		if (bHorizontal) nPos += 20;
	} else if (m_pButtonCreate) m_pButtonCreate->SetVisible(false);

	// Add button 'scroll (prev)'
	if (m_pButtonPrev) {
		// Set position and size
		if (bHorizontal) m_pButtonPrev->SetPos(Vector2i(GetSize().x - 40, (m_nTabHeight - 16) / 2));
		else			 m_pButtonPrev->SetPos(Vector2i(m_nTabWidth - 40, GetSize().y - 20));
		m_pButtonPrev->SetSize(Vector2i(16, 16));
		m_pButtonPrev->SetEnabled(bScrollPrev);
		m_pButtonPrev->SetVisible(true);
		m_pButtonPrev->SetZPos(ZAbove);

		// Leave a gap
		nPos += 20;
	} else if (m_pButtonPrev) m_pButtonPrev->SetVisible(false);

	// Add button 'scroll (next)'
	if (m_pButtonNext) {
		// Set position and size
		if (bHorizontal) m_pButtonNext->SetPos(Vector2i(GetSize().x - 20, (m_nTabHeight - 16) / 2));
		else			 m_pButtonNext->SetPos(Vector2i(m_nTabWidth - 20, GetSize().y - 20));
		m_pButtonNext->SetSize(Vector2i(16, 16));
		m_pButtonNext->SetEnabled(bScrollNext);
		m_pButtonNext->SetVisible(true);
		m_pButtonNext->SetZPos(ZAbove);
	} else if (m_pButtonNext) m_pButtonNext->SetVisible(false);

	// Check if selection has scrolled outside the visible range
	if (m_nSelection > nLastTab) {
		SetSelection(nLastTab);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual TabBar functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a new tab is added
*/
void TabBar::OnAddTab(TabBarEntry *pTab)
{
	// Emit event
	EventAddTab.Emit(pTab);
}

/**
*  @brief
*    Called when a tab is removed
*/
void TabBar::OnRemoveTab(TabBarEntry *pTab)
{
	// Emit event
	EventRemoveTab.Emit(pTab);
}

/**
*  @brief
*    Called when a tab is to be closed
*/
void TabBar::OnCloseTab(TabBarEntry *pTab)
{
	// Emit event
	EventCloseTab.Emit(pTab);
}

/*
*  @brief
*    Called when the current selection has been changed
*/
void TabBar::OnSelectTab(int nTab)
{
	// Emit event
	EventSelectTab.Emit(nTab);
}

/**
*  @brief
*    Called when a new tab shall be created
*/
void TabBar::OnCreateTab()
{
	// Emit event
	EventCreateTab.Emit();
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
