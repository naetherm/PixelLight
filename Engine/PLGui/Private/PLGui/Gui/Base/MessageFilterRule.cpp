/*********************************************************\
 *  File: MessageFilterRule.cpp                          *
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
#include "PLGui/Gui/Base/MessageFilterRule.h"


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
MessageFilterRule::MessageFilterRule() :
	m_nFilterType(FilterInclusive),
	m_nMessageType(MessageOnUnknown),
	m_sWidget("")
{
}

/**
*  @brief
*    Constructor
*/
MessageFilterRule::MessageFilterRule(EMessageFilter nFilterType, EMessageType nMessageType) :
	m_nFilterType(nFilterType),
	m_nMessageType(nMessageType),
	m_sWidget("")
{
}

/**
*  @brief
*    Constructor
*/
MessageFilterRule::MessageFilterRule(EMessageFilter nFilterType, const String &sWidget) :
	m_nFilterType(nFilterType),
	m_nMessageType(MessageOnUnknown),
	m_sWidget(sWidget)
{
}

/**
*  @brief
*    Copy constructor
*/
MessageFilterRule::MessageFilterRule(const MessageFilterRule &cOther) :
	m_nFilterType(cOther.m_nFilterType),
	m_nMessageType(cOther.m_nMessageType),
	m_sWidget(cOther.m_sWidget)
{
}

/**
*  @brief
*    Destructor
*/
MessageFilterRule::~MessageFilterRule()
{
}

/**
*  @brief
*    Comparison operator
*/
bool MessageFilterRule::operator ==(const MessageFilterRule &cOther) const
{
	// Compare filter rule
	return (cOther.m_nFilterType == m_nFilterType && cOther.m_nMessageType == m_nMessageType && cOther.m_sWidget == m_sWidget);
}

/**
*  @brief
*    Assignment operator
*/
MessageFilterRule &MessageFilterRule::operator =(const MessageFilterRule &cOther)
{
	// Copy filter rule
	m_nFilterType	= cOther.m_nFilterType;
	m_nMessageType	= cOther.m_nMessageType;
	m_sWidget		= cOther.m_sWidget;
	return *this;
}

/**
*  @brief
*    Get filter type
*/
EMessageFilter MessageFilterRule::GetFilterType() const
{
	// Return filter type
	return m_nFilterType;
}

/**
*  @brief
*    Get message type
*/
EMessageType MessageFilterRule::GetMessageType() const
{
	// Return message type
	return m_nMessageType;
}

/**
*  @brief
*    Get widget name
*/
String MessageFilterRule::GetWidgetName() const
{
	// Return widget name
	return m_sWidget;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
