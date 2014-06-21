/*********************************************************\
 *  File: TabContainer.cpp                               *
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
#include "PLGui/Widgets/Containers/TabContainer.h"
#include "PLGui/Widgets/Controls/TabBar.h"


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
pl_implement_class(TabContainer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TabContainer::TabContainer(Widget *pParent) : WindowContainer(pParent),
	EventHandlerCloseTab (&TabContainer::OnTabBarCloseTab,  this),
	EventHandlerSelectTab(&TabContainer::OnTabBarSelectTab, this),
	EventHandlerCreateTab(&TabContainer::OnTabBarCreateTab, this),
	m_pTabBar(nullptr)
{
	// Create tab-bar
	m_pTabBar = new TabBar(this);
	m_pTabBar->EventCloseTab .Connect(&EventHandlerCloseTab);
	m_pTabBar->EventSelectTab.Connect(&EventHandlerSelectTab);
	m_pTabBar->EventCreateTab.Connect(&EventHandlerCreateTab);
	m_pTabBar->SetVisible(true);
}

/**
*  @brief
*    Destructor
*/
TabContainer::~TabContainer()
{
}

/**
*  @brief
*    Get side the tabbar is positioned on
*/
ESide TabContainer::GetSide() const
{
	// Get side
	return m_pTabBar->GetSide();
}

/**
*  @brief
*    Set side the tabbar is positioned on
*/
void TabContainer::SetSide(ESide nSide)
{
	// Set side
	m_pTabBar->SetSide(nSide);
}

/**
*  @brief
*    Get TabBar options
*/
uint32 TabContainer::GetOptions() const
{
	// Get options
	return m_pTabBar->GetOptions();
}

/**
*  @brief
*    Set TabBar options
*/
void TabContainer::SetOptions(uint32 nOptions)
{
	// Set options
	m_pTabBar->SetOptions(nOptions);
}

/**
*  @brief
*    Get tab width
*/
uint32 TabContainer::GetTabWidth() const
{
	// Get tab width
	return m_pTabBar->GetTabWidth();
}

/**
*  @brief
*    Set tab width
*/
void TabContainer::SetTabWidth(uint32 nWidth)
{
	// Set tab width
	m_pTabBar->SetTabWidth(nWidth);
}

/**
*  @brief
*    Get tab height
*/
uint32 TabContainer::GetTabHeight() const
{
	// Get tab height
	return m_pTabBar->GetTabHeight();
}

/**
*  @brief
*    Set tab height
*/
void TabContainer::SetTabHeight(uint32 nHeight)
{
	// Set tab height
	m_pTabBar->SetTabHeight(nHeight);
}


//[-------------------------------------------------------]
//[ Protected virtual TabContainer functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a new window shall be created
*/
void TabContainer::OnCreateWindow()
{
	// Emit event
	EventCreateWindow.Emit();
}


//[-------------------------------------------------------]
//[ Protected virtual WindowContainer functions           ]
//[-------------------------------------------------------]
void TabContainer::OnAddWindow(Widget *pWindow)
{
	// Call base implementation
	WindowContainer::OnAddWindow(pWindow);

	// Get window title
	String sTitle = pWindow->GetTitle();
	if (!sTitle.GetLength()) sTitle = "Unknown";

	// Get window icon
	Image cIcon = pWindow->GetIcon();

	// Add tab to tab bar
	TabBarEntry *pTab = m_pTabBar->AddTab(sTitle, cIcon);
	pTab->SetWidget(pWindow);

	// Update
	AdjustContent();
}

void TabContainer::OnRemoveWindow(Widget *pWindow)
{
	// Call base implementation
	WindowContainer::OnRemoveWindow(pWindow);

	// Remove tab from tab bar
	TabBarEntry *pTab = FindTab(pWindow);
	if (pTab) {
		m_pTabBar->RemoveTab(pTab);
	}

	// Update
	AdjustContent();
}

void TabContainer::OnSelectWindow(int nWindow)
{
	// Call base implementation
	WindowContainer::OnSelectWindow(nWindow);

	// Update tab bar
	m_pTabBar->SetSelection(nWindow);
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
void TabContainer::OnAdjustContent()
{
	// Call base implementation
	WindowContainer::OnAdjustContent();

	// Position TabBar and content widget
	if (m_pTabBar && m_pContentWidget) {
		// [TODO] Is this correct here? Maybe better implement OnPreferredSize and call from sub-widgets then
		m_pTabBar->CalculatePreferredSize();

		// Set TabBar size
		Vector2i vTabSize = m_pTabBar->GetPreferredSize();
		if (vTabSize.x == -1) vTabSize.x = m_vSize.x;
		if (vTabSize.y == -1) vTabSize.y = m_vSize.y;

		// Position TabBar and container widget
		ESide nSide = m_pTabBar->GetSide();
		if (nSide == SideLeft) {
			m_pTabBar->SetPos(Vector2i::Zero);
			m_pTabBar->SetSize(vTabSize);
			m_pContentWidget->SetPos(Vector2i(vTabSize.x, 0));
			m_pContentWidget->SetSize(Vector2i(m_vSize.x - vTabSize.x, m_vSize.y));
		} else if (nSide == SideTop) {
			m_pTabBar->SetPos(Vector2i::Zero);
			m_pTabBar->SetSize(vTabSize);
			m_pContentWidget->SetPos(Vector2i(0, vTabSize.y));
			m_pContentWidget->SetSize(Vector2i(m_vSize.x, m_vSize.y - vTabSize.y));
		} else if (nSide == SideRight) {
			m_pTabBar->SetPos(Vector2i(m_vSize.x - vTabSize.x, 0));
			m_pTabBar->SetSize(vTabSize);
			m_pContentWidget->SetPos(Vector2i(0, 0));
			m_pContentWidget->SetSize(Vector2i(m_vSize.x - vTabSize.x, m_vSize.y));
		} else if (nSide == SideBottom) {
			m_pTabBar->SetPos(Vector2i(0, m_vSize.y - vTabSize.y));
			m_pTabBar->SetSize(vTabSize);
			m_pContentWidget->SetPos(Vector2i(0, 0));
			m_pContentWidget->SetSize(Vector2i(m_vSize.x, m_vSize.y - vTabSize.y));
		}

		// Make all windows the same size as the content widget
		for (uint32 i=0; i<m_lstWindows.GetNumOfElements(); i++) {
			Widget *pWindow = m_lstWindows[i];
			pWindow->SetPos(Vector2i::Zero);
			pWindow->SetSize(m_pContentWidget->GetSize());
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a tab is closed
*/
void TabContainer::OnTabBarCloseTab(TabBarEntry *pTab)
{
	// Remove window
	Widget *pWindow = pTab->GetWidget();
	if (pWindow) {
		RemoveWindow(pWindow);
	}
}

/**
*  @brief
*    Called when the current selection has been changed
*/
void TabContainer::OnTabBarSelectTab(int nTab)
{
	// Set new selection
	SetSelection(nTab);
}

/**
*  @brief
*    Called when a new tab shall be created
*/
void TabContainer::OnTabBarCreateTab()
{
	// Create a new window
	OnCreateWindow();
}

/**
*  @brief
*    Find tab that belongs to a certain window
*/
TabBarEntry *TabContainer::FindTab(Widget *pWindow) const
{
	// Loop through tabs
	const Container<TabBarEntry*> &lstTabs = m_pTabBar->GetTabs();
	for (uint32 i=0; i<lstTabs.GetNumOfElements(); i++) {
		// Get tab
		TabBarEntry *pTab = lstTabs[i];
		if (pTab->GetWidget() == pWindow) {
			// Tab found
			return pTab;
		}
	}

	// Tab could not be found
	return nullptr;
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
