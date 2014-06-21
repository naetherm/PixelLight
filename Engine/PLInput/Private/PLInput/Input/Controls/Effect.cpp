/*********************************************************\
 *  File: Effect.cpp                                     *
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
#include "PLInput/Input/Controls/Effect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Effect)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Effect::Effect(Controller *pController, const String &sName, const String &sDescription) : Control(pController, ControlEffect, sName, sDescription),
	m_fValue(0.0f)
{
}

/**
*  @brief
*    Copy constructor
*/
Effect::Effect(const Effect &cOther) : Control(cOther.GetController(), ControlEffect, cOther.GetName(), cOther.GetDescription()),
	m_fValue(cOther.m_fValue)
{
}


/**
*  @brief
*    Destructor
*/
Effect::~Effect()
{
}

/**
*  @brief
*    Comparison operator
*/
bool Effect::operator ==(const Effect &cOther) const
{
	// Compare values
	return (m_fValue == cOther.m_fValue);
}

/**
*  @brief
*    Copy operator
*/
Effect &Effect::operator =(const Effect &cOther)
{
	// Copy value
	m_fValue = cOther.m_fValue;

	// Control has changed
	InformUpdate();

	// Return reference to this effect
	return *this;
}

/**
*  @brief
*    Get effect value
*/
float Effect::GetValue() const
{
	// Return value
	return m_fValue;
}

/**
*  @brief
*    Set effect value
*/
void Effect::SetValue(float fValue)
{
	// Set value
	m_fValue = fValue;

	// Control has changed
	InformUpdate();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
