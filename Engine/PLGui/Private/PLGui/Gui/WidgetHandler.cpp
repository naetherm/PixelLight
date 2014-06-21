/*********************************************************\
 *  File: WidgetHandler.cpp                              *
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
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Gui/WidgetHandler.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


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
WidgetHandler::WidgetHandler() :
	EventHandlerDestroy(&WidgetHandler::OnWidgetDestroy, this),
	m_pWidget(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
WidgetHandler::WidgetHandler(const WidgetHandler &cOther) :
	m_pWidget(nullptr)
{
	// Set widget pointer
	SetWidget(cOther.m_pWidget);
}

/**
*  @brief
*    Destructor
*/
WidgetHandler::~WidgetHandler()
{
	// Reset widget pointer
	Clear();
}

/**
*  @brief
*    Get widget pointer
*/
Widget *WidgetHandler::GetWidget() const
{
	// Return widget pointer
	return m_pWidget;
}

/**
*  @brief
*    Set widget pointer
*/
bool WidgetHandler::SetWidget(Widget *pWidget)
{
	// Clear pointer
	Clear();

	// Set widget pointer
	m_pWidget = pWidget;

	// Register at widget
	if (m_pWidget) {
		// Register destroy-signal
		m_pWidget->SignalDestroy.Connect(EventHandlerDestroy);
	}

	// Error!
	return false;
}

/**
*  @brief
*    Clear widget pointer
*/
void WidgetHandler::Clear()
{
	// Check status
	if (m_pWidget) {
		// Unregister from widget
		m_pWidget->SignalDestroy.Disconnect(EventHandlerDestroy);

		// Reset widget pointer
		m_pWidget = nullptr;
	}
}

/**
*  @brief
*    Check if the widget pointer is valid
*/
bool WidgetHandler::IsValid() const
{
	// Check if pointer is set
	return (m_pWidget != nullptr);
}

/**
*  @brief
*    Compare operator
*/
bool WidgetHandler::operator ==(const WidgetHandler &cOther) const
{
	// Compare pointers
	return (m_pWidget == cOther.m_pWidget);
}

/**
*  @brief
*    Assignment operator
*/
WidgetHandler &WidgetHandler::operator =(const WidgetHandler &cOther)
{
	// Set widget pointer
	SetWidget(cOther.m_pWidget);

	// Return reference to this object
	return *this;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
void WidgetHandler::OnWidgetDestroy()
{
	// Widget has been destroyed, reset pointer
	Clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
