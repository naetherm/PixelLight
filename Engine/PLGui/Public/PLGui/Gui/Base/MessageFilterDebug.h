/*********************************************************\
 *  File: MessageFilterDebug.h                           *
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


#ifndef __PLGUI_MESSAGEFILTERDEBUG_H__
#define __PLGUI_MESSAGEFILTERDEBUG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Gui/Base/MessageFilter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Message filter that dumps all messages to the console
*/
class MessageFilterDebug : public MessageFilter {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nDebugMode
		*    Debug mode
		*/
		PLGUI_API MessageFilterDebug(EDebugMode nDebugMode = DebugModeAll);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~MessageFilterDebug();

		/**
		*  @brief
		*    Get debug mode
		*
		*  @return
		*    Debug mode
		*/
		PLGUI_API EDebugMode GetDebugMode() const;

		/**
		*  @brief
		*    Set debug mode
		*
		*  @param[in] nDebugMode
		*    Debug mode
		*/
		PLGUI_API void SetDebugMode(EDebugMode nDebugMode);


	//[-------------------------------------------------------]
	//[ Protected virtual MessageFilter functions             ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnGuiMessage(const GuiMessage &cMessage) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EDebugMode m_nDebugMode;	/**< Debug mode */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MESSAGEFILTERDEBUG_H__
