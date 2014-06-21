/*********************************************************\
 *  File: WindowContainer.cpp                            *
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
#include "PLGui/Widgets/Containers/WindowContainer.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(WindowContainer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WindowContainer::WindowContainer(Widget *pParent) : Widget(pParent),
	m_pContentWidget(nullptr),
	m_nSelection(-1),
	m_pSelection(nullptr)
{
	// Create content widget
	m_pContentWidget = new Widget(this);
	m_pContentWidget->SetVisible(true);

	// Give focus to child windows
	SetFocusStyle(ChildFocus);
	m_pContentWidget->SetFocusStyle(ChildFocus);
}

/**
*  @brief
*    Destructor
*/
WindowContainer::~WindowContainer()
{
}

/**
*  @brief
*    Get content widget
*/
Widget *WindowContainer::GetContentWidget() const
{
	// Return content widget
	return m_pContentWidget;
}

/**
*  @brief
*    Clear container
*/
void WindowContainer::Clear()
{
	// Delete all windows
	while (m_lstWindows.GetNumOfElements()) {
		// Get window
		Widget *pWindow = m_lstWindows.Get(0);

		// Destroy window
		RemoveWindow(pWindow);
	}

	// Reset selection
	m_nSelection = -1;
	m_pSelection = nullptr;
}

/**
*  @brief
*    Get list of windows inside the container
*/
const Container<Widget*> &WindowContainer::GetWindows() const
{
	// Return window list
	return m_lstWindows;
}

/**
*  @brief
*    Add window to container
*/
void WindowContainer::AddWindow(Widget *pWindow)
{
	// Check parameter
	if (pWindow && !m_lstWindows.IsElement(pWindow)) {
		// Add window
		m_lstWindows.Add(pWindow);

		// Hide window (for now)
		pWindow->SetVisible(false);

		// Call virtual function
		OnAddWindow(pWindow);

		// Update selection
		if (m_nSelection < 0) {
			SetSelection(m_lstWindows.GetNumOfElements()-1);
		}
	}
}

/**
*  @brief
*    Remove window from container
*/
void WindowContainer::RemoveWindow(Widget *pWindow)
{
	// Check parameter
	if (pWindow && m_lstWindows.IsElement(pWindow)) {
		// Save currently selected window
		Widget *pSelection = (m_pSelection != pWindow ? m_pSelection : nullptr);
		int nSelection = m_lstWindows.GetIndex(m_pSelection);

		// Is the currently selected window being removed?
		if (m_pSelection == pWindow) {
			m_pSelection = nullptr;
			m_nSelection = -1;
		}

		// Remove window
		m_lstWindows.Remove(pWindow);

		// Call virtual function
		OnRemoveWindow(pWindow);

		// Delete window
		pWindow->Destroy();

		// Update selection
		if (pSelection) {
			// Update index of selection
			nSelection = m_lstWindows.GetIndex(m_pSelection);
		} else {
			// Try to select same index as before (clipped to new size)
			if (nSelection >= (int)m_lstWindows.GetNumOfElements())
				nSelection = (int)m_lstWindows.GetNumOfElements() - 1;
		}

		// Set new selection
		SetSelection(nSelection);
	}
}

/**
*  @brief
*    Get currently selected window
*/
int WindowContainer::GetSelection() const
{
	// Return selection
	return m_nSelection;
}

/**
*  @brief
*    Set currently selected window
*/
void WindowContainer::SetSelection(int nSelection)
{
	// Set selection
	if (nSelection >= 0 && nSelection < (int)m_lstWindows.GetNumOfElements()) {
		// Set new selection
		m_nSelection = nSelection;

		// Hide old window
		if (m_pSelection) m_pSelection->SetVisible(false);

		// Make new window visible
		m_pSelection = m_lstWindows[m_nSelection];
		if (m_pSelection) {
			m_pSelection->SetZPos(ZAbove);
			m_pSelection->SetVisible(true);
		}

		// Call virtual function
		OnSelectWindow(m_nSelection);
	}
}

/**
*  @brief
*    Set currently selected window
*/
void WindowContainer::SetSelection(Widget *pSelection)
{
	// Get index
	int nIndex = m_lstWindows.GetIndex(pSelection);

	// Set selection
	SetSelection(nIndex);
}


//[-------------------------------------------------------]
//[ Protected virtual WindowContainer functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a window has been added
*/
void WindowContainer::OnAddWindow(Widget *pWindow)
{
	// Emit event
	EventAddWindow.Emit(pWindow);
}

/**
*  @brief
*    Called when a window has been removed
*/
void WindowContainer::OnRemoveWindow(Widget *pWindow)
{
	// Emit event
	EventRemoveWindow.Emit(pWindow);
}

/**
*  @brief
*    Called when the current selection has been changed
*/
void WindowContainer::OnSelectWindow(int nWindow)
{
	// Emit event
	EventSelectWindow.Emit(nWindow);
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
void WindowContainer::OnAdjustContent()
{
	// Call base implementation
	Widget::OnAdjustContent();

	// Make content widget as big as container widget
	if (m_pContentWidget) {
		m_pContentWidget->SetSize(m_vSize);
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
