/*********************************************************\
 *  File: AbstractToggleButton.cpp                       *
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
#include "PLGui/Gui/Data/ButtonGroup.h"
#include "PLGui/Widgets/Base/AbstractToggleButton.h"


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
pl_implement_class(AbstractToggleButton)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AbstractToggleButton::AbstractToggleButton(Widget *pParent) : AbstractButton(pParent),
	PartiallyAllowed(this),
	State(this),
	SlotOnGroupActivate(this),
	m_pGroup(nullptr),
	m_bPartiallyAllowed(false),
	m_nState(NotChecked)
{
}

/**
*  @brief
*    Destructor
*/
AbstractToggleButton::~AbstractToggleButton()
{
}

/**
*  @brief
*    Get group that the radio button belongs to
*/
ButtonGroup *AbstractToggleButton::GetGroup() const
{
	// Return group
	return m_pGroup;
}

/**
*  @brief
*    Set group that the radio button belongs to
*/
void AbstractToggleButton::SetGroup(ButtonGroup *pGroup)
{
	// Remove event handler from old group
	if (m_pGroup) {
		m_pGroup->EventActivate.Disconnect(SlotOnGroupActivate);
	}

	// Set group
	m_pGroup = pGroup;

	// Connect event handler to new group
	if (m_pGroup) {
		m_pGroup->EventActivate.Connect(SlotOnGroupActivate);
	}
}

/**
*  @brief
*    Check if the toggle button can be checked partially (third state)
*/
bool AbstractToggleButton::GetPartiallyAllowed() const
{
	// Return flag
	return m_bPartiallyAllowed;
}

/**
*  @brief
*    Set if the toggle button can be checked partially (third state)
*/
void AbstractToggleButton::SetPartiallyAllowed(bool bPartiallyAllowed)
{
	// Set flag
	m_bPartiallyAllowed = bPartiallyAllowed;
}

/**
*  @brief
*    Get if the button is checked
*/
ECheckState AbstractToggleButton::GetState() const
{
	// Return checked-state
	return m_nState;
}

/**
*  @brief
*    Set if the button is checked
*/
void AbstractToggleButton::SetState(ECheckState nState)
{
	// New checked-state?
	if (m_nState != nState && (m_bPartiallyAllowed || nState != PartiallyChecked)) {
		// Set checked-state
		m_nState = nState;

		// Call virtual function
		OnButtonToggled(m_nState);

		// Inform group
		if (m_pGroup && m_nState == Checked) {
			m_pGroup->Activate(*this);
		}

		// Redraw, please
		Redraw();
	}
}

/**
*  @brief
*    Toggle button
*/
void AbstractToggleButton::Toggle()
{
	// Toggle state
	if (m_nState == NotChecked || m_nState == PartiallyChecked)
		SetState(Checked);
	else if (!m_pGroup || !m_pGroup->IsExclusive())
		SetState(NotChecked);
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractToggleButton functions      ]
//[-------------------------------------------------------]
void AbstractToggleButton::OnButtonToggled(ECheckState nChecked)
{
	// Send toggled-signal
	SignalToggled(nChecked);
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractButton functions            ]
//[-------------------------------------------------------]
void AbstractToggleButton::OnButtonClicked()
{
	// Call base implementation
	AbstractButton::OnButtonClicked();

	// Toggle button
	Toggle();
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    This function is called when a button of the group is activated
*/
void AbstractToggleButton::OnGroupActivate(AbstractToggleButton *pButton)
{
	if (m_pGroup) {
		if (m_pGroup->IsExclusive() && pButton != this) {
			// [TODO] This could be a problem (infinite loop), but let's try
			SetState(NotChecked);
//			m_nChecked = NotChecked;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
