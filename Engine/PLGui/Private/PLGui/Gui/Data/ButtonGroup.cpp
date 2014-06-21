/*********************************************************\
 *  File: ButtonGroup.cpp                                *
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ButtonGroup::ButtonGroup() :
	m_bExclusive(true),
	m_pButton(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ButtonGroup::~ButtonGroup()
{
}

/**
*  @brief
*    Check if the button activation is exclusive
*/
bool ButtonGroup::IsExclusive() const
{
	// Return exclusive-flag
	return m_bExclusive;
}

/**
*  @brief
*    Set if the button activation is exclusive
*/
void ButtonGroup::SetExclusive(bool bExclusive)
{
	// Set exclusive-flag
	m_bExclusive = bExclusive;
}

/**
*  @brief
*    Activates a certain button
*/
void ButtonGroup::Activate(AbstractToggleButton &cButton)
{
	// Has the active button changed?
	if (m_pButton != &cButton) {
		// Save active button
		m_pButton = &cButton;

		// Send event that a new button is active
		EventActivate(m_pButton);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
