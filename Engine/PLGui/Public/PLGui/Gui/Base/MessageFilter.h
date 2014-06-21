/*********************************************************\
 *  File: MessageFilter.h                                *
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


#ifndef __PLGUI_MESSAGEFILTER_H__
#define __PLGUI_MESSAGEFILTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include "PLGui/Gui/Base/MessageFilterRule.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class String;
}
namespace PLGui {
	class Gui;
	class GuiMessage;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class to retrieve and filter GUI messages
*/
class MessageFilter {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Gui;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API MessageFilter();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~MessageFilter();

		/**
		*  @brief
		*    Add filter rule
		*
		*  @param[in] nFilterType
		*    Filter type (inclusive or exclusive)
		*  @param[in] nMessageType
		*    Message type that is filtered
		*
		*  @remarks
		*    Use this constructor to filter against a message type
		*/
		PLGUI_API void AddRule(EMessageFilter nFilterType, EMessageType nMessageType);

		/**
		*  @brief
		*    Add filter rule
		*
		*  @param[in] nFilterType
		*    Filter type (inclusive or exclusive)
		*  @param[in] sWidget
		*    Name of widget that is filtered
		*
		*  @remarks
		*    Use this constructor to filter against a widget name
		*/
		PLGUI_API void AddRule(EMessageFilter nFilterType, const PLCore::String &sWidget);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Add GUI message
		*
		*  @param[in] cMessage
		*    GUI message
		*/
		PLGUI_API void AddMessage(const GuiMessage &cMessage);


	//[-------------------------------------------------------]
	//[ Protected virtual MessageFilter functions             ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a GUI message has arrived
		*
		*  @param[in] cMessage
		*    GUI message
		*/
		PLGUI_API virtual void OnGuiMessage(const GuiMessage &cMessage);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::Array<MessageFilterRule> m_lstRules;	/**< Filter rule list */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MESSAGEFILTER_H__
