/*********************************************************\
 *  File: LED.cpp                                        *
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
#include "PLInput/Input/Controls/LED.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(LED)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LED::LED(Controller *pController, const String &sName, const String &sDescription) : Control(pController, ControlLED, sName, sDescription),
	m_nLEDs(0)
{
}

/**
*  @brief
*    Copy constructor
*/
LED::LED(const LED &cOther) : Control(cOther.GetController(), ControlLED, cOther.GetName(), cOther.GetDescription()),
	m_nLEDs(cOther.m_nLEDs)
{
}


/**
*  @brief
*    Destructor
*/
LED::~LED()
{
}

/**
*  @brief
*    Comparison operator
*/
bool LED::operator ==(const LED &cOther) const
{
	// Compare values
	return (m_nLEDs == cOther.m_nLEDs);
}

/**
*  @brief
*    Copy operator
*/
LED &LED::operator =(const LED &cOther)
{
	// Copy value
	m_nLEDs = cOther.m_nLEDs;

	// Control has changed
	InformUpdate();

	// Return reference to this LED
	return *this;
}

/**
*  @brief
*    Get state of all LEDs as a bit field
*/
uint32 LED::GetLEDs() const
{
	// Return state of LEDs
	return m_nLEDs;
}

/**
*  @brief
*    Set state of all LEDs as a bit field
*/
void LED::SetLEDs(uint32 nLEDs)
{
	// Set state of LEDs
	m_nLEDs = nLEDs;

	// Control has changed
	InformUpdate();
}

/**
*  @brief
*    Get LED status
*/
bool LED::IsOn(int nLED) const
{
	// Check index
	if (nLED >= 0 && nLED < 32) {
		// Check if LED is on
		return ((m_nLEDs >> nLED) & 1) != 0;
	}

	// Wrong index
	return false;
}

/**
*  @brief
*    Set LED status
*/
void LED::SetOn(int nLED, bool bOn)
{
	// Check index
	if (nLED >= 0 && nLED < 32) {
		// Set LED state
		uint32 nMask = static_cast<uint32>(1) << nLED;
		if (bOn)
			m_nLEDs |= nMask;
		else
			m_nLEDs &= nMask;

		// Control has changed
		InformUpdate();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
