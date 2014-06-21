/*********************************************************\
 *  File: Control.cpp                                    *
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
#include "PLInput/Input/InputManager.h"
#include "PLInput/Input/Controller.h"
#include "PLInput/Input/Controls/Control.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Control)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Control::Control(Controller *pController, EControlType nType, const String &sName, const String &sDescription) :
	m_pController(pController),
	m_nType(nType),
	m_sName(sName),
	m_sDescription(sDescription)
{
	// Check that controller is valid
	if (pController) {
		// Add control to controller
		pController->AddControl(this);
	}
}

/**
*  @brief
*    Destructor
*/
Control::~Control()
{
	// Inform input manager
	InputManager::GetInstance()->RemoveControl(this);
}

/**
*  @brief
*    Get controller
*/
Controller *Control::GetController() const
{
	// Return controller
	return m_pController;
}

/**
*  @brief
*    Get control type
*/
EControlType Control::GetType() const
{
	// Return type of control
	return m_nType;
}

/**
*  @brief
*    Check if control is input or output control
*/
bool Control::IsInputControl() const
{
	// Input controls are:  Axis and Button
	// Output controls are: Effect and LED
	return (m_nType == ControlAxis || m_nType == ControlButton);
}

/**
*  @brief
*    Get control name
*/
String Control::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Get control description
*/
String Control::GetDescription() const
{
	// Return description
	return m_sDescription;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Inform input manager that control has been changed
*/
void Control::InformUpdate()
{
	// Inform input manager
	InputManager::GetInstance()->UpdateControl(this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
