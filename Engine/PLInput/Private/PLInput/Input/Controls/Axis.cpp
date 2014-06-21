/*********************************************************\
 *  File: Axis.cpp                                       *
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
#include "PLInput/Input/Controls/Axis.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Axis)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Axis::Axis(Controller *pController, const String &sName, const String &sDescription) : Control(pController, ControlAxis, sName, sDescription),
	m_fValue(0.0f),
	m_bValueRelative(false)
{
}

/**
*  @brief
*    Copy constructor
*/
Axis::Axis(const Axis &cOther) : Control(cOther.GetController(), ControlAxis, cOther.GetName(), cOther.GetDescription()),
	m_fValue(cOther.m_fValue),
	m_bValueRelative(cOther.m_bValueRelative)
{
}

/**
*  @brief
*    Destructor
*/
Axis::~Axis()
{
}

/**
*  @brief
*    Comparison operator
*/
bool Axis::operator ==(const Axis &cOther) const
{
	// Compare values
	return (m_fValue == cOther.m_fValue && m_bValueRelative == cOther.m_bValueRelative);
}

/**
*  @brief
*    Copy operator
*/
Axis &Axis::operator =(const Axis &cOther)
{
	// Copy value
	m_fValue		 = cOther.m_fValue;
	m_bValueRelative = cOther.m_bValueRelative;

	// Control has changed
	InformUpdate();

	// Return reference to this axis
	return *this;
}

/**
*  @brief
*    Get axis value
*/
float Axis::GetValue() const
{
	// Return value
	return m_fValue;
}

/**
*  @brief
*    Set axis value
*/
void Axis::SetValue(float fValue, bool bValueRelative)
{
	// Set value
	m_fValue		 = fValue;
	m_bValueRelative = bValueRelative;

	// Control has changed
	InformUpdate();
}

/**
*  @brief
*    Return whether the current value is relative or absolute
*/
bool Axis::IsValueRelative() const
{
	return m_bValueRelative;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
