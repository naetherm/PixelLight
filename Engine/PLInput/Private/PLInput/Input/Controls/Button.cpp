/*********************************************************\
 *  File: Button.cpp                                     *
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
#include "PLInput/Input/Controls/Button.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Button)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Button::Button(Controller *pController, const String &sName, const String &sDescription, char nCharacter) : Control(pController, ControlButton, sName, sDescription),
	m_nCharacter(nCharacter),
	m_bPressed(false),
	m_bHit(false)
{
}

/**
*  @brief
*    Copy constructor
*/
Button::Button(const Button &cOther) : Control(cOther.GetController(), ControlButton, cOther.GetName(), cOther.GetDescription()),
	m_nCharacter(cOther.m_nCharacter),
	m_bPressed(cOther.m_bPressed),
	m_bHit(cOther.m_bHit)
{
}


/**
*  @brief
*    Destructor
*/
Button::~Button()
{
}

/**
*  @brief
*    Comparison operator
*/
bool Button::operator ==(const Button &cOther) const
{
	// Compare values
	return (m_nCharacter == cOther.m_nCharacter && m_bPressed == cOther.m_bPressed && m_bHit == cOther.m_bHit);
}

/**
*  @brief
*    Copy operator
*/
Button &Button::operator =(const Button &cOther)
{
	// Copy value
	m_nCharacter = cOther.m_nCharacter;
	m_bPressed	 = cOther.m_bPressed;
	m_bHit		 = cOther.m_bHit;

	// Control has changed
	InformUpdate();

	// Return reference to this button
	return *this;
}

/**
*  @brief
*    Get character
*/
char Button::GetCharacter() const
{
	// Return character
	return m_nCharacter;
}

/**
*  @brief
*    Get button status
*/
bool Button::IsPressed() const
{
	// Return state
	return m_bPressed;
}

/**
*  @brief
*    Set button status
*/
void Button::SetPressed(bool bPressed)
{
	// If the button was previously pressed but now isn't, we received a hit
	m_bHit = (m_bPressed && !bPressed);

	// Set state
	m_bPressed = bPressed;

	// Control has changed
	InformUpdate();
}

/**
*  @brief
*    Check if the button has been hit
*/
bool Button::IsHit()
{
	// Return hit-state
	const bool bHit = m_bHit;
	m_bHit = false;
	return bHit;
}

/**
*  @brief
*    Check if the button has been hit
*/
bool Button::CheckHit()
{
	// Return hit-state
	return m_bHit;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
