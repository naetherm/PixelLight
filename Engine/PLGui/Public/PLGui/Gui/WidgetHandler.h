/*********************************************************\
 *  File: WidgetHandler.h                                *
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


#ifndef __PLGUI_WIDGETHANDLER_H__
#define __PLGUI_WIDGETHANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Widget handler class
*
*  @remarks
*    A widget handler is a smart pointer to widgets. Use it whenever you want to store a pointer
*    to a widget for a longer time, this way you can be sure that the pointer will be reset to a null pointer
*    if the widget has been destroyed in the meantime.
*/
class WidgetHandler {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API WidgetHandler();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    Widget handler
		*/
		PLGUI_API WidgetHandler(const WidgetHandler &cOther);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~WidgetHandler();

		/**
		*  @brief
		*    Get widget pointer
		*
		*  @return
		*    Pointer to widget, can be a null pointer
		*/
		PLGUI_API Widget *GetWidget() const;

		/**
		*  @brief
		*    Set widget pointer
		*
		*  @param[in] pWidget
		*    Pointer to widget, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLGUI_API bool SetWidget(Widget *pWidget);

		/**
		*  @brief
		*    Clear widget pointer
		*/
		PLGUI_API void Clear();

		/**
		*  @brief
		*    Check if the widget pointer is valid
		*
		*  @return
		*    'true' if the handler points to a valid widget, else 'false'
		*/
		PLGUI_API bool IsValid() const;

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] cOther
		*    Widget handler
		*
		*  @return
		*    'true' if equal, else 'false'
		*/
		PLGUI_API bool operator ==(const WidgetHandler &cOther) const;

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cOther
		*    Widget handler
		*
		*  @return
		*    Reference to this object
		*/
		PLGUI_API WidgetHandler &operator =(const WidgetHandler &cOther);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		void OnWidgetDestroy();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Event handlers
		PLCore::EventHandler<>	EventHandlerDestroy;	/**< Widget is going to be destroyed */

		// Data
		Widget *m_pWidget; /**< Pointer to widget, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WIDGETHANDLER_H__
