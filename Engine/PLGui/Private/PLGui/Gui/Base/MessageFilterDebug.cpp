/*********************************************************\
 *  File: MessageFilterDebug.cpp                         *
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
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Base/MessageFilterDebug.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MessageFilterDebug::MessageFilterDebug(EDebugMode nDebugMode) :
	m_nDebugMode(nDebugMode)
{
}

/**
*  @brief
*    Destructor
*/
MessageFilterDebug::~MessageFilterDebug()
{
}

/**
*  @brief
*    Get debug mode
*/
EDebugMode MessageFilterDebug::GetDebugMode() const
{
	// Return debug mode
	return m_nDebugMode;
}

/**
*  @brief
*    Set debug mode
*/
void MessageFilterDebug::SetDebugMode(EDebugMode nDebugMode)
{
	// Set debug mode
	m_nDebugMode = nDebugMode;
}


//[-------------------------------------------------------]
//[ Protected virtual MessageFilter functions             ]
//[-------------------------------------------------------]
void MessageFilterDebug::OnGuiMessage(const GuiMessage &cMessage)
{
	// Display anything at all?
	if (m_nDebugMode != DebugModeOff) {
		// Check if the message shall be displayed
		EMessageType nType	  = cMessage.GetType();
		bool		 bDisplay = false;
		if (m_nDebugMode == DebugModeAll) {
			// Display all messages
			bDisplay = true;
		} else if (m_nDebugMode == DebugModeFocus) {
			// Display all keyboard-focus related messages
			bDisplay = (nType == MessageOnGetFocus || nType == MessageOnLooseFocus);
		}

		// Display this message?
		if (bDisplay) {
			// Print message
			System::GetInstance()->GetConsole().Print(cMessage.ToString() + '\n');
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
