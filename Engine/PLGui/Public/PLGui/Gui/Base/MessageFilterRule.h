/*********************************************************\
 *  File: MessageFilterRule.h                            *
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


#ifndef __PLGUI_MESSAGEFILTERRULE_H__
#define __PLGUI_MESSAGEFILTERRULE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class that represents a filter rule
*/
class MessageFilterRule {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API MessageFilterRule();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nFilterType
		*    Filter type (inclusive or exclusive)
		*  @param[in] nMessageType
		*    Message type that is filtered
		*
		*  @remarks
		*    Use this constructor to filter against message type
		*/
		PLGUI_API MessageFilterRule(EMessageFilter nFilterType, EMessageType nMessageType);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nFilterType
		*    Filter type (inclusive or exclusive)
		*  @param[in] sWidget
		*    Name of widget that is filtered
		*
		*  @remarks
		*    Use this constructor to filter against widget name
		*/
		PLGUI_API MessageFilterRule(EMessageFilter nFilterType, const PLCore::String &sWidget);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    Filter rule
		*/
		PLGUI_API MessageFilterRule(const MessageFilterRule &cOther);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~MessageFilterRule();

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cOther
		*    Filter rule
		*
		*  @return
		*    'true' if equal, else 'false'
		*/
		PLGUI_API bool operator ==(const MessageFilterRule &cOther) const;

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cOther
		*    Filter rule
		*
		*  @return
		*    Reference to this object
		*/
		PLGUI_API MessageFilterRule &operator =(const MessageFilterRule &cOther);

		/**
		*  @brief
		*    Get filter type
		*
		*  @return
		*    Filter type
		*/
		PLGUI_API EMessageFilter GetFilterType() const;

		/**
		*  @brief
		*    Get message type
		*
		*  @return
		*    Message type, MessageOnUnknown if not used
		*
		*  @remarks
		*    A filter rule can filter against message types or widget names.
		*    This will return MessageOnUnknown, if the type is a widget-name filter.
		*/
		PLGUI_API EMessageType GetMessageType() const;

		/**
		*  @brief
		*    Get widget name
		*
		*  @return
		*    Widget name, "" if not used
		*
		*  @remarks
		*    A filter rule can filter against message types or widget names.
		*    This will return "", if the type is a message-type filter.
		*/
		PLGUI_API PLCore::String GetWidgetName() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EMessageFilter	m_nFilterType;	/**< Filter type (inclusive or exclusive) */
		EMessageType	m_nMessageType;	/**< Message type that is filtered (MessageOnUnknown if not used) */
		PLCore::String	m_sWidget;		/**< Widget name that is filtered  ("" if not used) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MESSAGEFILTERRULE_H__
