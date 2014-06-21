/*********************************************************\
 *  File: TabBarButton.cpp                               *
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
pl_implement_class(TabBarButton)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TabBarButton::TabBarButton(Widget *pParent) : AbstractButton(pParent),
	m_pTabBar(nullptr),
	m_pTabBarEntry(nullptr),
	m_nFunction(TabFunctionCreateTab)
{
}

/**
*  @brief
*    Destructor
*/
TabBarButton::~TabBarButton()
{
}

/**
*  @brief
*    Get TabBar
*/
TabBar *TabBarButton::GetTabBar() const
{
	// Return TabBar
	return m_pTabBar;
}

/**
*  @brief
*    Get TabBarEntry
*/
TabBarEntry *TabBarButton::GetTabBarEntry() const
{
	// Return TabBarEntry
	return m_pTabBarEntry;
}

/**
*  @brief
*    Get tab-bar function
*/
ETabBarFunction TabBarButton::GetFunction() const
{
	// Return function
	return m_nFunction;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set TabBar
*/
void TabBarButton::SetTabBar(TabBar *pTabBar)
{
	// Set TabBar
	m_pTabBar = pTabBar;
}

/**
*  @brief
*    Set TabBarEntry
*/
void TabBarButton::SetTabBarEntry(TabBarEntry *pEntry)
{
	// Set TabBarEntry
	m_pTabBarEntry = pEntry;
}

/**
*  @brief
*    Set tab-bar function
*/
void TabBarButton::SetFunction(ETabBarFunction nFunction)
{
	// Set function
	m_nFunction = nFunction;
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
void TabBarButton::OnMouseEnter()
{
	// Call base function
	AbstractButton::OnMouseEnter();

	// Update mouseover-index
	if (m_pTabBarEntry) {
		m_pTabBar->SetMouseSelection(m_pTabBarEntry->GetIndex());
	}

	// Redraw button
	Redraw();
}

void TabBarButton::OnMouseLeave()
{
	// Call base function
	AbstractButton::OnMouseLeave();

	// Redraw button
	Redraw();
}


//[-------------------------------------------------------]
//[ Private virtual AbstractButton functions              ]
//[-------------------------------------------------------]
void TabBarButton::OnButtonClicked()
{
	// Call base implementation
	AbstractButton::OnButtonClicked();

	// Execute function
	if (m_nFunction == TabFunctionCreateTab) {
		// Create tab
		if (m_pTabBar) {
			m_pTabBar->CreateTab();
		}
	} else if (m_nFunction == TabFunctionCloseTab) {
		// Close tab
		if (m_pTabBar && m_pTabBarEntry) {
			m_pTabBar->CloseTab(m_pTabBarEntry->GetIndex());
		}
	} else if (m_nFunction == TabFunctionScrollNext) {
		// Scroll (next)
		if (m_pTabBar) {
			m_pTabBar->SetScrollingPos(m_pTabBar->GetScrollingPos() + 1);
		}
	} else if (m_nFunction == TabFunctionScrollPrev) {
		// Scroll (prev)
		if (m_pTabBar) {
			m_pTabBar->SetScrollingPos(m_pTabBar->GetScrollingPos() - 1);
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
